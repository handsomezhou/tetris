/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <string.h>
#include <unistd.h>
#include "handle_tetris.h"
#include "output_tetris.h"

static void draw_screen(screen_t *screen);
static void clear_screen(screen_t *screen);
static void draw_prompt(const screen_t *screen, prompt_t *prompt,status_t status);
static void draw_grid(const screen_t *screen,const grid_t (*pgrid)[TETRIS_WIDTH]);
static void open_colors(block_type_t type);
static void close_colors(block_type_t type);
static void draw_block(const screen_t *screen, int start_y, int start_x,const block_t *block);
static void draw_current_block(const screen_t *screen,const block_t *block);
static void draw_next_block(const screen_t *screen,block_t *block);


void paint_tetris(tetris_t *tetris)
{
	tetris_t *ttrs=tetris;
	if(NULL==ttrs){
		return;
	}
	clear_screen(&ttrs->scr);
	draw_screen(&ttrs->scr);
	draw_prompt(&ttrs->scr,&ttrs->prompt,ttrs->status);
	draw_grid(&ttrs->scr,&ttrs->grid[0]);
	draw_next_block(&ttrs->scr,&ttrs->next_block);
	draw_current_block(&ttrs->scr,&ttrs->cur_block);
	if(ttrs->status==STATUS_CONFIRM_QUIT){
		draw_confirm_exit();
	}
	
	return;
}

void draw_confirm_exit()
{
		char confirm_quit[]="Please press key 'y' or 'Y' to confirm exit the program,";
		char confirm_continue[]="press others key to continue!";
	
		int scr_y,scr_x;
		getmaxyx(stdscr,scr_y,scr_x);
		if(has_colors()){attron(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}
		mvwprintw(stdscr,scr_y/2,((scr_x-strlen(confirm_quit))/2>0)?((scr_x-strlen(confirm_quit))/2):(1),"%s",confirm_quit);
		mvwprintw(stdscr,scr_y/2+1,((scr_x-strlen(confirm_continue))/2>0)?((scr_x-strlen(confirm_continue))/2):(1),"%s",confirm_continue);
		if(has_colors()){attroff(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}

		wrefresh(stdscr);
}

void draw_help_info()
{
	char ttrs_title[]="Tetris help";
	char ttrs_rotate[]="Rotate:[W] [w] [I] [i] [Arrow keys UP] ";
	char ttrs_right[] ="Right :[D] [d] [L] [l] [Arrow keys Right]";
	char ttrs_down[]  ="Down  :[S] [s] [K] [k] [Arrow keys Down]";
	char ttrs_left[]  ="Left  :[A] [a] [J] [j] [Arrow keys Left]";
	char ttrs_begin[] ="begin :[B] [b]";
	char ttrs_pause[] ="Pause :[P] [p] [Spacebar]";
	char ttrs_quit[]  ="Quit  :[Q] [q] [Esc]";
	char ttrs_help[]  ="Help  :[H] [h]";
	char ttrs_continue[]="Press any key or click mouse to continue!";
	int y=0;
	int x=1;
	int scr_y;
	int scr_x;
	int ch=ERR;
	nodelay(stdscr,TRUE);
	werase(stdscr);
	while(ch==ERR){
		y=0;
		getmaxyx(stdscr,scr_y,scr_x);
		wclear(stdscr);
		if(has_colors()){attron(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}
		mvwprintw(stdscr,++y,(scr_x-strlen(ttrs_title))/2,"%s",ttrs_title);
		y++;
		mvwprintw(stdscr,++y,x,"%s",ttrs_rotate);
		mvwprintw(stdscr,++y,x,"%s",ttrs_right);
		mvwprintw(stdscr,++y,x,"%s",ttrs_down);
		mvwprintw(stdscr,++y,x,"%s",ttrs_left);
		mvwprintw(stdscr,++y,x,"%s",ttrs_begin);
		mvwprintw(stdscr,++y,x,"%s",ttrs_pause);
		mvwprintw(stdscr,++y,x,"%s",ttrs_quit);
		mvwprintw(stdscr,++y,x,"%s",ttrs_help);
		box(stdscr,0,0);
		if(has_colors()){attroff(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}

		y++;
		if(has_colors()){attron(COLOR_PAIR(COLOR_BUTTON)|A_BOLD);}
		mvwprintw(stdscr,((scr_y/2)>y)?(scr_y/2):(y),(scr_x-strlen(ttrs_continue))/2,"%s",ttrs_continue);
		if(has_colors()){attroff(COLOR_PAIR(COLOR_BUTTON)|A_BOLD);}
		
		ch=wgetch(stdscr);
		wrefresh(stdscr);
		
		usleep(TIME_MIN_UNIT*2);
	}
	
	return;
}

static void draw_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return;
	}
	int y,x;
	int scr_y,scr_x;
	int max_y,max_x;

	getmaxyx(stdscr,scr_y,scr_x);
	scr->begin_y=(scr_y-scr->nlines)/2;
	scr->begin_x=(scr_x-scr->ncols)/2;
	max_y=scr->begin_y+scr->nlines;
	max_x=scr->begin_x+scr->ncols;
	
	if(has_colors()){attron(COLOR_PAIR(COLOR_SCREEN));}
	for(y=scr->begin_y; y<max_y; y++){
		for(x=scr->begin_x; x<max_x; x++){
			if(y==scr->begin_y||y==max_y-1||x==scr->begin_x||x==max_x-1||x==scr->begin_x+TETRIS_WIDTH+1){
				mvwaddch(scr->win,y,x,PAINT_SCREEN_WALL);
			}else if(y==scr->begin_y+NEXT_BLOCK_HEIGHT&&x>scr->begin_x+TETRIS_WIDTH&&x<max_x-1){
				mvwaddch(scr->win,y,x,PAINT_SCREEN_WALL);
			}
		}
	}
	if(has_colors()){attroff(COLOR_PAIR(COLOR_SCREEN));}
	
	wrefresh(scr->win);
}

static void clear_screen(screen_t *screen)
{
	screen_t *scr=screen;
	if(NULL==scr){
		return;
	}

	werase(scr->win);
	return;
}

static void draw_prompt(const screen_t *screen, prompt_t *prompt,status_t status)
{
	const screen_t *scr=screen;
	prompt_t *ppt=prompt;
	status_t sts=status;
	if(NULL==scr||NULL==ppt){
		return;
	}
	memset(ppt->level_show,0,LEN_LEVEL_SHOW);
	snprintf(ppt->level_show,LEN_LEVEL_SHOW,"%s%5d",ppt->level_key,ppt->level_value);

	memset(ppt->lines_show,0,LEN_LEVEL_SHOW);
	snprintf(ppt->lines_show,LEN_LEVEL_SHOW,"%s%5d",ppt->lines_key,ppt->lines_value);

	memset(ppt->score_show,0,LEN_LEVEL_SHOW);
	snprintf(ppt->score_show,LEN_LEVEL_SHOW,"%s%5d",ppt->score_key,ppt->score_value);
	
	if(has_colors()){attron(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}
	mvwprintw(scr->win,scr->begin_y+POS_LEVEL_Y,scr->begin_x+POS_LEVEL_X,\
		"%s",ppt->level_show);
	mvwprintw(scr->win,scr->begin_y+POS_LINES_Y,scr->begin_x+POS_LINES_X,\
		"%s",ppt->lines_show);
	mvwprintw(scr->win,scr->begin_y+POS_SCORE_Y,scr->begin_x+POS_SCORE_X,\
		"%s",ppt->score_show);
	if(has_colors()){attroff(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}

	
	if(has_colors()){attron(COLOR_PAIR(COLOR_BUTTON)|A_BOLD);}
	switch(sts){
		case STATUS_BEGIN:
		case STATUS_INIT:
			mvwprintw(scr->win,scr->begin_y+POS_PAUSE_Y,scr->begin_x+POS_PAUSE_X,\
		"%s",ppt->pause);
			break;
			
		case STATUS_PAUSE:
		case STATUS_CONFIRM_QUIT:
			mvwprintw(scr->win,scr->begin_y+POS_START_Y,scr->begin_x+POS_START_X,\
		"%s",ppt->start);
			break;
		default:
			break;
	}
	mvwprintw(scr->win,scr->begin_y+POS_QUIT_Y,scr->begin_x+POS_QUIT_X,\
		"%s",ppt->quit);
	if(has_colors()){attroff(COLOR_PAIR(COLOR_BUTTON)|A_BOLD);}

	if(has_colors()){attron(COLOR_PAIR(COLOR_TITLE)|A_BOLD);}
	mvwprintw(scr->win,scr->begin_y+POS_TITLE_Y,scr->begin_x+POS_TITLE_X,\
		"%s",ppt->title);
	if(has_colors()){attroff(COLOR_PAIR(COLOR_TITLE)|A_BOLD);}
	
	return;
}

static void draw_grid(const screen_t *screen, const grid_t (*pgrid)[TETRIS_WIDTH])
{
	const screen_t *scr=screen;
	const grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	int y,x;
	if(NULL==pg||NULL==scr){
		return ;
	}
	for(y=scr->begin_y+1; y<scr->begin_y+1+TETRIS_HEIGHT; y++){
		for(x=scr->begin_x+1; x<scr->begin_x+1+TETRIS_WIDTH; x++){
			if(pg[y-(scr->begin_y+1)][x-(scr->begin_x+1)].value=='1'){
				open_colors(pg[y-(scr->begin_y+1)][x-(scr->begin_x+1)].type);
				mvwaddch(scr->win,y,x,PAINT_BLOCK_UNIT);
				close_colors(pg[y-(scr->begin_y+1)][x-(scr->begin_x+1)].type);
			}
		}
	}
	
	wrefresh(scr->win);
	
	return;
}
static void open_colors(block_type_t type)
{
	switch(type){
		case BLOCK_I:
			if(has_colors()){attron(COLOR_PAIR(COLOR_I)|A_BOLD);}
			break;
		case BLOCK_J:
			if(has_colors()){attron(COLOR_PAIR(COLOR_J)|A_BOLD);}
			break;
		case BLOCK_L:
			if(has_colors()){attron(COLOR_PAIR(COLOR_L)|A_BOLD);}
			break;
		case BLOCK_O:
			if(has_colors()){attron(COLOR_PAIR(COLOR_O)|A_BOLD);}
			break;
		case BLOCK_S:
			if(has_colors()){attron(COLOR_PAIR(COLOR_S)|A_BOLD);}
			break;
		case BLOCK_Z:
			if(has_colors()){attron(COLOR_PAIR(COLOR_Z)|A_BOLD);}
			break;
		case BLOCK_T:
			if(has_colors()){attron(COLOR_PAIR(COLOR_T)|A_BOLD);}
			break;
		default:
			break;
	}
	
	return;
}

static void close_colors(block_type_t type)
{
	switch(type){
			case BLOCK_I:
				if(has_colors()){attroff(COLOR_PAIR(COLOR_I)|A_BOLD);}
				break;
			case BLOCK_J:
				if(has_colors()){attroff(COLOR_PAIR(COLOR_J)|A_BOLD);}
				break;
			case BLOCK_L:
				if(has_colors()){attroff(COLOR_PAIR(COLOR_L)|A_BOLD);}
				break;
			case BLOCK_O:
				if(has_colors()){attroff(COLOR_PAIR(COLOR_O)|A_BOLD);}
				break;
			case BLOCK_S:
				if(has_colors()){attroff(COLOR_PAIR(COLOR_S)|A_BOLD);}
				break;
			case BLOCK_Z:
				if(has_colors()){attroff(COLOR_PAIR(COLOR_Z)|A_BOLD);}
				break;
			case BLOCK_T:
				if(has_colors()){attroff(COLOR_PAIR(COLOR_T)|A_BOLD);}
				break;
			default:
				break;
		}

	return;
}

static void draw_block(const screen_t *screen, int start_y, int start_x,const block_t *block)
{
	const screen_t *scr=screen;
	const block_t *bck=block;
	int y,x;
	if(NULL==scr){
		return ;
	}
	open_colors(bck->type);
	for(y=start_y; y<start_y+GRID_LEN; y++){
		for(x=start_x; x<start_x+GRID_LEN; x++){
			if(bck->blck[(y-start_y)*GRID_LEN+(x-start_x)]=='1'&&(y>scr->begin_y)){
				mvwaddch(scr->win,y,x,PAINT_BLOCK_UNIT);
			}
		}
	}
	close_colors(bck->type);
	
	wrefresh(scr->win);
	
	return ;
}

static void draw_current_block(const screen_t *screen,const block_t *block)
{
	const screen_t *scr=screen;
	const block_t *bck=block;
	int y;
	int x;
	if(NULL==scr||NULL==bck){
		return;	
	}

	y=scr->begin_y+bck->y;
	x=scr->begin_x+bck->x;

	draw_block(scr,y,x,bck);
	
	return;
}

static void draw_next_block(const screen_t *screen,block_t *block)
{
	const screen_t *scr=screen;
	block_t *bck=block;
	if(NULL==scr||NULL==bck){
		return;
	}
	
	bck->y=scr->begin_y+POS_NEXT_BLOCK_Y;
	bck->x=scr->begin_x+POS_NEXT_BLOCK_X;

	draw_block(scr,bck->y,bck->x,bck);

	return;
}
