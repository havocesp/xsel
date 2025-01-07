#ifndef SELECTION_H
#define SELECTION_H

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "util.h"

#define MAXFNAME 256

extern Display *display;
extern Window window;
extern Atom timestamp_atom;
extern Atom multiple_atom;
extern Atom targets_atom;
extern Atom delete_atom;
extern Atom incr_atom;
extern Atom null_atom;
extern Atom text_atom;
extern Atom utf8_atom;
extern Atom compound_text_atom;

void set_selection(Atom selection, unsigned char *sel);
void set_selection__daemon(Atom selection, unsigned char *sel);
void set_selection_pair(unsigned char *sel_p, unsigned char *sel_s);
void set_selection_pair__daemon(unsigned char *sel_p, unsigned char *sel_s);
void keep_selections(void);
void exchange_selections(void);

#endif // SELECTION_H
