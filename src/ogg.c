/*
 * The Ogg Encapsulation Format Version 0:
 *  - http://www.ietf.org/rfc/rfc3533.txt
 */

#include <string.h>

#include "mediaprofile_internals.h"
#include "ogg.h"

static const unsigned char ogg_signature[5] = {
  0x4F, 0x67, 0x67, 0x53 // OggS
};

extern ogg_stream_t ogg_stream_vorbis;
extern ogg_stream_t ogg_stream_flac;

static ogg_stream_t *ogg_streams[] = {
  &ogg_stream_vorbis,
  &ogg_stream_flac,
  NULL,
};

static int
ogg_parser (media_profile_t *p, mp_stream_t *s,
            media_profile_verbosity_level_t v)
{
  uint8_t *sig, *ssig;
  uint8_t i;
  ogg_stream_t **st;

  MP_LOG_INFO (v, "Parsing OGG file ...");

  while (!mp_stream_reached_eof (s))
  {
    uint8_t version, header_type, segments;
    uint64_t granule_position;
    uint32_t sn, sqn, cksum;
    int header_size, page_size, page_data_size;

    /* check Ogg signature */
    sig = mp_stream_get_buffer (s, 4);
    if (memcmp (sig, ogg_signature, 4) != 0)
    {
      MP_FREE (sig);
      continue;
    }

    MP_FREE (sig);

    version          = mp_stream_get_u8   (s);

    if (version != 0)
      continue;

    header_type      = mp_stream_get_u8   (s);
    granule_position = mp_stream_get_le64 (s);
    sn               = mp_stream_get_le32 (s);
    sqn              = mp_stream_get_le32 (s);
    cksum            = mp_stream_get_le32 (s);
    segments         = mp_stream_get_u8   (s);

    header_size      = segments + 27;
    page_size        = header_size;
    for (i = 0; i < segments; i++)
      page_size     += mp_stream_get_u8   (s);
    page_data_size   = page_size - header_size;

    MP_LOG_DEBUG (v, "Version            = %u",    version);
    MP_LOG_DEBUG (v, "Header Type        = %u",    header_type);
    MP_LOG_DEBUG (v, "Granule Position   = %lu",   granule_position);
    MP_LOG_DEBUG (v, "Serial Number      = %u",    sn);
    MP_LOG_DEBUG (v, "Sequence Number    = %u",    sqn);
    MP_LOG_DEBUG (v, "Checksum           = %u",    cksum);
    MP_LOG_DEBUG (v, "Number of Segments = %u",    segments);

    MP_LOG_DEBUG (v, "Header Size        = 0x%x",  header_size);
    MP_LOG_DEBUG (v, "Page Size          = 0x%x",  page_size);
    MP_LOG_DEBUG (v, "Page Data Size     = 0x%x",  page_data_size);

    st = ogg_streams;
    while (*st)
    {
      int pos, err;

      /* save stream position */
      pos = mp_stream_get_pos (s);

      ssig = mp_stream_get_buffer (s, (*st)->signature_size);
      if (memcmp (ssig, (*st)->signature, (*st)->signature_size) != 0)
        goto next;

      /* check for correct stream header size */
      if ((*st)->header_size && (*st)->header_size != page_data_size)
        goto next;

      MP_LOG_DEBUG (v, "Found Ogg Stream: %s", (*st)->name);
      err = (*st)->parse (p, s, v);
      if (err == MP_PARSER_OK)
      {
        MP_FREE (ssig);
        mp_stream_seek (s, pos + (*st)->header_size);
        break;
      }

    next:
      MP_FREE (ssig);
      mp_stream_seek (s, pos);
      (void) *st++;
    }

    MP_LOG_DEBUG (v, "");
  }

  return MP_PARSER_OK;
}

mp_parser_t mp_parser_ogg = {
  .name       = "Ogg Audio",
  .type       = MEDIA_PROFILE_TYPE_AUDIO,
  .extensions = "ogg",
  .parse      = ogg_parser,
};
