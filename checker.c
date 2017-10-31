#include <stdio.h>
#include <unistd.h>

int
main(int argc, char * argv[])
{
    char rc = 0, ch;
    int i = 0;
    FILE *fp = fopen(argv[1], "r");
    while ((rc = getc(fp)) != EOF) {
        ch = getchar();
        if (rc != ch) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
