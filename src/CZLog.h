#ifndef _CZLOG_H_
#define _CZLOG_H_

#include <cstdio>

/// LOG
#define LOG_LEVEL_CUR	 LOG_LEVEL_DEBUG

#define LOG_LEVEL_ALL    4
#define LOG_LEVEL_DEBUG  3
#define LOG_LEVEL_INFO   2
#define LOG_LEVEL_WARN   1
#define LOG_LEVEL_ERROR  0

#define LOG(fmt, ...) printf(fmt, __VA_ARGS__)

#define LOG_ERROR(fmt, ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_ERROR) LOG("[ERROR]%s at:%d(%s):" fmt ,__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);} while (0)
#define LOG_WARN(fmt,  ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_WARN)  LOG("[WARN](%s):" fmt ,  __FUNCTION__,  __VA_ARGS__);} while (0)
#define LOG_INFO(fmt,  ...)	do { if (LOG_LEVEL_CUR >= LOG_LEVEL_INFO)  LOG("[INFO](%s):" fmt ,  __FUNCTION__,  __VA_ARGS__);} while (0)
#define LOG_DEBUG(fmt, ...) do { if (LOG_LEVEL_CUR >= LOG_LEVEL_DEBUG) LOG("[DEBUG](%s):" fmt,  __FUNCTION__ , __VA_ARGS__);} while (0)

#endif