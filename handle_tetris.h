/**
  *  Copyright (C) 2013  Handsome Zhou
  */

#ifndef HANDLE_TETRIS_H
#define HANDLE_TETRIS_H

#include "tetris.h"

block_t *next_block(const screen_t *screen,block_t *block);
block_t *current_block(const screen_t *screen,block_t *current,const block_t *next);
int handle_tetris(tetris_t *tetris);
int deal_key_event(block_t *block, status_t *status, int key);
int deal_mouse_event(const screen_t *screen, status_t *status,const MEVENT *mevent);

#endif	/*HANDLE_TETRIS_H*/
