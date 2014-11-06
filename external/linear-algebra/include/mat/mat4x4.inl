/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

/*
 * mat4x4 implementation using (row-major ordered) array
 */

/**
 * TODO comment
 */
template<typename T>
inline matrix4x4<T> operator*(const matrix4x4<T> & matA, const matrix4x4<T> & matB)
{
	return(matrix4x4<T>((matA[0][0]*matB[0][0]) + (matA[0][1]*matB[1][0]) + (matA[0][2]*matB[2][0]) + (matA[0][3]*matB[3][0]),
						(matA[0][0]*matB[0][1]) + (matA[0][1]*matB[1][1]) + (matA[0][2]*matB[2][1]) + (matA[0][3]*matB[3][1]),
						(matA[0][0]*matB[0][2]) + (matA[0][1]*matB[1][2]) + (matA[0][2]*matB[2][2]) + (matA[0][3]*matB[3][2]),
						(matA[0][0]*matB[0][3]) + (matA[0][1]*matB[1][3]) + (matA[0][2]*matB[2][3]) + (matA[0][3]*matB[3][3]),

						(matA[1][0]*matB[0][0]) + (matA[1][1]*matB[1][0]) + (matA[1][2]*matB[2][0]) + (matA[1][3]*matB[3][0]),
						(matA[1][0]*matB[0][1]) + (matA[1][1]*matB[1][1]) + (matA[1][2]*matB[2][1]) + (matA[1][3]*matB[3][1]),
						(matA[1][0]*matB[0][2]) + (matA[1][1]*matB[1][2]) + (matA[1][2]*matB[2][2]) + (matA[1][3]*matB[3][2]),
						(matA[1][0]*matB[0][3]) + (matA[1][1]*matB[1][3]) + (matA[1][2]*matB[2][3]) + (matA[1][3]*matB[3][3]),

						(matA[2][0]*matB[0][0]) + (matA[2][1]*matB[1][0]) + (matA[2][2]*matB[2][0]) + (matA[2][3]*matB[3][0]),
						(matA[2][0]*matB[0][1]) + (matA[2][1]*matB[1][1]) + (matA[2][2]*matB[2][1]) + (matA[2][3]*matB[3][1]),
						(matA[2][0]*matB[0][2]) + (matA[2][1]*matB[1][2]) + (matA[2][2]*matB[2][2]) + (matA[2][3]*matB[3][2]),
						(matA[2][0]*matB[0][3]) + (matA[2][1]*matB[1][3]) + (matA[2][2]*matB[2][3]) + (matA[2][3]*matB[3][3]),

						(matA[3][0]*matB[0][0]) + (matA[3][1]*matB[1][0]) + (matA[3][2]*matB[2][0]) + (matA[3][3]*matB[3][0]),
						(matA[3][0]*matB[0][1]) + (matA[3][1]*matB[1][1]) + (matA[3][2]*matB[2][1]) + (matA[3][3]*matB[3][1]),
						(matA[3][0]*matB[0][2]) + (matA[3][1]*matB[1][2]) + (matA[3][2]*matB[2][2]) + (matA[3][3]*matB[3][2]),
						(matA[3][0]*matB[0][3]) + (matA[3][1]*matB[1][3]) + (matA[3][2]*matB[2][3]) + (matA[3][3]*matB[3][3])));
}

/**
 * M * v
 */
template<typename T>
vector4<T> operator*(const matrix4x4<T> & mat, const vector4<T> & vec)
{
	return(vector4<T>(mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z + mat[0][3] * vec.w,
					  mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z + mat[1][3] * vec.w,
					  mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z + mat[2][3] * vec.w,
					  mat[3][0] * vec.x + mat[3][1] * vec.y + mat[3][2] * vec.z + mat[3][3] * vec.w));
}

/**
 * transpose(M) * v
 */
template<typename T>
vector4<T> operator*(const vector4<T> & vec, const matrix4x4<T> & mat)
{
	return(vector4<T>(vec.x * mat[0][0] + vec.y * mat[0][1] + vec.z * mat[0][2] + vec.w * mat[0][3],
					  vec.x * mat[1][0] + vec.y * mat[1][1] + vec.z * mat[1][2] + vec.w * mat[1][3],
					  vec.x * mat[2][0] + vec.y * mat[2][1] + vec.z * mat[2][2] + vec.w * mat[2][3],
					  vec.x * mat[3][0] + vec.y * mat[3][1] + vec.z * mat[3][2] + vec.w * mat[3][3]));
}

/**
 * TODO comment
 */
template<typename T>
inline matrix4x4<T> operator*(T s, const matrix4x4<T> & mat)
{
	return(matrix4x4<T>(s * mat[0], s * mat[1], s * mat[2], s * mat[3]));
}

/**
 * TODO comment
 */
template<typename T>
inline matrix4x4<T> operator*(const matrix4x4<T> & mat, T s)
{
	return(matrix4x4<T>(mat[0] * s, mat[1] * s, mat[2] * s, mat[3] * s));
}

/**
 * TODO comment
 */
template<typename T>
inline matrix4x4<T> operator/(const matrix4x4<T> & mat, T s)
{
	return(matrix4x4<T>(mat[0] / s, mat[1] / s, mat[2] / s, mat[3] / s));
}

/**
 * calculate the determinant of a matrix
 */
template<typename T>
inline T determinant(const matrix4x4<T> & mat)
{
	// TODO : factorize

	T r1  = mat[0][0]*mat[1][1]*mat[2][2]*mat[3][3];
	T r2  = mat[0][0]*mat[1][2]*mat[2][3]*mat[3][1];
	T r3  = mat[0][0]*mat[1][3]*mat[2][1]*mat[3][2];

	T r4  = mat[0][1]*mat[1][0]*mat[2][3]*mat[3][2];
	T r5  = mat[0][1]*mat[1][2]*mat[2][0]*mat[3][3];
	T r6  = mat[0][1]*mat[1][3]*mat[2][2]*mat[3][0];

	T r7  = mat[0][2]*mat[1][0]*mat[2][1]*mat[3][3];
	T r8  = mat[0][2]*mat[1][1]*mat[2][3]*mat[3][0];
	T r9  = mat[0][2]*mat[1][3]*mat[2][0]*mat[3][1];

	T r10 = mat[0][3]*mat[1][0]*mat[2][2]*mat[3][1];
	T r11 = mat[0][3]*mat[1][1]*mat[2][0]*mat[3][2];
	T r12 = mat[0][3]*mat[1][2]*mat[2][1]*mat[3][0];

	T r13 = mat[0][0]*mat[1][1]*mat[2][3]*mat[3][2];
	T r14 = mat[0][0]*mat[1][2]*mat[2][1]*mat[3][3];
	T r15 = mat[0][0]*mat[1][3]*mat[2][2]*mat[3][1];

	T r16 = mat[0][1]*mat[1][0]*mat[2][2]*mat[3][3];
	T r17 = mat[0][1]*mat[1][2]*mat[2][3]*mat[3][0];
	T r18 = mat[0][1]*mat[1][3]*mat[2][0]*mat[3][2];

	T r19 = mat[0][2]*mat[1][0]*mat[2][3]*mat[3][1];
	T r20 = mat[0][2]*mat[1][1]*mat[2][0]*mat[3][3];
	T r21 = mat[0][2]*mat[1][3]*mat[2][1]*mat[3][0];

	T r22 = mat[0][3]*mat[1][0]*mat[2][1]*mat[3][2];
	T r23 = mat[0][3]*mat[1][1]*mat[2][2]*mat[3][0];
	T r24 = mat[0][3]*mat[1][2]*mat[2][0]*mat[3][1];

	return( r1 +  r2 +  r3 +  r4 +  r5 +  r6 +  r7 +  r8 +  r9 + r10 + r11 + r12
		 - r13 - r14 - r15 - r16 - r17 - r18 - r19 - r20 - r21 - r22 - r23 - r24);
}

/**
 * calculate the inverse of a matrix
 */
template<typename T>
inline matrix4x4<T> inverse(const matrix4x4<T> & mat)
{
	// TODO
	return(matrix4x4<T>());
}

/**
 * calculate the outer product of a pair of vectors
 */
template<typename T>
inline matrix4x4<T> outerProduct(const vector4<T> & c, const vector4<T> & r)
{
	return(matrix4x4<T>(c.x * r.x, c.x * r.y, c.x * r.z, c.x * r.w,
				  c.y * r.x, c.y * r.y, c.y * r.z, c.y * r.w,
				  c.z * r.x, c.z * r.y, c.z * r.z, c.z * r.w,
				  c.w * r.x, c.w * r.y, c.w * r.z, c.w * r.w));
}

/**
 * calculate the transpose of a matrix
 */
template<typename T>
inline matrix4x4<T> transpose(const matrix4x4<T> & mat)
{
	return(matrix4x4<T>(mat[0][0], mat[1][0], mat[2][0], mat[3][0],
						mat[0][1], mat[1][1], mat[2][1], mat[3][1],
						mat[0][2], mat[1][2], mat[2][2], mat[3][2],
						mat[0][3], mat[1][3], mat[2][3], mat[3][3]));
}

/**
 * perform a component-wise multiplication of two matrices
 */
template<typename T>
inline matrix4x4<T> matrixCompMult(const matrix4x4<T> & matA, const matrix4x4<T> & matB)
{
	return(matrix4x4<T>((matA[0]*matB[0]), (matA[1]*matB[1]), (matA[2]*matB[2]), (matA[3]*matB[3])));
}
