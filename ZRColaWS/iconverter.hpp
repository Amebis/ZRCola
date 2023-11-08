/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include <stdex/compat.hpp>
#include <iconv.h>
#include <cstring>
#include <stdexcept>
#include <string>

inline static std::runtime_error errno_error(_In_z_ const char *file, _In_ int line, _In_z_ const char *func)
{
    int _errno = errno;
    return std::runtime_error(
        std::string(file) + ":" + std::to_string(line) +
        std::string(func) + " error " + std::to_string(_errno) + ": " +
        std::strerror(_errno));
}

template <typename T_from, typename T_to>
class iconverter
{
public:
    iconverter(_In_z_ const char* from, _In_z_ const char* to)
    {
        m_handle = iconv_open(to, from);
        if (m_handle == (iconv_t)-1)
            throw errno_error(__FILE__, __LINE__, __FUNCTION__);
    }

    ~iconverter()
    {
        iconv_close(m_handle);
    }

    std::basic_string<T_to> convert(_In_z_count_(count) const T_from* input, _In_ size_t count) const
    {
        T_to buf[0x100];
        std::basic_string<T_to> result;
        size_t inSize = sizeof(T_from) * count;
        do {
            T_to* output = &buf[0];
            size_t outSize = sizeof(buf);
            errno = 0;
            iconv(m_handle, (char**)&input, &inSize, (char**)&output, &outSize);
            if (errno)
                throw errno_error(__FILE__, __LINE__, __FUNCTION__);
            result.insert(result.end(), buf, (T_to*)((char*)buf + sizeof(buf) - outSize));
        } while (inSize);
        return result;
    }

    std::basic_string<T_to> convert(_In_ const std::basic_string<T_from>& input)
    {
        return convert(input.c_str(), input.length());
    }

protected:
    iconv_t m_handle;
};

class utf16toutf8 : public iconverter<char16_t, char>
{
public:
    utf16toutf8() : iconverter("UTF-16LE", "UTF-8") {}
};

class utf8toutf16 : public iconverter<char, char16_t>
{
public:
    utf8toutf16() : iconverter("UTF-8", "UTF-16LE") {}
};
