#include <stdlib.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT (WINDOW_WIDTH)



void get_centered(SDL_Surface *surface, SDL_Rect *rect) {
    rect->x = rect->x + ((rect->w - surface->w)/2);
    rect->y = rect->y + ((rect->h - surface->h)/2);
    rect->w = surface->w;
    rect->h = surface->h;

}

void get_labels(SDL_Renderer *renderer, char *text, TTF_Font *font, 
    SDL_Color color, SDL_Texture **texture, SDL_Rect *rect) {
    SDL_Surface *surface;
    SDL_Color textColor = color;
    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);

    get_centered(surface, rect);
    SDL_FreeSurface(surface);
}

void get_data_range(int cols, int rows, int **heatMap, double range[3]) {
    const int min = 0;
    const int avg = 1;
    const int max = 2;

    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            if (heatMap[i][j] < range[min]) {
                range[min] = (double)heatMap[i][j];
            }
            if (heatMap[i][j] > range[max]) {
                range[max] = (double)heatMap[i][j];
            }
            range[avg] = range[avg] + (double)heatMap[i][j];
        }
    }
    range[avg] = range[avg]/(double)(cols*rows);
}

void get_coloring(int spectrum[3][4], double point, double range[3], SDL_Color * coloring) {
    const int min = 0;
    const int avg = 1;
    const int max = 2;

    int floor;
    int ceiling;

    double ratio;

    int rgb[3];

    if (point < range[avg]) {
        floor = min;
        ceiling = avg;
    } else {
        floor = avg;
        ceiling = max;
    }

    ratio = (point - range[floor])/(range[ceiling]-range[floor]);

    /*
    std::cout << "Min: " + std::to_string(range[floor]) + ", Max: " + std::to_string(range[ceiling])
             + ", Num: " + std::to_string(point) + ", Ratio: " + std::to_string(ratio) << std::endl;
    */

    for (int i = 0; i < 3; i++) {
        rgb[i] = ((int)((double)(spectrum[ceiling][i]-spectrum[floor][i]))*ratio)
                                    + spectrum[floor][i];
                                    //+std::min(spectrum[floor][i], spectrum[ceiling][i]);
        /*
        std::cout << "MinC: " + std::to_string(spectrum[floor][i]) + ", MaxC: " 
            + std::to_string(spectrum[ceiling][i]) + ", Ratio: " + std::to_string(ratio) 
            + ", Col: " + std::to_string(rgb[i])  << std::endl;
        */
    }

    coloring->r = rgb[0];
    coloring->g = rgb[1];
    coloring->b = rgb[2];
    coloring->a = 0;
}

/**
 * Draws a col x row grid with basic alternate coloring
 * if heatMap array supplied, interpolates the colors to form heat map
 **/
void get_board(SDL_Renderer *renderer, int margin, int cols, int rows,
        int spectrum[3][4], TTF_Font *font, SDL_Texture **texture,
        int **heatMap = NULL) {
    int size = (WINDOW_WIDTH-(2*margin))/8;
    SDL_Rect rect;
    SDL_Color fontColor = {0, 0, 0, 0};
    SDL_Color backColor;
    char num_char[2 + sizeof(char)];
    double data_range[3];

    get_data_range(cols, rows, heatMap, data_range);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rect.x = margin+(j*size);
            rect.y = margin+(i*size);
            rect.w = size;
            rect.h = size;

            get_coloring(spectrum, (double)heatMap[i][j], data_range, &backColor);
            SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
            
            SDL_RenderFillRect(renderer, &rect);
            
            std::sprintf(num_char, "%d", heatMap[i][j]);
            get_labels(renderer, num_char, font, fontColor, texture, &rect);
            SDL_RenderCopy(renderer, *texture, NULL, &rect);
        }
    }
}


int main(int argc, char **argv) {
    SDL_Event event;

    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Window *window;
    char *font_path;
    int quit;

    if (argc == 1) {
        font_path = "FreeSans.ttf";
    } else if (argc == 2) {
        font_path = argv[1];
    } else {
        fprintf(stderr, "error: too many arguments\n");
        exit(EXIT_FAILURE);
    }

    /* Inint TTF. */
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont(font_path, 24);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        exit(EXIT_FAILURE);
    }
    
    int spectrum[3][4] = {{0, 100, 0, 0},{255, 255, 0, 0},{139, 0, 0, 0}};
    /*
    // Green
    spectrum[0] = {0, 100, 0, 0};
    // Yellow
    spectrum[1] = {255, 255, 0, 0};
    // Red
    spectrum[2] = {139, 0, 0, 0};
    */

    int **heat;
    heat = new int *[8];
    for(int i = 0; i <8; i++) {
        heat[i] = new int[8];
        for (int j = 0; j < 8; j++) {
            heat[i][j] =  (i*10)+j;
        }
    }

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        
        SDL_RenderClear(renderer);

        /* Use TTF textures. */
        get_board(renderer, 10, 8, 8, spectrum, font, &texture, heat);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderPresent(renderer);
    }

    /* Deinit TTF. */
    SDL_DestroyTexture(texture);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}