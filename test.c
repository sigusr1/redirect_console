#include <stdio.h>
#include "redirect.h"

int main(int argc, char **argv)
{
    char str[32];
    
    init_redirect_module();
    while(1)
    {
        printf("Please input string, shorter than 30 characters:\n");
        scanf("%s", str);
        printf("Your input str is:\n%s\n", str);
    }

    return 0;
}
