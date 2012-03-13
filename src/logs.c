#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "mediaprofile.h"

void
mp_log (media_profile_verbosity_level_t max,
        media_profile_verbosity_level_t level, const char *format, ...)
{
  char fmt[256];
  va_list va;

  if (!format)
    return;

  if (level > max)
    return;

  snprintf (fmt, sizeof (fmt),
            "[mediaprofile] %s\n", format);

  va_start (va, format);
  vfprintf (stderr, fmt, va);
  va_end (va);
}
