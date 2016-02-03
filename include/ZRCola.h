/*
    Copyright 2016 Amebis

    This file is part of ZRCola.

    ZRCola is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ZRCola is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ZRCola. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <Windows.h>

//////////////////////////////////////////////////////////////////////////
// _L(), __L()
//////////////////////////////////////////////////////////////////////////

#define __L(x)                      L ## x
#define _L(x)                       __L(x)


//////////////////////////////////////////////////////////////////////////
// aassert(), averify()
//////////////////////////////////////////////////////////////////////////

#ifdef  __cplusplus
extern "C" {
#endif
_CRTIMP void __cdecl _wassert(_In_z_ const wchar_t * _Message, _In_z_ const wchar_t *_File, _In_ unsigned _Line);
#ifdef  __cplusplus
}
#endif

#if defined(NDEBUG) // aassert() and averify() are defined according to NDEBUG for consistent experience with assert().
#define aassert(expr)               ((void)0)
#define averify(expr)               ((void)(expr))
#else
#define aassert(expr)               ((void)((expr) || (_wassert(_L(#expr), _L(__FILE__), __LINE__), 0)))
#define averify(expr)               aassert(expr)
#endif
