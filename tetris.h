/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <pthread.h>

#define	TETRIS_HEIGHT	20
#define TETRIS_WIDTH	10

#define SCREEN_HEIGHT 	(TETRIS_HEIGHT+2)
#define SCREEN_WIDTH	(TETRIS_WIDTH*2+3)

#define GRID_LEN	4

#define BLOCK_INIT_X		0
#define BLOCK_INIT_Y		0

#define BLOCK_ERROR_TYPE	-1
#define BLOCK_ERROR_NUM		-1

#define BLOCK_INIT_VALUE	'0'


#define TTRS_SUCCESS	0
#define TTRS_FAILED		-1

#define PAINT_SCREEN_WALL		'#'
#define PAINT_BLOCK_UNIT 	'o'

#define POS_NEXT_BLOCK_Y 	1
#define POS_NEXT_BLOCK_X 	(TETRIS_WIDTH+2+3)
#define POS_CURRENT_BLOCK_Y -3
#define POS_CURRENT_BLOCK_X	((TETRIS_WIDTH-GRID_LEN)/2+1)

#define NEXT_BLOCK_HEIGHT	5
#define NEXT_BLOCK_WIDTH	4

#define STR_LEVEL		"Level"
#define LEN_LEVEL_KEY	(sizeof(STR_LEVEL))//no need to add one
#define LEVEL_KEY_VALUE	0
#define LEN_LEVEL_SHOW	(TETRIS_WIDTH*sizeof(char)+1)
#define POS_LEVEL_Y		(GRID_LEN+2)
#define POS_LEVEL_X		(TETRIS_WIDTH+2)

#define STR_LINES		"Lines"
#define LEN_LINES_KEY	(sizeof(STR_LINES))
#define LINES_KEY_VALUE	0
#define LEN_LINES_SHOW  LEN_LEVEL_SHOW
#define POS_LINES_Y		(POS_LEVEL_Y+2)
#define POS_LINES_X		POS_LEVEL_X

#define STR_SCORE		"Score"
#define LEN_SCORE_KEY	(sizeof(STR_SCORE))
#define SCORE_KET_VALUE	0
#define LEN_SCORE_SHOW	LEN_LEVEL_SHOW
#define POS_SCORE_Y		(POS_LINES_Y+2)
#define POS_SCORE_X		POS_LINES_X

#define STR_START		"BEGIN"
#define LEN_START_STR	sizeof(STR_START)
#define POS_START_Y		(POS_SCORE_Y+3)
#define POS_START_X		(POS_SCORE_X+(TETRIS_WIDTH-strlen(STR_START))/2)

#define STR_PAUSE		"PAUSE"
#define LEN_PAUSE_STR	sizeof(STR_PAUSE)
#define POS_PAUSE_Y		POS_START_Y
#define POS_PAUSE_X		(POS_SCORE_X+(TETRIS_WIDTH-strlen(STR_PAUSE))/2)

#define STR_QUIT		"QUIT"
#define LEN_QUIT_STR	sizeof(STR_QUIT)
#define POS_QUIT_Y		(POS_START_Y+3)
#define POS_QUIT_X		(POS_SCORE_X+(TETRIS_WIDTH-strlen(STR_QUIT))/2)

#define STR_TITLE		"Tetris"
#define LEN_TITLE_STR	sizeof(STR_TITLE)
#define POS_TITLE_Y		(POS_QUIT_Y+3)
#define POS_TITLE_X		(POS_SCORE_X+(TETRIS_WIDTH-strlen(STR_TITLE))/2)

#define BLOCK_TOTAL_NUM	19	//BLOCK_I_NUM+...+BLOCK_T_NUM
#define BLOCK_TYPE_NUM	7

#define BLOCK_I_NUM 	2
#define BLOCK_J_NUM 	4
#define BLOCK_L_NUM 	4
#define BLOCK_O_NUM 	1
#define BLOCK_S_NUM 	2
#define BLOCK_Z_NUM 	2
#define BLOCK_T_NUM 	4
#define BLOCK_SINGLE_MAX_NUM	4


#define BLOCK_I_START_NUM	0
#define BLOCK_J_START_NUM	(BLOCK_I_START_NUM+BLOCK_I_NUM)
#define BLOCK_L_START_NUM	(BLOCK_J_START_NUM+BLOCK_J_NUM)
#define BLOCK_O_START_NUM	(BLOCK_L_START_NUM+BLOCK_L_NUM)
#define BLOCK_S_START_NUM	(BLOCK_O_START_NUM+BLOCK_O_NUM)
#define BLOCK_Z_START_NUM	(BLOCK_S_START_NUM+BLOCK_S_NUM)
#define BLOCK_T_START_NUM	(BLOCK_Z_START_NUM+BLOCK_Z_NUM)

#define BLOCK_I0_MIN_X		2
#define BLOCK_I0_MIN_Y		1
#define BLOCK_I0_MAX_X		2
#define BLOCK_I0_MAX_Y		4

#define BLOCK_I1_MIN_X		1
#define BLOCK_I1_MIN_Y		2
#define BLOCK_I1_MAX_X		4
#define BLOCK_I1_MAX_Y		2

#define BLOCK_J0_MIN_X		2
#define BLOCK_J0_MIN_Y		1
#define BLOCK_J0_MAX_X		3
#define BLOCK_J0_MAX_Y		3

#define BLOCK_J1_MIN_X		1
#define BLOCK_J1_MAX_X		3
#define BLOCK_J1_MIN_Y		1
#define BLOCK_J1_MAX_Y		2

#define BLOCK_J2_MIN_X		2
#define BLOCK_J2_MIN_Y		1
#define BLOCK_J2_MAX_X		3
#define BLOCK_J2_MAX_Y		3

#define BLOCK_J3_MIN_X		1
#define BLOCK_J3_MIN_Y		2
#define BLOCK_J3_MAX_X		3
#define BLOCK_J3_MAX_Y		3

#define BLOCK_L0_MIN_X		2
#define BLOCK_L0_MIN_Y		1
#define BLOCK_L0_MAX_X		3
#define BLOCK_L0_MAX_Y		3

#define BLOCK_L1_MIN_X		1
#define BLOCK_L1_MIN_Y		2
#define BLOCK_L1_MAX_X		3
#define BLOCK_L1_MAX_Y		3

#define BLOCK_L2_MIN_X		1
#define BLOCK_L2_MIN_Y		1
#define BLOCK_L2_MAX_X		2
#define BLOCK_L2_MAX_Y		3

#define BLOCK_L3_MIN_X		1
#define BLOCK_L3_MIN_Y		1
#define BLOCK_L3_MAX_X		3
#define BLOCK_L3_MAX_Y		2

#define BLOCK_O0_MIN_X		2
#define BLOCK_O0_MIN_Y		2
#define BLOCK_O0_MAX_X		3
#define BLOCK_O0_MAX_Y		3

#define BLOCK_S0_MIN_X		1
#define BLOCK_S0_MIN_Y		1
#define BLOCK_S0_MAX_X		3
#define BLOCK_S0_MAX_Y		2

#define BLOCK_S1_MIN_X		2
#define BLOCK_S1_MIN_Y		1
#define BLOCK_S1_MAX_X		3
#define BLOCK_S1_MAX_Y		3

#define BLOCK_Z0_MIN_X		1
#define BLOCK_Z0_MIN_Y		1
#define BLOCK_Z0_MAX_X		3
#define BLOCK_Z0_MAX_Y		2

#define BLOCK_Z1_MIN_X		2
#define BLOCK_Z1_MIN_Y		1
#define BLOCK_Z1_MAX_X		3
#define BLOCK_Z1_MAX_Y		3

#define BLOCK_T0_MIN_X		1
#define BLOCK_T0_MIN_Y		1
#define BLOCK_T0_MAX_X		3
#define BLOCK_T0_MAX_Y		2

#define BLOCK_T1_MIN_X		2
#define BLOCK_T1_MIN_Y		1
#define BLOCK_T1_MAX_X		3
#define BLOCK_T1_MAX_Y		3

#define BLOCK_T2_MIN_X		1
#define BLOCK_T2_MIN_Y		2
#define BLOCK_T2_MAX_X		3
#define BLOCK_T2_MAX_Y		3

#define BLOCK_T3_MIN_X		1
#define BLOCK_T3_MIN_Y		1
#define BLOCK_T3_MAX_X		2
#define BLOCK_T3_MAX_Y		3

#define REMOVE_MAX_LAYER	4
#define REMOVE_MIN_LAYER	0

#define KEY_ESC			27
#define TIME_MIN_UNIT	100000 //us
#define TIME_MAX_DELAY	1200000 //us

#define TRUE	1
#define FALSE	0
typedef int BOOL;


typedef struct screen{
	WINDOW *win;
	int nlines;
	int ncols;
	int begin_y;
	int begin_x;
}screen_t;

typedef struct prompt{
	char level_key[LEN_LEVEL_KEY];
	int level_value;
	char *level_show;
	
	char lines_key[LEN_LINES_KEY];
	int lines_value;
	char *lines_show;
	
	char score_key[LEN_SCORE_KEY];
	int score_value;
	char *score_show;

	char start[LEN_START_STR];
	char pause[LEN_PAUSE_STR];
	char quit[LEN_QUIT_STR];
	char title[LEN_TITLE_STR];
}prompt_t;

typedef enum block_type{
	BLOCK_I=0,
	BLOCK_J,
	BLOCK_L,
	BLOCK_O,
	BLOCK_S,
	BLOCK_Z,
	BLOCK_T,
	BLOCK_TYPE_NONE,
}block_type_t;	//Tetris block type

typedef enum color{
	COLOR_I=1, 
	COLOR_J,
	COLOR_L,
	COLOR_O,
	COLOR_S,
	COLOR_Z,
	COLOR_T,
	COLOR_SCREEN,
	COLOR_ERROR_PROMPT,
	COLOR_NORMAL_PROMPT,
	COLOR_BUTTON,
	COLOR_TITLE,
	
}color_t;

typedef struct grid{
	int y;
	int x;		//Grid coordinates
	block_type_t type;	//Grid belongs to which block,used to set the grid color
	char value;			//'1' or '0','1' indicates block,'0' indicates no block in this position
}grid_t;

typedef struct block{
	int y;
	int x;
	block_type_t type;
	int number;		//Block type number(0~BLOCK_X_NUM-1)
	char blck[GRID_LEN*GRID_LEN+1];	//Displayed on the screen
}block_t;

typedef enum direction{
	DIR_LEFT,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_NONE,
}dir_t;		//Move direction of the block

typedef enum status{
	STATUS_INIT,
	STATUS_BEGIN,
	STATUS_PAUSE,
	STATUS_CONFIRM_QUIT,
	STATUS_QUIT,
	STATUS_OTHERS,
}status_t;

typedef enum event{
	EVENT_KEY,
	EVENT_MOUSE,
	EVENT_NONE,
}event_t;

typedef struct tetris{
	screen_t scr;
	prompt_t prompt;
	grid_t grid[TETRIS_HEIGHT][TETRIS_WIDTH];
	block_t cur_block;
	dir_t dir;	//move direction of the cur_block
	block_t next_block;
	status_t status;
	event_t event;
	unsigned int min_unit_time;
}tetris_t;

extern char blck[BLOCK_TOTAL_NUM][GRID_LEN*GRID_LEN+1];


int init_tetris(tetris_t **tetris);
void exit_tetris(tetris_t *tetris);

pthread_mutex_t mutex;
#endif	/*TETRIS_H*/
