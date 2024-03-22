# MPI Applications: Caesar Cipher and Counting Primes

## Introduction

This project demonstrates the implementation of parallel algorithms using the Message Passing Interface (MPI). Two applications are included in this project:

1. **Caesar Cipher**: Encrypts or decrypts text using the Caesar Cipher algorithm in parallel.
2. **Counting Primes**: Counts the number of prime numbers within a given range in parallel.

The project showcases how parallelism can be leveraged to efficiently solve computational problems.

## Files Included

1. `caesar_cipher_mpi.c`: Contains the implementation of the Caesar Cipher encryption and decryption algorithms using MPI.
2. `count_primes_mpi.c`: Implements a parallel algorithm to count prime numbers within a given range using MPI.

## Compilation and Execution

To compile the programs, you need an MPI compiler such as `mpicc`.

```plaintext
mpicc -o caesar_cipher_mpi caesar_cipher_mpi.c
mpicc -o count_primes_mpi count_primes_mpi.c
```

To execute the programs, use `mpirun`.

```plaintext
mpirun -np <num_processes> caesar_cipher_mpi
mpirun -np <num_processes> count_primes_mpi
```

Replace `<num_processes>` with the desired number of MPI processes.

## Caesar Cipher MPI

### Overview

The Caesar Cipher MPI program encrypts or decrypts text using the Caesar Cipher algorithm in parallel. It divides the text among multiple MPI processes, each responsible for encrypting or decrypting a portion of the text.

### Usage

1. The program prompts the user to choose whether to input text from the console or a file.
2. If the user chooses to input text from a file, they need to provide the filename (without extension).
3. Next, the user selects encryption or decryption.
4. The program then distributes the text among MPI processes, performs the encryption or decryption operation, and gathers the results to reconstruct the processed text.
5. Finally, the program displays the processed text.

## Counting Primes MPI

### Overview

The Counting Primes MPI program counts the number of prime numbers within a given range using parallel processing. It distributes the range among MPI processes, with each process responsible for counting primes within a subrange.

### Usage

1. The user inputs the range of numbers (start and end) for prime number counting.
2. The master process distributes the range among MPI processes.
3. Each process counts primes within its subrange and sends the count back to the master process.
4. The master process accumulates the counts from all processes to compute the total number of prime numbers.
5. Finally, the program displays the total count of prime numbers.

## Conclusion

This project demonstrates the effectiveness of parallel computing using MPI for solving computational problems. It provides insights into leveraging parallelism to enhance performance in various applications. Feel free to explore and modify the provided code for your specific needs.

**Note:** Ensure you have MPI installed and configured properly on your system to execute these programs.

For any issues or suggestions, please contact the project maintainers. Thank you for using this MPI applications repository!
