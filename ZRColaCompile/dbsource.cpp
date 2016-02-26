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

#include "stdafx.h"


ZRCola::DBSource::DBSource()
{
}


ZRCola::DBSource::~DBSource()
{
}


bool ZRCola::DBSource::Open(const wxString& filename)
{
    wxASSERT_MSG(!m_db, wxT("database already open"));

    HRESULT hr;

    // Create COM object.
    hr = ::CoCreateInstance(CLSID_CADOConnection, NULL, CLSCTX_ALL, IID_IADOConnection, (LPVOID*)&m_db);
    if (SUCCEEDED(hr)) {
        // Open the database.
        std::wstring cn;
//#ifdef __WIN64__
//        cn  = L"jdbc:odbc:Driver={Microsoft Access Driver (*.mdb, *.accdb)};";
//#else
        cn  = L"Driver={Microsoft Access Driver (*.mdb)};";
//#endif
        cn += L"Dbq=";
        cn += filename.c_str();
        cn += L";Uid=;Pwd=;";
        hr = m_db->Open(ATL::CComBSTR(cn.c_str()));
        if (SUCCEEDED(hr)) {
            return true;

            m_db->Close();
        } else
            LogErrors();
        wxLogMessage(_("Could not open database %s (%x)."), filename.c_str(), hr);
        m_db.Release();
    } else
        wxLogMessage(_("Creating ADOConnection object failed (%x)."), hr);

    return false;
}


void ZRCola::DBSource::LogErrors() const
{
    wxASSERT_MSG(m_db, wxT("database does not exist"));

    // Get array of errors.
    ADOErrors *errors = NULL;
    if (SUCCEEDED(m_db->get_Errors(&errors))) {
        // Get number of errors.
        long n = 0;
        errors->get_Count(&n);

        // Iterate the errors.
        for (long i = 0; i < n; i++) {
            ADOError *err = NULL;
            if (SUCCEEDED(errors->get_Item(ATL::CComVariant(i), &err))) {
                // Write error number and description to the log.
                long num = 0;
                err->get_Number(&num);

                ATL::CComBSTR desc;
                err->get_Description(&desc);

                wxLogMessage(_("ADO Error 0x%x: %ls"), num, (BSTR)desc);

                err->Release();
            }
        }

        errors->Release();
    }
}
