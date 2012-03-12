#ifndef MEDIA_PROFILE_INTERNALS_H
#define MEDIA_PROFILE_INTERNALS_H

#include <stdio.h>
#include <stdlib.h>

#include "mediaprofile.h"
#include "reader.h"

#define MP_FREE(x) do { if (x) free (x); } while (0)

media_profile_metadata_t *mp_metadata_new (void);
void mp_metadata_free (media_profile_metadata_t *meta);

media_profile_audio_t *mp_audio_new (void);
void mp_audio_free (media_profile_audio_t *audio);
void mp_add_audio_stream (media_profile_t *mp, media_profile_audio_t *a);

media_profile_video_t *mp_video_new (void);
void mp_video_free (media_profile_video_t *video);
void mp_add_video_stream (media_profile_t *mp, media_profile_video_t *v);

media_profile_image_t *mp_image_new (void);
void mp_image_free (media_profile_image_t *image);

media_profile_sub_t *mp_sub_new (void);
void mp_sub_free (media_profile_sub_t *sub);

media_profile_t *mp_new (void);
void mp_free (media_profile_t *mp);

#define MP_PARSER_OK 0
#define MP_PARSER_ERROR 1

typedef struct mp_parser_s {
  const char *name;
  media_profile_type_t type;
  const char *extensions;
  int (*parse) (media_profile_t *p, mp_stream_t *s);
} mp_parser_t;

int mp_file_extension_match (const char *filename, const char *extensions);

#endif /* MEDIA_PROFILE_INTERNALS_H */
