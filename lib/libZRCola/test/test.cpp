#include <zrcola/idrec.h>
#include <fstream>
#include <iostream>
#include <typeinfo>

using namespace std;
using namespace ZRCola;

translation_db t_db;
transet_db ts_db;
transeq_db tsq_db;
langchar_db lc_db;
language_db lang_db;
character_db chr_db;
chrcat_db cc_db;
chrtag_db ct_db;
tagname_db tn_db;
highlight_db h_db;

static void load_database()
{
    fstream dat("../../../output/data/ZRCola.zrcdb", ios_base::in | ios_base::binary);
    if (!dat.good())
        throw runtime_error("ZRCola.zrcdb not found or cannot be opened.");

    if (!stdex::idrec::find<recordid_t, recordsize_t, ZRCOLA_RECORD_ALIGN>(dat, ZRCOLA_DB_ID, sizeof(recordid_t)))
        throw runtime_error("ZRCola.zrcdb is not a valid ZRCola database.");

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
                    cerr << "Error reading translation data from ZRCola.zrcdb.\n";
                    t_db.clear();
                }
            } else if (id == transet_rec::id()) {
                dat >> transet_rec(ts_db);
                if (!dat.good()) {
                    cerr << "Error reading translation set data from ZRCola.zrcdb.\n";
                    ts_db.clear();
                }
            } else if (id == transeq_rec::id()) {
                dat >> transeq_rec(tsq_db);
                if (!dat.good()) {
                    cerr << "Error reading translation sequence data from ZRCola.zrcdb.\n";
                    tsq_db.clear();
                }
            } else if (id == langchar_rec::id()) {
                dat >> langchar_rec(lc_db);
                if (!dat.good()) {
                    cerr << "Error reading language character data from ZRCola.zrcdb.\n";
                    lc_db.clear();
                }
            } else if (id == language_rec::id()) {
                dat >> language_rec(lang_db);
                if (!dat.good()) {
                    cerr << "Error reading language character data from ZRCola.zrcdb.\n";
                    lang_db.clear();
                }
            } else if (id == character_rec::id()) {
                dat >> character_rec(chr_db);
                if (!dat.good()) {
                    cerr << "Error reading character data from ZRCola.zrcdb.\n";
                    chr_db.clear();
                }
            } else if (id == chrcat_rec::id()) {
                dat >> chrcat_rec(cc_db);
                if (!dat.good()) {
                    cerr << "Error reading character category data from ZRCola.zrcdb.\n";
                    cc_db.clear();
                }
            } else if (id == chrtag_rec::id()) {
                dat >> chrtag_rec(ct_db);
                if (!dat.good()) {
                    cerr << "Error reading character tag data from ZRCola.zrcdb.\n";
                    ct_db.clear();
                }
            } else if (id == tagname_rec::id()) {
                dat >> tagname_rec(tn_db);
                if (!dat.good()) {
                    cerr << "Error reading tag name data from ZRCola.zrcdb.\n";
                    tn_db.clear();
                }
            } else if (id == highlight_rec::id()) {
                dat >> highlight_rec(h_db);
                if (!dat.good()) {
                    cerr << "Error reading highlight data from ZRCola.zrcdb.\n";
                    h_db.clear();
                }
            } else
                stdex::idrec::ignore<recordsize_t, ZRCOLA_RECORD_ALIGN>(dat);
        }

        if (!has_translation_data)
            throw runtime_error("ZRCola.zrcdb has no translation data.");
    }
}

int main()
{
    try {
        load_database();

        u16string output;
        vector<mapping> map;
        t_db.Translate(ZRCOLA_TRANSETID_DEFAULT, u"", -1, output, &map);
        if (!output.empty()) throw runtime_error("Empty string translated to nonempty output.");
        if (!map.empty()) throw runtime_error("Empty string translation produced non-empty map.");

        t_db.Translate(ZRCOLA_TRANSETID_DEFAULT, u"To je test.", -1, output, &map);
        if (output != u"T  ťᵉⓢṭ.") throw runtime_error("Unexpected translation.");

        cout << "Passed\n";
        return 0;
    } catch (exception &ex) {
        cerr << typeid(ex).name() << ": " << ex.what() << endl;
        return 1;
    }
}
