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

#include "sdl2.h"

#define RETURN_NOK return make_number(-1, result)
#define RETURN_OK return make_number(0, result)

#define make_string_malloc(str, len, result) \
    r_make_string(api, ext_id, str, len, 1, result)

static const gawk_api_t *api;
static awk_ext_id_t ext_id;
static awk_bool_t init_sdl2(void);
static awk_bool_t (*init_func)(void) = init_sdl2;
static const char *ext_version = "SDL2 extension: version (untracked)";

awk_bool_t gawk_api_varinit_constant(const gawk_api_t *,
                                     awk_ext_id_t,
                                     const char *,
                                     awk_value_t *,
                                     awk_scalar_t *);

int plugin_is_GPL_compatible;

/* Initialization and Shutdown */
static awk_scalar_t SDL_INIT_TIMER_node;
static awk_scalar_t SDL_INIT_AUDIO_node;
static awk_scalar_t SDL_INIT_VIDEO_node;
static awk_scalar_t SDL_INIT_JOYSTICK_node;
static awk_scalar_t SDL_INIT_HAPTIC_node;
static awk_scalar_t SDL_INIT_GAMECONTROLLER_node;
static awk_scalar_t SDL_INIT_EVENTS_node;
static awk_scalar_t SDL_INIT_SENSOR_node;
static awk_scalar_t SDL_INIT_NOPARACHUTE_node;
static awk_scalar_t SDL_INIT_EVERYTHING_node;

/* Querying SDL Version */
static awk_scalar_t SDL_MAJOR_VERSION_node;
static awk_scalar_t SDL_MINOR_VERSION_node;
static awk_scalar_t SDL_PATCHLEVEL_node;
static awk_scalar_t SDL_COMPILEDVERSION_node;

/* Display and Window Management */
static awk_scalar_t SDL_WINDOW_FULLSCREEN_node;
static awk_scalar_t SDL_WINDOW_OPENGL_node;
static awk_scalar_t SDL_WINDOW_SHOWN_node;
static awk_scalar_t SDL_WINDOW_HIDDEN_node;
static awk_scalar_t SDL_WINDOW_BORDERLESS_node;
static awk_scalar_t SDL_WINDOW_RESIZABLE_node;
static awk_scalar_t SDL_WINDOW_MINIMIZED_node;
static awk_scalar_t SDL_WINDOW_MAXIMIZED_node;
static awk_scalar_t SDL_WINDOW_MOUSE_GRABBED_node;
static awk_scalar_t SDL_WINDOW_INPUT_FOCUS_node;
static awk_scalar_t SDL_WINDOW_MOUSE_FOCUS_node;
static awk_scalar_t SDL_WINDOW_FULLSCREEN_DESKTOP_node;
static awk_scalar_t SDL_WINDOW_FOREIGN_node;
static awk_scalar_t SDL_WINDOW_ALLOW_HIGHDPI_node;
static awk_scalar_t SDL_WINDOW_MOUSE_CAPTURE_node;
static awk_scalar_t SDL_WINDOW_ALWAYS_ON_TOP_node;
static awk_scalar_t SDL_WINDOW_SKIP_TASKBAR_node;
static awk_scalar_t SDL_WINDOW_UTILITY_node;
static awk_scalar_t SDL_WINDOW_TOOLTIP_node;
static awk_scalar_t SDL_WINDOW_POPUP_MENU_node;
static awk_scalar_t SDL_WINDOW_KEYBOARD_GRABBED_node;
static awk_scalar_t SDL_WINDOW_VULKAN_node;
static awk_scalar_t SDL_WINDOW_METAL_node;
static awk_scalar_t SDL_WINDOW_INPUT_GRABBED_node;
static awk_scalar_t SDL_WINDOWPOS_UNDEFINED_node;
static awk_scalar_t SDL_WINDOWPOS_CENTERED_node;
static awk_scalar_t SDL_WINDOWEVENT_NONE_node;
static awk_scalar_t SDL_WINDOWEVENT_SHOWN_node;
static awk_scalar_t SDL_WINDOWEVENT_HIDDEN_node;
static awk_scalar_t SDL_WINDOWEVENT_EXPOSED_node;
static awk_scalar_t SDL_WINDOWEVENT_MOVED_node;
static awk_scalar_t SDL_WINDOWEVENT_RESIZED_node;
static awk_scalar_t SDL_WINDOWEVENT_SIZE_CHANGED_node;
static awk_scalar_t SDL_WINDOWEVENT_MINIMIZED_node;
static awk_scalar_t SDL_WINDOWEVENT_MAXIMIZED_node;
static awk_scalar_t SDL_WINDOWEVENT_RESTORED_node;
static awk_scalar_t SDL_WINDOWEVENT_ENTER_node;
static awk_scalar_t SDL_WINDOWEVENT_LEAVE_node;
static awk_scalar_t SDL_WINDOWEVENT_FOCUS_GAINED_node;
static awk_scalar_t SDL_WINDOWEVENT_FOCUS_LOST_node;
static awk_scalar_t SDL_WINDOWEVENT_CLOSE_node;
static awk_scalar_t SDL_WINDOWEVENT_TAKE_FOCUS_node;
static awk_scalar_t SDL_WINDOWEVENT_HIT_TEST_node;
static awk_scalar_t SDL_WINDOWEVENT_ICCPROF_CHANGED_node;
static awk_scalar_t SDL_WINDOWEVENT_DISPLAY_CHANGED_node;

/* 2D Accelerated Rendering */
static awk_scalar_t SDL_RENDERER_SOFTWARE_node;
static awk_scalar_t SDL_RENDERER_ACCELERATED_node;
static awk_scalar_t SDL_RENDERER_PRESENTVSYNC_node;
static awk_scalar_t SDL_RENDERER_TARGETTEXTURE_node;
static awk_scalar_t SDL_TEXTUREACCESS_STATIC_node;
static awk_scalar_t SDL_TEXTUREACCESS_STREAMING_node;
static awk_scalar_t SDL_TEXTUREACCESS_TARGET_node;

/* Pixel Formats and Conversion Routines */
static awk_scalar_t SDL_PIXELFORMAT_UNKNOWN_node;
static awk_scalar_t SDL_PIXELFORMAT_INDEX1LSB_node;
static awk_scalar_t SDL_PIXELFORMAT_INDEX1MSB_node;
static awk_scalar_t SDL_PIXELFORMAT_INDEX4LSB_node;
static awk_scalar_t SDL_PIXELFORMAT_INDEX4MSB_node;
static awk_scalar_t SDL_PIXELFORMAT_INDEX8_node;
static awk_scalar_t SDL_PIXELFORMAT_RGB332_node;
static awk_scalar_t SDL_PIXELFORMAT_XRGB4444_node;
static awk_scalar_t SDL_PIXELFORMAT_RGB444_node;
static awk_scalar_t SDL_PIXELFORMAT_XBGR4444_node;
static awk_scalar_t SDL_PIXELFORMAT_BGR444_node;
static awk_scalar_t SDL_PIXELFORMAT_XRGB1555_node;
static awk_scalar_t SDL_PIXELFORMAT_RGB555_node;
static awk_scalar_t SDL_PIXELFORMAT_XBGR1555_node;
static awk_scalar_t SDL_PIXELFORMAT_BGR555_node;
static awk_scalar_t SDL_PIXELFORMAT_ARGB4444_node;
static awk_scalar_t SDL_PIXELFORMAT_RGBA4444_node;
static awk_scalar_t SDL_PIXELFORMAT_ABGR4444_node;
static awk_scalar_t SDL_PIXELFORMAT_BGRA4444_node;
static awk_scalar_t SDL_PIXELFORMAT_ARGB1555_node;
static awk_scalar_t SDL_PIXELFORMAT_RGBA5551_node;
static awk_scalar_t SDL_PIXELFORMAT_ABGR1555_node;
static awk_scalar_t SDL_PIXELFORMAT_BGRA5551_node;
static awk_scalar_t SDL_PIXELFORMAT_RGB565_node;
static awk_scalar_t SDL_PIXELFORMAT_BGR565_node;
static awk_scalar_t SDL_PIXELFORMAT_RGB24_node;
static awk_scalar_t SDL_PIXELFORMAT_BGR24_node;
static awk_scalar_t SDL_PIXELFORMAT_XRGB8888_node;
static awk_scalar_t SDL_PIXELFORMAT_RGB888_node;
static awk_scalar_t SDL_PIXELFORMAT_RGBX8888_node;
static awk_scalar_t SDL_PIXELFORMAT_XBGR8888_node;
static awk_scalar_t SDL_PIXELFORMAT_BGR888_node;
static awk_scalar_t SDL_PIXELFORMAT_BGRX8888_node;
static awk_scalar_t SDL_PIXELFORMAT_ARGB8888_node;
static awk_scalar_t SDL_PIXELFORMAT_RGBA8888_node;
static awk_scalar_t SDL_PIXELFORMAT_ABGR8888_node;
static awk_scalar_t SDL_PIXELFORMAT_BGRA8888_node;
static awk_scalar_t SDL_PIXELFORMAT_ARGB2101010_node;
static awk_scalar_t SDL_PIXELFORMAT_RGBA32_node;
static awk_scalar_t SDL_PIXELFORMAT_ARGB32_node;
static awk_scalar_t SDL_PIXELFORMAT_BGRA32_node;
static awk_scalar_t SDL_PIXELFORMAT_ABGR32_node;
static awk_scalar_t SDL_PIXELFORMAT_YV12_node;
static awk_scalar_t SDL_PIXELFORMAT_IYUV_node;
static awk_scalar_t SDL_PIXELFORMAT_YUY2_node;
static awk_scalar_t SDL_PIXELFORMAT_UYVY_node;
static awk_scalar_t SDL_PIXELFORMAT_YVYU_node;
static awk_scalar_t SDL_PIXELFORMAT_NV12_node;
static awk_scalar_t SDL_PIXELFORMAT_NV21_node;
static awk_scalar_t SDL_PIXELFORMAT_EXTERNAL_OES_node;

/* Event Handling */
static awk_scalar_t SDL_RELEASED_node;
static awk_scalar_t SDL_PRESSED_node;
static awk_scalar_t SDL_FIRSTEVENT_node;
static awk_scalar_t SDL_QUIT_node;
static awk_scalar_t SDL_APP_TERMINATING_node;
static awk_scalar_t SDL_APP_LOWMEMORY_node;
static awk_scalar_t SDL_APP_WILLENTERBACKGROUND_node;
static awk_scalar_t SDL_APP_DIDENTERBACKGROUND_node;
static awk_scalar_t SDL_APP_WILLENTERFOREGROUND_node;
static awk_scalar_t SDL_APP_DIDENTERFOREGROUND_node;
static awk_scalar_t SDL_LOCALECHANGED_node;
static awk_scalar_t SDL_DISPLAYEVENT_node;
static awk_scalar_t SDL_WINDOWEVENT_node;
static awk_scalar_t SDL_SYSWMEVENT_node;
static awk_scalar_t SDL_KEYDOWN_node;
static awk_scalar_t SDL_KEYUP_node;
static awk_scalar_t SDL_TEXTEDITING_node;
static awk_scalar_t SDL_TEXTINPUT_node;
static awk_scalar_t SDL_KEYMAPCHANGED_node;
static awk_scalar_t SDL_TEXTEDITING_EXT_node;
static awk_scalar_t SDL_MOUSEMOTION_node;
static awk_scalar_t SDL_MOUSEBUTTONDOWN_node;
static awk_scalar_t SDL_MOUSEBUTTONUP_node;
static awk_scalar_t SDL_MOUSEWHEEL_node;
static awk_scalar_t SDL_JOYAXISMOTION_node;
static awk_scalar_t SDL_JOYBALLMOTION_node;
static awk_scalar_t SDL_JOYHATMOTION_node;
static awk_scalar_t SDL_JOYBUTTONDOWN_node;
static awk_scalar_t SDL_JOYBUTTONUP_node;
static awk_scalar_t SDL_JOYDEVICEADDED_node;
static awk_scalar_t SDL_JOYDEVICEREMOVED_node;
static awk_scalar_t SDL_JOYBATTERYUPDATED_node;
static awk_scalar_t SDL_CONTROLLERAXISMOTION_node;
static awk_scalar_t SDL_CONTROLLERBUTTONDOWN_node;
static awk_scalar_t SDL_CONTROLLERBUTTONUP_node;
static awk_scalar_t SDL_CONTROLLERDEVICEADDED_node;
static awk_scalar_t SDL_CONTROLLERDEVICEREMOVED_node;
static awk_scalar_t SDL_CONTROLLERDEVICEREMAPPED_node;
static awk_scalar_t SDL_CONTROLLERTOUCHPADDOWN_node;
static awk_scalar_t SDL_CONTROLLERTOUCHPADMOTION_node;
static awk_scalar_t SDL_CONTROLLERTOUCHPADUP_node;
static awk_scalar_t SDL_CONTROLLERSENSORUPDATE_node;
static awk_scalar_t SDL_FINGERDOWN_node;
static awk_scalar_t SDL_FINGERUP_node;
static awk_scalar_t SDL_FINGERMOTION_node;
static awk_scalar_t SDL_DOLLARGESTURE_node;
static awk_scalar_t SDL_DOLLARRECORD_node;
static awk_scalar_t SDL_MULTIGESTURE_node;
static awk_scalar_t SDL_CLIPBOARDUPDATE_node;
static awk_scalar_t SDL_DROPFILE_node;
static awk_scalar_t SDL_DROPTEXT_node;
static awk_scalar_t SDL_DROPBEGIN_node;
static awk_scalar_t SDL_DROPCOMPLETE_node;
static awk_scalar_t SDL_AUDIODEVICEADDED_node;
static awk_scalar_t SDL_AUDIODEVICEREMOVED_node;
static awk_scalar_t SDL_SENSORUPDATE_node;
static awk_scalar_t SDL_RENDER_TARGETS_RESET_node;
static awk_scalar_t SDL_RENDER_DEVICE_RESET_node;
static awk_scalar_t SDL_POLLSENTINEL_node;
static awk_scalar_t SDL_USEREVENT_node;
static awk_scalar_t SDL_LASTEVENT_node;

/* Keyboard Support */
static awk_scalar_t SDL_SCANCODE_UNKNOWN_node;
static awk_scalar_t SDL_SCANCODE_A_node;
static awk_scalar_t SDL_SCANCODE_B_node;
static awk_scalar_t SDL_SCANCODE_C_node;
static awk_scalar_t SDL_SCANCODE_D_node;
static awk_scalar_t SDL_SCANCODE_E_node;
static awk_scalar_t SDL_SCANCODE_F_node;
static awk_scalar_t SDL_SCANCODE_G_node;
static awk_scalar_t SDL_SCANCODE_H_node;
static awk_scalar_t SDL_SCANCODE_I_node;
static awk_scalar_t SDL_SCANCODE_J_node;
static awk_scalar_t SDL_SCANCODE_K_node;
static awk_scalar_t SDL_SCANCODE_L_node;
static awk_scalar_t SDL_SCANCODE_M_node;
static awk_scalar_t SDL_SCANCODE_N_node;
static awk_scalar_t SDL_SCANCODE_O_node;
static awk_scalar_t SDL_SCANCODE_P_node;
static awk_scalar_t SDL_SCANCODE_Q_node;
static awk_scalar_t SDL_SCANCODE_R_node;
static awk_scalar_t SDL_SCANCODE_S_node;
static awk_scalar_t SDL_SCANCODE_T_node;
static awk_scalar_t SDL_SCANCODE_U_node;
static awk_scalar_t SDL_SCANCODE_V_node;
static awk_scalar_t SDL_SCANCODE_W_node;
static awk_scalar_t SDL_SCANCODE_X_node;
static awk_scalar_t SDL_SCANCODE_Y_node;
static awk_scalar_t SDL_SCANCODE_Z_node;
static awk_scalar_t SDL_SCANCODE_1_node;
static awk_scalar_t SDL_SCANCODE_2_node;
static awk_scalar_t SDL_SCANCODE_3_node;
static awk_scalar_t SDL_SCANCODE_4_node;
static awk_scalar_t SDL_SCANCODE_5_node;
static awk_scalar_t SDL_SCANCODE_6_node;
static awk_scalar_t SDL_SCANCODE_7_node;
static awk_scalar_t SDL_SCANCODE_8_node;
static awk_scalar_t SDL_SCANCODE_9_node;
static awk_scalar_t SDL_SCANCODE_0_node;
static awk_scalar_t SDL_SCANCODE_RETURN_node;
static awk_scalar_t SDL_SCANCODE_ESCAPE_node;
static awk_scalar_t SDL_SCANCODE_BACKSPACE_node;
static awk_scalar_t SDL_SCANCODE_TAB_node;
static awk_scalar_t SDL_SCANCODE_SPACE_node;
static awk_scalar_t SDL_SCANCODE_MINUS_node;
static awk_scalar_t SDL_SCANCODE_EQUALS_node;
static awk_scalar_t SDL_SCANCODE_LEFTBRACKET_node;
static awk_scalar_t SDL_SCANCODE_RIGHTBRACKET_node;
static awk_scalar_t SDL_SCANCODE_BACKSLASH_node;
static awk_scalar_t SDL_SCANCODE_NONUSHASH_node;
static awk_scalar_t SDL_SCANCODE_SEMICOLON_node;
static awk_scalar_t SDL_SCANCODE_APOSTROPHE_node;
static awk_scalar_t SDL_SCANCODE_GRAVE_node;
static awk_scalar_t SDL_SCANCODE_COMMA_node;
static awk_scalar_t SDL_SCANCODE_PERIOD_node;
static awk_scalar_t SDL_SCANCODE_SLASH_node;
static awk_scalar_t SDL_SCANCODE_CAPSLOCK_node;
static awk_scalar_t SDL_SCANCODE_F1_node;
static awk_scalar_t SDL_SCANCODE_F2_node;
static awk_scalar_t SDL_SCANCODE_F3_node;
static awk_scalar_t SDL_SCANCODE_F4_node;
static awk_scalar_t SDL_SCANCODE_F5_node;
static awk_scalar_t SDL_SCANCODE_F6_node;
static awk_scalar_t SDL_SCANCODE_F7_node;
static awk_scalar_t SDL_SCANCODE_F8_node;
static awk_scalar_t SDL_SCANCODE_F9_node;
static awk_scalar_t SDL_SCANCODE_F10_node;
static awk_scalar_t SDL_SCANCODE_F11_node;
static awk_scalar_t SDL_SCANCODE_F12_node;
static awk_scalar_t SDL_SCANCODE_PRINTSCREEN_node;
static awk_scalar_t SDL_SCANCODE_SCROLLLOCK_node;
static awk_scalar_t SDL_SCANCODE_PAUSE_node;
static awk_scalar_t SDL_SCANCODE_INSERT_node;
static awk_scalar_t SDL_SCANCODE_HOME_node;
static awk_scalar_t SDL_SCANCODE_PAGEUP_node;
static awk_scalar_t SDL_SCANCODE_DELETE_node;
static awk_scalar_t SDL_SCANCODE_END_node;
static awk_scalar_t SDL_SCANCODE_PAGEDOWN_node;
static awk_scalar_t SDL_SCANCODE_RIGHT_node;
static awk_scalar_t SDL_SCANCODE_LEFT_node;
static awk_scalar_t SDL_SCANCODE_DOWN_node;
static awk_scalar_t SDL_SCANCODE_UP_node;
static awk_scalar_t SDL_SCANCODE_NUMLOCKCLEAR_node;
static awk_scalar_t SDL_SCANCODE_KP_DIVIDE_node;
static awk_scalar_t SDL_SCANCODE_KP_MULTIPLY_node;
static awk_scalar_t SDL_SCANCODE_KP_MINUS_node;
static awk_scalar_t SDL_SCANCODE_KP_PLUS_node;
static awk_scalar_t SDL_SCANCODE_KP_ENTER_node;
static awk_scalar_t SDL_SCANCODE_KP_1_node;
static awk_scalar_t SDL_SCANCODE_KP_2_node;
static awk_scalar_t SDL_SCANCODE_KP_3_node;
static awk_scalar_t SDL_SCANCODE_KP_4_node;
static awk_scalar_t SDL_SCANCODE_KP_5_node;
static awk_scalar_t SDL_SCANCODE_KP_6_node;
static awk_scalar_t SDL_SCANCODE_KP_7_node;
static awk_scalar_t SDL_SCANCODE_KP_8_node;
static awk_scalar_t SDL_SCANCODE_KP_9_node;
static awk_scalar_t SDL_SCANCODE_KP_0_node;
static awk_scalar_t SDL_SCANCODE_KP_PERIOD_node;
static awk_scalar_t SDL_SCANCODE_NONUSBACKSLASH_node;
static awk_scalar_t SDL_SCANCODE_APPLICATION_node;
static awk_scalar_t SDL_SCANCODE_POWER_node;
static awk_scalar_t SDL_SCANCODE_KP_EQUALS_node;
static awk_scalar_t SDL_SCANCODE_F13_node;
static awk_scalar_t SDL_SCANCODE_F14_node;
static awk_scalar_t SDL_SCANCODE_F15_node;
static awk_scalar_t SDL_SCANCODE_F16_node;
static awk_scalar_t SDL_SCANCODE_F17_node;
static awk_scalar_t SDL_SCANCODE_F18_node;
static awk_scalar_t SDL_SCANCODE_F19_node;
static awk_scalar_t SDL_SCANCODE_F20_node;
static awk_scalar_t SDL_SCANCODE_F21_node;
static awk_scalar_t SDL_SCANCODE_F22_node;
static awk_scalar_t SDL_SCANCODE_F23_node;
static awk_scalar_t SDL_SCANCODE_F24_node;
static awk_scalar_t SDL_SCANCODE_EXECUTE_node;
static awk_scalar_t SDL_SCANCODE_HELP_node;
static awk_scalar_t SDL_SCANCODE_MENU_node;
static awk_scalar_t SDL_SCANCODE_SELECT_node;
static awk_scalar_t SDL_SCANCODE_STOP_node;
static awk_scalar_t SDL_SCANCODE_AGAIN_node;
static awk_scalar_t SDL_SCANCODE_UNDO_node;
static awk_scalar_t SDL_SCANCODE_CUT_node;
static awk_scalar_t SDL_SCANCODE_COPY_node;
static awk_scalar_t SDL_SCANCODE_PASTE_node;
static awk_scalar_t SDL_SCANCODE_FIND_node;
static awk_scalar_t SDL_SCANCODE_MUTE_node;
static awk_scalar_t SDL_SCANCODE_VOLUMEUP_node;
static awk_scalar_t SDL_SCANCODE_VOLUMEDOWN_node;
static awk_scalar_t SDL_SCANCODE_KP_COMMA_node;
static awk_scalar_t SDL_SCANCODE_KP_EQUALSAS400_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL1_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL2_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL3_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL4_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL5_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL6_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL7_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL8_node;
static awk_scalar_t SDL_SCANCODE_INTERNATIONAL9_node;
static awk_scalar_t SDL_SCANCODE_LANG1_node;
static awk_scalar_t SDL_SCANCODE_LANG2_node;
static awk_scalar_t SDL_SCANCODE_LANG3_node;
static awk_scalar_t SDL_SCANCODE_LANG4_node;
static awk_scalar_t SDL_SCANCODE_LANG5_node;
static awk_scalar_t SDL_SCANCODE_LANG6_node;
static awk_scalar_t SDL_SCANCODE_LANG7_node;
static awk_scalar_t SDL_SCANCODE_LANG8_node;
static awk_scalar_t SDL_SCANCODE_LANG9_node;
static awk_scalar_t SDL_SCANCODE_ALTERASE_node;
static awk_scalar_t SDL_SCANCODE_SYSREQ_node;
static awk_scalar_t SDL_SCANCODE_CANCEL_node;
static awk_scalar_t SDL_SCANCODE_CLEAR_node;
static awk_scalar_t SDL_SCANCODE_PRIOR_node;
static awk_scalar_t SDL_SCANCODE_RETURN2_node;
static awk_scalar_t SDL_SCANCODE_SEPARATOR_node;
static awk_scalar_t SDL_SCANCODE_OUT_node;
static awk_scalar_t SDL_SCANCODE_OPER_node;
static awk_scalar_t SDL_SCANCODE_CLEARAGAIN_node;
static awk_scalar_t SDL_SCANCODE_CRSEL_node;
static awk_scalar_t SDL_SCANCODE_EXSEL_node;
static awk_scalar_t SDL_SCANCODE_KP_00_node;
static awk_scalar_t SDL_SCANCODE_KP_000_node;
static awk_scalar_t SDL_SCANCODE_THOUSANDSSEPARATOR_node;
static awk_scalar_t SDL_SCANCODE_DECIMALSEPARATOR_node;
static awk_scalar_t SDL_SCANCODE_CURRENCYUNIT_node;
static awk_scalar_t SDL_SCANCODE_CURRENCYSUBUNIT_node;
static awk_scalar_t SDL_SCANCODE_KP_LEFTPAREN_node;
static awk_scalar_t SDL_SCANCODE_KP_RIGHTPAREN_node;
static awk_scalar_t SDL_SCANCODE_KP_LEFTBRACE_node;
static awk_scalar_t SDL_SCANCODE_KP_RIGHTBRACE_node;
static awk_scalar_t SDL_SCANCODE_KP_TAB_node;
static awk_scalar_t SDL_SCANCODE_KP_BACKSPACE_node;
static awk_scalar_t SDL_SCANCODE_KP_A_node;
static awk_scalar_t SDL_SCANCODE_KP_B_node;
static awk_scalar_t SDL_SCANCODE_KP_C_node;
static awk_scalar_t SDL_SCANCODE_KP_D_node;
static awk_scalar_t SDL_SCANCODE_KP_E_node;
static awk_scalar_t SDL_SCANCODE_KP_F_node;
static awk_scalar_t SDL_SCANCODE_KP_XOR_node;
static awk_scalar_t SDL_SCANCODE_KP_POWER_node;
static awk_scalar_t SDL_SCANCODE_KP_PERCENT_node;
static awk_scalar_t SDL_SCANCODE_KP_LESS_node;
static awk_scalar_t SDL_SCANCODE_KP_GREATER_node;
static awk_scalar_t SDL_SCANCODE_KP_AMPERSAND_node;
static awk_scalar_t SDL_SCANCODE_KP_DBLAMPERSAND_node;
static awk_scalar_t SDL_SCANCODE_KP_VERTICALBAR_node;
static awk_scalar_t SDL_SCANCODE_KP_DBLVERTICALBAR_node;
static awk_scalar_t SDL_SCANCODE_KP_COLON_node;
static awk_scalar_t SDL_SCANCODE_KP_HASH_node;
static awk_scalar_t SDL_SCANCODE_KP_SPACE_node;
static awk_scalar_t SDL_SCANCODE_KP_AT_node;
static awk_scalar_t SDL_SCANCODE_KP_EXCLAM_node;
static awk_scalar_t SDL_SCANCODE_KP_MEMSTORE_node;
static awk_scalar_t SDL_SCANCODE_KP_MEMRECALL_node;
static awk_scalar_t SDL_SCANCODE_KP_MEMCLEAR_node;
static awk_scalar_t SDL_SCANCODE_KP_MEMADD_node;
static awk_scalar_t SDL_SCANCODE_KP_MEMSUBTRACT_node;
static awk_scalar_t SDL_SCANCODE_KP_MEMMULTIPLY_node;
static awk_scalar_t SDL_SCANCODE_KP_MEMDIVIDE_node;
static awk_scalar_t SDL_SCANCODE_KP_PLUSMINUS_node;
static awk_scalar_t SDL_SCANCODE_KP_CLEAR_node;
static awk_scalar_t SDL_SCANCODE_KP_CLEARENTRY_node;
static awk_scalar_t SDL_SCANCODE_KP_BINARY_node;
static awk_scalar_t SDL_SCANCODE_KP_OCTAL_node;
static awk_scalar_t SDL_SCANCODE_KP_DECIMAL_node;
static awk_scalar_t SDL_SCANCODE_KP_HEXADECIMAL_node;
static awk_scalar_t SDL_SCANCODE_LCTRL_node;
static awk_scalar_t SDL_SCANCODE_LSHIFT_node;
static awk_scalar_t SDL_SCANCODE_LALT_node;
static awk_scalar_t SDL_SCANCODE_LGUI_node;
static awk_scalar_t SDL_SCANCODE_RCTRL_node;
static awk_scalar_t SDL_SCANCODE_RSHIFT_node;
static awk_scalar_t SDL_SCANCODE_RALT_node;
static awk_scalar_t SDL_SCANCODE_RGUI_node;
static awk_scalar_t SDL_SCANCODE_MODE_node;
static awk_scalar_t SDL_SCANCODE_AUDIONEXT_node;
static awk_scalar_t SDL_SCANCODE_AUDIOPREV_node;
static awk_scalar_t SDL_SCANCODE_AUDIOSTOP_node;
static awk_scalar_t SDL_SCANCODE_AUDIOPLAY_node;
static awk_scalar_t SDL_SCANCODE_AUDIOMUTE_node;
static awk_scalar_t SDL_SCANCODE_MEDIASELECT_node;
static awk_scalar_t SDL_SCANCODE_WWW_node;
static awk_scalar_t SDL_SCANCODE_MAIL_node;
static awk_scalar_t SDL_SCANCODE_CALCULATOR_node;
static awk_scalar_t SDL_SCANCODE_COMPUTER_node;
static awk_scalar_t SDL_SCANCODE_AC_SEARCH_node;
static awk_scalar_t SDL_SCANCODE_AC_HOME_node;
static awk_scalar_t SDL_SCANCODE_AC_BACK_node;
static awk_scalar_t SDL_SCANCODE_AC_FORWARD_node;
static awk_scalar_t SDL_SCANCODE_AC_STOP_node;
static awk_scalar_t SDL_SCANCODE_AC_REFRESH_node;
static awk_scalar_t SDL_SCANCODE_AC_BOOKMARKS_node;
static awk_scalar_t SDL_SCANCODE_BRIGHTNESSDOWN_node;
static awk_scalar_t SDL_SCANCODE_BRIGHTNESSUP_node;
static awk_scalar_t SDL_SCANCODE_DISPLAYSWITCH_node;
static awk_scalar_t SDL_SCANCODE_KBDILLUMTOGGLE_node;
static awk_scalar_t SDL_SCANCODE_KBDILLUMDOWN_node;
static awk_scalar_t SDL_SCANCODE_KBDILLUMUP_node;
static awk_scalar_t SDL_SCANCODE_EJECT_node;
static awk_scalar_t SDL_SCANCODE_SLEEP_node;
static awk_scalar_t SDL_SCANCODE_APP1_node;
static awk_scalar_t SDL_SCANCODE_APP2_node;
static awk_scalar_t SDL_SCANCODE_AUDIOREWIND_node;
static awk_scalar_t SDL_SCANCODE_AUDIOFASTFORWARD_node;
static awk_scalar_t SDL_SCANCODE_SOFTLEFT_node;
static awk_scalar_t SDL_SCANCODE_SOFTRIGHT_node;
static awk_scalar_t SDL_SCANCODE_CALL_node;
static awk_scalar_t SDL_SCANCODE_ENDCALL_node;

/* Message boxes */
static awk_scalar_t SDL_MESSAGEBOX_ERROR_node;
static awk_scalar_t SDL_MESSAGEBOX_WARNING_node;
static awk_scalar_t SDL_MESSAGEBOX_INFORMATION_node;
static awk_scalar_t SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT_node;
static awk_scalar_t SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT_node;

/* Standard Library Functionality */
static awk_scalar_t SDL_FALSE_node;
static awk_scalar_t SDL_TRUE_node;

struct varinit {
    awk_scalar_t *spec;
    const char *name;
    int dfltval;
    int read_only;
};

static const struct varinit varinit[] = {
    ENTRY(SDL_INIT_TIMER, 1),
    ENTRY(SDL_INIT_AUDIO, 1),
    ENTRY(SDL_INIT_VIDEO, 1),
    ENTRY(SDL_INIT_JOYSTICK, 1),
    ENTRY(SDL_INIT_HAPTIC, 1),
    ENTRY(SDL_INIT_GAMECONTROLLER, 1),
    ENTRY(SDL_INIT_EVENTS, 1),
    ENTRY(SDL_INIT_SENSOR, 1),
    ENTRY(SDL_INIT_NOPARACHUTE, 1),
    ENTRY(SDL_INIT_EVERYTHING, 1),
    ENTRY(SDL_MAJOR_VERSION, 1),
    ENTRY(SDL_MINOR_VERSION, 1),
    ENTRY(SDL_PATCHLEVEL, 1),
    ENTRY(SDL_COMPILEDVERSION, 1),
    ENTRY(SDL_WINDOW_FULLSCREEN, 1),
    ENTRY(SDL_WINDOW_OPENGL, 1),
    ENTRY(SDL_WINDOW_SHOWN, 1),
    ENTRY(SDL_WINDOW_HIDDEN, 1),
    ENTRY(SDL_WINDOW_BORDERLESS, 1),
    ENTRY(SDL_WINDOW_RESIZABLE, 1),
    ENTRY(SDL_WINDOW_MINIMIZED, 1),
    ENTRY(SDL_WINDOW_MAXIMIZED, 1),
    ENTRY(SDL_WINDOW_MOUSE_GRABBED, 1),
    ENTRY(SDL_WINDOW_INPUT_FOCUS, 1),
    ENTRY(SDL_WINDOW_MOUSE_FOCUS, 1),
    ENTRY(SDL_WINDOW_FULLSCREEN_DESKTOP, 1),
    ENTRY(SDL_WINDOW_FOREIGN, 1),
    ENTRY(SDL_WINDOW_ALLOW_HIGHDPI, 1),
    ENTRY(SDL_WINDOW_MOUSE_CAPTURE, 1),
    ENTRY(SDL_WINDOW_ALWAYS_ON_TOP, 1),
    ENTRY(SDL_WINDOW_SKIP_TASKBAR, 1),
    ENTRY(SDL_WINDOW_UTILITY, 1),
    ENTRY(SDL_WINDOW_TOOLTIP, 1),
    ENTRY(SDL_WINDOW_POPUP_MENU, 1),
    ENTRY(SDL_WINDOW_KEYBOARD_GRABBED, 1),
    ENTRY(SDL_WINDOW_VULKAN, 1),
    ENTRY(SDL_WINDOW_METAL, 1),
    ENTRY(SDL_WINDOW_INPUT_GRABBED, 1),
    ENTRY(SDL_WINDOWPOS_UNDEFINED, 1),
    ENTRY(SDL_WINDOWPOS_CENTERED, 1),
    ENTRY(SDL_WINDOWEVENT_NONE, 1),
    ENTRY(SDL_WINDOWEVENT_SHOWN, 1),
    ENTRY(SDL_WINDOWEVENT_HIDDEN, 1),
    ENTRY(SDL_WINDOWEVENT_EXPOSED, 1),
    ENTRY(SDL_WINDOWEVENT_MOVED, 1),
    ENTRY(SDL_WINDOWEVENT_RESIZED, 1),
    ENTRY(SDL_WINDOWEVENT_SIZE_CHANGED, 1),
    ENTRY(SDL_WINDOWEVENT_MINIMIZED, 1),
    ENTRY(SDL_WINDOWEVENT_MAXIMIZED, 1),
    ENTRY(SDL_WINDOWEVENT_RESTORED, 1),
    ENTRY(SDL_WINDOWEVENT_ENTER, 1),
    ENTRY(SDL_WINDOWEVENT_LEAVE, 1),
    ENTRY(SDL_WINDOWEVENT_FOCUS_GAINED, 1),
    ENTRY(SDL_WINDOWEVENT_FOCUS_LOST, 1),
    ENTRY(SDL_WINDOWEVENT_CLOSE, 1),
    ENTRY(SDL_WINDOWEVENT_TAKE_FOCUS, 1),
    ENTRY(SDL_WINDOWEVENT_HIT_TEST, 1),
    ENTRY(SDL_WINDOWEVENT_ICCPROF_CHANGED, 1),
    ENTRY(SDL_WINDOWEVENT_DISPLAY_CHANGED, 1),
    ENTRY(SDL_RENDERER_SOFTWARE, 1),
    ENTRY(SDL_RENDERER_ACCELERATED, 1),
    ENTRY(SDL_RENDERER_PRESENTVSYNC, 1),
    ENTRY(SDL_RENDERER_TARGETTEXTURE, 1),
    ENTRY(SDL_TEXTUREACCESS_STATIC, 1),
    ENTRY(SDL_TEXTUREACCESS_STREAMING, 1),
    ENTRY(SDL_TEXTUREACCESS_TARGET, 1),
    ENTRY(SDL_PIXELFORMAT_UNKNOWN, 1),
    ENTRY(SDL_PIXELFORMAT_INDEX1LSB, 1),
    ENTRY(SDL_PIXELFORMAT_INDEX1MSB, 1),
    ENTRY(SDL_PIXELFORMAT_INDEX4LSB, 1),
    ENTRY(SDL_PIXELFORMAT_INDEX4MSB, 1),
    ENTRY(SDL_PIXELFORMAT_INDEX8, 1),
    ENTRY(SDL_PIXELFORMAT_RGB332, 1),
    ENTRY(SDL_PIXELFORMAT_XRGB4444, 1),
    ENTRY(SDL_PIXELFORMAT_RGB444, 1),
    ENTRY(SDL_PIXELFORMAT_XBGR4444, 1),
    ENTRY(SDL_PIXELFORMAT_BGR444, 1),
    ENTRY(SDL_PIXELFORMAT_XRGB1555, 1),
    ENTRY(SDL_PIXELFORMAT_RGB555, 1),
    ENTRY(SDL_PIXELFORMAT_XBGR1555, 1),
    ENTRY(SDL_PIXELFORMAT_BGR555, 1),
    ENTRY(SDL_PIXELFORMAT_ARGB4444, 1),
    ENTRY(SDL_PIXELFORMAT_RGBA4444, 1),
    ENTRY(SDL_PIXELFORMAT_ABGR4444, 1),
    ENTRY(SDL_PIXELFORMAT_BGRA4444, 1),
    ENTRY(SDL_PIXELFORMAT_ARGB1555, 1),
    ENTRY(SDL_PIXELFORMAT_RGBA5551, 1),
    ENTRY(SDL_PIXELFORMAT_ABGR1555, 1),
    ENTRY(SDL_PIXELFORMAT_BGRA5551, 1),
    ENTRY(SDL_PIXELFORMAT_RGB565, 1),
    ENTRY(SDL_PIXELFORMAT_BGR565, 1),
    ENTRY(SDL_PIXELFORMAT_RGB24, 1),
    ENTRY(SDL_PIXELFORMAT_BGR24, 1),
    ENTRY(SDL_PIXELFORMAT_XRGB8888, 1),
    ENTRY(SDL_PIXELFORMAT_RGB888, 1),
    ENTRY(SDL_PIXELFORMAT_RGBX8888, 1),
    ENTRY(SDL_PIXELFORMAT_XBGR8888, 1),
    ENTRY(SDL_PIXELFORMAT_BGR888, 1),
    ENTRY(SDL_PIXELFORMAT_BGRX8888, 1),
    ENTRY(SDL_PIXELFORMAT_ARGB8888, 1),
    ENTRY(SDL_PIXELFORMAT_RGBA8888, 1),
    ENTRY(SDL_PIXELFORMAT_ABGR8888, 1),
    ENTRY(SDL_PIXELFORMAT_BGRA8888, 1),
    ENTRY(SDL_PIXELFORMAT_ARGB2101010, 1),
    ENTRY(SDL_PIXELFORMAT_RGBA32, 1),
    ENTRY(SDL_PIXELFORMAT_ARGB32, 1),
    ENTRY(SDL_PIXELFORMAT_BGRA32, 1),
    ENTRY(SDL_PIXELFORMAT_ABGR32, 1),
    ENTRY(SDL_PIXELFORMAT_YV12, 1),
    ENTRY(SDL_PIXELFORMAT_IYUV, 1),
    ENTRY(SDL_PIXELFORMAT_YUY2, 1),
    ENTRY(SDL_PIXELFORMAT_UYVY, 1),
    ENTRY(SDL_PIXELFORMAT_YVYU, 1),
    ENTRY(SDL_PIXELFORMAT_NV12, 1),
    ENTRY(SDL_PIXELFORMAT_NV21, 1),
    ENTRY(SDL_PIXELFORMAT_EXTERNAL_OES, 1),
    ENTRY(SDL_RELEASED, 1),
    ENTRY(SDL_PRESSED, 1),
    ENTRY(SDL_FIRSTEVENT, 1),
    ENTRY(SDL_QUIT, 1),
    ENTRY(SDL_APP_TERMINATING, 1),
    ENTRY(SDL_APP_LOWMEMORY, 1),
    ENTRY(SDL_APP_WILLENTERBACKGROUND, 1),
    ENTRY(SDL_APP_DIDENTERBACKGROUND, 1),
    ENTRY(SDL_APP_WILLENTERFOREGROUND, 1),
    ENTRY(SDL_APP_DIDENTERFOREGROUND, 1),
    ENTRY(SDL_LOCALECHANGED, 1),
    ENTRY(SDL_DISPLAYEVENT, 1),
    ENTRY(SDL_WINDOWEVENT, 1),
    ENTRY(SDL_SYSWMEVENT, 1),
    ENTRY(SDL_KEYDOWN, 1),
    ENTRY(SDL_KEYUP, 1),
    ENTRY(SDL_TEXTEDITING, 1),
    ENTRY(SDL_TEXTINPUT, 1),
    ENTRY(SDL_KEYMAPCHANGED, 1),
    ENTRY(SDL_TEXTEDITING_EXT, 1),
    ENTRY(SDL_MOUSEMOTION, 1),
    ENTRY(SDL_MOUSEBUTTONDOWN, 1),
    ENTRY(SDL_MOUSEBUTTONUP, 1),
    ENTRY(SDL_MOUSEWHEEL, 1),
    ENTRY(SDL_JOYAXISMOTION, 1),
    ENTRY(SDL_JOYBALLMOTION, 1),
    ENTRY(SDL_JOYHATMOTION, 1),
    ENTRY(SDL_JOYBUTTONDOWN, 1),
    ENTRY(SDL_JOYBUTTONUP, 1),
    ENTRY(SDL_JOYDEVICEADDED, 1),
    ENTRY(SDL_JOYDEVICEREMOVED, 1),
    ENTRY(SDL_JOYBATTERYUPDATED, 1),
    ENTRY(SDL_CONTROLLERAXISMOTION, 1),
    ENTRY(SDL_CONTROLLERBUTTONDOWN, 1),
    ENTRY(SDL_CONTROLLERBUTTONUP, 1),
    ENTRY(SDL_CONTROLLERDEVICEADDED, 1),
    ENTRY(SDL_CONTROLLERDEVICEREMOVED, 1),
    ENTRY(SDL_CONTROLLERDEVICEREMAPPED, 1),
    ENTRY(SDL_CONTROLLERTOUCHPADDOWN, 1),
    ENTRY(SDL_CONTROLLERTOUCHPADMOTION, 1),
    ENTRY(SDL_CONTROLLERTOUCHPADUP, 1),
    ENTRY(SDL_CONTROLLERSENSORUPDATE, 1),
    ENTRY(SDL_FINGERDOWN, 1),
    ENTRY(SDL_FINGERUP, 1),
    ENTRY(SDL_FINGERMOTION, 1),
    ENTRY(SDL_DOLLARGESTURE, 1),
    ENTRY(SDL_DOLLARRECORD, 1),
    ENTRY(SDL_MULTIGESTURE, 1),
    ENTRY(SDL_CLIPBOARDUPDATE, 1),
    ENTRY(SDL_DROPFILE, 1),
    ENTRY(SDL_DROPTEXT, 1),
    ENTRY(SDL_DROPBEGIN, 1),
    ENTRY(SDL_DROPCOMPLETE, 1),
    ENTRY(SDL_AUDIODEVICEADDED, 1),
    ENTRY(SDL_AUDIODEVICEREMOVED, 1),
    ENTRY(SDL_SENSORUPDATE, 1),
    ENTRY(SDL_RENDER_TARGETS_RESET, 1),
    ENTRY(SDL_RENDER_DEVICE_RESET, 1),
    ENTRY(SDL_POLLSENTINEL, 1),
    ENTRY(SDL_USEREVENT, 1),
    ENTRY(SDL_LASTEVENT, 1),
    ENTRY(SDL_SCANCODE_UNKNOWN, 1),
    ENTRY(SDL_SCANCODE_A, 1),
    ENTRY(SDL_SCANCODE_B, 1),
    ENTRY(SDL_SCANCODE_C, 1),
    ENTRY(SDL_SCANCODE_D, 1),
    ENTRY(SDL_SCANCODE_E, 1),
    ENTRY(SDL_SCANCODE_F, 1),
    ENTRY(SDL_SCANCODE_G, 1),
    ENTRY(SDL_SCANCODE_H, 1),
    ENTRY(SDL_SCANCODE_I, 1),
    ENTRY(SDL_SCANCODE_J, 1),
    ENTRY(SDL_SCANCODE_K, 1),
    ENTRY(SDL_SCANCODE_L, 1),
    ENTRY(SDL_SCANCODE_M, 1),
    ENTRY(SDL_SCANCODE_N, 1),
    ENTRY(SDL_SCANCODE_O, 1),
    ENTRY(SDL_SCANCODE_P, 1),
    ENTRY(SDL_SCANCODE_Q, 1),
    ENTRY(SDL_SCANCODE_R, 1),
    ENTRY(SDL_SCANCODE_S, 1),
    ENTRY(SDL_SCANCODE_T, 1),
    ENTRY(SDL_SCANCODE_U, 1),
    ENTRY(SDL_SCANCODE_V, 1),
    ENTRY(SDL_SCANCODE_W, 1),
    ENTRY(SDL_SCANCODE_X, 1),
    ENTRY(SDL_SCANCODE_Y, 1),
    ENTRY(SDL_SCANCODE_Z, 1),
    ENTRY(SDL_SCANCODE_1, 1),
    ENTRY(SDL_SCANCODE_2, 1),
    ENTRY(SDL_SCANCODE_3, 1),
    ENTRY(SDL_SCANCODE_4, 1),
    ENTRY(SDL_SCANCODE_5, 1),
    ENTRY(SDL_SCANCODE_6, 1),
    ENTRY(SDL_SCANCODE_7, 1),
    ENTRY(SDL_SCANCODE_8, 1),
    ENTRY(SDL_SCANCODE_9, 1),
    ENTRY(SDL_SCANCODE_0, 1),
    ENTRY(SDL_SCANCODE_RETURN, 1),
    ENTRY(SDL_SCANCODE_ESCAPE, 1),
    ENTRY(SDL_SCANCODE_BACKSPACE, 1),
    ENTRY(SDL_SCANCODE_TAB, 1),
    ENTRY(SDL_SCANCODE_SPACE, 1),
    ENTRY(SDL_SCANCODE_MINUS, 1),
    ENTRY(SDL_SCANCODE_EQUALS, 1),
    ENTRY(SDL_SCANCODE_LEFTBRACKET, 1),
    ENTRY(SDL_SCANCODE_RIGHTBRACKET, 1),
    ENTRY(SDL_SCANCODE_BACKSLASH, 1),
    ENTRY(SDL_SCANCODE_NONUSHASH, 1),
    ENTRY(SDL_SCANCODE_SEMICOLON, 1),
    ENTRY(SDL_SCANCODE_APOSTROPHE, 1),
    ENTRY(SDL_SCANCODE_GRAVE, 1),
    ENTRY(SDL_SCANCODE_COMMA, 1),
    ENTRY(SDL_SCANCODE_PERIOD, 1),
    ENTRY(SDL_SCANCODE_SLASH, 1),
    ENTRY(SDL_SCANCODE_CAPSLOCK, 1),
    ENTRY(SDL_SCANCODE_F1, 1),
    ENTRY(SDL_SCANCODE_F2, 1),
    ENTRY(SDL_SCANCODE_F3, 1),
    ENTRY(SDL_SCANCODE_F4, 1),
    ENTRY(SDL_SCANCODE_F5, 1),
    ENTRY(SDL_SCANCODE_F6, 1),
    ENTRY(SDL_SCANCODE_F7, 1),
    ENTRY(SDL_SCANCODE_F8, 1),
    ENTRY(SDL_SCANCODE_F9, 1),
    ENTRY(SDL_SCANCODE_F10, 1),
    ENTRY(SDL_SCANCODE_F11, 1),
    ENTRY(SDL_SCANCODE_F12, 1),
    ENTRY(SDL_SCANCODE_PRINTSCREEN, 1),
    ENTRY(SDL_SCANCODE_SCROLLLOCK, 1),
    ENTRY(SDL_SCANCODE_PAUSE, 1),
    ENTRY(SDL_SCANCODE_INSERT, 1),
    ENTRY(SDL_SCANCODE_HOME, 1),
    ENTRY(SDL_SCANCODE_PAGEUP, 1),
    ENTRY(SDL_SCANCODE_DELETE, 1),
    ENTRY(SDL_SCANCODE_END, 1),
    ENTRY(SDL_SCANCODE_PAGEDOWN, 1),
    ENTRY(SDL_SCANCODE_RIGHT, 1),
    ENTRY(SDL_SCANCODE_LEFT, 1),
    ENTRY(SDL_SCANCODE_DOWN, 1),
    ENTRY(SDL_SCANCODE_UP, 1),
    ENTRY(SDL_SCANCODE_NUMLOCKCLEAR, 1),
    ENTRY(SDL_SCANCODE_KP_DIVIDE, 1),
    ENTRY(SDL_SCANCODE_KP_MULTIPLY, 1),
    ENTRY(SDL_SCANCODE_KP_MINUS, 1),
    ENTRY(SDL_SCANCODE_KP_PLUS, 1),
    ENTRY(SDL_SCANCODE_KP_ENTER, 1),
    ENTRY(SDL_SCANCODE_KP_1, 1),
    ENTRY(SDL_SCANCODE_KP_2, 1),
    ENTRY(SDL_SCANCODE_KP_3, 1),
    ENTRY(SDL_SCANCODE_KP_4, 1),
    ENTRY(SDL_SCANCODE_KP_5, 1),
    ENTRY(SDL_SCANCODE_KP_6, 1),
    ENTRY(SDL_SCANCODE_KP_7, 1),
    ENTRY(SDL_SCANCODE_KP_8, 1),
    ENTRY(SDL_SCANCODE_KP_9, 1),
    ENTRY(SDL_SCANCODE_KP_0, 1),
    ENTRY(SDL_SCANCODE_KP_PERIOD, 1),
    ENTRY(SDL_SCANCODE_NONUSBACKSLASH, 1),
    ENTRY(SDL_SCANCODE_APPLICATION, 1),
    ENTRY(SDL_SCANCODE_POWER, 1),
    ENTRY(SDL_SCANCODE_KP_EQUALS, 1),
    ENTRY(SDL_SCANCODE_F13, 1),
    ENTRY(SDL_SCANCODE_F14, 1),
    ENTRY(SDL_SCANCODE_F15, 1),
    ENTRY(SDL_SCANCODE_F16, 1),
    ENTRY(SDL_SCANCODE_F17, 1),
    ENTRY(SDL_SCANCODE_F18, 1),
    ENTRY(SDL_SCANCODE_F19, 1),
    ENTRY(SDL_SCANCODE_F20, 1),
    ENTRY(SDL_SCANCODE_F21, 1),
    ENTRY(SDL_SCANCODE_F22, 1),
    ENTRY(SDL_SCANCODE_F23, 1),
    ENTRY(SDL_SCANCODE_F24, 1),
    ENTRY(SDL_SCANCODE_EXECUTE, 1),
    ENTRY(SDL_SCANCODE_HELP, 1),
    ENTRY(SDL_SCANCODE_MENU, 1),
    ENTRY(SDL_SCANCODE_SELECT, 1),
    ENTRY(SDL_SCANCODE_STOP, 1),
    ENTRY(SDL_SCANCODE_AGAIN, 1),
    ENTRY(SDL_SCANCODE_UNDO, 1),
    ENTRY(SDL_SCANCODE_CUT, 1),
    ENTRY(SDL_SCANCODE_COPY, 1),
    ENTRY(SDL_SCANCODE_PASTE, 1),
    ENTRY(SDL_SCANCODE_FIND, 1),
    ENTRY(SDL_SCANCODE_MUTE, 1),
    ENTRY(SDL_SCANCODE_VOLUMEUP, 1),
    ENTRY(SDL_SCANCODE_VOLUMEDOWN, 1),
    ENTRY(SDL_SCANCODE_KP_COMMA, 1),
    ENTRY(SDL_SCANCODE_KP_EQUALSAS400, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL1, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL2, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL3, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL4, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL5, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL6, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL7, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL8, 1),
    ENTRY(SDL_SCANCODE_INTERNATIONAL9, 1),
    ENTRY(SDL_SCANCODE_LANG1, 1),
    ENTRY(SDL_SCANCODE_LANG2, 1),
    ENTRY(SDL_SCANCODE_LANG3, 1),
    ENTRY(SDL_SCANCODE_LANG4, 1),
    ENTRY(SDL_SCANCODE_LANG5, 1),
    ENTRY(SDL_SCANCODE_LANG6, 1),
    ENTRY(SDL_SCANCODE_LANG7, 1),
    ENTRY(SDL_SCANCODE_LANG8, 1),
    ENTRY(SDL_SCANCODE_LANG9, 1),
    ENTRY(SDL_SCANCODE_ALTERASE, 1),
    ENTRY(SDL_SCANCODE_SYSREQ, 1),
    ENTRY(SDL_SCANCODE_CANCEL, 1),
    ENTRY(SDL_SCANCODE_CLEAR, 1),
    ENTRY(SDL_SCANCODE_PRIOR, 1),
    ENTRY(SDL_SCANCODE_RETURN2, 1),
    ENTRY(SDL_SCANCODE_SEPARATOR, 1),
    ENTRY(SDL_SCANCODE_OUT, 1),
    ENTRY(SDL_SCANCODE_OPER, 1),
    ENTRY(SDL_SCANCODE_CLEARAGAIN, 1),
    ENTRY(SDL_SCANCODE_CRSEL, 1),
    ENTRY(SDL_SCANCODE_EXSEL, 1),
    ENTRY(SDL_SCANCODE_KP_00, 1),
    ENTRY(SDL_SCANCODE_KP_000, 1),
    ENTRY(SDL_SCANCODE_THOUSANDSSEPARATOR, 1),
    ENTRY(SDL_SCANCODE_DECIMALSEPARATOR, 1),
    ENTRY(SDL_SCANCODE_CURRENCYUNIT, 1),
    ENTRY(SDL_SCANCODE_CURRENCYSUBUNIT, 1),
    ENTRY(SDL_SCANCODE_KP_LEFTPAREN, 1),
    ENTRY(SDL_SCANCODE_KP_RIGHTPAREN, 1),
    ENTRY(SDL_SCANCODE_KP_LEFTBRACE, 1),
    ENTRY(SDL_SCANCODE_KP_RIGHTBRACE, 1),
    ENTRY(SDL_SCANCODE_KP_TAB, 1),
    ENTRY(SDL_SCANCODE_KP_BACKSPACE, 1),
    ENTRY(SDL_SCANCODE_KP_A, 1),
    ENTRY(SDL_SCANCODE_KP_B, 1),
    ENTRY(SDL_SCANCODE_KP_C, 1),
    ENTRY(SDL_SCANCODE_KP_D, 1),
    ENTRY(SDL_SCANCODE_KP_E, 1),
    ENTRY(SDL_SCANCODE_KP_F, 1),
    ENTRY(SDL_SCANCODE_KP_XOR, 1),
    ENTRY(SDL_SCANCODE_KP_POWER, 1),
    ENTRY(SDL_SCANCODE_KP_PERCENT, 1),
    ENTRY(SDL_SCANCODE_KP_LESS, 1),
    ENTRY(SDL_SCANCODE_KP_GREATER, 1),
    ENTRY(SDL_SCANCODE_KP_AMPERSAND, 1),
    ENTRY(SDL_SCANCODE_KP_DBLAMPERSAND, 1),
    ENTRY(SDL_SCANCODE_KP_VERTICALBAR, 1),
    ENTRY(SDL_SCANCODE_KP_DBLVERTICALBAR, 1),
    ENTRY(SDL_SCANCODE_KP_COLON, 1),
    ENTRY(SDL_SCANCODE_KP_HASH, 1),
    ENTRY(SDL_SCANCODE_KP_SPACE, 1),
    ENTRY(SDL_SCANCODE_KP_AT, 1),
    ENTRY(SDL_SCANCODE_KP_EXCLAM, 1),
    ENTRY(SDL_SCANCODE_KP_MEMSTORE, 1),
    ENTRY(SDL_SCANCODE_KP_MEMRECALL, 1),
    ENTRY(SDL_SCANCODE_KP_MEMCLEAR, 1),
    ENTRY(SDL_SCANCODE_KP_MEMADD, 1),
    ENTRY(SDL_SCANCODE_KP_MEMSUBTRACT, 1),
    ENTRY(SDL_SCANCODE_KP_MEMMULTIPLY, 1),
    ENTRY(SDL_SCANCODE_KP_MEMDIVIDE, 1),
    ENTRY(SDL_SCANCODE_KP_PLUSMINUS, 1),
    ENTRY(SDL_SCANCODE_KP_CLEAR, 1),
    ENTRY(SDL_SCANCODE_KP_CLEARENTRY, 1),
    ENTRY(SDL_SCANCODE_KP_BINARY, 1),
    ENTRY(SDL_SCANCODE_KP_OCTAL, 1),
    ENTRY(SDL_SCANCODE_KP_DECIMAL, 1),
    ENTRY(SDL_SCANCODE_KP_HEXADECIMAL, 1),
    ENTRY(SDL_SCANCODE_LCTRL, 1),
    ENTRY(SDL_SCANCODE_LSHIFT, 1),
    ENTRY(SDL_SCANCODE_LALT, 1),
    ENTRY(SDL_SCANCODE_LGUI, 1),
    ENTRY(SDL_SCANCODE_RCTRL, 1),
    ENTRY(SDL_SCANCODE_RSHIFT, 1),
    ENTRY(SDL_SCANCODE_RALT, 1),
    ENTRY(SDL_SCANCODE_RGUI, 1),
    ENTRY(SDL_SCANCODE_MODE, 1),
    ENTRY(SDL_SCANCODE_AUDIONEXT, 1),
    ENTRY(SDL_SCANCODE_AUDIOPREV, 1),
    ENTRY(SDL_SCANCODE_AUDIOSTOP, 1),
    ENTRY(SDL_SCANCODE_AUDIOPLAY, 1),
    ENTRY(SDL_SCANCODE_AUDIOMUTE, 1),
    ENTRY(SDL_SCANCODE_MEDIASELECT, 1),
    ENTRY(SDL_SCANCODE_WWW, 1),
    ENTRY(SDL_SCANCODE_MAIL, 1),
    ENTRY(SDL_SCANCODE_CALCULATOR, 1),
    ENTRY(SDL_SCANCODE_COMPUTER, 1),
    ENTRY(SDL_SCANCODE_AC_SEARCH, 1),
    ENTRY(SDL_SCANCODE_AC_HOME, 1),
    ENTRY(SDL_SCANCODE_AC_BACK, 1),
    ENTRY(SDL_SCANCODE_AC_FORWARD, 1),
    ENTRY(SDL_SCANCODE_AC_STOP, 1),
    ENTRY(SDL_SCANCODE_AC_REFRESH, 1),
    ENTRY(SDL_SCANCODE_AC_BOOKMARKS, 1),
    ENTRY(SDL_SCANCODE_BRIGHTNESSDOWN, 1),
    ENTRY(SDL_SCANCODE_BRIGHTNESSUP, 1),
    ENTRY(SDL_SCANCODE_DISPLAYSWITCH, 1),
    ENTRY(SDL_SCANCODE_KBDILLUMTOGGLE, 1),
    ENTRY(SDL_SCANCODE_KBDILLUMDOWN, 1),
    ENTRY(SDL_SCANCODE_KBDILLUMUP, 1),
    ENTRY(SDL_SCANCODE_EJECT, 1),
    ENTRY(SDL_SCANCODE_SLEEP, 1),
    ENTRY(SDL_SCANCODE_APP1, 1),
    ENTRY(SDL_SCANCODE_APP2, 1),
    ENTRY(SDL_SCANCODE_AUDIOREWIND, 1),
    ENTRY(SDL_SCANCODE_AUDIOFASTFORWARD, 1),
    ENTRY(SDL_SCANCODE_SOFTLEFT, 1),
    ENTRY(SDL_SCANCODE_SOFTRIGHT, 1),
    ENTRY(SDL_SCANCODE_CALL, 1),
    ENTRY(SDL_SCANCODE_ENDCALL, 1),
    ENTRY(SDL_MESSAGEBOX_ERROR, 1),
    ENTRY(SDL_MESSAGEBOX_WARNING, 1),
    ENTRY(SDL_MESSAGEBOX_INFORMATION, 1),
    ENTRY(SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT, 1),
    ENTRY(SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT, 1),
    ENTRY(SDL_FALSE, 1),
    ENTRY(SDL_TRUE, 1),
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
            fatal(ext_id, _("sdl2: unable to initialize %s"), vp->name);
    }
}

/*----- Initialization and Shutdown ----------------------------------------*/

/* int SDL_Init(Uint32 flags); */
/* do_SDL_Init --- provide a SDL_Init() function for gawk */

static awk_value_t *
do_SDL_Init(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t flags_param;
    uint32_t flags;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &flags_param)) {
        warning(ext_id, _("SDL_Init: bad parameter(s)"));
        RETURN_NOK;
    }

    flags = flags_param.num_value;

    ret = SDL_Init(flags);
    if (ret < 0)
        update_ERRNO_string(_("SDL_Init failed"));

    return make_number(ret, result);
}

/* void SDL_Quit(void); */
/* do_SDL_Quit --- provide a SDL_Quit() function for gawk */

static awk_value_t *
do_SDL_Quit(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    SDL_Quit();
    RETURN_OK;
}

/* void SDL_QuitSubSystem(Uint32 flags); */
/* do_SDL_QuitSubSystem --- provide a SDL_QuitSubSystem() function for gawk */

static awk_value_t *
do_SDL_QuitSubSystem(int nargs,
                     awk_value_t *result,
                     struct awk_ext_func *finfo)
{
    awk_value_t flags_param;
    uint32_t flags;

    if (! get_argument(0, AWK_NUMBER, &flags_param)) {
        warning(ext_id, _("SDL_QuitSubSystem: bad parameter(s)"));
        RETURN_NOK;
    }

    flags = flags_param.num_value;

    SDL_QuitSubSystem(flags);
    RETURN_OK;
}

/*----- Error Handling -----------------------------------------------------*/

/* const char *SDL_GetError(void); */
/* do_SDL_GetError --- provide a SDL_GetError() function for gawk */

static awk_value_t *
do_SDL_GetError(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    const char *errormsg;

    errormsg = SDL_GetError();

    if (errormsg)
        return make_string_malloc(errormsg, strlen(errormsg), result);

    update_ERRNO_string(_("SDL_GetError failed"));
    return make_null_string(result);
}

/*----- Querying SDL Version -----------------------------------------------*/

/* SDL_VERSIONNUM(X, Y, Z) */
/* do_SDL_VERSIONNUM --- provide a SDL_VERSIONNUM() macro for gawk */

static awk_value_t *
do_SDL_VERSIONNUM(int nargs,
                  awk_value_t *result,
                  struct awk_ext_func *finfo)
{
    awk_value_t X_param, Y_param, Z_param;
    uint8_t X, Y, Z;

    if (! get_argument(0, AWK_NUMBER, &X_param)
        || ! get_argument(1, AWK_NUMBER, &Y_param)
        || ! get_argument(2, AWK_NUMBER, &Z_param)) {
        warning(ext_id, _("SDL_VERSIONNUM: bad parameter(s)"));
        RETURN_NOK;
    }

    X = X_param.num_value;
    Y = Y_param.num_value;
    Z = Z_param.num_value;

    return make_number(X * 1000 + Y * 100 + Z, result);
}

/*----- Display and Window Management --------------------------------------*/

/* SDL_Window *SDL_CreateWindow(const char *title,
                                int x, int y,
                                int w, int h,
                                Uint32 flags); */
/* do_SDL_CreateWindow --- provide a SDL_CreateWindow() function for gawk */

static awk_value_t *
do_SDL_CreateWindow(int nargs,
                    awk_value_t *result,
                    struct awk_ext_func *finfo)
{
    SDL_Window *window;
    awk_value_t title_param;
    awk_value_t x_param, y_param;
    awk_value_t w_param, h_param;
    awk_value_t flags_param;
    const char *title;
    int x, y;
    int w, h;
    uint32_t flags;

    if (! get_argument(0, AWK_STRING, &title_param)
        || ! get_argument(1, AWK_NUMBER, &x_param)
        || ! get_argument(2, AWK_NUMBER, &y_param)
        || ! get_argument(3, AWK_NUMBER, &w_param)
        || ! get_argument(4, AWK_NUMBER, &h_param)
        || ! get_argument(5, AWK_NUMBER, &flags_param)) {
        warning(ext_id, _("SDL_CreateWindow: bad parameter(s)"));
        RETURN_NOK;
    }

    title = title_param.str_value.str;
    x = x_param.num_value;
    y = y_param.num_value;
    w = w_param.num_value;
    h = h_param.num_value;
    flags = flags_param.num_value;

    window = SDL_CreateWindow(title, x, y, w, h, flags);

    if (window) {
        char window_addr[20];
        // NOLINTNEXTLINE
        snprintf(window_addr, sizeof(window_addr), "%p", window);
        return make_string_malloc(window_addr, strlen(window_addr), result);
    }

    update_ERRNO_string(_("SDL_CreateWindow failed"));
    return make_null_string(result);
}

/* void SDL_DestroyWindow(SDL_Window *window); */
/* do_SDL_DestroyWindow --- provide a SDL_DestroyWindow() function for gawk */

static awk_value_t *
do_SDL_DestroyWindow(int nargs,
                     awk_value_t *result,
                     struct awk_ext_func *finfo)
{
    awk_value_t window_ptr_param;
    uintptr_t window_ptr;

    if (! get_argument(0, AWK_STRING, &window_ptr_param)) {
        warning(ext_id, _("SDL_DestroyWindow: bad parameter(s)"));
        RETURN_NOK;
    }

    window_ptr = strtoull(window_ptr_param.str_value.str, (char **)NULL, 16);

    SDL_DestroyWindow((SDL_Window *)window_ptr);
    RETURN_OK;
}

/* Uint32 SDL_GetWindowPixelFormat(SDL_Window *window); */
/* do_SDL_GetWindowPixelFormat --- provide a SDL_GetWindowPixelFormat()
                                   function for gawk */

static awk_value_t *
do_SDL_GetWindowPixelFormat(int nargs,
                            awk_value_t *result,
                            struct awk_ext_func *finfo)
{
    awk_value_t window_ptr_param;
    uintptr_t window_ptr;
    uint32_t format;

    if (! get_argument(0, AWK_STRING, &window_ptr_param)) {
        warning(ext_id, _("SDL_GetWindowPixelFormat: bad parameter(s)"));
        RETURN_NOK;
    }

    window_ptr = strtoull(window_ptr_param.str_value.str, (char **)NULL, 16);

    format = SDL_GetWindowPixelFormat((SDL_Window *)window_ptr);
    return make_number(format, result);
}

/* void SDL_SetWindowTitle(SDL_Window *window, const char *title); */
/* do_SDL_SetWindowTitle --- provide a SDL_SetWindowTitle()
                             function for gawk */

static awk_value_t *
do_SDL_SetWindowTitle(int nargs,
                      awk_value_t *result,
                      struct awk_ext_func *finfo)
{
    awk_value_t window_ptr_param;
    awk_value_t title_param;
    uintptr_t window_ptr;
    const char *title;

    if (! get_argument(0, AWK_STRING, &window_ptr_param)
        || ! get_argument(1, AWK_STRING, &title_param)) {
        warning(ext_id, _("SDL_SetWindowTitle: bad parameter(s)"));
        RETURN_NOK;
    }

    window_ptr = strtoull(window_ptr_param.str_value.str, (char **)NULL, 16);
    title = title_param.str_value.str;

    SDL_SetWindowTitle((SDL_Window *)window_ptr, title);
    RETURN_OK;
}

/*----- 2D Accelerated Rendering -------------------------------------------*/

/* SDL_Renderer *SDL_CreateRenderer(SDL_Window *window,
                                    int index,
                                    Uint32 flags); */
/* do_SDL_CreateRenderer --- provide a SDL_CreateRenderer()
                             function for gawk */

static awk_value_t *
do_SDL_CreateRenderer(int nargs,
                      awk_value_t *result,
                      struct awk_ext_func *finfo)
{
    SDL_Renderer *renderer;
    awk_value_t window_ptr_param;
    awk_value_t index_param;
    awk_value_t flags_param;
    uintptr_t window_ptr;
    int index;
    uint32_t flags;

    if (! get_argument(0, AWK_STRING, &window_ptr_param)
        || ! get_argument(1, AWK_NUMBER, &index_param)
        || ! get_argument(2, AWK_NUMBER, &flags_param)) {
        warning(ext_id, _("SDL_CreateRenderer: bad parameter(s)"));
        RETURN_NOK;
    }

    window_ptr = strtoull(window_ptr_param.str_value.str, (char **)NULL, 16);
    index = index_param.num_value;
    flags = flags_param.num_value;

    renderer = SDL_CreateRenderer((SDL_Window *)window_ptr, index, flags);

    if (renderer) {
        char renderer_addr[20];
        // NOLINTNEXTLINE
        snprintf(renderer_addr, sizeof(renderer_addr), "%p", renderer);
        return make_string_malloc(renderer_addr, strlen(renderer_addr),
                                  result);
    }

    update_ERRNO_string(_("SDL_CreateRenderer failed"));
    return make_null_string(result);
}

/* void SDL_DestroyRenderer(SDL_Renderer *renderer); */
/* do_SDL_DestroyRenderer --- provide a SDL_DestroyRenderer()
                              function for gawk */

static awk_value_t *
do_SDL_DestroyRenderer(int nargs,
                       awk_value_t *result,
                       struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    uintptr_t renderer_ptr;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)) {
        warning(ext_id, _("SDL_DestroyRenderer: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);

    SDL_DestroyRenderer((SDL_Renderer *)renderer_ptr);
    RETURN_OK;
}

/* int SDL_RenderClear(SDL_Renderer *renderer); */
/* do_SDL_RenderClear --- provide a SDL_RenderClear() function for gawk */

static awk_value_t *
do_SDL_RenderClear(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    uintptr_t renderer_ptr;
    int ret;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)) {
        warning(ext_id, _("SDL_RenderClear: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);

    ret = SDL_RenderClear((SDL_Renderer *)renderer_ptr);
    if (ret < 0)
        update_ERRNO_string(_("SDL_RenderClear failed"));

    return make_number(ret, result);
}

/* int SDL_RenderCopy(SDL_Renderer *renderer,
                      SDL_Texture *texture,
                      const SDL_Rect *srcrect,
                      const SDL_Rect *dstrect); */
/* do_SDL_RenderCopy --- provide a SDL_RenderCopy() function for gawk */

static awk_value_t *
do_SDL_RenderCopy(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    awk_value_t texture_ptr_param;
    awk_value_t srcrect_ptr_param;
    awk_value_t dstrect_ptr_param;
    uintptr_t renderer_ptr;
    uintptr_t texture_ptr;
    uintptr_t srcrect_ptr;
    uintptr_t dstrect_ptr;
    int ret;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)
        || ! get_argument(1, AWK_STRING, &texture_ptr_param)
        || ! get_argument(2, AWK_STRING, &srcrect_ptr_param)
        || ! get_argument(3, AWK_STRING, &dstrect_ptr_param)) {
        warning(ext_id, _("SDL_RenderCopy: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);
    texture_ptr = strtoull(texture_ptr_param.str_value.str,
                           (char **)NULL,
                           16);
    srcrect_ptr = strtoull(srcrect_ptr_param.str_value.str,
                           (char **)NULL,
                           16);
    dstrect_ptr = strtoull(dstrect_ptr_param.str_value.str,
                           (char **)NULL,
                           16);

    ret = SDL_RenderCopy((SDL_Renderer *)renderer_ptr,
                         (SDL_Texture *)texture_ptr,
                         (const SDL_Rect *)srcrect_ptr,
                         (const SDL_Rect *)dstrect_ptr);
    if (ret < 0)
        update_ERRNO_string(_("SDL_RenderCopy failed"));

    return make_number(ret, result);
}

/* int SDL_RenderDrawLine(SDL_Renderer *renderer,
                          int x1, int y1,
                          int x2, int y2); */
/* do_SDL_RenderDrawLine --- provide a SDL_RenderDrawLine()
                             function for gawk */

static awk_value_t *
do_SDL_RenderDrawLine(int nargs,
                      awk_value_t *result,
                      struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    awk_value_t x1_param, y1_param;
    awk_value_t x2_param, y2_param;
    uintptr_t renderer_ptr;
    int x1, y1;
    int x2, y2;
    int ret;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)
        || ! get_argument(1, AWK_NUMBER, &x1_param)
        || ! get_argument(2, AWK_NUMBER, &y1_param)
        || ! get_argument(3, AWK_NUMBER, &x2_param)
        || ! get_argument(4, AWK_NUMBER, &y2_param)) {
        warning(ext_id, _("SDL_RenderDrawLine: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);
    x1 = x1_param.num_value;
    y1 = y1_param.num_value;
    x2 = x2_param.num_value;
    y2 = y2_param.num_value;

    ret = SDL_RenderDrawLine((SDL_Renderer *)renderer_ptr, x1, y1, x2, y2);
    if (ret < 0)
        update_ERRNO_string(_("SDL_RenderDrawLine failed"));

    return make_number(ret, result);
}

/* int SDL_RenderDrawRect(SDL_Renderer *renderer, const SDL_Rect *rect); */
/* do_SDL_RenderDrawRect --- provide a SDL_RenderDrawRect()
                             function for gawk */

static awk_value_t *
do_SDL_RenderDrawRect(int nargs,
                      awk_value_t *result,
                      struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    awk_value_t rect_ptr_param;
    uintptr_t renderer_ptr;
    uintptr_t rect_ptr;
    int ret;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)
        || ! get_argument(1, AWK_STRING, &rect_ptr_param)) {
        warning(ext_id, _("SDL_RenderDrawRect: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);
    rect_ptr = strtoull(rect_ptr_param.str_value.str, (char **)NULL, 16);

    ret = SDL_RenderDrawRect((SDL_Renderer *)renderer_ptr,
                             (const SDL_Rect *)rect_ptr);
    if (ret < 0)
        update_ERRNO_string(_("SDL_RenderDrawRect failed"));

    return make_number(ret, result);
}

/* int SDL_RenderFillRect(SDL_Renderer *renderer, const SDL_Rect *rect); */
/* do_SDL_RenderFillRect --- provide a SDL_RenderFillRect()
                             function for gawk */

static awk_value_t *
do_SDL_RenderFillRect(int nargs,
                      awk_value_t *result,
                      struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    awk_value_t rect_ptr_param;
    uintptr_t renderer_ptr;
    uintptr_t rect_ptr;
    int ret;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)
        || ! get_argument(1, AWK_STRING, &rect_ptr_param)) {
        warning(ext_id, _("SDL_RenderFillRect: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);
    rect_ptr = strtoull(rect_ptr_param.str_value.str, (char **)NULL, 16);

    ret = SDL_RenderFillRect((SDL_Renderer *)renderer_ptr,
                             (const SDL_Rect *)rect_ptr);
    if (ret < 0)
        update_ERRNO_string(_("SDL_RenderFillRect failed"));

    return make_number(ret, result);
}

/* void SDL_RenderPresent(SDL_Renderer *renderer); */
/* do_SDL_RenderPresent --- provide a SDL_RenderPresent() function for gawk */

static awk_value_t *
do_SDL_RenderPresent(int nargs,
                     awk_value_t *result,
                     struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    uintptr_t renderer_ptr;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)) {
        warning(ext_id, _("SDL_RenderPresent: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);

    SDL_RenderPresent((SDL_Renderer *)renderer_ptr);
    RETURN_OK;
}

/* int SDL_SetRenderDrawColor(SDL_Renderer *renderer,
                              Uint8 r, Uint8 g, Uint8 b, Uint8 a); */
/* do_SDL_SetRenderDrawColor --- provide a SDL_SetRenderDrawColor()
                                 function for gawk */

static awk_value_t *
do_SDL_SetRenderDrawColor(int nargs,
                          awk_value_t *result,
                          struct awk_ext_func *finfo)
{
    awk_value_t renderer_ptr_param;
    awk_value_t r_param, g_param, b_param, a_param;
    uintptr_t renderer_ptr;
    uint8_t r, g, b, a;
    int ret;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)
        || ! get_argument(1, AWK_NUMBER, &r_param)
        || ! get_argument(2, AWK_NUMBER, &g_param)
        || ! get_argument(3, AWK_NUMBER, &b_param)
        || ! get_argument(4, AWK_NUMBER, &a_param)) {
        warning(ext_id, _("SDL_SetRenderDrawColor: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);
    r = r_param.num_value;
    g = g_param.num_value;
    b = b_param.num_value;
    a = a_param.num_value;

    ret = SDL_SetRenderDrawColor((SDL_Renderer *)renderer_ptr, r, g, b, a);
    if (ret < 0)
        update_ERRNO_string(_("SDL_SetRenderDrawColor failed"));

    return make_number(ret, result);
}

/* SDL_Texture *SDL_CreateTexture(SDL_Renderer *renderer,
                                  Uint32 format,
                                  int access,
                                  int w, int h); */
/* do_SDL_CreateTexture --- provide a SDL_CreateTexture() function for gawk */

static awk_value_t *
do_SDL_CreateTexture(int nargs,
                     awk_value_t *result,
                     struct awk_ext_func *finfo)
{
    SDL_Texture *texture;
    awk_value_t renderer_ptr_param;
    awk_value_t format_param;
    awk_value_t access_param;
    awk_value_t w_param, h_param;
    uintptr_t renderer_ptr;
    uint32_t format;
    int access;
    int w, h;

    if (! get_argument(0, AWK_STRING, &renderer_ptr_param)
        || ! get_argument(1, AWK_NUMBER, &format_param)
        || ! get_argument(2, AWK_NUMBER, &access_param)
        || ! get_argument(3, AWK_NUMBER, &w_param)
        || ! get_argument(4, AWK_NUMBER, &h_param)) {
        warning(ext_id, _("SDL_CreateTexture: bad parameter(s)"));
        RETURN_NOK;
    }

    renderer_ptr = strtoull(renderer_ptr_param.str_value.str,
                            (char **)NULL,
                            16);
    format = format_param.num_value;
    access = access_param.num_value;
    w = w_param.num_value;
    h = h_param.num_value;

    texture = SDL_CreateTexture((SDL_Renderer *)renderer_ptr,
                                format,
                                access,
                                w, h);

    if (texture) {
        char texture_addr[20];
        // NOLINTNEXTLINE
        snprintf(texture_addr, sizeof(texture_addr), "%p", texture);
        return make_string_malloc(texture_addr, strlen(texture_addr), result);
    }

    update_ERRNO_string(_("SDL_CreateTexture failed"));
    return make_null_string(result);
}

/* void SDL_DestroyTexture(SDL_Texture *texture); */
/* do_SDL_DestroyTexture --- provide a SDL_DestroyTexture()
                             function for gawk */

static awk_value_t *
do_SDL_DestroyTexture(int nargs,
                      awk_value_t *result,
                      struct awk_ext_func *finfo)
{
    awk_value_t texture_ptr_param;
    uintptr_t texture_ptr;

    if (! get_argument(0, AWK_STRING, &texture_ptr_param)) {
        warning(ext_id, _("SDL_DestroyTexture: bad parameter(s)"));
        RETURN_NOK;
    }

    texture_ptr = strtoull(texture_ptr_param.str_value.str,
                           (char **)NULL,
                           16);

    SDL_DestroyTexture((SDL_Texture *)texture_ptr);
    RETURN_OK;
}

/* int SDL_UpdateTexture(SDL_Texture *texture,
                         const SDL_Rect *rect,
                         const void *pixels,
                         int pitch); */
/* do_SDL_UpdateTexture --- provide a SDL_UpdateTexture() function for gawk */

static awk_value_t *
do_SDL_UpdateTexture(int nargs,
                     awk_value_t *result,
                     struct awk_ext_func *finfo)
{
    awk_value_t texture_ptr_param;
    awk_value_t rect_ptr_param;
    awk_value_t pixels_ptr_param;
    awk_value_t pitch_param;
    uintptr_t texture_ptr;
    uintptr_t rect_ptr;
    uintptr_t pixels_ptr;
    int pitch;
    int ret;

    if (! get_argument(0, AWK_STRING, &texture_ptr_param)
        || ! get_argument(1, AWK_STRING, &rect_ptr_param)
        || ! get_argument(2, AWK_STRING, &pixels_ptr_param)
        || ! get_argument(3, AWK_NUMBER, &pitch_param)) {
        warning(ext_id, _("SDL_UpdateTexture: bad parameter(s)"));
        RETURN_NOK;
    }

    texture_ptr = strtoull(texture_ptr_param.str_value.str,
                           (char **)NULL,
                           16);
    rect_ptr = strtoull(rect_ptr_param.str_value.str, (char **)NULL, 16);
    pixels_ptr = strtoull(pixels_ptr_param.str_value.str, (char **)NULL, 16);
    pitch = pitch_param.num_value;

    ret = SDL_UpdateTexture((SDL_Texture *)texture_ptr,
                            (const SDL_Rect *)rect_ptr,
                            (const void *)pixels_ptr,
                            pitch);
    if (ret < 0)
        update_ERRNO_string(_("SDL_UpdateTexture failed"));

    return make_number(ret, result);
}

/*----- Pixel Formats and Conversion Routines ------------------------------*/

/* SDL_Palette *SDL_AllocPalette(int ncolors); */
/* do_SDL_AllocPalette --- provide a SDL_AllocPalette() function for gawk */

static awk_value_t *
do_SDL_AllocPalette(int nargs,
                    awk_value_t *result,
                    struct awk_ext_func *finfo)
{
    SDL_Palette *palette;
    awk_value_t ncolors_param;
    int ncolors;

    if (! get_argument(0, AWK_NUMBER, &ncolors_param)) {
        warning(ext_id, _("SDL_AllocPalette: bad parameter(s)"));
        RETURN_NOK;
    }

    ncolors = ncolors_param.num_value;

    palette = SDL_AllocPalette(ncolors);

    if (palette) {
        char palette_addr[20];
        // NOLINTNEXTLINE
        snprintf(palette_addr, sizeof(palette_addr), "%p", palette);
        return make_string_malloc(palette_addr, strlen(palette_addr), result);
    }

    update_ERRNO_string(_("SDL_AllocPalette failed"));
    return make_null_string(result);
}

/* void SDL_FreePalette(SDL_Palette *palette); */
/* do_SDL_FreePalette --- provide a SDL_FreePalette() function for gawk */

static awk_value_t *
do_SDL_FreePalette(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t palette_ptr_param;
    uintptr_t palette_ptr;

    if (! get_argument(0, AWK_STRING, &palette_ptr_param)) {
        warning(ext_id, _("SDL_FreePalette: bad parameter(s)"));
        RETURN_NOK;
    }

    palette_ptr = strtoull(palette_ptr_param.str_value.str,
                           (char **)NULL,
                           16);

    SDL_FreePalette((SDL_Palette *)palette_ptr);
    RETURN_OK;
}

/* int SDL_SetPaletteColors(SDL_Palette *palette,
                            const SDL_Color *colors,
                            int firstcolor, int ncolors); */
/* do_SDL_SetPaletteColors --- provide a SDL_SetPaletteColors()
                               function for gawk */

static awk_value_t *
do_SDL_SetPaletteColors(int nargs,
                        awk_value_t *result,
                        struct awk_ext_func *finfo)
{
    awk_value_t palette_ptr_param;
    awk_value_t colors_ptr_param;
    awk_value_t firstcolor_param;
    awk_value_t ncolors_param;
    uintptr_t palette_ptr;
    uintptr_t colors_ptr;
    int firstcolor;
    int ncolors;
    int ret;

    if (! get_argument(0, AWK_STRING, &palette_ptr_param)
        || ! get_argument(1, AWK_STRING, &colors_ptr_param)
        || ! get_argument(2, AWK_NUMBER, &firstcolor_param)
        || ! get_argument(3, AWK_NUMBER, &ncolors_param)) {
        warning(ext_id, _("SDL_SetPaletteColors: bad parameter(s)"));
        RETURN_NOK;
    }

    palette_ptr = strtoull(palette_ptr_param.str_value.str,
                           (char **)NULL,
                           16);
    colors_ptr = strtoull(colors_ptr_param.str_value.str, (char **)NULL, 16);
    firstcolor = firstcolor_param.num_value;
    ncolors = ncolors_param.num_value;

    ret = SDL_SetPaletteColors((SDL_Palette *)palette_ptr,
                               (const SDL_Color *)colors_ptr,
                               firstcolor, ncolors);
    if (ret < 0)
        update_ERRNO_string(_("SDL_SetPaletteColors failed"));

    return make_number(ret, result);
}

/* void SDL_Gawk_PixelFormatToArray(SDL_PixelFormat *fmt,
                                    awk_array_t *array); */
// /* It doesn't exist in SDL2 */
/* do_SDL_Gawk_PixelFormatToArray --- provide a SDL_Gawk_PixelFormatToArray()
                                      function for gawk */

static awk_value_t *
do_SDL_Gawk_PixelFormatToArray(int nargs,
                               awk_value_t *result,
                               struct awk_ext_func *finfo)
{
    awk_value_t fmt_ptr_param;
    awk_value_t array_param;
    uintptr_t fmt_ptr;
    awk_array_t array;
    SDL_PixelFormat *fmt;
    char palette_addr[20];
    awk_value_t index, value;

    if (! get_argument(0, AWK_STRING, &fmt_ptr_param)
        || ! get_argument(1, AWK_ARRAY, &array_param)) {
        warning(ext_id, _("SDL_Gawk_PixelFormatToArray: bad parameter(s)"));
        RETURN_NOK;
    }

    fmt_ptr = strtoull(fmt_ptr_param.str_value.str,
                       (char **)NULL,
                       16);
    array = array_param.array_cookie;

    if (! fmt_ptr) {
        warning(ext_id,
                _("SDL_Gawk_PixelFormatToArray: invalid pixel format"));
        RETURN_NOK;
    }

    fmt = (SDL_PixelFormat *)fmt_ptr;
    // NOLINTBEGIN
    snprintf(palette_addr, sizeof(palette_addr), "%p", fmt->palette);
    // NOLINTEND

    clear_array(array);

    set_array_element(array,
                      make_const_string("format", 6, &index),
                      make_number(fmt->format, &value));
    set_array_element(array,
                      make_const_string("palette", 7, &index),
                      make_const_string(palette_addr, strlen(palette_addr),
                                        &value));
    set_array_element(array,
                      make_const_string("BitsPerPixel", 12, &index),
                      make_number(fmt->BitsPerPixel, &value));
    set_array_element(array,
                      make_const_string("BytesPerPixel", 13, &index),
                      make_number(fmt->BytesPerPixel, &value));
    set_array_element(array,
                      make_const_string("Rmask", 5, &index),
                      make_number(fmt->Rmask, &value));
    set_array_element(array,
                      make_const_string("Gmask", 5, &index),
                      make_number(fmt->Gmask, &value));
    set_array_element(array,
                      make_const_string("Bmask", 5, &index),
                      make_number(fmt->Bmask, &value));
    set_array_element(array,
                      make_const_string("Amask", 5, &index),
                      make_number(fmt->Amask, &value));
    RETURN_OK;
}

/* SDL_bool SDL_PixelFormatEnumToMasks(Uint32 format,
                                       int *bpp,
                                       Uint32 *Rmask,
                                       Uint32 *Gmask,
                                       Uint32 *Bmask,
                                       Uint32 *Amask); */
/* do_SDL_PixelFormatEnumToMasks --- provide a SDL_PixelFormatEnumToMasks()
                                     function for gawk */

static awk_value_t *
do_SDL_PixelFormatEnumToMasks(int nargs,
                              awk_value_t *result,
                              struct awk_ext_func *finfo)
{
    awk_value_t format_param;
    awk_value_t bpp_ptr_param;
    awk_value_t Rmask_ptr_param;
    awk_value_t Gmask_ptr_param;
    awk_value_t Bmask_ptr_param;
    awk_value_t Amask_ptr_param;
    uint32_t format;
    uintptr_t bpp_ptr;
    uintptr_t Rmask_ptr;
    uintptr_t Gmask_ptr;
    uintptr_t Bmask_ptr;
    uintptr_t Amask_ptr;

    if (! get_argument(0, AWK_NUMBER, &format_param)
        || ! get_argument(1, AWK_STRING, &bpp_ptr_param)
        || ! get_argument(2, AWK_STRING, &Rmask_ptr_param)
        || ! get_argument(3, AWK_STRING, &Gmask_ptr_param)
        || ! get_argument(4, AWK_STRING, &Bmask_ptr_param)
        || ! get_argument(5, AWK_STRING, &Amask_ptr_param)) {
        warning(ext_id, _("SDL_PixelFormatEnumToMasks: bad parameter(s)"));
        RETURN_NOK;
    }

    format = format_param.num_value;
    bpp_ptr = strtoull(bpp_ptr_param.str_value.str, (char **)NULL, 16);
    Rmask_ptr = strtoull(Rmask_ptr_param.str_value.str, (char **)NULL, 16);
    Gmask_ptr = strtoull(Gmask_ptr_param.str_value.str, (char **)NULL, 16);
    Bmask_ptr = strtoull(Bmask_ptr_param.str_value.str, (char **)NULL, 16);
    Amask_ptr = strtoull(Amask_ptr_param.str_value.str, (char **)NULL, 16);

    return make_number(SDL_PixelFormatEnumToMasks(format,
                                                  (int *)bpp_ptr,
                                                  (uint32_t *)Rmask_ptr,
                                                  (uint32_t *)Gmask_ptr,
                                                  (uint32_t *)Bmask_ptr,
                                                  (uint32_t *)Amask_ptr),
                       result);
}

/*----- Rectangle Functions ------------------------------------------------*/

/* SDL_Rect *SDL_Gawk_CreateRect(void); */
// /* It doesn't exist in SDL2 */
/* do_SDL_Gawk_CreateRect --- provide a SDL_Gawk_CreateRect()
                              function for gawk */

static awk_value_t *
do_SDL_Gawk_CreateRect(int nargs,
                       awk_value_t *result,
                       struct awk_ext_func *finfo)
{
    SDL_Rect *rect;

    rect = gawk_malloc(sizeof(SDL_Rect));

    if (rect) {
        char rect_addr[20];
        // NOLINTNEXTLINE
        snprintf(rect_addr, sizeof(rect_addr), "%p", rect);
        return make_string_malloc(rect_addr, strlen(rect_addr), result);
    }

    update_ERRNO_string(_("SDL_Gawk_CreateRect failed"));
    return make_null_string(result);
}

/* void SDL_Gawk_UpdateRect(SDL_Rect *rect, int x, int y, int w, int h); */
// /* It doesn't exist in SDL2 */
/* do_SDL_Gawk_UpdateRect --- provide a SDL_Gawk_UpdateRect()
                              function for gawk */

static awk_value_t *
do_SDL_Gawk_UpdateRect(int nargs,
                       awk_value_t *result,
                       struct awk_ext_func *finfo)
{
    awk_value_t rect_ptr_param;
    awk_value_t x_param, y_param;
    awk_value_t w_param, h_param;
    uintptr_t rect_ptr;
    int x, y;
    int w, h;

    if (! get_argument(0, AWK_STRING, &rect_ptr_param)
        || ! get_argument(1, AWK_NUMBER, &x_param)
        || ! get_argument(2, AWK_NUMBER, &y_param)
        || ! get_argument(3, AWK_NUMBER, &w_param)
        || ! get_argument(4, AWK_NUMBER, &h_param)) {
        warning(ext_id, _("SDL_Gawk_UpdateRect: bad parameter(s)"));
        RETURN_NOK;
    }

    rect_ptr = strtoull(rect_ptr_param.str_value.str, (char **)NULL, 16);
    x = x_param.num_value;
    y = y_param.num_value;
    w = w_param.num_value;
    h = h_param.num_value;

    if (! rect_ptr) {
        warning(ext_id, _("SDL_Gawk_UpdateRect: invalid rectangle"));
        RETURN_NOK;
    }

    ((SDL_Rect *)rect_ptr)->x = x;
    ((SDL_Rect *)rect_ptr)->y = y;
    ((SDL_Rect *)rect_ptr)->w = w;
    ((SDL_Rect *)rect_ptr)->h = h;
    RETURN_OK;
}

/* SDL_bool SDL_HasIntersection(const SDL_Rect *A, const SDL_Rect *B); */
/* do_SDL_HasIntersection --- provide a SDL_HasIntersection()
                              function for gawk */

static awk_value_t *
do_SDL_HasIntersection(int nargs,
                       awk_value_t *result,
                       struct awk_ext_func *finfo)
{
    awk_value_t A_ptr_param;
    awk_value_t B_ptr_param;
    uintptr_t A_ptr;
    uintptr_t B_ptr;

    if (! get_argument(0, AWK_STRING, &A_ptr_param)
        || ! get_argument(1, AWK_STRING, &B_ptr_param)) {
        warning(ext_id, _("SDL_HasIntersection: bad parameter(s)"));
        RETURN_NOK;
    }

    A_ptr = strtoull(A_ptr_param.str_value.str, (char **)NULL, 16);
    B_ptr = strtoull(B_ptr_param.str_value.str, (char **)NULL, 16);

    return make_number(SDL_HasIntersection((const SDL_Rect *)A_ptr,
                                           (const SDL_Rect *)B_ptr),
                       result);
}

/*----- Surface Creation and Simple Drawing --------------------------------*/

/* int SDL_BlitSurface(SDL_Surface *src, const SDL_Rect *srcrect,
                       SDL_Surface *dst, SDL_Rect *dstrect); */
/* do_SDL_BlitSurface --- provide a SDL_BlitSurface() function for gawk */

static awk_value_t *
do_SDL_BlitSurface(int nargs,
                   awk_value_t *result,
                   struct awk_ext_func *finfo)
{
    awk_value_t src_ptr_param;
    awk_value_t srcrect_ptr_param;
    awk_value_t dst_ptr_param;
    awk_value_t dstrect_ptr_param;
    uintptr_t src_ptr;
    uintptr_t srcrect_ptr;
    uintptr_t dst_ptr;
    uintptr_t dstrect_ptr;
    int ret;

    if (! get_argument(0, AWK_STRING, &src_ptr_param)
        || ! get_argument(1, AWK_STRING, &srcrect_ptr_param)
        || ! get_argument(2, AWK_STRING, &dst_ptr_param)
        || ! get_argument(3, AWK_STRING, &dstrect_ptr_param)) {
        warning(ext_id, _("SDL_BlitSurface: bad parameter(s)"));
        RETURN_NOK;
    }

    src_ptr = strtoull(src_ptr_param.str_value.str, (char **)NULL, 16);
    srcrect_ptr = strtoull(srcrect_ptr_param.str_value.str,
                           (char **)NULL,
                           16);
    dst_ptr = strtoull(dst_ptr_param.str_value.str, (char **)NULL, 16);
    dstrect_ptr = strtoull(dstrect_ptr_param.str_value.str,
                           (char **)NULL,
                           16);

    ret = SDL_BlitSurface((SDL_Surface *)src_ptr,
                          (const SDL_Rect *)srcrect_ptr,
                          (SDL_Surface *)dst_ptr,
                          (SDL_Rect *)dstrect_ptr);
    if (ret < 0)
        update_ERRNO_string(_("SDL_BlitSurface failed"));

    return make_number(ret, result);
}

/* SDL_Surface *SDL_CreateRGBSurface(Uint32 flags,
                                     int width, int height,
                                     int depth,
                                     Uint32 Rmask,
                                     Uint32 Gmask,
                                     Uint32 Bmask,
                                     Uint32 Amask); */
/* do_SDL_CreateRGBSurface --- provide a SDL_CreateRGBSurface()
                               function for gawk */

static awk_value_t *
do_SDL_CreateRGBSurface(int nargs,
                        awk_value_t *result,
                        struct awk_ext_func *finfo)
{
    SDL_Surface *surface;
    awk_value_t flags_param;
    awk_value_t width_param, height_param;
    awk_value_t depth_param;
    awk_value_t Rmask_param, Gmask_param, Bmask_param, Amask_param;
    uint32_t flags;
    int width, height;
    int depth;
    uint32_t Rmask, Gmask, Bmask, Amask;

    if (! get_argument(0, AWK_NUMBER, &flags_param)
        || ! get_argument(1, AWK_NUMBER, &width_param)
        || ! get_argument(2, AWK_NUMBER, &height_param)
        || ! get_argument(3, AWK_NUMBER, &depth_param)
        || ! get_argument(4, AWK_NUMBER, &Rmask_param)
        || ! get_argument(5, AWK_NUMBER, &Gmask_param)
        || ! get_argument(6, AWK_NUMBER, &Bmask_param)
        || ! get_argument(7, AWK_NUMBER, &Amask_param)) {
        warning(ext_id, _("SDL_CreateRGBSurface: bad parameter(s)"));
        RETURN_NOK;
    }

    flags = flags_param.num_value;
    width = width_param.num_value;
    height = height_param.num_value;
    depth = depth_param.num_value;
    Rmask = Rmask_param.num_value;
    Gmask = Gmask_param.num_value;
    Bmask = Bmask_param.num_value;
    Amask = Amask_param.num_value;

    surface = SDL_CreateRGBSurface(flags,
                                   width, height,
                                   depth,
                                   Rmask, Gmask, Bmask, Amask);

    if (surface) {
        char surface_addr[20];
        // NOLINTNEXTLINE
        snprintf(surface_addr, sizeof(surface_addr), "%p", surface);
        return make_string_malloc(surface_addr, strlen(surface_addr), result);
    }

    update_ERRNO_string(_("SDL_CreateRGBSurface failed"));
    return make_null_string(result);
}

/* void SDL_Gawk_SurfaceToArray(SDL_Surface *surface, awk_array_t *array); */
// /* It doesn't exist in SDL2 */
/* do_SDL_Gawk_SurfaceToArray --- provide a SDL_Gawk_SurfaceToArray()
                                  function for gawk */

static awk_value_t *
do_SDL_Gawk_SurfaceToArray(int nargs,
                           awk_value_t *result,
                           struct awk_ext_func *finfo)
{
    awk_value_t surface_ptr_param;
    awk_value_t array_param;
    uintptr_t surface_ptr;
    awk_array_t array;
    SDL_Surface *surface;
    char format_addr[20];
    char pixels_addr[20];
    char userdata_addr[20];
    awk_value_t index, value;

    if (! get_argument(0, AWK_STRING, &surface_ptr_param)
        || ! get_argument(1, AWK_ARRAY, &array_param)) {
        warning(ext_id, _("SDL_Gawk_SurfaceToArray: bad parameter(s)"));
        RETURN_NOK;
    }

    surface_ptr = strtoull(surface_ptr_param.str_value.str,
                           (char **)NULL,
                           16);
    array = array_param.array_cookie;

    if (! surface_ptr) {
        warning(ext_id, _("SDL_Gawk_SurfaceToArray: invalid surface"));
        RETURN_NOK;
    }

    surface = (SDL_Surface *)surface_ptr;
    // NOLINTBEGIN
    snprintf(format_addr, sizeof(format_addr), "%p", surface->format);
    snprintf(pixels_addr, sizeof(pixels_addr), "%p", surface->pixels);
    snprintf(userdata_addr, sizeof(userdata_addr), "%p", surface->userdata);
    // NOLINTEND

    clear_array(array);

    set_array_element(array,
                      make_const_string("format", 6, &index),
                      make_const_string(format_addr, strlen(format_addr),
                                        &value));
    set_array_element(array,
                      make_const_string("w", 1, &index),
                      make_number(surface->w, &value));
    set_array_element(array,
                      make_const_string("h", 1, &index),
                      make_number(surface->h, &value));
    set_array_element(array,
                      make_const_string("pitch", 5, &index),
                      make_number(surface->pitch, &value));
    set_array_element(array,
                      make_const_string("pixels", 6, &index),
                      make_const_string(pixels_addr, strlen(pixels_addr),
                                        &value));
    set_array_element(array,
                      make_const_string("userdata", 8, &index),
                      make_const_string(userdata_addr, strlen(userdata_addr),
                                        &value));
    set_array_element(array,
                      make_const_string("clip_rect.x", 11, &index),
                      make_number(surface->clip_rect.x, &value));
    set_array_element(array,
                      make_const_string("clip_rect.y", 11, &index),
                      make_number(surface->clip_rect.y, &value));
    set_array_element(array,
                      make_const_string("clip_rect.w", 11, &index),
                      make_number(surface->clip_rect.w, &value));
    set_array_element(array,
                      make_const_string("clip_rect.h", 11, &index),
                      make_number(surface->clip_rect.h, &value));
    set_array_element(array,
                      make_const_string("refcount", 8, &index),
                      make_number(surface->refcount, &value));
    RETURN_OK;
}

/* int SDL_FillRect(SDL_Surface *dst, const SDL_Rect *rect, Uint32 color); */
/* do_SDL_FillRect --- provide a SDL_FillRect() function for gawk */

static awk_value_t *
do_SDL_FillRect(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t dst_ptr_param;
    awk_value_t rect_ptr_param;
    awk_value_t color_param;
    uintptr_t dst_ptr;
    uintptr_t rect_ptr;
    uint32_t color;
    int ret;

    if (! get_argument(0, AWK_STRING, &dst_ptr_param)
        || ! get_argument(1, AWK_STRING, &rect_ptr_param)
        || ! get_argument(2, AWK_NUMBER, &color_param)) {
        warning(ext_id, _("SDL_FillRect: bad parameter(s)"));
        RETURN_NOK;
    }

    dst_ptr = strtoull(dst_ptr_param.str_value.str, (char **)NULL, 16);
    rect_ptr = strtoull(rect_ptr_param.str_value.str, (char **)NULL, 16);
    color = color_param.num_value;

    ret = SDL_FillRect((SDL_Surface *)dst_ptr,
                       (const SDL_Rect *)rect_ptr,
                       color);
    if (ret < 0)
        update_ERRNO_string(_("SDL_FillRect failed"));

    return make_number(ret, result);
}

/*----- Event Handling -----------------------------------------------------*/

/* SDL_Event *SDL_Gawk_CreateEvent(void); */
// /* It doesn't exist in SDL2 */
/* do_SDL_Gawk_CreateEvent --- provide a SDL_Gawk_CreateEvent()
                               function for gawk */

static awk_value_t *
do_SDL_Gawk_CreateEvent(int nargs,
                        awk_value_t *result,
                        struct awk_ext_func *finfo)
{
    SDL_Event *event;

    event = gawk_malloc(sizeof(SDL_Event));

    if (event) {
        char event_addr[20];
        // NOLINTNEXTLINE
        snprintf(event_addr, sizeof(event_addr), "%p", event);
        return make_string_malloc(event_addr, strlen(event_addr), result);
    }

    update_ERRNO_string(_("SDL_Gawk_CreateEvent failed"));
    return make_null_string(result);
}

/* Uint32 SDL_Gawk_GetEventType(SDL_Event *event); */
// /* It doesn't exist in SDL2 */
/* do_SDL_Gawk_GetEventType --- provide a SDL_Gawk_GetEventType()
                                function for gawk */

static awk_value_t *
do_SDL_Gawk_GetEventType(int nargs,
                         awk_value_t *result,
                         struct awk_ext_func *finfo)
{
    awk_value_t event_ptr_param;
    uintptr_t event_ptr;
    uint32_t type;

    if (! get_argument(0, AWK_STRING, &event_ptr_param)) {
        warning(ext_id, _("SDL_Gawk_GetEventType: bad parameter(s)"));
        RETURN_NOK;
    }

    event_ptr = strtoull(event_ptr_param.str_value.str, (char **)NULL, 16);

    if (! event_ptr) {
        warning(ext_id, _("SDL_Gawk_GetEventType: invalid event"));
        RETURN_NOK;
    }

    type = ((SDL_Event *)event_ptr)->type;
    return make_number(type, result);
}

/* int SDL_PollEvent(SDL_Event *event); */
/* do_SDL_PollEvent --- provide a SDL_PollEvent() function for gawk */

static awk_value_t *
do_SDL_PollEvent(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t event_ptr_param;
    uintptr_t event_ptr;

    if (! get_argument(0, AWK_STRING, &event_ptr_param)) {
        warning(ext_id, _("SDL_PollEvent: bad parameter(s)"));
        RETURN_NOK;
    }

    event_ptr = strtoull(event_ptr_param.str_value.str, (char **)NULL, 16);

    return make_number(SDL_PollEvent((SDL_Event *)event_ptr), result);
}

/*----- Keyboard Support ---------------------------------------------------*/

/* void SDL_Gawk_GetKeyboardState(int *numkeys, awk_array_t *array); */
// /* It doesn't exist in SDL2 */
/* do_SDL_Gawk_GetKeyboardState --- provide a SDL_Gawk_GetKeyboardState()
                                    function for gawk */

static awk_value_t *
do_SDL_Gawk_GetKeyboardState(int nargs,
                             awk_value_t *result,
                             struct awk_ext_func *finfo)
{
    awk_value_t numkeys_ptr_param;
    awk_value_t array_param;
    uintptr_t numkeys_ptr;
    awk_array_t array;
    const uint8_t *state;
    awk_value_t index, value;
    size_t i;

    if (! get_argument(0, AWK_STRING, &numkeys_ptr_param)
        || ! get_argument(1, AWK_ARRAY, &array_param)) {
        warning(ext_id, _("SDL_Gawk_GetKeyboardState: bad parameter(s)"));
        RETURN_NOK;
    }

    numkeys_ptr = strtoull(numkeys_ptr_param.str_value.str,
                           (char **)NULL,
                           16);
    array = array_param.array_cookie;

    clear_array(array);

    state = SDL_GetKeyboardState((int *)numkeys_ptr);
    for (i = 0; i < SDL_NUM_SCANCODES; i++)
        set_array_element(array,
                          make_number(i, &index),
                          make_number(state[i], &value));
    RETURN_OK;
}

/*----- Timer Support ------------------------------------------------------*/

/* void SDL_Delay(Uint32 ms); */
/* do_SDL_Delay --- provide a SDL_Delay() function for gawk */

static awk_value_t *
do_SDL_Delay(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    awk_value_t ms_param;
    uint32_t ms;

    if (! get_argument(0, AWK_NUMBER, &ms_param)) {
        warning(ext_id, _("SDL_Delay: bad parameter(s)"));
        RETURN_NOK;
    }

    ms = ms_param.num_value;

    SDL_Delay(ms);
    RETURN_OK;
}

/* Uint32 SDL_GetTicks(void); */
/* do_SDL_GetTicks --- provide a SDL_GetTicks() function for gawk */

static awk_value_t *
do_SDL_GetTicks(int nargs, awk_value_t *result, struct awk_ext_func *finfo)
{
    uint32_t ms = SDL_GetTicks();
    return make_number(ms, result);
}

/*----- Message boxes ------------------------------------------------------*/

/* int SDL_ShowSimpleMessageBox(Uint32 flags,
                                const char *title,
                                const char *message,
                                SDL_Window *window); */
/* do_SDL_ShowSimpleMessageBox --- provide a SDL_ShowSimpleMessageBox()
                                   function for gawk */

static awk_value_t *
do_SDL_ShowSimpleMessageBox(int nargs,
                            awk_value_t *result,
                            struct awk_ext_func *finfo)
{
    awk_value_t flags_param;
    awk_value_t title_param;
    awk_value_t message_param;
    awk_value_t window_ptr_param;
    uint32_t flags;
    const char *title;
    const char *message;
    uintptr_t window_ptr;
    int ret;

    if (! get_argument(0, AWK_NUMBER, &flags_param)
        || ! get_argument(1, AWK_STRING, &title_param)
        || ! get_argument(2, AWK_STRING, &message_param)
        || ! get_argument(3, AWK_STRING, &window_ptr_param)) {
        warning(ext_id, _("SDL_ShowSimpleMessageBox: bad parameter(s)"));
        RETURN_NOK;
    }

    flags = flags_param.num_value;
    title = title_param.str_value.str;
    message = message_param.str_value.str;
    window_ptr = strtoull(window_ptr_param.str_value.str, (char **)NULL, 16);

    ret = SDL_ShowSimpleMessageBox(flags,
                                   title,
                                   message,
                                   (SDL_Window *)window_ptr);
    if (ret < 0)
        update_ERRNO_string(_("SDL_ShowSimpleMessageBox failed"));

    return make_number(ret, result);
}

/*--------------------------------------------------------------------------*/

/* init_sdl2 --- initialization routine */

static awk_bool_t
init_sdl2(void)
{
    load_vars();
    return awk_true;
}

static awk_ext_func_t func_table[] = {
    { "SDL_Init", do_SDL_Init, 1, 1, awk_false, NULL },
    { "SDL_InitSubSystem", do_SDL_Init, 1, 1, awk_false, NULL },
    { "SDL_Quit", do_SDL_Quit, 0, 0, awk_false, NULL },
    { "SDL_QuitSubSystem", do_SDL_QuitSubSystem, 1, 1, awk_false, NULL },
    { "SDL_GetError", do_SDL_GetError, 0, 0, awk_false, NULL },
    { "SDL_VERSIONNUM", do_SDL_VERSIONNUM, 3, 3, awk_false, NULL },
    { "SDL_CreateWindow", do_SDL_CreateWindow, 6, 6, awk_false, NULL },
    { "SDL_DestroyWindow", do_SDL_DestroyWindow, 1, 1, awk_false, NULL },
    { "SDL_GetWindowPixelFormat", do_SDL_GetWindowPixelFormat,
      1, 1,
      awk_false,
      NULL },
    { "SDL_SetWindowTitle", do_SDL_SetWindowTitle, 2, 2, awk_false, NULL },
    { "SDL_CreateRenderer", do_SDL_CreateRenderer, 3, 3, awk_false, NULL },
    { "SDL_DestroyRenderer", do_SDL_DestroyRenderer, 1, 1, awk_false, NULL },
    { "SDL_RenderClear", do_SDL_RenderClear, 1, 1, awk_false, NULL },
    { "SDL_RenderCopy", do_SDL_RenderCopy, 4, 4, awk_false, NULL },
    { "SDL_RenderDrawLine", do_SDL_RenderDrawLine, 5, 5, awk_false, NULL },
    { "SDL_RenderDrawRect", do_SDL_RenderDrawRect, 2, 2, awk_false, NULL },
    { "SDL_RenderFillRect", do_SDL_RenderFillRect, 2, 2, awk_false, NULL },
    { "SDL_RenderPresent", do_SDL_RenderPresent, 1, 1, awk_false, NULL },
    { "SDL_SetRenderDrawColor", do_SDL_SetRenderDrawColor,
      5, 5,
      awk_false,
      NULL },
    { "SDL_CreateTexture", do_SDL_CreateTexture, 5, 5, awk_false, NULL },
    { "SDL_DestroyTexture", do_SDL_DestroyTexture, 1, 1, awk_false, NULL },
    { "SDL_UpdateTexture", do_SDL_UpdateTexture, 4, 4, awk_false, NULL },
    { "SDL_AllocPalette", do_SDL_AllocPalette, 1, 1, awk_false, NULL },
    { "SDL_FreePalette", do_SDL_FreePalette, 1, 1, awk_false, NULL },
    { "SDL_SetPaletteColors", do_SDL_SetPaletteColors,
      4, 4,
      awk_false,
      NULL },
    { "SDL_Gawk_PixelFormatToArray", do_SDL_Gawk_PixelFormatToArray,
      2, 2,
      awk_false,
      NULL },
    { "SDL_PixelFormatEnumToMasks", do_SDL_PixelFormatEnumToMasks,
      6, 6,
      awk_false,
      NULL },
    { "SDL_Gawk_CreateRect", do_SDL_Gawk_CreateRect, 0, 0, awk_false, NULL },
    { "SDL_Gawk_UpdateRect", do_SDL_Gawk_UpdateRect, 5, 5, awk_false, NULL },
    { "SDL_HasIntersection", do_SDL_HasIntersection, 2, 2, awk_false, NULL },
    { "SDL_BlitSurface", do_SDL_BlitSurface, 4, 4, awk_false, NULL },
    { "SDL_CreateRGBSurface", do_SDL_CreateRGBSurface,
      8, 8,
      awk_false,
      NULL },
    { "SDL_Gawk_SurfaceToArray", do_SDL_Gawk_SurfaceToArray,
      2, 2,
      awk_false,
      NULL },
    { "SDL_FillRect", do_SDL_FillRect, 3, 3, awk_false, NULL },
    { "SDL_Gawk_CreateEvent", do_SDL_Gawk_CreateEvent,
      0, 0,
      awk_false,
      NULL },
    { "SDL_Gawk_GetEventType", do_SDL_Gawk_GetEventType,
      1, 1,
      awk_false,
      NULL },
    { "SDL_PollEvent", do_SDL_PollEvent, 1, 1, awk_false, NULL },
    { "SDL_Gawk_GetKeyboardState", do_SDL_Gawk_GetKeyboardState,
      2, 2,
      awk_false,
      NULL },
    { "SDL_Delay", do_SDL_Delay, 1, 1, awk_false, NULL },
    { "SDL_GetTicks", do_SDL_GetTicks, 0, 0, awk_false, NULL },
    { "SDL_ShowSimpleMessageBox", do_SDL_ShowSimpleMessageBox,
      4, 4,
      awk_false,
      NULL },
};

/* define the dl_load() function using the boilerplate macro */

dl_load_func(func_table, sdl2, "")
