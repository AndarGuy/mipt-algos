#include "bigint.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

BigInteger big_integer_add(BigInteger a, BigInteger b) {
    BigInteger result = {0};
    int a_size = big_integer_size(a), b_size = big_integer_size(b);
    char carry = 0;
    for (size_t i = 0; i < MAX(a_size, b_size) || carry != 0; i++) {
        int temp_a = (char)a.storage[i], temp_b = (char)b.storage[i];
        int temp = temp_a + temp_b + carry;
        if (carry != 0) carry = 0;
        if (temp >= 10) carry = 1;
        result.storage[i] = temp % 10;
    }
    return result;
}

BigInteger big_integer_multiply(BigInteger a, BigInteger b) {
    BigInteger result = {0};
    int a_size = big_integer_size(a), b_size = big_integer_size(b);
    int carry = 0;
    for (int i = 0; i < a_size; i++) {
        BigInteger addend = {0};
        for (int j = 0; j < b_size || carry != 0; j++) {
            int temp;
            temp = (a.storage[i] * b.storage[j]) + carry;

            assert(i + j < INTEGER_SIZE);

            addend.storage[i + j] = temp % 10;
            carry = temp / 10;
        }
        result = big_integer_add(result, addend);
    }
    return result;
}

void big_integer_print(BigInteger integer) {
    int size = big_integer_size(integer);
    for (size_t i = 0; i < size; i++) {
        printf("%hd", integer.storage[size - i - 1]);
    }
    if (size == 0) printf("0");
    printf(" ");
}

int big_integer_size(BigInteger integer) {
    for (size_t i = 0; i < INTEGER_SIZE; i++)
        if (integer.storage[INTEGER_SIZE - i - 1] != 0) return INTEGER_SIZE - i;
    return 0;
}