#! /usr/bin/gawk -f

# SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
# SPDX-License-Identifier: GPL-3.0-or-later

@load "sdl2"

BEGIN {
    PROCINFO["sorted_in"] = "@ind_str_asc"

    WINDOW_W = 300
    WINDOW_H = 300
    WAIT     = 10

    SDL_Init(SDL_INIT_VIDEO)
    window = SDL_CreateWindow("gawk-sdl2 - struct2array.awk",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_W, WINDOW_H,
                              0)
    renderer = SDL_CreateRenderer(window, -1, 0)
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_TARGET,
                                WINDOW_W, WINDOW_H)

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)
    SDL_RenderClear(renderer)

    rect = SDL_Gawk_AllocRect()
    SDL_Gawk_UpdateRect(rect, 100, 100, 100, 100)

    surface = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0)
    SDL_FillRect(surface, rect, 0)

    SDL_RenderCopy(renderer, texture, NULL, rect)
    SDL_RenderPresent(renderer)

    SDL_Gawk_SurfaceToArray(surface, array_of["surface"])
    for (field in array_of["surface"]) {
        message = message sprintf("array_of[\"surface\"][\"%s\"] = %s\n",
                                  field, array_of["surface"][field])
    }

    SDL_Gawk_PixelFormatToArray(array_of["surface"]["format"],
                                array_of["fmt"])
    for (field in array_of["fmt"]) {
        message = message sprintf("array_of[\"fmt\"][\"%s\"] = %s\n",
                                  field, array_of["fmt"][field])
    }

    event = SDL_Gawk_AllocEvent()
    printf "%s", message
    print "\033[1;34mPress [q] or [ESC] to quit\033[0m"

    quit = 0
    while (! quit) {
        while (SDL_PollEvent(event)) {
            event_type = SDL_Gawk_GetEventType(event)
            if (event_type == SDL_QUIT) {
                quit = 1
            }
        }

        SDL_Gawk_GetKeyboardState(NULL, curr_state)
        if (curr_state[SDL_SCANCODE_Q] || curr_state[SDL_SCANCODE_ESCAPE]) {
            quit = 1
        }

        SDL_Delay(WAIT)
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "Struct to Assoc Array",
                             message,
                             window)
    SDL_DestroyRenderer(renderer)
    SDL_DestroyWindow(window)
    SDL_Quit()
    exit 0
}
