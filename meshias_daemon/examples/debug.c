
#define DEBUG 5

#include "../log.h"

int main (int argc, char **argv)
{
    debug(1, "testing level 1");
    debug(2, "testing level 2 %s %d", "example", 4);
    debug(3, "testing level 3");
    return 0;
}