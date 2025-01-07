#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#define MAXLINE 1024

extern char *progname;
extern int debug_level;

void usage(void);
void exit_err(const char *fmt, ...);
void print_err(const char *fmt, ...);
void print_debug(int level, const char *fmt, ...);
void *xs_malloc(size_t size);
char *_xs_strdup(const char *s);
char *_xs_strncpy(char *dest, const char *src, size_t n);

#endif // UTIL_H
