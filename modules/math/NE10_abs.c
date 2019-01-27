/*
 *  Copyright 2011-16 ARM Limited and Contributors.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of ARM Limited nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY ARM LIMITED AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL ARM LIMITED AND CONTRIBUTORS BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * NE10 Library : math/NE10_abs.c
 */

#include "NE10_types.h"
#include "macros.h"

#include <assert.h>

#include <math.h>

ne10_result_t ne10_abs_float_c (ne10_float32_t * dst, ne10_float32_t * src, ne10_uint32_t count)
{
    NE10_CHECKPOINTER_DstSrc;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[itr] = fabs (src[itr]);
    }
    return NE10_OK;
}

ne10_result_t ne10_abs_vec2f_c (ne10_vec2f_t * dst, ne10_vec2f_t * src, ne10_uint32_t count)
{
    NE10_CHECKPOINTER_DstSrc;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].x = fabs (src[ itr ].x);
        dst[ itr ].y = fabs (src[ itr ].y);
    }
    return NE10_OK;
}

ne10_result_t ne10_abs_vec3f_c (ne10_vec3f_t * dst, ne10_vec3f_t * src, ne10_uint32_t count)
{
    NE10_CHECKPOINTER_DstSrc;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].x = fabs (src[ itr ].x);
        dst[ itr ].y = fabs (src[ itr ].y);
        dst[ itr ].z = fabs (src[ itr ].z);
    }
    return NE10_OK;
}

ne10_result_t ne10_abs_vec4f_c (ne10_vec4f_t * dst, ne10_vec4f_t * src, ne10_uint32_t count)
{
    NE10_CHECKPOINTER_DstSrc;
    for ( unsigned int itr = 0; itr < count; itr++ )
    {
        dst[ itr ].x = fabs (src[ itr ].x);
        dst[ itr ].y = fabs (src[ itr ].y);
        dst[ itr ].z = fabs (src[ itr ].z);
        dst[ itr ].w = fabs (src[ itr ].w);
    }
    return NE10_OK;
}
