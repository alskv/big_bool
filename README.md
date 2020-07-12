Library for working with a Big Boolean vector
===
The library functions work with a fixed maximum vector size. Memory is handled by the caller. Byte order - little endian.

Implemented functions:

```
int BB_random (BB *res);
int BB_null(BB *vector);
int BB_change_size(BB *vector, BB *larger_vec);
size_t vector_len(BB *vector);
int BB_null(BB *vector);
int BB_create_copy(BB *vector, BB *res);

int BB_from_uint64_t(uint64_t num, BB *vector);
int BB_from_str(char* str, BB *vector );
int BB_to_str(BB *vector, char *str);

int BB_or (BB *vector1, BB *vector2, BB *res);
int BB_and (BB *vector1, BB *vector2, BB *res);
int BB_xor (BB *vector1, BB *vector2, BB *res);
int BB_not (BB *vector, BB *res);

int BB_right_shift (BB *vector, BB *res, int len_shift);
int BB_left_shift (BB *vector, BB *res, int len_shift);
int BB_right_cshift(BB *vector, BB *res, int len_shift);
int BB_left_cshift (BB *vector, BB *res, int len_shift);
```