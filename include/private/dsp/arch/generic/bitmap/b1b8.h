/*
 * Copyright (C) 2023 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2023 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-dsp-lib
 * Created on: 8 апр. 2023 г.
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

#ifndef PRIVATE_DSP_ARCH_GENERIC_BITMAP_B1B8_H_
#define PRIVATE_DSP_ARCH_GENERIC_BITMAP_B1B8_H_

#ifndef PRIVATE_DSP_ARCH_GENERIC_IMPL
    #error "This header should not be included directly"
#endif /* PRIVATE_DSP_ARCH_GENERIC_IMPL */

namespace lsp
{
    namespace generic
    {

        void bitmap_put_b1b8(bitmap_t *dst, const bitmap_t *src, ssize_t x, ssize_t y)
        {
            bitmap_part_t part;
            if (!test_bitmap_put(&part, dst, src, x, y))
                return;

            uint8_t *dptr = &dst->data[part.dst_y * dst->stride + part.dst_x];
            const uint8_t *sptr = &src->data[part.src_x * src->stride];

            for (ssize_t y=0; y<part.count_y; ++y)
            {
                for (ssize_t x=0; x<part.count_x; ++x)
                {
                    size_t xx       = part.src_x + x;
                    uint8_t b       = sptr[xx >> 3];
                    size_t mask     = 0x80 >> (xx & 0x7);
                    dptr[x]         = (b & mask) ? 0xff : 0x00;
                }
                dptr       += dst->stride;
                sptr       += src->stride;
            }
        }

        void bitmap_add_b1b8(bitmap_t *dst, const bitmap_t *src, ssize_t x, ssize_t y)
        {
            bitmap_part_t part;
            if (!test_bitmap_put(&part, dst, src, x, y))
                return;

            uint8_t *dptr = &dst->data[part.dst_y * dst->stride + part.dst_x];
            const uint8_t *sptr = &src->data[part.src_y * src->stride];

            for (ssize_t y=0; y<part.count_y; ++y)
            {
                for (ssize_t x=0; x<part.count_x; ++x)
                {
                    size_t xx       = part.src_x + x;
                    uint8_t b       = sptr[xx >> 3];
                    size_t mask     = 0x80 >> (xx & 0x7);
                    int16_t res     = dptr[x] + ((b & mask) ? 0xff : 0x00);
                    dptr[x]         = (res > 0) ? 0xff : 0x00;
                }
                dptr       += dst->stride;
                sptr       += src->stride;
            }
        }

        void bitmap_sub_b1b8(bitmap_t *dst, const bitmap_t *src, ssize_t x, ssize_t y)
        {
            bitmap_part_t part;
            if (!test_bitmap_put(&part, dst, src, x, y))
                return;

            uint8_t *dptr = &dst->data[part.dst_y * dst->stride + part.dst_x];
            const uint8_t *sptr = &src->data[part.src_y * src->stride];

            for (ssize_t y=0; y<part.count_y; ++y)
            {
                for (ssize_t x=0; x<part.count_x; ++x)
                {
                    size_t xx       = part.src_x + x;
                    uint8_t b       = sptr[xx >> 3];
                    size_t mask     = 0x80 >> (xx & 0x7);
                    int16_t res     = dptr[x] - ((b & mask) ? 0xff : 0x00);
                    dptr[x]         = (res > 0) ? 0xff : 0x00;
                }
                dptr       += dst->stride;
                sptr       += src->stride;
            }
        }

        void bitmap_max_b1b8(bitmap_t *dst, const bitmap_t *src, ssize_t x, ssize_t y)
        {
            bitmap_part_t part;
            if (!test_bitmap_put(&part, dst, src, x, y))
                return;

            uint8_t *dptr = &dst->data[part.dst_y * dst->stride + part.dst_x];
            const uint8_t *sptr = &src->data[part.src_y * src->stride];

            for (ssize_t y=0; y<part.count_y; ++y)
            {
                for (ssize_t x=0; x<part.count_x; ++x)
                {
                    size_t xx       = part.src_x + x;
                    uint8_t b       = sptr[xx >> 3];
                    size_t mask     = 0x80 >> (xx & 0x7);
                    dptr[x]        |= ((b & mask) ? 0xff : 0x00);
                }
                dptr       += dst->stride;
                sptr       += src->stride;
            }
        }

        void bitmap_min_b1b8(bitmap_t *dst, const bitmap_t *src, ssize_t x, ssize_t y)
        {
            bitmap_part_t part;
            if (!test_bitmap_put(&part, dst, src, x, y))
                return;

            uint8_t *dptr = &dst->data[part.dst_y * dst->stride + part.dst_x];
            const uint8_t *sptr = &src->data[part.src_y * src->stride];

            for (ssize_t y=0; y<part.count_y; ++y)
            {
                for (ssize_t x=0; x<part.count_x; ++x)
                {
                    size_t xx       = part.src_x + x;
                    uint8_t b       = sptr[xx >> 3];
                    size_t mask     = 0x80 >> (xx & 0x7);
                    dptr[x]        &= ((b & mask) ? 0xff : 0x00);
                }
                dptr       += dst->stride;
                sptr       += src->stride;
            }
        }

    } /* namespace generic */
} /* namespaec lsp */


#endif /* PRIVATE_DSP_ARCH_GENERIC_BITMAP_B1B8_H_ */
