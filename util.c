#include "util.h"

char *progname;
int debug_level = 0;

/*
 * usage ()
 *
 * print usage information.
 */
static void usage (void)
{
    printf ("Usage: xsel [options]\n");
    printf ("Manipulate the X selection.\n\n");
    printf ("By default the current selection is output and not modified if both\n");
    printf ("standard input and standard output are terminals (ttys).  Otherwise,\n");
    printf ("the current selection is output if standard output is not a terminal\n");
    printf ("(tty), and the selection is set from standard input if standard input\n");
    printf ("is not a terminal (tty). If any input or output options are given then\n");
    printf ("the program behaves only in the requested mode.\n\n");
    printf ("If both input and output is required then the previous selection is\n");
    printf ("output before being replaced by the contents of standard input.\n\n");
    printf ("Input options\n");
    printf ("  -a, --append          Append standard input to the selection\n");
    printf ("  -f, --follow          Append to selection as standard input grows\n");
    printf ("  -z, --zeroflush       Overwrites selection when zero ('\\0') is received\n");
    printf ("  -i, --input           Read standard input into the selection\n\n");
    printf ("Output options\n");
    printf ("  -o, --output          Write the selection to standard output\n\n");
    printf ("Action options\n");
    printf ("  -c, --clear           Clear the selection\n");
    printf ("  -d, --delete          Request that the selection be cleared and that\n");
    printf ("                        the application owning it delete its contents\n\n");
    printf ("Selection options\n");
    printf ("  -p, --primary         Operate on the PRIMARY selection (default)\n");
    printf ("  -s, --secondary       Operate on the SECONDARY selection\n");
    printf ("  -b, --clipboard       Operate on the CLIPBOARD selection\n\n");
    printf ("  -k, --keep            Do not modify the selections, but make the PRIMARY\n");
    printf ("                        and SECONDARY selections persist even after the\n");
    printf ("                        programs they were selected in exit.\n");
    printf ("  -x, --exchange        Exchange the PRIMARY and SECONDARY selections\n\n");
    printf ("X options\n");
    printf ("  --display displayname\n");
    printf ("                        Specify the connection to the X server\n");
    printf ("  -m wm, --name wm      Name with the process will be identified\n");
    printf ("  -t ms, --selectionTimeout ms\n");
    printf ("                        Specify the timeout in milliseconds within which the\n");
    printf ("                        selection must be retrieved. A value of 0 (zero)\n");
    printf ("                        specifies no timeout (default)\n\n");
    printf ("Miscellaneous options\n");
    printf ("  --trim                Remove newline ('\\n') char from end of input / output\n");
    printf ("  -l, --logfile         Specify file to log errors to when detached.\n");
    printf ("  -n, --nodetach        Do not detach from the controlling terminal. Without\n");
    printf ("                        this option, xsel will fork to become a background\n");
    printf ("                        process in input, exchange and keep modes.\n\n");
    printf ("  -h, --help            Display this help and exit\n");
    printf ("  -v, --verbose         Print informative messages\n");
    printf ("  --version             Output version information and exit\n\n");
    printf ("Please report bugs to <conrad@vergenet.net>.\n");
}


/*
 * exit_err (fmt)
 *
 * Print a formatted error message and errno information to stderr,
 * then exit with return code 1.
 */
static void exit_err (const char * fmt, ...)
{
    va_list ap;
    int errno_save;
    char buf[MAXLINE];
    int n;
    
    errno_save = errno;
    
    va_start (ap, fmt);
    
    snprintf (buf, MAXLINE, "%s: ", progname);
    n = strlen (buf);
    
    vsnprintf (buf+n, MAXLINE-n, fmt, ap);
    n = strlen (buf);
    
    snprintf (buf+n, MAXLINE-n, ": %s\n", strerror (errno_save));
    
    fflush (stdout); /* in case stdout and stderr are the same */
    fputs (buf, stderr);
    fflush (NULL);
    
    va_end (ap);
    exit (1);
}

/*
 * print_err (fmt)
 *
 * Print a formatted error message to stderr.
 */
static void print_err (const char * fmt, ...)
{
    va_list ap;
    int errno_save;
    char buf[MAXLINE];
    int n;
    
    errno_save = errno;
    
    va_start (ap, fmt);
    
    snprintf (buf, MAXLINE, "%s: ", progname);
    n = strlen (buf);
    
    vsnprintf (buf+n, MAXLINE-n, fmt, ap);
    n = strlen (buf);
    
    fflush (stdout); /* in case stdout and stderr are the same */
    fputs (buf, stderr);
    fputc ('\n', stderr);
    fflush (NULL);
    
    va_end (ap);
}


/*
 * print_debug (level, fmt)
 *
 * Print a formatted debugging message of level 'level' to stderr
 */
#define print_debug(x,y...) {if (x <= debug_level) print_err (y);}


/*
 * xs_malloc (size)
 *
 * Malloc wrapper. Always returns a successful allocation. Exits if the
 * allocation didn't succeed.
 */
static void *xs_malloc (size_t size)
{
    void * ret;
    
    if (size == 0) size = 1;
    if ((ret = malloc (size)) == NULL) {
    exit_err ("malloc error");
    }
    
    return ret;
}


/*
 * xs_strdup (s)
 *
 * strdup wrapper for unsigned char *
 */
#define xs_strdup(s) ((unsigned char *) _xs_strdup ((const char *)s))
static char *_xs_strdup (const char * s)
{
    char *ret;
    
    if (s == NULL) return NULL;
    if ((ret = strdup(s)) == NULL) {
    exit_err ("strdup error");
    }
    
    return ret; 
}

/*
 * xs_strncpy (s)
 *
 * strncpy wrapper for unsigned char *
 */
#define xs_strncpy(dest,s,n) (_xs_strncpy ((char *)dest, (const char *)s, n))
static char *_xs_strncpy (char * dest, const char * src, size_t n)
{
    if (n > 0) {
    strncpy (dest, src, n-1);
    dest[n-1] = '\0';
    }
    return dest;
}
