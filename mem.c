/*
 * Copyright (c) 2013, Derek Buitenhuis
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <inttypes.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "prefix.h"
#include "global.h"

#define REALDEF(PROT,FAIL,SUCC) \
PROT \
{ \
    uint_fast64_t iter; \
\
    init(); \
\
    iter = getiter(); \
\
    if (!(getrand() % getprob())) { \
        fprintf(stderr, \
                "FAILED. Iteration = %"PRIuFAST64", Seed = %u.\n", iter, getseed()); \
        printbt(); \
        return FAIL; \
    } else { \
        return SUCC; \
    } \
}
#define DEFFUNC(PROT,FAIL,SUCC) REALDEF(PROT,FAIL,SUCC)

DEFFUNC(void *PREFIX(malloc)(size_t size), NULL, malloc(size))
DEFFUNC(void *PREFIX(memalign)(size_t align, size_t size), NULL, memalign(align, size))
DEFFUNC(int PREFIX(posix_memalign)(void **ptr, size_t align, size_t size), 0, posix_memalign(ptr, align, size))
DEFFUNC(void *PREFIX(realloc)(void *ptr, size_t size), NULL, realloc(ptr, size))

void PREFIX(free)(void *ptr)
{
    free(ptr);
}
