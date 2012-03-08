#include <stdio.h>

#include "mediaprofile.h"

int
main (int argc, char **argv)
{
  media_profile_t *mp;

  if (argc != 2)
  {
    printf ("Usage: %s filename\n", argv[0]);
    return 1;
  }

  mp = media_profile_guess (argv[1]);
  if (!mp)
    return 1;

  printf ("Media Information:\n");
  printf ("  Filename:\t\t%s\n", argv[1]);
  printf ("  Size:\t\t\t%jd bytes\n", mp->size);

  media_profile_free (mp);

  return 0;
}
