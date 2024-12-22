#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<math.h>
#include<malloc.h>
#include<SDL2/SDL.h>
#include<point.h>
#include<app.h>

App* app;
SDL_Point areas[5];
SDL_Point arrow_points[4];

double electric_constant = 10000; //8990000000.0; // 8.99 x 10^9
double gravity_constant  =  1000; //0.00000000006674; // 6.674 x 10^−11
const uint8_t background_color[3] = {0x50, 0x50, 0x50};

void app_init_sdl() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        print("SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    app->window = SDL_CreateWindow(
        "Physics for fun",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        app->screen_width,
        app->screen_height,
        0x00
    );
    if (app->window == NULL) {
        print("SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
    if (app->renderer == NULL) {
        print("SDL Error: %s\n", SDL_GetError());
        exit(1);    
    }
}

void app_handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                app->game_running = false;
                break;
            default:
                break;
        }
    }
}

void app_update() {
    for (int i = 0; i < app->used_points; i++) {
        app->points[i].disp.x = 0.0;
        app->points[i].disp.y = 0.0;
    }

    for (int i = 0; i < app->used_points; i++) {
        Point p1 = app->points[i];

        for (int j = 0; j < app->used_points; j++) {
            if (i == j) continue;
            Point p2 = app->points[j];

            double dist = v_dist(p1.pos, p2.pos);
            double ep = 0.1;
            if (((dist-ep) <= 0.0) && (0 <= (dist + ep))) {
                continue;
            }

            double elec_fact = electric_constant * p1.charge * p2.charge;
            Vector elec_unit_v =  v_mul( v_diff(p1.pos, p2.pos), 1 / v_dist(p1.pos, p2.pos));
            Vector elec_inf = v_mul(
                v_mul(elec_unit_v, 1 / v_dist2(p1.pos, p2.pos)),
                elec_fact
            );
            p1.disp.x += elec_inf.x;
            p1.disp.y += elec_inf.y;

            double grav_fact = -1 * gravity_constant * p1.mass * p2.mass;
            Vector grav_unit_v = v_mul(v_diff(p1.pos, p2.pos), 1 / v_dist(p1.pos, p2.pos));
            Vector grav_inf = v_mul(
                v_mul(grav_unit_v, 1 / v_dist2(p1.pos, p2.pos)),
                grav_fact
            );
            p1.disp.x += grav_inf.x;
            p1.disp.y += grav_inf.y;
        }

        app->points[i] = p1;
    }

    for (int i = 0; i < app->used_points; i++) {
        Point p = app->points[i];
        
        double x = p.pos.x + p.disp.x;
        double y = p.pos.y + p.disp.y;

        while (x > app->screen_width) x -= app->screen_width;
        while (x < 0) x += app->screen_width;
        while (y > app->screen_height) y -= app->screen_height;
        while (y < 0) y += app->screen_height;

        p.pos.x = x;
        p.pos.y = y;
        app->points[i] = p;
    }
}

void app_render() {
    SDL_Renderer* renderer = app->renderer;
    SDL_SetRenderDrawColor(
        renderer,
        background_color[0],
        background_color[1],
        background_color[2],
        0xFF
    );
    SDL_RenderClear(renderer);

    for (int i = 0; i < app->used_points; i++) {
        Point p = app->points[i];

        SDL_SetRenderDrawColor(
            renderer,
            p.color[0],
            p.color[1],
            p.color[2],
            0xFF
        );


        int i = 0;
        #define add_area(xo, yo) \
            areas[i].x = (int)(p.pos.x + xo); \
            areas[i].y = (int)(p.pos.y + yo); \
            i++;
        
        double r = p.radius;

        add_area(-r, 0);
        add_area(0, r);
        add_area(r, 0);
        add_area(0, -r);
        add_area(-r, 0);
        SDL_RenderDrawLines(renderer, areas, i);

        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        arrow_points[0].x = (int)(p.pos.x);
        arrow_points[0].y = (int)(p.pos.y);

        arrow_points[1].x = (int)(p.pos.x + 100 * p.disp.x);
        arrow_points[1].y = (int)(p.pos.y + 100 * p.disp.y);
        SDL_RenderDrawLines(renderer, arrow_points, 2);
    }

}

void app_loop() {
    while (app->game_running) {
        app_handle_events();
        app_update();
        app_render();
        SDL_RenderPresent(app->renderer);
        SDL_Delay(1000 / 120);
    }
}

double get_num(char* buffer, int* index) {
    double num = 0;
    double float_part = 0.0;
    bool is_neg = false;
    if (buffer[*index] == '-') {
        is_neg = true;
        (*index)++;
    }
    while (buffer[*index] != '.') {
        num *= 10.0;
        num += (buffer[*index] - '0');
        (*index)++;
    }
    (*index)++;
    while (('0' <= buffer[*index]) && (buffer[*index] <= '9')) {
        float_part /= 10.0;
        float_part += (buffer[*index] - '0');
        (*index)++;
    }
    (*index)++;
    num += float_part;
    if (is_neg) {
        num *= -1;
    }
    return num;
}

int get_proper_num(char* buffer, int* index) {
    int num = 0;
    while (buffer[*index] != ' ') {
        num *= 10;
        num += (buffer[*index] - '0');
        (*index)++;
    }
    (*index)++;
    return num;
}

#define progress() while ((buffer[index] == ' ') || (buffer[index] == '|') || (buffer[index] == '\r') || (buffer[index] == '\n')) (index)++
#define get_numi() get_num(buffer, &index)
#define get_nump() get_proper_num(buffer, &index)

void read_from_file() {
    FILE* fp = fopen("points.txt", "rb");
    if (fp == NULL) {
        print("Error opening file.\n");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = (char*) malloc(size);
    fread(buffer, 1, size, fp);
    fclose(fp);

    int index = 0;
    while (buffer[index] != '=') index++;
    index++;
    while (buffer[index] == ' ') index++;
    electric_constant = get_num(buffer, &index);

    while (buffer[index] != '=') index++;
    index++;
    while (buffer[index] == ' ') index++;
    gravity_constant = get_num(buffer, &index);
    print("Electric constant: %f\n", electric_constant);
    print("Gravity constant: %f\n", gravity_constant);

    while (buffer[index] != 'X') index++;
    while (buffer[index] != '\n') index++;
    index++;

    app->used_points = 0;

    while (index < size) {
        double x = get_numi();
        progress();
        double y = get_numi();
        progress();
        double sdx = get_numi();
        progress();
        double sdy = get_numi();
        progress();
        double mass = get_numi();
        progress();
        double charge = get_numi();
        progress();
        int r = (int)get_nump();
        progress();
        int g = (int)get_nump();
        progress();
        int b = (int)get_nump();
        progress();
        double radius = get_numi();
        progress();

        Point p = app->points[app->used_points++];
        p.pos.x = x;
        p.pos.y = y;

        p.disp.x = sdx;
        p.disp.y = sdy;

        p.mass = mass;
        p.charge = charge;
        p.color[0] = r;
        p.color[1] = g;
        p.color[2] = b;

        p.radius = radius;
        app->points[app->used_points - 1] = p;
    }
}

int main(int argc, char* args[]) {
    app = (App*) malloc(sizeof(App));
    app->game_running = true;
    app->screen_height = 500;
    app->screen_width = 800;
    app->used_points = 0;
    app->max_points = 100;
    app->points = (Point*) malloc(sizeof(Point) * app->max_points);

    read_from_file();

    app_init_sdl();
    print("Initialized SDL.\n");
    print("Entering mainloop.\n");
    app_loop();
}