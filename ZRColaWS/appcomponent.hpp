/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#pragma once

#include "controller.hpp"
#include <oatpp/core/base/CommandLineArguments.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>

class AppComponent
{
protected:
    const oatpp::base::CommandLineArguments& m_cmdArgs;

public:
    AppComponent(const oatpp::base::CommandLineArguments& cmdArgs) : m_cmdArgs(cmdArgs) {}

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
        oatpp::String host = m_cmdArgs.getNamedArgumentValue("--host", "localhost");
        v_uint16 port = oatpp::utils::conversion::strToInt32(m_cmdArgs.getNamedArgumentValue("--port", "8000"));
        oatpp::network::Address::Family family = oatpp::network::Address::UNSPEC;
        if (m_cmdArgs.hasArgument("-4"))
            family = oatpp::network::Address::IP_4;
        else if (m_cmdArgs.hasArgument("-6"))
            family = oatpp::network::Address::IP_6;
        return oatpp::network::tcp::server::ConnectionProvider::createShared({host, port, family});
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

    OATPP_CREATE_COMPONENT(std::shared_ptr<Controller>, controller)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
        return std::make_shared<Controller>(objectMapper);
    }());

    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::Server>, server)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
        return oatpp::network::Server::createShared(connectionProvider, connectionHandler);
    }());
};
