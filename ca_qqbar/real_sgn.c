/*
    Copyright (C) 2020 Fredrik Johansson

    This file is part of Calcium.

    Calcium is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "ca_qqbar.h"

int
ca_qqbar_real_sgn(const ca_qqbar_t x)
{
    if (ca_qqbar_degree(x) == 1)
    {
        return -fmpz_sgn(CA_QQBAR_POLY(x)->coeffs);
    }
    else if (arb_is_zero(acb_realref(CA_QQBAR_ENCLOSURE(x))))
    {
        return 0;
    }
    else if (!arb_contains_zero(acb_realref(CA_QQBAR_ENCLOSURE(x))))
    {
        return arf_sgn(arb_midref(acb_realref(CA_QQBAR_ENCLOSURE(x))));
    }
    else
    {
        slong d, i;
        slong prec;
        int res, maybe_zero;
        acb_t t, u;
        acb_init(t);
        acb_init(u);

        d = ca_qqbar_degree(x);

        maybe_zero = 1;
        for (i = 1; i < d && maybe_zero; i += 2)
            if (!fmpz_is_zero(CA_QQBAR_POLY(x)->coeffs + i))
                maybe_zero = 0;

        acb_set(t, CA_QQBAR_ENCLOSURE(x));
        res = 0;

        for (prec = CA_QQBAR_DEFAULT_PREC / 2; ; prec *= 2)
        {
            _ca_qqbar_enclosure_raw(t, CA_QQBAR_POLY(x), t, prec);

            if (!arb_contains_zero(acb_realref(t)))
            {
                res = arf_sgn(arb_midref(acb_realref(t)));
                break;
            }

            if (maybe_zero)
            {
                acb_set(u, t);
                arb_zero(acb_realref(u));
                if (_ca_qqbar_validate_enclosure(u, CA_QQBAR_POLY(x), u, prec * 2) && arb_is_zero(acb_realref(u)))
                {
                    res = 0;
                    break;
                }
            }
        }

        acb_clear(t);
        acb_clear(u);

        return res;

    }
}
