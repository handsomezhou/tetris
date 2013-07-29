/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#ifndef HANDLE_TETRIS_H
#define HANDLE_TETRIS_H

#include "tetris.h"

void interrupt_info(int sign);
block_t *next_block(const screen_t *screen,block_t *block);
block_t *current_block(const screen_t *screen,block_t *current,const block_t *next);
int handle_tetris(tetris_t *tetris);
int deal_key_event(grid_t (*pgrid)[TETRIS_WIDTH],block_t *block, status_t *status, int key);
int deal_mouse_event(const screen_t *screen, status_t *status,const MEVENT *mevent);
block_t *get_next_rotate_block(block_t *next_block, const block_t *cur_block);


#endif	/*HANDLE_TETRIS_H*/
