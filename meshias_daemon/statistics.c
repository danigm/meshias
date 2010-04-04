#include "statistics.h"

#include <string.h>
#include <stdio.h>

void stats_init()
{
    memset(&stats, 0, sizeof(stats));
}

void stats_reset()
{
    memset(&stats, 0, sizeof(stats));
}
