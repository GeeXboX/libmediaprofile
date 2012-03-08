#ifndef MEDIA_PROFILE_INTERNALS_H
#define MEDIA_PROFILE_INTERNALS_H

#include "mediaprofile.h"

#define MP_FREE(x) do { if (x) free (x); } while (0)

media_profile_metadata_t *mp_metadata_new (void);
void mp_metadata_free (media_profile_metadata_t *meta);

media_profile_audio_t *mp_audio_new (void);
void mp_audio_free (media_profile_audio_t *audio);

media_profile_video_t *mp_video_new (void);
void mp_video_free (media_profile_video_t *video);

media_profile_image_t *mp_image_new (void);
void mp_image_free (media_profile_image_t *image);

media_profile_sub_t *mp_sub_new (void);
void mp_sub_free (media_profile_sub_t *sub);

media_profile_t *mp_new (void);
void mp_free (media_profile_t *mp);

#endif /* MEDIA_PROFILE_INTERNALS_H */
