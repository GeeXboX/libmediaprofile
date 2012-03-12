#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "mediaprofile_internals.h"

media_profile_metadata_t *
mp_metadata_new (void)
{
  media_profile_metadata_t *meta = calloc (1, sizeof (*meta));
  return meta;
}

void
mp_metadata_free (media_profile_metadata_t *meta)
{
  if (!meta)
    return;

  MP_FREE (meta->title);
  MP_FREE (meta->artist);
  MP_FREE (meta->genre);
  MP_FREE (meta->album);
  MP_FREE (meta->year);
  MP_FREE (meta->track);
  MP_FREE (meta->comment);
  MP_FREE (meta);
}

media_profile_audio_t *
mp_audio_new (void)
{
  media_profile_audio_t *audio = calloc (1, sizeof (*audio));
  return audio;
}

void
mp_audio_free (media_profile_audio_t *audio)
{
  if (!audio)
    return;

  MP_FREE (audio->codec);
  MP_FREE (audio);
}

media_profile_video_t *
mp_video_new (void)
{
  media_profile_video_t *video = calloc (1, sizeof (*video));
  return video;
}

void
mp_video_free (media_profile_video_t *video)
{
  if (!video)
    return;

  MP_FREE (video->codec);
  MP_FREE (video);
}

media_profile_image_t *
mp_image_new (void)
{
  media_profile_image_t *image = calloc (1, sizeof (*image));
  return image;
}

void
mp_image_free (media_profile_image_t *image)
{
  if (!image)
    return;

  MP_FREE (image->codec);
  MP_FREE (image);
}

media_profile_sub_t *
mp_sub_new (void)
{
  media_profile_sub_t *sub = calloc (1, sizeof (*sub));
  return sub;
}

void
mp_sub_free (media_profile_sub_t *sub)
{
  if (!sub)
    return;

  MP_FREE (sub->codec);
  MP_FREE (sub->lang);
  MP_FREE (sub);
}

media_profile_t *
mp_new (void)
{
  media_profile_t *mp = calloc (1, sizeof (*mp));
  return mp;
}

void
mp_free (media_profile_t *mp)
{
  int i;

  if (!mp)
    return;

  mp_metadata_free (mp->meta);

  for (i = 0; i < mp->nr_audio_streams; i++)
    mp_audio_free (mp->audio[i]);
  MP_FREE (mp->audio);

  for (i = 0; i < mp->nr_video_streams; i++)
    mp_video_free (mp->video[i]);
  MP_FREE (mp->video);

  mp_image_free (mp->img);
  mp_sub_free (mp->sub);

  MP_FREE (mp);
}

int
mp_file_extension_match (const char *filename, const char *extensions)
{
    const char *ext, *e;
    char ext1[32], *q;

    if (!filename || !extensions)
      return MP_PARSER_ERROR;

    ext = strrchr (filename, '.');
    if (!ext)
      return MP_PARSER_ERROR;

    ext++;
    e = extensions;

    for (;;)
    {
      q = ext1;
      while ((*e != '\0' ) && (*e != ',' ) && (q - ext1 < sizeof (ext1) - 1))
        *q++ = *e++;
      *q = '\0';
      if (!strcasecmp (ext1, ext))
        return MP_PARSER_OK;
      if (*e == '\0')
        break;
      e++;
    }

    return MP_PARSER_ERROR;
}
