#include "util.h"

char *progname;
int debug_level = 0;

void usage(void) {
    printf("Usage: xsel [options]\n");
    // ... (remaining usage information)
}

void exit_err(const char *fmt, ...) {
    va_list ap;
    int errno_save = errno;
    char buf[MAXLINE];
    int n;

    va_start(ap, fmt);
    snprintf(buf, MAXLINE, "%s: ", progname);
    n = strlen(buf);
    vsnprintf(buf + n, MAXLINE - n, fmt, ap);
    n = strlen(buf);
    snprintf(buf + n, MAXLINE - n, ": %s\n", strerror(errno_save));
    fflush(stdout); // in case stdout and stderr are the same
    fputs(buf, stderr);
    fflush(NULL);
    va_end(ap);
    exit(1);
}

void print_err(const char *fmt, ...) {
    va_list ap;
    int errno_save = errno;
    char buf[MAXLINE];
    int n;

    va_start(ap, fmt);
    snprintf(buf, MAXLINE, "%s: ", progname);
    n = strlen(buf);
    vsnprintf(buf + n, MAXLINE - n, fmt, ap);
    n = strlen(buf);
    fflush(stdout); // in case stdout and stderr are the same
    fputs(buf, stderr);
    fputc('\n', stderr);
    fflush(NULL);
    va_end(ap);
}

void print_debug(int level, const char *fmt, ...) {
    if (level <= debug_level) {
        va_list ap;
        va_start(ap, fmt);
        print_err(fmt, ap);
        va_end(ap);
    }
}

void *xs_malloc(size_t size) {
    void *ret = malloc(size);
    if (size == 0) size = 1;
    if (ret == NULL) {
        exit_err("malloc error");
    }
    return ret;
}

char *_xs_strdup(const char *s) {
    if (s == NULL) return NULL;
    char *ret = strdup(s);
    if (ret == NULL) {
        exit_err("strdup error");
    }
    return ret;
}

char *_xs_strncpy(char *dest, const char *src, size_t n) {
    if (n > 0) {
        strncpy(dest, src, n - 1);
        dest[n - 1] = '\0';
    }
    return dest;
}
