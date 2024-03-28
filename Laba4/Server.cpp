#include <sys/types.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <netinet/in.h>

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


struct Employee {
    char name[35];
    char number[10];
    char income[10];
    char tax[4];
} em[5];

// процедура для обслуживания соединения
int Func(int newS) {
    long int i, num, t, mon, doh, nal;
    float sum;
    int m;
    char p, p1, s;
    char buf[256], b[256];
    while (true) {
        recv(newS, buf, sizeof(buf), 0);
        p = buf[0];
        switch (p) {
        case '1':
            buf[0] = '\0';
            sum = 0;
            recv(newS, buf, sizeof(buf), 0);
            mon = atoi(buf);
            recv(newS, buf, sizeof(buf), 0);

            s = buf[0];
            for (i = 1; i <= 5; i++)
                if (em[i].name[0] == s) {
                    nal = atoi(em[i].tax);
                    doh = atoi(em[i].income);
                    printf("mon %ld\n", mon);
                    sum = sum + (nal * doh * mon) / 100.0;
                }
            int* decpt, * sgn;
            printf("%f\n", sum);
            strcpy(buf, fcvt(sum, 3, decpt, sgn));
            send(newS, buf, sizeof(buf), 0);
            puts(buf);
            break;
        case '2':
            recv(newS, buf, sizeof(buf), 0);  //Номер
            num = atoi(buf);
            printf("%ld\n", num);

            recv(newS, buf, sizeof(buf), 0);
            p1 = buf[0];
            switch (p1) {
            case '1':
                recv(newS, buf, sizeof(buf), 0); //Имя
                strcpy(em[num].name, buf);
                break;
            case '2':
                recv(newS, buf, sizeof(buf), 0); //Таб.номер
                strcpy(em[num].number, buf);
                break;
            case '3':
                recv(newS, buf, sizeof(buf), 0); //Доход
                strcpy(em[num].income, buf);
                break;
            case '4':
                recv(newS, buf, sizeof(buf), 0); //Ставка налога
                strcpy(em[num].tax, buf);
            }
            break;
        case '3':
            for (i = 1; i <= 5; i++) {
                buf[0] = '\0';
                strcat(buf, em[i].name);
                strcat(buf, " ");
                strcat(buf, em[i].number);
                strcat(buf, " ");
                strcat(buf, em[i].income);
                strcat(buf, " ");
                strcat(buf, em[i].tax);
                strcat(buf, "\n");
                send(newS, buf, sizeof(buf), 0);
            }
            break;
        case '4':
            exit(0);
        }
    }
}

void reaper(int sig) {
    int status;
    while (wait3(&status, WNOHANG, (struct rusage*)0) >= 0);
}

int main() {
    strcpy(em[1].name, "Sergeev Sergei Sergeevich");
    strcpy(em[1].number, "1");
    strcpy(em[1].income, "100000");
    strcpy(em[1].tax, "10");

    strcpy(em[2].name, "Ivanov Ivan Ivanovich");
    strcpy(em[2].number, "2");
    strcpy(em[2].income, "200000");
    strcpy(em[2].tax, "20");

    strcpy(em[3].name, "Vladimirov Vladimir Vladmirvich");
    strcpy(em[3].number, "3");
    strcpy(em[3].income, "300000");
    strcpy(em[3].tax, "30");

    strcpy(em[4].name, "Vasilev Vasilii Vasilievich");
    strcpy(em[4].number, "5");
    strcpy(em[4].income, "500000");
    strcpy(em[4].tax, "50");

    struct sockaddr_in local;
    int s, newS, rc;

    local.sin_family = AF_INET;
    local.sin_port = htons(7500);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    s = socket(AF_INET, SOCK_STREAM, 0);
    rc = bind(s, (struct sockaddr*)&local, sizeof(local));
    rc = listen(s, 5);
    (void)signal(SIGCHLD, reaper);
    while (true) {
        newS = accept(s, NULL, NULL);
        switch (fork()) {
        case 0:
            (void)close(s);
            exit(Func(newS));
        default:
            (void)close(newS);
        }
    }

    return 0;
}
