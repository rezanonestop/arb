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

#include "fmpcb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("interpolate_newton....");
    fflush(stdout);

    flint_randinit(state);

    for (iter = 0; iter < 10000; iter++)
    {
        long i, n, qbits1, qbits2, rbits1, rbits2, rbits3;
        fmpq_poly_t P;
        fmpcb_poly_t R, S;
        fmpq_t t, u;
        fmpcb_ptr xs, ys;

        fmpq_poly_init(P);
        fmpcb_poly_init(R);
        fmpcb_poly_init(S);
        fmpq_init(t);
        fmpq_init(u);

        qbits1 = 2 + n_randint(state, 200);
        qbits2 = 2 + n_randint(state, 5);
        rbits1 = 2 + n_randint(state, 200);
        rbits2 = 2 + n_randint(state, 200);
        rbits3 = 2 + n_randint(state, 200);

        fmpq_poly_randtest(P, state, 1 + n_randint(state, 20), qbits1);
        n = P->length;

        xs = _fmpcb_vec_init(n);
        ys = _fmpcb_vec_init(n);

        fmpcb_poly_set_fmpq_poly(R, P, rbits1);

        if (n > 0)
        {
            fmpq_randtest(t, state, qbits2);
            fmpcb_set_fmpq(xs, t, rbits2);

            for (i = 1; i < n; i++)
            {
                fmpq_randtest_not_zero(u, state, qbits2);
                fmpq_abs(u, u);
                fmpq_add(t, t, u);
                fmpcb_set_fmpq(xs + i, t, rbits2);
            }
        }

        for (i = 0; i < n; i++)
            fmpcb_poly_evaluate(ys + i, R, xs + i, rbits2);

        fmpcb_poly_interpolate_newton(S, xs, ys, n, rbits3);

        if (!fmpcb_poly_contains_fmpq_poly(S, P))
        {
            printf("FAIL:\n");
            printf("P = "); fmpq_poly_print(P); printf("\n\n");
            printf("R = "); fmpcb_poly_printd(R, 15); printf("\n\n");
            printf("S = "); fmpcb_poly_printd(S, 15); printf("\n\n");
            abort();
        }

        fmpq_poly_clear(P);
        fmpcb_poly_clear(R);
        fmpcb_poly_clear(S);
        fmpq_clear(t);
        fmpq_clear(u);
        _fmpcb_vec_clear(xs, n);
        _fmpcb_vec_clear(ys, n);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}
