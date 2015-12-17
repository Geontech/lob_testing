/*
 * Copyright 2011, 2013 Hong Xu. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY HONG XU ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL HONG XU OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Hong Xu.
 *
 *
 *
 * Purpose: Peak detection in a wave.
 *
 * Author: Hong Xu <hong@hong.me>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace peakdetect {

void print_help(void)
{
    fprintf(stderr,
            "Usage: peakdetect [OPTIONS]\n"
            "Peak detection in a wave\n"
            "\n"
            "Options:\n"
            "-i inputfile \t\tInput file.\n"
            "             \t\tThe input file should be a csv format file, "
            "whose first\n"
            "             \t\tcolumn is X and second column is Y.\n"
            "-o outfile   \t\tOutput file.\n"
            "             \t\tEmission peaks will be output first, "
            "followed by\n"
            "             \t\tabsorption peaks with an empty line seperated."
            "\n"
            "-d deltavalue\t\tDelta, a parameter used to determine peaks.\n"
            "-m mode      \t\tDetecting mode, "
            "could be either \"a\" (detect absorption peak\n"
            "             \t\tfirst) or \"e\" (detect emission peak first).\n"
            "             \t\tDefault value is \"a\".\n"
            "--version    \t\tDisplay version information.\n"
            "--help       \t\tShow this help information.\n"
            "\n"
            "e.g.\n"
            "peakdetect -i input.csv -o output.csv -d 1e-7 -m a\n"
            "peakdetect <input.csv -d 0.1 -m e | tee out.csv\n");
    exit(0);
}

void print_version(void)
{
    fprintf(stderr,
            "peakdetect version 0.1.3\n"
            "Copyright (C) 2011 Hong Xu <xuphys@gmail.com>\n"
            "Originally inspired by Eli Billauer\'s peakdet for MATLAB:\n"
            "http://billauer.co.il/peakdet.html\n"
            "\n"
            "See the README file for license information.\n");
    exit(0);
}

int detect_peak(
        const double*   data, /* the data */ 
        int             data_count, /* row count of data */ 
        int*            emi_peaks, /* emission peaks will be put here */ 
        int*            num_emi_peaks, /* number of emission peaks found */
        int             max_emi_peaks, /* maximum number of emission peaks */ 
        int*            absop_peaks, /* absorption peaks will be put here */ 
        int*            num_absop_peaks, /* number of absorption peaks found */
        int             max_absop_peaks, /* maximum number of absorption peaks
                                            */ 
        double          delta, /* delta used for distinguishing peaks */
        int             emi_first /* should we search emission peak first of
                                     absorption peak first? */
        )
{
    int     i;
    double  mx;
    double  mn;
    int     mx_pos = 0;
    int     mn_pos = 0;
    int     is_detecting_emi = emi_first;


    mx = data[0];
    mn = data[0];

    *num_emi_peaks = 0;
    *num_absop_peaks = 0;

    for(i = 1; i < data_count; ++i)
    {
        if(data[i] > mx)
        {
            mx_pos = i;
            mx = data[i];
        }
        if(data[i] < mn)
        {
            mn_pos = i;
            mn = data[i];
        }

        if(is_detecting_emi &&
                data[i] < mx - delta)
        {
            if(*num_emi_peaks >= max_emi_peaks) /* not enough spaces */
                return 1;

            emi_peaks[*num_emi_peaks] = mx_pos;
            ++ (*num_emi_peaks);

            is_detecting_emi = 0;

            i = mx_pos - 1;

            mn = data[mx_pos];
            mn_pos = mx_pos;
        }
        else if((!is_detecting_emi) &&
                data[i] > mn + delta)
        {
            if(*num_absop_peaks >= max_absop_peaks)
                return 2;

            absop_peaks[*num_absop_peaks] = mn_pos;
            ++ (*num_absop_peaks);

            is_detecting_emi = 1;
            
            i = mn_pos - 1;

            mx = data[mn_pos];
            mx_pos = mn_pos;
        }
    }

    return 0;
}

}
