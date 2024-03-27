///
/// \file LMS7002M_logger.c
///
/// Logging facilities for the LMS7002M C driver.
///
/// \copyright
/// Copyright (c) 2015-2015 Fairwaves, Inc.
/// Copyright (c) 2015-2015 Rice University
/// SPDX-License-Identifier: Apache-2.0
/// http://www.apache.org/licenses/LICENSE-2.0
///

//TODO ifdef this for printing on other platforms, ex kprintf

#include <LMS7002M/LMS7002M_logger.h>
#include <linux/kernel.h>
//#include <stdio.h>
//#include <stdlib.h>

/***********************************************************************
 * ANSI terminal colors for default logger
 **********************************************************************/
 #if 0
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[1m"
#else

#define ANSI_COLOR_RED     ""
#define ANSI_COLOR_GREEN   ""
#define ANSI_COLOR_YELLOW  ""
#define ANSI_COLOR_BLUE    ""
#define ANSI_COLOR_MAGENTA ""
#define ANSI_COLOR_CYAN    ""
#define ANSI_COLOR_RESET   ""
#define ANSI_COLOR_BOLD    ""

#endif
/***********************************************************************
 * Default log message handler implementation
 **********************************************************************/
void default_handler(const LMS7_log_level_t level, const char *message)
{
    switch (level)
    {
    case LMS7_FATAL:    printk(ANSI_COLOR_BOLD ANSI_COLOR_RED "[FATAL] %s" ANSI_COLOR_RESET "\n", message); break;
    case LMS7_CRITICAL: printk( ANSI_COLOR_BOLD ANSI_COLOR_RED "[CRITICAL] %s" ANSI_COLOR_RESET "\n", message); break;
    case LMS7_ERROR:    printk( ANSI_COLOR_BOLD ANSI_COLOR_RED "[ERROR] %s" ANSI_COLOR_RESET "\n", message); break;
    case LMS7_WARNING:  printk( ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "[WARNING] %s" ANSI_COLOR_RESET "\n", message); break;
    case LMS7_NOTICE:   printk( ANSI_COLOR_GREEN "[NOTICE] %s" ANSI_COLOR_RESET "\n", message); break;
    case LMS7_INFO:     printk( "[INFO] %s\n", message); break;
    case LMS7_DEBUG:    printk( "[DEBUG] %s\n", message); break;
    case LMS7_TRACE:    printk( "[TRACE] %s\n", message); break;
    }
}

/***********************************************************************
 * logging api implementation
 **********************************************************************/
static LMS7_log_level_t _log_level = LMS7_NOTICE;
static LMS7_log_handler_t _log_handler = default_handler;

void LMS7_set_log_level(const LMS7_log_level_t level)
{
    _log_level = level;
}

void LMS7_log(const LMS7_log_level_t level, const char *message)
{
    if (level > _log_level) return;
    _log_handler(level, message);
}

void LMS7_vlogf(const LMS7_log_level_t level, const char *format, va_list args)
{
#if 0
    if (level > _log_level) return;
    char *message;
    if (vasprintf(&message, format, args) < 0) return;
    LMS7_log(level, message);
    free(message);
#endif

    if (level > _log_level) return;
    char message[1000];
    if (vsprintf(message, format, args) < 0) return;
    LMS7_log(level, message);


}

void LMS7_set_log_handler(const LMS7_log_handler_t handler)
{
    _log_handler = handler;
}


