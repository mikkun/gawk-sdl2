#! /usr/bin/gawk -f

# SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
# SPDX-License-Identifier: GPL-3.0-or-later

@load "sdl2"
@load "sdl2_mixer"

BEGIN {
    TITLE        = "gawk-sdl2 - pong.awk"
    WINDOW_W     = 800
    WINDOW_H     = 600
    PADDLE_W     = 20
    PADDLE_H     = 80
    PADDLE_SPEED = 15
    BALL_SIZE    = 20
    BALL_SPEED   = 10
    MAX_POINTS   = 9
    PIXEL_SIZE   = 10
    WAIT         = 1000 / 60

    srand()
    SDL_Init(or(SDL_INIT_AUDIO, SDL_INIT_VIDEO))
    window = SDL_CreateWindow(TITLE,
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_W, WINDOW_H,
                              0)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_S16SYS, 2, 1024)
    channel["paddle"] = 0
    channel["score"]  = 1
    channel["wall"]   = 2
    chunk["paddle"]   = Mix_LoadWAV("./assets/paddle.wav")
    chunk["score"]    = Mix_LoadWAV("./assets/score.wav")
    chunk["wall"]     = Mix_LoadWAV("./assets/wall.wav")
    volume            = 0

    paddle_l["ptr"] = SDL_Gawk_CreateRect()
    paddle_l["x"]   = PADDLE_W
    paddle_l["y"]   = (WINDOW_H - PADDLE_H) / 2
    paddle_l["vy"]  = PADDLE_SPEED

    paddle_r["ptr"] = SDL_Gawk_CreateRect()
    paddle_r["x"]   = WINDOW_W - PADDLE_W * 2
    paddle_r["y"]   = (WINDOW_H - PADDLE_H) / 2
    paddle_r["vy"]  = PADDLE_SPEED * 0.75

    ball["ptr"] = SDL_Gawk_CreateRect()
    ball["x"]   = (WINDOW_W - BALL_SIZE) / 2
    ball["y"]   = 0
    ball["vx"]  = int(rand() * 2) ? BALL_SPEED : -BALL_SPEED
    ball["vy"]  = BALL_SPEED

    score_l["points"] = 0
    score_l["x"]      = WINDOW_W * 0.25 - PIXEL_SIZE * 1.5
    score_l["y"]      = PIXEL_SIZE * 2
    init_pixels(score_l)

    score_r["points"] = 0
    score_r["x"]      = WINDOW_W * 0.75 - PIXEL_SIZE * 1.5
    score_r["y"]      = PIXEL_SIZE * 2
    init_pixels(score_r)

    event = SDL_Gawk_CreateEvent()
    SDL_Gawk_UpdateRect(paddle_l["ptr"],
                        paddle_l["x"], paddle_l["y"],
                        PADDLE_W, PADDLE_H)
    SDL_Gawk_UpdateRect(paddle_r["ptr"],
                        paddle_r["x"], paddle_r["y"],
                        PADDLE_W, PADDLE_H)
    SDL_Gawk_UpdateRect(ball["ptr"],
                        ball["x"], ball["y"],
                        BALL_SIZE, BALL_SIZE)

    quit   = 0
    scored = 0
    while (! quit) {
        while (SDL_PollEvent(event)) {
            event_type = SDL_Gawk_GetEventType(event)
            if (event_type == SDL_QUIT) {
                message = "THE GAME IS QUIT"
                quit = 1
            }
        }

        SDL_Gawk_GetKeyboardState(NULL, curr_state)
        if (curr_state[SDL_SCANCODE_Q] || curr_state[SDL_SCANCODE_ESCAPE]) {
            message = "THE GAME IS QUIT"
            quit = 1
        }
        if (curr_state[SDL_SCANCODE_UP]) {
            paddle_l["y"] -= paddle_l["vy"]
        }
        if (curr_state[SDL_SCANCODE_DOWN]) {
            paddle_l["y"] += paddle_l["vy"]
        }
        if (paddle_l["y"] < 0) {
            paddle_l["y"] = 0
        }
        if (paddle_l["y"] > WINDOW_H - PADDLE_H) {
            paddle_l["y"] = WINDOW_H - PADDLE_H
        }

        if (ball["vx"] > 0) {
            if (ball["y"] + BALL_SIZE / 2 < paddle_r["y"] + PADDLE_H / 2) {
                paddle_r["y"] -= paddle_r["vy"]
            } else {
                paddle_r["y"] += paddle_r["vy"]
            }
        }
        if (paddle_r["y"] < 0) {
            paddle_r["y"] = 0
        }
        if (paddle_r["y"] > WINDOW_H - PADDLE_H) {
            paddle_r["y"] = WINDOW_H - PADDLE_H
        }

        if (ball["x"] <= -BALL_SIZE && ! scored) {
            Mix_PlayChannel(channel["score"], chunk["score"], 0)
            score_r["points"] += 1
            scored = 1
        }
        if (ball["x"] >= WINDOW_W && ! scored) {
            Mix_PlayChannel(channel["score"], chunk["score"], 0)
            score_l["points"] += 1
            scored = 1
        }

        if (ball["x"] <= -WINDOW_W) {
            if (score_r["points"] == MAX_POINTS) {
                message = sprintf("[YOU %d - COM %d] YOU LOST",
                                  score_l["points"],
                                  score_r["points"])
                quit = 1
            }
            ball["x"]  = (WINDOW_W - BALL_SIZE) / 2
            ball["y"]  = int(rand() * 2) ? 0 : WINDOW_H - BALL_SIZE
            ball["vx"] = -BALL_SPEED
            ball["vy"] = ball["y"] == 0 ? BALL_SPEED : -BALL_SPEED
            scored = 0
        }
        if (ball["x"] >= WINDOW_W * 2) {
            if (score_l["points"] == MAX_POINTS) {
                message = sprintf("[YOU %d - COM %d] YOU WON!",
                                  score_l["points"],
                                  score_r["points"])
                quit = 1
            }
            ball["x"]  = (WINDOW_W - BALL_SIZE) / 2
            ball["y"]  = int(rand() * 2) ? 0 : WINDOW_H - BALL_SIZE
            ball["vx"] = BALL_SPEED
            ball["vy"] = ball["y"] == 0 ? BALL_SPEED : -BALL_SPEED
            scored = 0
        }

        ball["x"] += ball["vx"]
        ball["y"] += ball["vy"]

        volume = int((ball["x"] + BALL_SIZE / 2) / WINDOW_W * 255)
        if (volume < 0) {
            volume = 0
        }
        if (volume > 255) {
            volume = 255
        }
        Mix_SetPanning(channel["paddle"], 255 - volume, volume)
        Mix_SetPanning(channel["score"], volume, 255 - volume)
        Mix_SetPanning(channel["wall"], 255 - volume, volume)

        if (ball["y"] <= 0) {
            if (! scored) {
                Mix_PlayChannel(channel["wall"], chunk["wall"], 0)
            }
            ball["vy"] = -ball["vy"]
            ball["vy"] = ball["vy"] <= 0 ? 1 : ball["vy"]
        }
        if (ball["y"] >= WINDOW_H - BALL_SIZE) {
            if (! scored) {
                Mix_PlayChannel(channel["wall"], chunk["wall"], 0)
            }
            ball["vy"] = -ball["vy"]
            ball["vy"] = ball["vy"] >= 0 ? -1 : ball["vy"]
        }

        if (SDL_HasIntersection(paddle_l["ptr"], ball["ptr"])) {
            Mix_PlayChannel(channel["paddle"], chunk["paddle"], 0)
            ball["x"]  = paddle_l["x"] + PADDLE_W
            ball["vx"] = -ball["vx"]
            if ((ball["y"] + BALL_SIZE - paddle_l["y"] - PADDLE_H) / 2 \
                 < -BALL_SIZE) {
                ball["vy"] -= 1
            }
            if ((ball["y"] + BALL_SIZE - paddle_l["y"] - PADDLE_H) / 2 \
                 > BALL_SIZE) {
                ball["vy"] += 1
            }
            if (abs(ball["vy"]) < BALL_SPEED / 2) {
                if (! int(rand() * 15)) {
                    ball["vy"] += int(rand() * 2) ? 1 : -1
                    ball["vy"] *= 2
                }
            }
        }

        if (SDL_HasIntersection(paddle_r["ptr"], ball["ptr"])) {
            Mix_PlayChannel(channel["paddle"], chunk["paddle"], 0)
            ball["x"]  = paddle_r["x"] - BALL_SIZE
            ball["vx"] = -ball["vx"]
            if ((ball["y"] + BALL_SIZE - paddle_r["y"] - PADDLE_H) / 2 \
                 < -BALL_SIZE) {
                ball["vy"] -= 1
            }
            if ((ball["y"] + BALL_SIZE - paddle_r["y"] - PADDLE_H) / 2 \
                 > BALL_SIZE) {
                ball["vy"] += 1
            }
            if (abs(ball["vy"]) < BALL_SPEED / 2) {
                if (! int(rand() * 15)) {
                    ball["vy"] += int(rand() * 2) ? 1 : -1
                    ball["vy"] *= 2
                }
            }
        }

        ball["vy"] = ball["vy"] < -PADDLE_H / 2 ? -PADDLE_H / 2 : ball["vy"]
        ball["vy"] = ball["vy"] > PADDLE_H / 2 ? PADDLE_H / 2 : ball["vy"]

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255)
        SDL_RenderClear(renderer)

        SDL_Gawk_UpdateRect(paddle_l["ptr"],
                            paddle_l["x"], paddle_l["y"],
                            PADDLE_W, PADDLE_H)
        SDL_Gawk_UpdateRect(paddle_r["ptr"],
                            paddle_r["x"], paddle_r["y"],
                            PADDLE_W, PADDLE_H)
        SDL_Gawk_UpdateRect(ball["ptr"],
                            ball["x"], ball["y"],
                            BALL_SIZE, BALL_SIZE)

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255)

        for (i = 0; i < WINDOW_H / 20; i++) {
            SDL_RenderDrawLine(renderer,
                               WINDOW_W / 2, i * 20,
                               WINDOW_W / 2, i * 20 + 10)
        }

        SDL_RenderFillRect(renderer, paddle_l["ptr"])
        SDL_RenderFillRect(renderer, paddle_r["ptr"])
        if (! quit) {
            SDL_RenderFillRect(renderer, ball["ptr"])
        }

        render_score(score_l)
        render_score(score_r)

        SDL_RenderPresent(renderer)
        SDL_Delay(WAIT)
    }

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "GAME OVER",
                             message,
                             window)

    Mix_FreeChunk(chunk["paddle"])
    Mix_FreeChunk(chunk["score"])
    Mix_FreeChunk(chunk["wall"])
    Mix_CloseAudio()

    SDL_DestroyRenderer(renderer)
    SDL_DestroyWindow(window)
    SDL_Quit()
    exit 0
}

function abs(num)
{
    return num < 0 ? -num : num
}

function init_pixels(score,    i)
{
    for (i = 1; i <= 15; i++) {
        score["pixels"][i]["ptr"] = SDL_Gawk_CreateRect()
        score["pixels"][i]["x"]   = score["x"] + PIXEL_SIZE * ((i - 1) % 3)
        score["pixels"][i]["y"]   = score["y"] + PIXEL_SIZE * int((i - 1) / 3)
        SDL_Gawk_UpdateRect(score["pixels"][i]["ptr"],
                            score["pixels"][i]["x"], score["pixels"][i]["y"],
                            PIXEL_SIZE, PIXEL_SIZE)
    }
}

function render_score(score,    pixels, i)
{
    switch (score["points"]) {
        case 0:
            split("111101101101111", pixels, "")
            break
        case 1:
            split("001001001001001", pixels, "")
            break
        case 2:
            split("111001111100111", pixels, "")
            break
        case 3:
            split("111001111001111", pixels, "")
            break
        case 4:
            split("101101111001001", pixels, "")
            break
        case 5:
            split("111100111001111", pixels, "")
            break
        case 6:
            split("100100111101111", pixels, "")
            break
        case 7:
            split("111001001001001", pixels, "")
            break
        case 8:
            split("111101111101111", pixels, "")
            break
        case 9:
            split("111101111001001", pixels, "")
            break
        default:
            split("111001011000010", pixels, "")
            break
    }

    for (i = 1; i <= 15; i++) {
        if (pixels[i]) {
            SDL_RenderFillRect(renderer, score["pixels"][i]["ptr"])
        }
    }
}
