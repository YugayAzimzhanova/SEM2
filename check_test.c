#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <dirent.h>

int
main(void)
{
    int i, pid1, pid2, test_number[10], j;
    char test_directory[15], task_test[15];
    int file_count[11], dir_count = 0;
    DIR * test_files;
    DIR * test_dir;
    struct dirent * entry;
    test_dir = opendir("test");
    while ((entry = readdir(test_dir)) != NULL) {
        dir_count++;
    }
    closedir(test_dir);
    dir_count = dir_count - 2;
    for (i = 1; i <= dir_count; i++) {
        sprintf(test_directory, "test/0%d", i);
        test_files = opendir(test_directory);
        file_count[i] = 0;
        while ((entry = readdir(test_files)) != NULL) {
            if (entry -> d_type == DT_REG) {
                file_count[i]++;
           }
        }
    }
    closedir(test_files);
    for (i = 1; i <= dir_count; i++) {
        file_count[i] = file_count[i] / 2;
    }
    FILE *file = fopen("test/info.txt", "w");
    fprintf(file, "%d\n", dir_count);
    for (i = 1; i <= dir_count; i++) {
        sprintf(task_test, "0%d { ", i);
        fprintf(file, "%s", task_test);
        for (j = 1; j <= file_count[i]; j++) {
            fprintf(file, "%03d ", j);
        }
        fprintf(file, "}\n");
    }
    return 0;
}
