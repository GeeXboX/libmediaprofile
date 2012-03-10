#include <stdio.h>

#include "mediaprofile.h"

int
main (int argc, char **argv)
{
  media_profile_t *mp;
  const char *media_type;

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

  if (mp->img)
  {
    printf ("Image Properties:\n");
    printf ("  Codec:\t\t%s\n", mp->img->codec);
    printf ("  Width:\t\t%d pixels\n", mp->img->width);
    printf ("  Height:\t\t%d pixels\n", mp->img->height);
    printf ("  Depth:\t\t%d bits\n", mp->img->depth);
  }

  media_profile_free (mp);

  return 0;
}
