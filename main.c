/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "tetris.h"
#include "output_tetris.h"
#include "input_tetris.h"
#include "handle_tetris.h"

int main(int argc, char *argv[])
{
	tetris_t *tetris=NULL;
	int ret=TTRS_FAILED;
	int delay_time;
	pthread_t tid_input=-1;
	
	srand((unsigned int)(time(NULL)));
	ret=init_tetris(&tetris);
	if(TTRS_FAILED==ret){
		exit_tetris(tetris);
		
		return TTRS_FAILED;
	}
	
	paint_tetris(tetris);
	if(pthread_create(&tid_input,NULL,input_tetris,(void *)tetris)!=0){
		exit_tetris(tetris);
		perror("pthread_create() for tid_input");
		
		return TTRS_FAILED;
	}
	
	while(tetris->status!=STATUS_QUIT){
		if(STATUS_PAUSE!=tetris->status&&STATUS_CONFIRM_QUIT!=tetris->status&&\
			STATUS_OTHERS!=tetris->status){
			handle_tetris(tetris);
		}
		
		if(STATUS_OTHERS!=tetris->status){
			paint_tetris(tetris);
		}
		
		delay_time=(TIME_MAX_DELAY-tetris->prompt.level_value*TIME_MIN_UNIT);
		usleep((delay_time>0)?(delay_time):(TIME_MIN_UNIT));
	}

	//wait thread tid_input finish
	if(pthread_join(tid_input,NULL)!=0){
		exit_tetris(tetris);
		perror("pthread_join() for tid_input");
		return TTRS_FAILED;
	}
	
	exit_tetris(tetris);
	
	return TTRS_SUCCESS;
}
