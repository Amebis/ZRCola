/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#include "../include/version.h"
#include "appcomponent.h"
#include "controller.h"
#include <oatpp/network/Server.hpp>

using namespace std;

int main()
{
    oatpp::base::Environment::init();
    AppComponent components;
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
    auto myController = std::make_shared<MyController>();
    router->addController(myController);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);
    oatpp::network::Server server(connectionProvider, connectionHandler);
    OATPP_LOGI("ZRColaWS", "Server " PRODUCT_VERSION_STR " running on %s:%s",
        connectionProvider->getProperty("host").getData(), connectionProvider->getProperty("port").getData());
    server.run();
    oatpp::base::Environment::destroy();
    return 0;
}
