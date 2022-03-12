#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../abs2home/abs2home.c"
#include "utils/cutest/CuTest.h"

#define _PREFIX_LEN 6

char *get_prefix()
{
    char username[64];
    getlogin_r(username, 63);
    char *prefix = (char *) malloc(_PREFIX_LEN + strlen(username) + 1);
    strcpy(prefix, "/home/");
    strcat(prefix, username);
    prefix[_PREFIX_LEN + strlen(username)] = '\0';
    return prefix;
}

void test_dont_change_path(CuTest *tc)
{
    CuAssertStrEquals(tc, "/", abs2home("/"));
    CuAssertStrEquals(tc, "/usr", abs2home("/usr"));
    CuAssertStrEquals(tc, "/usr/bin", abs2home("/usr/bin"));
    CuAssertStrEquals(tc, "/very/long/path/nested/paths/here/omg",
                      abs2home("/very/long/path/nested/paths/here/omg"));
    CuAssertStrEquals(tc, "/home", abs2home("/home"));

    char username[64];
    getlogin_r(username, 63);
    CuAssertStrEquals(tc, username, abs2home(username));

    char dont_change[_PREFIX_LEN + strlen(username) + 3];
    strcpy(dont_change, "/home/");
    strcat(dont_change, username);
    strcat(dont_change, "X/\0");
    CuAssertStrEquals(tc, dont_change, abs2home(dont_change));
}

void test_change_path(CuTest *tc)
{
    char *prefix = get_prefix();
    char *result;

    char *buffer = (char *) malloc(strlen(prefix) + strlen("/ahoj") + 1);
    strcpy(buffer, prefix);
    strcat(buffer, "/ahoj");
    result = abs2home(buffer);
    CuAssertStrEquals(tc, "~/ahoj", result);
    free(buffer);
    free(result);

    buffer = (char *) malloc(strlen(prefix) + strlen("/ahoj/ja") + 1);
    strcpy(buffer, prefix);
    strcat(buffer, "/ahoj/ja");
    result = abs2home(buffer);
    CuAssertStrEquals(tc, "~/ahoj/ja", result);
    free(buffer);
    free(result);

    buffer = (char *) malloc(strlen(prefix) + strlen("/ahoj/ja/jsem/zanoreny") + 1);
    strcpy(buffer, prefix);
    strcat(buffer, "/ahoj/ja/jsem/zanoreny");
    result = abs2home(buffer);
    CuAssertStrEquals(tc, "~/ahoj/ja/jsem/zanoreny", result);
    free(buffer);
    free(result);

    free(prefix);
}

void test_change_path_edgecase(CuTest *tc)
{
    char *prefix = get_prefix();
    char *result;

    result = abs2home(prefix);
    CuAssertStrEquals(tc, "~", result);
    free(result);

    char *buffer = (char *) malloc(strlen(prefix) + 2);
    strcpy(buffer, prefix);
    strcat(buffer, "/");
    result = abs2home(buffer);
    CuAssertStrEquals(tc, "~", result);
    free(result);
    free(buffer);

    free(prefix);
}

CuSuite *test_abs2home_get_suite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_dont_change_path);
    SUITE_ADD_TEST(suite, test_change_path);
    SUITE_ADD_TEST(suite, test_change_path_edgecase);
    return suite;
}
