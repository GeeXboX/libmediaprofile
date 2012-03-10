#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mediaprofile_internals.h"

mp_stream_t *
mp_stream_open (const char *filename)
{
  mp_stream_t *s;

  if (!filename)
    return NULL;

  s = calloc (1, sizeof (*s));
  s->f = fopen (filename, "r");

  return s;
}

void
mp_stream_skip (mp_stream_t *s, int len)
{
  if (!s)
    return;

  fseek (s->f, len, SEEK_CUR);
}

uint8_t
mp_stream_get_u8 (mp_stream_t *s)
{
  if (!s)
    return -1;

  return getc (s->f);
}

uint8_t *
mp_stream_get_buffer (mp_stream_t *s, int len)
{
  uint8_t *dst;
  int i;

  if (!s)
    return NULL;

  dst = calloc (1, len);
  for (i = 0; i < len; i++)
    dst[i] = mp_stream_get_u8 (s);

  return dst;
}

uint32_t
mp_stream_get_le32 (mp_stream_t *s)
{
  uint8_t *buf;
  uint32_t r;

  buf = mp_stream_get_buffer (s, 4);
  r = MP_RD_LE32 (buf);
  MP_FREE (buf);

  return r;
}

uint32_t
mp_stream_get_be32 (mp_stream_t *s)
{
  uint8_t *buf;
  uint32_t r;

  buf = mp_stream_get_buffer (s, 4);
  r = MP_RD_BE32 (buf);
  MP_FREE (buf);

  return r;
}

void
mp_stream_close (mp_stream_t *s)
{
  if (!s)
    return;

  fclose (s->f);
  MP_FREE (s);
}