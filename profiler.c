#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define _GNU_SOURCE
#include <getopt.h>

#include "mediaprofile.h"

#define APPNAME "media-profiler"

#define PROFILER_OPTIONS                                \
  APPNAME " for libmediaprofile\n"                      \
  "\n"                                                  \
  "Usage: " APPNAME " [-v verbosity] [-f file]\n"       \
  "\n"                                                  \
  "Options:\n"                                          \
  " -h --help               this help\n"                \
  " -v --verbose <level>    specify verbosity\n"        \
  " -f --file <filename>    specify file\n"             \
  "\n"

int
main (int argc, char **argv)
{
  media_profile_t *mp;
  const char *media_type;
  char *filename = NULL;
  media_profile_verbosity_level_t v = MEDIA_PROFILE_MSG_NONE;
  int i;

  int c, index;
  const char *const short_options = "hv:f:";
  const struct option long_options [] = {
    {"help",    no_argument,       0, 'h' },
    {"verbose", required_argument, 0, 'v' },
    {"file",    required_argument, 0, 'f' },
    {0,         0,                 0,  0  }
  };

  if (argc == 1)
  {
    printf (PROFILER_OPTIONS);
    return 1;
  }

  /* command line argument processing */
  while (1)
  {
    c = getopt_long (argc, argv, short_options, long_options, &index);

    if (c == EOF)
      break;

    switch (c)
    {
    case 0:
      /* opt = long_options[index].name; */
      break;

    case '?':
    case 'h':
      printf (PROFILER_OPTIONS);
      return 0;

    case 'v':
      v = atoi (optarg);
      break;

    case 'f':
      filename = optarg;
      break;

    default:
      printf (PROFILER_OPTIONS);
      return 1;
    }
  }

  mp = media_profile_guess (filename, v);
  if (!mp)
    return 1;

  printf ("\n");
  printf ("Media Information:\n");
  printf ("  Filename:\t\t%s\n", filename);
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
