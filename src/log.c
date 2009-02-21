#include <stdio.h>
#include "log.h"

void debug_real(int x, char* str)
{
    printf("[%d] %s\n",x,str);
}
