//          Copyright Jean Pierre Cimalando 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gettext-po.h>
#include <stdexcept>
#include <stdio.h>

static void print_text_pos(FILE *stderr, size_t lineno, size_t column)
{
    if (lineno != (size_t)-1) {
        fprintf(stderr, " (line %zu", lineno);
        if (column != (size_t)-1)
            fprintf(stderr, ", column %zu", column);
        fputc(')', stderr);
    }
}

static size_t po_xerror_count;

static void po_handle_xerror(
    int severity,
    po_message_t message, const char *filename, size_t lineno, size_t column, int multiline_p, const char *message_text)
{
    if (po_xerror_count == 0) {
        fputs(message_text, stderr);
        print_text_pos(stderr, lineno, column);
        fputc('\n', stderr);
    }

    if (severity == PO_SEVERITY_FATAL_ERROR)
        throw std::runtime_error("gettext fatal error");

    if (severity == PO_SEVERITY_ERROR)
        ++po_xerror_count;
}

static void po_handle_xerror2(
    int severity,
    po_message_t message1, const char *filename1, size_t lineno1, size_t column1, int multiline_p1, const char *message_text1,
    po_message_t message2, const char *filename2, size_t lineno2, size_t column2, int multiline_p2, const char *message_text2)
{
    if (po_xerror_count == 0) {
        fprintf(stderr, "%s...", message_text1);
        print_text_pos(stderr, lineno1, column1);
        fputc('\n', stderr);

        fprintf(stderr, "...%s", message_text2);
        print_text_pos(stderr, lineno2, column2);
        fputc('\n', stderr);
    }

    if (severity == PO_SEVERITY_FATAL_ERROR)
        throw std::runtime_error("gettext fatal error");

    if (severity == PO_SEVERITY_ERROR)
        ++po_xerror_count;
}
