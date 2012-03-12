#include <stdio.h>

#include "mediaprofile.h"

int
main (int argc, char **argv)
{
  media_profile_t *mp;
  const char *media_type;
  int i;

  if (argc != 2)
  {
    printf ("Usage: %s filename\n", argv[0]);
    return 1;
  }

  mp = media_profile_guess (argv[1]);
  if (!mp)
    return 1;

  printf ("\n");
  printf ("Media Information:\n");
  printf ("  Filename:\t\t%s\n", argv[1]);
  printf ("  Size:\t\t\t%jd bytes\n", mp->size);

  switch (mp->type)
  {
  case MEDIA_PROFILE_TYPE_AUDIO:
    media_type = "Audio";
    break;
  case MEDIA_PROFILE_TYPE_VIDEO:
    media_type = "Video";
    break;
  case MEDIA_PROFILE_TYPE_IMAGE:
    media_type = "Image";
    break;
  case MEDIA_PROFILE_TYPE_SUB:
    media_type = "Subtitle";
    break;
  default:
    media_type = "Unknown";
    break;
  }

  printf ("  Media Type:\t\t%s\n", media_type);
  printf ("\n");

  if (mp->meta)
  {
    printf ("Metadata:\n");
    printf ("  Title:\t\t%s\n", mp->meta->title);
    printf ("  Artist:\t\t%s\n", mp->meta->artist);
    printf ("  Genre:\t\t%s\n", mp->meta->genre);
    printf ("  Album:\t\t%s\n", mp->meta->album);
    printf ("  Year:\t\t\t%s\n", mp->meta->year);
    printf ("  Track:\t\t%s\n", mp->meta->track);
    printf ("  Comment:\t\t%s\n", mp->meta->comment);
    printf ("\n");
  }

  if (mp->img)
  {
    printf ("Image Properties:\n");
    printf ("  Codec:\t\t%s\n", mp->img->codec);
    printf ("  Width:\t\t%d pixels\n", mp->img->width);
    printf ("  Height:\t\t%d pixels\n", mp->img->height);
    printf ("  Depth:\t\t%d bits\n", mp->img->depth);
  }

  if (mp->nr_audio_streams > 0)
  {
    for (i = 0; i < mp->nr_audio_streams; i++)
    {
      media_profile_audio_t *a = mp->audio[i];

      if (!a)
        continue;

      printf ("Audio Stream #%d Properties:\n", i);
      printf ("  Codec:\t\t%s\n", a->codec);
      printf ("  Length:\t\t%d seconds\n", a->length);
      printf ("  Bitrate:\t\t%d bps\n", a->bitrate);
      printf ("  Bits:\t\t\t%d bits\n", a->bits);
      printf ("  Channels:\t\t%d ch\n", a->channels);
      printf ("  Samplerate:\t\t%d Hz\n", a->samplerate);
      printf ("  VBR:\t\t\t%s\n", a->vbr ? "Yes" : "No");
      printf ("\n");
    }
  }

  media_profile_free (mp);

  return 0;
}
