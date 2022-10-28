/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "../include/version.h"
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
    Controller(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& defaultObjectMapper, const oatpp::String &routerPrefix = nullptr) :
        oatpp::web::server::api::ApiController(defaultObjectMapper, routerPrefix)
    {}

    ADD_CORS(getAbout)
    ENDPOINT_INFO(getAbout) {
        info->summary = "Returns service information";
        info->addResponse<Object<dto::About>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/about", getAbout)
    {
        auto dto = dto::About::createShared();
        dto->vendor = PRODUCT_CFG_VENDOR;
        dto->application = PRODUCT_CFG_APPLICATION;
        dto->version = PRODUCT_VERSION_STR;
        return createDtoResponse(Status::CODE_200, dto);
    }

    ADD_CORS(getTranset)
    ENDPOINT_INFO(getTranset) {
        info->summary = "Lists supported translation sets";
        info->description = "Each translation set describes a set of replacements that are performed to translate text from one script or encoding to another. ";
        info->addResponse<oatpp::Vector<oatpp::Object<dto::TranSet>>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/transet", getTranset)
    {
        try {
            utf16toutf8 c;
            auto result = oatpp::Vector<oatpp::Object<dto::TranSet>>::createShared();
            auto dto = dto::TranSet::createShared();
            dto->set = ZRCOLA_TRANSETID_DEFAULT;
            dto->src = "ZRCola Decomposed";
            dto->dst = "ZRCola Composed";
            result->push_back(dto);
            for (size_t i = 0, n = ts_db.idxTranSet.size(); i < n; i++) {
                const auto &ts = ts_db.idxTranSet[i];
                dto = dto::TranSet::createShared();
                dto->set = ts.set;
                dto->src = c.convert(ts.src(), ts.src_len());
                dto->dst = c.convert(ts.dst(), ts.dst_len());
                result->push_back(dto);
            }
            dto = dto::TranSet::createShared();
            dto->set = ZRCOLA_TRANSETID_UNICODE;
            dto->src = "ZRCola Decomposed";
            dto->dst = "Unicode";
            result->push_back(dto);
            return createDtoResponse(Status::CODE_200, result);
        } catch (std::exception &ex) {
            OATPP_LOGE(__FUNCTION__, "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }

    ADD_CORS(getLanguage)
    ENDPOINT_INFO(getLanguage) {
        info->summary = "Lists supported languages";
        info->description = "Each language describes a set of special characters that are specific to that language (e.g. č, š, ž in Slovenian, or ä, ö, ü in German).";
        info->addResponse<oatpp::Vector<oatpp::Object<dto::Language>>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("GET", "/language", getLanguage)
    {
        try {
            utf16toutf8 c;
            auto result = oatpp::Vector<oatpp::Object<dto::Language>>::createShared();
            for (size_t i = 0, n = lang_db.idxLang.size(); i < n; i++) {
                const auto &lang = lang_db.idxLang[i];
                auto dto = dto::Language::createShared();
                dto->lang = std::string(&lang.lang.data[0], strnlen(lang.lang.data, std::size(lang.lang.data)));
                dto->name = c.convert(lang.name(), lang.name_len());
                result->push_back(dto);
            }
            return createDtoResponse(Status::CODE_200, result);
        } catch (std::exception &ex) {
            OATPP_LOGE(__FUNCTION__, "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }

    ADD_CORS(postTranslate)
    ENDPOINT_INFO(postTranslate) {
        info->summary = "Translate text";
        info->description =
            "Performs any number of supported translations (see /transet) on a given input text in a sequence. "
            "Together with the output text, it also returns character index mapping between input and output texts.";
        auto transet = oatpp::Vector<UInt16>::createShared();
        transet->push_back(ZRCOLA_TRANSETID_DEFAULT);
        auto dto = dto::TranslateIn::createShared();
        dto->transet = transet;
        dto->text = "To je test.";
        info->addConsumes<Object<dto::TranslateIn>>("application/json")
            .addExample("Perform ZRCola composition", dto);
        info->addResponse<Object<dto::TranslateOut>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/translate", postTranslate, BODY_DTO(Object<dto::TranslateIn>, input))
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
                        map.clear();
                        t_db.Translate(*ts, dst2.data(), dst2.size(), dst, &map);
                        mapping.push_back(std::move(map));
                        map.clear();
                        break;

                    default:
                        t_db.Translate(*ts, dst.data(), dst.size(), dst2, &map);
                        mapping.push_back(std::move(map));
                        map.clear();
                        dst = std::move(dst2);
                        dst2.clear();
                    }
                }
            }

            utf16toutf8 cOut;
            auto dto = dto::TranslateOut::createShared();
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
            OATPP_LOGE(__FUNCTION__, "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }

    ADD_CORS(postTranslateInv)
    ENDPOINT_INFO(postTranslateInv) {
        info->summary = "Inverse translate text";
        info->description =
            "Performs any number of supported translations (see /transet) on a given input text in a sequence in reverse. "
            "Together with the output text, it also returns character index mapping between input and output texts.";
        auto transet = oatpp::Vector<UInt16>::createShared();
        transet->push_back(ZRCOLA_TRANSETID_DEFAULT);
        auto dto = dto::TranslateIn::createShared();
        dto->transet = transet;
        dto->text = "T  ťᵉⓢṭ.";
        dto->lang = "slv";
        info->addConsumes<Object<dto::TranslateIn>>("application/json")
            .addExample("Perform ZRCola decomposition", dto);
        info->addResponse<Object<dto::TranslateOut>>(Status::CODE_200, "application/json");
    }
    ENDPOINT("POST", "/translateInv", postTranslateInv, BODY_DTO(Object<dto::TranslateIn>, input))
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
                        dst2.clear();
                        map.invert();
                        mapping.push_back(std::move(map));
                        map.clear();
                        break;

                    default:
                        t_db.TranslateInv(*ts, dst.data(), dst.size(), dst2, &map);
                        dst = std::move(dst2);
                        dst2.clear();
                        map.invert();
                        mapping.push_back(std::move(map));
                        map.clear();
                    }
                }
            }

            utf16toutf8 cOut;
            auto dto = dto::TranslateOut::createShared();
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
            OATPP_LOGE(__FUNCTION__, "%s: %s", typeid(ex).name(), ex.what());
            return ResponseFactory::createResponse(Status::CODE_500, ex.what());
        }
    }
};

#include OATPP_CODEGEN_END(ApiController)
