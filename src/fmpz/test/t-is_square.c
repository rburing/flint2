/*
    Copyright (C) 2009 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <https://www.gnu.org/licenses/>.
*/

#include "test_helpers.h"
#include "ulong_extras.h"
#include "fmpz.h"

TEST_FUNCTION_START(fmpz_is_square, state)
{
    int i, result;

    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpz_t a;
        mpz_t b;
        int r1, r2;

        fmpz_init(a);

        mpz_init(b);

        fmpz_randtest(a, state, 200);
        if (n_randint(state, 2) == 0)
            fmpz_mul(a, a, a);

        fmpz_get_mpz(b, a);

        r1 = fmpz_is_square(a);
        r2 = mpz_perfect_square_p(b);
        result = (r1 == r2);

        if (!result)
        {
            flint_printf("FAIL:\n");
            gmp_printf("b = %Zd\n", b);
            fflush(stdout);
            flint_abort();
        }

        fmpz_clear(a);

        mpz_clear(b);
    }

    TEST_FUNCTION_END(state);
}
