#ifndef APPUTILS_H
#define APPUTILS_H

#include "parseDir.h"

#define COUNT_SUPPORTED_APPS 1
#define LENGTH_APP_ID 5

typedef struct s_appData
{
    unsigned char   priority;
    unsigned char   RID[LENGTH_APP_ID];
    unsigned char   *PIX;
}   t_appData;

typedef struct s_appItem
{
    t_appData *data;
    struct s_appItem *next;
}   t_appItem;

t_appItem*  getSupportedApplications(t_tagDir *resp, unsigned char suppApps[COUNT_SUPPORTED_APPS][LENGTH_APP_ID]);
void        printAppList(t_appItem *head);
void        clearAppList(t_appItem **head);
unsigned long int* initSuppApps();

#endif //APPUTILS_H