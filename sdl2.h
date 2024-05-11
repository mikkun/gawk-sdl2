// SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <SDL2/SDL.h>

/* Initialization and Shutdown */
#define DEFAULT_SDL_INIT_TIMER SDL_INIT_TIMER
#define DEFAULT_SDL_INIT_AUDIO SDL_INIT_AUDIO
#define DEFAULT_SDL_INIT_VIDEO SDL_INIT_VIDEO
#define DEFAULT_SDL_INIT_JOYSTICK SDL_INIT_JOYSTICK
#define DEFAULT_SDL_INIT_HAPTIC SDL_INIT_HAPTIC
#define DEFAULT_SDL_INIT_GAMECONTROLLER SDL_INIT_GAMECONTROLLER
#define DEFAULT_SDL_INIT_EVENTS SDL_INIT_EVENTS
#define DEFAULT_SDL_INIT_SENSOR SDL_INIT_SENSOR
#define DEFAULT_SDL_INIT_NOPARACHUTE SDL_INIT_NOPARACHUTE
#define DEFAULT_SDL_INIT_EVERYTHING SDL_INIT_EVERYTHING

/* Querying SDL Version */
#define DEFAULT_SDL_MAJOR_VERSION SDL_MAJOR_VERSION
#define DEFAULT_SDL_MINOR_VERSION SDL_MINOR_VERSION
#define DEFAULT_SDL_PATCHLEVEL SDL_PATCHLEVEL
#define DEFAULT_SDL_COMPILEDVERSION SDL_COMPILEDVERSION

/* Display and Window Management */
#define DEFAULT_SDL_WINDOW_FULLSCREEN SDL_WINDOW_FULLSCREEN
#define DEFAULT_SDL_WINDOW_OPENGL SDL_WINDOW_OPENGL
#define DEFAULT_SDL_WINDOW_SHOWN SDL_WINDOW_SHOWN
#define DEFAULT_SDL_WINDOW_HIDDEN SDL_WINDOW_HIDDEN
#define DEFAULT_SDL_WINDOW_BORDERLESS SDL_WINDOW_BORDERLESS
#define DEFAULT_SDL_WINDOW_RESIZABLE SDL_WINDOW_RESIZABLE
#define DEFAULT_SDL_WINDOW_MINIMIZED SDL_WINDOW_MINIMIZED
#define DEFAULT_SDL_WINDOW_MAXIMIZED SDL_WINDOW_MAXIMIZED
#define DEFAULT_SDL_WINDOW_MOUSE_GRABBED SDL_WINDOW_MOUSE_GRABBED
#define DEFAULT_SDL_WINDOW_INPUT_FOCUS SDL_WINDOW_INPUT_FOCUS
#define DEFAULT_SDL_WINDOW_MOUSE_FOCUS SDL_WINDOW_MOUSE_FOCUS
#define DEFAULT_SDL_WINDOW_FULLSCREEN_DESKTOP SDL_WINDOW_FULLSCREEN_DESKTOP
#define DEFAULT_SDL_WINDOW_FOREIGN SDL_WINDOW_FOREIGN
#define DEFAULT_SDL_WINDOW_ALLOW_HIGHDPI SDL_WINDOW_ALLOW_HIGHDPI
#define DEFAULT_SDL_WINDOW_MOUSE_CAPTURE SDL_WINDOW_MOUSE_CAPTURE
#define DEFAULT_SDL_WINDOW_ALWAYS_ON_TOP SDL_WINDOW_ALWAYS_ON_TOP
#define DEFAULT_SDL_WINDOW_SKIP_TASKBAR SDL_WINDOW_SKIP_TASKBAR
#define DEFAULT_SDL_WINDOW_UTILITY SDL_WINDOW_UTILITY
#define DEFAULT_SDL_WINDOW_TOOLTIP SDL_WINDOW_TOOLTIP
#define DEFAULT_SDL_WINDOW_POPUP_MENU SDL_WINDOW_POPUP_MENU
#define DEFAULT_SDL_WINDOW_KEYBOARD_GRABBED SDL_WINDOW_KEYBOARD_GRABBED
#define DEFAULT_SDL_WINDOW_VULKAN SDL_WINDOW_VULKAN
#define DEFAULT_SDL_WINDOW_METAL SDL_WINDOW_METAL
#define DEFAULT_SDL_WINDOW_INPUT_GRABBED SDL_WINDOW_INPUT_GRABBED
#define DEFAULT_SDL_WINDOWPOS_UNDEFINED SDL_WINDOWPOS_UNDEFINED
#define DEFAULT_SDL_WINDOWPOS_CENTERED SDL_WINDOWPOS_CENTERED
#define DEFAULT_SDL_WINDOWEVENT_NONE SDL_WINDOWEVENT_NONE
#define DEFAULT_SDL_WINDOWEVENT_SHOWN SDL_WINDOWEVENT_SHOWN
#define DEFAULT_SDL_WINDOWEVENT_HIDDEN SDL_WINDOWEVENT_HIDDEN
#define DEFAULT_SDL_WINDOWEVENT_EXPOSED SDL_WINDOWEVENT_EXPOSED
#define DEFAULT_SDL_WINDOWEVENT_MOVED SDL_WINDOWEVENT_MOVED
#define DEFAULT_SDL_WINDOWEVENT_RESIZED SDL_WINDOWEVENT_RESIZED
#define DEFAULT_SDL_WINDOWEVENT_SIZE_CHANGED SDL_WINDOWEVENT_SIZE_CHANGED
#define DEFAULT_SDL_WINDOWEVENT_MINIMIZED SDL_WINDOWEVENT_MINIMIZED
#define DEFAULT_SDL_WINDOWEVENT_MAXIMIZED SDL_WINDOWEVENT_MAXIMIZED
#define DEFAULT_SDL_WINDOWEVENT_RESTORED SDL_WINDOWEVENT_RESTORED
#define DEFAULT_SDL_WINDOWEVENT_ENTER SDL_WINDOWEVENT_ENTER
#define DEFAULT_SDL_WINDOWEVENT_LEAVE SDL_WINDOWEVENT_LEAVE
#define DEFAULT_SDL_WINDOWEVENT_FOCUS_GAINED SDL_WINDOWEVENT_FOCUS_GAINED
#define DEFAULT_SDL_WINDOWEVENT_FOCUS_LOST SDL_WINDOWEVENT_FOCUS_LOST
#define DEFAULT_SDL_WINDOWEVENT_CLOSE SDL_WINDOWEVENT_CLOSE
#define DEFAULT_SDL_WINDOWEVENT_TAKE_FOCUS SDL_WINDOWEVENT_TAKE_FOCUS
#define DEFAULT_SDL_WINDOWEVENT_HIT_TEST SDL_WINDOWEVENT_HIT_TEST
#define DEFAULT_SDL_WINDOWEVENT_ICCPROF_CHANGED \
    SDL_WINDOWEVENT_ICCPROF_CHANGED
#define DEFAULT_SDL_WINDOWEVENT_DISPLAY_CHANGED \
    SDL_WINDOWEVENT_DISPLAY_CHANGED

/* 2D Accelerated Rendering */
#define DEFAULT_SDL_RENDERER_SOFTWARE SDL_RENDERER_SOFTWARE
#define DEFAULT_SDL_RENDERER_ACCELERATED SDL_RENDERER_ACCELERATED
#define DEFAULT_SDL_RENDERER_PRESENTVSYNC SDL_RENDERER_PRESENTVSYNC
#define DEFAULT_SDL_RENDERER_TARGETTEXTURE SDL_RENDERER_TARGETTEXTURE
#define DEFAULT_SDL_TEXTUREACCESS_STATIC SDL_TEXTUREACCESS_STATIC
#define DEFAULT_SDL_TEXTUREACCESS_STREAMING SDL_TEXTUREACCESS_STREAMING
#define DEFAULT_SDL_TEXTUREACCESS_TARGET SDL_TEXTUREACCESS_TARGET

/* Pixel Formats and Conversion Routines */
#define DEFAULT_SDL_PIXELFORMAT_UNKNOWN SDL_PIXELFORMAT_UNKNOWN
#define DEFAULT_SDL_PIXELFORMAT_INDEX1LSB SDL_PIXELFORMAT_INDEX1LSB
#define DEFAULT_SDL_PIXELFORMAT_INDEX1MSB SDL_PIXELFORMAT_INDEX1MSB
#define DEFAULT_SDL_PIXELFORMAT_INDEX4LSB SDL_PIXELFORMAT_INDEX4LSB
#define DEFAULT_SDL_PIXELFORMAT_INDEX4MSB SDL_PIXELFORMAT_INDEX4MSB
#define DEFAULT_SDL_PIXELFORMAT_INDEX8 SDL_PIXELFORMAT_INDEX8
#define DEFAULT_SDL_PIXELFORMAT_RGB332 SDL_PIXELFORMAT_RGB332
#define DEFAULT_SDL_PIXELFORMAT_XRGB4444 SDL_PIXELFORMAT_XRGB4444
#define DEFAULT_SDL_PIXELFORMAT_RGB444 SDL_PIXELFORMAT_RGB444
#define DEFAULT_SDL_PIXELFORMAT_XBGR4444 SDL_PIXELFORMAT_XBGR4444
#define DEFAULT_SDL_PIXELFORMAT_BGR444 SDL_PIXELFORMAT_BGR444
#define DEFAULT_SDL_PIXELFORMAT_XRGB1555 SDL_PIXELFORMAT_XRGB1555
#define DEFAULT_SDL_PIXELFORMAT_RGB555 SDL_PIXELFORMAT_RGB555
#define DEFAULT_SDL_PIXELFORMAT_XBGR1555 SDL_PIXELFORMAT_XBGR1555
#define DEFAULT_SDL_PIXELFORMAT_BGR555 SDL_PIXELFORMAT_BGR555
#define DEFAULT_SDL_PIXELFORMAT_ARGB4444 SDL_PIXELFORMAT_ARGB4444
#define DEFAULT_SDL_PIXELFORMAT_RGBA4444 SDL_PIXELFORMAT_RGBA4444
#define DEFAULT_SDL_PIXELFORMAT_ABGR4444 SDL_PIXELFORMAT_ABGR4444
#define DEFAULT_SDL_PIXELFORMAT_BGRA4444 SDL_PIXELFORMAT_BGRA4444
#define DEFAULT_SDL_PIXELFORMAT_ARGB1555 SDL_PIXELFORMAT_ARGB1555
#define DEFAULT_SDL_PIXELFORMAT_RGBA5551 SDL_PIXELFORMAT_RGBA5551
#define DEFAULT_SDL_PIXELFORMAT_ABGR1555 SDL_PIXELFORMAT_ABGR1555
#define DEFAULT_SDL_PIXELFORMAT_BGRA5551 SDL_PIXELFORMAT_BGRA5551
#define DEFAULT_SDL_PIXELFORMAT_RGB565 SDL_PIXELFORMAT_RGB565
#define DEFAULT_SDL_PIXELFORMAT_BGR565 SDL_PIXELFORMAT_BGR565
#define DEFAULT_SDL_PIXELFORMAT_RGB24 SDL_PIXELFORMAT_RGB24
#define DEFAULT_SDL_PIXELFORMAT_BGR24 SDL_PIXELFORMAT_BGR24
#define DEFAULT_SDL_PIXELFORMAT_XRGB8888 SDL_PIXELFORMAT_XRGB8888
#define DEFAULT_SDL_PIXELFORMAT_RGB888 SDL_PIXELFORMAT_RGB888
#define DEFAULT_SDL_PIXELFORMAT_RGBX8888 SDL_PIXELFORMAT_RGBX8888
#define DEFAULT_SDL_PIXELFORMAT_XBGR8888 SDL_PIXELFORMAT_XBGR8888
#define DEFAULT_SDL_PIXELFORMAT_BGR888 SDL_PIXELFORMAT_BGR888
#define DEFAULT_SDL_PIXELFORMAT_BGRX8888 SDL_PIXELFORMAT_BGRX8888
#define DEFAULT_SDL_PIXELFORMAT_ARGB8888 SDL_PIXELFORMAT_ARGB8888
#define DEFAULT_SDL_PIXELFORMAT_RGBA8888 SDL_PIXELFORMAT_RGBA8888
#define DEFAULT_SDL_PIXELFORMAT_ABGR8888 SDL_PIXELFORMAT_ABGR8888
#define DEFAULT_SDL_PIXELFORMAT_BGRA8888 SDL_PIXELFORMAT_BGRA8888
#define DEFAULT_SDL_PIXELFORMAT_ARGB2101010 SDL_PIXELFORMAT_ARGB2101010
#define DEFAULT_SDL_PIXELFORMAT_RGBA32 SDL_PIXELFORMAT_RGBA32
#define DEFAULT_SDL_PIXELFORMAT_ARGB32 SDL_PIXELFORMAT_ARGB32
#define DEFAULT_SDL_PIXELFORMAT_BGRA32 SDL_PIXELFORMAT_BGRA32
#define DEFAULT_SDL_PIXELFORMAT_ABGR32 SDL_PIXELFORMAT_ABGR32
#define DEFAULT_SDL_PIXELFORMAT_YV12 SDL_PIXELFORMAT_YV12
#define DEFAULT_SDL_PIXELFORMAT_IYUV SDL_PIXELFORMAT_IYUV
#define DEFAULT_SDL_PIXELFORMAT_YUY2 SDL_PIXELFORMAT_YUY2
#define DEFAULT_SDL_PIXELFORMAT_UYVY SDL_PIXELFORMAT_UYVY
#define DEFAULT_SDL_PIXELFORMAT_YVYU SDL_PIXELFORMAT_YVYU
#define DEFAULT_SDL_PIXELFORMAT_NV12 SDL_PIXELFORMAT_NV12
#define DEFAULT_SDL_PIXELFORMAT_NV21 SDL_PIXELFORMAT_NV21
#define DEFAULT_SDL_PIXELFORMAT_EXTERNAL_OES SDL_PIXELFORMAT_EXTERNAL_OES

/* Event Handling */
#define DEFAULT_SDL_RELEASED SDL_RELEASED
#define DEFAULT_SDL_PRESSED SDL_PRESSED
#define DEFAULT_SDL_FIRSTEVENT SDL_FIRSTEVENT
#define DEFAULT_SDL_QUIT SDL_QUIT
#define DEFAULT_SDL_APP_TERMINATING SDL_APP_TERMINATING
#define DEFAULT_SDL_APP_LOWMEMORY SDL_APP_LOWMEMORY
#define DEFAULT_SDL_APP_WILLENTERBACKGROUND SDL_APP_WILLENTERBACKGROUND
#define DEFAULT_SDL_APP_DIDENTERBACKGROUND SDL_APP_DIDENTERBACKGROUND
#define DEFAULT_SDL_APP_WILLENTERFOREGROUND SDL_APP_WILLENTERFOREGROUND
#define DEFAULT_SDL_APP_DIDENTERFOREGROUND SDL_APP_DIDENTERFOREGROUND
#define DEFAULT_SDL_LOCALECHANGED SDL_LOCALECHANGED
#define DEFAULT_SDL_DISPLAYEVENT SDL_DISPLAYEVENT
#define DEFAULT_SDL_WINDOWEVENT SDL_WINDOWEVENT
#define DEFAULT_SDL_SYSWMEVENT SDL_SYSWMEVENT
#define DEFAULT_SDL_KEYDOWN SDL_KEYDOWN
#define DEFAULT_SDL_KEYUP SDL_KEYUP
#define DEFAULT_SDL_TEXTEDITING SDL_TEXTEDITING
#define DEFAULT_SDL_TEXTINPUT SDL_TEXTINPUT
#define DEFAULT_SDL_KEYMAPCHANGED SDL_KEYMAPCHANGED
#define DEFAULT_SDL_TEXTEDITING_EXT SDL_TEXTEDITING_EXT
#define DEFAULT_SDL_MOUSEMOTION SDL_MOUSEMOTION
#define DEFAULT_SDL_MOUSEBUTTONDOWN SDL_MOUSEBUTTONDOWN
#define DEFAULT_SDL_MOUSEBUTTONUP SDL_MOUSEBUTTONUP
#define DEFAULT_SDL_MOUSEWHEEL SDL_MOUSEWHEEL
#define DEFAULT_SDL_JOYAXISMOTION SDL_JOYAXISMOTION
#define DEFAULT_SDL_JOYBALLMOTION SDL_JOYBALLMOTION
#define DEFAULT_SDL_JOYHATMOTION SDL_JOYHATMOTION
#define DEFAULT_SDL_JOYBUTTONDOWN SDL_JOYBUTTONDOWN
#define DEFAULT_SDL_JOYBUTTONUP SDL_JOYBUTTONUP
#define DEFAULT_SDL_JOYDEVICEADDED SDL_JOYDEVICEADDED
#define DEFAULT_SDL_JOYDEVICEREMOVED SDL_JOYDEVICEREMOVED
#define DEFAULT_SDL_JOYBATTERYUPDATED SDL_JOYBATTERYUPDATED
#define DEFAULT_SDL_CONTROLLERAXISMOTION SDL_CONTROLLERAXISMOTION
#define DEFAULT_SDL_CONTROLLERBUTTONDOWN SDL_CONTROLLERBUTTONDOWN
#define DEFAULT_SDL_CONTROLLERBUTTONUP SDL_CONTROLLERBUTTONUP
#define DEFAULT_SDL_CONTROLLERDEVICEADDED SDL_CONTROLLERDEVICEADDED
#define DEFAULT_SDL_CONTROLLERDEVICEREMOVED SDL_CONTROLLERDEVICEREMOVED
#define DEFAULT_SDL_CONTROLLERDEVICEREMAPPED SDL_CONTROLLERDEVICEREMAPPED
#define DEFAULT_SDL_CONTROLLERTOUCHPADDOWN SDL_CONTROLLERTOUCHPADDOWN
#define DEFAULT_SDL_CONTROLLERTOUCHPADMOTION SDL_CONTROLLERTOUCHPADMOTION
#define DEFAULT_SDL_CONTROLLERTOUCHPADUP SDL_CONTROLLERTOUCHPADUP
#define DEFAULT_SDL_CONTROLLERSENSORUPDATE SDL_CONTROLLERSENSORUPDATE
#define DEFAULT_SDL_FINGERDOWN SDL_FINGERDOWN
#define DEFAULT_SDL_FINGERUP SDL_FINGERUP
#define DEFAULT_SDL_FINGERMOTION SDL_FINGERMOTION
#define DEFAULT_SDL_DOLLARGESTURE SDL_DOLLARGESTURE
#define DEFAULT_SDL_DOLLARRECORD SDL_DOLLARRECORD
#define DEFAULT_SDL_MULTIGESTURE SDL_MULTIGESTURE
#define DEFAULT_SDL_CLIPBOARDUPDATE SDL_CLIPBOARDUPDATE
#define DEFAULT_SDL_DROPFILE SDL_DROPFILE
#define DEFAULT_SDL_DROPTEXT SDL_DROPTEXT
#define DEFAULT_SDL_DROPBEGIN SDL_DROPBEGIN
#define DEFAULT_SDL_DROPCOMPLETE SDL_DROPCOMPLETE
#define DEFAULT_SDL_AUDIODEVICEADDED SDL_AUDIODEVICEADDED
#define DEFAULT_SDL_AUDIODEVICEREMOVED SDL_AUDIODEVICEREMOVED
#define DEFAULT_SDL_SENSORUPDATE SDL_SENSORUPDATE
#define DEFAULT_SDL_RENDER_TARGETS_RESET SDL_RENDER_TARGETS_RESET
#define DEFAULT_SDL_RENDER_DEVICE_RESET SDL_RENDER_DEVICE_RESET
#define DEFAULT_SDL_POLLSENTINEL SDL_POLLSENTINEL
#define DEFAULT_SDL_USEREVENT SDL_USEREVENT
#define DEFAULT_SDL_LASTEVENT SDL_LASTEVENT

/* Keyboard Support */
#define DEFAULT_SDL_SCANCODE_UNKNOWN SDL_SCANCODE_UNKNOWN
#define DEFAULT_SDL_SCANCODE_A SDL_SCANCODE_A
#define DEFAULT_SDL_SCANCODE_B SDL_SCANCODE_B
#define DEFAULT_SDL_SCANCODE_C SDL_SCANCODE_C
#define DEFAULT_SDL_SCANCODE_D SDL_SCANCODE_D
#define DEFAULT_SDL_SCANCODE_E SDL_SCANCODE_E
#define DEFAULT_SDL_SCANCODE_F SDL_SCANCODE_F
#define DEFAULT_SDL_SCANCODE_G SDL_SCANCODE_G
#define DEFAULT_SDL_SCANCODE_H SDL_SCANCODE_H
#define DEFAULT_SDL_SCANCODE_I SDL_SCANCODE_I
#define DEFAULT_SDL_SCANCODE_J SDL_SCANCODE_J
#define DEFAULT_SDL_SCANCODE_K SDL_SCANCODE_K
#define DEFAULT_SDL_SCANCODE_L SDL_SCANCODE_L
#define DEFAULT_SDL_SCANCODE_M SDL_SCANCODE_M
#define DEFAULT_SDL_SCANCODE_N SDL_SCANCODE_N
#define DEFAULT_SDL_SCANCODE_O SDL_SCANCODE_O
#define DEFAULT_SDL_SCANCODE_P SDL_SCANCODE_P
#define DEFAULT_SDL_SCANCODE_Q SDL_SCANCODE_Q
#define DEFAULT_SDL_SCANCODE_R SDL_SCANCODE_R
#define DEFAULT_SDL_SCANCODE_S SDL_SCANCODE_S
#define DEFAULT_SDL_SCANCODE_T SDL_SCANCODE_T
#define DEFAULT_SDL_SCANCODE_U SDL_SCANCODE_U
#define DEFAULT_SDL_SCANCODE_V SDL_SCANCODE_V
#define DEFAULT_SDL_SCANCODE_W SDL_SCANCODE_W
#define DEFAULT_SDL_SCANCODE_X SDL_SCANCODE_X
#define DEFAULT_SDL_SCANCODE_Y SDL_SCANCODE_Y
#define DEFAULT_SDL_SCANCODE_Z SDL_SCANCODE_Z
#define DEFAULT_SDL_SCANCODE_1 SDL_SCANCODE_1
#define DEFAULT_SDL_SCANCODE_2 SDL_SCANCODE_2
#define DEFAULT_SDL_SCANCODE_3 SDL_SCANCODE_3
#define DEFAULT_SDL_SCANCODE_4 SDL_SCANCODE_4
#define DEFAULT_SDL_SCANCODE_5 SDL_SCANCODE_5
#define DEFAULT_SDL_SCANCODE_6 SDL_SCANCODE_6
#define DEFAULT_SDL_SCANCODE_7 SDL_SCANCODE_7
#define DEFAULT_SDL_SCANCODE_8 SDL_SCANCODE_8
#define DEFAULT_SDL_SCANCODE_9 SDL_SCANCODE_9
#define DEFAULT_SDL_SCANCODE_0 SDL_SCANCODE_0
#define DEFAULT_SDL_SCANCODE_RETURN SDL_SCANCODE_RETURN
#define DEFAULT_SDL_SCANCODE_ESCAPE SDL_SCANCODE_ESCAPE
#define DEFAULT_SDL_SCANCODE_BACKSPACE SDL_SCANCODE_BACKSPACE
#define DEFAULT_SDL_SCANCODE_TAB SDL_SCANCODE_TAB
#define DEFAULT_SDL_SCANCODE_SPACE SDL_SCANCODE_SPACE
#define DEFAULT_SDL_SCANCODE_MINUS SDL_SCANCODE_MINUS
#define DEFAULT_SDL_SCANCODE_EQUALS SDL_SCANCODE_EQUALS
#define DEFAULT_SDL_SCANCODE_LEFTBRACKET SDL_SCANCODE_LEFTBRACKET
#define DEFAULT_SDL_SCANCODE_RIGHTBRACKET SDL_SCANCODE_RIGHTBRACKET
#define DEFAULT_SDL_SCANCODE_BACKSLASH SDL_SCANCODE_BACKSLASH
#define DEFAULT_SDL_SCANCODE_NONUSHASH SDL_SCANCODE_NONUSHASH
#define DEFAULT_SDL_SCANCODE_SEMICOLON SDL_SCANCODE_SEMICOLON
#define DEFAULT_SDL_SCANCODE_APOSTROPHE SDL_SCANCODE_APOSTROPHE
#define DEFAULT_SDL_SCANCODE_GRAVE SDL_SCANCODE_GRAVE
#define DEFAULT_SDL_SCANCODE_COMMA SDL_SCANCODE_COMMA
#define DEFAULT_SDL_SCANCODE_PERIOD SDL_SCANCODE_PERIOD
#define DEFAULT_SDL_SCANCODE_SLASH SDL_SCANCODE_SLASH
#define DEFAULT_SDL_SCANCODE_CAPSLOCK SDL_SCANCODE_CAPSLOCK
#define DEFAULT_SDL_SCANCODE_F1 SDL_SCANCODE_F1
#define DEFAULT_SDL_SCANCODE_F2 SDL_SCANCODE_F2
#define DEFAULT_SDL_SCANCODE_F3 SDL_SCANCODE_F3
#define DEFAULT_SDL_SCANCODE_F4 SDL_SCANCODE_F4
#define DEFAULT_SDL_SCANCODE_F5 SDL_SCANCODE_F5
#define DEFAULT_SDL_SCANCODE_F6 SDL_SCANCODE_F6
#define DEFAULT_SDL_SCANCODE_F7 SDL_SCANCODE_F7
#define DEFAULT_SDL_SCANCODE_F8 SDL_SCANCODE_F8
#define DEFAULT_SDL_SCANCODE_F9 SDL_SCANCODE_F9
#define DEFAULT_SDL_SCANCODE_F10 SDL_SCANCODE_F10
#define DEFAULT_SDL_SCANCODE_F11 SDL_SCANCODE_F11
#define DEFAULT_SDL_SCANCODE_F12 SDL_SCANCODE_F12
#define DEFAULT_SDL_SCANCODE_PRINTSCREEN SDL_SCANCODE_PRINTSCREEN
#define DEFAULT_SDL_SCANCODE_SCROLLLOCK SDL_SCANCODE_SCROLLLOCK
#define DEFAULT_SDL_SCANCODE_PAUSE SDL_SCANCODE_PAUSE
#define DEFAULT_SDL_SCANCODE_INSERT SDL_SCANCODE_INSERT
#define DEFAULT_SDL_SCANCODE_HOME SDL_SCANCODE_HOME
#define DEFAULT_SDL_SCANCODE_PAGEUP SDL_SCANCODE_PAGEUP
#define DEFAULT_SDL_SCANCODE_DELETE SDL_SCANCODE_DELETE
#define DEFAULT_SDL_SCANCODE_END SDL_SCANCODE_END
#define DEFAULT_SDL_SCANCODE_PAGEDOWN SDL_SCANCODE_PAGEDOWN
#define DEFAULT_SDL_SCANCODE_RIGHT SDL_SCANCODE_RIGHT
#define DEFAULT_SDL_SCANCODE_LEFT SDL_SCANCODE_LEFT
#define DEFAULT_SDL_SCANCODE_DOWN SDL_SCANCODE_DOWN
#define DEFAULT_SDL_SCANCODE_UP SDL_SCANCODE_UP
#define DEFAULT_SDL_SCANCODE_NUMLOCKCLEAR SDL_SCANCODE_NUMLOCKCLEAR
#define DEFAULT_SDL_SCANCODE_KP_DIVIDE SDL_SCANCODE_KP_DIVIDE
#define DEFAULT_SDL_SCANCODE_KP_MULTIPLY SDL_SCANCODE_KP_MULTIPLY
#define DEFAULT_SDL_SCANCODE_KP_MINUS SDL_SCANCODE_KP_MINUS
#define DEFAULT_SDL_SCANCODE_KP_PLUS SDL_SCANCODE_KP_PLUS
#define DEFAULT_SDL_SCANCODE_KP_ENTER SDL_SCANCODE_KP_ENTER
#define DEFAULT_SDL_SCANCODE_KP_1 SDL_SCANCODE_KP_1
#define DEFAULT_SDL_SCANCODE_KP_2 SDL_SCANCODE_KP_2
#define DEFAULT_SDL_SCANCODE_KP_3 SDL_SCANCODE_KP_3
#define DEFAULT_SDL_SCANCODE_KP_4 SDL_SCANCODE_KP_4
#define DEFAULT_SDL_SCANCODE_KP_5 SDL_SCANCODE_KP_5
#define DEFAULT_SDL_SCANCODE_KP_6 SDL_SCANCODE_KP_6
#define DEFAULT_SDL_SCANCODE_KP_7 SDL_SCANCODE_KP_7
#define DEFAULT_SDL_SCANCODE_KP_8 SDL_SCANCODE_KP_8
#define DEFAULT_SDL_SCANCODE_KP_9 SDL_SCANCODE_KP_9
#define DEFAULT_SDL_SCANCODE_KP_0 SDL_SCANCODE_KP_0
#define DEFAULT_SDL_SCANCODE_KP_PERIOD SDL_SCANCODE_KP_PERIOD
#define DEFAULT_SDL_SCANCODE_NONUSBACKSLASH SDL_SCANCODE_NONUSBACKSLASH
#define DEFAULT_SDL_SCANCODE_APPLICATION SDL_SCANCODE_APPLICATION
#define DEFAULT_SDL_SCANCODE_POWER SDL_SCANCODE_POWER
#define DEFAULT_SDL_SCANCODE_KP_EQUALS SDL_SCANCODE_KP_EQUALS
#define DEFAULT_SDL_SCANCODE_F13 SDL_SCANCODE_F13
#define DEFAULT_SDL_SCANCODE_F14 SDL_SCANCODE_F14
#define DEFAULT_SDL_SCANCODE_F15 SDL_SCANCODE_F15
#define DEFAULT_SDL_SCANCODE_F16 SDL_SCANCODE_F16
#define DEFAULT_SDL_SCANCODE_F17 SDL_SCANCODE_F17
#define DEFAULT_SDL_SCANCODE_F18 SDL_SCANCODE_F18
#define DEFAULT_SDL_SCANCODE_F19 SDL_SCANCODE_F19
#define DEFAULT_SDL_SCANCODE_F20 SDL_SCANCODE_F20
#define DEFAULT_SDL_SCANCODE_F21 SDL_SCANCODE_F21
#define DEFAULT_SDL_SCANCODE_F22 SDL_SCANCODE_F22
#define DEFAULT_SDL_SCANCODE_F23 SDL_SCANCODE_F23
#define DEFAULT_SDL_SCANCODE_F24 SDL_SCANCODE_F24
#define DEFAULT_SDL_SCANCODE_EXECUTE SDL_SCANCODE_EXECUTE
#define DEFAULT_SDL_SCANCODE_HELP SDL_SCANCODE_HELP
#define DEFAULT_SDL_SCANCODE_MENU SDL_SCANCODE_MENU
#define DEFAULT_SDL_SCANCODE_SELECT SDL_SCANCODE_SELECT
#define DEFAULT_SDL_SCANCODE_STOP SDL_SCANCODE_STOP
#define DEFAULT_SDL_SCANCODE_AGAIN SDL_SCANCODE_AGAIN
#define DEFAULT_SDL_SCANCODE_UNDO SDL_SCANCODE_UNDO
#define DEFAULT_SDL_SCANCODE_CUT SDL_SCANCODE_CUT
#define DEFAULT_SDL_SCANCODE_COPY SDL_SCANCODE_COPY
#define DEFAULT_SDL_SCANCODE_PASTE SDL_SCANCODE_PASTE
#define DEFAULT_SDL_SCANCODE_FIND SDL_SCANCODE_FIND
#define DEFAULT_SDL_SCANCODE_MUTE SDL_SCANCODE_MUTE
#define DEFAULT_SDL_SCANCODE_VOLUMEUP SDL_SCANCODE_VOLUMEUP
#define DEFAULT_SDL_SCANCODE_VOLUMEDOWN SDL_SCANCODE_VOLUMEDOWN
#define DEFAULT_SDL_SCANCODE_KP_COMMA SDL_SCANCODE_KP_COMMA
#define DEFAULT_SDL_SCANCODE_KP_EQUALSAS400 SDL_SCANCODE_KP_EQUALSAS400
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL1 SDL_SCANCODE_INTERNATIONAL1
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL2 SDL_SCANCODE_INTERNATIONAL2
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL3 SDL_SCANCODE_INTERNATIONAL3
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL4 SDL_SCANCODE_INTERNATIONAL4
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL5 SDL_SCANCODE_INTERNATIONAL5
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL6 SDL_SCANCODE_INTERNATIONAL6
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL7 SDL_SCANCODE_INTERNATIONAL7
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL8 SDL_SCANCODE_INTERNATIONAL8
#define DEFAULT_SDL_SCANCODE_INTERNATIONAL9 SDL_SCANCODE_INTERNATIONAL9
#define DEFAULT_SDL_SCANCODE_LANG1 SDL_SCANCODE_LANG1
#define DEFAULT_SDL_SCANCODE_LANG2 SDL_SCANCODE_LANG2
#define DEFAULT_SDL_SCANCODE_LANG3 SDL_SCANCODE_LANG3
#define DEFAULT_SDL_SCANCODE_LANG4 SDL_SCANCODE_LANG4
#define DEFAULT_SDL_SCANCODE_LANG5 SDL_SCANCODE_LANG5
#define DEFAULT_SDL_SCANCODE_LANG6 SDL_SCANCODE_LANG6
#define DEFAULT_SDL_SCANCODE_LANG7 SDL_SCANCODE_LANG7
#define DEFAULT_SDL_SCANCODE_LANG8 SDL_SCANCODE_LANG8
#define DEFAULT_SDL_SCANCODE_LANG9 SDL_SCANCODE_LANG9
#define DEFAULT_SDL_SCANCODE_ALTERASE SDL_SCANCODE_ALTERASE
#define DEFAULT_SDL_SCANCODE_SYSREQ SDL_SCANCODE_SYSREQ
#define DEFAULT_SDL_SCANCODE_CANCEL SDL_SCANCODE_CANCEL
#define DEFAULT_SDL_SCANCODE_CLEAR SDL_SCANCODE_CLEAR
#define DEFAULT_SDL_SCANCODE_PRIOR SDL_SCANCODE_PRIOR
#define DEFAULT_SDL_SCANCODE_RETURN2 SDL_SCANCODE_RETURN2
#define DEFAULT_SDL_SCANCODE_SEPARATOR SDL_SCANCODE_SEPARATOR
#define DEFAULT_SDL_SCANCODE_OUT SDL_SCANCODE_OUT
#define DEFAULT_SDL_SCANCODE_OPER SDL_SCANCODE_OPER
#define DEFAULT_SDL_SCANCODE_CLEARAGAIN SDL_SCANCODE_CLEARAGAIN
#define DEFAULT_SDL_SCANCODE_CRSEL SDL_SCANCODE_CRSEL
#define DEFAULT_SDL_SCANCODE_EXSEL SDL_SCANCODE_EXSEL
#define DEFAULT_SDL_SCANCODE_KP_00 SDL_SCANCODE_KP_00
#define DEFAULT_SDL_SCANCODE_KP_000 SDL_SCANCODE_KP_000
#define DEFAULT_SDL_SCANCODE_THOUSANDSSEPARATOR \
    SDL_SCANCODE_THOUSANDSSEPARATOR
#define DEFAULT_SDL_SCANCODE_DECIMALSEPARATOR SDL_SCANCODE_DECIMALSEPARATOR
#define DEFAULT_SDL_SCANCODE_CURRENCYUNIT SDL_SCANCODE_CURRENCYUNIT
#define DEFAULT_SDL_SCANCODE_CURRENCYSUBUNIT SDL_SCANCODE_CURRENCYSUBUNIT
#define DEFAULT_SDL_SCANCODE_KP_LEFTPAREN SDL_SCANCODE_KP_LEFTPAREN
#define DEFAULT_SDL_SCANCODE_KP_RIGHTPAREN SDL_SCANCODE_KP_RIGHTPAREN
#define DEFAULT_SDL_SCANCODE_KP_LEFTBRACE SDL_SCANCODE_KP_LEFTBRACE
#define DEFAULT_SDL_SCANCODE_KP_RIGHTBRACE SDL_SCANCODE_KP_RIGHTBRACE
#define DEFAULT_SDL_SCANCODE_KP_TAB SDL_SCANCODE_KP_TAB
#define DEFAULT_SDL_SCANCODE_KP_BACKSPACE SDL_SCANCODE_KP_BACKSPACE
#define DEFAULT_SDL_SCANCODE_KP_A SDL_SCANCODE_KP_A
#define DEFAULT_SDL_SCANCODE_KP_B SDL_SCANCODE_KP_B
#define DEFAULT_SDL_SCANCODE_KP_C SDL_SCANCODE_KP_C
#define DEFAULT_SDL_SCANCODE_KP_D SDL_SCANCODE_KP_D
#define DEFAULT_SDL_SCANCODE_KP_E SDL_SCANCODE_KP_E
#define DEFAULT_SDL_SCANCODE_KP_F SDL_SCANCODE_KP_F
#define DEFAULT_SDL_SCANCODE_KP_XOR SDL_SCANCODE_KP_XOR
#define DEFAULT_SDL_SCANCODE_KP_POWER SDL_SCANCODE_KP_POWER
#define DEFAULT_SDL_SCANCODE_KP_PERCENT SDL_SCANCODE_KP_PERCENT
#define DEFAULT_SDL_SCANCODE_KP_LESS SDL_SCANCODE_KP_LESS
#define DEFAULT_SDL_SCANCODE_KP_GREATER SDL_SCANCODE_KP_GREATER
#define DEFAULT_SDL_SCANCODE_KP_AMPERSAND SDL_SCANCODE_KP_AMPERSAND
#define DEFAULT_SDL_SCANCODE_KP_DBLAMPERSAND SDL_SCANCODE_KP_DBLAMPERSAND
#define DEFAULT_SDL_SCANCODE_KP_VERTICALBAR SDL_SCANCODE_KP_VERTICALBAR
#define DEFAULT_SDL_SCANCODE_KP_DBLVERTICALBAR SDL_SCANCODE_KP_DBLVERTICALBAR
#define DEFAULT_SDL_SCANCODE_KP_COLON SDL_SCANCODE_KP_COLON
#define DEFAULT_SDL_SCANCODE_KP_HASH SDL_SCANCODE_KP_HASH
#define DEFAULT_SDL_SCANCODE_KP_SPACE SDL_SCANCODE_KP_SPACE
#define DEFAULT_SDL_SCANCODE_KP_AT SDL_SCANCODE_KP_AT
#define DEFAULT_SDL_SCANCODE_KP_EXCLAM SDL_SCANCODE_KP_EXCLAM
#define DEFAULT_SDL_SCANCODE_KP_MEMSTORE SDL_SCANCODE_KP_MEMSTORE
#define DEFAULT_SDL_SCANCODE_KP_MEMRECALL SDL_SCANCODE_KP_MEMRECALL
#define DEFAULT_SDL_SCANCODE_KP_MEMCLEAR SDL_SCANCODE_KP_MEMCLEAR
#define DEFAULT_SDL_SCANCODE_KP_MEMADD SDL_SCANCODE_KP_MEMADD
#define DEFAULT_SDL_SCANCODE_KP_MEMSUBTRACT SDL_SCANCODE_KP_MEMSUBTRACT
#define DEFAULT_SDL_SCANCODE_KP_MEMMULTIPLY SDL_SCANCODE_KP_MEMMULTIPLY
#define DEFAULT_SDL_SCANCODE_KP_MEMDIVIDE SDL_SCANCODE_KP_MEMDIVIDE
#define DEFAULT_SDL_SCANCODE_KP_PLUSMINUS SDL_SCANCODE_KP_PLUSMINUS
#define DEFAULT_SDL_SCANCODE_KP_CLEAR SDL_SCANCODE_KP_CLEAR
#define DEFAULT_SDL_SCANCODE_KP_CLEARENTRY SDL_SCANCODE_KP_CLEARENTRY
#define DEFAULT_SDL_SCANCODE_KP_BINARY SDL_SCANCODE_KP_BINARY
#define DEFAULT_SDL_SCANCODE_KP_OCTAL SDL_SCANCODE_KP_OCTAL
#define DEFAULT_SDL_SCANCODE_KP_DECIMAL SDL_SCANCODE_KP_DECIMAL
#define DEFAULT_SDL_SCANCODE_KP_HEXADECIMAL SDL_SCANCODE_KP_HEXADECIMAL
#define DEFAULT_SDL_SCANCODE_LCTRL SDL_SCANCODE_LCTRL
#define DEFAULT_SDL_SCANCODE_LSHIFT SDL_SCANCODE_LSHIFT
#define DEFAULT_SDL_SCANCODE_LALT SDL_SCANCODE_LALT
#define DEFAULT_SDL_SCANCODE_LGUI SDL_SCANCODE_LGUI
#define DEFAULT_SDL_SCANCODE_RCTRL SDL_SCANCODE_RCTRL
#define DEFAULT_SDL_SCANCODE_RSHIFT SDL_SCANCODE_RSHIFT
#define DEFAULT_SDL_SCANCODE_RALT SDL_SCANCODE_RALT
#define DEFAULT_SDL_SCANCODE_RGUI SDL_SCANCODE_RGUI
#define DEFAULT_SDL_SCANCODE_MODE SDL_SCANCODE_MODE
#define DEFAULT_SDL_SCANCODE_AUDIONEXT SDL_SCANCODE_AUDIONEXT
#define DEFAULT_SDL_SCANCODE_AUDIOPREV SDL_SCANCODE_AUDIOPREV
#define DEFAULT_SDL_SCANCODE_AUDIOSTOP SDL_SCANCODE_AUDIOSTOP
#define DEFAULT_SDL_SCANCODE_AUDIOPLAY SDL_SCANCODE_AUDIOPLAY
#define DEFAULT_SDL_SCANCODE_AUDIOMUTE SDL_SCANCODE_AUDIOMUTE
#define DEFAULT_SDL_SCANCODE_MEDIASELECT SDL_SCANCODE_MEDIASELECT
#define DEFAULT_SDL_SCANCODE_WWW SDL_SCANCODE_WWW
#define DEFAULT_SDL_SCANCODE_MAIL SDL_SCANCODE_MAIL
#define DEFAULT_SDL_SCANCODE_CALCULATOR SDL_SCANCODE_CALCULATOR
#define DEFAULT_SDL_SCANCODE_COMPUTER SDL_SCANCODE_COMPUTER
#define DEFAULT_SDL_SCANCODE_AC_SEARCH SDL_SCANCODE_AC_SEARCH
#define DEFAULT_SDL_SCANCODE_AC_HOME SDL_SCANCODE_AC_HOME
#define DEFAULT_SDL_SCANCODE_AC_BACK SDL_SCANCODE_AC_BACK
#define DEFAULT_SDL_SCANCODE_AC_FORWARD SDL_SCANCODE_AC_FORWARD
#define DEFAULT_SDL_SCANCODE_AC_STOP SDL_SCANCODE_AC_STOP
#define DEFAULT_SDL_SCANCODE_AC_REFRESH SDL_SCANCODE_AC_REFRESH
#define DEFAULT_SDL_SCANCODE_AC_BOOKMARKS SDL_SCANCODE_AC_BOOKMARKS
#define DEFAULT_SDL_SCANCODE_BRIGHTNESSDOWN SDL_SCANCODE_BRIGHTNESSDOWN
#define DEFAULT_SDL_SCANCODE_BRIGHTNESSUP SDL_SCANCODE_BRIGHTNESSUP
#define DEFAULT_SDL_SCANCODE_DISPLAYSWITCH SDL_SCANCODE_DISPLAYSWITCH
#define DEFAULT_SDL_SCANCODE_KBDILLUMTOGGLE SDL_SCANCODE_KBDILLUMTOGGLE
#define DEFAULT_SDL_SCANCODE_KBDILLUMDOWN SDL_SCANCODE_KBDILLUMDOWN
#define DEFAULT_SDL_SCANCODE_KBDILLUMUP SDL_SCANCODE_KBDILLUMUP
#define DEFAULT_SDL_SCANCODE_EJECT SDL_SCANCODE_EJECT
#define DEFAULT_SDL_SCANCODE_SLEEP SDL_SCANCODE_SLEEP
#define DEFAULT_SDL_SCANCODE_APP1 SDL_SCANCODE_APP1
#define DEFAULT_SDL_SCANCODE_APP2 SDL_SCANCODE_APP2
#define DEFAULT_SDL_SCANCODE_AUDIOREWIND SDL_SCANCODE_AUDIOREWIND
#define DEFAULT_SDL_SCANCODE_AUDIOFASTFORWARD SDL_SCANCODE_AUDIOFASTFORWARD
#define DEFAULT_SDL_SCANCODE_SOFTLEFT SDL_SCANCODE_SOFTLEFT
#define DEFAULT_SDL_SCANCODE_SOFTRIGHT SDL_SCANCODE_SOFTRIGHT
#define DEFAULT_SDL_SCANCODE_CALL SDL_SCANCODE_CALL
#define DEFAULT_SDL_SCANCODE_ENDCALL SDL_SCANCODE_ENDCALL

/* Message boxes */
#define DEFAULT_SDL_MESSAGEBOX_ERROR SDL_MESSAGEBOX_ERROR
#define DEFAULT_SDL_MESSAGEBOX_WARNING SDL_MESSAGEBOX_WARNING
#define DEFAULT_SDL_MESSAGEBOX_INFORMATION SDL_MESSAGEBOX_INFORMATION
#define DEFAULT_SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT \
    SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT
#define DEFAULT_SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT \
    SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT

/* Standard Library Functionality */
#define DEFAULT_SDL_FALSE SDL_FALSE
#define DEFAULT_SDL_TRUE SDL_TRUE

#define ENTRY(VAR, CONST)                       \
    {                                           \
        &VAR##_node, #VAR, DEFAULT_##VAR, CONST \
    }

#define NUM_SCALARS (sizeof(varinit) / sizeof(varinit[0]))
#define NUM_RESET (NUM_SCALARS - 1)
