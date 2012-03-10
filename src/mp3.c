#include "mediaprofile_internals.h"

static int
mp3_parser (media_profile_t *p, mp_stream_t *s)
{
  printf ("Parsing MP3 file ...\n");

  return MP_PARSER_OK;
}

mp_parser_t mp_parser_mp3 = {
  .name       = "MPEG Audio",
  .type       = MEDIA_PROFILE_TYPE_AUDIO,
  .extensions = "mpa,mp3",
  .parse      = mp3_parser,
};
