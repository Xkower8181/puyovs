/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2012 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef _SDL_config_minimal_h
#define _SDL_config_minimal_h

#include "SDL_platform.h"

/**
 *  \file SDL_config_linux.h
 *  
 *  Minimal linux config.
 */

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __GNUC__
#define HAVE_GCC_SYNC_LOCK_TEST_AND_SET 1
#endif

#define HAVE_ALLOCA_H    1
#define HAVE_SYS_TYPES_H 1
#define HAVE_STDIO_H     1
#define STDC_HEADERS     1
#define HAVE_STRING_H    1
#define HAVE_INTTYPES_H  1
#define HAVE_STDINT_H    1
#define HAVE_CTYPE_H     1
#define HAVE_MATH_H      1
#define HAVE_SIGNAL_H    1

#define HAVE_MALLOC      1
#define HAVE_CALLOC      1
#define HAVE_REALLOC     1
#define HAVE_FREE        1
#define HAVE_ALLOCA      1
#define HAVE_GETENV      1
#define HAVE_SETENV      1
#define HAVE_PUTENV      1
#define HAVE_UNSETENV    1
#define HAVE_QSORT       1
#define HAVE_ABS         1
#define HAVE_BCOPY       1
#define HAVE_MEMSET      1
#define HAVE_MEMCPY      1
#define HAVE_MEMMOVE     1
#define HAVE_MEMCMP      1
#define HAVE_STRLEN      1
#define HAVE_STRNCPY     1
#define HAVE_STRNCAT     1
#define HAVE_STRDUP      1
#define HAVE_STRCHR      1
#define HAVE_STRRCHR     1
#define HAVE_STRSTR      1
#define HAVE_STRTOL      1
#define HAVE_STRTOUL     1
#define HAVE_STRTOLL     1
#define HAVE_STRTOULL    1
#define HAVE_STRTOD      1
#define HAVE_ATOI        1
#define HAVE_ATOF        1
#define HAVE_STRCMP      1
#define HAVE_STRNCMP     1
#define HAVE_STRCASECMP  1
#define HAVE_STRNCASECMP 1
#define HAVE_SSCANF      1
#define HAVE_SNPRINTF    1
#define HAVE_VSNPRINTF   1
#define HAVE_CEIL        1
#define HAVE_COPYSIGN    1
#define HAVE_COS         1
#define HAVE_COSF        1
#define HAVE_FABS        1
#define HAVE_FLOOR       1
#define HAVE_LOG         1
#define HAVE_POW         1
#define HAVE_SCALBN      1
#define HAVE_SIN         1
#define HAVE_SINF        1
#define HAVE_SQRT        1
#define HAVE_SIGACTION   1
#define HAVE_SETJMP      1
#define HAVE_NANOSLEEP   1
#define HAVE_SYSCONF     1
#define HAVE_ATAN        1
#define HAVE_ATAN2       1
#define HAVE_INOTIFY     1

// Audio drivers
#define SDL_AUDIO_DRIVER_ALSA	        1
#define SDL_AUDIO_DRIVER_ALSA_DYNAMIC   "libasound.so.2"
#define SDL_AUDIO_DRIVER_PULSEAUDIO	    1
#define SDL_AUDIO_DRIVER_PULSEAUDIO_DYNAMIC "libpulse-simple.so.0"
#define SDL_AUDIO_DRIVER_OSS            1

// Input
#define SDL_INPUT_LINUXEV               1

// Joystick drivers
#define SDL_JOYSTICK_LINUX              1
#define SDL_JOYSTICK_HIDAPI             1

// Haptic drivers (or not)
#define SDL_HAPTIC_DISABLED             1

// Shared object loading code
#define SDL_LOADSO_DLOPEN               1

// Threading
#define SDL_THREAD_PTHREAD              1

// Timers
#define SDL_TIMER_UNIX                  1

// Video
#define SDL_VIDEO_DRIVER_X11            0
#define SDL_VIDEO_OPENGL                0
#define SDL_VIDEO_OPENGL_GLX            0
#define SDL_VIDEO_DRIVER_DUMMY          1
#define SDL_VIDEO_RENDER_SOFTWARE       1

// Sensor
#define SDL_SENSOR_DUMMY                1

// Filesystem
#define SDL_FILESYSTEM_UNIX             1

#endif /* _SDL_config_minimal_h */
