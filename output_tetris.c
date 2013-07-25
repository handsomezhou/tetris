#include "output_tetris.h"

static void draw_screen(screen_t *screen);
static void clear_screen(screen_t *screen);
//just for test
//static void draw_grid();
static void draw_current_block(screen_t *screen,const block_t *block);
static void draw_next_block(screen_t *screen,block_t *block);

void paint_tetris(tetris_t *tetris)
{
	tetris_t *ttrs=tetris;
	if(NULL==ttrs){
		return;
	}
	clear_screen(&ttrs->scr);
	draw_screen(&ttrs->scr);
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

static void draw_current_block(screen_t *screen,const block_t *block)
{
	screen_t *scr=screen;
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
	if(has_colors()){attroff(COLOR_PAIR(COLOR_TEST));}
	wrefresh(scr->win);
	
	return;
}
static void draw_next_block(screen_t *screen,block_t *block)
{
	screen_t *scr=screen;
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

