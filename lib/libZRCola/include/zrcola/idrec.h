/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "character.h"
#include "highlight.h"
#include "language.h"
#include "tag.h"
#include "translate.h"

#include <stdex/idrec.h>

namespace ZRCola {
    typedef stdex::idrec::record<character_db, recordid_t, 0x524843 /*"CHR"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> character_rec;
    typedef stdex::idrec::record<chrcat_db, recordid_t, 0x544343 /*"CCT"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> chrcat_rec;
    typedef stdex::idrec::record<highlight_db, recordid_t, 0x484748 /*"HGH"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> highlight_rec;
    typedef stdex::idrec::record<langchar_db, recordid_t, 0x432d4c /*"L-C"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> langchar_rec;
    typedef stdex::idrec::record<language_db, recordid_t, 0x474e4c /*"LNG"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> language_rec;
    typedef stdex::idrec::record<chrtag_db, recordid_t, 0x542d43 /*"C-T"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> chrtag_rec;
    typedef stdex::idrec::record<tagname_db, recordid_t, 0x4e4754 /*"TGN"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> tagname_rec;
    typedef stdex::idrec::record<translation_db, recordid_t, 0x4e5254 /*"TRN"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> translation_rec;
    typedef stdex::idrec::record<transet_db, recordid_t, 0x455354 /*"TSE"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> transet_rec;
    typedef stdex::idrec::record<transeq_db, recordid_t, 0x515354 /*"TSQ"*/, recordsize_t, ZRCOLA_RECORD_ALIGN> transeq_rec;
}
