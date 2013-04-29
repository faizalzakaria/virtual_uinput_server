/**
   @file   main.c
   @brief  Main for remote handler


   @author Faizal Zakaria
   @date   2012-07-03
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>

#include <signal.h>

#include <linux/input.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "key_table.h"
#include "log.h"

#define BUFFER_SIZE 12
#define MAX_CALLBACK 3

typedef int (*KeyCallback)(void *pContext);

struct callback_context {
	unsigned int output_key;
	KeyCallback callback;
	void *callbackContext;
};

struct server_context {
	int *newsockfd;
	int *sockfd;

	int callbackCount;
	struct callback_context callbackList[MAX_CALLBACK];
};

static const int ASCII_TABLE_SIZE = sizeof( ascii_map_table ) / sizeof( KEYBOARD_KEY );
static const int ESCAPE_TABLE_SIZE = sizeof( escape_map_table ) / sizeof( KEYBOARD_KEY );

static const KEYBOARD_KEY *search_key( const KEYBOARD_KEY *key_table, int size, const unsigned int buffer );

static struct server_context gContext;

static int chomp( char *s );
static void terminate( const int sig_no );
static void close_all( void );
static void install_sig_handler(void);

static int filter( const KEYBOARD_KEY *key );

static int register_callback( unsigned int output_key, void *callback, void *context );

static int exit_app_callback( void *pContext );
static unsigned int get_mask( int size );

int main( int argc, char *argv[])
{
	LOGE( "\033[1mE Test server\033[0m\n" );	
	LOGW( "\033[1mW Test server\033[0m\n" );
	LOGI( "\033[1mI Test server\033[0m\n" );
	LOGD( "\033[1mD Test server\033[0m\n" );
	
	int sockfd, newsockfd, portno, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2) {
		LOGE( "ERROR, no port provided\n" );
		exit(1);
	}

	memset( &gContext, 0, sizeof( struct server_context ) );

	install_sig_handler();
	
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if (sockfd < 0) 
		perror("ERROR opening socket");

	bzero( (char *) &serv_addr, sizeof(serv_addr) );
	portno = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if ( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) 
		perror("ERROR on binding");

	if (open_uinput_device() < 0)
	{
		LOGE( "Error to open /dev/input/event0\n" );
		goto exit;
	}

START_LISTENING:

	if ( gContext.newsockfd )
		close( *gContext.newsockfd );

	LOGI( "Listenening to port %d\n", portno );
	listen( sockfd, 1 );
	clilen = sizeof( cli_addr );

	newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
	if (newsockfd < 0) 
		perror("ERROR on accept");
	
	LOGI( "Accepting connection ...\n" );

	gContext.newsockfd = &newsockfd;
	gContext.sockfd = &sockfd;

	const KEYBOARD_KEY *key = NULL;

#if 0
	if ( register_callback( KEY_ESC, exit_app_callback, NULL ) < 0 )
	{
		LOGE( "Error registering callback\n" );
		goto exit;
	}
#endif

	while (1) 
	{
		char buffer[BUFFER_SIZE];
		int size = 0;
		const KEYBOARD_KEY *key = NULL;
		int status = 0;
		memset( &buffer, 0, BUFFER_SIZE );

		if ( recv( newsockfd, buffer, BUFFER_SIZE, 0 ) <= 0 )
			goto START_LISTENING;

		size = chomp( buffer );
		unsigned int mask = get_mask( size );
		LOGD( "MASK : 0x%08x \n", mask );

		LOGD( "message : %s %d\n", buffer, size );

		if ( buffer[0] == 0x1b ) // escape key
		{
			key = search_key( escape_map_table, ESCAPE_TABLE_SIZE, *((unsigned int *) buffer) & mask );
		}
		else if ( !strcmp( buffer, "quit" ) )
		{
			goto START_LISTENING;
		}
		else
		{
			key = search_key( ascii_map_table, ASCII_TABLE_SIZE, *((unsigned int *) buffer) & mask );
		}

		if ( key == NULL )
			continue;

#ifdef DEBUG_DATA
		else
		{
			int k;
			for ( k = 0; k < size; k++ )
			{
				LOGD( "data : 0x%08x 0x%08x buffer size : %d\n", buffer[k], key->output_key, size );
			}
		}
#endif
		
		if ( filter( key ) >= 0 )
			continue;
		
		if ( key->shift )
			send_key_wifh_shift( key->output_key );
		else
			send_key( key->output_key );

	}

exit:
	close_all();
	return 0; /* we never get here */
}

static void close_all( void )
{
	close_uinput_device();
	if ( gContext.newsockfd )
		close( *gContext.newsockfd );
	if ( gContext.sockfd )
		close( *gContext.sockfd );
}

static int chomp( char *s ) 
{
	int i = 0;
	while( *s && *s != '\n' && *s != '\r' && *s != 0xffffffff ) 
	{
		i++;
		s++;
	}

    *s = 0;
    return i;
}

static unsigned int get_mask( int size )
{
	int i;
	unsigned int mask = 0;

	if ( size > sizeof( unsigned int ) )
		return 0xffffffff;

	for( i = 0; i < size; i++ )
	{
		mask |= ( 0xff << ( i * 8 ) );
	}
	
	return mask;
}

static void terminate( const int sig_no )
{
	close_all();
	exit( 1 );
}

/* To install signal handlers */
static void install_sig_handler( void )
{
	signal(SIGBUS, terminate);
	signal(SIGFPE, terminate);
	signal(SIGHUP, terminate);
	signal(SIGILL, terminate);
	signal(SIGINT, terminate);
	signal(SIGIOT, terminate);
	signal(SIGPIPE, terminate);
	signal(SIGQUIT, terminate);
	signal(SIGSEGV, terminate);
	signal(SIGSYS, terminate);
	signal(SIGTERM, terminate);
	signal(SIGTRAP, terminate);
	signal(SIGUSR1, terminate);
	signal(SIGUSR2, terminate);
}

static const KEYBOARD_KEY *search_key( const KEYBOARD_KEY *key_table, int size, const unsigned int buffer )
{
	int end = size - 1;
	int start = 0;
	KEYBOARD_KEY key;
	unsigned int middle_key;

	LOGD( "Search Key : 0x%08x %d\n", buffer, size);

	while (1) {
		int middle = ( end + start ) / 2;
		
		middle_key = key_table[middle].input_key;
		if ( buffer == middle_key )
			return &key_table[middle];

		if ( buffer < middle_key )
			end = middle - 1;
		else
			start = middle + 1;
		
		if ( end < start )
		{
			LOGD( "not found\n");
			return NULL;
		}
		LOGD( "%d %d %d\n", start, middle, end );
	}
}

static int register_callback( unsigned int output_key, void *callback, void *context )
{
	if (gContext.callbackCount >= ( MAX_CALLBACK - 1 ) )
		return -1;

	LOGD( "Registering callback, 0x%08x\n", output_key );
	
	struct callback_context *callback_ctx = &gContext.callbackList[gContext.callbackCount++];
	callback_ctx->output_key = output_key;
	callback_ctx->callback = callback;
	callback_ctx->callbackContext = context;
}

static int exit_app_callback( void *pContext )
{
	send_key( KEY_POWER );
	return 0;
}

static int filter( const KEYBOARD_KEY *key )
{
	int i = 0;
	for ( i = 0; i <= gContext.callbackCount; i++ )
	{
		if ( gContext.callbackList[i].output_key == key->output_key )
		{
			LOGD( "Found callback for key 0x%08x\n", key->output_key );
			if ( gContext.callbackList[i].callback != NULL )
				return gContext.callbackList[i].callback( gContext.callbackList[i].callbackContext );
		}
	}
	return -1;
}
