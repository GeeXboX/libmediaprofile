#ifndef MEDIA_PROFILE_LOGS_H
#define MEDIA_PROFILE_LOGS_H

void mp_log (media_profile_verbosity_level_t max,
             media_profile_verbosity_level_t level, const char *format, ...);

#define MP_LOG_ERROR(max, format, arg...)                       \
    mp_log (max, MEDIA_PROFILE_MSG_ERROR, format, ##arg)

#define MP_LOG_INFO(max, format, arg... )                       \
    mp_log (max, MEDIA_PROFILE_MSG_INFO,  format, ##arg)

#define MP_LOG_DEBUG(max, format, arg...)                       \
    mp_log (max, MEDIA_PROFILE_MSG_DEBUG, format, ##arg)

#endif /* MEDIA_PROFILE_LOGS_H */
