#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

int
main(void)
{
    int i, pid, fd[2], status, c, k, j, tasks_amount, file_count[100];
    char user_number[15], test_amount[15], num_of_task[15], new_var_dir[15], test_directory[15], testing[15];
    char rc;
    int user_amount, task_number = 0, flag = 0;
    struct dirent * entry;
    DIR * test_files;
    FILE *fp = fopen("data/info.txt","r");
    fscanf(fp, "%d", &user_amount);                        // Считывание из файла data/info.txt количества пользователей
    FILE *file1 = fopen("test/info.txt","r");
    fscanf(file1, "%d", &tasks_amount);                    // Считывание из файла test/info.txt количества задач
    FILE * file = fopen("result.txt", "w");
    for (i = 1; i <= tasks_amount; i++){                   //   Подсчет количества
        sprintf(test_directory, "test/0%d", i);            //   файлов (входных и выходных),
        test_files = opendir(test_directory);              //   находящихся в папке
        file_count[i] = 0;                                 //   тестов на
        while ((entry = readdir(test_files)) != NULL) {    //   каждую задачу
            if (entry -> d_type == DT_REG) {
                file_count[i]++;
            }
        }
    }
    for (i = 1; i <= tasks_amount; i++){                   // Определение реального
        file_count[i] = file_count[i] / 2;                 // количества тестов
    }                                                      // для каждой задачи


    for (k = 1; k <= user_amount; k++)                     // Цикл по кол-ву пользователей
    {
        sprintf(new_var_dir, "var/user%d", k);
        pid = fork();                                      // Создание в директории var папок пользователей,
        if (pid == 0)                                      // куда будут отправляться исполняемые файлы
            execlp("mkdir","mkdir", new_var_dir, NULL);
        wait(NULL);
        sprintf(user_number, "user%d", k);
        fprintf(file, "%s ", user_number);
        while (rc != '{') {
            rc = getc(fp);
        }
        for (i = 1; i <= tasks_amount; i++){                         // Цикл по кол-ву задач
            fscanf(fp, "%d", &task_number);                          // Считываем из файла номер задачи
            if (task_number == i - 1) {                              // Условие определяет, если был считан номер последней задачи или если задач нет
                if (flag != 0) {                                     // Флаг определяет, была ли у пользователя хоть одна задача: если да - flag != 0
                    for (j = task_number; j < tasks_amount; j++) {   // Цикл для простановки нулей, если есть хотя бы одна задача
                        fprintf(file, "%d ", 0);
                    }
                }
                rc = getc(fp);
                if ((rc == '}') && (flag == 0)) {                    // Если встретился "{" и нет ни одной задачи,
                    for (j = 1; j <= tasks_amount; j++) {            // запускается цикл простановки нулей на все задачи
                        fprintf(file, "%d ", 0);
                    }
                }
                break;
            }
            sprintf(testing, "%d", file_count[task_number]);
            if (task_number != i) {                                  // Условие, если какая-то задача не найдена
                for (j = i; j < task_number; j++) {                  // запускается цикл простановки нулей на пропущенных задачах
                    flag++;
                    fprintf(file, "%d ", 0);
                }
                i = task_number;
            }
            pipe(fd);
            pid = fork();
            sprintf(num_of_task, "%d", i);
            if (pid == 0)                                            // Запускается дочка, запускающая текущую задачу текущего юзера
            {
                dup2(fd[1], 1);
                close(fd[1]);
                close(fd[0]);
                execlp("./judge_unit","./judge_unit", user_number, num_of_task, testing, NULL);
            }
            dup2(fd[0], 0);
            wait(&status);                                           // Ожидание статуса, возвращаемого программой judge_unit
            scanf("%d", &c);                                         // Считывание статуса из стандартного вывода
            flag++;
            fprintf(file, "%d ", c);                                 // Простановка баллов (кол-во пройденных тестов) в файл результатов
            rc = getc(fp);
            rc = getc(fp);
        }
        flag = 0;
        task_number = 0;
        fprintf(file,"\n");
        rc = getc(fp);
    }
        return 0;
}
