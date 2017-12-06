/*
 * Copyright (c) 2017 Derek Buitenhuis
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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static void errprint(void *data, const char *msg, int errnum)
{
    fprintf(stdout, "Backtrace Error (%d): %s.\n", errnum, msg);
}

static int stackprint(void *data, uintptr_t pc, const char *filename, int lineno, const char *function)
{
    /* We've reach the end of usefulness. */
    if (lineno == 0)
        return 1;
    fprintf(stdout, "%s:%d in %s\n", function, lineno, filename);
    return 0;
}

void binit(struct backtrace_state **state)
{
    *state = backtrace_create_state("", 1, errprint, NULL);

    /* If we can't backtrace, there's no point in continuing. */
    if (*state == NULL)
        abort();
}

void bprint(struct backtrace_state *state)
{
    /* Skip one symbol. */
    backtrace_full(state, 1, stackprint, errprint, NULL);
}
