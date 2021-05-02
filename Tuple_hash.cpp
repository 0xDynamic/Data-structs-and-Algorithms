/*
    An interface that allows us to use "tuples" of integers as keys in std::unordered_map. This
basic implementation was written mainly for usage in Competitive Programming contests, and has
limited uses beyond such scope owing to the potential issues with integer overflow.
    In the context of this "hash" function, a "tuple" is defined implicitly using variable length
arguments. To support actual tuples, an auxiliary interface is needed to unpack the contents of
said tuple and pipe them into this function.
The idea:
    - Represent an N-tuple of integers T as a 64-bit key K.
    - Internally subdivide key K into N ordered groups of 64/N-bit segments.
    - Each 64/N-bit segment represents one element in tuple T.
Implementation notes:
    - For an N-element tuple, each element must be in the range [-2^(64/N-1), +2^(64/N-1)-1]
      (2's complement signed integer).
        - For a tuple of four integers, each integer must be representable with 64/4 = 16 bits.
        - For a tuple of seven integers, each integer must be representable with 64/7 = 9 bits.
    - Each element will be offset by 2^(64/N-1) so that the final range is [0, 2^(64/N)-1].
        - For an 8-element tuple where each segment covers a range of [-128, 127], each element
          is offset by 2^7 such that the final range is [0, 255].
        - This integer-to-unsigned integer transformation keeps element representations compact,
          thereby preventing bit-overriding/ corruption.
Example:
    - Consider a tuple T containing 4 integers (32767, 17, -88, -32768).
    - The corresponding key K will contain four 16-bit segments covering bits [0, 15], [16, 31],
      [32, 47] and [48, 63].
    Converstion (2's Complement):
        [dec->bin]  +32767 =  (00...)0111111111111111
        [dec->bin]  +17    =  (00...)0000000000010001
        [dec->bin]  -88    =  (11...)1111111110101000
        [dec->bin]  -32768 =  (11...)1000000000000000
    [ Without Offset ]
    Tuple  T  = ( 32767           ,  17              ,  -88             ,  -32768           )
    Key    K  = [ 0111111111111111 | 0000000000010001 | 1111111110101000 | 1000000000000000 ]
    [ Adding Offset ]
    Tuple  T  = ( 65535           ,  32785           ,  32680           ,  0                )
    Key    K  = [ 1111111111111111 | 1000000000010001 | 0111111110101000 | 0000000000000000 ]
    -----------------------------------------------------------------------------------------
    MSB(K)    =   0 -> 1             0 -> 1             1 -> 0             1 -> 0
*/

#include <bits/stdc++.h>
using namespace std;

typedef uint64_t hash_key;

/**
 * Generates a unique 64-bit key K for a tuple of integers.
 *
 * The same tuple will always generate the same key, and no two unique N-tuples
 * will generate a common key. It is, however, possible for an N-tuple T_n and
 * an M-tuple T_m to generate a common key if N != M.
 *
 * For any given N-tuple of integers, each integer has to be within a certain
 * (signed) range depending on the value of N. The details are provided below:
 *
 * N-tuple =    Range =                    |  Segment width =
 * ---------    -------------------------  |  ---------------
 * 2 -Tuple     [-2147483648, 2147483647]  |  32 bits
 * 3 -Tuple     [-1048576   , 1048575   ]  |  21 bits
 * 4 -Tuple     [-32768     , 32767     ]  |  16 bits
 * 5 -Tuple     [-2048      , 2047      ]  |  12 bits
 * 6 -Tuple     [-512       , 511       ]  |  10 bits
 * 7 -Tuple     [-256       , 255       ]  |  9  bits
 * 8 -Tuple     [-128       , 127       ]  |  8  bits
 * 9 -Tuple     [-64        , 63        ]  |  7  bits
 * 10-Tuple     [-32        , 31        ]  |  6  bits
 *
 * @param the number of elements in a tuple, followed by their values.
 * @return a uint64_t value representing key K.
 */
hash_key encode(int num_args, ...) {
    int segment_len = 64 / num_args;                       // Based on N, the tuple size,
    int offset = (1 << (segment_len - 1));                 // calculate the corresponding offset.

    va_list  tuple_obj;
    va_start (tuple_obj, num_args);

    hash_key K = 0;

    for (int i = 0; i < num_args; i += 1) {
        K <<= segment_len;

        int element = va_arg(tuple_obj, int);              // Translates each element by the offset
        element += offset;                                 // to make it "unsigned".

        if (element != 0) {                                // __builtin_clz(0) is undefined.
            int sz = sizeof(element) * 8;
            int packet_len = sz - __builtin_clz(element);  // Given N, verify that each element is
            assert(packet_len <= segment_len);             // within the valid range.
        }

        K |=  element;
    }

    va_end(tuple_obj);
    return K;
}

int main() {
    unordered_map<hash_key, int> hash_table;

    hash_key K = 0;

    K = encode(2, 22, 95);            // Mapping the 2-tuple (22, 95)
    hash_table[K] = 542;              // to a value of 542.

    K = encode(3, -234, 14366, 7);    // Mapping the 3-tuple (-234, 14366, 7)
    hash_table[K] = -3;               // to a value of -3.

    K = encode(4, -2, 146, 0, 31234); // Mapping the 4-tuple (-2, 146, 0, 31234)
    hash_table[K] = 19245;            // to a value of 19245.

    return 0;
}