/*
    randomvector.c

    https://github.com/cosinekitty/randomvector

    MIT License

    Copyright (c) 2020 Don Cross <cosinekitty@gmail.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "randomvector.h"

#ifdef __cplusplus
extern "C" {
#endif

static const double TWO_PI = 6.283185307179586476925287;

#define BUFFER_LENGTH   1024

static const char *RandomFileName = "/dev/urandom";

int ruv_init(ruv_buffer_t *buffer)
{
    int error;
    memset(buffer, 0, sizeof(ruv_buffer_t));

    buffer->file = fopen(RandomFileName, "rb");
    if (buffer->file == NULL)
        FAIL("ruv_init: cannot open random device %s\n", RandomFileName);

    buffer->array = calloc(BUFFER_LENGTH, sizeof(buffer->array[0]));
    if (buffer->array == NULL)
        FAIL("ruv_init: cannot allocate memory\n");

    /* Cause first call to ruv_uniform() to trigger a read. */
    buffer->offset = BUFFER_LENGTH;

    error = 0;
fail:
    if (error != 0)
        ruv_free(buffer);

    return error;
}


int ruv_uniform(ruv_buffer_t *buffer, double *x)
{
    int error;

    /* Keep reading until the resulting random number is not zero. */
    do
    {
        if (buffer->offset == BUFFER_LENGTH)
        {
            /* We need to load a new batch of random 64-bit unsigned integers. */
            size_t nread = fread(buffer->array, sizeof(buffer->array[0]), BUFFER_LENGTH, buffer->file);
            if (nread != BUFFER_LENGTH)
                FAIL("ruv_uniform: Error reading from device %s\n", RandomFileName);
            buffer->offset = 0;
        }

        *x = (double)buffer->array[buffer->offset++] / (double)0xffffffffffffffffLU;
    } while (*x == 0.0);

    /* Now *x contains a uniformly-distributed random value in the half-open range (0, 1]. */
    error = 0;
fail:
    return error;
}


int ruv_generate(ruv_buffer_t *buffer, int ndimensions, double vector[])
{
    int error, i;
    double A, B;    /* uniform random variables */
    double radius, angle;
    double sum;

    do
    {
        sum = 0.0;
        for (i=0; i < ndimensions; i += 2)
        {
            CHECK(ruv_uniform(buffer, &A));
            CHECK(ruv_uniform(buffer, &B));
            radius = sqrt(-2 * log(A));
            angle = TWO_PI * B;
            vector[i] = radius * cos(angle);
            sum += vector[i] * vector[i];
            if (i+1 < ndimensions)
            {
                vector[i+1] = radius * sin(angle);
                sum += vector[i+1] * vector[i+1];
            }
        }
    } while (sum == 0.0);   /* While extremely unlikely, it is possible to pick a zero vector. AVOID! */

    /* Convert to a unit vector by dividing through by the length. */
    sum = sqrt(sum);
    for (i=0; i < ndimensions; ++i)
        vector[i] /= sum;

    error = 0;
fail:
    return error;
}


void ruv_free(ruv_buffer_t *buffer)
{
    if (buffer->file != NULL)
    {
        fclose(buffer->file);
        buffer->file = NULL;
    }

    if (buffer->array != NULL)
    {
        free(buffer->array);
        buffer->array = NULL;
    }
}

#ifdef __cplusplus
}
#endif
