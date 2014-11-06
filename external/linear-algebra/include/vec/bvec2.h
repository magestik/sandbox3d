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

struct bvec2
{
	friend bool all(const bvec2 & x);
	friend bool any(const bvec2 & x);
	friend bvec2 operator!(const bvec2 & x);

public:

	inline explicit bvec2 (void) : bitset(0) { }

	inline explicit bvec2 (int v) : bitset(v & 0x3) { }

	inline explicit bvec2 (bool b1, bool b2)
	{
		bitset = ((b1 ? 0x1 : 0) | (b2 ? 0x2 : 0));
	}

	bool operator[] (int index)
	{
		return bitset & (1 << index);
	}

private:

	char bitset;

};

#include "bvec2.inl"
