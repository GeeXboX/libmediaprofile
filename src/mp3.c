/*
 * MPEG Audio Format Specifications:
 *  - http://www.codeproject.com/Articles/8295/MPEG-Audio-Frame-Header
 *  - http://mpgedit.org/mpgedit/mpeg_format/MP3Format.html
 */

#include <string.h>

#include "mediaprofile_internals.h"
#include "id3v1.h"

#define MPEG_MARKER 0xFFF00000

static int
mpeg_audio_header_sync_check (uint32_t header)
{
  /* header */
  if ((header & 0xffe00000) != 0xffe00000)
    return 0;

  /* layer check */
  if ((header & (3 << 17)) == 0)
    return 0;

  /* bit rate */
  if ((header & (0xF << 12)) == 0xF << 12)
    return 0;

  /* frequency */
  if ((header & (3 << 10)) == 3 << 10)
    return 0;

  return 1;
}

static const uint16_t
mpeg_audio_bitrate_table[8][16] = {
  /* reserved */
  { 0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 },
  /* MPEG-2, 2.5 (LSF) Layer III */
  { 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0 },
  /* MPEG-2, 2.5 (LSF) Layer II */
  { 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0 },
  /* MPEG-2, 2.5 (LSF) Layer I */
  { 0, 32, 48, 56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, 0 },
  /* reserved */
  { 0,  0,  0,  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 },
  /* MPEG-1 Layer III (MP3) */
  { 0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 0 },
  /* MPEG-1 Layer II */
  { 0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, 0 },
  /* MPEG-1 Layer I */
  { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0 }
};

static const uint16_t
mpeg_audio_frequency_table[4][2] = {
  { 22050, 44100 },
  { 24000, 48000 },
  { 16000, 32000 },
  {     0,     0 }
};

static int
mp3_parser (media_profile_t *p, mp_stream_t *s,
            media_profile_verbosity_level_t v)
{
  media_profile_audio_t *audio;
  int frames = 0, bitrate = 0, frequency = 0, channels = 0, vbr = 0;
  int current_btr, bps;

  MP_LOG_INFO (v, "Parsing MP3 file ...");

  while (!mp_stream_reached_eof (s))
  {
    uint32_t header;

    /* read next MPEG Audio marker */
    header = mp_stream_get_be32 (s);
    while (((header & MPEG_MARKER) != MPEG_MARKER)
           && !mp_stream_reached_eof (s))
    {
      mp_stream_skip (s, -3);
      header = mp_stream_get_be32 (s);
    }

    if ((header & MPEG_MARKER) == MPEG_MARKER)
    {
      int vs, btr, frame_size;

      /* ensure we hit a valid MPEG Audio header sync mark */
      if (!mpeg_audio_header_sync_check (header))
        break;

      /* MPEG Audio version */
      vs = (header & 0xE0000) >> 17;

      /* Audio Frequency */
      frequency = mpeg_audio_frequency_table[(header & 0xC00) >> 10][vs >> 2];

      /* Audio Bitrate */
      btr = mpeg_audio_bitrate_table[vs][(header & 0xF000) >> 12] * 1000;

      /* check for variable bitrate */
      if (current_btr != 0 && btr != current_btr)
        vbr = 1;
      current_btr = btr;
      bitrate += btr;

      /* Audio Channels */
      channels = (header & 0xC0) == 0xC0 ? 1 : 2;

      /* go to next frame */
      frame_size = 144 * btr / frequency + ((header & 0x200) ? 1 : 0) - 4;
      mp_stream_skip (s, frame_size);

      frames++;
    }
  }

  bps = (bitrate && frames) ? (bitrate / frames / 8) : 1;

  audio             = mp_audio_new ();
  audio->codec      = strdup ("MP3");
  audio->length     = p->size / bps;
  audio->bitrate    = (bitrate && frames) ? bitrate / frames : 0;
  audio->channels   = channels;
  audio->samplerate = frequency;
  audio->vbr        = vbr;

  /* assign audio stream properties */
  mp_add_audio_stream (p, audio);

  /* check for ID3 tags */
  p->meta = id3v1_parse (p, s);

  return MP_PARSER_OK;
}

mp_parser_t mp_parser_mp3 = {
  .name       = "MPEG Audio",
  .type       = MEDIA_PROFILE_TYPE_AUDIO,
  .extensions = "mpa,mp2,mp3,m2a",
  .parse      = mp3_parser,
};
