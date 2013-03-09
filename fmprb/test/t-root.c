/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include "fmprb.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("root....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 100000; iter++)
    {
        fmprb_t a, b, c;
        ulong k;
        long prec;

        prec = 2 + n_randint(state, 200);
        k = 1 + n_randint(state, 10);

        fmprb_init(a);
        fmprb_init(b);
        fmprb_init(c);

        fmprb_randtest(a, state, 1 + n_randint(state, 200), 10);

        fmprb_root(b, a, k, prec);
        fmprb_pow_ui(c, b, k, prec);

        if (!fmprb_contains(c, a))
        {
            printf("FAIL: containment\n\n");
            printf("k = %lu\n", k);
            printf("a = "); fmprb_print(a); printf("\n\n");
            printf("b = "); fmprb_print(b); printf("\n\n");
            printf("c = "); fmprb_print(c); printf("\n\n");
            abort();
        }

        fmprb_root(a, a, k, prec);

        if (!fmprb_equal(a, b))
        {
            printf("FAIL: aliasing\n\n");
            abort();
        }

        fmprb_clear(a);
        fmprb_clear(b);
        fmprb_clear(c);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
