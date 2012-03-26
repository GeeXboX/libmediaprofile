#ifndef MEDIA_PROFILE_READER_H
#define MEDIA_PROFILE_READER_H

#define MP_FOURCC(a,b,c,d) \
  ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))

#define MP_RD_BE16(x)                                \
  (((uint16_t)((const uint8_t*)(x))[0] << 8) |       \
              ((const uint8_t*)(x))[1])

#define MP_RD_LE16(x)                                \
  (((uint16_t)((const uint8_t*)(x))[1] << 8) |       \
              ((const uint8_t*)(x))[0])

#define MP_RD_BE24(x)                                \
            ((((const uint8_t*)(x))[0] << 16) |      \
             (((const uint8_t*)(x))[1] <<  8) |      \
              ((const uint8_t*)(x))[2])

#define MP_RD_LE24(x)                                \
            ((((const uint8_t*)(x))[2] << 16) |      \
             (((const uint8_t*)(x))[1] <<  8) |      \
              ((const uint8_t*)(x))[0])

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

#define MP_RD_BE64(x)                                \
  (((uint64_t)((const uint8_t*)(x))[0] << 56) |      \
   ((uint64_t)((const uint8_t*)(x))[1] << 48) |      \
   ((uint64_t)((const uint8_t*)(x))[2] << 40) |      \
   ((uint64_t)((const uint8_t*)(x))[3] << 32) |      \
   ((uint64_t)((const uint8_t*)(x))[4] << 24) |      \
   ((uint64_t)((const uint8_t*)(x))[5] << 16) |      \
   ((uint64_t)((const uint8_t*)(x))[6] <<  8) |      \
    (uint64_t)((const uint8_t*)(x))[7])

#define MP_RD_LE64(x)                                \
  (((uint64_t)((const uint8_t*)(x))[7] << 56) |      \
   ((uint64_t)((const uint8_t*)(x))[6] << 48) |      \
   ((uint64_t)((const uint8_t*)(x))[5] << 40) |      \
   ((uint64_t)((const uint8_t*)(x))[4] << 32) |      \
   ((uint64_t)((const uint8_t*)(x))[3] << 24) |      \
   ((uint64_t)((const uint8_t*)(x))[2] << 16) |      \
   ((uint64_t)((const uint8_t*)(x))[1] <<  8) |      \
    (uint64_t)((const uint8_t*)(x))[0])

typedef struct mp_stream_s {
  FILE *f;
} mp_stream_t;

mp_stream_t *mp_stream_open (const char *filename);
void mp_stream_seek (mp_stream_t *s, int len);
void mp_stream_skip (mp_stream_t *s, int len);
int mp_stream_reached_eof (mp_stream_t *s);
int mp_stream_get_pos (mp_stream_t *s);
uint8_t *mp_stream_get_buffer (mp_stream_t *s, int len);
uint8_t mp_stream_get_u8 (mp_stream_t *s);

uint16_t mp_stream_get_le16 (mp_stream_t *s);
uint16_t mp_stream_get_be16 (mp_stream_t *s);

uint32_t mp_stream_get_le24 (mp_stream_t *s);
uint32_t mp_stream_get_be24 (mp_stream_t *s);

uint32_t mp_stream_get_le32 (mp_stream_t *s);
uint32_t mp_stream_get_be32 (mp_stream_t *s);

uint64_t mp_stream_get_le64 (mp_stream_t *s);
uint64_t mp_stream_get_be64 (mp_stream_t *s);

void mp_stream_close (mp_stream_t *s);

#endif /* MEDIA_PROFILE_READER_H */
