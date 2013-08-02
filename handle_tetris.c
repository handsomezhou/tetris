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
static block_t *rotate_block(grid_t (*pgrid)[TETRIS_WIDTH], block_t *block);
block_t *get_next_rotate_block(block_t *next_block, const block_t *cur_block);
static block_t *move_block(grid_t (*pgrid)[TETRIS_WIDTH], block_t *block, dir_t direction);
static BOOL is_collision_wall(const block_t *block,int min_x,int max_x,int max_y);
static BOOL is_collision_block(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block);
static BOOL can_move_block(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block,int y, int x);
static int add_block_to_grid(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block);
static int  remove_block_layer(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block);
static int get_remove_layer_num(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block);
static int remove_layer(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block, int layer);
static int remove_layer_from_grid(grid_t (*pgrid)[TETRIS_WIDTH],int layer_y);
static int set_prompt(const block_t *block, prompt_t *prompt, int remove_layer);


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
	int type;
	int num;
	int ret;
	block_t tmp;
	
	type=rand()%BLOCK_TYPE_NUM;
	num=rand()%BLOCK_SINGLE_MAX_NUM;
	switch(type){
		case BLOCK_O:
			num=0;
			break;
		case BLOCK_I:
		case BLOCK_S:
		case BLOCK_Z:
			num=num%2;
			break;
		case BLOCK_J:
		case BLOCK_L:
		case BLOCK_T:
			num=num;
			break;
		default:break;
	}
	
	tmp.type=type;
	tmp.number=num;
	ret=get_num_from_block(&tmp);

	return ret;
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

static block_t *rotate_block(grid_t (*pgrid)[TETRIS_WIDTH], block_t *block)
{

	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	block_t *bck=block;
	block_t *pbck=NULL;
	block_t tmp;

	pbck=get_next_rotate_block(&tmp,bck);
	if(NULL==pbck){
		return NULL;
	}
	if(can_move_block(pg,pbck,pbck->y,pbck->x)==TRUE){
		block_copy(bck,&tmp);
		return bck;
	}

	return NULL;
}

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

static block_t *move_block(grid_t (*pgrid)[TETRIS_WIDTH], block_t *block, dir_t direction)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	block_t *bck=block;
	dir_t dir=direction;

	if(NULL==pg||NULL==bck){
		return NULL;
	}
	
	switch(dir){
		case DIR_LEFT:
			if(can_move_block(pg,bck,bck->y,bck->x-1)==TRUE){
				bck->x--;
				return bck;
			}
			break;

		case DIR_RIGHT:
			if(can_move_block(pg,bck,bck->y,bck->x+1)==TRUE){
				bck->x++;
				return bck;
			}
			break;

		case DIR_DOWN:
			if(can_move_block(pg,bck,bck->y+1,bck->x)==TRUE){
				bck->y++;
				return bck;
			}
			break;
			
		default:
			break;
	}
	
	return NULL;
}

static BOOL is_collision_wall(const block_t *block,int min_x,int max_x,int max_y)
{
	const block_t *bck=block;
	if(NULL==bck){
		return TRUE;
	}

	if(bck->x+min_x-1<1||bck->x+max_x-1>TETRIS_WIDTH||bck->y+max_y-1>TETRIS_HEIGHT){
		return TRUE;
	}
	
	return FALSE;
}

static BOOL is_collision_block(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	const block_t *bck=block;
	int y,x;
	if(NULL==pg||NULL==bck){
		return TRUE;
	}
	
	for(y=GRID_LEN-1; y>=0; y--){
		for(x=GRID_LEN-1; x>=0; x--){
			if(bck->blck[y*GRID_LEN+x]=='1'){
//no necessary to judge the range of (bck->x+x-1),because it not less than zero when the condition is fulfilled.			
				if(bck->y+y-1>=0&&pg[bck->y+y-1][bck->x+x-1].value=='1'){
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

static BOOL can_move_block(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block,int y, int x)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	const block_t *bck=block;
	block_t tmp;
	block_copy(&tmp,bck);
	tmp.y=y;
	tmp.x=x;
	
	if(NULL==pg||NULL==bck){
		return FALSE;
	}
#if 0
	//This code must not be added
	if(tmp.y==bck->y&&tmp.x==bck->x){
		return TRUE;
	}
#endif

	switch(bck->type){
		case BLOCK_I:
			switch(bck->number){
				case 0:
					if(is_collision_wall(&tmp,BLOCK_I0_MIN_X,BLOCK_I0_MAX_X,BLOCK_I0_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case BLOCK_I_NUM-1:
					if(is_collision_wall(&tmp,BLOCK_I1_MIN_X,BLOCK_I1_MAX_X,BLOCK_I1_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
			}
			break;
		case BLOCK_J:
			switch(bck->number){
				case 0:
					if(is_collision_wall(&tmp,BLOCK_J0_MIN_X,BLOCK_J0_MAX_X,BLOCK_J0_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case 1:
					if(is_collision_wall(&tmp,BLOCK_J1_MIN_X,BLOCK_J1_MAX_X,BLOCK_J1_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case 2:
					if(is_collision_wall(&tmp,BLOCK_J2_MIN_X,BLOCK_J2_MAX_X,BLOCK_J2_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case BLOCK_J_NUM-1:
					if(is_collision_wall(&tmp,BLOCK_J3_MIN_X,BLOCK_J3_MAX_X,BLOCK_J3_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
			}
			break;
		case BLOCK_L:
			switch(bck->number){
				case 0:
					if(is_collision_wall(&tmp,BLOCK_L0_MIN_X,BLOCK_L0_MAX_X,BLOCK_L0_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case 1:
					if(is_collision_wall(&tmp,BLOCK_L1_MIN_X,BLOCK_L1_MAX_X,BLOCK_L1_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case 2:
					if(is_collision_wall(&tmp,BLOCK_L2_MIN_X,BLOCK_L2_MAX_X,BLOCK_L2_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case BLOCK_L_NUM-1:
					if(is_collision_wall(&tmp,BLOCK_L3_MIN_X,BLOCK_L3_MAX_X,BLOCK_L3_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
			}
			break;
		case BLOCK_O:
			if(is_collision_wall(&tmp,BLOCK_O0_MIN_X,BLOCK_O0_MAX_X,BLOCK_O0_MAX_Y)==TRUE||\
				is_collision_block(pg,&tmp)==TRUE){
					return FALSE;
			}
			break;
		case BLOCK_S:
			switch(bck->number){
				case 0:
					if(is_collision_wall(&tmp,BLOCK_S0_MIN_X,BLOCK_S0_MAX_X,BLOCK_S0_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case BLOCK_S_NUM-1:
					if(is_collision_wall(&tmp,BLOCK_S1_MIN_X,BLOCK_S1_MAX_X,BLOCK_S1_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
			}
			break;
		case BLOCK_Z:
			switch(bck->number){
				case 0:
					if(is_collision_wall(&tmp,BLOCK_Z0_MIN_X,BLOCK_Z0_MAX_X,BLOCK_Z0_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case BLOCK_Z_NUM-1:
					if(is_collision_wall(&tmp,BLOCK_Z1_MIN_X,BLOCK_Z1_MAX_X,BLOCK_Z1_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
			}
			break;
		case BLOCK_T:
			switch(bck->number){
				case 0:
					if(is_collision_wall(&tmp,BLOCK_T0_MIN_X,BLOCK_T0_MAX_X,BLOCK_T0_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case 1:
					if(is_collision_wall(&tmp,BLOCK_T1_MIN_X,BLOCK_T1_MAX_X,BLOCK_T1_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case 2:
					if(is_collision_wall(&tmp,BLOCK_T2_MIN_X,BLOCK_T2_MAX_X,BLOCK_T2_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
				case BLOCK_T_NUM-1:
					if(is_collision_wall(&tmp,BLOCK_T3_MIN_X,BLOCK_T3_MAX_X,BLOCK_T3_MAX_Y)==TRUE||\
						is_collision_block(pg,&tmp)==TRUE){
						return FALSE;
					}
					break;
			}
			break;
		default:
			break;
	}
	
	return TRUE;
}

static int add_block_to_grid(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	const block_t *bck=block;
	if(NULL==pg||NULL==bck){
		return TTRS_FAILED;
	}
	int x=0;
	int y=0;
	if(bck->y+y-1>=0){
		for(y=0; y<GRID_LEN; y++){
			for(x=0; x<GRID_LEN; x++){
				if(bck->blck[y*GRID_LEN+x]=='1'&&pg[bck->y+y-1][bck->x+x-1].value!='1'){
						pg[bck->y+y-1][bck->x+x-1].value='1';
						pg[bck->y+y-1][bck->x+x-1].type=bck->type;
				}
			}
		}
	}else{//Block quickly filled case
		return TTRS_FAILED;
	}
	
	return TTRS_SUCCESS;
}

static int  remove_block_layer(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	const block_t *bck=block;
	int rm_layer=0;
	int num=0;
	if(NULL==pg||NULL==bck){
		return TTRS_FAILED;
	}  
	num=get_remove_layer_num(pg,bck);

	if(num!=0){
		remove_layer(pg,bck,num);
	}
	
	do{
		if((num&1)==1){
			rm_layer++;
		}
	}while((num=num>>1)>0);

	return rm_layer;
}

static int get_remove_layer_num(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	const block_t *bck=block;
	int ret_num=0;//Binary number:0000~1111
	int x,y;
	if(NULL==pg||NULL==bck){
		return TTRS_FAILED;
	}  
	int max_y=(bck->y-1+GRID_LEN)>(TETRIS_HEIGHT)?(TETRIS_HEIGHT):(bck->y-1+GRID_LEN);
	for(y=bck->y-1; y<max_y; y++){
		for(x=0;x<TETRIS_WIDTH;x++){
			if(pg[y][x].value!='1'){
				break;
			}else {
				if(x==TETRIS_WIDTH-1){
					ret_num += 1<<(y-(bck->y-1));
				}
			}
			
		}
	}

	return ret_num;
}

static int remove_layer(grid_t (*pgrid)[TETRIS_WIDTH], const block_t *block, int layer)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	const block_t *bck=block;
	int layer_num=layer;
	if(NULL==pg||NULL==bck){
		return TTRS_FAILED;
	}
	int y;
	
	y=bck->y-1;
	if(((y+0)<TETRIS_HEIGHT)&&(((layer_num>>0)&1)==1)){
		remove_layer_from_grid(pg,y+1);
	}

	if(((y+1)<TETRIS_HEIGHT)&&(((layer_num>>1)&1)==1)){
		remove_layer_from_grid(pg,y+1);
	}
	
	if(((y+2)<TETRIS_HEIGHT)&&(((layer_num>>2)&1)==1)){
		remove_layer_from_grid(pg,y+2);
	}
	
	if(((y+3)<TETRIS_HEIGHT)&&(((layer_num>>3)&1)==1)){
		remove_layer_from_grid(pg,y+3);
	}
	
	return TTRS_SUCCESS;
}

static int remove_layer_from_grid(grid_t (*pgrid)[TETRIS_WIDTH],int layer_y)
{
	grid_t (*pg)[TETRIS_WIDTH]=pgrid;
	if(NULL==pg){
		return TTRS_FAILED;
	}

	int y,x;
	y=layer_y;
	for(y=layer_y; y>0; y--){
		for(x=0; x<TETRIS_WIDTH; x++){
			pg[y][x].value=pg[y-1][x].value;
		}
	}

	for(x=0; x<TETRIS_WIDTH; x++){
		pg[y][x].value='0';	//y=0
	}
	
	return TTRS_SUCCESS;
}

static int set_prompt(const block_t *block, prompt_t *prompt, int remove_layer)
{
	const block_t *bck=block;
	prompt_t *ppt=prompt;
	int rm_layer=(remove_layer>=REMOVE_MIN_LAYER&&remove_layer<=REMOVE_MAX_LAYER)?(remove_layer):(REMOVE_MIN_LAYER);
	
	if(NULL==ppt||NULL==bck){
		return TTRS_FAILED;
	}
	
	ppt->lines_value  += rm_layer;
	switch(rm_layer){
		case REMOVE_MIN_LAYER:	
			switch(bck->type){
				case BLOCK_I:
					switch(bck->number){
						case 0:
							ppt->score_value += (BLOCK_I0_MAX_Y-BLOCK_I0_MIN_Y+1);
							break;
						case BLOCK_I_NUM-1:
							ppt->score_value += (BLOCK_I1_MAX_Y-BLOCK_I1_MAX_Y+1);
							break;
					}
					break;
				case BLOCK_J:
					switch(bck->number){
						case 0:
							ppt->score_value += (BLOCK_J0_MAX_Y-BLOCK_J0_MIN_Y+1);
							break;
						case 1:
							ppt->score_value += (BLOCK_J1_MAX_Y-BLOCK_J1_MIN_Y+1);
							break;
						case 2:
							ppt->score_value += (BLOCK_J2_MAX_Y-BLOCK_J2_MIN_Y+1);
							break;
						case BLOCK_J_NUM-1:
							ppt->score_value += (BLOCK_J3_MAX_Y-BLOCK_J3_MIN_Y+1);
							break;
					}
					break;
				case BLOCK_L:
					switch(bck->number){
						case 0:
							ppt->score_value += (BLOCK_L0_MAX_Y-BLOCK_L0_MIN_Y+1);
							break;
						case 1:
							ppt->score_value += (BLOCK_L1_MAX_Y-BLOCK_L1_MIN_Y+1);
							break;
						case 2:
							ppt->score_value += (BLOCK_L2_MAX_Y-BLOCK_L2_MIN_Y+1);
							break;
						case BLOCK_L_NUM-1:
							ppt->score_value += (BLOCK_L3_MAX_Y-BLOCK_L3_MIN_Y+1);
							break;
					}
					break;
				case BLOCK_O:
					ppt->score_value += (BLOCK_O0_MAX_Y-BLOCK_O0_MIN_Y+1);
					break;
				case BLOCK_S:
					switch(bck->number){
						case 0:
							ppt->score_value += (BLOCK_S0_MAX_Y-BLOCK_S0_MIN_Y+1);
							break;
						case BLOCK_S_NUM-1:
							ppt->score_value += (BLOCK_S1_MAX_Y-BLOCK_S1_MIN_Y+1);
							break;
					}
					break;
				case BLOCK_Z:
					switch(bck->number){
						case 0:
							ppt->score_value += (BLOCK_Z0_MAX_Y-BLOCK_Z0_MIN_Y+1);
							break;
						case BLOCK_Z_NUM-1:
							ppt->score_value += (BLOCK_Z1_MAX_Y-BLOCK_Z1_MIN_Y+1);
							break;
					}
					break;
				case BLOCK_T:
					switch(bck->number){
						case 0:
							ppt->score_value += (BLOCK_T0_MAX_Y-BLOCK_T0_MIN_Y+1);
							break;
						case 1:
							ppt->score_value += (BLOCK_T1_MAX_Y-BLOCK_T1_MIN_Y+1);
							break;
						case 2:
							ppt->score_value += (BLOCK_T2_MAX_Y-BLOCK_T2_MIN_Y+1);
							break;
						case BLOCK_T_NUM-1:
							ppt->score_value += (BLOCK_T3_MAX_Y-BLOCK_T3_MIN_Y+1);
							break;
					}
					break;
				default:
					break;
			}
			break;
		case 1:
			ppt->score_value += 1*(ppt->level_value+REMOVE_MAX_LAYER);
			break;
		case 2:
			ppt->score_value += 2*(ppt->level_value+REMOVE_MAX_LAYER);
			break;
		case 3:
			ppt->score_value += 3*(ppt->level_value+REMOVE_MAX_LAYER);
			break;
		case REMOVE_MAX_LAYER:
			ppt->score_value += 4*(ppt->level_value+REMOVE_MAX_LAYER);
			break;
		default:
			break;
		}

		ppt->level_value = ((ppt->lines_value/(TETRIS_HEIGHT+ppt->level_value))==ppt->level_value)
			?(ppt->level_value):(ppt->lines_value/(TETRIS_HEIGHT+ppt->level_value));
	return TTRS_SUCCESS;
}

void interrupt_info(int sign)
{
	char info_key[]="you can exit the program by press key  'ESC','q','Q',";
	char info_mouse[]="exit the program by mouse click 'QUIT'.";
	
	int scr_y,scr_x;
	getmaxyx(stdscr,scr_y,scr_x);
	if(has_colors()){attron(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}
	mvwprintw(stdscr,scr_y/2,((scr_x-strlen(info_key))/2>0)?((scr_x-strlen(info_key))/2):(1),"%s",info_key);
	mvwprintw(stdscr,scr_y/2+1,((scr_x-strlen(info_mouse))/2>0)?((scr_x-strlen(info_mouse))/2):(1),"%s",info_mouse);
	if(has_colors()){attroff(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}

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

	switch(cur->type){
		case BLOCK_I:
			switch(cur->number){
				case 0:
					cur->y = cur->y+GRID_LEN-BLOCK_I0_MAX_Y;
					break;
				case BLOCK_I_NUM-1:
					cur->y = cur->y+GRID_LEN-BLOCK_I1_MAX_Y;
					break;
			}
			break;
		case BLOCK_J:
			switch(cur->number){
				case 0:
					cur->y = cur->y+GRID_LEN-BLOCK_J0_MAX_Y;
					break;
				case 1:
					cur->y = cur->y+GRID_LEN-BLOCK_J1_MAX_Y;
					break;
				case 2:
					cur->y = cur->y+GRID_LEN-BLOCK_J2_MAX_Y;
					break;
				case BLOCK_J_NUM-1:
					cur->y = cur->y+GRID_LEN-BLOCK_J3_MAX_Y;
					break;
			}
			break;
		case BLOCK_L:
			switch(cur->number){
				case 0:
					cur->y = cur->y+GRID_LEN-BLOCK_L0_MAX_Y;
					break;
				case 1:
					cur->y = cur->y+GRID_LEN-BLOCK_L1_MAX_Y;
					break;
				case 2:
					cur->y = cur->y+GRID_LEN-BLOCK_L2_MAX_Y;
					break;
				case BLOCK_L_NUM-1:
					cur->y = cur->y+GRID_LEN-BLOCK_L3_MAX_Y;
					break;
			}
			break;
		case BLOCK_O:
			cur->y=cur->y+GRID_LEN-BLOCK_O0_MAX_Y;
			break;
		case BLOCK_S:
			switch(cur->number){
				case 0:
					cur->y = cur->y+GRID_LEN-BLOCK_S0_MAX_Y;
					break;
				case BLOCK_S_NUM-1:
					cur->y = cur->y+GRID_LEN-BLOCK_S1_MAX_Y;
					break;
			}
			break;
		case BLOCK_Z:
			switch(cur->number){
				case 0:
					cur->y = cur->y+GRID_LEN-BLOCK_Z0_MAX_Y;
					break;
				case BLOCK_Z_NUM-1:
					cur->y = cur->y+GRID_LEN-BLOCK_Z1_MAX_Y;
					break;
			}
			break;
		case BLOCK_T:
			switch(cur->number){
				case 0:
					cur->y = cur->y+GRID_LEN-BLOCK_T0_MAX_Y;
					break;
				case 1:
					cur->y = cur->y+GRID_LEN-BLOCK_T1_MAX_Y;
					break;
				case 2:
					cur->y = cur->y+GRID_LEN-BLOCK_T2_MAX_Y;
					break;
				case BLOCK_T_NUM-1:
					cur->y = cur->y+GRID_LEN-BLOCK_T3_MAX_Y;
					break;
			}
			break;
		default:
			break;
	}
	
	return cur;
}

int handle_tetris(tetris_t *tetris)
{
	tetris_t *ttrs=tetris;
	block_t *bck=NULL;
	int num=0;
	char lose[]="Sorry, you lose!";
	int ret;
	
	pthread_mutex_lock(&mutex);
	bck=move_block(&ttrs->grid[0],&ttrs->cur_block,DIR_DOWN);
	if(NULL==bck){
		ret=add_block_to_grid(&ttrs->grid[0],&ttrs->cur_block);
		if(TTRS_FAILED==ret){
			ttrs->status=STATUS_QUIT;
			if(has_colors()){attron(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}
			mvwprintw(ttrs->scr.win,ttrs->scr.begin_y+ttrs->scr.nlines/2,
				ttrs->scr.begin_x+ttrs->scr.ncols/2-strlen(lose)/2,"%s",lose);
			if(has_colors()){attroff(COLOR_PAIR(COLOR_NORMAL_PROMPT)|A_BOLD);}
			wrefresh(ttrs->scr.win);
			nodelay(ttrs->scr.win,FALSE);
			wgetch(ttrs->scr.win);
			nodelay(ttrs->scr.win,TRUE);
		}else{
			num=remove_block_layer(&ttrs->grid[0],&ttrs->cur_block);
			if(num>-1){
				set_prompt(&ttrs->cur_block,&ttrs->prompt,num);
			}
		}
		current_block(&ttrs->scr,&ttrs->cur_block,&ttrs->next_block);
		next_block(&ttrs->scr,&ttrs->next_block);
	}
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
			if(STATUS_PAUSE!=*sts){
				pthread_mutex_lock(&mutex);
				rotate_block(pg,bck);
				pthread_mutex_unlock(&mutex);
			}
			break;
			
		case KEY_RIGHT://Right
			if(STATUS_PAUSE!=*sts){
				pthread_mutex_lock(&mutex);
				move_block(pg,bck,DIR_RIGHT);
				pthread_mutex_unlock(&mutex);
			}
			
			break;
			
		case KEY_DOWN://Down
			if(STATUS_PAUSE!=*sts){
				pthread_mutex_lock(&mutex);
				move_block(pg,bck,DIR_DOWN);
				pthread_mutex_unlock(&mutex);
			}
			
			break;
			
		case KEY_LEFT://Left
			if(STATUS_PAUSE!=*sts){
				pthread_mutex_lock(&mutex);
				move_block(pg,bck,DIR_LEFT);
				pthread_mutex_unlock(&mutex);
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


