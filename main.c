#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "tetris.h"
#include "output_tetris.h"

int main(int argc, char *argv[])
{
	tetris_t *tetris=NULL;
	srand((unsigned int)(time(NULL)));
	
	init_tetris(&tetris);
	
	while(tetris->status!=STATUS_QUIT){
		paint_tetris(tetris);
		usleep(500000);
	}
	
	exit_tetris(tetris);
	
	return 0;
}
