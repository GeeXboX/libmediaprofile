/*
 * The Free Lossless Audio Format (FLAC)
 *  - http://flac.sourceforge.net/ogg_mapping.html
 *  - http://flac.sourceforge.net/documentation_format_overview.html
 *  - http://flac.sourceforge.net/format.html
 */

#include <string.h>

#include "mediaprofile_internals.h"
#include "ogg.h"

#define FLAC_METADATA_TYPE_STREAMINFO 0x7F
#define FLAC_STREAMINFO_SIZE          34
#define FLAC_VERSION                  1

static int
ogg_parse_flac (media_profile_t *p, mp_stream_t *s,
                media_profile_verbosity_level_t v)
{
  media_profile_audio_t *audio;
  uint8_t type, version;
  uint32_t header_size, tmp;
  int base, rate, channels, bps;

  MP_LOG_DEBUG (v, "Parsing FLAC Packet");

  /* rewind to start of header */
  base = mp_stream_get_pos (s);
  mp_stream_seek (s, base - 5);

  type = mp_stream_get_u8 (s) & 0x7F;
  MP_LOG_DEBUG (v, "Block Type: %d", type);
  if (type != FLAC_METADATA_TYPE_STREAMINFO)
    return MP_PARSER_ERROR;

  mp_stream_skip (s, 4);
  version = mp_stream_get_u8 (s);
  if (version != FLAC_VERSION)
    return MP_PARSER_ERROR;

  mp_stream_skip (s, 1);
  mp_stream_skip (s, 2);
  mp_stream_skip (s, 4); // fLaC

  /* Metadata Block Header */
  header_size = mp_stream_get_be32 (s);
  MP_LOG_DEBUG (v, "Header Size: %d", header_size);
  if (header_size != FLAC_STREAMINFO_SIZE)
    return MP_PARSER_ERROR;

  /* skip min/max blocksize and min/max framesize */
  mp_stream_skip (s, 10);

  tmp      = mp_stream_get_be32 (s);
  rate     = tmp >> 12;
  channels = ((tmp << 20) >> 29) + 1;
  bps      = ((tmp << 23) >> 27) + 1;

  /*  skip samples */
  mp_stream_skip (s, 4);

  /*  skip checksum */
  mp_stream_skip (s, 16);

  audio             = mp_audio_new ();
  audio->codec      = strdup ("FLAC");
  audio->length     = 0;
  audio->bitrate    = 0;
  audio->bits       = bps;
  audio->channels   = channels;
  audio->samplerate = rate;
  audio->vbr        = 0;

  /* assign audio stream properties */
  mp_add_audio_stream (p, audio);

  return MP_PARSER_OK;
}

const ogg_stream_t ogg_stream_flac = {
  .name = "FLAC",
  .signature = (const int8_t *) "\177FLAC",
  .signature_size = 5,
  .header_size = 0,
  .parse = ogg_parse_flac,
};
