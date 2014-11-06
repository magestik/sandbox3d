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
struct vector3
{
	union
	{
		struct
		{
			T x;

			union
			{
				struct { T y, z; };
				struct { vector2<T> yz; };
			};
		};

		struct { vector2<T> xy; /* T w; */};

		//vector3<T> xyz;

		float data [3];
	};

	explicit vector3 (void) : x(0), y(0), z(0) { }

	explicit vector3 (T x, T y, T z) : x(x), y(y), z(z) { }

	explicit vector3 (const vector2<T> & v, T z) : x(v.x), y(v.y), z(z) { }

	T & operator [] (unsigned int index)
	{
		return(data[index]);
	}

	const T & operator [] (unsigned int index) const
	{
		return(data[index]);
	}
};

#include "vec3.inl"

typedef		vector3<float>		 vec3;
typedef		vector3<int>		ivec3;
typedef		vector3<double>		dvec3;
