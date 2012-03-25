#include <string.h>

#include "mediaprofile_internals.h"
#include "ogg.h"

static int
ogg_parse_vorbis (media_profile_t *p, mp_stream_t *s,
                  media_profile_verbosity_level_t v)
{
  media_profile_audio_t *audio;
  int frequency = 0, channels = 0;
  int bps_min, bps_avg, bps_max;
  uint32_t blocksize, bs0, bs1;

  MP_LOG_DEBUG (v, "Parsing Vorbis Packet");

  mp_stream_skip (s, 4); /* version */
  channels  = mp_stream_get_u8   (s);
  frequency = mp_stream_get_le32 (s);
  bps_max   = mp_stream_get_le32 (s);
  bps_avg   = mp_stream_get_le32 (s);
  bps_min   = mp_stream_get_le32 (s);
  blocksize = mp_stream_get_u8 (s);

  bs0 = blocksize & 15;
  bs1 = blocksize >> 4;

  if (bs0 > bs1)
    return MP_PARSER_ERROR;

  if (bs0 < 6 || bs1 > 13)
    return MP_PARSER_ERROR;

  if (mp_stream_get_u8 (s) != 1) /* framing_flag */
    return MP_PARSER_ERROR;

  audio             = mp_audio_new ();
  audio->codec      = strdup ("Vorbis");
  audio->bitrate    = bps_avg;
  audio->channels   = channels;
  audio->samplerate = frequency;
  audio->vbr        = (bps_min != bps_max ) ? 1 : 0;

  /* assign audio stream properties */
  mp_add_audio_stream (p, audio);

  return MP_PARSER_OK;
}

const ogg_stream_t ogg_stream_vorbis = {
  .name = "Vorbis",
  .signature = (const int8_t *) "\001vorbis",
  .signature_size = 7,
  .header_size = 30,
  .parse = ogg_parse_vorbis,
};
