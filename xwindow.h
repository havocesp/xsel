#ifndef XWINDOW_H
#define XWINDOW_H

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "util.h"

Time get_timestamp(void);
Bool get_append_property(XSelectionEvent *xsl, unsigned char **buffer, unsigned long *offset, unsigned long *alloc);
unsigned char *wait_incr_selection(Atom selection, XSelectionEvent *xsl, int init_alloc);
unsigned char *wait_selection(Atom selection, Atom request_target);
unsigned char *get_selection(Atom selection, Atom request_target);
unsigned char *get_selection_text(Atom selection);

#endif // XWINDOW_H
