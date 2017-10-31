#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <dirent.h>
#include <string.h>

int
main(void)
{
    char test_directory[15], task_test[15];
    int file_count[15], i, j, user_amount = 0;
    DIR * user_files;
    DIR * user_folder;
    struct dirent ** namelist;
    struct dirent * entry;
    user_folder = opendir("data");
    while ((entry = readdir(user_folder)) != NULL) {
        user_amount++;
    }
    user_amount = user_amount - 2;
    FILE *file = fopen("data/info.txt", "w");
    fprintf(file, "%d\n", user_amount);
    for (i = 1; i <= user_amount; i++) {
        sprintf(test_directory, "data/user%d", i);
        file_count[i] = 0;
        file_count[i] = scandir(test_directory, &namelist, NULL, alphasort);
        sprintf(task_test, "user%d { ", i);
        fprintf(file, "%s", task_test);
        for (j = 2; j < file_count[i]; j++) {
            fprintf(file, "%s ", namelist[j] -> d_name);
        }
        fprintf(file, "}\n");
    }
    fclose(file);
    return 0;
}
