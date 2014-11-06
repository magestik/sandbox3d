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
 * bvec3 implementation using bitset
 */

/**
 * check whether all elements of a boolean vector are true
 */
inline bool all(const bvec3 & x)
{
	return(x.bitset == 0x7);
}

/**
 * check whether any element of a boolean vector is true
 */
inline bool any(const bvec3 & x)
{
	return(x.bitset > 0);
}

/**
 * logically invert a boolean vector
 */
inline bvec3 operator!(const bvec3 & x)
{
	return(bvec3((~x.bitset) & 0x7));
}

