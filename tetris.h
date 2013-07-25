#ifndef TETRIS_H
#define TETRIS_H
#include <ncurses.h>

#define	TETRIS_HEIGHT	20
#define TETRIS_WIDTH	10

#define SCREEN_HEIGHT 	(TETRIS_HEIGHT+2)
#define SCREEN_WIDTH	(TETRIS_WIDTH*2+3)

#define GRID_LEN	4

#define BLOCK_TOTAL_NUM	19	//BLOCK_I_NUM+...+BLOCK_T_NUM

#define BLOCK_INIT_X		0
#define BLOCK_INIT_Y		0

#define BLOCK_ERROR_TYPE	-1
#define BLOCK_ERROR_NUM		-1


#define TTRS_SUCCESS	0
#define TTRS_FAILED		-1

#define PAINT_SCREEN_Y		'#'
#define PAINT_SCREEN_X		'#'
#define PAINT_BLOCK_UNIT 	'o'

#define POS_NEXT_BLOCK_Y 	1
#define POS_NEXT_BLOCK_X 	(TETRIS_WIDTH+2+3)
#define POS_CURRENT_BLOCK_Y -4
#define POS_CURRENT_BLOCK_X	((TETRIS_WIDTH-GRID_LEN)/2+1)

#define NEXT_BLOCK_HEIGHT	5
#define NEXT_BLOCK_WIDTH	4

#define TRUE	1
#define FALSE	0
typedef int BOOL;

#define BLOCK_I_NUM 2
#define BLOCK_J_NUM 4
#define BLOCK_L_NUM 4
#define BLOCK_O_NUM 1
#define BLOCK_S_NUM 2
#define BLOCK_Z_NUM 2
#define BLOCK_T_NUM 4

#define BLOCK_I_START_NUM	0
#define BLOCK_J_START_NUM	(BLOCK_I_START_NUM+BLOCK_I_NUM)
#define BLOCK_L_START_NUM	(BLOCK_J_START_NUM+BLOCK_J_NUM)
#define BLOCK_O_START_NUM	(BLOCK_L_START_NUM+BLOCK_L_NUM)
#define BLOCK_S_START_NUM	(BLOCK_O_START_NUM+BLOCK_O_NUM)
#define BLOCK_Z_START_NUM	(BLOCK_S_START_NUM+BLOCK_S_NUM)
#define BLOCK_T_START_NUM	(BLOCK_Z_START_NUM+BLOCK_Z_NUM)

typedef struct screen{
	WINDOW *win;
	int nlines;
	int ncols;
	int begin_y;
	int begin_x;
}screen_t;

typedef enum block_type{
	BLOCK_I=0,
	BLOCK_J,
	BLOCK_L,
	BLOCK_O,
	BLOCK_S,
	BLOCK_Z,
	BLOCK_T,
	BLOCK_NONE,
}block_type_t;	//Tetris block type

typedef enum color{
	COLOR_I=0,
	COLOR_J,
	COLOR_L,
	COLOR_O,
	COLOR_S,
	COLOR_Z,
	COLOR_T,

	COLOR_SCREEN,
	COLOR_TEST,	//just for test
}color_t;

typedef struct grid{
	int y;
	int x;		//Grid coordinates
	block_type_t type;	//Grid belongs to which block,used to set the grid color
}grid_t;

typedef struct block{
	int y;
	int x;
	block_type_t type;
	int number;		//Block type number(0~BLOCK_X_NUM-1)
	char blck[GRID_LEN*GRID_LEN+1];	//Displayed on the screen
}block_t;

typedef enum status{
	STATUS_START,
	STATUS_PAUSE,
	STATUS_QUIT,
}status_t;

typedef struct tetris{
	screen_t scr;
	grid_t grid[TETRIS_HEIGHT][TETRIS_WIDTH];
	block_t cur_block;
	block_t next_block;
	status_t status;
}tetris_t;

extern char blck[BLOCK_TOTAL_NUM][GRID_LEN*GRID_LEN+1];


int init_tetris(tetris_t **tetris);
void exit_tetris(tetris_t *tetris);
#endif	/*TETRIS_H*/
