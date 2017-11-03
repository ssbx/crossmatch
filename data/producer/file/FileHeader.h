/*
 * Copyright (c) 2004 CSIRO ICT Centre
 *
 */

#ifndef FILEHEADER_DEFINED
#define FILEHEADER_DEFINED

/*
 * Was having trouble getting the large file support to work.
 * Isolate the include and defines to a single include file
 * and use this. Should make it easier to get working on
 * other platforms if needed.
 */

#define _GNU_SOURCE 1   /* Enable large-files-on-32-bit-systems */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#endif // ifndef FILEHEADER_DEFINED
