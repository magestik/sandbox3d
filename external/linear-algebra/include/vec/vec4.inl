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
 * Vector4 template implementation using structure
 */

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator-(const vector4<T> & v)
{
   return(vector4<T>(-v.x, -v.y, -v.z, -v.w));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator+(const vector4<T> & a, const vector4<T> & b)
{
	return(vector4<T>((a.x + b.x), (a.y + b.y), (a.z + b.z), (a.w + b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator-(const vector4<T> & a, const vector4<T> & b)
{
	return(vector4<T>((a.x - b.x), (a.y - b.y), (a.z - b.z), (a.w - b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator*(const vector4<T> & a, const vector4<T> & b)
{
	return(vector4<T>((a.x * b.x), (a.y * b.y), (a.z * b.z), (a.w * b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator/(const vector4<T> & a, const vector4<T> & b)
{
	return(vector4<T>((a.x / b.x), (a.y / b.y), (a.z / b.z), (a.w / b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator*(float s, const vector4<T> & v)
{
	return(vector4<T>((s * v.x), (s * v.y), (s * v.z), (s * v.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator*(const vector4<T> & v, float s)
{
	return(vector4<T>((s * v.x), (s * v.y), (s * v.z), (s * v.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator/(float s, const vector4<T> & v)
{
	return(vector4<T>((s / v.x), (s / v.y), (s / v.z), (s / v.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector4<T> operator/(const vector4<T> & v, float s)
{
	return(vector4<T>((v.x / s), (v.y / s), (v.z / s), (v.w / s)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec4 operator==(const vector4<T> & a, const vector4<T> & b)
{
	return(bvec4((a.x == b.x), (a.y == b.y), (a.z == b.z), (a.w == b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec4 operator>(const vector4<T> & a, const vector4<T> & b)
{
	return(bvec4((a.x > b.x), (a.y > b.y), (a.z > b.z), (a.w > b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec4 operator>=(const vector4<T> & a, const vector4<T> & b)
{
	return(bvec4((a.x >= b.x), (a.y >= b.y), (a.z >= b.z), (a.w >= b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec4 operator<(const vector4<T> & a, const vector4<T> & b)
{
	return(bvec4((a.x < b.x), (a.y < b.y), (a.z < b.z), (a.w < b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec4 operator<=(const vector4<T> & a, const vector4<T> & b)
{
	return(bvec4((a.x <= b.x), (a.y <= b.y), (a.z <= b.z), (a.w <= b.w)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec4 operator!=(const vector4<T> & a, const vector4<T> & b)
{
	return(bvec4((a.x != b.x), (a.y != b.y), (a.z != b.z), (a.w != b.w)));
}

/**
 * calculate the length of a vector
 */
template<typename T>
static inline T length(const vector4<T> & v)
{
	return(sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w)));
}

/**
 * calculate the distance between two points
 */
template<typename T>
static inline T distance(const vector4<T> & a, const vector4<T> & b)
{
	return(length(b - a));
}

/**
 * calculate the dot product of two vectors
 */
template<typename T>
static inline T dot(const vector4<T> & a, const vector4<T> & b)
{
	return((a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w));
}

/**
 * calculate the normalize product of two vectors
 */
template<typename T>
static inline vector4<T> normalize(const vector4<T> & v)
{
	T len = length(v);

	if (!(bool)len)
	{
		return vector4<T>(0, 0, 0, 0);
	}

	return(v / len);
}

/**
 * calculate the reflection direction for an incident vector
 */
template<typename T>
static inline vector4<T> reflect(const vector4<T> & I, const vector4<T> & N)
{
	return(I - 2 * dot(N, I) * N);
}

/**
 * calculate the refraction direction for an incident vector
 */
template<typename T>
static inline vector4<T> refract(const vector4<T> & I, const vector4<T> & N, float eta)
{
	float k = 1 - eta * eta * (1 - dot(N, I) * dot(N, I));

	if (0 > k)
	{
		return vector4<T>(0, 0, 0, 0);
	}

	return(eta * I - (eta * dot(N, I) + sqrt(k)) * N);
}

/**
 * perform a component-wise equal-to comparison of two vectors
 */
template<typename T>
static bvec4 equal(const vector4<T> & a, const vector4<T> & b)
{
	return(a == b);
}

/**
 * perform a component-wise greater-than comparison of two vectors
 */
template<typename T>
static bvec4 greaterThan(const vector4<T> & a, const vector4<T> & b)
{
	return(a > b);
}

/**
 * perform a component-wise greater-than-or-equal comparison of two vectors
 */
template<typename T>
static bvec4 greaterThanEqual(const vector4<T> & a, const vector4<T> & b)
{
	return(a >= b);
}

/**
 * perform a component-wise less-than comparison of two vectors
 */
template<typename T>
static bvec4 lessThan(const vector4<T> & a, const vector4<T> & b)
{
	return(a < b);
}

/**
 * perform a component-wise less-than-or-equal comparison of two vectors
 */
template<typename T>
static bvec4 lessThanEqual(const vector4<T> & a, const vector4<T> & b)
{
	return(a <= b);
}

/**
 * perform a component-wise not-equal-to comparison of two vectors
 */
template<typename T>
static bvec4 notEqual(const vector4<T> & a, const vector4<T> & b)
{
	return(a != b);
}

