#! /usr/bin/gawk -f

# SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
# SPDX-License-Identifier: GPL-3.0-or-later
#
# This code is based on <https://github.com/amroibrahim/DOOMFire>.

@load "sdl2"

BEGIN {
    TITLE       = "gawk-sdl2 - doomfire.awk"
    WINDOW_W    = 200
    WINDOW_H    = 200
    COLOR_DEPTH = 8
    NUM_COLORS  = 39
    WAIT        = 1000 / 60

    srand()
    SDL_Init(SDL_INIT_VIDEO)
    window = SDL_CreateWindow(TITLE,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_W, WINDOW_H,
                              SDL_WINDOW_SHOWN)
    screen_buffer = SDL_CreateRGBSurface(0,
                                         WINDOW_W, WINDOW_H,
                                         COLOR_DEPTH,
                                         0, 0, 0, 0)

    SDL_Gawk_SurfaceToArray(screen_buffer, struct["screen_buffer"])
    SDL_Gawk_PixelFormatToArray(struct["screen_buffer"]["format"],
                                struct["pixel_format"])

    init_color_palette()
    SDL_SetPaletteColors(struct["pixel_format"]["palette"],
                         palette,
                         0, NUM_COLORS)

    init_screen_buffer()
    SDL_FillRect(screen_buffer, NULL, NUM_COLORS - 1)

    event = SDL_Gawk_AllocEvent()
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

        update_screen_buffer()

        window_surface = SDL_GetWindowSurface(window)
        SDL_BlitSurface(screen_buffer, NULL, window_surface, NULL)
        SDL_UpdateWindowSurface(window)

        SDL_Delay(WAIT)
    }

    SDL_FreeSurface(screen_buffer)
    SDL_DestroyWindow(window)
    SDL_Quit()
    exit 0
}

function init_color_palette(    i)
{
    i = 0
    palette = SDL_Gawk_AllocColorPalette(COLOR_DEPTH)

    SDL_Gawk_UpdateColorPalette(palette, i++, 0x00, 0x00, 0x00, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x00, 0x00, 0x00, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x07, 0x07, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x1F, 0x07, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x2F, 0x0F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x47, 0x0F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x57, 0x17, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x67, 0x1F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x77, 0x1F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x8F, 0x27, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0x9F, 0x2F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xAF, 0x3F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xBF, 0x47, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xC7, 0x47, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xDF, 0x4F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xDF, 0x57, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xDF, 0x57, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xD7, 0x5F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xD7, 0x5F, 0x07, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xD7, 0x67, 0x0F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xCF, 0x6F, 0x0F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xCF, 0x77, 0x0F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xCF, 0x7F, 0x0F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xCF, 0x87, 0x17, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xC7, 0x87, 0x17, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xC7, 0x8F, 0x17, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xC7, 0x97, 0x1F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xBF, 0x9F, 0x1F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xBF, 0x9F, 0x1F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xBF, 0xA7, 0x27, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xBF, 0xA7, 0x27, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xBF, 0xAF, 0x2F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xB7, 0xAF, 0x2F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xB7, 0xB7, 0x2F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xB7, 0xB7, 0x37, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xCF, 0xCF, 0x6F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xDF, 0xDF, 0x9F, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xEF, 0xEF, 0xC7, 0xFF)
    SDL_Gawk_UpdateColorPalette(palette, i++, 0xFF, 0xFF, 0xFF, 0xFF)
}

function init_screen_buffer(    x)
{
    for (x = 0; x < WINDOW_W; x++) {
        SDL_Gawk_SetPixelColor(screen_buffer,
                               WINDOW_W * (WINDOW_H - 1) + x,
                               NUM_COLORS - 1)
    }
}

function update_screen_buffer(    color, rand_num, x, y)
{
    for (y = 1; y < WINDOW_H; y++) {
        for (x = 0; x < WINDOW_W; x++) {
            color = SDL_Gawk_GetPixelColor(screen_buffer, WINDOW_W * y + x)
            if (color > 0 && color < NUM_COLORS) {
                rand_num = int(rand() * 3)
                SDL_Gawk_SetPixelColor(screen_buffer,
                                       WINDOW_W * (y - 1) + x,
                                       color - and(rand_num, 1))
            }
        }
    }
}
