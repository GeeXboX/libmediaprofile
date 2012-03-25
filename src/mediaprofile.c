#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mediaprofile_internals.h"

/* image parsers */
extern mp_parser_t mp_parser_bmp;
extern mp_parser_t mp_parser_png;
extern mp_parser_t mp_parser_jpg;
extern mp_parser_t mp_parser_tiff;

/* audio parsers */
extern mp_parser_t mp_parser_mp3;
extern mp_parser_t mp_parser_ogg;

static mp_parser_t *mp_parsers[] = {
  &mp_parser_bmp,
  &mp_parser_png,
  &mp_parser_jpg,
  &mp_parser_tiff,
  &mp_parser_mp3,
  &mp_parser_ogg,
  NULL,
};

static void
mp_file_parse (media_profile_t *mp,
               const char *filename, media_profile_verbosity_level_t v)
{
  mp_parser_t **p;
  mp_stream_t *s;
  int err;

  s = mp_stream_open (filename);

  p = mp_parsers;
  while (*p)
  {
    err = mp_file_extension_match (filename, (*p)->extensions);
    if (err != MP_PARSER_OK)
      goto next;

    MP_LOG_DEBUG (v, "Found Parser: %s", (*p)->name);
    err = (*p)->parse (mp, s, v);
    if (err == MP_PARSER_OK)
    {
      mp->type = (*p)->type;
      goto end;
    }

  next:
    (void) *p++;
  }

end:
  mp_stream_close (s);
}

media_profile_t *
media_profile_guess (const char *filename, media_profile_verbosity_level_t v)
{
  media_profile_t *mp = NULL;
  struct stat st;
  int err;

  if (!filename)
    return NULL;

  err = stat (filename, &st);
  if (err == -1)
    return NULL;

  mp = calloc (1, sizeof (*mp));

  mp->size = st.st_size;
  mp->type = MEDIA_PROFILE_TYPE_UNKNOWN;

  /* now extract file information */
  mp_file_parse (mp, filename, v);

  return mp;
}

void
media_profile_free (media_profile_t *media)
{
  mp_free (media);
}
