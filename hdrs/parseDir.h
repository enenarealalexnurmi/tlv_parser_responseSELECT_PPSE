#ifndef PARSEDIR_H
#define PARSEDIR_H

#include <stdlib.h>
#include <stdio.h>
#include "tlv.h"

#define FCI_TEMPLATE_TAG 0x6F
#define FCI_PROPRIETARY_TAG 0xA5 
#define FCI_IDD_TAG 0xBF0C
#define FCI_DIRECTORY_ENTRY_TAG 0x61
#define ADF_NAME_TAG 0x4F
#define APP_PRIORITY_TAG 0x87

typedef struct s_tagDir
{
    t_tlv *data;
    struct s_tagDir *down;
    struct s_tagDir *next;
}   t_tagDir;

t_tagDir*   getFCIfromBinary(FILE *input);
int         fillDown(t_tagDir *root);
void        clearTree(t_tagDir **dir);
void        printTree(t_tagDir **dir, int depth);

#endif //PARSEDIR_H