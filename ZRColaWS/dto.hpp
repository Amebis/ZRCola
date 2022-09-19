/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include <oatpp/core/data/mapping/type/Object.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

namespace dto {

class About : public oatpp::DTO
{
    DTO_INIT(About, DTO)

    DTO_FIELD_INFO(vendor) { info->description = "Application vendor"; }
    DTO_FIELD(String, vendor);

    DTO_FIELD_INFO(application) { info->description = "Application name"; }
    DTO_FIELD(String, application);

    DTO_FIELD_INFO(version) { info->description = "Application version"; }
    DTO_FIELD(String, version);
};

class TranSet : public oatpp::DTO
{
    DTO_INIT(TranSet, DTO)

    DTO_FIELD_INFO(set) { info->description = "Translation set ID"; }
    DTO_FIELD(UInt16, set);

    DTO_FIELD_INFO(src) { info->description = "Input transcript name in English"; }
    DTO_FIELD(String, src);

    DTO_FIELD_INFO(dst) { info->description = "Output transcript name in English"; }
    DTO_FIELD(String, dst);
};

class Language : public oatpp::DTO
{
    DTO_INIT(Language, DTO)

    DTO_FIELD_INFO(lang) { info->description = "Language ID"; }
    DTO_FIELD(String, lang);

    DTO_FIELD_INFO(name) { info->description = "Language name in English"; }
    DTO_FIELD(String, name);
};

class TranslateIn : public oatpp::DTO
{
    DTO_INIT(TranslateIn, DTO)

    DTO_FIELD_INFO(transet) {
        info->description = "Array of one or multiple translation set IDs to perform translation on the text. When inverse translating, the translation sets are read in reverse order listed in this array. Use /transet to get IDs of all supported translation sets.";
        info->required = true;
    }
    DTO_FIELD(Vector<UInt16>, transet);

    DTO_FIELD_INFO(text) {
        info->description = "Text to be translated";
        info->required = true;
    }
    DTO_FIELD(String, text);

    DTO_FIELD_INFO(lang) {
        info->description = "Language ID of the text to be translated. This is used on inverse translating to skip decomposing language-specific common characters (e.g. č, š, ž in Slovenian, or ä, ö, ü in German). Use /language to get IDs of all supported languages.";
    }
    DTO_FIELD(String, lang) = "slv";
};

class TranslateOut : public oatpp::DTO
{
    DTO_INIT(TranslateOut, DTO)

    DTO_FIELD_INFO(text) { info->description = "Translated text"; }
    DTO_FIELD(String, text);

    DTO_FIELD_INFO(map) { info->description = "Character index mapping between input and translated text. The map[i] value represents the index of the beginning of a character (in translated text) that translated from the character (in the input text) beginning at index i. All input indexes are measured in characters after input string is translated to UTF-16, and all output indexes are measured in characters before output string is translated from UTF-16."; }
    DTO_FIELD(Vector<UInt32>, map);
};

}

#include OATPP_CODEGEN_END(DTO)
