/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "../include/version.h"
#include "controller.hpp"
#include <oatpp-swagger/Model.hpp>
#include <oatpp-swagger/Resources.hpp>
#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

class AppComponent
{
protected:
    oatpp::network::Address m_address;

public:
    AppComponent(const oatpp::network::Address& address) : m_address(address) {}

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared({m_address.host, m_address.port, m_address.family});
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        return oatpp::web::server::HttpConnectionHandler::createShared(router);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        return oatpp::parser::json::mapping::ObjectMapper::createShared();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::Server>, server)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
        return oatpp::network::Server::createShared(connectionProvider, connectionHandler);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([this] {
        oatpp::swagger::DocumentInfo::Builder builder;
        builder
            .setTitle("ZRCola Web Service")
            .setDescription(
                "ZRCola is an input system designed mainly, although not exclusively, for linguistic use. "
                "It allows the user to combine basic letters with any diacritic marks and insert the resulting complex characters into the texts with ease.")
            .setVersion(PRODUCT_VERSION_STR)
            .setContactName("ZRCola")
            .setContactUrl("https://zrcola.zrc-sazu.si/en/")

            .setLicenseName("GNU General Public License, Version 3")
            .setLicenseUrl("https://www.gnu.org/licenses/gpl-3.0.en.html");
        return builder.build();
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
        return oatpp::swagger::Resources::loadResources(PREFIX "/include/oatpp-1.3.0/bin/oatpp-swagger/res");
    }());
};
