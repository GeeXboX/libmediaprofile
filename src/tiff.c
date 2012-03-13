/*
 * TIFF Format Specifications:
 *  - http://www.alternatiff.com/resources/TIFF6.pdf
 *  - http://www.awaresystems.be/imaging/tiff/faq.html
 *  - http://www.digitalpreservation.gov/formats/content/tiff_tags.shtml
 */

#include <string.h>

#include "mediaprofile_internals.h"

#define TIFF_LE             0x4949
#define TIFF_BE             0x4D4D
#define TIFF_MAGIC_NUMBER   42

#define TIFF_TAG_WIDTH      0x100
#define TIFF_TAG_HEIGHT     0x101
#define TIFF_TAG_BPP        0x102

typedef enum {
  TIFF_TYPE_BYTE = 1,
  TIFF_TYPE_STRING,
  TIFF_TYPE_SHORT,
  TIFF_TYPE_LONG,
  TIFF_TYPE_RATIONAL,
} tiff_type_t;

static uint16_t
tiff_get_u16 (mp_stream_t *s, int le)
{
  return le ? mp_stream_get_le16 (s) : mp_stream_get_be16 (s);
}

static uint32_t
tiff_get_u32 (mp_stream_t *s, int le)
{
  return le ? mp_stream_get_le32 (s) : mp_stream_get_be32 (s);
}

static uint32_t
tiff_get_value_by_type (mp_stream_t *s, int le, uint16_t type, uint32_t off)
{
  uint32_t value;

  switch (type)
  {
  case TIFF_TYPE_BYTE:
    mp_stream_skip (s, -4);
    value = mp_stream_get_u8 (s);
    mp_stream_skip (s, 3);
    break;
  case TIFF_TYPE_SHORT:
    mp_stream_skip (s, -4);
    value = tiff_get_u16 (s, le);
    mp_stream_skip (s, 2);
    break;
  case TIFF_TYPE_LONG:
    value = off;
    break;
  default:
    mp_stream_skip (s, 4);
    break;
  }

  return value;
}

static uint32_t
tiff_get_bpp_by_type (mp_stream_t *s, int le,
                      uint16_t type, uint32_t count, uint32_t off)
{
  uint32_t value = 0;
  int i;

  switch (type)
  {
  case TIFF_TYPE_BYTE:
    value = (off & 0xFF) + ((off >> 8) & 0xFF) +
      ((off >> 16) & 0xFF) + ((off >> 24) & 0xFF);
    break;
  case TIFF_TYPE_SHORT:
    for (i = 0; i < count; i++)
      value += tiff_get_u16 (s, le);
    break;
  case TIFF_TYPE_LONG:
    for (i = 0; i < count; i++)
      value += tiff_get_u32 (s, le);
    break;
  default:
    break;
  }

  return value;
}

static int
tiff_parser (media_profile_t *p, mp_stream_t *s,
             media_profile_verbosity_level_t v)
{
  media_profile_image_t *img;
  int width, height, depth;
  int le; /* data endianness */
  uint16_t sig, ifd_tags;
  uint32_t offset;
  int i;

  MP_LOG_INFO (v, "Parsing TIFF file ...");

  /* check TIFF signature */
  sig = mp_stream_get_le16 (s);
  if (sig == TIFF_LE)
    le = 1;
  else if (sig == TIFF_BE)
    le = 0;
  else
    return MP_PARSER_ERROR;

  sig = tiff_get_u16 (s, le);
  if (sig != TIFF_MAGIC_NUMBER)
    return MP_PARSER_ERROR;

  offset = tiff_get_u32 (s, le);
  mp_stream_seek (s, offset);

  /* find number of IFD tags */
  ifd_tags = tiff_get_u16 (s, le);
  MP_LOG_DEBUG (v, "Offset: %d - Entries: %d", offset, ifd_tags);

  for (i = 0; i < ifd_tags; i++)
  {
    uint16_t tag, type;
    uint32_t count, off;
    uint32_t value = 0;

    tag   = tiff_get_u16 (s, le);
    type  = tiff_get_u16 (s, le);
    count = tiff_get_u32 (s, le);
    off   = tiff_get_u32 (s, le);

    MP_LOG_DEBUG (v, "Tag: %x - Type: %d - Count: %d - Off: %d",
                  tag, type, count, off);

    if (count == 1)
      value = tiff_get_value_by_type (s, le, type, off);
    else
      mp_stream_skip (s, -4);

    switch (tag)
    {
    case TIFF_TAG_WIDTH:
      width = value;
      break;
    case TIFF_TAG_HEIGHT:
      height = value;
      break;
    case TIFF_TAG_BPP:
      if (count == 1)
        depth = value;
      else
        depth = tiff_get_bpp_by_type (s, le, type, count, off);
      break;
    }
  }

  img = mp_image_new ();
  img->codec  = strdup ("TIFF");
  img->width  = width;
  img->height = height;
  img->depth  = depth;

  /* assing image properties */
  p->img = img;

  return MP_PARSER_OK;
}

mp_parser_t mp_parser_tiff = {
  .name       = "TIFF",
  .type       = MEDIA_PROFILE_TYPE_IMAGE,
  .extensions = "tiff,tif",
  .parse      = tiff_parser,
};
