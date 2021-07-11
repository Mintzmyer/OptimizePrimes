#ifndef DRAWBOARD_H 
#define DRAWBOARD_H

#include <stdlib.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void get_centered(SDL_Surface *surface, SDL_Rect *rect);
void get_labels(SDL_Renderer *renderer, char *text, TTF_Font *font, 
    SDL_Color color, SDL_Texture **texture, SDL_Rect *rect);

void get_data_range(int cols, int rows, int **heatMap, double range[3]);

void get_coloring(int spectrum[3][4], double point, double range[3], SDL_Color * coloring);

/**
 * Draws a col x row grid with basic alternate coloring
 * if heatMap array supplied, interpolates the colors to form heat map
 **/
void get_board(SDL_Renderer *renderer, int margin, int cols, int rows,
        int spectrum[3][4], TTF_Font *font, SDL_Texture **texture, char * piece,
        int **heatMap = NULL);

int draw(char* piece, int margin, int rows, int cols, int** map);

#endif /* DRAWBOARD_H */