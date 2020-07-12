#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "vectorlib.h"


//~ 20-22 sec (for case 6)
#define REPEAT 10000000
#define RESET "\033[0m"
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define LOG_ERR(msg) \
	printf(RED"%s:%d %s\n ERROR: %s\n"RESET,\
		__FILE__, __LINE__, __func__, msg);


const char* usage = "Usage: ./test <number test case>\n" \
                    "Description of cases:\n" \
                    "0 - const conversion test (from/to BB, from uint64)\n" \
                    "1 - const logical operations test (or, and, xor, not)\n" \
                    "2 - const shifts operations test (left/right shift/cycle shift)\n" \
                    "3 - const service functions test (null, copy, change size, len)\n"
                    "4 - const error codes test\n" \
                    "5 - all short const test (cases 0-4)\n" \
                    "6 - long random test (de Morgan) (or, and, not, to str)\n" \
                    "7 - all tests (cases 5-6)";


int test_BB_conversion(BB *vector)
{
    int errors = 0;

    //len < byte
    BB_from_str("000", vector);
    char *str = BB_to_str(vector);
    if (strcmp("000", str)) {
        LOG_ERR ("incorrect BB_from_str -> BB_to_str");
        printf(" input:   000 \n output:  %s\n ", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    //len == byte (len % byte == 0)
    BB_from_str("11111111", vector);
    str = BB_to_str(vector);
    if (strcmp("11111111", str)) {
        LOG_ERR ("incorrect BB_from_str -> BB_to_str");
        printf(" input:   11111111 \n output:  %s\n ", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    //standard situation
    BB_from_str("000000001011011011000001001011111111101000000000000001010000001", vector);
    str = BB_to_str(vector);
    if (strcmp("000000001011011011000001001011111111101000000000000001010000001", str))
    {
        LOG_ERR ("incorrect BB_from_str -> BB_to_str");
        printf(" input:   000000001011011011000001001011111111101000000000000001010000001 \n output:  %s\n ", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    //max len (MAX_PARTS * 8)
    BB_from_str("1010011111111011010110110101100101000000101100101010101011100000000000000000001011111111111111100000000000101001010011000100100001010000010000010101000000000010",vector);
    str = BB_to_str(vector);
    if (strcmp("1010011111111011010110110101100101000000101100101010101011100000000000000000001011111111111111100000000000101001010011000100100001010000010000010101000000000010", str))
    {
        LOG_ERR ("incorrect BB_from_str -> BB_to_str");
        printf(" input:   1010011111111011010110110101100101000000101100101010101011100000000000000000001011111111111111100000000000101001010011000100100001010000010000010101000000000010 \n output:  %s\n ", str);
        errors++;
    }

    BB_null(vector);
    free(str);

    return errors;
}


int test_uint64_conversion(BB *vector)
{
    int errors = 0;

    //empty number
    BB_from_uint64_t(0, vector);
    char *str = BB_to_str(vector);
    if(strcmp("", str))
    {
        LOG_ERR ("incorrect BB_from_uint64_t -> BB_to_str");
        printf(" number:  0\n input:    \n output:  %s\n ", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    //len % byte == 0
    BB_from_uint64_t(2048, vector);
    str = BB_to_str(vector);
    if(strcmp("100000000000", str))
    {
        LOG_ERR ("incorrect BB_from_uint64_t -> BB_to_str");
        printf(" number:  2048\n input:   100000000000 \n output:  %s\n ", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    //standard situation
    BB_from_uint64_t(81273822, vector);
    str = BB_to_str(vector);
    if(strcmp("100110110000010001111011110", str))
    {
        LOG_ERR ("incorrect BB_from_uint64_t -> BB_to_str");
        printf(" number:  81273822\n input:   100110110000010001111011110 \n output:  %s\n ", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    //max number (2^64)
    BB_from_uint64_t(18446744073709551615, vector);
    str = BB_to_str(vector);
    if(strcmp("1111111111111111111111111111111111111111111111111111111111111111", str))
    {
        LOG_ERR ("incorrect BB_from_uint64_t -> BB_to_str");
        printf(" number:  18446744073709551615\n input:   1111111111111111111111111111111111111111111111111111111111111111 \n output:  %s\n ", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    return errors;
}


int test_BB_or(BB *vector, BB *vector1, BB *vector2)
{
    int errors = 0;

    //opposite vectors
    BB_from_str("101101101100000100100111101010011011000010010101010101", vector1);
    BB_from_str("010010010011111011011000010101100100111101101010101010", vector2);
    BB_or(vector1, vector2, vector);
    char *str = BB_to_str(vector);
    if(strcmp("111111111111111111111111111111111111111111111111111111", str))
    {
        LOG_ERR ("incorrect BB_or");
        printf(" vector_1: 101101101100000100100111101010011011000010010101010101\n vector_2: 010010010011111011011000010101100100111101101010101010\n output:   %s\n correct:  111111111111111111111111111111111111111111111111111111 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //same vectors
    BB_from_str("00100111111111111111010010101010100000010110010000101110111101010010101", vector1);
    BB_from_str("00100111111111111111010010101010100000010110010000101110111101010010101", vector2);
    BB_or(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("00100111111111111111010010101010100000010110010000101110111101010010101", str))
    {
        LOG_ERR ("incorrect BB_or");
        printf(" vector_1: 00100111111111111111010010101010100000010110010000101110111101010010101\n vector_2: 00100111111111111111010010101010100000010110010000101110111101010010101\n output:   %s\n correct:  00100111111111111111010010101010100000010110010000101110111101010010101 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //len(vector1) < len(vector2)
    BB_from_str("10010101111111111001111111111111000000000101000101010", vector1);
    BB_from_str("000000101001111110101001010111000000111111111010101001100001010100010010010101001010100010010100100101001", vector2);
    BB_or(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("000000101001111110101001010111000000111111111010101011110101111111111011111111111111100010010101100101011", str))
    {
        LOG_ERR ("incorrect BB_or");
        printf(" vector_1: 10010101111111111001111111111111000000000101000101010\n vector_2: 000000101001111110101001010111000000111111111010101001100001010100010010010101001010100010010100100101001\n output:   %s\n correct:  000000101001111110101001010111000000111111111010101011110101111111111011111111111111100010010101100101011 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //len(vector2) < len(vector1)
    BB_from_str("10010000000000000000000000000010101010111111111110010101010010101000001", vector1);
    BB_from_str("1001011111111", vector2);
    BB_or(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("10010000000000000000000000000010101010111111111110010101011011111111111", str))
    {
        LOG_ERR ("incorrect BB_or");
        printf(" vector_1: 10010000000000000000000000000010101010111111111110010101010010101000001\n vector_2: 1001011111111\n output:   %s\n correct:  10010000000000000000000000000010101010111111111110010101011011111111111 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    return errors;
}


int test_BB_and(BB *vector, BB *vector1, BB *vector2)
{
    int errors = 0;

    //opposite vectors
    BB_from_str("101101101100000100100111101010011011000010010101010101", vector1);
    BB_from_str("010010010011111011011000010101100100111101101010101010", vector2);
    BB_and(vector1, vector2, vector);
    char *str = BB_to_str(vector);
    if(strcmp("000000000000000000000000000000000000000000000000000000", str))
    {
        LOG_ERR ("incorrect BB_and");
        printf(" vector_1: 101101101100000100100111101010011011000010010101010101\n vector_2: 010010010011111011011000010101100100111101101010101010\n output:   %s\n correct:  000000000000000000000000000000000000000000000000000000 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //same vectors
    BB_from_str("00100111111111111111010010101010100000010110010000101110111101010010101", vector1);
    BB_from_str("00100111111111111111010010101010100000010110010000101110111101010010101", vector2);
    BB_and(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("00100111111111111111010010101010100000010110010000101110111101010010101", str))
    {
        LOG_ERR ("incorrect BB_and");
        printf(" vector_1: 00100111111111111111010010101010100000010110010000101110111101010010101\n vector_2: 00100111111111111111010010101010100000010110010000101110111101010010101\n output:   %s\n correct:  00100111111111111111010010101010100000010110010000101110111101010010101 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //len(vector2) < len(vector1)
    BB_from_str("101101101100000100100111101010011011000010010101010101", vector1);
    BB_from_str("1010010100100000010010001", vector2);
    BB_and(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("0010010000000000000010001", str))
    {
        LOG_ERR ("incorrect BB_and");
        printf(" vector_1: 101101101100000100100111101010011011000010010101010101\n vector_2: 1010010100100000010010001\n output:   %s\n correct:  0010010000000000000010001 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //len(vector1) < len(vector2)
    BB_from_str("1000101111111111111100100101010", vector1);
    BB_from_str("00010100101010010011111111000000000100000100010000100011111111110010000100", vector2);
    BB_and(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("0000000100011111111100000000000", str))
    {
        LOG_ERR ("incorrect BB_and");
        printf(" vector_1: 1000101111111111111100100101010\n vector_2: 00010100101010010011111111000000000100000100010000100011111111110010000100\n output:   %s\n correct:  0000000100011111111100000000000 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    return errors;
}


int test_BB_xor(BB *vector, BB *vector1, BB *vector2)
{
    int errors = 0;

    //opposite vectors
    BB_from_str("101101101100000100100111101010011011000010010101010101", vector1);
    BB_from_str("010010010011111011011000010101100100111101101010101010", vector2);
    BB_xor(vector1, vector2, vector);
    char *str = BB_to_str(vector);
    if(strcmp("111111111111111111111111111111111111111111111111111111", str))
    {
        LOG_ERR ("incorrect BB_xor");
        printf(" vector_1: 101101101100000100100111101010011011000010010101010101\n vector_2: 010010010011111011011000010101100100111101101010101010\n output:   %s\n correct:  111111111111111111111111111111111111111111111111111111 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //same vectors
    BB_from_str("00100111111111111111010010101010100000010110010000101110111101010010101", vector1);
    BB_from_str("00100111111111111111010010101010100000010110010000101110111101010010101", vector2);
    BB_xor(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("00000000000000000000000000000000000000000000000000000000000000000000000", str))
    {
        LOG_ERR ("incorrect BB_or");
        printf(" vector_1: 00100111111111111111010010101010100000010110010000101110111101010010101\n vector_2: 00100111111111111111010010101010100000010110010000101110111101010010101\n output:   %s\n correct:  00000000000000000000000000000000000000000000000000000000000000000000000 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //len(vector1) < len(vector2)
    BB_from_str("0100111111111111001010010101100000101000101010", vector1);
    BB_from_str("000000101001111110101001010111000000111111111010101001100001010100010010010101001010100010010100100101001", vector2);
    BB_xor(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("000000101001111110101001010111000000111111111010101001100001110011101101101100011000001110010001100000011", str))
    {
        LOG_ERR ("incorrect BB_xor");
        printf(" vector_1: 0100111111111111001010010101100000101000101010\n vector_2: 000000101001111110101001010111000000111111111010101001100001010100010010010101001010100010010100100101001\n output:   %s\n correct:  000000101001111110101001010111000000111111111010101001100001110011101101101100011000001110010001100000011 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    //len(vector2) < len(vector1)
    BB_from_str("01001010010101111111111111110000000001010010101111111110010101010010101000001", vector1);
    BB_from_str("110100101000000010111001011111111", vector2);
    BB_xor(vector1, vector2, vector);
    str = BB_to_str(vector);
    if(strcmp("01001010010101111111111111110000000001010010011011010110010111101011110111110", str))
    {
        LOG_ERR ("incorrect BB_xor");
        printf(" vector_1: 01001010010101111111111111110000000001010010101111111110010101010010101000001\n vector_2: 110100101000000010111001011111111\n output:   %s\n correct:  01001010010101111111111111110000000001010010011011010110010111101011110111110 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    free(str);

    return errors;
}


int test_BB_not(BB *vector1, BB *vector)
{
    int errors = 0;

    BB_from_str("000000000000000000000000000000000000000", vector1);
    BB_not(vector1, vector);
    char *str = BB_to_str(vector);
    if(strcmp("111111111111111111111111111111111111111", str))
    {
        LOG_ERR ("incorrect BB_not");
        printf(" vector: 000000000000000000000000000000000000000\n output:   %s\n correct:  111111111111111111111111111111111111111 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    BB_from_str("01001011111110101000000000101010100101000001001010101001", vector1);
    BB_not(vector1, vector);
    str = BB_to_str(vector);
    if(strcmp("10110100000001010111111111010101011010111110110101010110", str))
    {
        LOG_ERR ("incorrect BB_not");
        printf(" vector: 01001011111110101000000000101010100101000001001010101001\n output:   %s\n correct:  10110100000001010111111111010101011010111110110101010110 \n", str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    return errors;
}


int test_BB_shifts (BB *vector1, BB *vector)
{
    int errors = 0;

    //shift = len
    BB_from_str("10110110110101010101010000000001010000100101", vector1);
    BB_left_shift(vector1, vector, 44);
    char *str = BB_to_str(vector);
    if (strcmp("", str)) {
        LOG_ERR ("incorrect BB_left_shift");
        printf(" shift:  44\n vector: 10110110110101010101010000000001010000100101\n output: %s\n correct:<empty vector> \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //shift < byte
    BB_from_str("10110110110101010101000000000000000010101010101011010000000001010000100101", vector1);
    BB_left_shift(vector1, vector, 7);
    str = BB_to_str(vector);
    if (strcmp("0110101010101000000000000000010101010101011010000000001010000100101", str)) {
        LOG_ERR ("incorrect BB_left_shift");
        printf(" shift:  7\n vector: 10110110110101010101000000000000000010101010101011010000000001010000100101\n output: %s\n correct:0110101010101000000000000000010101010101011010000000001010000100101 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //standard situation
    BB_from_str("10110110110101010101000000000000000010101010101011010000000001010000100101", vector1);
    BB_left_shift(vector1, vector, 48);
    str = BB_to_str(vector);
    if (strcmp("11010000000001010000100101", str)) {
        LOG_ERR ("incorrect BB_left_shift");
        printf(" shift:  48\n vector: 10110110110101010101000000000000000010101010101011010000000001010000100101\n output: %s\n correct:11010000000001010000100101 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //negative shift
    BB_from_str("10110110110101010101000000000000000010101010101011010000000001010000100101", vector1);
    BB_left_shift(vector1, vector, -16);
    str = BB_to_str(vector);
    if (strcmp("1011011011010101010100000000000000001010101010101101000000", str)) {
        LOG_ERR ("incorrect BB_left_shift");
        printf(" shift:  -16\n vector: 10110110110101010101000000000000000010101010101011010000000001010000100101\n output: %s\n correct:1011011011010101010100000000000000001010101010101101000000 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    /////////////////////////////
    //shift = len
    BB_from_str("10110110110101010101010000000001010000100101", vector1);
    BB_right_shift(vector1, vector, 44);
    str = BB_to_str(vector);
    if (strcmp("", str)) {
        LOG_ERR ("incorrect BB_right_shift");
        printf(" shift:  44\n vector: 10110110110101010101010000000001010000100101\n output: %s\n correct:<empty vector> \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //shift < byte
    BB_from_str("10110110110101010101000000000000000010101010101011010000000001010000100101", vector1);
    BB_right_shift(vector1, vector, 7);
    str = BB_to_str(vector);
    if (strcmp("1011011011010101010100000000000000001010101010101101000000000101000", str)) {
        LOG_ERR ("incorrect BB_right_shift");
        printf(" shift:  7\n vector: 10110110110101010101000000000000000010101010101011010000000001010000100101\n output: %s\n correct:1011011011010101010100000000000000001010101010101101000000000101000 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //standard situation
    BB_from_str("10110110110101010101000000000000000010101010101011010000000001010000100101", vector1);
    BB_right_shift(vector1, vector, 48);
    str = BB_to_str(vector);
    if (strcmp("10110110110101010101000000", str)) {
        LOG_ERR ("incorrect BB_right_shift");
        printf(" shift:  48\n vector: 10110110110101010101000000000000000010101010101011010000000001010000100101\n output: %s\n correct:10110110110101010101000000 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //negative shift
    BB_from_str("10110110110101010101000000000000000010101010101011010000000001010000100101", vector1);
    BB_right_shift(vector1, vector, -16);
    str = BB_to_str(vector);
    if (strcmp("0101000000000000000010101010101011010000000001010000100101", str)) {
        LOG_ERR ("incorrect BB_right_shift");
        printf(" shift:  -16\n vector: 10110110110101010101000000000000000010101010101011010000000001010000100101\n output: %s\n correct:0101000000000000000010101010101011010000000001010000100101 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    return errors;
}


int test_BB_cshifts(BB *vector1, BB *vector)
{
    int errors = 0;

    //len == shift
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_left_cshift(vector1, vector, 58);
    char *str = BB_to_str(vector);
    if(strcmp("1000110101010010001000000000111111111100110000000001111110", str))
    {
        LOG_ERR ("incorrect BB_left_cshift\n");
        printf(" shift:  58\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:1000110101010010001000000000111111111100110000000001111110 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //empty shift
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_left_cshift(vector1, vector, 0);
    str = BB_to_str(vector);
    if(strcmp("1000110101010010001000000000111111111100110000000001111110", str))
    {
        LOG_ERR ("incorrect BB_left_cshift\n");
        printf(" shift:  0\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:1000110101010010001000000000111111111100110000000001111110 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //shift > len
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_left_cshift(vector1, vector, 276);
    str = BB_to_str(vector);
    if(strcmp("0000000111111010001101010100100010000000001111111111001100", str))
    {
        LOG_ERR ("incorrect BB_left_cshift\n");
        printf(" shift:  276\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:0000000111111010001101010100100010000000001111111111001100 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //standard situation
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_left_cshift(vector1, vector, 14);
    str = BB_to_str(vector);
    if(strcmp("1000100000000011111111110011000000000111111010001101010100", str))
    {
        LOG_ERR ("incorrect BB_left_cshift\n");
        printf(" shift:  14\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:1000100000000011111111110011000000000111111010001101010100 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //negative shift
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_left_cshift(vector1, vector, -20);
    str = BB_to_str(vector);
    if(strcmp("0011000000000111111010001101010100100010000000001111111111", str))
    {
        LOG_ERR ("incorrect BB_left_cshift\n");
        printf(" shift:  14\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:0011000000000111111010001101010100100010000000001111111111 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    /////////////////////////////////////
    //len == shift
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_right_cshift(vector1, vector, 58);
    str = BB_to_str(vector);
    if(strcmp("1000110101010010001000000000111111111100110000000001111110", str))
    {
        LOG_ERR ("incorrect BB_right_cshift\n");
        printf(" shift:  58\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:1000110101010010001000000000111111111100110000000001111110 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //empty shift
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_right_cshift(vector1, vector, 0);
    str = BB_to_str(vector);
    if(strcmp("1000110101010010001000000000111111111100110000000001111110", str))
    {
        LOG_ERR ("incorrect BB_right_cshift\n");
        printf(" shift:  0\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:1000110101010010001000000000111111111100110000000001111110 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //shift > len
    BB_from_str("1000110101010010001000000000111111111100110000000001111110", vector1);
    BB_right_cshift(vector1, vector, 314);
    str = BB_to_str(vector);
    if(strcmp("1111001100000000011111101000110101010010001000000000111111", str))
    {
        LOG_ERR ("incorrect BB_right_cshift\n");
        printf(" shift:  314\n vector: 1000110101010010001000000000111111111100110000000001111110\n output: %s\n correct:1111001100000000011111101000110101010010001000000000111111 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //standard situation
    BB_from_str("101100101111111111111111111110100101010000001010000111111111000100000100001", vector1);
    BB_right_cshift(vector1, vector, 36);
    str = BB_to_str(vector);
    if(strcmp("000001010000111111111000100000100001101100101111111111111111111110100101010", str))
    {
        LOG_ERR ("incorrect BB_right_cshift\n");
        printf(" shift:  36\n vector: 101100101111111111111111111110100101010000001010000111111111000100000100001\n output: %s\n correct:000001010000111111111000100000100001101100101111111111111111111110100101010 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    //negative shift
    BB_from_str("101100101111111111111111111110100101010000001010000111111111000100000100001", vector1);
    BB_right_cshift(vector1, vector, -40);
    str = BB_to_str(vector);
    if(strcmp("000010100001111111110001000001000011011001011111111111111111111101001010100", str))
    {
        LOG_ERR ("incorrect BB_right_cshift\n");
        printf(" shift:  14\n vector: 101100101111111111111111111110100101010000001010000111111111000100000100001\n output: %s\n correct:000010100001111111110001000001000011011001011111111111111111111101001010100 \n",
               str);
        errors++;
    }
    BB_null(vector);
    BB_null(vector1);
    free(str);

    return errors;
}


int test_BB_service(BB *vector, BB *vector1)
{
    int errors = 0;

    BB_from_str("101100101111111111111111111110100101010000001010000111111111000100000100001", vector);
    BB_null(vector);
    char *str = BB_to_str(vector);
    if(strcmp("", str))
    {
        LOG_ERR ("incorrect BB_null\n");
        errors++;
    }
    free(str);

    BB_from_str("101100101111111111111111111110100101010000001010000111111111000100000100001", vector1);
    BB_create_copy(vector1, vector);
    str = BB_to_str(vector);
    if(strcmp("101100101111111111111111111110100101010000001010000111111111000100000100001", str))
    {
        LOG_ERR ("incorrect BB_create_copy\n");
        printf(" vector: 101100101111111111111111111110100101010000001010000111111111000100000100001\n output: %s\n", str);
        errors++;
    }
    BB_null(vector1);
    free(str);

    BB_from_str("1001011111001", vector1);
    BB_change_size(vector1, vector);
    str = BB_to_str(vector1);
    if(strcmp("000000000000000000000000000000000000000000000000000000000000001001011111001", str))
    {
        LOG_ERR ("incorrect BB_change_size\n");
        printf(" vector: 1001011111001\n output: %s\n correct:000000000000000000000000000000000000000000000000000000000000001001011111001\n", str);
        errors++;
    }
    BB_null(vector);
    free(str);

    if(vector_len(vector1) != 75)
    {
        LOG_ERR ("incorrect vector_len\n");
        printf(" vector: 000000000000000000000000000000000000000000000000000000000000001001011111001\n output len: %ld\n correct len:  75\n", vector_len(vector1));
        errors++;
    }
    BB_null(vector1);

    return errors;
}


int test_de_Morgan(BB *vector, BB *vector1, BB *vector2, BB *vector3, BB *vector4)
{
    int errors = 0;

    BB_random(vector1);
    BB_random(vector2);
    BB_change_size(vector1, vector2);

    //~(A | B) = ~A & ~B
    BB_or(vector1, vector2, vector);
    BB_not(vector, vector3);
    char *str1 = BB_to_str(vector3);
    BB_null(vector);
    BB_null(vector3);

    BB_not(vector1, vector3);
    BB_not(vector2, vector4);
    BB_and(vector3, vector4, vector);
    char *str2 = BB_to_str(vector);

    if(strcmp(str1, str2))
    {
        LOG_ERR ("incorrect ~(A | B) = ~A & ~B\n");
        printf("%s != %s\n", str1, str2);
        errors++;
    }

    BB_null(vector);
    free(str1);
    free(str2);

    //~(A & B) = ~A | ~B
    BB_and(vector1, vector2, vector);
    BB_null(vector1);
    BB_not(vector, vector1);
    str1 = BB_to_str(vector1);
    BB_null(vector);

    BB_or(vector3, vector4, vector);
    str2 = BB_to_str(vector);

    if(strcmp(str1, str2))
    {
        LOG_ERR ("incorrect ~(A & B) = ~A | ~B\n");
        printf("%s != %s\n", str1, str2);
        errors++;
    }

    BB_null(vector);
    BB_null(vector1);
    BB_null(vector2);
    BB_null(vector3);
    BB_null(vector4);
    free(str1);
    free(str2);
    return errors;
}


int test_error_code(BB *vector)
{
    int errors = 0;

    if ((BB_from_str("129932", vector))  != ERR_INCORRECT_STR)
    {
        LOG_ERR ("incorrect ERR_INCORRECT_STR in BB_from_str\n");
        errors++;
    }

    if ((BB_from_str("1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", vector))  != ERR_INCORRECT_STR)
    {
        LOG_ERR ("incorrect ERR_INCORRECT_STR in BB_from_str\n");
        errors++;
    }

    return errors;
}




int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        puts(usage);
        return 0;
    }

    int errors = 0;
    int repeat = REPEAT;
    int case_num = atoi(argv[1]);

    BB* vector1 = calloc(1, sizeof(BB));
    BB* vector2 = calloc(1, sizeof(BB));
    BB* vector3 = calloc(1, sizeof(BB));
    BB* vector4 = calloc(1, sizeof(BB));
    BB* vector = calloc(1, sizeof(BB));

    switch (case_num)
    {
        case 0:
            errors += test_BB_conversion(vector);
            errors += test_uint64_conversion(vector);
            break;

        case 1:
            errors += test_BB_and(vector, vector1, vector2);
            errors += test_BB_or(vector, vector1, vector2);
            errors += test_BB_xor(vector, vector1, vector2);
            errors += test_BB_not(vector, vector1);
            break;

        case 2:
            errors += test_BB_shifts(vector1, vector);
            errors += test_BB_cshifts(vector1, vector);
            break;

        case 3:
            errors += test_BB_service(vector, vector1);
            break;

        case 4:
            errors += test_error_code(vector);
            break;

        case 5:
            errors += test_BB_conversion(vector);
            errors += test_uint64_conversion(vector);
            errors += test_BB_and(vector, vector1, vector2);
            errors += test_BB_or(vector, vector1, vector2);
            errors += test_BB_xor(vector, vector1, vector2);
            errors += test_BB_not(vector, vector1);
            errors += test_BB_shifts(vector1, vector);
            errors += test_BB_cshifts(vector1, vector);
            errors += test_BB_service(vector, vector1);
            errors += test_error_code(vector);
            break;

        case 6:
            while (repeat)
            {
                errors += test_de_Morgan(vector, vector1, vector2, vector3, vector4);
                repeat--;
            }
            break;

        case 7:
            errors += test_BB_conversion(vector);
            errors += test_uint64_conversion(vector);
            errors += test_BB_and(vector, vector1, vector2);
            errors += test_BB_or(vector, vector1, vector2);
            errors += test_BB_xor(vector, vector1, vector2);
            errors += test_BB_not(vector, vector1);
            errors += test_BB_shifts(vector1, vector);
            errors += test_BB_cshifts(vector1, vector);
            errors += test_BB_service(vector, vector1);
            errors += test_error_code(vector);
            while (repeat)
            {
                errors += test_de_Morgan(vector, vector1, vector2, vector3, vector4);
                repeat--;
            }
            break;

        default:
            puts(usage);
            break;
    }

    if (errors > 0)
        printf (RED"\nNumber error tests: %d\n"RESET, errors);
    else
        puts (GREEN"All tests are correct"RESET);

    free(vector);
    free(vector1);
    free(vector2);
    free(vector3);
    free(vector4);

    return 0;
}