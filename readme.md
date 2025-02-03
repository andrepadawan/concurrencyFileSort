Exercise 02: Concurrent file sorting

Implement a concurrent program with n threads that sorts an input file: the program must proceed in parallel on different files, according to the following specifications.

The program (which name pgrm) receives 3 parameters from command line: pgrm n strA strB.
Where:

n is an integer value
strA is a string that identifies the name of n input files of name 'strA1.txt, strA2.txt, …, strAn.txt'
strB is a string that identifies the name of n output files of name 'strB1.txt, strB2.txt, …, strBn.txt'
The input files strA contain:

in the first row, the number of integer numbers stored in the lines following the first
in the following rows, the integer numbers.
The following is an example of correct file:

5
102
99
34
234
25

The program, after the generation of the n names of the two input and output files, activates n threads.

Each thread:

reads from “its” input files the integer vector
sorts (with a sorting algorithm of your choice) the vector with a numeric sort, and in an increasing order, and saves the result in “its” corresponding output file.
Note that the program implements the following precedence graph:

    Mi---------
    /\        |  
   /  \       |
  R1    R2   ...
  |     |
  O1    O2   ...
  |     |
  W1    W2   ...
   \  /
    \/        |
    Mf---------
where:

Mi and Mf are the initial and final operations of the main
The flows Ri, Oi and Wi represent the execution of the threads, each of which
read its input file (Ri)
sort it (Oi)
save it in its output file (Wi).