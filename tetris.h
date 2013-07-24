#ifndef TETRIS_H
#define TETRIS_H
#include <ncurses.h>

#define	TETRIS_HEIGHT	20
#define TETRIS_WIDTH	10

#define SCREEN_HEIGHT 	TETRIS_HEIGHT+4
#define SCREEN_WIDTH	(TETRIS_WIDTH+2)*2

#define GIRD_LEN	4

#define BLOCK_I_NUM	2
#define BLOCK_J_NUM	4
#define BLOCK_L_NUM	4
#define BLOCK_O_NUM	1
#define BLOCK_S_NUM	2
#define BLOCK_Z_NUM	2
#define BLOCK_T_NUM	4

#define BLOCK_TOTAL_NUM	19	//BLOCK_I_NUM+...+BLOCK_T_NUM

#define TTRS_SUCCESS	0
#define TTRS_FAILED		-1

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

#if 0
typedef enum color{
	COLOR_I=0,
	COLOR_J,
	COLOR_L,
	COLOR_O,
	COLOR_S,
	COLOR_Z,
	COLOR_T,
}color_t;
#endif

typedef struct grid{
	int y;
	int x;		//Grid coordinates
	block_type_t type;	//Grid belongs to which block,used to set the grid color
}grid_t;

typedef enum status{
	STATUS_START,
	STATUS_PAUSE,
	STATUS_QUIT,
}status_t;

typedef struct tetris{
	screen_t scr;
	grid_t grid[TETRIS_HEIGHT][TETRIS_WIDTH];
	
	status_t status;
}tetris_t;

extern char block[BLOCK_TOTAL_NUM][GIRD_LEN*GIRD_LEN];


int init_tetris(tetris_t **tetris);
void exit_tetris(tetris_t *tetris);
#endif	/*TETRIS_H*/
