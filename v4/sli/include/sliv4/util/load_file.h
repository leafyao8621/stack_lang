#ifndef UTIL_LOAD_FILE_H_
#define UTIL_LOAD_FILE_H_

#include <containers/dstring.h>

#include <sliv4/util/errcode.h>

SLErr load_file(char *fn, String *out);

#endif
