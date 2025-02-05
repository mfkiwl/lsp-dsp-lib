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
        void    fmadd_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        void    fmsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        void    fmrsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        void    fmmul_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        void    fmdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        void    fmrdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        void    fmmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        void    fmrmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
    }

    IF_ARCH_X86(
        namespace sse
        {
            void    fmadd_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmul_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        }

        namespace sse2
        {
            void    fmmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        }

        namespace avx
        {
            void    fmadd_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmul_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);

            void    fmadd_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmsub_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrsub_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmod_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrmod_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
        }

        namespace avx2
        {
            void    fmadd_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmul_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);

            void    fmadd_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmsub_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrsub_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmod_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrmod_k4_fma3(float *dst, const float *src1, const float *src2, float k, size_t count);
        }
    )

    IF_ARCH_ARM(
        namespace neon_d32
        {
            void    fmadd_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmul_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        }
    )

    IF_ARCH_AARCH64(
        namespace asimd
        {
            void    fmadd_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrsub_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmul_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrdiv_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
            void    fmrmod_k4(float *dst, const float *src1, const float *src2, float k, size_t count);
        }
    )
}

typedef void (* fmop_k4_t)(float *dst, const float *src1, const float *src2, float k, size_t count);

UTEST_BEGIN("dsp.pmath", fmop_k4)

    void call(const char *label, size_t align, fmop_k4_t func1, fmop_k4_t func2)
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

                FloatBuffer src1(count, align, mask & 0x01);
                FloatBuffer src2(count, align, mask & 0x02);
                FloatBuffer dst1(count, align, mask & 0x04);
                FloatBuffer dst2(count, align, mask & 0x04);

                // Call functions
                src1.randomize_sign();
                src2.randomize_sign();
                func1(dst1, src1, src2, 0.4f, count);
                func2(dst2, src1, src2, 0.4f, count);

                UTEST_ASSERT_MSG(src1.valid(), "Source buffer 1 corrupted");
                UTEST_ASSERT_MSG(src2.valid(), "Source buffer 2 corrupted");
                UTEST_ASSERT_MSG(dst1.valid(), "Destination buffer 1 corrupted");
                UTEST_ASSERT_MSG(dst2.valid(), "Destination buffer 2 corrupted");

                // Compare buffers
                if (!dst1.equals_adaptive(dst2, 1e-4))
                {
                    src1.dump("src1");
                    src2.dump("src2");
                    dst1.dump("dst1");
                    dst2.dump("dst2");
                    printf("index=%d, %.6f vs %.6f\n", dst1.last_diff(), dst1.get_diff(), dst2.get_diff());
                    UTEST_FAIL_MSG("Output of functions for test '%s' differs", label);
                }
            }
        }
    }

    UTEST_MAIN
    {
        #define CALL(generic, func, align) \
            call(#func, align, generic, func)

        IF_ARCH_X86(CALL(generic::fmadd_k4, sse::fmadd_k4, 16));
        IF_ARCH_X86(CALL(generic::fmsub_k4, sse::fmsub_k4, 16));
        IF_ARCH_X86(CALL(generic::fmrsub_k4, sse::fmrsub_k4, 16));
        IF_ARCH_X86(CALL(generic::fmmul_k4, sse::fmmul_k4, 16));
        IF_ARCH_X86(CALL(generic::fmdiv_k4, sse::fmdiv_k4, 16));
        IF_ARCH_X86(CALL(generic::fmrdiv_k4, sse::fmrdiv_k4, 16));
        IF_ARCH_X86(CALL(generic::fmmod_k4, sse2::fmmod_k4, 16));
        IF_ARCH_X86(CALL(generic::fmrmod_k4, sse2::fmrmod_k4, 16));

        IF_ARCH_X86(CALL(generic::fmadd_k4, avx::fmadd_k4, 32));
        IF_ARCH_X86(CALL(generic::fmsub_k4, avx::fmsub_k4, 32));
        IF_ARCH_X86(CALL(generic::fmrsub_k4, avx::fmrsub_k4, 32));
        IF_ARCH_X86(CALL(generic::fmmul_k4, avx::fmmul_k4, 32));
        IF_ARCH_X86(CALL(generic::fmdiv_k4, avx::fmdiv_k4, 32));
        IF_ARCH_X86(CALL(generic::fmrdiv_k4, avx::fmrdiv_k4, 32));
        IF_ARCH_X86(CALL(generic::fmmod_k4, avx::fmmod_k4, 32));
        IF_ARCH_X86(CALL(generic::fmrmod_k4, avx::fmrmod_k4, 32));
        IF_ARCH_X86(CALL(generic::fmadd_k4, avx::fmadd_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmsub_k4, avx::fmsub_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmrsub_k4, avx::fmrsub_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmmod_k4, avx::fmmod_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmrmod_k4, avx::fmrmod_k4_fma3, 32));

        IF_ARCH_X86(CALL(generic::fmadd_k4, avx2::fmadd_k4, 32));
        IF_ARCH_X86(CALL(generic::fmsub_k4, avx2::fmsub_k4, 32));
        IF_ARCH_X86(CALL(generic::fmrsub_k4, avx2::fmrsub_k4, 32));
        IF_ARCH_X86(CALL(generic::fmmul_k4, avx2::fmmul_k4, 32));
        IF_ARCH_X86(CALL(generic::fmdiv_k4, avx2::fmdiv_k4, 32));
        IF_ARCH_X86(CALL(generic::fmrdiv_k4, avx2::fmrdiv_k4, 32));
        IF_ARCH_X86(CALL(generic::fmmod_k4, avx2::fmmod_k4, 32));
        IF_ARCH_X86(CALL(generic::fmrmod_k4, avx2::fmrmod_k4, 32));
        IF_ARCH_X86(CALL(generic::fmadd_k4, avx2::fmadd_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmsub_k4, avx2::fmsub_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmrsub_k4, avx2::fmrsub_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmmod_k4, avx2::fmmod_k4_fma3, 32));
        IF_ARCH_X86(CALL(generic::fmrmod_k4, avx2::fmrmod_k4_fma3, 32));

        IF_ARCH_ARM(CALL(generic::fmadd_k4, neon_d32::fmadd_k4, 16));
        IF_ARCH_ARM(CALL(generic::fmsub_k4, neon_d32::fmsub_k4, 16));
        IF_ARCH_ARM(CALL(generic::fmrsub_k4, neon_d32::fmrsub_k4, 16));
        IF_ARCH_ARM(CALL(generic::fmmul_k4, neon_d32::fmmul_k4, 16));
        IF_ARCH_ARM(CALL(generic::fmdiv_k4, neon_d32::fmdiv_k4, 16));
        IF_ARCH_ARM(CALL(generic::fmrdiv_k4, neon_d32::fmrdiv_k4, 16));
        IF_ARCH_ARM(CALL(generic::fmmod_k4, neon_d32::fmmod_k4, 16));
        IF_ARCH_ARM(CALL(generic::fmrmod_k4, neon_d32::fmrmod_k4, 16));

        IF_ARCH_AARCH64(CALL(generic::fmadd_k4, asimd::fmadd_k4, 16));
        IF_ARCH_AARCH64(CALL(generic::fmsub_k4, asimd::fmsub_k4, 16));
        IF_ARCH_AARCH64(CALL(generic::fmrsub_k4, asimd::fmrsub_k4, 16));
        IF_ARCH_AARCH64(CALL(generic::fmmul_k4, asimd::fmmul_k4, 16));
        IF_ARCH_AARCH64(CALL(generic::fmdiv_k4, asimd::fmdiv_k4, 16));
        IF_ARCH_AARCH64(CALL(generic::fmrdiv_k4, asimd::fmrdiv_k4, 16));
        IF_ARCH_AARCH64(CALL(generic::fmmod_k4, asimd::fmmod_k4, 16));
        IF_ARCH_AARCH64(CALL(generic::fmrmod_k4, asimd::fmrmod_k4, 16));
    }
UTEST_END


