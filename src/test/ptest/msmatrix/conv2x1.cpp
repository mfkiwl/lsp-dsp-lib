/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-dsp-lib
 * Created on: 31 мар. 2020 г.
 *
 * lsp-dsp-lib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-dsp-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-dsp-lib. If not, see <https://www.gnu.org/licenses/>.
 */

#include <lsp-plug.in/common/alloc.h>
#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/dsp/dsp.h>
#include <lsp-plug.in/test-fw/helpers.h>
#include <lsp-plug.in/test-fw/ptest.h>

#define MIN_RANK 8
#define MAX_RANK 16

namespace lsp
{
    namespace generic
    {
        void    lr_to_mid(float *m, const float *l, const float *r, size_t count);
        void    lr_to_side(float *s, const float *l, const float *r, size_t count);
        void    ms_to_left(float *l, const float *m, const float *s, size_t count);
        void    ms_to_right(float *r, const float *m, const float *s, size_t count);
    }

    IF_ARCH_X86(
        namespace sse
        {
            void    lr_to_mid(float *m, const float *l, const float *r, size_t count);
            void    lr_to_side(float *s, const float *l, const float *r, size_t count);
            void    ms_to_left(float *l, const float *m, const float *s, size_t count);
            void    ms_to_right(float *r, const float *m, const float *s, size_t count);
        }

        namespace avx
        {
            void    lr_to_mid(float *m, const float *l, const float *r, size_t count);
            void    lr_to_side(float *s, const float *l, const float *r, size_t count);
            void    ms_to_left(float *l, const float *m, const float *s, size_t count);
            void    ms_to_right(float *r, const float *m, const float *s, size_t count);
        }
    )

    IF_ARCH_ARM(
        namespace neon_d32
        {
            void    lr_to_mid(float *m, const float *l, const float *r, size_t count);
            void    lr_to_side(float *s, const float *l, const float *r, size_t count);
            void    ms_to_left(float *l, const float *m, const float *s, size_t count);
            void    ms_to_right(float *r, const float *m, const float *s, size_t count);
        }
    )

    IF_ARCH_AARCH64(
        namespace asimd
        {
            void    lr_to_mid(float *m, const float *l, const float *r, size_t count);
            void    lr_to_side(float *s, const float *l, const float *r, size_t count);
            void    ms_to_left(float *l, const float *m, const float *s, size_t count);
            void    ms_to_right(float *r, const float *m, const float *s, size_t count);
        }
    )

    typedef void (* conv2x1_t)(float *d, const float *s1, const float *s2, size_t count);
}

PTEST_BEGIN("dsp.msmatrix", conv2x1, 5, 1000)

    void call(const char *label, float *d, const float *s1, const float *s2, size_t count, conv2x1_t func)
    {
        if (!PTEST_SUPPORTED(func))
            return;

        char buf[80];
        sprintf(buf, "%s x %d", label, int(count));
        printf("Testing %s numbers...\n", buf);

        PTEST_LOOP(buf,
            func(d, s1, s2, count);
        );
    }

    PTEST_MAIN
    {
        size_t buf_size = 1 << MAX_RANK;
        uint8_t *data   = NULL;

        float *d        = alloc_aligned<float>(data, buf_size * 3, 64);
        float *s1       = &d[buf_size];
        float *s2       = &s1[buf_size];

        for (size_t i=0; i < buf_size * 3; ++i)
            d[i]            = randf(0.0f, 1.0f);

        #define CALL(func) \
            call(#func, d, s1, s2, count, func)

        for (size_t i=MIN_RANK; i <= MAX_RANK; i++)
        {
            size_t count = 1 << i;

            CALL(generic::lr_to_mid);
            IF_ARCH_X86(CALL(sse::lr_to_mid));
            IF_ARCH_X86(CALL(avx::lr_to_mid));
            IF_ARCH_ARM(CALL(neon_d32::lr_to_mid));
            IF_ARCH_AARCH64(CALL(asimd::lr_to_mid));
            PTEST_SEPARATOR;

            CALL(generic::lr_to_side);
            IF_ARCH_X86(CALL(sse::lr_to_side));
            IF_ARCH_X86(CALL(avx::lr_to_side));
            IF_ARCH_ARM(CALL(neon_d32::lr_to_side));
            IF_ARCH_AARCH64(CALL(asimd::lr_to_side));
            PTEST_SEPARATOR;

            CALL(generic::ms_to_left);
            IF_ARCH_X86(CALL(sse::ms_to_left));
            IF_ARCH_X86(CALL(avx::ms_to_left));
            IF_ARCH_ARM(CALL(neon_d32::ms_to_left));
            IF_ARCH_AARCH64(CALL(asimd::ms_to_left));
            PTEST_SEPARATOR;

            CALL(generic::ms_to_right);
            IF_ARCH_X86(CALL(sse::ms_to_right));
            IF_ARCH_X86(CALL(avx::ms_to_right));
            IF_ARCH_ARM(CALL(neon_d32::ms_to_right));
            IF_ARCH_AARCH64(CALL(asimd::ms_to_right));
            PTEST_SEPARATOR2;
        }

        free_aligned(data);
    }
PTEST_END


