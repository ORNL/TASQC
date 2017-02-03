/**----------------------------------------------------------------------------
Copyright (c) 2015-, UT-Battelle LLC
All rights reserved.

Authors: Jay Jay Billings, Phil Evans, Alex McCaskey
Author Contact: Phil Evans, evanspg@ornl.gov

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of keytransclient nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/

/**
 * Source certification:
 *
 * I wrote this test code for complementary multiply with carry based on
 * descriptions and source code from Wikipedia, Javamex, an archived email
 * from George Marsaglia and Numerical Recipes, 3rd Edition. Most of my
 * modifications minor additions to expand the period and make it testable with
 * the Diehard test suite. I originally compiled the code below from these
 * source on or around December 11th 2011.
 *
 * Jay Jay Billings
 * 20160713 10:28
 */

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Complementary Multiply-with-Carry Class
struct CMWC {

    // Array of last values
    const static int qLength = 4096;
    unsigned long long Q[4096];
    
    // Default CMWC "c" value
    int c;

    // Constructor, sets seed to 12345678 if seed comes in as zero
    CMWC(unsigned int seed) {

        // Make sure the seed is not zero, set it to something else
        if (seed == 0)
           seed = 12345678;

        // Setup rand and generate some initial values
        srand(seed);
        c = (long) rand();
        Q[0] = (long) rand();
        Q[1] = (long) rand();
        Q[2] = (long) rand();
        for (int i = 3; i < qLength; i++) {
            Q[i] = Q[i-3] ^ Q[i-2] ^ 0x9e3779b9 ^ i;
        }
 
    }

    // Create a random unsigned 64-bit integer using a complementary multiply-
    // with-carry generator with a base of b = pow(2,32)
    inline unsigned long long int64() {

        // Local Declarations
        unsigned long long t, a = 4294957665LL;
        static unsigned long i = qLength - 1;
        unsigned long x, r = 0xfffffffe;

        // Generate the random number
        i = (i+1) & (qLength - 1);
        t = a*Q[i] + c;
        c = (t>>32); 
        x = t + c; 
        if (x<c) {
           x++;
           c++;
        }

        // Return the number after storing it
        return (Q[i] = r - x);
    }
  
    // Return a double
    inline double doub() {
        return 5.42101086242752217E-20 * int64();
    }

    // Return a 32-bit integer
    inline unsigned int int32() {
        return (unsigned int) int64();
    }

};

// Main
int main(int argc, char **argv) {

    // Local Declarations
    clock_t endwait;
    int seconds = 5, seed = time(NULL);
    long totalGens = 0L;

    // Create the random number generator, seeded with the time converted to an
    // unsigned long integer
    CMWC ranGen = CMWC(seed);

    // Calculate the end time
    endwait = clock() + seconds * CLOCKS_PER_SEC;
    
    // Generate random numbers until the time is up
    while (clock() < endwait) {
        ranGen.int64();
        ++totalGens;
    }

    // Print the experiment data
    printf("Generated %ld random numbers in %d seconds.\n",totalGens,seconds);
    printf("Generation rate = %ld (ints/sec)\n",totalGens/(long) seconds);
    printf("Seed = %d \nFinished.\n",seed);

    return EXIT_SUCCESS;
}
