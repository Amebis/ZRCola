/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright © 2022 Amebis
*/

#include "appcomponent.hpp"
#include "controller.hpp"
#include "zrcolaws.hpp"
#include <oatpp/network/Server.hpp>
#include <fstream>

using namespace std;
using namespace ZRCola;

translation_db t_db;
transet_db ts_db;
// transeq_db tsq_db;
langchar_db lc_db;
language_db lang_db;
// character_db chr_db;
// chrcat_db cc_db;
// chrtag_db ct_db;
// tagname_db tn_db;
// highlight_db h_db;

static void load_database()
{
    fstream dat(PREFIX "/share/zrcola/ZRCola.zrcdb", ios_base::in | ios_base::binary);
    if (!dat.good())
        throw runtime_error(PREFIX "/share/zrcola/ZRCola.zrcdb not found or cannot be opened.");

    if (!stdex::idrec::find<recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN>(dat, ZRCOLA_DB_ID, sizeof(recordid_t)))
        throw runtime_error(PREFIX "/share/zrcola/ZRCola.zrcdb is not a valid ZRCola database.");

    recordsize_t size;
    dat.read((char*)&size, sizeof(recordsize_t));
    if (dat.good()) {
        bool has_translation_data = false;

        for (;;) {
            recordid_t id;
            if (!stdex::idrec::read_id(dat, id, size)) break;

            if (id == translation_rec::id()) {
                dat >> translation_rec(t_db);
                if (dat.good()) {
                    has_translation_data = true;
                } else {
                    OATPP_LOGE("ZRColaWS", "ZRColaWS", "Error reading translation data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
                    t_db.clear();
                }
            } else if (id == transet_rec::id()) {
                dat >> transet_rec(ts_db);
                if (!dat.good()) {
                    OATPP_LOGE("ZRColaWS", "Error reading translation set data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
                    ts_db.clear();
                }
            // } else if (id == transeq_rec::id()) {
            //     dat >> transeq_rec(tsq_db);
            //     if (!dat.good()) {
            //         OATPP_LOGE("ZRColaWS", "Error reading translation sequence data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
            //         tsq_db.clear();
            //     }
            } else if (id == langchar_rec::id()) {
                dat >> langchar_rec(lc_db);
                if (!dat.good()) {
                    OATPP_LOGE("ZRColaWS", "Error reading language character data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
                    lc_db.clear();
                }
            } else if (id == language_rec::id()) {
                dat >> language_rec(lang_db);
                if (!dat.good()) {
                    OATPP_LOGE("ZRColaWS", "Error reading language character data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
                    lang_db.clear();
                }
            // } else if (id == character_rec::id()) {
            //     dat >> character_rec(chr_db);
            //     if (!dat.good()) {
            //         OATPP_LOGE("ZRColaWS", "Error reading character data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
            //         chr_db.clear();
            //     }
            // } else if (id == chrcat_rec::id()) {
            //     dat >> chrcat_rec(cc_db);
            //     if (!dat.good()) {
            //         OATPP_LOGE("ZRColaWS", "Error reading character category data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
            //         cc_db.clear();
            //     }
            // } else if (id == chrtag_rec::id()) {
            //     dat >> chrtag_rec(ct_db);
            //     if (!dat.good()) {
            //         OATPP_LOGE("ZRColaWS", "Error reading character tag data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
            //         ct_db.clear();
            //     }
            // } else if (id == tagname_rec::id()) {
            //     dat >> tagname_rec(tn_db);
            //     if (!dat.good()) {
            //         OATPP_LOGE("ZRColaWS", "Error reading tag name data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
            //         tn_db.clear();
            //     }
            // } else if (id == highlight_rec::id()) {
            //     dat >> highlight_rec(h_db);
            //     if (!dat.good()) {
            //         OATPP_LOGE("ZRColaWS", "Error reading highlight data from " PREFIX "/share/zrcola/ZRCola.zrcdb.");
            //         h_db.clear();
            //     }
            } else
                stdex::idrec::ignore<recordsize_t, ZRCOLA_RECORD_ALIGN>(dat);
        }

        if (!has_translation_data)
            throw runtime_error(PREFIX "/share/zrcola/ZRCola.zrcdb has no translation data.");
    }
}

int main()
{
    oatpp::base::Environment::init();
    try {
        load_database();

        AppComponent components;
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);
        auto controller = std::make_shared<Controller>();
        router->addController(controller);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);
        oatpp::network::Server server(connectionProvider, connectionHandler);
        OATPP_LOGI("ZRColaWS", "Server " PRODUCT_VERSION_STR " running on %s:%s",
            connectionProvider->getProperty("host").getData(), connectionProvider->getProperty("port").getData());
        server.run();
        oatpp::base::Environment::destroy();
    } catch (exception &ex) {
        OATPP_LOGE("ZRColaWS", "%s: %s", typeid(ex).name(), ex.what());
        return 1;
    }
    return 0;
}
