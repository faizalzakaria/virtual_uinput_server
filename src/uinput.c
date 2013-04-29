/**
   @file   uinput
   @brief  Virtual input


   @author Faizal Zakaria
   @date   2012-07-03
*/

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/input.h>
#include <linux/uinput.h>

#include "log.h"

static struct uinput_user_dev uinp;
static struct input_event event, syn_event;

static const char *uinp_dev = "/dev/uinput";
static int uinp_fno = -1;

int open_uinput_device( void )
{
	int i = 0;
	
	if ( (uinp_fno = open( uinp_dev, O_WRONLY | O_NDELAY )) <= 0 )
		return -1;

	memset( &uinp, 0, sizeof(uinp) );
	strncpy( uinp.name, "Input Simulator", UINPUT_MAX_NAME_SIZE );
	uinp.id.version = 1;
	uinp.id.bustype = BUS_VIRTUAL;
	
	if ( ioctl( uinp_fno, UI_SET_EVBIT, EV_SYN ) < 0 ) {
		perror( "Unable to set evbit \n" );
		return -1;
	}

	ioctl( uinp_fno, UI_SET_EVBIT, EV_KEY );
	ioctl( uinp_fno, UI_SET_EVBIT, EV_REL );
	ioctl( uinp_fno, UI_SET_RELBIT, REL_X );
	ioctl( uinp_fno, UI_SET_RELBIT, REL_Y );

	for (i = 0; i < 65536; i++)
		ioctl( uinp_fno, UI_SET_KEYBIT, i );

	ioctl( uinp_fno, UI_SET_KEYBIT, BTN_MOUSE );
	ioctl( uinp_fno, UI_SET_KEYBIT, BTN_LEFT );
	ioctl( uinp_fno, UI_SET_KEYBIT, BTN_RIGHT );

	write( uinp_fno, &uinp, sizeof(uinp) );

	if ( ioctl( uinp_fno, UI_DEV_CREATE ) ) {
		perror( "Unable to create UINPUT device\n" );
		return -1;
	}

	return 0;
}

int close_uinput_device( void )
{
	if ( uinp_fno >= 0 ) {
		ioctl( uinp_fno, UI_DEV_DESTROY );
		close( uinp_fno );
		uinp_fno = -1;
	}
}

void send_movement( int x, int y )
{
	const int STEPS = 10;
	int i = 0;
	
	for ( i = 0; i < STEPS; i++ )
	{
		memset( &event, 0, sizeof(event) );
		gettimeofday( &event.time, NULL );
		gettimeofday( &syn_event.time, NULL );
		
		/* Send in movement */
		if ( x != 0 ) 
		{
			event.type = EV_REL;
			event.code = REL_X;
			event.value = x;
			write(uinp_fno, &event, sizeof(event));
		}

		if ( y != 0 ) 
		{
			event.type = EV_REL;
			event.code = REL_Y;
			event.value = y;
			write(uinp_fno, &event, sizeof(event));
		}

		if ((x != 0) || (y != 0)) {
			write( uinp_fno, &syn_event, sizeof(syn_event) );
		}
	}
}

void send_key_wifh_shift( unsigned int key )
{
	LOGD( "key : 0x%08x \n", key );
	static struct input_event event;

	memset( &event, 0, sizeof(event) );

	event.type = EV_KEY;
	event.code = KEY_LEFTSHIFT;
	event.value = 1;
	write( uinp_fno, &event, sizeof(event) );

	event.type = EV_KEY;
	event.code = key;
	event.value = 1;
	write( uinp_fno, &event, sizeof(event) );

	event.type = EV_KEY;
	event.code = key;
	event.value = 0;
	write( uinp_fno, &event, sizeof(event) );

	event.type = EV_KEY;
	event.code = KEY_LEFTSHIFT;
	event.value = 0;
	write( uinp_fno, &event, sizeof(event) );

}

void send_key( unsigned int key )
{
	LOGD( "key : 0x%08x \n", key );
	static struct input_event event;

	memset( &event, 0, sizeof(event) );

	event.type = EV_KEY;
	event.code = key;
	event.value = 1;
	write( uinp_fno, &event, sizeof(event) );

	event.type = EV_KEY;
	event.code = key;
	event.value = 0;
	write( uinp_fno, &event, sizeof(event) );

}
