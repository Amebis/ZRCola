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

#include <wx/wx.h>

#define __L(x)                      L ## x
#define _L(x)                       __L(x)

#if defined(NDEBUG) // Ker je assert() definiran glede na NDEBUG, zaradi konsistentne izkušnje enako definiramo tudi aassert() in averify().
#define aassert(expr)               ((void)0)
#define averify(expr)               ((void)(expr))
#else
#if _MSC_VER < 1300
#define aassert(expr)               ((void)((expr) || (_assert (   #expr,     __FILE__,  __LINE__), 0)))
#else
#define aassert(expr)               ((void)((expr) || (_wassert(_L(#expr), _L(__FILE__), __LINE__), 0)))
#endif
#define averify(expr)               aassert(expr)
#endif
