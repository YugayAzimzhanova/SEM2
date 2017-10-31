#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char * argv[])
{
    int pid, pid2, pid3, fd[2], fd2[2], fd3[2], status, i, right_ans = 0;
    char input[15], output[15], prog_file[15], prog_comp[15], comp_prog[15];
    int test_amount = atoi(argv[3]);
    int task_number = atoi(argv[2]);
    sprintf(prog_file, "data/%s/%02d.c", argv[1], task_number);
    sprintf(prog_comp, "var/%s/%02d", argv[1], task_number);
    pipe(fd2);
    pid = fork();
    if (pid == 0)
    {
        execlp("gcc", "gcc", prog_file, "-o", prog_comp, NULL);     // компилируем программу пользователя
        return -1;
    }
    wait(&status);                                                  // ждем статус
    if (status != 0)
    {
        printf("%d\n", -1);                                         // если не скомпилировалась выводим -1 , завершаемся
        return -1;
    }
    for (i = 1; i <= test_amount; i++)                              // если скомпилировалась, проверяет по всем тестам
    {
        sprintf(input, "test/0%s/%03d.dat", argv[2], i);
        sprintf(output, "test/0%s/%03d.ans", argv[2], i);
        pipe(fd);
        pid2 = fork();
        sprintf(comp_prog, "./%s", prog_comp);
        if (pid2 == 0)
        {
            int fd4 = open(input, O_RDONLY);                        // соединяем  файл с входными данными с вводом программы
            dup2(fd4,0);
            dup2(fd[1],1);
            close(fd[1]);
            close(fd[0]);
            execlp(comp_prog, comp_prog, NULL);              // запускаем программу на входных данных с перенаправлением вывода в checker
        }
        wait(NULL);
        pid3 = fork();
        if (pid3 == 0)
        {
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            execlp("./checker", "./checker", output, NULL);        // отправляем в checker вывод программы
        }
        wait(&status);
        if (status == 0)                                           // если вывод правильный, увеличиваем количество правильных ответов
    	    right_ans++;
        close(fd[0]);
        close(fd[1]);
    }
    printf("%d\n", right_ans);                                     // выводим количество правильных ответов.
    return 0;
}


