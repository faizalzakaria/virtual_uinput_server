/**
   @file   key_table.h
   @brief  Key table


   @author Faizal Zakaria
   @date   2012-07-03
*/

#ifndef __KEY_TABLE__
#define __KEY_TABLE__

typedef struct 
{
	unsigned int input_key;	/* Remote code */
	int shift;			/* 0: rel_pos, 1: key */
	unsigned int output_key;
} KEYBOARD_KEY;

// put it in order of input_key, for fast search purposes. Merge Sort Algo
static const KEYBOARD_KEY ascii_map_table[] = {
	{ 0x00, 0, KEY_ENTER },
	{ 0x08, 0, KEY_BACKSPACE },
	{ 0x09, 0, KEY_TAB },
	{ 0x1b, 0, KEY_POWER/*KEY_ESC*/ },
	{ 0x20, 0, KEY_SPACE },
	{ 0x21, 1, KEY_1 },
	{ 0x22, 1, KEY_COMMA },
	{ 0x23, 1, KEY_3 },
	{ 0x24, 1, KEY_4 },
	{ 0x25, 1, KEY_5 },
	{ 0x26, 1, KEY_6 },
	{ 0x27, 0, KEY_APOSTROPHE },
	{ 0x28, 1, KEY_9 },
	{ 0x29, 1, KEY_0 },
	{ 0x2a, 1, KEY_8 },
	{ 0x2b, 1, KEY_EQUAL },
	{ 0x2c, 0, KEY_COMMA },
	{ 0x2d, 0, KEY_MINUS },
	{ 0x2e, 0, KEY_DOT },
	{ 0x2f, 0, KEY_SLASH },
	{ 0x30, 0, KEY_0 },
	{ 0x31, 0, KEY_1 },
	{ 0x32, 0, KEY_2 },
	{ 0x33, 0, KEY_3 },
	{ 0x34, 0, KEY_4 },
	{ 0x35, 0, KEY_5 },
	{ 0x36, 0, KEY_6 },
	{ 0x37, 0, KEY_7 },
	{ 0x38, 0, KEY_8 },
	{ 0x39, 0, KEY_9 },
	{ 0x3a, 1, KEY_SEMICOLON },
	{ 0x3b, 0, KEY_SEMICOLON },
	{ 0x3c, 1, KEY_COMMA },
	{ 0x3d, 0, KEY_EQUAL },
	{ 0x3e, 1, KEY_DOT },
	{ 0x3f, 1, KEY_QUESTION },
	{ 0x40, 1, KEY_2 },
	{ 0x41, 1, KEY_A },
	{ 0x42, 1, KEY_B },
	{ 0x43, 1, KEY_C },
	{ 0x44, 1, KEY_D },
	{ 0x45, 1, KEY_E },
	{ 0x46, 1, KEY_F },
	{ 0x47, 1, KEY_G },
	{ 0x48, 1, KEY_H },
	{ 0x49, 1, KEY_I },
	{ 0x4a, 1, KEY_J },
	{ 0x4b, 1, KEY_K },
	{ 0x4c, 1, KEY_L },
	{ 0x4d, 1, KEY_M },
	{ 0x4e, 1, KEY_N },
	{ 0x4f, 1, KEY_O },
	{ 0x50, 1, KEY_P },
	{ 0x51, 1, KEY_Q },
	{ 0x52, 1, KEY_R },
	{ 0x53, 1, KEY_S },
	{ 0x54, 1, KEY_T },
	{ 0x55, 1, KEY_U },
	{ 0x56, 1, KEY_V },
	{ 0x57, 1, KEY_W },
	{ 0x58, 1, KEY_X },
	{ 0x59, 1, KEY_Y },
	{ 0x5a, 1, KEY_Z },
	{ 0x5b, 1, KEY_LEFTBRACE },
	{ 0x5c, 0, KEY_BACKSLASH },
	{ 0x5d, 1, KEY_RIGHTBRACE },
	{ 0x5e, 1, KEY_6 },
	{ 0x5f, 1, KEY_MINUS },
	{ 0x60, 0, KEY_GRAVE },
	{ 0x61, 0, KEY_A },
	{ 0x62, 0, KEY_B },
	{ 0x63, 0, KEY_C },
	{ 0x64, 0, KEY_D },
	{ 0x65, 0, KEY_E },
	{ 0x66, 0, KEY_F },
	{ 0x67, 0, KEY_G },
	{ 0x68, 0, KEY_H },
	{ 0x69, 0, KEY_I },
	{ 0x6a, 0, KEY_J },
	{ 0x6b, 0, KEY_K },
	{ 0x6c, 0, KEY_L },
	{ 0x6d, 0, KEY_M },
	{ 0x6e, 0, KEY_N },
	{ 0x6f, 0, KEY_O },
	{ 0x70, 0, KEY_P },
	{ 0x71, 0, KEY_Q },
	{ 0x72, 0, KEY_R },
	{ 0x73, 0, KEY_S },
	{ 0x74, 0, KEY_T },
	{ 0x75, 0, KEY_U },
	{ 0x76, 0, KEY_V },
	{ 0x77, 0, KEY_W },
	{ 0x78, 0, KEY_X },
	{ 0x79, 0, KEY_Y },
	{ 0x7a, 0, KEY_Z },
	{ 0x7b, 0, KEY_LEFTBRACE },
	{ 0x7c, 1, KEY_BACKSLASH },
	{ 0x7d, 0, KEY_RIGHTBRACE },
	{ 0x7e, 1, KEY_GRAVE },
	{ 0x7f, 0, KEY_DELETE }
};

static const KEYBOARD_KEY escape_map_table[] = {
	{ 0x1b, 0, KEY_POWER },
	{ 0x415b1b, 0, KEY_UP },
	{ 0x425b1b, 0, KEY_DOWN },
	{ 0x435b1b, 0, KEY_RIGHT },
	{ 0x445b1b, 0, KEY_LEFT },
	{ 0x504f1b, 0, KEY_BACK },
	{ 0x7e335b1b, 0, KEY_DELETE }
};

#endif
