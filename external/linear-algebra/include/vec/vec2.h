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
struct vector2
{
	union
	{
		struct { T x, y; };

		//vector2<T> xy;

		float data [2];
	};

	explicit vector2 (void) : x(0), y(0) { }

	explicit vector2 (T x, T y) : x(x), y(y) { }

	T & operator [] (unsigned int index)
	{
		return(data[index]);
	}

	const T & operator [] (unsigned int index) const
	{
		return(data[index]);
	}
};

#include "vec2.inl"

typedef		vector2<float>		 vec2;
typedef		vector2<int>		ivec2;
typedef		vector2<double>		dvec2;
