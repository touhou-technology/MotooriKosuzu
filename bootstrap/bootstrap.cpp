#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    int pipe_to_child[2]; // 从接收者到发送者的管道
    int pipe_to_parent[2]; // 从发送者到接收者的管道

    // 创建两个管道
    if (pipe(pipe_to_child) == -1 || pipe(pipe_to_parent) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) { 
        close(pipe_to_child[1]); 
        close(pipe_to_parent[0]); 

        // 准备执行 sender 程序，并传递 pid_t
        char pid_str[10];
        snprintf(pid_str, sizeof(pid_str), "%d", getppid());

        // 使用 exec 执行 sender 程序
        execlp("/home/awalwa/projects/Project/bin/ARM64/Debug/Project.out", "Project.out", pid_str, NULL);

        // 如果 exec 失败
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else { // 父进程 (接收者)
        close(pipe_to_child[0]);
        close(pipe_to_parent[1]);

        // 从发送者接收消息
        char buffer[256];
        while (true) {
            ssize_t n = read(pipe_to_parent[0], buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0'; // 确保以 NULL 结尾
                std::cout << "Received message: " << buffer << std::endl;

                // 结束接收条件
                //if (strncmp(buffer, "Goodbye", 7) == 0) {
                //    break;
                //}
            }
        }

        // 关闭写端
        close(pipe_to_parent[0]); // 关闭读端
        close(pipe_to_child[1]); //关闭写端

        wait(NULL); // 等待子进程结束
    }

    return 0;
}