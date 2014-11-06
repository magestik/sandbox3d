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

struct bvec3
{
	friend bool all(const bvec3 & x);
	friend bool any(const bvec3 & x);
	friend bvec3 operator!(const bvec3 & x);

public:

	bvec3 (void) : bitset(0) { }

	bvec3 (int v) : bitset(v & 0x7) { }

	bvec3 (bool b1, bool b2, bool b3)
	{
		bitset = ((b1 ? 0x1 : 0) | (b2 ? 0x2 : 0) | (b3 ? 0x4 : 0));
	}

	bool operator[] (int index)
	{
		return bitset & (1 << index);
	}

private:

	char bitset;

};

#include "bvec3.inl"
