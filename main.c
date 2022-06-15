#include "parseDir.h"
#include "appUtils.h"

#define INPUT_RESPONSE_FILE "./response.dat"

int main()
{
    t_tagDir *fci;
    {
    FILE *input;
    input = fopen(INPUT_RESPONSE_FILE, "r");
    fci = getFCIfromBinary(input);
    fclose(input);
    }
    if (fci)
    {
        if (!(fillDown(fci)))
            clearTree(&fci);
        else
        {
            unsigned char suppApps[COUNT_SUPPORTED_APPS][LENGTH_APP_ID] = {{0xA0, 0x00,0x00, 0x06, 0x58}};
            //printTree(&fci, 0);
            t_appItem *appList = getSupportedApplications(fci, suppApps);
            printAppList(appList);
            clearAppList(&appList);
            clearTree(&fci);
        }
    }
}