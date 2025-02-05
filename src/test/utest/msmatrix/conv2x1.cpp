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

#include <lsp-plug.in/common/types.h>
#include <lsp-plug.in/test-fw/utest.h>
#include <lsp-plug.in/test-fw/FloatBuffer.h>

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

//-----------------------------------------------------------------------------
// Unit test for complex multiplication
UTEST_BEGIN("dsp.msmatrix", conv2x1)

    void call(const char *label, size_t align, conv2x1_t func1, conv2x1_t func2)
    {
        if (!UTEST_SUPPORTED(func1))
            return;
        if (!UTEST_SUPPORTED(func2))
            return;

        UTEST_FOREACH(count, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
                32, 64, 65, 100, 999, 0xfff)
        {
            for (size_t mask=0; mask <= 0x07; ++mask)
            {
                printf("Testing %s on input buffer of %d numbers, mask=0x%x...\n", label, int(count), int(mask));

                FloatBuffer srcA(count, align, mask & 0x01);
                FloatBuffer srcB(count, align, mask & 0x02);
                FloatBuffer dst1(count, align, mask & 0x04);
                FloatBuffer dst2(dst1);

                // Call functions
                func1(dst1, srcA, srcB, count);
                func1(dst2, srcA, srcB, count);

                UTEST_ASSERT_MSG(srcA.valid(), "Source buffer A corrupted");
                UTEST_ASSERT_MSG(srcB.valid(), "Source buffer A corrupted");
                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_relative(dst2, 1e-4))
                {
                    srcA.dump("srcA");
                    srcB.dump("srcB");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    if (dst1.last_diff() >= 0)
                        printf("dst1 vs dst2 index=%d, %.6f vs %.6f\n", dst1.last_diff(), dst1.get_diff(), dst2.get_diff());
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", label);
                }
            }
        }
    }

    UTEST_MAIN
    {
        #define CALL(generic, func, align) \
            call(#func, align, generic, func)

        IF_ARCH_X86(CALL(generic::lr_to_mid, sse::lr_to_mid, 16));
        IF_ARCH_X86(CALL(generic::lr_to_side, sse::lr_to_side, 16));
        IF_ARCH_X86(CALL(generic::ms_to_left, sse::ms_to_left, 16));
        IF_ARCH_X86(CALL(generic::ms_to_right, sse::ms_to_right, 16));

        IF_ARCH_X86(CALL(generic::lr_to_mid, avx::lr_to_mid, 32));
        IF_ARCH_X86(CALL(generic::lr_to_side, avx::lr_to_side, 32));
        IF_ARCH_X86(CALL(generic::ms_to_left, avx::ms_to_left, 32));
        IF_ARCH_X86(CALL(generic::ms_to_right, avx::ms_to_right, 32));

        IF_ARCH_ARM(CALL(generic::lr_to_mid, neon_d32::lr_to_mid, 16));
        IF_ARCH_ARM(CALL(generic::lr_to_side, neon_d32::lr_to_side, 16));
        IF_ARCH_ARM(CALL(generic::ms_to_left, neon_d32::ms_to_left, 16));
        IF_ARCH_ARM(CALL(generic::ms_to_right, neon_d32::ms_to_right, 16));

        IF_ARCH_AARCH64(CALL(generic::lr_to_mid, asimd::lr_to_mid, 16));
        IF_ARCH_AARCH64(CALL(generic::lr_to_side, asimd::lr_to_side, 16));
        IF_ARCH_AARCH64(CALL(generic::ms_to_left, asimd::ms_to_left, 16));
        IF_ARCH_AARCH64(CALL(generic::ms_to_right, asimd::ms_to_right, 16));
    }
UTEST_END


