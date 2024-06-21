// SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <gawkapi.h>
#define _(msgid) msgid

#include "sdl2_mixer.h"

#define RETURN_NOK return make_number(-1, result)
#define RETURN_OK return make_number(0, result)

#define make_string_malloc(str, len, result) \
    r_make_string(api, ext_id, str, len, 1, result)

static const gawk_api_t *api;
static awk_ext_id_t ext_id;
static awk_bool_t init_sdl2_mixer(void);
static awk_bool_t (*init_func)(void) = init_sdl2_mixer;
static const char *ext_version = "SDL2_mixer extension: version (untracked)";

awk_bool_t gawk_api_varinit_constant(const gawk_api_t *,
                                     awk_ext_id_t,
                                     const char *,
                                     awk_value_t *,
                                     awk_scalar_t *);

int plugin_is_GPL_compatible;

/* Enable API */
static awk_scalar_t SDL_MIXER_MAJOR_VERSION_node;
static awk_scalar_t SDL_MIXER_MINOR_VERSION_node;
static awk_scalar_t SDL_MIXER_PATCHLEVEL_node;
static awk_scalar_t MIX_DEFAULT_FREQUENCY_node;
static awk_scalar_t MIX_DEFAULT_FORMAT_node;
static awk_scalar_t MIX_DEFAULT_CHANNELS_node;
static awk_scalar_t MIX_MAX_VOLUME_node;
static awk_scalar_t AUDIO_U16SYS_node;
static awk_scalar_t AUDIO_S16SYS_node;
static awk_scalar_t AUDIO_S32SYS_node;
static awk_scalar_t AUDIO_F32SYS_node;

/* Handle Channels for Sound Effects */
static awk_scalar_t MIX_CHANNEL_POST_node;

struct varinit {
    awk_scalar_t *spec;
    const char *name;
    int dfltval;
    int read_only;
};

static const struct varinit varinit[] = {
    ENTRY(SDL_MIXER_MAJOR_VERSION, 1),
    ENTRY(SDL_MIXER_MINOR_VERSION, 1),
    ENTRY(SDL_MIXER_PATCHLEVEL, 1),
    ENTRY(MIX_DEFAULT_FREQUENCY, 1),
    ENTRY(MIX_DEFAULT_FORMAT, 1),
    ENTRY(MIX_DEFAULT_CHANNELS, 1),
    ENTRY(MIX_MAX_VOLUME, 1),
    ENTRY(AUDIO_U16SYS, 1),
    ENTRY(AUDIO_S16SYS, 1),
    ENTRY(AUDIO_S32SYS, 1),
    ENTRY(AUDIO_F32SYS, 1),
    ENTRY(MIX_CHANNEL_POST, 1),
};

/* load_vars --- load constant variables */

static void
load_vars(void)
{
    const struct varinit *vp;
    size_t i;

    for (vp = varinit, i = 0; i < NUM_SCALARS; i++, vp++) {
        awk_value_t val;

        if (! gawk_api_varinit_constant(api,
                                        ext_id,
                                        vp->name,
                                        make_number(vp->dfltval, &val),
                                        vp->spec))
            fatal(ext_id, _("sdl2_mixer: unable to initialize %s"), vp->name);
    }
}

/*----- Enable API ---------------------------------------------------------*/

/* void Mix_Gawk_Linked_Version(awk_array_t *array); */
// /* It doesn't exist in SDL2_mixer */
/* do_Mix_Gawk_Linked_Version --- provide a Mix_Gawk_Linked_Version()
                                  function for gawk */

static awk_value_t *
do_Mix_Gawk_Linked_Version(int nargs,
                           awk_value_t *result,
                           struct awk_ext_func *finfo)
{
    const SDL_version *link_version;
    awk_value_t array_param;
    awk_array_t array;
    awk_value_t index, value;

    if (! get_argument(0, AWK_ARRAY, &array_param)) {
        warning(ext_id, _("Mix_Gawk_Linked_Version: bad parameter(s)"));
        RETURN_NOK;
    }

    array = array_param.array_cookie;

    clear_array(array);

    link_version = Mix_Linked_Version();
    set_array_element(array,
                      make_const_string("major", 5, &index),
                      make_number(link_version->major, &value));
    set_array_element(array,
                      make_const_string("minor", 5, &index),
                      make_number(link_version->minor, &value));
    set_array_element(array,
                      make_const_string("patch", 5, &index),
                      make_number(link_version->patch, &value));
    RETURN_OK;
}

/* int Mix_OpenAudio(int frequency,
                     Uint16 format,
                     int channels,
                     int chunksize); */
/* do_Mix_OpenAudio --- provide a Mix_OpenAudio() function for gawk */

static awk_value_t *
do_Mix_OpenAudio(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t frequency_param;
    awk_value_t format_param;
    awk_value_t channels_param;
    awk_value_t chunksize_param;
    int frequency;
    uint16_t format;
    int channels;
    int chunksize;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &frequency_param)
        || ! get_argument(1, AWK_NUMBER, &format_param)
        || ! get_argument(2, AWK_NUMBER, &channels_param)
        || ! get_argument(3, AWK_NUMBER, &chunksize_param)) {
        warning(ext_id, _("Mix_OpenAudio: bad parameter(s)"));
        RETURN_NOK;
    }

    frequency = frequency_param.num_value;
    format = format_param.num_value;
    channels = channels_param.num_value;
    chunksize = chunksize_param.num_value;

    ret = Mix_OpenAudio(frequency, format, channels, chunksize);
    if (ret < 0)
        update_ERRNO_string(_("Mix_OpenAudio failed"));

    return make_number(ret, result);
}

/* void Mix_CloseAudio(void); */
/* do_Mix_CloseAudio --- provide a Mix_CloseAudio() function for gawk */

static awk_value_t *
do_Mix_CloseAudio(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    Mix_CloseAudio();
    RETURN_OK;
}

/* int Mix_Gawk_QuerySpec(int frequency, Uint16 format, int channels); */
// /* It doesn't exist in SDL2_mixer */
/* do_Mix_Gawk_QuerySpec --- provide a Mix_Gawk_QuerySpec()
                             function for gawk */

static awk_value_t *
do_Mix_Gawk_QuerySpec(int nargs,
                      awk_value_t *result,
                      struct awk_ext_func *finfo)
{
    awk_value_t frequency_param;
    awk_value_t format_param;
    awk_value_t channels_param;
    int frequency;
    uint16_t format;
    int channels;

    if (! get_argument(0, AWK_NUMBER, &frequency_param)
        || ! get_argument(1, AWK_NUMBER, &format_param)
        || ! get_argument(2, AWK_NUMBER, &channels_param)) {
        warning(ext_id, _("Mix_Gawk_QuerySpec: bad parameter(s)"));
        RETURN_NOK;
    }

    frequency = frequency_param.num_value;
    format = format_param.num_value;
    channels = channels_param.num_value;

    return make_number(Mix_QuerySpec(&frequency, &format, &channels), result);
}

/* const char *Mix_GetError(void); */
/* do_Mix_GetError --- provide a Mix_GetError() function for gawk */

static awk_value_t *
do_Mix_GetError(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    const char *errormsg;

    errormsg = Mix_GetError();

    if (errormsg)
        return make_string_malloc(errormsg, strlen(errormsg), result);

    update_ERRNO_string(_("Mix_GetError failed"));
    return make_null_string(result);
}

/*----- Read Sampling Audio from File or Memory ----------------------------*/

/* Mix_Chunk *Mix_LoadWAV(const char *file); */
/* do_Mix_LoadWAV --- provide a Mix_LoadWAV() function for gawk */

static awk_value_t *
do_Mix_LoadWAV(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    Mix_Chunk *chunk;
    awk_value_t file_param;
    const char *file;

    if (! get_argument(0, AWK_STRING, &file_param)) {
        warning(ext_id, _("Mix_LoadWAV: bad parameter(s)"));
        RETURN_NOK;
    }

    file = file_param.str_value.str;

    chunk = Mix_LoadWAV(file);

    if (chunk) {
        char chunk_addr[20];
        // NOLINTNEXTLINE
        snprintf(chunk_addr, sizeof(chunk_addr), "%p", chunk);
        return make_string_malloc(chunk_addr, strlen(chunk_addr), result);
    }

    update_ERRNO_string(_("Mix_LoadWAV failed"));
    return make_null_string(result);
}

/* void Mix_FreeChunk(Mix_Chunk *chunk); */
/* do_Mix_FreeChunk --- provide a Mix_FreeChunk() function for gawk */

static awk_value_t *
do_Mix_FreeChunk(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t chunk_ptr_param;
    uintptr_t chunk_ptr;

    if (! get_argument(0, AWK_STRING, &chunk_ptr_param)) {
        warning(ext_id, _("Mix_FreeChunk: bad parameter(s)"));
        RETURN_NOK;
    }

    chunk_ptr = strtoull(chunk_ptr_param.str_value.str, (char **)NULL, 16);

    Mix_FreeChunk((Mix_Chunk *)chunk_ptr);
    RETURN_OK;
}

/*----- Handle Channels for Sound Effects ----------------------------------*/

/* int Mix_AllocateChannels(int numchans); */
/* do_Mix_AllocateChannels --- provide a Mix_AllocateChannels()
                               function for gawk */

static awk_value_t *
do_Mix_AllocateChannels(int nargs,
                        awk_value_t *result,
                        struct awk_ext_func *finfo)
{
    awk_value_t numchans_param;
    int numchans;

    if (! get_argument(0, AWK_NUMBER, &numchans_param)) {
        warning(ext_id, _("Mix_AllocateChannels: bad parameter(s)"));
        RETURN_NOK;
    }

    numchans = numchans_param.num_value;

    return make_number(Mix_AllocateChannels(numchans), result);
}

/* int Mix_PlayChannelTimed(int channel,
                            Mix_Chunk *chunk,
                            int loops,
                            int ticks); */
/* do_Mix_PlayChannelTimed --- provide a Mix_PlayChannelTimed()
                               function for gawk */

static awk_value_t *
do_Mix_PlayChannelTimed(int nargs,
                        awk_value_t *result,
                        struct awk_ext_func *finfo)
{
    awk_value_t channel_param;
    awk_value_t chunk_ptr_param;
    awk_value_t loops_param;
    awk_value_t ticks_param;
    int channel;
    uintptr_t chunk_ptr;
    int loops;
    int ticks;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &channel_param)
        || ! get_argument(1, AWK_STRING, &chunk_ptr_param)
        || ! get_argument(2, AWK_NUMBER, &loops_param)) {
        warning(ext_id, _("Mix_PlayChannelTimed: bad parameter(s)"));
        RETURN_NOK;
    }

    if (nargs > 3) {
        if (! get_argument(3, AWK_NUMBER, &ticks_param)) {
            warning(ext_id, _("Mix_PlayChannelTimed: bad parameter(s)"));
            RETURN_NOK;
        }
    } else
        ticks_param.num_value = -1;

    channel = channel_param.num_value;
    chunk_ptr = strtoull(chunk_ptr_param.str_value.str, (char **)NULL, 16);
    loops = loops_param.num_value;
    ticks = ticks_param.num_value;

    ret = Mix_PlayChannelTimed(channel, (Mix_Chunk *)chunk_ptr, loops, ticks);
    if (ret < 0)
        update_ERRNO_string(_("Mix_PlayChannelTimed failed"));

    return make_number(ret, result);
}

/* int Mix_HaltChannel(int channel); */
/* do_Mix_HaltChannel --- provide a Mix_HaltChannel() function for gawk */

static awk_value_t *
do_Mix_HaltChannel(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t channel_param;
    int channel;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &channel_param)) {
        warning(ext_id, _("Mix_HaltChannel: bad parameter(s)"));
        RETURN_NOK;
    }

    channel = channel_param.num_value;

    ret = Mix_HaltChannel(channel);
    if (ret < 0)
        update_ERRNO_string(_("Mix_HaltChannel failed"));

    return make_number(ret, result);
}

/* int Mix_Playing(int channel); */
/* do_Mix_Playing --- provide a Mix_Playing() function for gawk */

static awk_value_t *
do_Mix_Playing(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t channel_param;
    int channel;

    if (! get_argument(0, AWK_NUMBER, &channel_param)) {
        warning(ext_id, _("Mix_Playing: bad parameter(s)"));
        RETURN_NOK;
    }

    channel = channel_param.num_value;

    return make_number(Mix_Playing(channel), result);
}

/*----- Play Music ---------------------------------------------------------*/

/* Mix_Music *Mix_LoadMUS(const char *file); */
/* do_Mix_LoadMUS --- provide a Mix_LoadMUS() function for gawk */

static awk_value_t *
do_Mix_LoadMUS(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    Mix_Music *music;
    awk_value_t file_param;
    const char *file;

    if (! get_argument(0, AWK_STRING, &file_param)) {
        warning(ext_id, _("Mix_LoadMUS: bad parameter(s)"));
        RETURN_NOK;
    }

    file = file_param.str_value.str;

    music = Mix_LoadMUS(file);

    if (music) {
        char music_addr[20];
        // NOLINTNEXTLINE
        snprintf(music_addr, sizeof(music_addr), "%p", music);
        return make_string_malloc(music_addr, strlen(music_addr), result);
    }

    update_ERRNO_string(_("Mix_LoadMUS failed"));
    return make_null_string(result);
}

/* void Mix_FreeMusic(Mix_Music *music); */
/* do_Mix_FreeMusic --- provide a Mix_FreeMusic() function for gawk */

static awk_value_t *
do_Mix_FreeMusic(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t music_ptr_param;
    uintptr_t music_ptr;

    if (! get_argument(0, AWK_STRING, &music_ptr_param)) {
        warning(ext_id, _("Mix_FreeMusic: bad parameter(s)"));
        RETURN_NOK;
    }

    music_ptr = strtoull(music_ptr_param.str_value.str, (char **)NULL, 16);

    Mix_FreeMusic((Mix_Music *)music_ptr);
    RETURN_OK;
}

/* int Mix_PlayMusic(Mix_Music *music, int loops); */
/* do_Mix_PlayMusic --- provide a Mix_PlayMusic() function for gawk */

static awk_value_t *
do_Mix_PlayMusic(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t music_ptr_param;
    awk_value_t loops_param;
    uintptr_t music_ptr;
    int loops;
    int ret;

    if (! get_argument(0, AWK_STRING, &music_ptr_param)
        || ! get_argument(1, AWK_NUMBER, &loops_param)) {
        warning(ext_id, _("Mix_PlayMusic: bad parameter(s)"));
        RETURN_NOK;
    }

    music_ptr = strtoull(music_ptr_param.str_value.str, (char **)NULL, 16);
    loops = loops_param.num_value;

    ret = Mix_PlayMusic((Mix_Music *)music_ptr, loops);
    if (ret < 0)
        update_ERRNO_string(_("Mix_PlayMusic failed"));

    return make_number(ret, result);
}

/* int Mix_VolumeMusic(int volume); */
/* do_Mix_VolumeMusic --- provide a Mix_VolumeMusic() function for gawk */

static awk_value_t *
do_Mix_VolumeMusic(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t volume_param;
    int volume;

    if (! get_argument(0, AWK_NUMBER, &volume_param)) {
        warning(ext_id, _("Mix_VolumeMusic: bad parameter(s)"));
        RETURN_NOK;
    }

    volume = volume_param.num_value;

    return make_number(Mix_VolumeMusic(volume), result);
}

/* int Mix_SetMusicPosition(double position); */
/* do_Mix_SetMusicPosition --- provide a Mix_SetMusicPosition()
                               function for gawk */

static awk_value_t *
do_Mix_SetMusicPosition(int nargs,
                        awk_value_t *result,
                        struct awk_ext_func *finfo)
{
    awk_value_t position_param;
    double position;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &position_param)) {
        warning(ext_id, _("Mix_SetMusicPosition: bad parameter(s)"));
        RETURN_NOK;
    }

    position = position_param.num_value;

    ret = Mix_SetMusicPosition(position);
    if (ret < 0)
        update_ERRNO_string(_("Mix_SetMusicPosition failed"));

    return make_number(ret, result);
}

/* int Mix_SetMusicCMD(const char *command); */
/* do_Mix_SetMusicCMD --- provide a Mix_SetMusicCMD() function for gawk */

static awk_value_t *
do_Mix_SetMusicCMD(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t command_param;
    const char *command;
    int ret;

    if (! get_argument(0, AWK_STRING, &command_param)) {
        warning(ext_id, _("Mix_SetMusicCMD: bad parameter(s)"));
        RETURN_NOK;
    }

    command = command_param.str_value.str;

    ret = Mix_SetMusicCMD(command);
    if (ret < 0)
        update_ERRNO_string(_("Mix_SetMusicCMD failed"));

    return make_number(ret, result);
}

/* int Mix_HaltMusic(void); */
/* do_Mix_HaltMusic --- provide a Mix_HaltMusic() function for gawk */

static awk_value_t *
do_Mix_HaltMusic(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    return make_number(Mix_HaltMusic(), result);
}

/* int Mix_PlayingMusic(void); */
/* do_Mix_PlayingMusic --- provide a Mix_PlayingMusic() function for gawk */

static awk_value_t *
do_Mix_PlayingMusic(int nargs,
                    awk_value_t *result,
                    struct awk_ext_func *finfo)
{
    return make_number(Mix_PlayingMusic(), result);
}

/*----- Handle Effects -----------------------------------------------------*/

/* int Mix_SetPanning(int channel, Uint8 left, Uint8 right); */
/* do_Mix_SetPanning --- provide a Mix_SetPanning() function for gawk */

static awk_value_t *
do_Mix_SetPanning(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t channel_param;
    awk_value_t left_param, right_param;
    int channel;
    uint8_t left, right;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &channel_param)
        || ! get_argument(1, AWK_NUMBER, &left_param)
        || ! get_argument(2, AWK_NUMBER, &right_param)) {
        warning(ext_id, _("Mix_SetPanning: bad parameter(s)"));
        RETURN_NOK;
    }

    channel = channel_param.num_value;
    left = left_param.num_value;
    right = right_param.num_value;

    ret = Mix_SetPanning(channel, left, right);
    if (ret == 0)
        update_ERRNO_string(_("Mix_SetPanning failed"));

    return make_number(ret, result);
}

/* int Mix_SetDistance(int channel, Uint8 distance); */
/* do_Mix_SetDistance --- provide a Mix_SetDistance() function for gawk */

static awk_value_t *
do_Mix_SetDistance(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t channel_param;
    awk_value_t distance_param;
    int channel;
    uint8_t distance;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &channel_param)
        || ! get_argument(1, AWK_NUMBER, &distance_param)) {
        warning(ext_id, _("Mix_SetDistance: bad parameter(s)"));
        RETURN_NOK;
    }

    channel = channel_param.num_value;
    distance = distance_param.num_value;

    ret = Mix_SetDistance(channel, distance);
    if (ret == 0)
        update_ERRNO_string(_("Mix_SetDistance failed"));

    return make_number(ret, result);
}

/* int Mix_SetPosition(int channel, Sint16 angle, Uint8 distance); */
/* do_Mix_SetPosition --- provide a Mix_SetPosition() function for gawk */

static awk_value_t *
do_Mix_SetPosition(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t channel_param;
    awk_value_t angle_param;
    awk_value_t distance_param;
    int channel;
    int16_t angle;
    uint8_t distance;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &channel_param)
        || ! get_argument(1, AWK_NUMBER, &angle_param)
        || ! get_argument(2, AWK_NUMBER, &distance_param)) {
        warning(ext_id, _("Mix_SetPosition: bad parameter(s)"));
        RETURN_NOK;
    }

    channel = channel_param.num_value;
    angle = angle_param.num_value;
    distance = distance_param.num_value;

    ret = Mix_SetPosition(channel, angle, distance);
    if (ret == 0)
        update_ERRNO_string(_("Mix_SetPosition failed"));

    return make_number(ret, result);
}

/* int Mix_SetReverseStereo(int channel, int flip); */
/* do_Mix_SetReverseStereo --- provide a Mix_SetReverseStereo()
                               function for gawk */

static awk_value_t *
do_Mix_SetReverseStereo(int nargs,
                        awk_value_t *result,
                        struct awk_ext_func *finfo)
{
    awk_value_t channel_param;
    awk_value_t flip_param;
    int channel;
    int flip;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &channel_param)
        || ! get_argument(1, AWK_NUMBER, &flip_param)) {
        warning(ext_id, _("Mix_SetReverseStereo: bad parameter(s)"));
        RETURN_NOK;
    }

    channel = channel_param.num_value;
    flip = flip_param.num_value;

    ret = Mix_SetReverseStereo(channel, flip);
    if (ret == 0)
        update_ERRNO_string(_("Mix_SetReverseStereo failed"));

    return make_number(ret, result);
}

/*--------------------------------------------------------------------------*/

/* init_sdl2_mixer --- initialization routine */

static awk_bool_t
init_sdl2_mixer(void)
{
    load_vars();
    return awk_true;
}

static awk_ext_func_t func_table[] = {
    { "Mix_Gawk_Linked_Version", do_Mix_Gawk_Linked_Version,
      1, 1,
      awk_false,
      NULL },
    { "Mix_OpenAudio", do_Mix_OpenAudio, 4, 4, awk_false, NULL },
    { "Mix_CloseAudio", do_Mix_CloseAudio, 0, 0, awk_false, NULL },
    { "Mix_Gawk_QuerySpec", do_Mix_Gawk_QuerySpec, 3, 3, awk_false, NULL },
    { "Mix_GetError", do_Mix_GetError, 0, 0, awk_false, NULL },
    { "Mix_LoadWAV", do_Mix_LoadWAV, 1, 1, awk_false, NULL },
    { "Mix_FreeChunk", do_Mix_FreeChunk, 1, 1, awk_false, NULL },
    { "Mix_AllocateChannels", do_Mix_AllocateChannels,
      1, 1,
      awk_false,
      NULL },
    { "Mix_PlayChannel", do_Mix_PlayChannelTimed, 3, 3, awk_false, NULL },
    { "Mix_PlayChannelTimed", do_Mix_PlayChannelTimed,
      4, 4,
      awk_false,
      NULL },
    { "Mix_HaltChannel", do_Mix_HaltChannel, 1, 1, awk_false, NULL },
    { "Mix_Playing", do_Mix_Playing, 1, 1, awk_false, NULL },
    { "Mix_LoadMUS", do_Mix_LoadMUS, 1, 1, awk_false, NULL },
    { "Mix_FreeMusic", do_Mix_FreeMusic, 1, 1, awk_false, NULL },
    { "Mix_PlayMusic", do_Mix_PlayMusic, 2, 2, awk_false, NULL },
    { "Mix_VolumeMusic", do_Mix_VolumeMusic, 1, 1, awk_false, NULL },
    { "Mix_SetMusicPosition", do_Mix_SetMusicPosition,
      1, 1,
      awk_false,
      NULL },
    { "Mix_SetMusicCMD", do_Mix_SetMusicCMD, 1, 1, awk_false, NULL },
    { "Mix_HaltMusic", do_Mix_HaltMusic, 0, 0, awk_false, NULL },
    { "Mix_PlayingMusic", do_Mix_PlayingMusic, 0, 0, awk_false, NULL },
    { "Mix_SetPanning", do_Mix_SetPanning, 3, 3, awk_false, NULL },
    { "Mix_SetDistance", do_Mix_SetDistance, 2, 2, awk_false, NULL },
    { "Mix_SetPosition", do_Mix_SetPosition, 3, 3, awk_false, NULL },
    { "Mix_SetReverseStereo", do_Mix_SetReverseStereo,
      2, 2,
      awk_false,
      NULL },
};

/* define the dl_load() function using the boilerplate macro */

dl_load_func(func_table, sdl2_mixer, "")
