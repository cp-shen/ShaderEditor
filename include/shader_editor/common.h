/********************************/
/* Date: 2020-04-25             */
/* Desc: common utility code    */
/********************************/

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#define LOG_MSG(...)                                                           \
    fprintf(stdout, "MSG: ");                                                  \
    fprintf(stdout, __VA_ARGS__);                                              \
    fprintf(stdout, "\n")
#define LOG_ERR(...)                                                           \
    fprintf(stderr, "ERR: ");                                                  \
    fprintf(stderr, __VA_ARGS__);                                              \
    fprintf(stderr, "\n")

#endif
