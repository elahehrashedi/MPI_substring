# MPI_substring
Given two character strings s1 and s2, using C and MPI to write parallel programs to find the number of substrings, in string s1, that are exactly the same as s2.

The strings are ended with ‘\0’. For example, suppose number_substring(s1, s2) implements the function, then number_substring(“abcdab”, “ab”) = 2, number_substring(“aaa”, “a”) = 3,
number_substring(“abac”, “bc”) = 0. Suppose the size of s1 and s2 are n1 and n2,
respectively, and p processes are used, we assume that n2 < n1/nprocs. However, n1 is
not necessary a multiple of nprocs .

In your program, one process (e.g. process 0) opens a file named “strings.txt” and read
two strings, s1 and s2, respectively.

Then it first evenly partitions s1 into segments and sends respective segments as well as
the entire s2 to each of the other processes. Then you have the initial data arrangement
similar to the one seen as below. After this more data communications may be needed to
allow each process to carry out its assigned work. After each process completes its work, process 0 collects the results from the other processes and prints the number of substrings
that are matched.
The format of file ‘strings.txt’ is like this (the first string is s1 and the second one is s2):

       abcassghbca
       bca

As an example, after the initial data arrangement among three processes, these processes
own the data in this way:

P0        P1      P2
abca   ssgh    bca
bca     bca     bca

