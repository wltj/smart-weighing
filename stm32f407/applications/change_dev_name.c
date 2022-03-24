#include "change_dev_name.h"
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>

extern void saveDevName( char *s);
unsigned char readDevName(void);
static void Cdevname(int argc, char**argv)
{
    if (argc < 2)
    {
        rt_kprintf("Please input'Cdevname <devname>'\n");
        return;
    }
    if(strlen(argv[1]) > 100)
    {
        rt_kprintf("dev name lenth must lass than 100 byte\n");
        return ;
    }
    saveDevName(argv[1]);
    rt_kprintf("dev name : %s\n",readDevName());
}


MSH_CMD_EXPORT(Cdevname, Cdevname example <Cdevname devname>);
