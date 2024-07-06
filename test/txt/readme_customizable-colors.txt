
// Basic Type
[0m[02m[dump] [0m[34mfalse[0m[02m => [0m[36mfalse[0m[02m, [0m[34m0[0m[02m => [0m[36m0[0m[02m, [0m[34m0.0[0m[02m => [0m[36m0.000000[0m[02m, [0m[34m'0'[0m[02m => [0m[36m'0'[0m
[0m[02m[dump] [0m[34mtrue[0m[02m => [0m[36mtrue[0m[02m, [0m[34m3.14[0m[02m => [0m[36m3.140000[0m[02m, [0m[34mmy_int[0m[02m => [0m[36m15[0m[02m, [0m[34m9265[0m[02m => [0m[36m9265[0m
[0m[02m[dump] [0m[34m"This is a string."[0m[02m => [0m[36m"This is a string.[0m[36m"[0m
[0m[02m[dump] [0m[34mptr[0m[02m => [0m[32m*[0m[36m15[0m[02m, [0m[34mvoid_ptr[0m[02m => [0m[32m[0m[02m, [0m[34mnullptr[0m[02m => [0m[36mnullptr[0m

// Container
[0m[02m[dump] [0m[34mmy_vector[0m[02m => [0m[33m[[0m
         [35m[ [0m[36m3[0m[02m, [0m[36m5[0m[02m, [0m[36m8[0m[02m, [0m[36m9[0m[02m, [0m[36m7[0m[35m ][0m[02m,[0m
         [35m[ [0m[36m9[0m[02m, [0m[36m3[0m[02m, [0m[36m2[0m[02m, [0m[36m3[0m[02m, [0m[36m8[0m[35m ][0m
       [33m][0m

// Set/Map
[0m[02m[dump] [0m[34mmy_set[0m[02m => [0m[33m{ [0m[36m'A'[0m[02m, [0m[36m'e'[0m[02m, [0m[36m'l'[0m[02m, [0m[36m'p'[0m[33m }[0m
[0m[02m[dump] [0m[34mmy_map[0m[02m => [0m[33m{ [0m[36m2[0m[02m: [0m[36m6[0m[02m, [0m[36m4[0m[02m: [0m[36m6[0m[02m, [0m[36m5[0m[02m: [0m[36m3[0m[33m }[0m

// Multiset/Multimap
[0m[02m[dump] [0m[34mmy_multiset[0m[02m => [0m[33m{ [0m[36m'A'[0m[36m (1)[0m[02m, [0m[36m'e'[0m[36m (1)[0m[02m, [0m[36m'l'[0m[36m (1)[0m[02m, [0m[36m'p'[0m[36m (2)[0m[33m }[0m
[0m[02m[dump] [0m[34mmy_multimap[0m[02m => [0m[33m{[0m
         [36m2[0m[36m (1)[0m[02m: [0m[35m[ [0m[36m4[0m[35m ][0m[02m,[0m
         [36m4[0m[36m (2)[0m[02m: [0m[35m[ [0m[36m6[0m[02m, [0m[36m7[0m[35m ][0m[02m,[0m
         [36m5[0m[36m (1)[0m[02m: [0m[35m[ [0m[36m3[0m[35m ][0m
       [33m}[0m

// Tuple
[0m[02m[dump] [0m[34mmy_tuple[0m[02m => [0m[33m( [0m[36m7[0m[02m, [0m[36m4.500000[0m[02m, [0m[36m"This is a string.[0m[36m"[0m[33m )[0m
[0m[02m[dump] [0m[34mmy_pair[0m[02m => [0m[33m( [0m[36m8[0m[02m, [0m[36m'a'[0m[33m )[0m

// FIFO/LIFO
[0m[02m[dump] [0m[34mmy_queue[0m[02m => [0m[32mstd[0m[02m::[0m[32mqueue[0m[33m{ [0m[36mfront()[0m[02m= [0m[36m1[0m[02m, [0m[36mback()[0m[02m= [0m[36m5[0m[02m, [0m[36msize()[0m[02m= [0m[36m5[0m[33m }[0m
[0m[02m[dump] [0m[34mmy_priority_queue[0m[02m => [0m[32mstd[0m[02m::[0m[32mpriority_queue[0m[33m{ [0m[36mtop()[0m[02m= [0m[36m5[0m[02m, [0m[36msize()[0m[02m= [0m[36m5[0m[33m }[0m
[0m[02m[dump] [0m[34mmy_stack[0m[02m => [0m[32mstd[0m[02m::[0m[32mstack[0m[33m{ [0m[36mtop()[0m[02m= [0m[36m5[0m[02m, [0m[36msize()[0m[02m= [0m[36m5[0m[33m }[0m

// Other
[0m[02m[dump] [0m[34mmy_bitset[0m[02m => [0m[32m0b 0011 1010[0m
[0m[02m[dump] [0m[34mmy_complex[0m[02m => [0m[32m1.000000[0m [32m+[0m [32m1.000000i [0m[33m( [0m[36mabs[0m[02m= [0m[36m1.414214[0m[02m, [0m[36marg/pi[0m[02m= [0m[36m0.250000[0m[33m )[0m
[0m[02m[dump] [0m[34mmy_optional[0m[02m => [0m[32m?[0m[36m15[0m[02m, [0m[34mstd::nullopt[0m[02m => [0m[32mstd[0m[02m::[0m[32mnullopt[0m
[0m[02m[dump] [0m[34mmy_variant[0m[02m => [0m[32m|[0m[36m"1[0m[36m"[0m

// Combination
[0m[02m[dump] [0m[34mvector_of_pairs[0m[02m => [0m[33m[[0m
         [35m( [0m[36m1[0m[02m, [0m[36m"apple[0m[36m"[0m[35m )[0m[02m,[0m
         [35m( [0m[36m3[0m[02m, [0m[36m"banana[0m[36m"[0m[35m )[0m
       [33m][0m

