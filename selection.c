#include "selection.h"

Display *display;
Window window;
Atom timestamp_atom;
Atom multiple_atom;
Atom targets_atom;
Atom delete_atom;
Atom incr_atom;
Atom null_atom;
Atom text_atom;
Atom utf8_atom;
Atom compound_text_atom;

/*
 * set_selection (selection, sel)
 *
 * Takes ownership of the selection 'selection', then loops waiting for
 * its SelectionClear or SelectionRequest events.
 *
 * Handles SelectionRequest events, first checking for additional
 * input if the user has specified 'follow' mode. Returns when a
 * SelectionClear event is received for the specified selection.
 */
static void set_selection (Atom selection, unsigned char * sel)
{
  XEvent event;
  IncrTrack * it;

  if (own_selection (selection) == False) return;

  for (;;) {
    /* Flush before unblocking signals so we send replies before exiting */
    XFlush (display);
    unblock_exit_sigs ();
    XNextEvent (display, &event);
    block_exit_sigs ();

    switch (event.type) {
    case SelectionClear:
      if (event.xselectionclear.selection == selection) return;
      break;
    case SelectionRequest:
      if (event.xselectionrequest.selection != selection) break;

      if (do_follow)
        sel = read_input (sel, True);
      
      if (!handle_selection_request (event, sel)) return;
      
      break;
    case PropertyNotify:
      if (event.xproperty.state != PropertyDelete) break;

      it = find_incrtrack (event.xproperty.atom);

      if (it != NULL) {
        continue_incr (it);
      }

      break;
    default:
      break;
    }
  }
}

/*
 * set_selection__daemon (selection, sel)
 *
 * Creates a daemon process to handle selection requests for the
 * specified selection 'selection', to respond with selection text 'sel'.
 * If 'sel' is an empty string (NULL or "") then no daemon process is
 * created and the specified selection is cleared instead.
 */
static void set_selection__daemon (Atom selection, unsigned char * sel)
{
  if (empty_string (sel) && !do_follow) {
    clear_selection (selection);
    return;
  }

  become_daemon ();

  set_selection (selection, sel);
}

/*
 * set_selection_pair (sel_p, sel_s)
 *
 * Handles SelectionClear and SelectionRequest events for both the
 * primary and secondary selections. Returns once SelectionClear events
 * have been received for both selections. Responds to SelectionRequest
 * events for the primary selection with text 'sel_p' and for the
 * secondary selection with text 'sel_s'.
 */
static void set_selection_pair (unsigned char * sel_p, unsigned char * sel_s)
{
  XEvent event;
  IncrTrack * it;
  
  if (sel_p) {
    if (own_selection (XA_PRIMARY) == False)
      free_string (sel_p);
  } else {
    clear_selection (XA_PRIMARY);
  }

  if (sel_s) {
    if (own_selection (XA_SECONDARY) == False)
      free_string (sel_s);
  } else {
    clear_selection (XA_SECONDARY);
  }

  for (;;) {
    /* Flush before unblocking signals so we send replies before exiting */
    XFlush (display);
    unblock_exit_sigs ();
    XNextEvent (display, &event);
    block_exit_sigs ();

    switch (event.type) {
    case SelectionClear:
      if (event.xselectionclear.selection == XA_PRIMARY) {
        free_string (sel_p);
        if (sel_s == NULL) return;
      } else if (event.xselectionclear.selection == XA_SECONDARY) {
        free_string (sel_s);
        if (sel_p == NULL) return;
      }
      break;
    case SelectionRequest:
      if (event.xselectionrequest.selection == XA_PRIMARY) {
        if (!handle_selection_request (event, sel_p)) {
          free_string (sel_p);
          if (sel_s == NULL) return;
        }
      } else if (event.xselectionrequest.selection == XA_SECONDARY) {
        if (!handle_selection_request (event, sel_s)) {
          free_string (sel_s);
          if (sel_p == NULL) return;
        }
      }
      break;
    case PropertyNotify:
      if (event.xproperty.state != PropertyDelete) break;

      it = find_incrtrack (event.xproperty.atom);

      if (it != NULL) {
        continue_incr (it);
      }
      break;
    default:
      break;
    }
  }
}


/*
 * set_selection_pair__daemon (sel_p, sel_s)
 *
 * Creates a daemon process to handle selection requests for both the
 * primary and secondary selections with texts 'sel_p' and 'sel_s'
 * respectively.
 *
 * If both 'sel_p' and 'sel_s' are empty strings (NULL or "") then no
 * daemon process is created, and both selections are cleared instead.
 */
static void set_selection_pair__daemon (unsigned char * sel_p, unsigned char * sel_s)
{
  if (empty_string (sel_p) && empty_string (sel_s)) {
    clear_selection (XA_PRIMARY);
    clear_selection (XA_SECONDARY);
    return;
  }

  become_daemon ();

  set_selection_pair (sel_p, sel_s);
}


/*
 * keep_selections ()
 *
 * Takes ownership of both the primary and secondary selections. The current
 * selection texts are retrieved and a new daemon process is created to
 * handle both selections unmodified.
 */
static void keep_selections (void)
{
  unsigned char * text1, * text2;

  text1 = get_selection_text (XA_PRIMARY);
  text2 = get_selection_text (XA_SECONDARY);

  set_selection_pair__daemon (text1, text2);
}


/*
 * exchange_selections ()
 *
 * Exchanges the primary and secondary selections. The current selection
 * texts are retrieved and a new daemon process is created to handle both
 * selections with their texts exchanged.
 */
static void exchange_selections (void)
{
  unsigned char * text1, * text2;

  text1 = get_selection_text (XA_PRIMARY);
  text2 = get_selection_text (XA_SECONDARY);

  set_selection_pair__daemon (text2, text1);
}
