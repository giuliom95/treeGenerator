#ifndef SDL_H
#define SDL_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

#define FULL_SCREEN 0

#define MOUSE 1

int SDL_init();

void SDL_refresh();

void SDL_clear_color_buffer();

void SDL_shut_down();

#endif
