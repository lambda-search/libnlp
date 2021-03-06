
/****************************************************************
 * Copyright (c) 2022, liyinbin
 * All rights reserved.
 * Author by liyibin (jeff.li)
 *
 *****************************************************************/
#pragma once

#include <sstream>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#ifdef XLOG
#error "XLOG has been defined already"
#endif // XLOG
#ifdef XCHECK
#error "XCHECK has been defined already"
#endif // XCHECK

#define XLOG(level) libnlp::logger(libnlp::LL_##level, __FILE__, __LINE__).Stream()
#define XCHECK(exp) if(!(exp)) XLOG(FATAL) << "exp: ["#exp << "] false. "

namespace libnlp {

enum {
  LL_DEBUG = 0, 
  LL_INFO = 1, 
  LL_WARNING = 2, 
  LL_ERROR = 3, 
  LL_FATAL = 4,
}; // enum

static const char * LOG_LEVEL_ARRAY[] = {"DEBUG","INFO","WARN","ERROR","FATAL"};
static const char * LOG_TIME_FORMAT = "%Y-%m-%d %H:%M:%S";

class logger {
 public:
  logger(size_t level, const char* filename, int lineno)
   : level_(level) {
#ifdef LOGGING_LEVEL
     if (level_ < LOGGING_LEVEL) {
       return;
     }
#endif
    assert(level_ <= sizeof(LOG_LEVEL_ARRAY)/sizeof(*LOG_LEVEL_ARRAY));
    char buf[32];
    time_t now;
    time(&now);
    strftime(buf, sizeof(buf), LOG_TIME_FORMAT, localtime(&now));
    stream_ << buf 
      << " " << filename 
      << ":" << lineno 
      << " " << LOG_LEVEL_ARRAY[level_] 
      << " ";
  }
  ~logger() {
#ifdef LOGGING_LEVEL
     if (level_ < LOGGING_LEVEL) {
       return;
     }
#endif
    std::cerr << stream_.str() << std::endl;
    if (level_ == LL_FATAL) {
      abort();
    }
  }

  std::ostream& Stream() {
    return stream_;
  }

 private:
  std::ostringstream stream_;
  size_t level_;
}; // class logger

} // namespace libnlp
