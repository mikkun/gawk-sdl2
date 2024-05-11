// SPDX-FileCopyrightText: 2024 KUSANAGI Mitsuhisa <mikkun@mbg.nifty.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <gawkapi.h>

/* gawk_api_varinit_constant is taken from the gawkextlib project */

awk_bool_t
gawk_api_varinit_constant(const gawk_api_t *api,
                          awk_ext_id_t ext_id,
                          const char *name,
                          awk_value_t *initial_value,
                          awk_scalar_t *cookie_result)
{
    awk_value_t val;

    if (sym_lookup(name, AWK_UNDEFINED, &val)
        || ! sym_update(name, initial_value)) {
        if (initial_value->val_type == AWK_STRING)
            gawk_free(initial_value->str_value.str);
        return awk_false;
    }

    if (sym_lookup(name, AWK_SCALAR, &val)) {
        *cookie_result = val.scalar_cookie;
        return awk_true;
    }

    return awk_false;
}
