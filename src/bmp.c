#include <string.h>

#include "mediaprofile_internals.h"

#define HEADER_SIZE_WINDIB_V3 40
#define HEADER_SIZE_WINDIB_V4 108
#define HEADER_SIZE_WINDIB_V5 124

#define HEADER_SIZE_OS2_V1 12
#define HEADER_SIZE_OS2_V2 64

static int
bmp_parser (media_profile_t *p, mp_stream_t *s)
{
  media_profile_image_t *img;
  unsigned int hsize, ihsize, depth;
  int width, height;

  printf ("Parsing BMP file ...\n");

  /* check BMP signature */
  if (mp_stream_get_u8 (s) != 'B' || mp_stream_get_u8 (s) != 'M')
    return MP_PARSER_ERROR;

  mp_stream_skip (s, 8);

  hsize  = mp_stream_get_le32 (s);
  ihsize = mp_stream_get_le32 (s);
  if (ihsize + 14 > hsize)
    return MP_PARSER_ERROR;

  switch (ihsize)
  {
  case HEADER_SIZE_OS2_V1:
    width  = mp_stream_get_le16 (s);
    height = mp_stream_get_le16 (s);
    break;
  case HEADER_SIZE_WINDIB_V3:
  case HEADER_SIZE_OS2_V2:
  case HEADER_SIZE_WINDIB_V4:
  case HEADER_SIZE_WINDIB_V5:
    width  = mp_stream_get_le32 (s);
    height = mp_stream_get_le32 (s);
    break;
  default:
    break;
  }

  if (mp_stream_get_le16 (s) != 1)
    return MP_PARSER_ERROR;

  depth = mp_stream_get_le16 (s);

  img = mp_image_new ();
  img->codec  = strdup ("BMP");
  img->width  = width;
  img->height = height;
  img->depth  = depth;

  /* assing image properties */
  p->img = img;

  return MP_PARSER_OK;
}

mp_parser_t mp_parser_bmp = {
  .name       = "Bitmap",
  .type       = MEDIA_PROFILE_TYPE_IMAGE,
  .extensions = "bmp",
  .parse      = bmp_parser,
};
