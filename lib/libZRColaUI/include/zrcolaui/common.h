/*
    Copyright 2015-2016 Amebis

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


///
/// Public function calling convention
///
#ifdef LIBZRCOLAUI
#define ZRCOLAUI_API      __declspec(dllexport)
#else
#define ZRCOLAUI_API      __declspec(dllimport)
#endif
#define ZRCOLA_NOVTABLE __declspec(novtable)
#pragma warning(push)
#pragma warning(disable: 4251)


namespace ZRCola {
};

#pragma warning(pop)
