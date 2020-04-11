#include <stdlib.h>
#include <string.h>

#include "MLVEngine/util/matrix.h"
#include "MLVEngine/util/contract.h"
#include "MLVEngine/util/error.h"
#include "MLVEngine/util/lang.h"

#define _FOR_EACH_TERM_FLAT(matrix) RANGE(i, 0, N_LINES(matrix) * N_COLUMNS(matrix), 1)
#define _TERM_FLAT(matrix, n) (TERMS(matrix)[n])

/* --- declaration of private helpers */
Boolean _Matrix_index_is_in_bounds(const Matrix self, int i, int j);
Boolean _Matrix_same_size(const Matrix a, const Matrix b);
void _Matrix_superficial_copy(Matrix dest, const Matrix src);

/* global identity matrix */
const double _IDENTITY_TERMS[] = { 1., 0., 0., 0., 1., 0., 0., 0., 1. };
const _Matrix _MATRIX_IDENTITY = { CAST(double*, _IDENTITY_TERMS), 3, 3 };
const Matrix MATRIX_IDENTITY = CAST(Matrix, &_MATRIX_IDENTITY);

Matrix Matrix_new(int m, int n)
{

    REQUIRE_THAT(m > 0 && n > 0);

    Matrix self = NEW(_Matrix);
    TERMS(self) = NEW_MULTIPLE(double, m * n);
    N_LINES(self) = m;
    N_COLUMNS(self) = n;

    return self;

}

Matrix Matrix_new_filled(int m, int n, double value)
{

    Matrix self = Matrix_new(m, n);
    Matrix_fill(self, value);

    return self;

}

Matrix Matrix_new_line(int m)
{
    return Matrix_new(m, 1);
}

Matrix Matrix_new_line_filled(int m, double value)
{
    return Matrix_new_filled(m, 1, value);
}

Matrix Matrix_new_column(int n)
{
    return Matrix_new(1, n);
}

Matrix Matrix_new_column_filled(int n, double value)
{
    return Matrix_new_filled(1, n, value);
}

Matrix Matrix_new_square(int n)
{
    return Matrix_new(n, n);
}

Matrix Matrix_new_square_filled(int n, double value)
{
    return Matrix_new_filled(n, n, value);
}

Matrix Matrix_new_diagonal(int n, double value)
{
    
    Matrix self = Matrix_new_zero(n);
    RANGE(i, 0, n, 1) { TERM(self, i, i) = value; }

    return self;

}

Matrix Matrix_new_identity(int n)
{
    return Matrix_new_diagonal(n, 1.);
}

Matrix Matrix_new_zero(int n)
{
    return Matrix_new_square_filled(n, 0.);
}

Boolean Matrix_is_line(const Matrix self)
{
    
    REQUIRE_NON_NULL(self);

    return N_LINES(self) == 1;

}

Boolean Matrix_is_column(const Matrix self)
{

    REQUIRE_NON_NULL(self);

    return N_COLUMNS(self) == 1;

}

Boolean Matrix_is_square(const Matrix self)
{

    REQUIRE_NON_NULL(self);

    return N_LINES(self) == N_COLUMNS(self);

}

Boolean Matrix_is_diagonal(const Matrix self)
{

    REQUIRE_NON_NULL(self);

    FOR_EACH_TERM(self)
    {
        double term = TERM(self, i, j);
        if ((i != j) && !FLOAT_EQUALS(term, 0.)) return FALSE;
    }

    return TRUE;

}

Boolean Matrix_is_identity(const Matrix self)
{

    REQUIRE_NON_NULL(self);

    FOR_EACH_TERM(self)
    {
        double term = TERM(self, i, j);
        if ((i == j) && !FLOAT_EQUALS(term, 1.)) return FALSE;
        if ((i != j) && !FLOAT_EQUALS(term, 0.)) return FALSE;
    }

    return TRUE;

}

Boolean Matrix_is_zero(const Matrix self)
{

    REQUIRE_NON_NULL(self);

    FOR_EACH_TERM(self)
        if (TERM(self, i, j) != 0.)
            return FALSE;

    return TRUE;

}

double Matrix_get(const Matrix self, int i, int j)
{

    REQUIRE_NON_NULL(self);

    if (!_Matrix_index_is_in_bounds(self, i, j))
        THROW_AND_KILL("Index out of bounds !", "Cannot get term (%d, %d) of matrix with size (%d, %d)", i, j, self -> m, self -> n);

    return TERM(self, i - 1, j - 1);

}

void Matrix_set(Matrix self, int i, int j, double value)
{
    
    REQUIRE_NON_NULL(self);

    if (!_Matrix_index_is_in_bounds(self, i, j))
        THROW_AND_KILL("Index out of bounds !", "Cannot set term (%d, %d) of matrix with size (%d, %d)", i, j, self -> m, self -> n);

    TERM(self, i - 1, j - 1) = value;

}

void Matrix_fill(Matrix self, double value)
{

    REQUIRE_NON_NULL(self);

    FOR_EACH_TERM(self)
        TERM(self, i, j) = value;

}

Boolean Matrix_equals(const Matrix self, const Matrix other)
{

    if (self == other)
        return TRUE;

    if (IS_NULL(self) || IS_NULL(other))
        return FALSE;

    if (!_Matrix_same_size(self, other))
        return FALSE;
    
    FOR_EACH_TERM(self)
    {
        double a = TERM(self, i, j);
        double b = TERM(other, i, j);
        if (!FLOAT_EQUALS(a, b)) return FALSE;
    }

    return TRUE;

}

Matrix Matrix_copy(const Matrix self)
{

    REQUIRE_NON_NULL(self);

    Matrix copy = Matrix_new(N_LINES(self), N_COLUMNS(self));
    memcpy(TERMS(copy), TERMS(self), SIZE_ARRAY(double, N_TERMS(self)));

    return copy;

}

void Matrix_print(const Matrix self)
{
    
    printf("Matrix(%d, %d) [\n", N_LINES(self), N_COLUMNS(self));

    FOR_EACH_TERM(self)
    {
        printf("  %.2f", TERM(self, i, j));
        if (j == (N_COLUMNS(self) - 1)) printf("\n");
    }

    printf("]\n");

}

void Matrix_transpose(Matrix self)
{
    
    REQUIRE_NON_NULL(self);

    if (Matrix_is_square(self))
    {
        
        DOUBLE_RANGE(
            i, 0, N_COLUMNS(self) - 1, 1,
            j, 1, N_COLUMNS(self), 1
        ) {
            double tmp = TERM(self, i, j);
            TERM(self, i, j) = TERM(self, j, i);
            TERM(self, j, i) = tmp;
        }

        return;

    }

    Matrix transposed = Matrix_transposed(self);

    FREE(TERMS(self));
    _Matrix_superficial_copy(self, transposed);
    FREE(transposed);

}

Matrix Matrix_transposed(const Matrix matrix)
{
    
    REQUIRE_NON_NULL(matrix);

    Matrix transposed = Matrix_new(N_COLUMNS(matrix), N_LINES(matrix));
    FOR_EACH_TERM(matrix)
        TERM(transposed, j, i) = TERM(matrix, i, j);

    return transposed;

}

void Matrix_add_value(Matrix self, double value)
{
    
    REQUIRE_NON_NULL(self);

    FOR_EACH_TERM(self)
        TERM(self, i, j) += value;

}

void Matrix_add(Matrix self, Matrix other)
{
    
    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(other);

    if (!_Matrix_same_size(self, other))
        THROW_AND_KILL("Size of the two matrices must be the same !",
                       "Cannot add matrix (%d, %d) to matrix (%d, %d)",
                       N_LINES(other), N_COLUMNS(other),
                       N_LINES(self), N_COLUMNS(self));
    
    _FOR_EACH_TERM_FLAT(self)
        _TERM_FLAT(self, i) += _TERM_FLAT(other, i);

}

Matrix Matrix_sum_value(const Matrix matrix, double value)
{

    REQUIRE_NON_NULL(matrix);

    Matrix result = Matrix_copy(matrix);
    Matrix_add_value(result, value);

    return result;

}

Matrix Matrix_sum(const Matrix a, const Matrix b)
{
    
    REQUIRE_NON_NULL(a);
    REQUIRE_NON_NULL(b);

    Matrix result = Matrix_copy(a);
    Matrix_add(result, b);

    return result;

}

void Matrix_multiply_value(Matrix self, double value)
{
    
    REQUIRE_NON_NULL(self);

    FOR_EACH_TERM(self)
        TERM(self, i, j) *= value;
    
}

void Matrix_multiply(Matrix self, Matrix other)
{
    
    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(other);

    if (!_Matrix_same_size(self, other))
        THROW_AND_KILL("Number of columns of the first matrix must be equal to the number of rows in the second !",
                       "Cannot multiply matrix (%d, %d) to matrix (%d, %d)",
                       N_LINES(other), N_COLUMNS(other),
                       N_LINES(self), N_COLUMNS(self));

    Matrix result = Matrix_new(N_LINES(self), N_COLUMNS(other));

    FOR_EACH_TERM(result)
    {
        double sum = 0.;
        RANGE(k, 0, N_COLUMNS(self), 1)
            sum = sum + TERM(self, i, k) * TERM(other, k, j);

        TERM(result, i, j) = sum;

    }

    FREE(TERMS(self));
    _Matrix_superficial_copy(self, result);
    FREE(result);

}

Matrix Matrix_product_value(const Matrix matrix, double value)
{
    
    REQUIRE_NON_NULL(matrix);

    Matrix result = Matrix_copy(matrix);
    Matrix_multiply_value(result, value);

    return result;

}

Matrix Matrix_product(const Matrix a, const Matrix b)
{
    
    REQUIRE_NON_NULL(a);
    REQUIRE_NON_NULL(b);

    Matrix result = Matrix_copy(a);
    Matrix_multiply(result, b);

    return result;

}

void Matrix_free(Matrix self)
{
    
    ENSURE_FREEABLE(self);

    FREE(TERMS(self));
    FREE(self);

}

/* --- private helpers */

Boolean _Matrix_index_is_in_bounds(const Matrix self, int i, int j)
{
    return !((i < 1) || (i > N_LINES(self))) ||  (j < 1) || (j > N_COLUMNS(self));
}

Boolean _Matrix_same_size(const Matrix a, const Matrix b)
{
    return (N_LINES(a) == N_LINES(b)) && (N_COLUMNS(a) == N_COLUMNS(b));
}

void _Matrix_superficial_copy(Matrix dest, const Matrix src)
{

    TERMS(dest) = TERMS(src);
    N_LINES(dest) = N_LINES(src);
    N_COLUMNS(dest) = N_COLUMNS(src);

}
