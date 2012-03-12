/*
 * ID3v1 Specifications:
 *  - http://www.id3.org/ID3v1
 *  - http://en.wikipedia.org/wiki/ID3
 */

#include <string.h>

#include "mediaprofile_internals.h"
#include "id3v1.h"

#define ID3v1_TAG_SIZE  128
#define ID3v1_GENRE_MAX 147

const char * const
id3v1_genre_table[ID3v1_GENRE_MAX + 1] = {
  [0]   = "Blues",
  [1]   = "Classic Rock",
  [2]   = "Country",
  [3]   = "Dance",
  [4]   = "Disco",
  [5]   = "Funk",
  [6]   = "Grunge",
  [7]   = "Hip-Hop",
  [8]   = "Jazz",
  [9]   = "Metal",
  [10]  = "New Age",
  [11]  = "Oldies",
  [12]  = "Other",
  [13]  = "Pop",
  [14]  = "R&B",
  [15]  = "Rap",
  [16]  = "Reggae",
  [17]  = "Rock",
  [18]  = "Techno",
  [19]  = "Industrial",
  [20]  = "Alternative",
  [21]  = "Ska",
  [22]  = "Death Metal",
  [23]  = "Pranks",
  [24]  = "Soundtrack",
  [25]  = "Euro-Techno",
  [26]  = "Ambient",
  [27]  = "Trip-Hop",
  [28]  = "Vocal",
  [29]  = "Jazz+Funk",
  [30]  = "Fusion",
  [31]  = "Trance",
  [32]  = "Classical",
  [33]  = "Instrumental",
  [34]  = "Acid",
  [35]  = "House",
  [36]  = "Game",
  [37]  = "Sound Clip",
  [38]  = "Gospel",
  [39]  = "Noise",
  [40]  = "AlternRock",
  [41]  = "Bass",
  [42]  = "Soul",
  [43]  = "Punk",
  [44]  = "Space",
  [45]  = "Meditative",
  [46]  = "Instrumental Pop",
  [47]  = "Instrumental Rock",
  [48]  = "Ethnic",
  [49]  = "Gothic",
  [50]  = "Darkwave",
  [51]  = "Techno-Industrial",
  [52]  = "Electronic",
  [53]  = "Pop-Folk",
  [54]  = "Eurodance",
  [55]  = "Dream",
  [56]  = "Southern Rock",
  [57]  = "Comedy",
  [58]  = "Cult",
  [59]  = "Gangsta",
  [60]  = "Top 40",
  [61]  = "Christian Rap",
  [62]  = "Pop/Funk",
  [63]  = "Jungle",
  [64]  = "Native American",
  [65]  = "Cabaret",
  [66]  = "New Wave",
  [67]  = "Psychadelic",
  [68]  = "Rave",
  [69]  = "Showtunes",
  [70]  = "Trailer",
  [71]  = "Lo-Fi",
  [72]  = "Tribal",
  [73]  = "Acid Punk",
  [74]  = "Acid Jazz",
  [75]  = "Polka",
  [76]  = "Retro",
  [77]  = "Musical",
  [78]  = "Rock & Roll",
  [79]  = "Hard Rock",
  [80]  = "Folk",
  [81]  = "Folk-Rock",
  [82]  = "National Folk",
  [83]  = "Swing",
  [84]  = "Fast Fusion",
  [85]  = "Bebob",
  [86]  = "Latin",
  [87]  = "Revival",
  [88]  = "Celtic",
  [89]  = "Bluegrass",
  [90]  = "Avantgarde",
  [91]  = "Gothic Rock",
  [92]  = "Progressive Rock",
  [93]  = "Psychedelic Rock",
  [94]  = "Symphonic Rock",
  [95]  = "Slow Rock",
  [96]  = "Big Band",
  [97]  = "Chorus",
  [98]  = "Easy Listening",
  [99]  = "Acoustic",
  [100] = "Humour",
  [101] = "Speech",
  [102] = "Chanson",
  [103] = "Opera",
  [104] = "Chamber Music",
  [105] = "Sonata",
  [106] = "Symphony",
  [107] = "Booty Bass",
  [108] = "Primus",
  [109] = "Porn Groove",
  [110] = "Satire",
  [111] = "Slow Jam",
  [112] = "Club",
  [113] = "Tango",
  [114] = "Samba",
  [115] = "Folklore",
  [116] = "Ballad",
  [117] = "Power Ballad",
  [118] = "Rhythmic Soul",
  [119] = "Freestyle",
  [120] = "Duet",
  [121] = "Punk Rock",
  [122] = "Drum Solo",
  [123] = "A capella",
  [124] = "Euro-House",
  [125] = "Dance Hall",
  [126] = "Goa",
  [127] = "Drum & Bass",
  [128] = "Club-House",
  [129] = "Hardcore",
  [130] = "Terror",
  [131] = "Indie",
  [132] = "BritPop",
  [133] = "Negerpunk",
  [134] = "Polsk Punk",
  [135] = "Beat",
  [136] = "Christian Gangsta",
  [137] = "Heavy Metal",
  [138] = "Black Metal",
  [139] = "Crossover",
  [140] = "Contemporary Christian",
  [141] = "Christian Rock",
  [142] = "Merengue",
  [143] = "Salsa",
  [144] = "Thrash Metal",
  [145] = "Anime",
  [146] = "JPop",
  [147] = "SynthPop",
};

media_profile_metadata_t *
id3v1_parse (media_profile_t *p, mp_stream_t *s)
{
  media_profile_metadata_t *meta = NULL;
  uint8_t *buf;
  char str[5];
  int genre;

  mp_stream_seek (s, p->size - ID3v1_TAG_SIZE);
  buf = mp_stream_get_buffer (s, ID3v1_TAG_SIZE);
  if (!buf)
    return NULL;

  /* check for ID3v1 tags signature */
  if (!(buf[0] == 'T' && buf[1] == 'A' && buf[2] == 'G'))
    return NULL;

  meta          = mp_metadata_new ();
  meta->title   = strndup ((char *) buf +  3, 30);
  meta->artist  = strndup ((char *) buf + 33, 30);
  meta->album   = strndup ((char *) buf + 63, 30);
  meta->year    = strndup ((char *) buf + 93,  4);
  meta->comment = strndup ((char *) buf + 97, 30);

  if (buf[125] == 0 && buf[126] != 0) {
    snprintf (str, sizeof (str), "%d", buf[126]);
    meta->track = strdup (str);
  }

  genre = buf[127];
  if (genre <= ID3v1_GENRE_MAX)
    meta->genre = strdup (id3v1_genre_table[genre]);

  return meta;
}
