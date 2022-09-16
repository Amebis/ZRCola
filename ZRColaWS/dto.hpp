/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "../include/version.h"
#include <oatpp/core/data/mapping/type/Object.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class aboutDto : public oatpp::DTO
{
    DTO_INIT(aboutDto, DTO)
    DTO_FIELD(String, vendor) = PRODUCT_CFG_VENDOR;
    DTO_FIELD(String, application) = PRODUCT_CFG_APPLICATION;
    DTO_FIELD(String, version) = PRODUCT_VERSION_STR;
};

class transetDto : public oatpp::DTO
{
    DTO_INIT(transetDto, DTO)
    DTO_FIELD(UInt16, set);
    DTO_FIELD(String, src);
    DTO_FIELD(String, dst);
};

class translateInDto : public oatpp::DTO
{
    DTO_INIT(translateInDto, DTO)
    DTO_FIELD(Vector<UInt16>, transet);
    DTO_FIELD(String, text);
};

class translateOutDto : public oatpp::DTO
{
    DTO_INIT(translateOutDto, DTO)
    DTO_FIELD(String, text);
    DTO_FIELD(Vector<UInt32>, map);
};

#include OATPP_CODEGEN_END(DTO)
