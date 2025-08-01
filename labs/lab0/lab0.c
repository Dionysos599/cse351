/*
  *** CSE 351 Lab 0 ***
  Learning Objectives:
  Familiarize yourself with the workflow to modify, compile,
  and execute source files on a Linux environment.
  This lab also will preview some of the major concepts we
  will cover later in this course!
  - Part 1: Data (Pointers, Numbers, and Bits)
  - Part 2: Arrays
  - Part 3: Structs
  - Part 4: Performance
  - Part 5: Memory Allocation
  Motivation: Most of the code behaviors will seem inexplicable
              at this point, but our goal is that you will be
              able to explain to someone else what is going on
              by the end of this course! :D
*/


// These #includes tell the compiler to include the named
// header files, similar to imports in Java.  The functions
// mentioned below are used in our code.  Functions are
// like Java static methods that are not in a class.
//   assert.h - contains declaration of assert()
//   stdio.h  - contains declaration of printf()
//   stdlib.h - contains declaration of malloc() and free()
//   time.h   - contains declaration of clock()
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// This #define tells the C preprocessor to do a straight
// substitution of instances of the text "SIZE" in the code
// below with the text "500". This example acts like a
// global variable without actually allocating memory for
// a variable.
#define SIZE 500


/******************************************************
*   LOOK AT MAIN() AT THE BOTTOM OF THIS FILE FIRST   *
*******************************************************/


// ------------------------------------------- //
// PART 1 - Data (Pointers, Numbers, and Bits) //
// ------------------------------------------- //
//
// All data are internally stored as bits (binary digits).
// Data stored in memory (RAM) are associated with addresses.
// Addresses can be stored in variables called pointers.
void part_1() {
    printf("*** LAB 0 PART 1 ***\n");

    // C has different integer datatypes, which determine
    // the bit width of the binary representation. Here we
    // use 'int', which is usually 32 bits wide, depending
    // on the system and programming language.
    int x = 351, y = 410;

    // Printf is a library function to print formatted
    // output to the console. It will interpret stored bits
    // based on the format specifiers you provide it:
    //  - http://www.cplusplus.com/reference/cstdio/printf/
    printf("x = %i\ny = %i\n", x, y);

    // The following line creates integer pointers p and q
    // and stores the addresses of x and y, respectively,
    // into them.
    int *p = &x, *q = &y;
    printf("p = %p\nq = %p\n", p, q);

    // Q1: The following line is an example. Feel free to
    // copy and/or modify it as needed for part 1 of this lab.
    printf("x & x = %i\n", x & x);
    printf("p - q = %li\n", p - q);
    printf("351 & 410 = %i\n", x & y);
    printf("~1 = %i\n", ~1);
}


// --------------- //
// PART 2 - Arrays //
// --------------- //
//
// PART 2 - HELPER FUNCTION - fill_array()
//
// Fill a given array with values according to a formula.
// C doesn't keep track of the length of arrays, so we have
// to specify it as an explicit parameter (len), rather than
// looking it up from the array as in Java.
// The type of the array parameter here is int*, a pointer
// to an int. We'll learn more about why int* is used here,
// but for now it is enough to understand that "array" is an
// array of ints.
void fill_array(int* array, int len) {
    printf("Filling an array at address %p with %d "
        "values\n", array, len);
    for (int i = 0; i < len; ++i) {
        array[i] = i * 3 + 2;
        // assert() verifies that the given condition is true
        // and exits the program otherwise. This is just a
        // "sanity check" to make sure that the line of code
        // above is doing what we intend.
        assert(array[i] == i * 3 + 2);
    }
    printf("Done!\n");
}


// PART 2 - Arrays
//
// Create an array and fill it with values using the
// fill_array() function defined above. Learn about array
// representation in memory and (lack of) boundaries.
void part_2() {
    printf("*** LAB 0 PART 2 ***\n");

    // This is a block of memory big enough to store 10
    // ints. The name "array" here actually refers to the
    // address of this block of memory.
    // array[0] is the first int in this block of memory,
    // array[1] is the second, and so on. C does not track
    // or check array lengths, so it is up to us to know how
    // many elements the array contains.
    int array[10];

    // Q2.1: What happens if the second argument is greater
    // than the size of the array (10)?
    fill_array(array, 10);

    int value = 351;

    // In C, we can take the address of something using the
    // & operator. &value is of the type int*, meaning that
    // it is a pointer to an integer (it stores the address
    // in memory of where the actual int is located).

    // Q2.2: Here we use the address of the variable "value"
    // as if it were an array of a single element. What data
    // is stored in value after the following code executes?
    // Think about why the result is what it is.
    fill_array(&value, 1);
    printf("value = %d\n", value);
}


// ---------------- //
// PART 3 - Structs //
// ---------------- //
//
// PART 3 - Definition - Scores
//
// Structs are blocks of memory composed of smaller parts,
// each of which has a name and is called a field. The
// following struct definition has four int fields named
// hw, lab, midterm, and final.
// In this case, we use typedef to give structs of this
// type a name, Scores, which can be used like we use
// other types such as int or char.
typedef struct {
    int hw;
    int lab;
    int midterm;
    int final;
} Scores;


// PART 3 - Structs
void part_3() {
    printf("*** LAB 0 PART 3 ***\n");

    // The following creates an instance of Scores. The
    // space taken up in memory by student is equivalent
    // to an array of four ints, although we can refer to
    // the ints stored in it by name as well.
    Scores student;

    // Set the first int to have a value of 0 and verify
    // that the value changed.
    student.hw = 0;
    assert(student.hw == 0);

    // Depending on whether or not you like to live
    // dangerously, the following is either exciting or
    // terrifying. Though &student is of type Scores* (a
    // pointer to a Scores struct), we can use a cast to
    // pretend that it is actually an array of integers
    // instead.  It's all just memory after all!
    // The "(int*)" tells the C compiler that we want to
    // treat "&student" as an address of an int (in this
    // case, the start of an array) rather than an address
    // of a Scores struct.
    fill_array((int*) &student, 4);

    // We can confirm that fill_array updated the values
    // in the Scores struct:
    assert(student.hw == 2);
    assert(student.lab == 5);
    assert(student.midterm == 8);
    assert(student.final == 11);


    // Q3.1: Given that we were able to get our program to
    // treat &student as an int array, we should be able
    // to use array indexing syntax to access the struct
    // values as well (this is possible, but not practical).
    // Uncomment and modify the left-hand side of the assert
    // statement below to use array indexing ([#]) so that
    // the condition passes.

    printf("%d\n", ((int*)&student)[3]);
    assert( ((int*)&student)[3] == 11 );
}


// -------------------- //
// PART 4 - Performance //
// -------------------- //
//
// PART 4 - HELPER FUNCTION - big_array_index()
//
// To create a sufficiently large array, it must be created
// in the Heap using malloc(). We will use this array like
// a 3D array, so this function mimics multi-level array
// syntax: big_array[big_array_index(i,j,k)] is equivalent to
// big_array[i][j][k] if big_array[] were a 3D array.
int big_array_index(int i, int j, int k) {
    return SIZE * SIZE * i + SIZE * j + k;
}


// PART 4 - Performance
//
// It turns out that "Big O" is not the only important thing
// in determining how quickly a program executes.
// Here we will see that even without altering results,
// changing the order of memory accesses can alter execution
// speed.
void part_4() {
    printf("*** LAB 0 PART 4 ***\n");

    // Raw execution times are often poor metrics due to
    // inconsistencies and dependence on system, but they
    // can still be used to show general trends.
    clock_t timer;
    // allocate space in the Heap for a "3D array"
    int* big_array = (int*) malloc(SIZE * SIZE * SIZE * sizeof(int));
    // start timer
    timer = clock();


    // Q4.1: Try changing the order of the loops (switch the
    // "for" lines). The original ordering below is
    // considered "ijk".  Which loop orderings are fastest?
    for (int j = 0; j < SIZE; j++) {
        for (int k = 0; k < SIZE; k++) {
            for (int i = 0; i < SIZE; i++) {
                big_array[big_array_index(i, j, k)] = i + j + k;
            }
        }
    }

    // stop timer and print result
    printf("Approximate runtime = %d\n", (int)(clock() - timer));
}


// -------------------------- //
// PART 5 - Memory Allocation //
// -------------------------- //
//
// In the case that the size of an array is not known until
// runtime, the malloc function can be used to allocate
// memory dynamically. C is unlike Java, however, in that
// dynamically-allocated memory must be freed explicitly
// when the program is done using it via the free function.
// This can affect not only the performance of your program,
// but the performance of your overall system.
void part_5() {
    printf("*** LAB 0 PART 5 ***\n");

    // malloc takes a single argument, the number of bytes
    // to allocate, and returns the address of a newly
    // allocated chunk of memory whose size is the given
    // argument.
    // sizeof(Scores) gives the size of the Scores struct in
    // bytes (which is 16), so sizeof(Scores) * 5 is 80.
    Scores* class_grades = (Scores*) malloc(sizeof(Scores) * 300);
    fill_array((int*)class_grades, 300*4);

    // Q5: Once we are finished with malloc-ed memory, we
    // need to free the memory associated with it.
    // Run "valgrind ./lab0 5" after compiling the program,
    // both with (Q5.1) and without (Q5.2) the free statement below.
    // Valgrind is a tool for analyzing how programs
    // use memory, which is often invaluable for C and
    // C++ programming.
   //free(class_grades);
}


// ---------------------------- //
// main() FUNCTION - START HERE //
// ---------------------------- //
//
// main() is the entry point of the program. It has two
// parameters: argc is the number of arguments that were
// passed on the command line; argv is an array of those
// arguments as strings.
// Strings in C are arrays of chars.
int main(int argc, char* argv[]) {
    // input checking - note that the executable name is
    // included in the argument count
    if ( argc != 2 || !atoi(argv[1]) ) {
        printf("Usage: %s <num>\n", argv[0]);
        exit(0);
    }

    // atoi() is a library function that converts a String
    // to an integer
    switch ( atoi(argv[1]) ) {
        case 1:   part_1();  break;
        case 2:   part_2();  break;
        case 3:   part_3();  break;
        case 4:   part_4();  break;
        case 5:   part_5();  break;
        default:  printf("No part %s in this lab!\n", argv[1]);
                  exit(0);
    }

    return 0;
}
