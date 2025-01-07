# XSel

## Description

Manipulate the X selection.

## Intro

XSel is a command-line program for getting and setting the contents of the
X selection. Normally this is only accessible by manually highlighting
information and pasting it with the middle mouse button.


### To read a file into the X selection:

```shell
xsel < file
```

After which you can paste the file's contents into any X application with
the middle mouse button, as though you had highlighted its text. XSel will
read in the file contents exactly, whereas manual highlighting invariably
breaks lines and transforms tabs into spaces. This is especially handy for
copying in large files.


### To write the X selection to a file:

```shell
xsel > file
```

After which file will contain exactly the contents of the X selection,
without trailing newlines and spaces and crap.


XSel is more than just cat for the X selection.


### Append to the X selection:

```shell
xsel --append < file
```

### To follow a growing file:

```shell
xsel --follow < file
```

To make the X selection follow standard input as it grows (like `tail -f`).


## Advanced features

XSel also lets you access some of the more esoteric features of the
X selection:

### Delete the contents of the selection

```shell
xsel --delete
```

Will cause the program in which text is selected to delete that text. This
really works, you can try it on xedit to remotely delete text in the editor
window.

### Manipulate the secondary selection

The X Window System maintains two selections, the usual primary selection
and a secondary, which isn't used much ... XSel lets you use the secondary
selection, for example:

#### To get and set the secondary selection:

```shell
xsel --secondary < file
xsel --secondary > file
```

#### To swap the primary and secondary selections:

```shell
xsel --exchange
```

So, for example, you can store useful text in the secondary selection and
retrieve it later.


### Manipulate the clipboard selection

Similarly, X has a clipboard selection. You can use the standard xclipboard
program to manage a history of selected text, and you can use xsel to
actually get text into that clipboard:

```shell
xsel --clipboard < file
```

### Make the selection contents persist in memory

Normally the X selection only exists as long as the program it was selected
in is running. Further, some buggy applications tend to forget their
selection text after a little while. If you run:

```shell
xsel --keep
```

After selecting some important text, xsel will copy the text into its own
memory so you can paste it elsewhere even if the original program exits or crashes.


## Further information

Naturally all these options have single character equivalents, and

```shell
xsel --help
```

provides usage information. For complete details, see the xsel(1x) man page.


## Standards

### X11 ICCCM2

XSel conforms to the X Window System Inter-Client Communication Conventions
Manual Version 2.0 (ICCCM2), including correct handling of TARGETS,
MULTIPLE, TIMESTAMP, and DELETE targets, INCR properties and large data
transfers.

My thoughts on ICCCM are available at:

https://web.archive.org/web/20110720132303/https://lists.slug.org.au/archives/slug-chat/2001/July/msg00054.html
