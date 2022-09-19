/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "dto.hpp"
#include "iconverter.hpp"
#include "zrcolaws.hpp"
#include <zrcola/translate.h>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController)

class Controller : public oatpp::web::server::api::ApiController
{
public:
    Controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) : oatpp::web::server::api::ApiController(objectMapper) {}

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

    ADD_CORS(getLanguage)
    ENDPOINT("GET", "/language", getLanguage)
    {
        try {
            utf16toutf8 c;
            auto result = oatpp::Vector<oatpp::Object<languageDto>>::createShared();
            for (size_t i = 0, n = lang_db.idxLang.size(); i < n; i++) {
                const auto &lang = lang_db.idxLang[i];
                auto dto = languageDto::createShared();
                dto->lang = std::string(&lang.lang.data[0], strnlen(lang.lang.data, std::size(lang.lang.data)));
                dto->name = c.convert(lang.name(), lang.name_len());
                result->push_back(dto);
            }
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
            size_t src_len = dst.size();
            std::vector<ZRCola::mapping_vector> mapping;
            if (input->transet) {
                ZRCola::mapping_vector map;
                const auto ts_end = input->transet->cend();
                for (auto ts = input->transet->cbegin(); ts != ts_end; ++ts) {
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

            utf16toutf8 cOut;
            auto dto = translateOutDto::createShared();
            dto->text = cOut.convert(dst);
            auto map = oatpp::Vector<oatpp::UInt32>::createShared();
            auto m_end = mapping.cend();
            for (size_t i = 0; i < src_len; ++i) {
                auto j = i;
                for (auto m = mapping.cbegin(); m != m_end; ++m)
                    j = m->to_dst(j);
                map->push_back(j);
            }
            dto->map = map;
            return createDtoResponse(Status::CODE_200, dto);
        } catch (std::exception &ex) {
            OATPP_LOGE("ZRColaWS", "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }

    ADD_CORS(postTranslateInv)
    ENDPOINT("POST", "/translateInv", postTranslateInv, BODY_DTO(Object<translateInDto>, input))
    {
        try {
            utf8toutf16 cIn;
            std::u16string dst, dst2;
            ZRCola::langid_t lang;
            if (input->text)
                dst = cIn.convert(*input->text);
            lang = input->lang->c_str();
            size_t src_len = dst.size();
            std::vector<ZRCola::mapping_vector> mapping;
            if (input->transet) {
                ZRCola::mapping_vector map;
                const auto ts_begin = input->transet->cbegin();
                for (auto ts = input->transet->cend(); (ts--) != ts_begin; ) {
                    switch (*ts) {
                    case ZRCOLA_TRANSETID_DEFAULT:
                    case ZRCOLA_TRANSETID_UNICODE:
                        t_db.TranslateInv(*ts, dst.data(), dst.size(), &lc_db, lang, dst2, &map);
                        dst = std::move(dst2);
                        map.invert();
                        mapping.push_back(std::move(map));
                        break;

                    default:
                        t_db.TranslateInv(*ts, dst.data(), dst.size(), dst2, &map);
                        dst = std::move(dst2);
                        map.invert();
                        mapping.push_back(std::move(map));
                    }
                }
            }

            utf16toutf8 cOut;
            auto dto = translateOutDto::createShared();
            dto->text = cOut.convert(dst);
            auto map = oatpp::Vector<oatpp::UInt32>::createShared();
            auto m_end = mapping.crend();
            for (size_t i = 0; i < src_len; ++i) {
                auto j = i;
                for (auto m = mapping.crbegin(); m != m_end; ++m)
                    j = m->to_src(j);
                map->push_back(j);
            }
            dto->map = map;
            return createDtoResponse(Status::CODE_200, dto);
        } catch (std::exception &ex) {
            OATPP_LOGE("ZRColaWS", "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)
