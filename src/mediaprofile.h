#ifndef MEDIA_PROFILE_H
#define MEDIA_PROFILE_H

#ifdef __cplusplus
extern "C" {
#if 0 /* avoid EMACS indent */
}
#endif /* 0 */
#endif /* __cplusplus */

#define MEDIA_PROFILE_STRINGIFY(s)         MEDIA_PROFILE_TOSTRING(s)
#define MEDIA_PROFILE_TOSTRING(s) #s

#define MEDIA_PROFILE_VERSION_INT  ((0 << 16) + (2 << 8) + 3)
#define MEDIA_PROFILE_VERSION      0.0.1
#define MEDIA_PROFILE_BUILD        MEDIA_PROFILE_VERSION_INT

#define MEDIA_PROFILE_IDENT        "MEDIA_PROFILE " MEDIA_PROFILE_STRINGIFY(MEDIA_PROFILE_VERSION)

#include <inttypes.h>
#include <sys/types.h>

typedef enum media_profile_type {
  MEDIA_PROFILE_TYPE_UNKNOWN,
  MEDIA_PROFILE_TYPE_AUDIO,
  MEDIA_PROFILE_TYPE_VIDEO,
  MEDIA_PROFILE_TYPE_IMAGE,
  MEDIA_PROFILE_TYPE_SUB,
} media_profile_type_t;

typedef struct media_profile_metadata_s {
  char *title;
  char *artist;
  char *genre;
  char *album;
  char *year;
  char *track;
  char *comment;
} media_profile_metadata_t;

typedef struct media_profile_audio_s {
  char *codec;
  uint32_t length;
  uint32_t bitrate;
  uint32_t bits;
  uint32_t channels;
  uint32_t samplerate;
  uint8_t vbr;
} media_profile_audio_t;

typedef struct media_profile_video_s {
  char *codec;
  uint32_t length;
  uint32_t bitrate;
  uint32_t width;
  uint32_t height;
} media_profile_video_t;

typedef struct media_profile_image_s {
  char *codec;
  uint32_t width;
  uint32_t height;
  uint32_t depth;
} media_profile_image_t;

typedef struct media_profile_sub_s {
  char *codec;
  char *lang;
} media_profile_sub_t;

typedef struct media_profile_s {

  off_t size;
  media_profile_type_t type;
  media_profile_metadata_t *meta;

  int nr_audio_channels;
  media_profile_audio_t **audio;

  int nr_video_channels;
  media_profile_video_t **video;

  media_profile_image_t *img;
  media_profile_sub_t *sub;

} media_profile_t;

media_profile_t *media_profile_guess (const char *filename);
void media_profile_free (media_profile_t *media);

#ifdef __cplusplus
#if 0 /* avoid EMACS indent */
{
#endif /* 0 */
}
#endif /* __cplusplus */

#endif /* MEDIA_PROFILE_H */
