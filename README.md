# decimal.h
### Implementation of d3_decimal.h library in the C programming language. This library add the ability to work with the "decimal" type, which is not in the language standard. It's used for financial calculations, for example.

   This is an educational project, so it's no longer supported!

## Important notes

 - The library developed in C language of C11 standard using gcc compiler as a static library with the header file d3_decimal.h(see `d3_decimal.a` goal in Makefile)
 - The library's code, including headers, makefile and library itself located in the src folder
 - Written code follows the Google style. Legacy and outdated functions are not used according to POSIX.1-2017 standard(see `clang` goal in Makefile).
 - Integration tests covered all of the library's functions by unit-tests using the `check.h` library. Unit-tests checks the results of implementation by comparing them with the implementation of the standard decimal.h library(see `test` goal in Makefile). 
 - Unit tests coverage checked using gcov. It provides a gcov report in the form of an html page(see `gcov_report` goal in Makefile).
 - The programs developed according to the principles of structured programming, duplication in the code is avoided
- Trailing zeros preserved as deleted (except for the `d3_truncate` function)
- The defined type must support numbers from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335.

## Information

The Decimal value type represents decimal numbers ranging from positive 79,228,162,514,264,337,593,543,950,335 to negative 79,228,162,514,264,337,593,543,950,335. The default value of a Decimal is 0. The Decimal value type is appropriate for financial calculations that require large numbers of significant integral and fractional digits and no round-off errors. The Decimal type does not eliminate the need for rounding. Rather, it minimizes errors due to rounding.

### Binary representation

The binary representation of a Decimal number consists of a 1-bit sign, a 96-bit integer number, and a scaling factor used to divide the integer number and specify what portion of it is a decimal fraction. The scaling factor is implicitly the number 10, raised to an exponent ranging from 0 to 28.

Decimal number can be implemented as a four-element array of 32-bit signed integers (`int bits[4];`).

`bits[0]`, `bits[1]`, and `bits[2]` contain the low, middle, and high 32 bits of the 96-bit integer number accordingly.

`bits[3]` contains the scale factor and sign, and consists of following parts:
- Bits 0 to 15, the lower word, are unused and must be zero.
- Bits 16 to 23 must contain an exponent between 0 and 28, which indicates the power of 10 to divide the integer number.
- Bits 24 to 30 are unused and must be zero.
- Bit 31 contains the sign; 0 meaning positive, and 1 meaning negative.

Note that the bit representation differentiates between negative and positive zero. These values can be treated as being equal in all operations.

### Example:

```c
typedef struct 
{
    int bits[4];
} d3_decimal;
```

## Everything that was implemented in the decimal.h library is located down below:

### Arithmetic Operators
- Addition
- Subtraction
- Multiplication
- Division
- Modulo/Mod

- *When getting numbers that do not fit into the mantissa during arithmetic operations, use bank rounding (for example, 79,228,162,514,264,337,593,543,950,335 - 0.6 = 79,228,162,514,264,337,593,543,950,334)*

### Comparison Operators

- Less than | <
- Less than or equal to | <=
- Greater than | >
- Greater than or equal to | >=
- Equal to | ==
- Not equal to | !=

### Convertors and parsers

- From int
- From float
- To int
- To float

*Note on the conversion of a float type number:*
- *If the numbers are too small (0 < |x| < 1e-28), return an error and value equal to 0*
- *If the numbers are too large (|x| > 79,228,162,514,264,337,593,543,950,335) or are equal to infinity, return an error*
- *When processing a number with the float type, convert all the significant decimal digits contained in it. If there are more than 7 such digits, the number is rounded to the closest one that does not have more than 7 significant decimal digits.*

*Note on the conversion from decimal type to int:*
- *If there is a fractional part in a decimal number, it should be discarded (for example, 0.9 is converted to 0)*


### Another functions

Rounds a specified Decimal number to the closest integer toward negative infinity. 
Rounds a decimal value to the nearest integer.
Returns the integral digits of the specified Decimal; any fractional digits are discarded, including trailing zeroes.
Returns the result of multiplying the specified Decimal value by negative one.
