/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "dto.h"
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

class MyController : public oatpp::web::server::api::ApiController
{
public:
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) : oatpp::web::server::api::ApiController(objectMapper) {}

    ENDPOINT("GET", "/hello", hello)
    {
        auto dto = MessageDto::createShared();
        dto->statusCode = 0;
        dto->message = "Hello World!";
        return createDtoResponse(Status::CODE_200, dto);
    }

    ADD_CORS(postUsers)
    ENDPOINT("POST", "/users", postUsers, BODY_DTO(Object<UserDto>, user))
    {
        auto dto = MessageDto::createShared();
        dto->statusCode = 0;
        std::string msg;
        msg  = "Hello ";
        msg += user->name ? user->name->c_str() : "unknown";
        msg += "!";
        dto->message = msg;
        return createDtoResponse(Status::CODE_200, dto);
    }
};

#include OATPP_CODEGEN_END(ApiController)
