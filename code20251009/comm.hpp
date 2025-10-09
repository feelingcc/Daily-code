#pragma once

#include <cstdio>
#include <cstdlib>

#define ERR_EXIT(errorstr)   \
    do {                     \
        perror(errorstr);    \
        exit(EXIT_FAILURE);  \
    } while(0)

// 