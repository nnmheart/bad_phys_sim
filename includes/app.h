#pragma once
#include<stdbool.h>
#include<SDL2/SDL.h>
#include<point.h>

#define print(...) printf("[%s]: ", __func__); printf(__VA_ARGS__);

typedef struct App {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool game_running;

    Point* points;
    int used_points;
    int max_points;

    int screen_width;
    int screen_height;
} App;