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
 * Vector2 template implementation using structure
 */

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator-(const vector2<T> & v)
{
   return(vector2<T>(-v.x, -v.y));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator+(const vector2<T> & a, const vector2<T> & b)
{
	return(vector2<T>((a.x + b.x), (a.y + b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator-(const vector2<T> & a, const vector2<T> & b)
{
	return(vector2<T>((a.x - b.x), (a.y - b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator*(const vector2<T> & a, const vector2<T> & b)
{
	return(vector2<T>((a.x * b.x), (a.y * b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator/(const vector2<T> & a, const vector2<T> & b)
{
	return(vector2<T>((a.x / b.x), (a.y / b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator*(float s, const vector2<T> & v)
{
	return(vector2<T>((s * v.x), (s * v.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator*(const vector2<T> & v, float s)
{
	return(vector2<T>((v.x * s), (v.y * s)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator/(float s, const vector2<T> & v)
{
	return(vector2<T>((s / v.x), (s / v.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline vector2<T> operator/(const vector2<T> & v, float s)
{
	return(vector2<T>((v.x / s), (v.y / s)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec2 operator==(const vector2<T> & a, const vector2<T> & b)
{
	return(bvec2((a.x == b.x), (a.y == b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec2 operator>(const vector2<T> & a, const vector2<T> & b)
{
	return(bvec2((a.x > b.x), (a.y > b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec2 operator>=(const vector2<T> & a, const vector2<T> & b)
{
	return(bvec2((a.x >= b.x), (a.y >= b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec2 operator<(const vector2<T> & a, const vector2<T> & b)
{
	return(bvec2((a.x < b.x), (a.y < b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec2 operator<=(const vector2<T> & a, const vector2<T> & b)
{
	return(bvec2((a.x <= b.x), (a.y <= b.y)));
}

/**
 * TODO comment
 */
template<typename T>
static inline bvec2 operator!=(const vector2<T> & a, const vector2<T> & b)
{
	return(bvec2((a.x != b.x), (a.y != b.y)));
}

/**
 * calculate the length of a vector
 */
template<typename T>
static inline float length(const vector2<T> & v)
{
	return(sqrt((v.x * v.x) + (v.y * v.y)));
}

/**
 * calculate the distance between two points
 */
template<typename T>
static inline float distance(const vector2<T> & a, const vector2<T> & b)
{
	return(length(b - a));
}

/**
 * calculate the dot product of two vectors
 */
template<typename T>
static inline float dot(const vector2<T> & a, const vector2<T> & b)
{
	return((a.x * b.x) + (a.y * b.y));
}

/**
 * calculate the normalize product of two vectors
 */
template<typename T>
static inline vector2<T> normalize(const vector2<T> & v)
{
	T len = length(v);

	if (!(bool)len)
	{
		return vector2<T>(0, 0);
	}

	return(v / len);
}

/**
 * calculate the reflection direction for an incident vector
 */
template<typename T>
static inline vector2<T> reflect(const vector2<T> & I, const vector2<T> & N)
{
	return(I - 2 * dot(N, I) * N);
}

/**
 * calculate the refraction direction for an incident vector
 */
template<typename T>
static inline vector2<T> refract(const vector2<T> & I, const vector2<T> & N, float eta)
{
	float k = 1 - eta * eta * (1 - dot(N, I) * dot(N, I));

	if (0 > k)
	{
		return vector2<T>(0, 0);
	}

	return(eta * I - (eta * dot(N, I) + sqrt(k)) * N);
}

/**
 * perform a component-wise equal-to comparison of two vectors
 */
template<typename T>
static bvec2 equal(const vector2<T> & a, const vector2<T> & b)
{
	return(a == b);
}

/**
 * perform a component-wise greater-than comparison of two vectors
 */
template<typename T>
static bvec2 greaterThan(const vector2<T> & a, const vector2<T> & b)
{
	return(a > b);
}

/**
 * perform a component-wise greater-than-or-equal comparison of two vectors
 */
template<typename T>
static bvec2 greaterThanEqual(const vector2<T> & a, const vector2<T> & b)
{
	return(a >= b);
}

/**
 * perform a component-wise less-than comparison of two vectors
 */
template<typename T>
static bvec2 lessThan(const vector2<T> & a, const vector2<T> & b)
{
	return(a < b);
}

/**
 * perform a component-wise less-than-or-equal comparison of two vectors
 */
template<typename T>
static bvec2 lessThanEqual(const vector2<T> & a, const vector2<T> & b)
{
	return(a <= b);
}

/**
 * perform a component-wise not-equal-to comparison of two vectors
 */
template<typename T>
static bvec2 notEqual(const vector2<T> & a, const vector2<T> & b)
{
	return(a != b);
}
