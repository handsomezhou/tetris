/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "tetris.h"
#include "output_tetris.h"
#include "input_tetris.h"
#include "handle_tetris.h"

void quit(int sign);
void quit(int sign)
{
	int scr_y,scr_x;
	getmaxyx(stdscr,scr_y,scr_x);
	nodelay(stdscr,FALSE);
	wgetch(stdscr);
	nodelay(stdscr,TRUE);
}

int main(int argc, char *argv[])
{
	tetris_t *tetris=NULL;
	int ret=TTRS_FAILED;
	pthread_t tid_input=-1;
	
	srand((unsigned int)(time(NULL)));
	ret=init_tetris(&tetris);
	if(TTRS_FAILED==ret){
		exit_tetris(tetris);
		
		return TTRS_FAILED;
	}
	signal(SIGINT,quit);
	//signal(SIGUSR1,quit);
	paint_tetris(tetris);
	if(pthread_create(&tid_input,NULL,input_tetris,(void *)tetris)!=0){
		exit_tetris(tetris);
		perror("pthread_create() for tid_input");
		
		return TTRS_FAILED;
	}
	
	while(tetris->status!=STATUS_QUIT){
		if(STATUS_PAUSE!=tetris->status){
			handle_tetris(tetris);
		}
		paint_tetris(tetris);
		usleep(1200000);
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
