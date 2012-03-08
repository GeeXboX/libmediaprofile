#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mediaprofile_internals.h"

media_profile_t *
media_profile_guess (const char *filename)
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

  return mp;
}

void
media_profile_free (media_profile_t *media)
{
  mp_free (media);
}
