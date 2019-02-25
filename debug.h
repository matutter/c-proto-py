#ifndef DEBUG_H
#define DEBUG_H

#define KRST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"
#define KBOLD "\033[1m"
#define KDIM "\e[2m"

#define KSEP KRST KDIM " - " KRST

#if defined(DEBUG)

#include <stdio.h>
#include <errno.h>
#include <string.h>
#define debug(fmt, args...) \
  dbg(KBOLD "%s:%d" KSEP fmt KRST "\n", __func__, __LINE__, ##args);

#define dbg(fmt, args...) \
  printf(fmt KRST, ##args)

//; fflush(NULL)

#else

#define debug(fmt, args...)

#endif

#define debug_where(fmt, args...) \
  debug(KMAG fmt, ##args)

#define debug_info(fmt, args...) \
  debug(KCYN fmt, ##args)

#define debug_danger(fmt, args...) \
  debug(KRED fmt, ##args)

#define debug_warning(fmt, args...) \
  debug(KYEL fmt, ##args)

#define debug_success(fmt, args...) \
  debug(KGRN fmt, ##args)

/// short

#define dbg_where(fmt, args...) \
  dbg(KMAG fmt, ##args)

#define dbg_info(fmt, args...) \
  dbg(KCYN fmt, ##args)

#define dbg_danger(fmt, args...) \
  dbg(KRED fmt, ##args)

#define dbg_warning(fmt, args...) \
  dbg(KYEL fmt, ##args)

#define dbg_success(fmt, args...) \
  dbg(KGRN fmt, ##args)

#define debug_errno(fmt, args...) \
  dbg(KMAG "(%d) %s " fmt "\n", errno, strerror(errno), ##args)

#define dbg_token() \
  dbg("%s", yytext)

#endif