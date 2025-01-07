#include "xwindow.h"

Time get_timestamp(void) {
    // (implementation)
}

Bool get_append_property(XSelectionEvent *xsl, unsigned char **buffer, unsigned long *offset, unsigned long *alloc) {
    // (implementation)
}

unsigned char *wait_incr_selection(Atom selection, XSelectionEvent *xsl, int init_alloc) {
    // (implementation)
}

unsigned char *wait_selection(Atom selection, Atom request_target) {
    // (implementation)
}

unsigned char *get_selection(Atom selection, Atom request_target) {
    // (implementation)
}

unsigned char *get_selection_text(Atom selection) {
    // (implementation)
}
