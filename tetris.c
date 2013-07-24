#include <stdlib.h>
#include "tetris.h"
char block[BLOCK_TOTAL_NUM][GIRD_LEN*GIRD_LEN]=
{//BLOCK_I
		{'0','1','0','0',
		 '0','1','0','0',
		 '0','1','0','0',
		 '0','1','0','0',},
		 
		{'0','0','0','0',
		 '1','1','1','1',
		 '0','0','0','0',
		 '0','0','0','0',},
//BLOCK_J
		{'0','0','1','0',
		 '0','0','1','0',
		 '0','1','1','0',
		 '0','0','0','0',},

		{'0','1','0','0',
		 '0','1','1','1',
		 '0','0','0','0',
		 '0','0','0','0',},

		{'0','0','1','1',
		 '0','0','1','0',
		 '0','0','1','0',
		 '0','0','0','0',},
		
		{'0','0','0','0',
		 '0','1','1','1',
		 '0','0','0','1',
		 '0','0','0','0',},
//BLOCK_L
		{'0','1','0','0',
		 '0','1','0','0',
		 '0','1','1','0',
		 '0','0','0','0',},

		{'0','0','0','0',
		 '1','1','1','0',
		 '1','0','0','0',
		 '0','0','0','0',},

		{'1','1','0','0',
		 '0','1','0','0',
		 '0','1','0','0',
		 '0','0','0','0',},

		{'0','0','1','0',
		 '1','1','1','0',
		 '0','0','0','0',
		 '0','0','0','0',},
//BLOCK_O
		{'0','0','0','0',
		 '0','1','1','0',
		 '0','1','1','0',
		 '0','0','0','0',},
//BLOCK_S
		{'0','0','0','0',
		 '0','1','1','0',
		 '1','1','0','0',
		 '0','0','0','0',},

		{'1','0','0','0',
		 '1','1','0','0',
		 '0','1','0','0',
		 '0','0','0','0',},

		{'0','1','1','0',
		 '1','1','0','0',
		 '0','0','0','0',
		 '0','0','0','0',},

		{'0','1','0','0',
		 '0','1','1','0',
		 '0','0','1','0',
		 '0','0','0','0',},
//BLOCK_Z
		{'1','1','0','0',
		 '0','1','1','0',
		 '0','0','0','0',
		 '0','0','0','0',},

		{'0','0','1','0',
		 '0','1','1','0',
		 '0','1','0','0',
		 '0','0','0','0',},

		{'0','0','0','0',
		 '1','1','0','0',
		 '0','1','1','0',
		 '0','0','0','0',},

		{'0','1','0','0',
		 '1','1','0','0',
		 '1','0','0','0',
		 '0','0','0','0',},
};


int init_screen(screen_t *scree);
void exit_screen();

int init_tetris(tetris_t **tetris)
{
	tetris_t **ttrs=tetris;
	//int ret;
	
	if(NULL!=ttrs){
		exit_tetris(*ttrs);
		*ttrs=NULL;
	}

	*ttrs=(tetris_t *)malloc(sizeof(tetris_t));
	do{
		if(NULL==*ttrs){
			break;
		}	

	}while(0);
	
	return TTRS_FAILED;
}

void exit_tetris(tetris_t *tetris)
{
	tetris_t *ttrs=tetris;
	if(NULL==ttrs){
		return;
	}
	
	return;
}

int init_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return TTRS_FAILED;
	}
	//init ncurses
	initscr();
	cbreak();
	curs_set(FALSE);
	clear();
	if(has_colors()){
		start_color();

		init_pair(BLOCK_I,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(BLOCK_J,COLOR_RED,COLOR_BLACK);
		init_pair(BLOCK_L,COLOR_GREEN,COLOR_BLACK);
		init_pair(BLOCK_O,COLOR_YELLOW,COLOR_BLACK);
		init_pair(BLOCK_S,COLOR_BLUE,COLOR_BLACK);
		init_pair(BLOCK_Z,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(BLOCK_T,COLOR_BLACK,COLOR_BLACK);
	}

	return TTRS_SUCCESS;
}

void exit_screen()
{
	endwin();
}


