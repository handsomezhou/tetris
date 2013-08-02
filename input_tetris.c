/**
  *  Copyright (C) 2013  Handsome Zhou
  */
#include <unistd.h>
#include "input_tetris.h"
#include "handle_tetris.h"

void *input_tetris(void *arg)
{
	tetris_t *ttrs=(tetris_t *)arg;
	if(NULL==ttrs){
		return NULL;
	}

	int ch=-1;
	MEVENT mevent;
	
	//deal with key and mouse event
	mousemask(ALL_MOUSE_EVENTS,NULL);
	while(ttrs->status!=STATUS_QUIT){
		ch=-1;
		ch=wgetch(ttrs->scr.win);
		if(-1!=ch){//no key and mouse event
			if(KEY_MOUSE==ch){
				if(getmouse(&mevent)!=OK){
					return NULL;
				}else{
					switch(mevent.bstate){
						case BUTTON1_CLICKED:
						case BUTTON1_TRIPLE_CLICKED:
							deal_mouse_event(&ttrs->scr,&ttrs->status,&mevent);
							break;
						default:
							break;

					}
				}
			}else{//key event
				deal_key_event(&ttrs->grid[0],&ttrs->cur_block,&ttrs->status,ch);
			}
		}
		usleep(ttrs->min_unit_time);
	}
	
	return ttrs;
}

