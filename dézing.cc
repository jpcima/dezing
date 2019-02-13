//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "dézing.h"
#include <pcrecpp.h>
#include <getopt.h>

static void explain_arguments()
{
    fprintf(stderr, u8"dézing <fichier-fr.po>\n");
}

static std::string dezing(const std::string &in)
{
    using pcrecpp::RE;
    using pcrecpp::RE_Options;

    const char *exp_pluriel = u8"·[[:alpha:]]+·s\\b";
    const char *exp_singulier = u8"·[[:alpha:]]+\\b";

    static const RE re_pluriel(exp_pluriel, RE_Options(PCRE_UTF8|PCRE_UCP));
    static const RE re_singulier(exp_singulier, RE_Options(PCRE_UTF8|PCRE_UCP));

    std::string out = in;
    size_t count = 0;

    count += re_pluriel.GlobalReplace("s", &out);
    count += re_singulier.GlobalReplace("", &out);

    if (false && count > 0) {
        fprintf(stderr, u8"← %s\n", in.c_str());
        fprintf(stderr, u8"→ %s\n", out.c_str());
    }

    return out;
}

int main(int argc, char *argv[])
{
    for (int arg = 0; (arg = getopt(argc, argv, "")) != -1;) {
        switch (arg) {
        default:
            explain_arguments();
            return 1;
        }
    }

    if (optind != argc - 1) {
        explain_arguments();
        return 1;
    }

    const char *pofilename = argv[optind];

    po_xerror_handler errh;
    errh.xerror = &po_handle_xerror;
    errh.xerror2 = &po_handle_xerror2;

    po_file_t po;
    po_xerror_count = 0;
    po = po_file_read(pofilename, &errh);

    if (po_xerror_count > 0) {
        po_file_free(po);
        return 1;
    }

    for (const char *dom, *const *pdom = po_file_domains(po); (dom = *pdom); ++pdom) {
        po_message_iterator_t it = po_message_iterator(po, dom);

        for (po_message_t msg; (msg = po_next_message(it));) {
            const char *msgstr = po_message_msgstr(msg);
            if (msgstr[0] == '\0')
                continue;
            po_message_set_msgstr(msg, dezing(msgstr).c_str());
        }

        po_message_iterator_free(it);
    }

    po_xerror_count = 0;
    po_file_write(po, "/dev/stdout", &errh);

    if (po_xerror_count > 0) {
        po_file_free(po);
        return 1;
    }

    po_file_free(po);

    return 0;
}
