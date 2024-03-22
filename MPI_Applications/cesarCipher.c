#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <mpi.h>

#define MAX_LENGTH 100

// Function to encrypt the text
void encrypt(char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (islower(text[i])) {
            text[i] = (text[i] - 'a' + 3) % 26 + 'a';
        } else if (isupper(text[i])) {
            text[i] = (text[i] - 'A' + 3) % 26 + 'A';
        } else if (isdigit(text[i])) {
            text[i] = (text[i] - '0' + 3) % 10 + '0';
        } else {
            text[i] = text[i] + 3;
        }
    }
}

// Function to decrypt the text
void decrypt(char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (islower(text[i])) {
            text[i] = (text[i] - 'a' + 23) % 26 + 'a';
        } else if (isupper(text[i])) {
            text[i] = (text[i] - 'A' + 23) % 26 + 'A';
        } else if (isdigit(text[i])) {
            text[i] = (text[i] - '0' + 7) % 10 + '0';
        } else {
            text[i] = text[i] - 3;
        }
    }
}

int main(int argc, char **argv) {
    int myRank, p;
    char inputText[MAX_LENGTH];
    char outputText[MAX_LENGTH];
    int length, source, destination, tag = 0, choice;
    MPI_Status status; // MPI status variable
    MPI_Init(&argc, &argv); // Initialize MPI environment
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank); // Get process rank
    MPI_Comm_size(MPI_COMM_WORLD, &p); // Get total number of processes

    if (myRank == 0) { // Only execute by the Master process
        int inputOption;
        printf("Enter 0 to read input from a file or 1 to enter input from the console: ");
        fflush(stdout);
        scanf("%d", &inputOption);

        if (inputOption == 0) { // Read input from file
            char fileName[MAX_LENGTH];
            printf("Enter the file name (without .txt): ");
            fflush(stdout);
            scanf("%s", fileName);
            strcat(fileName, ".txt");
            FILE *file = fopen(fileName, "r");
            if (file == NULL) {
                printf("Error opening the file put the file in the same direction of c code.\n");
                MPI_Finalize();
                return 0;
            }

            fgets(inputText, MAX_LENGTH, file);
            fclose(file);
        } else if (inputOption == 1) { // Read input from console
            printf("Enter a string: ");
            fflush(stdout);
            scanf("%s", inputText);
        } else {
            printf("Invalid option.\n");
            MPI_Finalize();
            return 0;
        }

        printf("Enter 0 for encryption or 1 for decryption: ");
        fflush(stdout);
        scanf("%d", &choice);

        int len = (int)strlen(inputText);
        inputText[len] = '\0';

        for (int i = 1; i < p; i++) { // send data to other processes
            MPI_Send(&len, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(inputText, len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Send(&choice, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else { // Other processes receive data
        int len;
        MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); // Receive data from root process
        MPI_Recv(inputText, len, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&choice, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    length = (int)strlen(inputText);
    int localSize = length / p; // Calculate local size for each process
    int remainder = length % p;
    int start = myRank * localSize;
    int end = start + localSize;

    if (myRank == p - 1) { // Last process handles the remainder
        end += remainder;
    }

    char localText[localSize + remainder]; // Local text for each process
    int x = 0;

    for (int i = start; i < end; i++) { // Assign local text to each process
        localText[x++] = inputText[i];
    }

    localText[x] = '\0';

    if (choice == 0) // Encryption
        encrypt(localText);
    else if (choice == 1) // Decryption
        decrypt(localText);
    else { // Invalid option
        printf("Invalid option.\n");
        MPI_Finalize();
        return 0;
    }

    destination = 0;
    MPI_Send(&localText, localSize + remainder, MPI_CHAR, destination, tag, MPI_COMM_WORLD); // Send processed text to Master process

    if (myRank == 0) { // Master process
        strcpy(outputText, localText); // Copy local text to output text

        for (source = 1; source < p; source++) { // Receive processed text from other processes
            MPI_Recv(&localText, localSize + remainder, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
            strcat(outputText, localText); // Concatenate received text to output text
        }

        outputText[length] = '\0';
        printf("Processed text: %s\n", outputText);
    }

    MPI_Finalize(); // Finalize MPI environment
    return 0;
}