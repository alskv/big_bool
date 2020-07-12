#include "vectorlib.h"

/* --------------------------
 * SERVICE FUNCTIONS
 * --------------------------
 */

int BB_random (BB *res)
{
    if (res == NULL)
        return ERR_NULL_ARG;

    res->last_bit = rand() % 8;
    res->last_byte = rand() % MAX_PARTS;
    for (size_t byte = 0;  byte <= res->last_byte; byte++)
        res->parts[byte] = rand() % 256;

    return ERR_OK;
}


int BB_null(BB *vector)
{
    if (vector == NULL)
        return ERR_NULL_ARG;

    vector->last_bit = 0;
    vector->last_byte = 0;
    memset(vector->parts, 0, MAX_PARTS);

    return ERR_OK;
};

int BB_create_copy(BB *vector, BB *res)
{
    if (vector == NULL || res == NULL)
        return ERR_NULL_ARG;

    res->last_byte = vector->last_byte;
    res->last_bit = vector->last_bit;

    for (size_t part = 0; part <= res->last_byte; part++)
        res->parts[part] = vector->parts[part];

    return ERR_OK;
}

int BB_change_size(BB *vector, BB *larger_vec)
{
    if (vector == NULL || larger_vec == NULL)
        return ERR_NULL_ARG;

    if (vector_len(vector) > vector_len(larger_vec))
        return BB_change_size(larger_vec, vector);

    vector->last_byte = larger_vec->last_byte;
    vector->last_bit = larger_vec->last_bit;
    return ERR_OK;
}

size_t vector_len(BB *vector)
{
    size_t len = (vector->last_byte * 8) + vector->last_bit;
    return len;
}


int BB_check_correctness(char *str)
{
    int len = strlen(str);
    if (len == 0)
        return  ERR_NULL_ARG;

    if (len > MAX_PARTS * 8)
        return ERR_INCORRECT_STR;

    for (size_t i = 0; i < len; i++)
    {
        if (str[i] != '1' && str[i] != '0')
            return ERR_INCORRECT_STR;
    }
    return ERR_OK;
};


/* --------------------------
 * CONVERSION FUNCTIONS
 * --------------------------
 */

int BB_from_uint64_t(uint64_t num, BB *res)
{
    if (res == NULL)
        return ERR_NULL_ARG;

    if (!num)
        return BB_null(res);

    uint8_t bit_mask = 255;
    size_t part = 0;
    while (num)
    {
        res->parts[part] = num & bit_mask;
        num >>= 8u;
        part++;
    }
    res->last_byte = part - 1;

    uint8_t older_part = res->parts[res->last_byte];
    size_t bit = 0;
    while (older_part)
    {
        older_part >>= 1u;
        bit++;
    }
    res->last_bit = bit;

    return ERR_OK;
}

int BB_from_str(char *str, BB *vector)
{
    int correct = BB_check_correctness(str);
    if (correct)
        //ERR_NULL_ARG or ERR_INCORRECT_INPUT_STR
        return correct;

    if (vector == NULL)
        return ERR_NULL_ARG;

    size_t len = strlen(str);

    vector->last_byte = len / 8;
    vector->last_bit = len % 8;

    size_t sym = 0;
    for (size_t bit = 0; bit < vector->last_bit; bit++)
    {
        vector->parts[vector->last_byte] <<= 1u;
        vector->parts[vector->last_byte] |= (str[sym] - '0');
        sym++;
    }

    for (size_t byte = (vector->last_byte - 1); byte != -1; byte--)
    {
        for (size_t bit = 0; bit < 8; bit++)
        {
            vector->parts[byte] <<= 1u;
            vector->parts[byte] |= (str[sym] - '0');
            sym++;
        }
    }

    return ERR_OK;
}

char* BB_to_str(BB *vector)
{
    size_t len = vector_len(vector);

    char *str = (char*)calloc((len + 1), sizeof(char));
    if (str == NULL)
        return str; //str = NULL

    if (vector == NULL)
        return str; //empty str

    for (size_t bit = 0; bit < vector->last_bit; bit++)
    {
        size_t get_bit = (vector->last_byte * 8) + bit;
        size_t sym = len - get_bit - 1;
        str[sym] = (vector->parts[vector->last_byte] >> bit) & 1;
        str[sym] += '0';
    }

    for (size_t byte = 0; byte < vector->last_byte; byte++)
    {
        for (size_t bit = 0; bit < 8; bit++)
        {
            size_t get_bit = (byte * 8) + bit;
            size_t sym = len - get_bit - 1;
            str[sym] = (vector->parts[byte] >> bit) & 1;
            str[sym] += '0';
        }
    }

    return str;
};


/* --------------------------
 * LOGICAL OPERATIONS
 * --------------------------
 */

int BB_or(BB *vector1, BB *vector2, BB* res)
{
    if (vector1 == NULL || res == NULL || vector2 == NULL)
        return ERR_NULL_ARG;

    if(vector_len(vector1) < vector_len(vector2))
        return BB_or(vector2, vector1, res);

    res->last_byte = vector1->last_byte;
    res->last_bit = vector1->last_bit;

    for(size_t part = 0; part <= res->last_byte; part++)
    {
        if (part <= vector2->last_byte)
            res->parts[part] = vector1->parts[part] | vector2->parts[part];
        else
            res->parts[part] = vector1->parts[part];
    }

    return ERR_OK;
};


int BB_and (BB *vector1, BB *vector2, BB* res)
{
    if (vector1 == NULL || res == NULL || vector2 == NULL)
        return ERR_NULL_ARG;

    if(vector_len(vector1) < vector_len(vector2))
        return BB_and(vector2, vector1, res);

    res->last_byte = vector2->last_byte;
    res->last_bit = vector2->last_bit;

    for(size_t part = 0; part <= res->last_byte; part++)
        res->parts[part] = vector1->parts[part] & vector2->parts[part];

    return ERR_OK;
};


int BB_xor (BB *vector1, BB* vector2, BB* res)
{
    if (vector1 == NULL || res == NULL || vector2 == NULL)
        return ERR_NULL_ARG;

    if(vector_len(vector1) < vector_len(vector2))
        return BB_xor(vector2, vector1, res);

    res->last_byte = vector1->last_byte;
    res->last_bit = vector1->last_bit;

    for(size_t part = 0; part <= res->last_byte; part++)
    {
        if (part <= vector2->last_byte)
            res->parts[part] = (vector1->parts[part] ^ vector2->parts[part]);
        else
            res->parts[part] = (vector1->parts[part] ^ 0);
    }

    return ERR_OK;
};


int BB_not (BB *vector, BB *res)
{
    if (vector == NULL || res == NULL)
        return ERR_NULL_ARG;

    res->last_bit = vector->last_bit;
    res->last_byte = vector->last_byte;

    for (int part = 0; part <= vector->last_byte; part++)
        res->parts[part] = ~(vector->parts[part]);

    return ERR_OK;
};


/* --------------------------
 * SHIFT OPERATIONS
 * --------------------------
 */

int BB_right_shift (BB *vector, BB *res, int len_shift)
{
    if (vector == NULL || res == NULL)
    return ERR_NULL_ARG;

    if (abs(len_shift) > vector_len(vector))
        return BB_null(res);

    if (len_shift < 0)
        return BB_left_shift(vector, res, abs(len_shift));

    if (!len_shift)
        return BB_create_copy(vector, res);

    size_t  vector_res_len = vector_len(vector) - len_shift;
    res->last_byte = vector_res_len / 8;
    res->last_bit = vector_res_len % 8;

    size_t dif_byte = len_shift / 8;
    size_t dif_bit = len_shift % 8;

    for (size_t part = 0; part <= (res->last_byte + 1); part++)
        res->parts[part] = vector->parts[part + dif_byte];

    for (size_t part = 0; part <= res->last_byte; part++)
    {
        res->parts[part] >>= dif_bit;
        uint8_t buf_part = res->parts[part + 1];
        buf_part <<= (8 - dif_bit);
        res->parts[part] |= buf_part;
    }
    res->parts[res->last_byte + 1] = 0;

    return ERR_OK;
}

int BB_left_shift (BB *vector, BB *res, int len_shift)
{
    if (vector == NULL || res == NULL)
        return ERR_NULL_ARG;

    if (abs(len_shift) > vector_len(vector))
        return BB_null(res);

    if (len_shift < 0)
        return BB_right_shift(vector, res, abs(len_shift));

    if (!len_shift)
        return BB_create_copy(vector, res);

    size_t  vector_res_len = vector_len(vector) - len_shift;
    res->last_byte = vector_res_len / 8;
    res->last_bit = vector_res_len % 8;

    for (size_t part = 0; part <= res->last_byte; part++)
        res->parts[part] = vector->parts[part];

    size_t bit_shift = 8 - res->last_bit;
    res->parts[res->last_byte] <<= bit_shift;
    res->parts[res->last_byte] >>= bit_shift;

    return ERR_OK;
}

int BB_left_cshift(BB *vector, BB *res, int len_shift)
{
    if (vector == NULL || res == NULL)
        return ERR_NULL_ARG;

    if (len_shift < 0)
        return BB_right_cshift(vector, res, abs(len_shift));

    if (len_shift == vector_len(vector) || len_shift == 0)
        return BB_create_copy(vector, res);

    BB *buf_vector = calloc(1, sizeof(BB));
    if (buf_vector == NULL)
        return ERR_MEM_NOT_ALLOC;

    len_shift %= vector_len(vector);
    BB_left_shift(vector, buf_vector, len_shift);
    BB_right_shift(vector, res, (vector_len(vector) - len_shift));

    for (size_t part = 0; part <= buf_vector->last_byte; part++)
        res->parts[res->last_byte + part + 1] = buf_vector->parts[part];

    for (size_t part = res->last_byte; part <= (vector->last_byte + 1); part++)
    {
        uint8_t buf_part = res->parts[part + 1];
        buf_part <<= (res->last_bit);
        res->parts[part] |= buf_part;
        res->parts[part + 1] >>= (8 - res->last_bit);
    }

    res->last_byte = vector->last_byte;
    res->last_bit = vector->last_bit;

    free(buf_vector);
    return ERR_OK;
}

int BB_right_cshift(BB *vector, BB *res, int len_shift)
{
    if (vector == NULL || res == NULL)
        return ERR_NULL_ARG;

    if (len_shift < 0)
        return BB_left_cshift(vector, res, abs(len_shift));

    if (len_shift == vector_len(vector) || len_shift == 0)
        return BB_create_copy(vector, res);

    len_shift %= vector_len(vector);
    size_t rev_len_shift = vector_len(vector) - len_shift;
    return BB_left_cshift(vector, res, rev_len_shift);
}
