#ifndef VECTORLIB_H
#define VECTORLIB_H

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PARTS 20

/* --------------------------
 *  ERROR CODES
 * --------------------------
 */
#define ERR_OK 0
#define ERR_MEM_NOT_ALLOC 1
#define ERR_NULL_ARG 2
#define ERR_INCORRECT_STR 3

/* --------------------------
 *  STRUCTURE OF VECTOR
 * --------------------------
 *
 * The structure has:
 * 1) uint8 for storing vector numbers (LE)
 * 2) number of the last significant byte (numbering from 0)
 * 3) number of the last significant bit (numbering from right to left, from 1)
 *
 */
struct BigBool
{
    uint8_t parts[MAX_PARTS];
    size_t last_byte;
    size_t last_bit;
} typedef BB;

/* --------------------------
 * SERVICE FUNCTIONS
 * --------------------------
 */


/*
 * int BB_random (BB *res)
 *
 * Description: creates a vector of random length (but no longer than MAX_PARTS)
 * and fills it with random numbers.
 *
 * Memory: the caller allocates, fills with zeros, and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - res == NULL
 *  2) ERR_OK - the function was completed correctly

 */
int BB_random (BB *res);


/*
 * int BB_null(BB *vector);
 *
 * Description: makes the vector null (empty).
 *
 * Memory: the caller allocates and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL
 *  2) ERR_OK - the function was completed correctly
 */
int BB_null(BB *vector);

/*
 * int BB_change_size(BB *vector, BB *larger_vec);
 *
 * Description: changes value of last byte and bit of smaller vector so that
 * its length becomes equal to length of larger vector.
 *
 * Memory: the caller allocates and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL || larger_vec == NULL
 *  2) ERR_OK - the function was completed correctly
 *
 * Note: insignificant bits of the smaller vector must be filled with zeros.
 *
 */
int BB_change_size(BB *vector, BB *larger_vec);


/*
 * size_t vector_len(BB *vector);
 *
 * Description: measures length of vector in bits
 *
 * Return value: int length of vector
 *
 * Note: it`s assumed that the check vector != NULL has already been made
 *
 */
size_t vector_len(BB *vector);


/*
 * int BB_create_copy(BB *vector, BB *res);
 *
 * Description: creates an identical copy of the vector.
 *
 * Memory: the caller allocates, fills with zeros (for BB *res), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL || res == NULL
 *  2) ERR_OK - the function was completed correctly
 *
 * Note: res must be filled with zeros.
 *
 */
int BB_create_copy(BB *vector, BB *res);



/* --------------------------
 * CONVERSION FUNCTIONS
 * --------------------------
 */

/*
 * int BB_from_uint64_t(uint64_t num, BB *vector);
 *
 * Description: converts a number uint64 to a vector
 *
 * Memory: the caller allocates, fills with zeros (for BB *vector), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL
 *  2) ERR_OK - the function was completed correctly
 *  3) empty vector (BB_null) - num == 0
 *
 * Note: res must be filled with zeros, num does`t need to be checked (e.g. num < 0),
 * since the number has the type uint64_t
 *
 */
int BB_from_uint64_t(uint64_t num, BB *vector);

/*
 * int BB_from_str(char* str, BB *vector );
 *
 * Description: converts a string to a vector
 *
 * Memory: the caller allocates, fills with zeros (for BB *vector), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL
 *  2) ERR_OK - the function was completed correctly
 *  3) ERR_INCORRECT_STR - the string is too large or has characters other than 0 and 1
 *
 */
int BB_from_str(char* str, BB *vector );

/*
 * char* BB_to_str(BB *vector);
 *
 * Description: creates a string from a vector
 *
 * Memory:
 *  1) for vector: the caller allocates and frees memory
 *  2) for str: the calling allocates, the caller frees memory
 *
 * Return value:
 *  1) str == NULL - str == NULL (cannot allocate memory)
 *  2) empty str - vector == NULL
 *  3) char *str - the function was completed correctly
 *
 */
char* BB_to_str(BB *vector);


/* --------------------------
 * LOGICAL OPERATIONS
 * --------------------------
 */

/*
 * int BB_or (BB *vector1, BB *vector2, BB *res);
 *
 * Description: disjunction of two vectors
 *
 * Memory: the caller allocates, fills with zeros (for BB *res), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector1 == NULL || res == NULL || vector2 == NULL
 *  2) ERR_OK - the function was completed correctly
 *
 */
int BB_or (BB *vector1, BB *vector2, BB *res);

/*
 * int BB_and (BB *vector1, BB *vector2, BB *res);
 *
 * Description: conjunction of two vectors
 *
 * Memory: the caller allocates, fills with zeros (for BB *res), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector1 == NULL || res == NULL || vector2 == NULL
 *  2) ERR_OK - the function was completed correctly
 *
 */
int BB_and (BB *vector1, BB *vector2, BB *res);

/*
 * int BB_xor (BB *vector1, BB *vector2, BB *res);
 *
 * Description: xor of two vectors
 *
 * Memory: the caller allocates, fills with zeros (for BB *res), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector1 == NULL || res == NULL || vector2 == NULL
 *  2) ERR_OK - the function was completed correctly
 *
 */
int BB_xor (BB *vector1, BB *vector2, BB *res);

/*
 * int BB_not (BB *vector, BB *res);
 *
 * Description: inversion of the vector
 *
 * Memory: the caller allocates, fills with zeros (for BB *res), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL || res == NULL
 *  2) ERR_OK - the function was completed correctly
 *
 */
int BB_not (BB *vector, BB *res);

/* --------------------------
 * SHIFT OPERATIONS
 * --------------------------
 */

/*
 * int BB_right_shift (BB *vector, BB *res, int len_shift);
 *
 * Description: shifts vector in the direction of the least significant bits. Reduces the vector
 *
 * Memory: the caller allocates, fills with zeros (for BB *res), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL || res == NULL
 *  2) BB_create_copy(vector, res) - len_shift == 0
 *  3) BB_null(res) - |len_shift| > vector_len(vector)
 *  4) BB_left_shift(vector, res, abs(len_shift)) - len_shift < 0
 *  5) ERR_OK - the function was completed correctly
 *
 */
int BB_right_shift (BB *vector, BB *res, int len_shift);

/*
 * int BB_left_shift (BB *vector, BB *res, int len_shift);
 *
 * Description: shifts the vector towards the higher digits. Reduces the vector
 *
 * Memory: the caller allocates, fills with zeros (for BB *res), and frees memory for the vector.
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL || res == NULL
 *  2) BB_create_copy(vector, res) - len_shift == 0
 *  3) BB_null(res) - |len_shift| > vector_len(vector)
 *  4) BB_right_shift(vector, res, abs(len_shift)) - len_shift < 0
 *  5) ERR_OK - the function was completed correctly
 *
 */
int BB_left_shift (BB *vector, BB *res, int len_shift);

/*
 * int BB_left_cshift (BB *vector, BB *res, int len_shift);
 *
 * Description: moves the highest bits to the end of the vector
 *
 * Memory: For vector and res: the caller allocates, fills with zeros (for BB *res),
 * and frees memory for the vector. For buf_vector (inside the function) - the called.
 *
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL || res == NULL
 *  2) BB_create_copy(vector, res) - len_shift == 0 || len_shift == vector_len(vector)
 *  3) BB_right_сshift(vector, res, abs(len_shift)) - len_shift < 0
 *  4) ERR_MEM_NOT_ALLOC - buf_vector == NULL
 *  5) ERR_OK - the function was completed correctly
 *
 */
int BB_left_cshift (BB *vector, BB *res, int len_shift);

/*
 * int BB_right_cshift (BB *vector, BB *res, int len_shift);
 *
 * Description: moves lower bits to the beginning of the vector
 *
 * Memory: For vector and res: the caller allocates, fills with zeros (for BB *res),
 * and frees memory for the vector.
 *
 *
 * Return value:
 *  1) ERR_NULL_ARG - vector == NULL || res == NULL
 *  2) BB_create_copy(vector, res) - len_shift == 0 || len_shift == vector_len(vector)
 *  3) BB_left_сshift(vector, res, abs(len_shift)) - len_shift < 0
 *  5) BB_left_cshift(vector, res, rev_len_shift) - the function was completed correctly
 *
 */
int BB_right_cshift(BB *vector, BB *res, int len_shift);

#endif