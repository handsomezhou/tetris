#ifndef HANDLE_TETRIS_H
#define HANDLE_TETRIS_H

#include "tetris.h"

block_t *next_block(const screen_t *screen,block_t *block);
block_t *current_block(const screen_t *screen,block_t *current,const block_t *next);


#endif	/*HANDLE_TETRIS_H*/
