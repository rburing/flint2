/*
    Copyright (C) 2016 William Hart

    This file is part of FLINT.

    FLINT is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <gmp.h>
#include <stdlib.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_mpoly.h"

slong _fmpz_mpoly_sub1(fmpz * poly1, ulong * exps1,
                      fmpz * poly2, ulong * exps2, slong len2,
                      fmpz * poly3, ulong * exps3, slong len3,
                                         slong bits, slong n, int deg, int rev)
{
   slong i = 0, j = 0, k = 0;

   while (i < len2 && j < len3)
   {
      if (exps2[i] < exps3[j])
      {
         fmpz_set(poly1 + k, poly2 + i);
         exps1[k] = exps2[i];
         i++;
      } else if (exps2[i] == exps3[j])
      {
         fmpz_sub(poly1 + k, poly2 + i, poly3 + j);
         exps1[k] = exps2[i];
         if (fmpz_is_zero(poly1 + k))
            k--;
         i++;
         j++;
      } else
      {
         fmpz_neg(poly1 + k, poly3 + j);
         exps1[k] = exps3[j];
         j++;         
      }
      k++;
   }

   while (i < len2)
   {
      fmpz_set(poly1 + k, poly2 + i);
      exps1[k] = exps2[i];
      i++;
      k++;
   }

   while (j < len3)
   {
      fmpz_neg(poly1 + k, poly3 + j);
      exps1[k] = exps3[j];
      j++;
      k++;
   }

   return k;
}

void fmpz_mpoly_sub(fmpz_mpoly_t poly1, fmpz_mpoly_t poly2,
                                      fmpz_mpoly_t poly3, fmpz_mpoly_ctx_t ctx)
{
   slong len = 0;
   int deg, rev;

   if (ctx->N == 1)
   {
      degrev_from_ord(deg, rev, ctx->ord);

      if (poly1 == poly2 || poly1 == poly3)
      {
         fmpz_mpoly_t temp;

         fmpz_mpoly_init2(temp, poly2->length + poly3->length, ctx);

         len = _fmpz_mpoly_sub1(temp->coeffs, temp->exps, 
                       poly2->coeffs, poly2->exps, poly2->length,
                       poly3->coeffs, poly3->exps, poly3->length,
                                                  ctx->bits, ctx->n, deg, rev);

         fmpz_mpoly_swap(temp, poly1, ctx);

         fmpz_mpoly_clear(temp, ctx);
      } else
      {
         fmpz_mpoly_fit_length(poly1, poly2->length + poly3->length, ctx);

         len = _fmpz_mpoly_sub1(poly1->coeffs, poly1->exps, 
                       poly2->coeffs, poly2->exps, poly2->length,
                       poly3->coeffs, poly3->exps, poly3->length,
                                                  ctx->bits, ctx->n, deg, rev);
      }

      _fmpz_mpoly_set_length(poly1, len, ctx);

   } else
      flint_throw(FLINT_ERROR, "Not implemented yet");
}
