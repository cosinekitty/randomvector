/*
    rvtest.h

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

#include <stdio.h>
#include "randomvector.h"

#define NDIMENSIONS 4

int main()
{
    int error;
    ruv_buffer_t buffer;
    int i;
    double vector[NDIMENSIONS];

    CHECK(ruv_init(&buffer));

    /* Generate 100 unit vectors in a 4-dimensional space. */
    for (i=0; i < 100; ++i)
    {
        CHECK(ruv_generate(&buffer, NDIMENSIONS, vector));
        printf("(%20.16lf, %20.16lf, %20.16lf, %20.16lf)\n", vector[0], vector[1], vector[2], vector[3]);
    }

    error = 0;
fail:
    ruv_free(&buffer);
    return error;
}