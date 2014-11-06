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
struct matrix4x4
{

	explicit matrix4x4 (void)
	{
		m[ 0] = 0.0f;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0.0f;
		m[ 5] = 0.0f;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = 0.0f;
		m[ 9] = 0.0f;
		m[10] = 0.0f;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 0.0f;
	}

	explicit matrix4x4 (T f)
	{
		m[ 0] = f;
		m[ 1] = 0.0f;
		m[ 2] = 0.0f;
		m[ 3] = 0.0f;
		m[ 4] = 0.0f;
		m[ 5] = f;
		m[ 6] = 0.0f;
		m[ 7] = 0.0f;
		m[ 8] = 0.0f;
		m[ 9] = 0.0f;
		m[10] = f;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = f;
	}

	explicit matrix4x4 (T a, T b, T c, T d, T e, T f, T g, T h, T i, T j, T k, T l, T n, T o, T p, T q)
	{
		m[ 0] = a;
		m[ 1] = b;
		m[ 2] = c;
		m[ 3] = d;
		m[ 4] = e;
		m[ 5] = f;
		m[ 6] = g;
		m[ 7] = h;
		m[ 8] = i;
		m[ 9] = j;
		m[10] = k;
		m[11] = l;
		m[12] = n;
		m[13] = o;
		m[14] = p;
		m[15] = q;
	}

	explicit matrix4x4 (const vector4<T> & v1, const vector4<T> & v2, const vector4<T> & v3, const vector4<T> & v4)
	{
		m[ 0] = v1.x;
		m[ 1] = v1.y;
		m[ 2] = v1.z;
		m[ 3] = v1.w;
		m[ 4] = v2.x;
		m[ 5] = v2.y;
		m[ 6] = v2.z;
		m[ 7] = v2.w;
		m[ 8] = v3.x;
		m[ 9] = v3.y;
		m[10] = v3.z;
		m[11] = v3.w;
		m[12] = v4.x;
		m[13] = v4.y;
		m[14] = v4.z;
		m[15] = v4.w;
	}

	vector4<T> & operator [] (unsigned int row)
	{
		return *(((vector4<T>*)&m) + row);
	}

	const vector4<T> & operator [] (unsigned int row) const
	{
		return *(((vector4<T>*)&m) + row);
	}

private:

	T m [16]; // row-major

};

#include "mat4x4.inl"

typedef		matrix4x4<float>	 mat4x4;
typedef		matrix4x4<int>		imat4x4;
typedef		matrix4x4<double>	dmat4x4;
