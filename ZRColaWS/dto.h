/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include <oatpp/core/data/mapping/type/Object.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class MessageDto : public oatpp::DTO
{
    DTO_INIT(MessageDto, DTO)
    DTO_FIELD(Int32, statusCode);
    DTO_FIELD(String, message);
};

class UserDto : public oatpp::DTO
{
    DTO_INIT(UserDto, DTO)
    DTO_FIELD(Int64, id);
    DTO_FIELD(String, name);
};

#include OATPP_CODEGEN_END(DTO)
