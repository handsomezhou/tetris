/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "handle_tetris.h"
#include "output_tetris.h"

static block_t get_block_from_num(int num);
static int get_num_from_block(const block_t *block);
static block_t *block_copy(block_t *dest, const block_t *src);
static int get_random_block();
static block_t *new_block(block_t *block);
static BOOL confirm_exit();
//static BOOL can_rotate(const grid_t (*pgrid)[TETRIS_WIDTH],const block_t *block);
static block_t *rotate_block(grid_t (*pgrid)[TETRIS_WIDTH], block_t *block);
block_t *get_next_rotate_block(block_t *next_block, const block_t *cur_block);

static block_t get_block_from_num(int num)
{
	int n=num;
	block_t bck;
	bck.y=BLOCK_INIT_Y;
	bck.x=BLOCK_INIT_X;
	bck.type=BLOCK_ERROR_TYPE;
	bck.number=BLOCK_ERROR_NUM;
	strncpy(bck.blck,&blck[num][0],GRID_LEN*GRID_LEN+1);
	
	if(n<0||n>=BLOCK_TOTAL_NUM){
		return bck;
	}	
	
	if(n>=BLOCK_I_START_NUM&&n<BLOCK_I_START_NUM+BLOCK_I_NUM){
		bck.type=BLOCK_I;
		bck.number=n-BLOCK_I_START_NUM;
	}else if(n>=BLOCK_J_START_NUM&&n<BLOCK_J_START_NUM+BLOCK_J_NUM){
		bck.type=BLOCK_J;
		bck.number=n-BLOCK_J_START_NUM;
	}else if(n>=BLOCK_L_START_NUM&&n<BLOCK_L_START_NUM+BLOCK_L_NUM){
		bck.type=BLOCK_L;
		bck.number=n-BLOCK_L_START_NUM;
	}else if(n>=BLOCK_O_START_NUM&&n<BLOCK_O_START_NUM+BLOCK_O_NUM){
		bck.type=BLOCK_O;
		bck.number=n-BLOCK_O_START_NUM;
	}else if(n>=BLOCK_S_START_NUM&&n<BLOCK_S_START_NUM+BLOCK_S_NUM){
		bck.type=BLOCK_S;
		bck.number=n-BLOCK_S_START_NUM;
	}else if(n>=BLOCK_Z_START_NUM&&n<BLOCK_Z_START_NUM+BLOCK_Z_NUM){
		bck.type=BLOCK_Z;
		bck.number=n-BLOCK_Z_START_NUM;
	}else if(n>=BLOCK_T_START_NUM&&n<BLOCK_T_START_NUM+BLOCK_T_NUM){
		bck.type=BLOCK_T;
		bck.number=n-BLOCK_T_START_NUM;
	}

	return bck;
}

static int get_num_from_block(const block_t *block)
{
	const block_t *bck=block;
	int num;
	
	if(NULL==bck){
		num=TTRS_FAILED;
		return num;
	}

	switch(bck->type){
		case BLOCK_I:
			num=BLOCK_I_START_NUM+bck->number;
			break;

		case BLOCK_J:
			num=BLOCK_J_START_NUM+bck->number;
			break;

		case BLOCK_L:
			num=BLOCK_L_START_NUM+bck->number;
			break;

		case BLOCK_O:
			num=BLOCK_O_START_NUM+bck->number;
			break;

		case BLOCK_S:
			num=BLOCK_S_START_NUM+bck->number;
			break;

		case BLOCK_Z:
			num=BLOCK_Z_START_NUM+bck->number;
			break;

		case BLOCK_T:
			num=BLOCK_T_START_NUM+bck->number;
			break;
		default:
			num=TTRS_FAILED;
			break;
	}

	return num;
}

static block_t *block_copy(block_t *dest, const block_t *src)
{
	block_t *dst=dest;
	if(NULL==dst||NULL==src){
		return dst;
	}
	dst->y=src->y;
	dst->x=src->x;
	dst->type=src->type;
	dst->number=src->number;
	memset(dst->blck,'\0',GRID_LEN*GRID_LEN+1);
	strncpy(dst->blck,src->blck,GRID_LEN*GRID_LEN+1);
	
	return dst;
}

static int get_random_block()
{
	int num;
	
	num=rand()%BLOCK_TOTAL_NUM;

	return num;
}

static block_t *new_block(block_t *block)
{
	block_t *bck=block;
	int num;
	block_t tmp;
	block_t *ret=NULL;

	do{
		if(NULL==bck){
			break;
		}
	
		num=get_random_block();
		tmp=get_block_from_num(num);
		if(tmp.type==BLOCK_ERROR_TYPE||tmp.number==BLOCK_ERROR_NUM){
			break;
		}

		ret=block_copy(bck,&tmp);
		if(NULL==ret){
			break;
		}
		
		return bck;
	}while(0);

	return NULL;
}

static BOOL confirm_exit()
{
	int ch=-1;
	draw_confirm_exit();
	nodelay(stdscr,FALSE);
	ch=wgetch(stdscr);
	nodelay(stdscr,TRUE);
	if('Y'==ch||'y'==ch){
		return TRUE;
	}else{
		return FALSE;
	}
	
}
//just for test
#if 0
static BOOL can_rotate(const grid_t (*pgrid)[TETRIS_WIDTH],const block_t *block)
{
	const grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	const block_t *bck=block;
	block_t tmp;
	block_t *pbck=NULL;
	
	if(NULL==pg||NULL==bck){
		return FALSE;
	}

	pbck=get_next_rotate_block(&tmp,bck);
	if(pbck==NULL){
		return FALSE;
	}
	
	return TRUE;
}
#endif
block_t *get_next_rotate_block(block_t *next_block, const block_t *cur_block)
{
	block_t *nbk=next_block;
	const block_t *cbk=cur_block;
	int num=-1;
	if(NULL==cbk||NULL==nbk){
		return NULL;
	}

	block_copy(nbk,cbk);
	nbk->number++;
	switch(nbk->type){
		case BLOCK_I:
			nbk->number=nbk->number%BLOCK_I_NUM;
			break;
		case BLOCK_J:
			nbk->number=nbk->number%BLOCK_J_NUM;
			break;
		case BLOCK_L:
			nbk->number=nbk->number%BLOCK_L_NUM;
			break;
		case BLOCK_O:
			nbk->number=nbk->number%BLOCK_O_NUM;
			break;
		case BLOCK_S:
			nbk->number=nbk->number%BLOCK_S_NUM;
			break;
		case BLOCK_Z:
			nbk->number=nbk->number%BLOCK_Z_NUM;
			break;
		case BLOCK_T:
			nbk->number=nbk->number%BLOCK_T_NUM;
			break;
		default:
			return NULL;
			break;
	}
	
	num=get_num_from_block(nbk);
	memset(nbk->blck,'\0',GRID_LEN*GRID_LEN+1);
	strncpy(nbk->blck,blck[num],GRID_LEN*GRID_LEN+1);

	return nbk;
	
}

static block_t *rotate_block(grid_t (*pgrid)[TETRIS_WIDTH], block_t *block)
{

	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	block_t *bck=block;
	block_t *pbck=NULL;
	block_t tmp;
	int num;
	BOOL bl=FALSE;

	pbck=get_next_rotate_block(&tmp,bck);
	if(NULL==pbck){
		return NULL;
	}
	
	block_copy(bck,&tmp);

	return bck;
}

void interrupt_info(int sign)
{
	char info_key[]="you can exit the program by press key  'ESC','q','Q',";
	char info_mouse[]="exit the program by mouse click 'QUIT'.";
	
	int scr_y,scr_x;
	getmaxyx(stdscr,scr_y,scr_x);
	if(has_colors()){attron(COLOR_PAIR(COLOR_NORMAL_PROMPT));}
	mvwprintw(stdscr,scr_y/2,((scr_x-strlen(info_key))/2>0)?((scr_x-strlen(info_key))/2):(1),"%s",info_key);
	mvwprintw(stdscr,scr_y/2+1,((scr_x-strlen(info_mouse))/2>0)?((scr_x-strlen(info_mouse))/2):(1),"%s",info_mouse);
	if(has_colors()){attroff(COLOR_PAIR(COLOR_NORMAL_PROMPT));}

	usleep(3000000);
}

block_t *next_block(const screen_t *screen,block_t *block)
{
	block_t *bck=block;
	const screen_t *scr=screen;
	block_t tmp;
	block_t *ret=NULL;
	
	do{
		if(NULL==bck||NULL==scr){
			break;
		}

		ret=new_block(&tmp);
		if(NULL==ret){
			break;
		}
		tmp.y=scr->begin_y+POS_NEXT_BLOCK_Y;
		tmp.x=scr->begin_x+POS_NEXT_BLOCK_X;

		ret=block_copy(bck,&tmp);
		if(NULL==ret){
			break;
		}

		return bck;
	}while(0);
	
	return NULL;
}

block_t *current_block(const screen_t *screen,block_t *current,const block_t *next)
{
	block_t *cur=current;
	const screen_t *scr=screen;
	block_t *ret=NULL;
	if(NULL==cur||NULL==next||NULL==scr){
		return NULL;
	}

	ret=block_copy(cur,next);
	if(NULL==ret){
		return NULL;
	}
	cur->y=POS_CURRENT_BLOCK_Y;
	cur->x=POS_CURRENT_BLOCK_X;
	
	return cur;
}

int handle_tetris(tetris_t *tetris)
{
	tetris_t *ttrs=tetris;
	//just for test
	ttrs->prompt.level_value++;
	ttrs->prompt.lines_value +=2;
	ttrs->prompt.score_value +=3;

	//just for test
	pthread_mutex_lock(&mutex);
	ttrs->cur_block.y++;
	pthread_mutex_unlock(&mutex);
	
	return TTRS_SUCCESS;
}


int deal_key_event(grid_t (*pgrid)[TETRIS_WIDTH],block_t *block, status_t *status, int key)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	block_t *bck=block;
	status_t *sts=status;
	status_t sts_tmp;
	if(NULL==pg||NULL==bck||NULL==sts){
		return TTRS_FAILED;
	}

	switch(key){
		case KEY_UP://Rotate
			//just for test
			//bck->y--;
			pthread_mutex_lock(&mutex);
			rotate_block(pg,bck);
			pthread_mutex_unlock(&mutex);
			break;
			
		case KEY_RIGHT://Right
			if(STATUS_PAUSE!=*sts){
				bck->x++;
			}
			
			break;
			
		case KEY_DOWN://Down
			if(STATUS_PAUSE!=*sts){
				//just for test
				bck->y++;
			}
			
			break;
			
		case KEY_LEFT://Left
			if(STATUS_PAUSE!=*sts){
				bck->x--;
			}
			
			break;

		case 'S'://Start
		case 's':
			if(STATUS_START!=*sts){
				*sts=STATUS_START;
			}
			break;
			
		case ' '://Pause
		case 'P':
		case 'p':
			if(STATUS_PAUSE!=*sts){
				*sts=STATUS_PAUSE;
			}else{
				*sts=STATUS_START;
			}
			break;

		case KEY_ESC://Quit
		case 'Q':
		case 'q':
			sts_tmp=*sts;
			*sts=STATUS_CONFIRM_QUIT;
			if(confirm_exit()==TRUE){
				*sts=STATUS_QUIT;
			}else{
				*sts=sts_tmp;
			}
			break;
			
		default:
			break;
	}
	
	return TTRS_SUCCESS;
}

int deal_mouse_event(const screen_t *screen, status_t *status, const MEVENT *mevent)
{
	const screen_t *scr=screen;
	status_t *sts=status;
	status_t sts_tmp;
	const MEVENT *evt=mevent;

	if(evt->y==scr->begin_y+POS_START_Y){
		switch(*sts){
			case STATUS_START:
			case STATUS_INIT:
				if(evt->x>=scr->begin_x+POS_START_X&&evt->x<scr->begin_x+POS_START_X+LEN_START_STR-1){
					*sts=STATUS_PAUSE;
				}
				break;
			case STATUS_PAUSE:
				if(evt->x>=scr->begin_x+POS_PAUSE_X&&evt->x<scr->begin_x+POS_PAUSE_X+LEN_PAUSE_STR-1){
					*sts=STATUS_START;
				}
				break;
			default:
				break;
		}
	}else if(evt->y==scr->begin_y+POS_QUIT_Y){
		if(evt->x>=scr->begin_x+POS_QUIT_X&&evt->x<scr->begin_x+POS_QUIT_X+LEN_QUIT_STR-1){
			sts_tmp=*sts;
			*sts=STATUS_CONFIRM_QUIT;
			if(confirm_exit()==TRUE){
				*sts=STATUS_QUIT;
			}else{
				*sts=sts_tmp;
			}
		}
	}else{

	}
	return TTRS_SUCCESS;
}


