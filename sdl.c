#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <SDL2/SDL.h>

#define WIDTH 1200
#define HEIGHT 800
#define MANDELBROT_LIMIT 1000.0
#define MAX_ITER 1000
#define OFFSET 100
#define ZOOM 250

// Return number of iterations before escape, or MAX_ITER if inside
int check(double real, double imag) {
    double complex c = real + I * imag;
    double complex z = 0;
    for (int i = 0; i < MAX_ITER; i++) {
        z = z * z + c;
        if (cabs(z) > MANDELBROT_LIMIT) {
            return i; // Escaped at iteration i
        }
    }
    return MAX_ITER; // Inside set
}

// Smooth coloring function (returns Uint32 color)
Uint32 get_color(SDL_PixelFormat* format, int iter) {
    if (iter >= MAX_ITER) {
        return SDL_MapRGB(format, 0, 0, 0); // Inside set: black
    }

    // Smooth gradient calculation
    double t = (double)iter / MAX_ITER;
    Uint8 r = (Uint8)(9 * (1 - t) * t * t * t * 255);
    Uint8 g = (Uint8)(15 * (1 - t) * (1 - t) * t * t * 255);
    Uint8 b = (Uint8)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);

    return SDL_MapRGB(format, r, g, b);
}

void visualize(SDL_Surface* psurface) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            double real = ((double)(x - WIDTH/2 - OFFSET))/ZOOM;
            double imag = -((double)(y - HEIGHT/2))/ZOOM;

            int iter = check(real, imag);
            Uint32 color = get_color(psurface->format, iter);

            SDL_Rect pixel = {x, y, 1, 1};
            SDL_FillRect(psurface, &pixel, color);
        }
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* pwindow = SDL_CreateWindow(
        "Mandelbrot Set",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT, 0
    );
    if (!pwindow) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface* psurface = SDL_GetWindowSurface(pwindow);
    visualize(psurface);
    SDL_UpdateWindowSurface(pwindow);

    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
    }

    SDL_DestroyWindow(pwindow);
    SDL_Quit();
    return 0;
}
