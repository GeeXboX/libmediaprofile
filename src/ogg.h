#ifndef MEDIA_PROFILE_OGG_H
#define MEDIA_PROFILE_OGG_H

typedef struct ogg_stream_s {
  const char *name;
  const int8_t *signature;
  uint8_t signature_size;
  uint8_t header_size;
  int (*parse) (media_profile_t *p, mp_stream_t *s,
                 media_profile_verbosity_level_t v);
} ogg_stream_t;

#endif /* MEDIA_PROFILE_OGG_H */
