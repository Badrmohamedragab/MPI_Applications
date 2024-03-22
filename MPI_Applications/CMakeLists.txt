cmake_minimum_required(VERSION 3.27)
project(MPI_Applications)

set(CMAKE_C_STANDARD 23)
find_package(MPI REQUIRED)

add_executable(MPI_Applications cesarCipher.c)

add_executable(MPI_Applications2 countingPrimes.c)

target_link_libraries(MPI_Applications PUBLIC MPI::MPI_CXX)

target_link_libraries(MPI_Applications2 PUBLIC MPI::MPI_CXX)