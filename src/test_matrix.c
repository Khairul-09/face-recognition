/**
 * @file test_matrix.c
 *
 * Test suite for the matrix library.
 */
#include <stdio.h>
#include "matrix.h"

#define ROWS 6
#define COLS 6

typedef void (*test_func_t)(void);

/**
 * Helper function to fill a matrix with a constant value.
 */
void fill_matrix_constant(matrix_t *M, precision_t c)
{
	int i, j;
	for ( i = 0; i < M->rows; i++ ) {
		for ( j = 0; j < M->cols; j++ ) {
			elem(M, i, j) = c;
		}
	}
}

/**
 * Helper function to fill a matrix with arbitrary data.
 */
void fill_matrix_data(matrix_t *M, precision_t data[][M->cols])
{
	int i, j;
	for ( i = 0; i < M->rows; i++ ) {
		for ( j = 0; j < M->cols; j++ ) {
			elem(M, i, j) = data[i][j];
		}
	}
}

/**
 * Helper function to fill a matrix with an increasing value.
 */
void fill_matrix_linear(matrix_t *M)
{
	int i, j;
	for ( i = 0; i < M->rows; i++ ) {
		for ( j = 0; j < M->cols; j++ ) {
			elem(M, i, j) = j * M->rows + i;
		}
	}
}

/**
 * Test the matrix initializers.
 */
void test_m_initialize()
{
	// identity matrix
	matrix_t *M = m_identity(ROWS);

	printf("m_identity (%d) = \n", ROWS);
	m_fprint(stdout, M);

	m_free(M);

	// zero matrix
	M = m_zeros(ROWS, COLS);

	printf("m_zeros (%d, %d) = \n", ROWS, COLS);
	m_fprint(stdout, M);

	m_free(M);
}

/**
 * Test the matrix copy constructor.
 */
void test_m_copy()
{
	matrix_t *M = m_initialize(ROWS, COLS);
	fill_matrix_linear(M);

	printf("M = \n");
	m_fprint(stdout, M);

	matrix_t *C1 = m_copy(M);
	matrix_t *C2 = m_copy_columns(M, 1, COLS - 2);

	printf("C1 = \n");
	m_fprint(stdout, C1);
	printf("C2 = \n");
	m_fprint(stdout, C2);

	m_free(M);
	m_free(C1);
	m_free(C2);
}

/**
 * Test the matrix convariance.
 */
void test_m_covariance()
{
	precision_t data[][3] = {
		{ 5, 1, 4 },
		{ 0, -5, 9 },
		{ 3, 7, 8 },
		{ 7, 3, 10 }
	};

	matrix_t *M = m_initialize(4, 3);

	fill_matrix_data(M, data);

	matrix_t *C = m_covariance(M);

	printf("M = \n");
	m_fprint(stdout, M);
	printf("m_covariance(M) = \n");
	m_fprint(stdout, C);

	m_free(M);
	m_free(C);
}

/**
 * Test the vector distance functions.
 */
void test_m_distance()
{
	precision_t data[][2] = {
		{ 1, 0 },
		{ 0, 1 },
		{ 0, 0 }
	};

	matrix_t *M = m_zeros(3, 2);

	fill_matrix_data(M, data);

	printf("M = \n");
	m_fprint(stdout, M);

	printf("d_COS(M[0], M[1]) = %lf\n", m_dist_COS(M, 0, M, 1));
	printf("d_L1(M[0], M[1]) = %lf\n", m_dist_L1(M, 0, M, 1));
	printf("d_L2(M[0], B[1]) = %lf\n", m_dist_L2(M, 0, M, 1));

	m_free(M);
}

/**
 * Test eigenvalues, eigenvectors.
 */
void test_m_eigenvalues_eigenvectors()
{
	precision_t data[][3] = {
		{ 2, 0, 0 },
		{ 0, 3, 4 },
		{ 0, 4, 9 }
	};

	matrix_t *M = m_initialize(3, 3);
	matrix_t *M_eval = m_initialize(3, 1);
	matrix_t *M_evec = m_initialize(3, 3);

	fill_matrix_data(M, data);

	m_eigenvalues_eigenvectors(M, M_eval, M_evec);

	printf("M = \n");
	m_fprint(stdout, M);

	printf("eigenvalues of M = \n");
	m_fprint(stdout, M_eval);

	printf("eigenvectors of M = \n");
	m_fprint(stdout, M_evec);

	m_free(M);
	m_free(M_eval);
	m_free(M_evec);
}

/**
 * Test matrix inverse.
 */
void test_m_inverse()
{
	// identity matrix
    matrix_t *M = m_identity(ROWS);
	matrix_t *M_inv = m_inverse(M);
	matrix_t *M_prod = m_product(M, M_inv);

    printf("M = \n");
    m_fprint(stdout, M);
    printf("M^-1 = \n");
    m_fprint(stdout, M_inv);
    printf("M * M^-1 = \n");
    m_fprint(stdout, M_prod);

    m_free(M);
	m_free(M_inv);
	m_free(M_prod);

	// 3-by-3 matrix, arbitrary data
	precision_t data[][3] = {
		{ 4, 1, 1 },
		{ 2, 1, -1 },
		{ 1, 1, 1 }
	};
	M = m_initialize(3, 3);
	fill_matrix_data(M, data);

	M_inv = m_inverse(M);
	M_prod = m_product(M, M_inv);

    printf("M = \n");
    m_fprint(stdout, M);
    printf("M^-1 = \n");
    m_fprint(stdout, M_inv);
    printf("M * M^-1 = \n");
    m_fprint(stdout, M_prod);

    m_free(M);
	m_free(M_inv);
	m_free(M_prod);

	// TODO: this test does not provide the correct inverse
	// ROWS-by-ROWS matrix, linear fill
	M = m_initialize(ROWS, ROWS);
	fill_matrix_linear(M);

	M_inv = m_inverse(M);
	M_prod = m_product(M, M_inv);

    fprintf(stdout, "M = \n");
    m_fprint(stdout, M);
    fprintf(stdout, "M^-1 = \n");
    m_fprint(stdout, M_inv);
    fprintf(stdout, "M * M^-1 = \n");
    m_fprint(stdout, M_prod);

    m_free(M);
	m_free(M_inv);
	m_free(M_prod);
}

/**
 * Test matrix mean column.
 */
void test_m_mean_column()
{
	matrix_t *M = m_identity(ROWS);
	matrix_t *a = m_mean_column(M);

	printf("M = \n");
	m_fprint(stdout, M);

	printf("m_mean_column (M) = \n");
	m_fprint(stdout, a);

	m_free(M);
	m_free(a);
}

/**
 * Test matrix product.
 */
void test_m_product()
{
	matrix_t *A = m_initialize(ROWS, COLS + 2);
	matrix_t *B = m_initialize(COLS + 2, COLS + 1);

	fill_matrix_linear(A);
	fill_matrix_linear(B);

	matrix_t *M = m_product(A, B);

	printf("A = \n");
	m_fprint(stdout, A);

	printf("B = \n");
	m_fprint(stdout, B);

	printf("A * B = \n");
	m_fprint(stdout, M);

	m_free(A);
	m_free(B);
	m_free(M);
}

/**
 * Test matrix square root.
 */
void test_m_sqrtm()
{
	precision_t data[][2] = {
		{ 7, 10 },
		{ 15, 22 }
	};
	matrix_t *M = m_initialize(2, 2);

	fill_matrix_data(M, data);

	matrix_t *X = m_sqrtm(M);
	matrix_t *X_sq = m_product(X, X);

	printf("M = \n");
	m_fprint(stdout, M);
	printf("X = m_sqrtm(M) = \n");
	m_fprint(stdout, X);
	printf("X * X = \n");
	m_fprint(stdout, X_sq);

	m_free(M);
	m_free(X);
}

/**
 * Test matrix transpose.
 */
void test_m_transpose()
{
	matrix_t *M = m_zeros(ROWS + 2, COLS);
	fill_matrix_linear(M);

	matrix_t *M_tr = m_transpose(M);

	printf("M = \n");
	m_fprint(stdout, M);

	printf("m_transpose (M) = \n");
	m_fprint(stdout, M_tr);

	m_free(M);
	m_free(M_tr);
}

/**
 * Test matrix addition and subtraction.
 */
void test_m_add_subtract()
{
	matrix_t *A1 = m_initialize(ROWS, COLS);
	matrix_t *A2 = m_initialize(ROWS, COLS);
	matrix_t *B = m_initialize(ROWS, COLS);

	fill_matrix_linear(A1);
	fill_matrix_linear(A2);
	fill_matrix_linear(B);

	printf("A = \n");
	m_fprint(stdout, A1);
	printf("B = \n");
	m_fprint(stdout, B);

	m_add(A1, B);
	m_subtract(A2, B);

	printf("A + B = \n");
	m_fprint(stdout, A1);

	printf("A - B = \n");
	m_fprint(stdout, A2);

	m_free(A1);
	m_free(A2);
	m_free(B);
}

/**
 * Test matrix multiplication by scalar.
 */
void test_m_elem_mult()
{
	matrix_t *M = m_initialize(ROWS, COLS);
	precision_t c = 2.0;

	fill_matrix_linear(M);

	printf("M = \n");
	m_fprint(stdout, M);

	m_elem_mult(M, c);

	printf("%g * M = \n", c);
	m_fprint(stdout, M);

	m_free(M);
}

/**
 * Test matrix column normalization.
 */
void test_m_subtract_columns()
{
	matrix_t *M = m_initialize(ROWS, COLS);
	matrix_t *a = m_initialize(ROWS, 1);

	fill_matrix_linear(M);
	fill_matrix_linear(a);

	printf("M = \n");
	m_fprint(stdout, M);

	printf("a = \n");
	m_fprint(stdout, a);

	m_subtract_columns(M, a);

	printf("m_subtract_columns (M, a) = \n");
	m_fprint(stdout, M);

	m_free(M);
	m_free(a);
}

int main (int argc, char **argv)
{
	test_func_t tests[] = {
		test_m_initialize,
		test_m_copy,
		test_m_covariance,
		test_m_distance,
		test_m_eigenvalues_eigenvectors,
		test_m_inverse,
		test_m_mean_column,
		test_m_product,
		test_m_sqrtm,
		test_m_transpose,
		test_m_add_subtract,
		test_m_elem_mult,
		test_m_subtract_columns
	};
	int num_tests = sizeof(tests) / sizeof(test_func_t);

	int i;
	for ( i = 0; i < num_tests; i++ ) {
		test_func_t test = tests[i];

		test();
		putchar('\n');
	}

/*
	fprintf (output, "\n-------------Test Group 2.0.0 -------------\n");
	m_flipCols (M);
	fprintf (output, "m_flipCols(M) = \n");
	m_fprint (output, M);
	m_free (M);

	M = m_initialize (FILL, ROWS, COLS);
	m_normalize (M);
	fprintf (output, "m_normalize(M) = \n");
	m_fprint (output, M);
	m_free (M);

	fprintf (output, "\n-------------Test Group 2.1.2 -------------\n");
	matrix_t *A = m_initialize (FILL, ROWS, COLS);
	fprintf (output, "A = \n");
	m_fprint (output, A);

	R = m_reshape (A, ROWS / 2, COLS * 2);
	fprintf (output, "m_reshape (A, ROWS / 2, COLS * 2) = \n");
	m_fprint (output, R);
	m_free (R);

	fprintf (output, "\n-------------Test Group 5 -------------\n");

	matrix_t *V = m_initialize (UNDEFINED, 1, 6);
	V->data[0] = 4; V->data[1] = 5; V->data[2] = 2; V->data[3] = 1;
	V->data[4] = 0; V->data[5] = 3;
	fprintf (output, "V = \n");
	m_fprint (output, V);

	R = m_reorder_columns (M, V);
	fprintf (output, "m_reorderCols (M, V) = \n");
	m_fprint (output, R);
	m_free (R);
*/

	return 0;
}