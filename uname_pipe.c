#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
//无名管道
#define N 10
#define MAX 100

int child_read_pipe(int fd){
    char buf[N];
    int n =0;
    while(1){
        n = read(fd,buf,sizeof(buf));
        buf[n]='\0';
        printf("Read %d bytes: %s.\n",n,buf);
        if(strncmp(buf,"quit",4)==0)
            break;
    }
    return 0;
}

int father_write_pipe(int fd){
    char buf[MAX]={0};
    while(1){
        printf(">");
        fgets(buf,sizeof(buf),stdin);
        buf[strlen(buf)-1]='\0';
        write(fd,buf,strlen(buf));
        usleep(500);
        if(strncmp(buf,"quit",4)==0)
            break;
    }
    return 0;
}

int main(){
    int pid;
    int fd[2];
    if(pipe(fd)<0){//父进程创建管道
        perror("Fail to pipe");
        exit(EXIT_FAILURE);
    }
    if((pid=fork())<0){
        perror("Fail to fork");
        exit(EXIT_FAILURE);
    }else if(pid == 0){
        close(fd[1]);//表示管道的方向,fd[1]用于写
        child_read_pipe(fd[0]);//子进程读取管道
    }else{
        close(fd[0]);//fd[0]用于读
        father_write_pipe(fd[1]);//父进程写入管道
    }
}