/*
    Copyright (C) 2012 Fredrik Johansson

    This file is part of Arb.

    Arb is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "acb.h"

void
acb_randtest(acb_t z, flint_rand_t state, slong prec, slong mag_bits)
{
    arb_randtest(acb_realref(z), state, prec, mag_bits);
    arb_randtest(acb_imagref(z), state, prec, mag_bits);
}

void
acb_randtest_special(acb_t z, flint_rand_t state, slong prec, slong mag_bits)
{
    arb_randtest_special(acb_realref(z), state, prec, mag_bits);
    arb_randtest_special(acb_imagref(z), state, prec, mag_bits);
}

void
acb_randtest_precise(acb_t z, flint_rand_t state, slong prec, slong mag_bits)
{
    arb_randtest_precise(acb_realref(z), state, prec, mag_bits);
    arb_randtest_precise(acb_imagref(z), state, prec, mag_bits);
}

void
acb_randtest_param(acb_t z, flint_rand_t state, slong prec, slong size)
{
    if (n_randint(state, 8) == 0)
    {
        fmpz_t t;
        fmpz_init(t);
        fmpz_randtest(t, state, 1 + n_randint(state, prec));
        arb_set_fmpz(acb_realref(z), t);
        arb_zero(acb_imagref(z));
        acb_mul_2exp_si(z, z, -1);
        fmpz_clear(t);
    }
    else
    {
        acb_randtest(z, state, prec, size);
    }
}

void
acb_urandom(acb_t z, flint_rand_t state, slong prec)
{
    arb_t x, y, r;
    int stop = 0;
    
    arb_init(x);
    arb_init(y);
    arb_init(r);
    
    while (!stop)
    {
        arb_urandom(x, state, prec);
        arb_urandom(y, state, prec);
        if (n_randint(state, 2) == 0) arb_neg(x, x);
        if (n_randint(state, 2) == 0) arb_neg(y, y);
        acb_set_arb_arb(z, x, y);
        acb_abs(r, z, prec);
        arb_sub_si(r, r, 1, prec);
        stop = arb_is_nonpositive(r);
    }

    arb_clear(x);
    arb_clear(y);
    arb_clear(r);
}
