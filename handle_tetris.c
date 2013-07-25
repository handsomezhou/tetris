#include <stdlib.h>
#include <string.h>
#include "handle_tetris.h"

static block_t get_block_from_num(int num);
static int get_num_from_block(const block_t *block);
static block_t *block_copy(block_t *dest, const block_t *src);
static int get_random_block();
static block_t *new_block(block_t *block);

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

