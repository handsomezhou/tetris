#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "tetris.h"
#include "output_tetris.h"
#include "handle_tetris.h"

int main(int argc, char *argv[])
{
	tetris_t *tetris=NULL;
	int ret=TTRS_FAILED;
	
	srand((unsigned int)(time(NULL)));
	
	ret=init_tetris(&tetris);
	if(TTRS_FAILED==ret){
		exit_tetris(tetris);
		
		return TTRS_FAILED;
	}
	
	paint_tetris(tetris);
	while(tetris->status!=STATUS_QUIT){
		handle_tetris(tetris);
		paint_tetris(tetris);
	}
	
	exit_tetris(tetris);
	
	return TTRS_SUCCESS;
}
