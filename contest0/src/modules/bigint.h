#define INTEGER_SIZE 512

typedef struct BigInteger {
    unsigned char storage[INTEGER_SIZE];
} BigInteger;

int big_integer_size(BigInteger integer);

void big_integer_print(BigInteger integer);

BigInteger big_integer_multiply(BigInteger a, BigInteger b);

BigInteger big_integer_add(BigInteger a, BigInteger b);