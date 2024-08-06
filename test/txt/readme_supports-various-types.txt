
// Basic Type
[0m[02m[dump] [0m[36mfalse[0m[02m => [0mfalse[0m[02m, [0m[36m0[0m[02m => [0m0[0m[02m, [0m[36m0.0[0m[02m => [0m0.000000[0m
[0m[02m[dump] [0m[36mtrue[0m[02m => [0mtrue[0m[02m, [0m[36m3.14[0m[02m => [0m3.140000[0m[02m, [0m[36mmy_int[0m[02m => [0m15[0m[02m, [0m[36m-9265[0m[02m => [0m-9265[0m
[0m[02m[dump] [0m[36m"This is a string."[0m[02m => [0m"This is a string.[0m"[0m[02m, [0m[36m'a'[0m[02m => [0m'a'[0m[02m, [0m[36m'\n'[0m[02m => [0m'[0m[02m\n[0m'[0m
[0m[02m[dump] [0m[36mptr[0m[02m => [0m[32m*[0m15[0m[02m, [0m[36mvoid_ptr[0m[02m => [0m[32m0x7fffffffffff[0m[02m, [0m[36mnullptr[0m[02m => [0mnullptr[0m

// Container
[0m[02m[dump] [0m[36mmy_vector[0m[02m => [0m[02m[[0m
         [02m[ [0m3[0m[02m, [0m5[0m[02m, [0m8[0m[02m, [0m9[0m[02m, [0m7[0m[02m ][0m[02m,[0m
         [02m[ [0m9[0m[02m, [0m3[0m[02m, [0m2[0m[02m, [0m3[0m[02m, [0m8[0m[02m ][0m
       [02m][0m

// Set/Map
[0m[02m[dump] [0m[36mmy_set[0m[02m => [0m[02m{ [0m'A'[0m[02m, [0m'e'[0m[02m, [0m'l'[0m[02m, [0m'p'[0m[02m }[0m
[0m[02m[dump] [0m[36mmy_map[0m[02m => [0m[02m{ [0m2[0m[02m: [0m6[0m[02m, [0m4[0m[02m: [0m6[0m[02m, [0m5[0m[02m: [0m3[0m[02m }[0m

// Multiset/Multimap
[0m[02m[dump] [0m[36mmy_multiset[0m[02m => [0m[02m{ [0m'A'[0m[36m (1)[0m[02m, [0m'e'[0m[36m (1)[0m[02m, [0m'l'[0m[36m (1)[0m[02m, [0m'p'[0m[36m (2)[0m[02m }[0m
[0m[02m[dump] [0m[36mmy_multimap[0m[02m => [0m[02m{[0m
         2[0m[36m (1)[0m[02m: [0m[02m[ [0m4[0m[02m ][0m[02m,[0m
         4[0m[36m (2)[0m[02m: [0m[02m[ [0m6[0m[02m, [0m7[0m[02m ][0m[02m,[0m
         5[0m[36m (1)[0m[02m: [0m[02m[ [0m3[0m[02m ][0m
       [02m}[0m

// Tuple
[0m[02m[dump] [0m[36mmy_tuple[0m[02m => [0m[02m( [0m7[0m[02m, [0m4.500000[0m[02m, [0m"This is a string.[0m"[0m[02m )[0m
[0m[02m[dump] [0m[36mmy_pair[0m[02m => [0m[02m( [0m8[0m[02m, [0m'a'[0m[02m )[0m

// FIFO/LIFO
[0m[02m[dump] [0m[36mmy_queue[0m[02m => [0m[32mstd::queue[0m[02m{ [0m[36mfront()[0m[02m= [0m1[0m[02m, [0m[36mback()[0m[02m= [0m5[0m[02m, [0m[36msize()[0m[02m= [0m5[0m[02m }[0m
[0m[02m[dump] [0m[36mmy_priority_queue[0m[02m => [0m[32mstd::priority_queue[0m[02m{ [0m[36mtop()[0m[02m= [0m5[0m[02m, [0m[36msize()[0m[02m= [0m5[0m[02m }[0m
[0m[02m[dump] [0m[36mmy_stack[0m[02m => [0m[32mstd::stack[0m[02m{ [0m[36mtop()[0m[02m= [0m5[0m[02m, [0m[36msize()[0m[02m= [0m5[0m[02m }[0m

// Other
[0m[02m[dump] [0m[36mmy_bitset[0m[02m => [0m[32m0b 0011 1010[0m
[0m[02m[dump] [0m[36mmy_complex[0m[02m => [0m[32m1.000000 + 1.000000i [0m[02m( [0m[36mabs[0m[02m= [0m1.414214[0m[02m, [0m[36marg/pi[0m[02m= [0m0.250000[0m[02m )[0m
[0m[02m[dump] [0m[36mmy_optional[0m[02m => [0m[32m?[0m15[0m[02m, [0m[36mstd::nullopt[0m[02m => [0m[32mstd::nullopt[0m
[0m[02m[dump] [0m[36mmy_variant[0m[02m => [0m[32m|[0m"1[0m"[0m

// Combination
[0m[02m[dump] [0m[36mvector_of_pairs[0m[02m => [0m[02m[[0m
         [02m( [0m1[0m[02m, [0m"apple[0m"[0m[02m )[0m[02m,[0m
         [02m( [0m3[0m[02m, [0m"banana[0m"[0m[02m )[0m
       [02m][0m

