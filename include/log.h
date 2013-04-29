/**
   @file   log.h
   @brief  logger


   @author Faizal Zakaria
   @date   2012-07-03
*/
#ifndef __LOG_H__
#define __LOG_H__

#define LOGE_LEVEL (1)
#define LOGW_LEVEL (1 << 1)
#define LOGI_LEVEL (1 << 2)
#define LOGD_LEVEL (1 << 3)

#define LOG_LEVEL 0xf

#ifndef LOGD
#if LOG_LEVEL & LOGD_LEVEL
#define LOGD(format, text...)					\
	fprintf(stdout, format, ##text);
#else
#define LOGD(format, text...)
#endif
#endif

#ifndef LOGI
#if LOG_LEVEL & LOGI_LEVEL
#define LOGI(format, text...)                                           \
	fprintf(stdout, format, ##text);
#else
#define LOGI(format, text...)
#endif
#endif

#ifndef LOGW
#if LOG_LEVEL & LOGW_LEVEL
#define LOGW(format, text...)										\
	fprintf(stdout, format, ##text);
#else
#define LOGW(format, text...)
#endif
#endif

#ifndef LOGE
#if LOG_LEVEL & LOGE_LEVEL
#define LOGE(format, text...)									\
	fprintf(stderr, format, ##text);
#else
#define LOGE(format, text...)
#endif
#endif

#endif // __LOG_H__
