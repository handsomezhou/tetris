/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <string.h>
#include "output_tetris.h"

static void draw_screen(screen_t *screen);
static void clear_screen(screen_t *screen);
static void draw_prompt(const screen_t *screen, prompt_t *prompt,status_t status);
//just for test
static void draw_grid(const screen_t *screen,const grid_t (*pgrid)[TETRIS_WIDTH]);
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
				mvwaddch(scr->win,y,x,PAINT_SCREEN_X);
			}else if(y==scr->begin_y+NEXT_BLOCK_HEIGHT&&x>scr->begin_x+TETRIS_WIDTH&&x<max_x-1){
				mvwaddch(scr->win,y,x,PAINT_SCREEN_X);
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
	
	if(has_colors()){attron(COLOR_PAIR(COLOR_NORMAL_PROMPT));}
	mvwprintw(scr->win,scr->begin_y+POS_LEVEL_Y,scr->begin_x+POS_LEVEL_X,\
		"%s",ppt->level_show);
	mvwprintw(scr->win,scr->begin_y+POS_LINES_Y,scr->begin_x+POS_LINES_X,\
		"%s",ppt->lines_show);
	mvwprintw(scr->win,scr->begin_y+POS_SCORE_Y,scr->begin_x+POS_SCORE_X,\
		"%s",ppt->score_show);
	if(has_colors()){attroff(COLOR_PAIR(COLOR_NORMAL_PROMPT));}

	
	if(has_colors()){attron(COLOR_PAIR(COLOR_BUTTON)|A_BOLD);}
	switch(sts){
		case STATUS_START:
		case STATUS_INIT:
			mvwprintw(scr->win,scr->begin_y+POS_PAUSE_Y,scr->begin_x+POS_PAUSE_X,\
		"%s",ppt->pause);
			break;
			
		case STATUS_PAUSE:
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
			mvwprintw(scr->win,y,x,"%c",pg[y-(scr->begin_y+1)][x-(scr->begin_x+1)].value);
		}
	}
	

	wrefresh(scr->win);
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
	
	//just for test
	if(has_colors()){attron(COLOR_PAIR(COLOR_TEST));}
	for(y=scr->begin_y+bck->y; y<scr->begin_y+bck->y+GRID_LEN; y++){
		for(x=scr->begin_x+bck->x; x<scr->begin_x+bck->x+GRID_LEN; x++){
			if(bck->blck[(y-scr->begin_y-bck->y)*GRID_LEN+(x-scr->begin_x-bck->x)]=='1'){
				mvwaddch(scr->win,y,x,PAINT_BLOCK_UNIT);
			}else{
			//just for test
				mvwaddch(scr->win,y,x,' ');
			}
		}
	}
	//just for test
	mvwprintw(scr->win,y+2,1,"%s:%d-%d",__func__,bck->type,bck->number);
	if(has_colors()){attroff(COLOR_PAIR(COLOR_TEST));}
	wrefresh(scr->win);
	
	return;
}

static void draw_next_block(const screen_t *screen,block_t *block)
{
	const screen_t *scr=screen;
	block_t *bck=block;
	int y,x;
	if(NULL==scr||NULL==bck){
		return;
	}
	
	bck->y=scr->begin_y+POS_NEXT_BLOCK_Y;
	bck->x=scr->begin_x+POS_NEXT_BLOCK_X;

	if(has_colors()){attron(COLOR_PAIR(COLOR_TEST));}
	for(y=bck->y; y<bck->y+GRID_LEN; y++){
		for(x=bck->x; x<bck->x+GRID_LEN; x++){
			if(bck->blck[(y-bck->y)*GRID_LEN+(x-bck->x)]=='1'){
				mvwaddch(scr->win,y,x,PAINT_BLOCK_UNIT);
			}else{
			//just for test
				mvwaddch(scr->win,y,x,' ');
			}
		}
	}
	if(has_colors()){attroff(COLOR_PAIR(COLOR_TEST));}
	wrefresh(scr->win);
	
	return;
}

