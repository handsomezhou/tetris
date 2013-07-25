#include <stdlib.h>
#include <unistd.h>
#include "tetris.h"
#include "handle_tetris.h"

char blck[BLOCK_TOTAL_NUM][GRID_LEN*GRID_LEN+1]=
{//BLOCK_I
		{'0','1','0','0',
		 '0','1','0','0',
		 '0','1','0','0',
		 '0','1','0','0','\0',},

		{'0','0','0','0',
		 '1','1','1','1',
		 '0','0','0','0',
		 '0','0','0','0','\0',},
//BLOCK_J
		{'0','0','1','0',
		 '0','0','1','0',
		 '0','1','1','0',
		 '0','0','0','0','\0',},

		{'0','1','0','0',
		 '0','1','1','1',
		 '0','0','0','0',
		 '0','0','0','0','\0',},

		{'0','0','1','1',
		 '0','0','1','0',
		 '0','0','1','0',
		 '0','0','0','0','\0',},
		
		{'0','0','0','0',
		 '0','1','1','1',
		 '0','0','0','1',
		 '0','0','0','0','\0',},
//BLOCK_L
		{'0','1','0','0',
		 '0','1','0','0',
		 '0','1','1','0',
		 '0','0','0','0','\0',},

		{'0','0','0','0',
		 '1','1','1','0',
		 '1','0','0','0',
		 '0','0','0','0','\0',},

		{'1','1','0','0',
		 '0','1','0','0',
		 '0','1','0','0',
		 '0','0','0','0','\0',},

		{'0','0','1','0',
		 '1','1','1','0',
		 '0','0','0','0',
		 '0','0','0','0','\0',},
//BLOCK_O
		{'0','0','0','0',
		 '0','1','1','0',
		 '0','1','1','0',
		 '0','0','0','0','\0',},
//BLOCK_S
		{'0','0','0','0',
		 '0','1','1','0',
		 '1','1','0','0',
		 '0','0','0','0','\0',},

		{'1','0','0','0',
		 '1','1','0','0',
		 '0','1','0','0',
		 '0','0','0','0','\0',},

		{'0','1','1','0',
		 '1','1','0','0',
		 '0','0','0','0',
		 '0','0','0','0','\0',},

		{'0','1','0','0',
		 '0','1','1','0',
		 '0','0','1','0',
		 '0','0','0','0','\0',},
//BLOCK_Z
		{'1','1','0','0',
		 '0','1','1','0',
		 '0','0','0','0',
		 '0','0','0','0','\0',},

		{'0','0','1','0',
		 '0','1','1','0',
		 '0','1','0','0',
		 '0','0','0','0','\0',},

		{'0','0','0','0',
		 '1','1','0','0',
		 '0','1','1','0',
		 '0','0','0','0','\0',},

		{'0','1','0','0',
		 '1','1','0','0',
		 '1','0','0','0',
		 '0','0','0','0','\0',},
};


static int init_screen(screen_t *screen);
static void exit_screen(screen_t *screen);


int init_tetris(tetris_t **tetris)
{
	tetris_t **ttrs=tetris;
	int ret;
	
	if(NULL!=*ttrs){
		exit_tetris(*ttrs);
		*ttrs=NULL;
	}

	*ttrs=(tetris_t *)malloc(sizeof(tetris_t));
	do{
		if(NULL==*ttrs){
			break;
		}	

		ret=init_screen(&(*ttrs)->scr);
		if(TTRS_FAILED==ret){
			break;
		}

		//just for test
		next_block(&(*ttrs)->scr,&(*ttrs)->next_block);
		current_block(&(*ttrs)->scr,&(*ttrs)->cur_block,&(*ttrs)->next_block);
		
		return TTRS_SUCCESS;
		
	}while(0);
	
	return TTRS_FAILED;
}

void exit_tetris(tetris_t *tetris)
{
	tetris_t *ttrs=tetris;
	if(NULL==ttrs){
		return;
	}
	
	exit_screen(&ttrs->scr);
	
	return;
}

static int init_screen(screen_t *screen)
{
	screen_t *scr=screen;
	int max_y;
	int max_x;
	
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

		init_pair(COLOR_I,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(COLOR_J,COLOR_RED,COLOR_BLACK);
		init_pair(COLOR_L,COLOR_GREEN,COLOR_BLACK);
		init_pair(COLOR_O,COLOR_YELLOW,COLOR_BLACK);
		init_pair(COLOR_S,COLOR_BLUE,COLOR_BLACK);
		init_pair(COLOR_Z,COLOR_MAGENTA,COLOR_BLACK);
		init_pair(COLOR_T,COLOR_BLACK,COLOR_BLACK);

		init_pair(COLOR_SCREEN,COLOR_BLUE,COLOR_BLUE);

		//just fot test
		init_pair(COLOR_TEST,COLOR_YELLOW,COLOR_RED);
	}
	getmaxyx(stdscr,max_y,max_x);
	scr->win=stdscr;
	scr->nlines=SCREEN_HEIGHT;
	scr->ncols=SCREEN_WIDTH;
	scr->begin_y=(max_y-scr->nlines)/2;
	scr->begin_x=(max_x-scr->ncols)/2;
	wrefresh(scr->win);
	
	return TTRS_SUCCESS;
}

static void exit_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(scr==NULL){
		return;
	}
	werase(scr->win);
	endwin();

	return;
}


