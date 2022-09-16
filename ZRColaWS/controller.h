/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "dto.h"
#include "iconverter.h"
#include "zrcolaws.h"
#include <zrcola/translate.h>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

class MyController : public oatpp::web::server::api::ApiController
{
public:
    MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) : oatpp::web::server::api::ApiController(objectMapper) {}

    ADD_CORS(getAbout)
    ENDPOINT("GET", "/about", getAbout)
    {
        return createDtoResponse(Status::CODE_200, aboutDto::createShared());
    }

    ADD_CORS(getTranset)
    ENDPOINT("GET", "/transet", getTranset)
    {
        try {
            utf16toutf8 c;
            auto result = oatpp::Vector<oatpp::Object<transetDto>>::createShared();
            auto dto = transetDto::createShared();
            dto->set = ZRCOLA_TRANSETID_DEFAULT;
            dto->src = "ZRCola Decomposed";
            dto->dst = "ZRCola Composed";
            result->push_back(dto);
            for (size_t i = 0, n = ts_db.idxTranSet.size(); i < n; i++) {
                const auto &ts = ts_db.idxTranSet[i];
                dto = transetDto::createShared();
                dto->set = ts.set;
                dto->src = c.convert(ts.src(), ts.src_len());
                dto->dst = c.convert(ts.dst(), ts.dst_len());
                result->push_back(dto);
            }
            dto = transetDto::createShared();
            dto->set = ZRCOLA_TRANSETID_UNICODE;
            dto->src = "ZRCola Decomposed";
            dto->dst = "Unicode";
            result->push_back(dto);
            return createDtoResponse(Status::CODE_200, result);
        } catch (std::exception &ex) {
            OATPP_LOGE("ZRColaWS", "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }

    ADD_CORS(postTranslate)
    ENDPOINT("POST", "/translate", postTranslate, BODY_DTO(Object<translateInDto>, input))
    {
        try {
            utf8toutf16 cIn;
            std::u16string dst, dst2;
            if (input->text)
                dst = cIn.convert(*input->text);
            std::vector<ZRCola::mapping_vector> mapping;
            if (input->transet) {
                ZRCola::mapping_vector map;
                auto ts = input->transet->cbegin();
                const auto ts_end = input->transet->cend();
                for (; ts != ts_end; ++ts) {
                    switch (*ts) {
                    case ZRCOLA_TRANSETID_DEFAULT:
                    case ZRCOLA_TRANSETID_UNICODE:
                        // Decompose first, then re-compose.
                        t_db.TranslateInv(*ts, dst.data(), dst.size(), dst2, &map);
                        mapping.push_back(std::move(map));
                        t_db.Translate(*ts, dst2.data(), dst2.size(), dst, &map);
                        mapping.push_back(std::move(map));
                        break;

                    default:
                        t_db.Translate(*ts, dst.data(), dst.size(), dst2, &map);
                        mapping.push_back(std::move(map));
                        dst = std::move(dst2);
                    }
                }
            }

            // TODO: Flatten mapping and return along with the translated text.

            utf16toutf8 cOut;
            auto dto = translateOutDto::createShared();
            dto->text = cOut.convert(dst);
            return createDtoResponse(Status::CODE_200, dto);
        } catch (std::exception &ex) {
            OATPP_LOGE("ZRColaWS", "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }

    // TODO: Add POST handler for inverse translation.
};

#include OATPP_CODEGEN_END(ApiController)
