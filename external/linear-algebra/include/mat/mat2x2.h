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

#pragma once

template<typename T>
struct matrix2x2
{

	inline explicit matrix2x2 (void)
	{
		m[0] = 0.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
	}

	inline explicit matrix2x2 (T f)
	{
		m[0] = f;
		m[1] = 0.0f;
		m[2] = f;
		m[3] = 0.0f;
	}

	inline explicit matrix2x2 (T a, T b, T c, T d)
	{
		m[0] = a;
		m[1] = b;
		m[2] = c;
		m[3] = d;
	}

	inline explicit matrix2x2 (const vector2<T> & v1, const vector2<T> & v2)
	{
		m[0] = v1.x;
		m[1] = v1.y;
		m[2] = v2.x;
		m[3] = v2.y;
	}

	inline vector2<T> & operator [] (unsigned int row)
	{
		return *(((vector2<T>*)&m) + row);
	}

	inline const vector2<T> & operator [] (unsigned int row) const
	{
		return *(((vector2<T>*)&m) + row);
	}

private:

	T m [4]; // row-major

};

#include "mat2x2.inl"

typedef		matrix2x2<float>	 mat2x2;
typedef		matrix2x2<int>		imat2x2;
typedef		matrix2x2<double>	dmat2x2;
