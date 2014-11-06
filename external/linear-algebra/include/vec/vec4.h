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
struct vector4
{
	union
	{
		struct
		{
			T x;

			union
			{
				struct { T y, z, w; };
				struct { vector2<T> yz; /* vector2<T> zw; */ };
				struct { vector3<T> yzw; /* T w; */ };
			};
		};

		struct { vector2<T> xy; vector2<T> zw; };
		struct { vector3<T> xyz; /* T w; */ };

		//vector4<T> xyzw;

		float data [4];
	};

	explicit vector4 (void) : x(0), y(0), z(0), w(0) { }

	explicit vector4 (T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }

	explicit vector4 (const vector3<T> & v, T w) : x(v.x), y(v.y), z(v.z), w(w) { }

	explicit vector4 (const vector2<T> & v1, const vector2<T> & v2) : x(v1.x), y(v1.y), z(v2.x), w(v2.y) { }

	T & operator [] (unsigned int index)
	{
		return(data[index]);
	}

	const T & operator [] (unsigned int index) const
	{
		return(data[index]);
	}
};

#include "vec4.inl"

typedef		vector4<float>		 vec4;
typedef		vector4<int>		ivec4;
typedef		vector4<double>		dvec4;
