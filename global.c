/*
 * Copyright (c) 2013-2017 Derek Buitenhuis
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

#include <backtrace.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#include "bt.h"

pthread_once_t inited = PTHREAD_ONCE_INIT;
pthread_mutex_t randl = PTHREAD_MUTEX_INITIALIZER;

unsigned int seed              = 0;
unsigned int prob              = 10000;
atomic_uint_fast64_t iteration = ATOMIC_VAR_INIT(0);

struct backtrace_state *bstate = NULL;

static void onceinit()
{
    char *userseed = getenv("FUZZ_SEED");
    char *userprob = getenv("FUZZ_FAILPROB");

    binit(&bstate);

    if (!userseed)
        seed = (unsigned int) time(NULL);
    else
        seed = (unsigned int) atoi(userseed);

    srand(seed);

    if (userprob)
        prob = atoi(userprob);
}

void init()
{
    pthread_once(&inited, onceinit);
}

/*
 * This could be a lot smarter by generating N random numbers at init time
 * and filling as needed.
 */
int getrand()
{
    int ret;
    pthread_mutex_lock(&randl);
    ret = rand();
    pthread_mutex_unlock(&randl);
    return ret;
}

unsigned int getseed()
{
    return seed;
}

unsigned int getprob()
{
    return prob;
}

uint_fast64_t getiter()
{
    return atomic_fetch_add(&iteration, 1);
}

/* backtrace is threadsafe when initialized properly. */
void printbt()
{
    bprint(bstate);
}
