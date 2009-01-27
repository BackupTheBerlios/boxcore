/*
 * hash.cpp
 *
 *  Created on: 26 Jan 2009
 *      Author: shaun
 */

#include "hash.h"

#define rot(x,k) (((x)<<(k)) | ((x)>>(32-(k))))

inline void final(UINT &p_a, UINT &p_b, UINT &p_c)
{
	p_c ^= p_b;
	p_c-= rot(p_b,14);
	p_a ^= p_c;
	p_a-= rot(p_c,11);
	p_b ^= p_a;
	p_b-= rot(p_a,25);
	p_c ^= p_b;
	p_c-= rot(p_b,16);
	p_a ^= p_c;
	p_a-= rot(p_c,4);
	p_b ^= p_a;
	p_b-= rot(p_a,14);
	p_c ^= p_b;
	p_c-= rot(p_b,24);
}

void IconHash(HWND p_hWnd, UINT p_uID, UINT &p_hash)
{
	UINT a, b;
	p_hash += 0xdeadbeef + 4;
	a = b = p_hash;

	b += p_uID;
	a += reinterpret_cast<UINT_PTR>(p_hWnd);

	final(a,b,p_hash);
}
