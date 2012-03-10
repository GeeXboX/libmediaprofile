#include <string.h>

#include "mediaprofile_internals.h"

#define COLOR_MASK_PALETTE    1
#define COLOR_MASK_COLOR      2
#define COLOR_MASK_ALPHA      4

#define COLOR_TYPE_GRAY       0
#define COLOR_TYPE_PALETTE    (COLOR_MASK_COLOR | COLOR_MASK_PALETTE)
#define COLOR_TYPE_RGB        (COLOR_MASK_COLOR)
#define COLOR_TYPE_RGB_ALPHA  (COLOR_MASK_COLOR | COLOR_MASK_ALPHA)
#define COLOR_TYPE_GRAY_ALPHA (COLOR_MASK_ALPHA)

static const uint8_t png_signature[8] = {
  0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A
};

static int
png_parser (media_profile_t *p, mp_stream_t *s)
{
  media_profile_image_t *img;
  uint32_t tag, length,w, h;
  uint8_t depth, color_type;
  uint8_t *sig;

  printf ("Parsing PNG file ...\n");

  /* check PNG signature */
  sig = mp_stream_get_buffer (s, 8);
  if (memcmp (sig, png_signature, 8) != 0)
  {
    MP_FREE (sig);
    return MP_PARSER_ERROR;
  }

  MP_FREE (sig);
  length = mp_stream_get_be32 (s);
  if (length != 13)
    return MP_PARSER_ERROR;

  tag = mp_stream_get_le32 (s);

  printf ("png: tag=%c%c%c%c length=%u\n",
          (tag & 0xff),
          ((tag >> 8) & 0xff),
          ((tag >> 16) & 0xff),
          ((tag >> 24) & 0xff), length);

  if (tag != MP_FOURCC ('I', 'H', 'D', 'R'))
    return MP_PARSER_ERROR;

  w          = mp_stream_get_be32 (s);
  h          = mp_stream_get_be32 (s);
  depth      = mp_stream_get_u8   (s);
  color_type = mp_stream_get_u8   (s);

  printf ("width: %d - height: %d\n", w, h);
  printf ("depth: %d - color type: %d\n", depth, color_type);

  img = mp_image_new ();
  img->codec  = strdup ("PNG");
  img->width  = w;
  img->height = h;

  switch (color_type)
  {
  case COLOR_TYPE_GRAY:
    img->depth = depth;
    break;
  case COLOR_TYPE_PALETTE:
    img->depth = depth;
    break;
  case COLOR_TYPE_GRAY_ALPHA:
    img->depth = 2 * depth;
    break;
  case COLOR_TYPE_RGB:
    img->depth = 3 * depth;
    break;
  case COLOR_TYPE_RGB_ALPHA:
    img->depth = 4 * depth;
    break;
  default:
    break;
  }

  /* assing image properties */
  p->img = img;

  return MP_PARSER_OK;
}

mp_parser_t mp_parser_png = {
  .name       = "PNG",
  .type       = MEDIA_PROFILE_TYPE_IMAGE,
  .extensions = "png",
  .parse      = png_parser,
};
