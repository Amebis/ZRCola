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
    if (m_db)
        m_db->Close();
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
        cn  = L"Driver={Microsoft Access Driver (*.mdb)};";
        cn += L"Dbq=";
        cn += filename.c_str();
        cn += L";Uid=;Pwd=;";
        hr = m_db->Open(ATL::CComBSTR(cn.c_str()));
        if (SUCCEEDED(hr)) {
            // Database open and ready.
            return true;
        } else {
            wxLogMessage(wxT("Could not open database %s (0x%x)."), filename.c_str(), hr);
            LogErrors();
        }
        m_db.Release();
    } else
        wxLogMessage(wxT("Creating ADOConnection object failed (0x%x)."), hr);

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
        wxVERIFY(SUCCEEDED(errors->get_Count(&n)));

        // Iterate the errors.
        for (long i = 0; i < n; i++) {
            ADOError *err = NULL;
            if (SUCCEEDED(errors->get_Item(ATL::CComVariant(i), &err))) {
                // Write error number and description to the log.
                long num = 0;
                wxVERIFY(SUCCEEDED(err->get_Number(&num)));

                ATL::CComBSTR desc;
                wxVERIFY(SUCCEEDED(err->get_Description(&desc)));

                wxLogMessage(wxT("ADO Error 0x%x: %ls"), num, (BSTR)desc);

                err->Release();
            }
        }

        errors->Release();
    }
}


bool ZRCola::DBSource::SelectCompositions(ATL::CComPtr<ADORecordset> &rs) const
{
    // Create a new recordset.
    if (rs) rs.Release();
    wxCHECK(SUCCEEDED(::CoCreateInstance(CLSID_CADORecordset, NULL, CLSCTX_ALL, IID_IADORecordset, (LPVOID*)&rs)), false);

    // Open it.
    if (FAILED(rs->Open(ATL::CComVariant(L"SELECT [komb], [znak] FROM [VRS_ReplChar] WHERE [rang_komb]=1 ORDER BY [komb] ASC"), ATL::CComVariant(m_db), adOpenForwardOnly, adLockReadOnly, adCmdText))) {
        LogErrors();
        return false;
    }

    return true;
}
