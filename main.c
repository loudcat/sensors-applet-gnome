#include <stdio.h>
#include "func.h"


int main(void)
{
    printf("Hello\n");
    find_dir_recurce("/sys/class/hwmon");
    return 0;
}