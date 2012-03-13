#include <string.h>

#include "mediaprofile_internals.h"

#define JPG_MARKER 0xFF

typedef enum {
    SEGMENT_TEM   = 0x01,

    SEGMENT_SOF0  = 0xC0,
    /* ... */
    SEGMENT_SOF3  = 0xC3,
    SEGMENT_DHT   = 0xC4,
    SEGMENT_SOF5  = 0xC5,
    /* ... */
    SEGMENT_SOF7  = 0xC7,
    SEGMENT_JPG   = 0xC8,
    SEGMENT_SOF9  = 0xC9,
    /* ... */
    SEGMENT_SOF15 = 0xCF,

    SEGMENT_SOI   = 0xD8,
    SEGMENT_EOI   = 0xD9,

    SEGMENT_APP0  = 0xE0,
    /* ... */
    SEGMENT_APP15 = 0xEF,

    SEGMENT_COM   = 0xFE,
} jpeg_segment_t; /* segment type */

static int
jpg_parser (media_profile_t *p, mp_stream_t *s,
            media_profile_verbosity_level_t v)
{
  media_profile_image_t *img;
  int width, height, depth;

  MP_LOG_INFO (v, "Parsing JPEG file ...");

  /* check JPEG signature */
  if (mp_stream_get_u8 (s) != JPG_MARKER ||
      mp_stream_get_u8 (s) != SEGMENT_SOI)
    return MP_PARSER_ERROR;

  for (;;)
  {
    uint8_t segment_type;
    uint16_t len;

    if (mp_stream_get_u8 (s) != JPG_MARKER)
      break;

    segment_type = mp_stream_get_u8 (s);
    if (segment_type < SEGMENT_TEM || segment_type > SEGMENT_COM)
      break;

    MP_LOG_DEBUG (v, "TAG: %.2x", segment_type);

    if (segment_type >= SEGMENT_APP0 && segment_type <= SEGMENT_APP15)
    {
      len = mp_stream_get_be16 (s) - 2;
      /* read some EXIF data some day ? */
      mp_stream_skip (s, len);
      continue;
    }
    else if (segment_type == SEGMENT_COM)
    {
      len = mp_stream_get_be16 (s) - 2;
      /* use Comment as a metadata ? */
      mp_stream_skip (s, len);
      continue;
    }
    else if (segment_type >= SEGMENT_SOF0 && segment_type <= SEGMENT_SOF15 &&
             segment_type != SEGMENT_DHT && segment_type != SEGMENT_JPG)
    {
      uint8_t bits, layers;

      mp_stream_skip (s, 2);
      bits   = mp_stream_get_u8   (s);
      height = mp_stream_get_be16 (s);
      width  = mp_stream_get_be16 (s);
      layers = mp_stream_get_u8   (s);
      depth  = bits * layers;
      break;
    }
    else
    {
      len = mp_stream_get_be16 (s) - 2;
      mp_stream_skip (s, len);
      continue;
    }
  }

  img = mp_image_new ();
  img->codec  = strdup ("JPEG");
  img->width  = width;
  img->height = height;
  img->depth  = depth;

  /* assing image properties */
  p->img = img;

  return MP_PARSER_OK;
}

mp_parser_t mp_parser_jpg = {
  .name       = "JPEG",
  .type       = MEDIA_PROFILE_TYPE_IMAGE,
  .extensions = "jpeg,jpg,jpe,jif,jfi,jfif",
  .parse      = jpg_parser,
};
