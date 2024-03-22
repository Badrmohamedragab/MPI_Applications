#include <stdio.h>
#include <mpi.h>

// Function to check if a number is prime or not
int is_prime(int num) {
    if (num <= 1)
        return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

// Function to count prime numbers in a range
int count_primes(int start, int end) {
    int count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i))
            count++;
    }
    return count;
}

int main(int argc, char *argv[]) {
    int myRank, p;
    int x, y;
    int total_count = 0, sub_count;

    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &p); // Get total number of processes

    if (myRank == 0) { // Master process
        printf("Enter the value of x: ");
        fflush(stdout);
        scanf("%d", &x);

        printf("Enter the value of y: ");
        fflush(stdout);
        scanf("%d", &y);

        // Distribute input range to slave processes
        for (int i = 1; i < p; i++) {
            MPI_Send(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&y, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else { // Slave processes
        // Receive input range from master process
        MPI_Recv(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Calculate subrange for each process
        int subRange = (y - x + 1) / (p - 1);
        int lower_bound = x + (myRank - 1) * subRange;
        int upper_bound = (myRank == p - 1) ? y : lower_bound + subRange - 1;

        // Count prime numbers in the subrange
        sub_count = count_primes(lower_bound, upper_bound);

        // Send count of prime numbers to master process
        printf("Total number of prime numbers in rank %d is: %d\n", myRank, sub_count);
        MPI_Send(&sub_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (myRank == 0) { // Master process
        // Receive count of prime numbers from each slave process and accumulate
        for (int i = 1; i < p; i++) {
            MPI_Recv(&sub_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_count += sub_count;
        }

        printf("Total number of prime numbers is: %d\n", total_count);
    }

    MPI_Finalize(); // Finalize MPI environment

    return 0;
}
