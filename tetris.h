#ifndef TETRIS_H
#define TETRIS_H

#define	TETRIS_HEIGHT	20
#define TETRIS_WIDTH	10

#define GIRD_LEN	4

#define BLOCK_I_NUM	2
#define BLOCK_J_NUM	4
#define BLOCK_L_NUM	4
#define BLOCK_O_NUM	1
#define BLOCK_S_NUM	2
#define BLOCK_Z_NUM	2
#define BLOCK_T_NUM	4



typedef enum block_type{
	BLOCK_I=0,
	BLOCK_J,
	BLOCK_L,
	BLOCK_O,
	BLOCK_S,
	BLOCK_Z,
	BLOCK_T,
	BLOCK_NONE,
}block_type_t;	//Tetris block type


typedef struct grid{
	int y;
	int x;		//Grid coordinates
	block_type_t type;	//Grid belongs to which block,used to set the grid color
}grid_t;

typedef enum status{
	STATUS_START,
	STATUS_PAUSE,
	STATUS_QUIT,
}status_t;


typedef struct tetris{
	grid_t grid[TETRIS_HEIGHT][TETRIS_WIDTH];
	
	status_t status;
}tetris_t;

#if 0
char lattice[][GIRD_LEN*GIRD_LEN]=
{
		{1,0,0,0,
		 1,0,0,0,
		 1,0,0,0,
		 1,0,0,0,}

		{1,1,1,1,
		 0,0,0,0,
		 0,0,0,0,
		 0,0,0,0,}

}

#endif














#endif	/*TETRIS_H*/
