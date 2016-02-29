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

#include <atlbase.h>
#include <adoint.h>

#include <algorithm>
#include <ostream>


namespace ZRCola {
    ///
    /// Source database
    ///
    class DBSource
    {
    public:
        ///
        /// Composition
        ///
        class composition {
        public:
            std::wstring src;   ///< decomposed string
            wchar_t dst;        ///< composed character
        };

    public:
        DBSource();
        virtual ~DBSource();

        ///
        /// Opens the database
        ///
        /// \param[in] filename  File name of the MDB database.
        ///
        /// \returns
        /// - true when open succeeds
        /// - false otherwise
        ///
        bool Open(LPCTSTR filename);

        ///
        /// Logs errors in database connections
        ///
        void LogErrors() const;

        ///
        /// Is recordset at end
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when at end
        /// - false otherwise
        ///
        static inline bool IsEOF(const ATL::CComPtr<ADORecordset>& rs)
        {
            VARIANT_BOOL eof = VARIANT_TRUE;
            return FAILED(rs->get_EOF(&eof)) || eof ? true : false;
        }

        ///
        /// Gets number of records in a recordset
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns Number of records
        ///
        static inline size_t GetRecordsetCount(const ATL::CComPtr<ADORecordset>& rs)
        {
            ADO_LONGPTR count;
            return SUCCEEDED(rs->get_RecordCount(&count)) ? count : (size_t)-1;
        }

        ///
        /// Returns ordered decomposed to composed character translations
        ///
        /// \param[out] rs  Recordset with results
        ///
        /// \returns
        /// - true when query succeeds
        /// - false otherwise
        ///
        bool SelectCompositions(ATL::CComPtr<ADORecordset>& rs) const;

        ///
        /// Returns composition data
        ///
        /// \param[in]  rs    Recordset with results
        /// \param[out] comp  Composition
        ///
        /// \returns
        /// - true when succeeded
        /// - false otherwise
        ///
        bool GetComposition(const ATL::CComPtr<ADORecordset>& rs, composition& comp) const;

    protected:
        std::basic_string<TCHAR> filename;  ///< the database filename
        ATL::CComPtr<ADOConnection> m_db;   ///< the database
    };
};


///
/// Stores composition data
///
/// \param[in] stream  Output stream
/// \param[in] comp    Composition
///
/// \returns The stream \p stream
///
inline std::ostream& operator <<(std::ostream& stream, const ZRCola::DBSource::composition& comp)
{
    // Store src.
    size_t len = comp.src.length();
    if (len > 0xffff) {
        // Decomposed string is too long.
        stream.setstate(std::ios::failbit);
        return stream;
    }
    unsigned short count = (unsigned short)len;
    if (stream.fail()) return stream;
    stream.write((const char*)&count, sizeof(count));
    if (stream.fail()) return stream;
    stream.write((const char*)comp.src.c_str(), sizeof(wchar_t) * count);

    // Store dst.
    if (stream.fail()) return stream;
    stream.write((const char*)&comp.dst, sizeof(comp.dst));

    return stream;
}
