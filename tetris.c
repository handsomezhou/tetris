/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
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

		{'1','0','0','0',
		 '1','1','1','0',
		 '0','0','0','0',
		 '0','0','0','0','\0',},

		{'0','1','1','0',
		 '0','1','0','0',
		 '0','1','0','0',
		 '0','0','0','0','\0',},
		
		{'0','0','0','0',
		 '1','1','1','0',
		 '0','0','1','0',
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
//BLOCK_T
		{'0','1','0','0',
		 '1','1','1','0',
		 '0','0','0','0',
		 '0','0','0','0','\0',},

		{'0','1','0','0',
		 '0','1','1','0',
		 '0','1','0','0',
		 '0','0','0','0','\0',},

		{'0','0','0','0',
		 '1','1','1','0',
		 '0','1','0','0',
		 '0','0','0','0','\0',},

		{'0','1','0','0',
		 '1','1','0','0',
		 '0','1','0','0',
		 '0','0','0','0','\0',},

};


static int init_screen(screen_t *screen);
static void exit_screen(screen_t *screen);
static int init_prompt(prompt_t *prompt);
static void exit_prompt(prompt_t *prompt);
static int init_grid(grid_t (*pgrid)[TETRIS_WIDTH]);

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

		ret=init_prompt(&(*ttrs)->prompt);
		if(TTRS_FAILED==ret){
			break;
		}

		ret=init_grid(&(*ttrs)->grid[0]);
		if(TTRS_FAILED==ret){
			break;
		}
		
		//just for test
		next_block(&(*ttrs)->scr,&(*ttrs)->next_block);
		current_block(&(*ttrs)->scr,&(*ttrs)->cur_block,&(*ttrs)->next_block);

		(*ttrs)->status=STATUS_INIT;
		(*ttrs)->event=EVENT_NONE;
		(*ttrs)->min_unit_time=MIN_UNIT_TIME;

		signal(SIGINT,interrupt_info);
		pthread_mutex_init(&mutex,NULL);
		
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

	pthread_mutex_destroy(&mutex);
	exit_prompt(&ttrs->prompt);
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

	//just for test
	curs_set(TRUE);
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
		init_pair(COLOR_ERROR_PROMPT,COLOR_RED,COLOR_WHITE);
		init_pair(COLOR_NORMAL_PROMPT,COLOR_GREEN,COLOR_BLACK);
		init_pair(COLOR_BUTTON,COLOR_YELLOW,COLOR_GREEN);
		init_pair(COLOR_TITLE,COLOR_GREEN,COLOR_BLACK);

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
	keypad(scr->win,TRUE);
	nodelay(stdscr,TRUE);
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


static int init_prompt(prompt_t *prompt)
{
	prompt_t *ppt=prompt;
	char level_prompt[]="Failed to allocate space for level_show";
	char lines_prompt[]="Failed to allocate space for lines_show";
	char score_prompt[]="Failed to allocate space for score_show";
	
	int scr_y,scr_x;
	getmaxyx(stdscr,scr_y,scr_x);
	if(NULL==ppt){
		return TTRS_FAILED;
	}
	
	if(NULL!=ppt->level_show||NULL!=ppt->lines_show||NULL!=ppt->score_show){
		exit_prompt(ppt);
		ppt->level_show=NULL;
		ppt->lines_show=NULL;
		ppt->score_show=NULL;
	}
	//init level_xxx
	memset(ppt->level_key,0,LEN_LEVEL_KEY);
	strncpy(ppt->level_key,STR_LEVEL,LEN_LEVEL_KEY);
	ppt->level_value=LEVEL_KEY_VALUE;
	ppt->level_show=malloc(LEN_LEVEL_SHOW);
	if(NULL==ppt->level_show){
		if(has_colors()){attron(COLOR_PAIR(COLOR_ERROR_PROMPT));}
		mvwprintw(stdscr,scr_y/2,(scr_x-strlen(level_prompt))/2,"%s",level_prompt);
		if(has_colors()){attroff(COLOR_PAIR(COLOR_ERROR_PROMPT));}
		nodelay(stdscr,FALSE);
		wgetch(stdscr);
		nodelay(stdscr,TRUE);
		
		return TTRS_FAILED;
	}
	snprintf(ppt->level_show,LEN_LEVEL_SHOW,"%s%5d",ppt->level_key,ppt->level_value);

	//init lines_xxx
	memset(ppt->lines_key,0,LEN_LINES_KEY);
	strncpy(ppt->lines_key,STR_LINES,LEN_LINES_KEY);
	ppt->lines_value=LINES_KEY_VALUE;
	ppt->lines_show=malloc(LEN_LINES_SHOW);
	if(NULL==ppt->lines_show){
		if(has_colors()){attron(COLOR_PAIR(COLOR_ERROR_PROMPT));}
		mvwprintw(stdscr,scr_y/2,(scr_x-strlen(lines_prompt))/2,"%s",lines_prompt);
		if(has_colors()){attroff(COLOR_PAIR(COLOR_ERROR_PROMPT));}
		nodelay(stdscr,FALSE);
		wgetch(stdscr);
		nodelay(stdscr,TRUE);
		
		return TTRS_FAILED;
	}
	snprintf(ppt->lines_show,LEN_LINES_SHOW,"%s%5d",ppt->lines_key,ppt->lines_value);

	//init score_xxx
	memset(ppt->score_key,0,LEN_SCORE_KEY);
	strncpy(ppt->score_key,STR_SCORE,LEN_SCORE_KEY);
	ppt->score_value=SCORE_KET_VALUE;
	ppt->score_show=malloc(LEN_SCORE_SHOW);
	if(NULL==ppt->score_show){
		if(has_colors()){attron(COLOR_PAIR(COLOR_ERROR_PROMPT));}
		mvwprintw(stdscr,scr_y/2,(scr_x-strlen(score_prompt))/2,"%s",score_prompt);
		if(has_colors()){attroff(COLOR_PAIR(COLOR_ERROR_PROMPT));}
		nodelay(stdscr,FALSE);
		wgetch(stdscr);
		nodelay(stdscr,TRUE);
		
		return TTRS_FAILED;
	}
	snprintf(ppt->score_show,LEN_LINES_SHOW,"%s%5d",ppt->score_key,ppt->score_value);

	//init start pause and quit
	memset(ppt->start,0,LEN_START_STR);
	strncpy(ppt->start,STR_START,LEN_START_STR);

	memset(ppt->pause,0,LEN_PAUSE_STR);
	strncpy(ppt->pause,STR_PAUSE,LEN_PAUSE_STR);

	memset(ppt->quit,0,LEN_QUIT_STR);
	strncpy(ppt->quit,STR_QUIT,LEN_QUIT_STR);

	memset(ppt->title,0,LEN_TITLE_STR);
	strncpy(ppt->title,STR_TITLE,LEN_TITLE_STR);
#if 0//just for test
	mvwprintw(stdscr,1,1,"%d-[%s]",LEN_LEVEL_SHOW,ppt->level_show);
	mvwprintw(stdscr,2,1,"%d-[%s]",LEN_LEVEL_SHOW,ppt->lines_show);
	mvwprintw(stdscr,3,1,"%d-[%s]",LEN_LEVEL_SHOW,ppt->score_show);
	nodelay(stdscr,FALSE);
	wgetch(stdscr);
	nodelay(stdscr,TRUE);
#endif

	return TTRS_SUCCESS;
}
static void exit_prompt(prompt_t *prompt)
{
	prompt_t *ppt=prompt;
	if(NULL==ppt){
		return;
	}
	
	if(NULL!=ppt->level_show){
		free(ppt->level_show);
		ppt->level_show=NULL;	
	}

	if(NULL!=ppt->lines_show){
		free(ppt->lines_show);
		ppt->lines_show=NULL;
	}

	if(NULL!=ppt->score_show){
		free(ppt->score_show);
		ppt->score_show=NULL;
	}
	
	return;
}

static int init_grid(grid_t (*pgrid)[TETRIS_WIDTH])
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;	
	int y,x;
	if(NULL==pg){
		return TTRS_FAILED;
	}
	
	for(y=0; y<TETRIS_HEIGHT; y++){
		for(x=0; x<TETRIS_WIDTH; x++){
			pg[y][x].y=y;
			pg[y][x].x=x;
			pg[y][x].type=BLOCK_TYPE_NONE;
			pg[y][x].value=BLOCK_INIT_VALUE;
		}
	}
	
	return TTRS_SUCCESS;
}

