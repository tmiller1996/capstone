#ifndef INPUT_H
#define INPUT_H
#pragma once

typedef struct play_context playctx;

void init_input();
void ctx_poll_input(playctx *ctx);
#endif