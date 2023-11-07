/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2015-2022 Amebis
*/

#pragma once

#include <zrcola/common.h>

#include <stdex/idrec.hpp>
#include <assert.h>
#include <algorithm>
#include <istream>
#include <locale>
#include <ostream>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4200)
#pragma warning(disable: 4251)
#pragma warning(disable: 4512)


namespace ZRCola {
    ///
    /// Character group ID
    ///
    typedef uint16_t chrgrpid_t;


    ///
    /// Character group database
    ///
    class chrgrp_db {
    public:
#pragma pack(push)
#pragma pack(2)
        ///
        /// Character group data
        ///
        struct chrgrp {
        public:
            chrgrpid_t grp;     ///< Character group ID
            uint16_t rank;      ///< Character group rank

        protected:
            uint16_t name_to;   ///< Character group name end in \c data
            uint16_t chrlst_to; ///< Character list end in \c data
            char_t data[];      ///< Character group name, character list, bit vector if particular character is displayed initially

        public:
            ///
            /// Constructs the character group
            ///
            /// \param[in] grp         Character group ID
            /// \param[in] rank        Character group rank
            /// \param[in] name        Character group name
            /// \param[in] name_len    Number of UTF-16 characters in \p name
            /// \param[in] chrlst      Character list (zero delimited)
            /// \param[in] chrlst_len  Number of UTF-16 characters in \p chrlst (including zero delimiters)
            /// \param[in] chrshow     Binary vector which particular character is displayed initially
            ///
            inline chrgrp(
                _In_opt_                                     chrgrpid_t  grp        = 0,
                _In_opt_                                     uint16_t    rank       = 0,
                _In_opt_z_count_(name_len)             const char_t     *name       = NULL,
                _In_opt_                                     size_t      name_len   = 0,
                _In_opt_z_count_(chrlst_len)           const char_t     *chrlst     = NULL,
                _In_opt_                                     size_t      chrlst_len = 0,
                _In_opt_count_x_((chrlst_len + 15)/16) const uint16_t   *chrshow    = NULL)
            {
                this->grp  = grp;
                this->rank = rank;
                this->name_to = static_cast<uint16_t>(name_len);
                if (name && name_len) memcpy(this->data, name, sizeof(char_t)*name_len);
                this->chrlst_to = static_cast<uint16_t>(this->name_to + chrlst_len);
                if (chrlst && chrshow && chrlst_len) {
                    memcpy(this->data + this->name_to, chrlst, sizeof(char_t)*chrlst_len);
                    memcpy(this->data + this->chrlst_to, chrshow, (chrlst_len + sizeof(*data)*8 - 1)/8);
                }
            }

            inline const char_t*  name    () const { return data;           };
            inline       char_t*  name    ()       { return data;           };
            inline const char_t*  name_end() const { return data + name_to; };
            inline       char_t*  name_end()       { return data + name_to; };
            inline       uint16_t name_len() const { return name_to;        };

            inline const char_t*  chrlst    () const { return data + name_to;      };
            inline       char_t*  chrlst    ()       { return data + name_to;      };
            inline const char_t*  chrlst_end() const { return data + chrlst_to;    };
            inline       char_t*  chrlst_end()       { return data + chrlst_to;    };
            inline       uint16_t chrlst_len() const { return chrlst_to - name_to; };

            inline const uint16_t* chrshow    () const { return reinterpret_cast<const uint16_t*>(data + chrlst_to                ); };
            inline       uint16_t* chrshow    ()       { return reinterpret_cast<      uint16_t*>(data + chrlst_to                ); };
            inline const uint16_t* chrshow_end() const { return reinterpret_cast<const uint16_t*>(data + chrlst_to + chrshow_len()); };
            inline       uint16_t* chrshow_end()       { return reinterpret_cast<      uint16_t*>(data + chrlst_to + chrshow_len()); };
            inline       uint16_t  chrshow_len() const { return (chrlst_len() + sizeof(*data)*8 - 1)/(sizeof(*data)*8);              };
        };
#pragma pack(pop)

        ///
        /// Rank index
        ///
        class indexRank : public index<uint16_t, uint32_t, chrgrp>
        {
        public:
            ///
            /// Constructs the index
            ///
            /// \param[in] h  Reference to vector holding the data
            ///
            indexRank(_In_ std::vector<uint16_t> &h) : index<uint16_t, uint32_t, chrgrp>(h) {}

            ///
            /// Compares two character groups by rank (for searching)
            ///
            /// \param[in] a  Pointer to character group
            /// \param[in] b  Pointer to second character group
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare(_In_ const chrgrp &a, _In_ const chrgrp &b) const
            {
                if (a.rank < b.rank) return -1;
                if (a.rank > b.rank) return +1;

                return 0;
            }

            ///
            /// Compares two character groups by rank (for sorting)
            ///
            /// \param[in] a  Pointer to character group
            /// \param[in] b  Pointer to second character group
            ///
            /// \returns
            /// - <0 when a <  b
            /// - =0 when a == b
            /// - >0 when a >  b
            ///
            virtual int compare_sort(_In_ const chrgrp &a, _In_ const chrgrp &b) const
            {
                     if (a.rank < b.rank) return -1;
                else if (a.rank > b.rank) return +1;

                auto &coll = std::use_facet<std::collate<char_t>>(std::locale());
                return coll.compare(a.name(), a.name_end(), b.name(), b.name_end());
            }
        } idxRank;  ///< Rank index

        std::vector<uint16_t> data; ///< Character groups data

    public:
        ///
        /// Constructs the database
        ///
        inline chrgrp_db() : idxRank(data) {}


        ///
        /// Writes character group database to a stream
        ///
        /// \param[in] stream  Output stream
        /// \param[in] db      Character group database
        ///
        /// \returns The stream \p stream
        ///
        friend std::ostream& operator <<(_In_ std::ostream& stream, _In_ const chrgrp_db& db)
        {
            // Write rank index.
            if (stream.fail()) return stream;
            stream << db.idxRank;

            // Write data count.
            auto data_count = db.data.size();
#if defined(_WIN64) || defined(__x86_64__) || defined(__ppc64__)
            // 4G check
            if (data_count > 0xffffffff) {
                stream.setstate(std::ios_base::failbit);
                return stream;
            }
#endif
            if (stream.fail()) return stream;
            uint32_t count = (uint32_t)data_count;
            stream.write((const char*)&count, sizeof(count));

            // Write data.
            if (stream.fail()) return stream;
            stream.write((const char*)db.data.data(), sizeof(uint16_t) * static_cast<std::streamsize>(count));

            return stream;
        }


        ///
        /// Reads character group database from a stream
        ///
        /// \param[in ] stream  Input stream
        /// \param[out] db      Character group database
        ///
        /// \returns The stream \p stream
        ///
        friend std::istream& operator >>(_In_ std::istream& stream, _Out_ chrgrp_db& db)
        {
            // Read rank index.
            stream >> db.idxRank;
            if (!stream.good()) return stream;

            // Read data count.
            uint32_t count;
            stream.read((char*)&count, sizeof(count));
            if (!stream.good()) return stream;

            if (count) {
                // Read data.
                db.data.resize(count);
                stream.read((char*)db.data.data(), sizeof(uint16_t) * static_cast<std::streamsize>(count));
            }
            else
                db.data.clear();

            return stream;
        }
    };


    typedef stdex::idrec::record<chrgrp_db, recordid_t, 0x524743 /*"CGR"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> chrgrp_rec;
};

#pragma warning(pop)
