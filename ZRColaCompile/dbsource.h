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

#include <wx/string.h>

#include <atlbase.h>

#include <adoint.h>


namespace ZRCola {
    ///
    /// Source database
    ///
    class DBSource {
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
        bool Open(const wxString& filename);

        ///
        /// Logs errors in database connections
        ///
        void LogErrors() const;

    protected:
        ATL::CComPtr<ADOConnection> m_db;    ///< the database
    };
};
