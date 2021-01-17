#include<stdio.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_image.h>

#define WIDTH 800
#define HEIGHT 600
#define SPEED (300)

void clean (SDL_Window *window, SDL_Renderer * render) {
    if (render) {
        SDL_DestroyRenderer(render);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(1);
}

int main () {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
        printf("error init sdl %s\n", SDL_GetError());
    }
    SDL_Window *window = SDL_CreateWindow("sdl cs50", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window) {
        printf("error creat window! %s \n", SDL_GetError());
    }
    Uint32 render_flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flag);
    if (!renderer) {
        printf("error create render %s\n", SDL_GetError());
        clean(window, NULL);
    }
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_Surface *surface = IMG_Load("./icon.png");
    if (!surface) {
        printf("error create surface %s\n", SDL_GetError());
        clean(window, renderer);
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!tex) {
        printf("error create texture %s\n", SDL_GetError());
        clean(window, renderer);
    }

    SDL_RenderClear(renderer);
    // SDL_RenderCopy(renderer, tex, NULL, NULL);

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow color
    SDL_RenderDrawPoint(renderer, 0, 0);
    SDL_RenderDrawPoint(renderer, 1, 1);
    SDL_RenderDrawPoint(renderer, 2, 2);
    // draw_random_points(500, 1, renderer);
    SDL_Rect rect = {50, 50, 200, 200};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
    SDL_Rect rect2 = {250, 300, 200, 250};
    SDL_RenderDrawRect(renderer, &rect2);
    SDL_RenderPresent(renderer);

    enum DemoType {
        animation, shape, keyboard
    };
    const enum DemoType dt = keyboard;
    // struct to hold the position and size of the sprite
    SDL_Rect dest;
    // get the dimensions of texture
    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.x = (WIDTH - dest.w) / 2;
    float x_pos = (WIDTH - dest.w) / 2;
    float y_pos = (HEIGHT - dest.h) / 2;
    // float y_pos = HEIGHT;
    float x_vel = 0;
    float y_vel = 0;

    // keep track of which inputs are given
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    SDL_Event event;
    Uint8 running = 1;
    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){ // click window close button
                running = 0;
            }
            if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym) {
                case SDLK_w:
                case SDLK_UP:
                    up = 1;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    left = 1;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    down = 1;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    right = 1;
                    break;
                case SDLK_ESCAPE:
                    running = 0;
                    break; 
                default:
                    break;
            }
            }
            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                case SDLK_UP:
                    up = 0;
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    left = 0;
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    down = 0;
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
                    right = 0;
                    break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN){
                // running = 0;
            }

        }
        if (dt == keyboard) {
            x_vel = y_vel = 0;
        if (up && !down) y_vel = -SPEED;
        if (down && !up) y_vel = SPEED;
        if (left && !right) x_vel = -SPEED;
        if (right && !left) x_vel = SPEED;

        // update positions
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        // collision detection with bounds
        if (x_pos <= 0) x_pos = 0;
        if (y_pos <= 0) y_pos = 0;
        if (x_pos >= WIDTH - dest.w) x_pos = WIDTH - dest.w;
        if (y_pos >= HEIGHT - dest.h) y_pos = HEIGHT - dest.h;

        // set the positions in the struct
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;
        
        // clear the window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw the image to the window
        SDL_RenderCopy(renderer, tex, NULL, &dest);
        SDL_RenderPresent(renderer);

        // wait 1/60th of a second
        SDL_Delay(1000/60);
        }
        if ((dest.y >= -dest.h) && (dt == animation)) {
        // clear the window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // set the y position in the struct
        dest.y = (int) y_pos;
        // draw the image to the window
        SDL_RenderCopy(renderer, tex, NULL, &dest);
        SDL_RenderPresent(renderer);
        // update sprite position
        y_pos -= (float) SPEED / 60;

        // wait 1/60th of a second
        SDL_Delay(1000/60);
        }
    }
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}