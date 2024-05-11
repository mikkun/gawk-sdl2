#! /usr/bin/gawk -f

# SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
# SPDX-License-Identifier: GPL-3.0-or-later

@load "sdl2"

BEGIN {
    WINDOW_W = 480
    WINDOW_H = 320
    WAIT     = 10

    srand()
    SDL_Init(SDL_INIT_VIDEO)
    window = SDL_CreateWindow("gawk-sdl2 - testsdl2.awk",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_W, WINDOW_H,
                              0)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE)

    event = SDL_Gawk_CreateEvent()
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)
    SDL_RenderClear(renderer)

    quit = 0
    while (! quit) {
        while (SDL_PollEvent(event)) {
            event_type = SDL_Gawk_GetEventType(event)
            if (event_type == SDL_QUIT) {
                quit = 1
            }
        }

        r = int(rand() * 256)
        g = int(rand() * 256)
        b = int(rand() * 256)
        SDL_SetRenderDrawColor(renderer, r, g, b, 255)

        x1 = int(rand() * WINDOW_W)
        y1 = int(rand() * WINDOW_H)
        x2 = int(rand() * WINDOW_W)
        y2 = int(rand() * WINDOW_H)
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2)

        SDL_RenderPresent(renderer)
        SDL_Delay(WAIT)
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "SDL_ShowSimpleMessageBox",
                             "Informational Dialog",
                             window)
    SDL_DestroyRenderer(renderer)
    SDL_DestroyWindow(window)
    SDL_Quit()
    exit 0
}
