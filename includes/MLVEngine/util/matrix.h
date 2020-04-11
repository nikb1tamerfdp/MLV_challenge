#ifndef _MATRIX_
#define _MATRIX_

#include "MLVEngine/util/lang.h"

struct _matrix {
    ARRAY(double) terms;
    int m;
    int n;
};

typedef struct _matrix _Matrix;
typedef _Matrix* Matrix;

Matrix Matrix_new(int m, int n);
Matrix Matrix_new_filled(int m, int n, double value);
Matrix Matrix_new_line(int m);
Matrix Matrix_new_line_filled(int m, double value);
Matrix Matrix_new_column(int n);
Matrix Matrix_new_column_filled(int n, double value);
Matrix Matrix_new_square(int n);
Matrix Matrix_new_square_filled(int n, double value);
Matrix Matrix_new_diagonal(int n, double value);
Matrix Matrix_new_identity(int n);
Matrix Matrix_new_zero(int n);
Boolean Matrix_is_line(const Matrix self);
Boolean Matrix_is_column(const Matrix self);
Boolean Matrix_is_square(const Matrix self);
Boolean Matrix_is_diagonal(const Matrix self);
Boolean Matrix_is_identity(const Matrix self);
Boolean Matrix_is_zero(const Matrix self);
double Matrix_get(const Matrix self, int i, int j);
void Matrix_set(Matrix self, int i, int j, double value);
void Matrix_fill(Matrix self, double value);
Boolean Matrix_equals(const Matrix self, const Matrix other);
Matrix Matrix_copy(const Matrix self);
void Matrix_print(const Matrix self);
void Matrix_transpose(Matrix self);
Matrix Matrix_transposed(const Matrix matrix);
void Matrix_add_value(Matrix self, double value);
void Matrix_add(Matrix self, Matrix other);
Matrix Matrix_sum_value(const Matrix matrix, double value);
Matrix Matrix_sum(const Matrix a, const Matrix b);
void Matrix_multiply_value(Matrix self, double value);
void Matrix_multiply(Matrix self, Matrix other);
Matrix Matrix_product_value(const Matrix matrix, double value);
Matrix Matrix_product(const Matrix a, const Matrix b);
void Matrix_free(Matrix self);

#define N_LINES(matrix) (matrix -> m)
#define N_COLUMNS(matrix) (matrix -> n)
#define N_TERMS(matrix) (N_LINES(matrix) * N_COLUMNS(matrix))
#define TERMS(matrix) (matrix -> terms)
#define FOR_EACH_TERM(matrix) DOUBLE_RANGE(i, 0, N_LINES(matrix), 1, j, 0, N_COLUMNS(matrix), 1)
#define TERM(matrix, i, j) (TERMS(matrix)[(i) * N_COLUMNS(matrix) + (j)])

extern const Matrix MATRIX_IDENTITY;

#endif
