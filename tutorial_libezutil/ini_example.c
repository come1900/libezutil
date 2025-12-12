/* Example: parse a simple configuration file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

typedef struct
{
    int version;
    const char* name;
    const char* email;
} configuration;

static int handler(void* userData, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)userData;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("protocol", "version")) {
        pconfig->version = atoi(value);
    } else if (MATCH("user", "name")) {
        pconfig->name = strdup(value);
    } else if (MATCH("user", "email")) {
        pconfig->email = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

#define _def_exaIniFileName "exa.ini"
int main(int argc, char* argv[])
{
    // write ini
    FILE *fpIni = fopen(_def_exaIniFileName, "w");
    if (NULL == fpIni)
    {
        printf("failed fopen '%s'!\n", _def_exaIniFileName);
        return -1;
    }
    fprintf(fpIni, "%s\n", "[protocol]");
    fprintf(fpIni, "\t%s\n", "version=100");
    fprintf(fpIni, "%s\n", "[user]");
    fprintf(fpIni, "\t%s\n", "name=tom");
    fprintf(fpIni, "\t%s\n", "email=tom@163.com");
    fclose(fpIni);
    // end write ini

    configuration config;

    if (ini_parse(_def_exaIniFileName, handler, &config) < 0) {
        printf("Can't load '%s'\n", _def_exaIniFileName);
        return 1;
    }
    printf("Config loaded from '%s':\n\t version=%d, name=%s, email=%s\n" \
    , _def_exaIniFileName
        ,config.version, config.name, config.email);

    free((void*)config.name);
    free((void*)config.email);

    return 0;
}
