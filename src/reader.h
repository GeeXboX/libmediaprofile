#ifndef MEDIA_PROFILE_READER_H
#define MEDIA_PROFILE_READER_H

#define MP_FOURCC(a,b,c,d) \
  ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))

#define MP_RD_BE32(x)                                \
  (((uint32_t)((const uint8_t*)(x))[0] << 24) |      \
             (((const uint8_t*)(x))[1] << 16) |      \
             (((const uint8_t*)(x))[2] <<  8) |      \
              ((const uint8_t*)(x))[3])

#define MP_RD_LE32(x)                                \
  (((uint32_t)((const uint8_t*)(x))[3] << 24) |      \
             (((const uint8_t*)(x))[2] << 16) |      \
             (((const uint8_t*)(x))[1] <<  8) |      \
              ((const uint8_t*)(x))[0])

typedef struct mp_stream_s {
  FILE *f;
} mp_stream_t;

mp_stream_t *mp_stream_open (const char *filename);
void mp_stream_skip (mp_stream_t *s, int len);
uint8_t *mp_stream_get_buffer (mp_stream_t *s, int len);
uint8_t mp_stream_get_u8 (mp_stream_t *s);

uint32_t mp_stream_get_le32 (mp_stream_t *s);
uint32_t mp_stream_get_be32 (mp_stream_t *s);

void mp_stream_close (mp_stream_t *s);

#endif /* MEDIA_PROFILE_READER_H */
