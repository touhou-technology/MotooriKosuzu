#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    int pipe_to_child[2]; // �ӽ����ߵ������ߵĹܵ�
    int pipe_to_parent[2]; // �ӷ����ߵ������ߵĹܵ�

    // ���������ܵ�
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

        // ׼��ִ�� sender ���򣬲����� pid_t
        char pid_str[10];
        snprintf(pid_str, sizeof(pid_str), "%d", getppid());

        // ʹ�� exec ִ�� sender ����
        execlp("/home/awalwa/projects/Project/bin/ARM64/Debug/Project.out", "Project.out", pid_str, NULL);

        // ��� exec ʧ��
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else { // ������ (������)
        close(pipe_to_child[0]);
        close(pipe_to_parent[1]);

        // �ӷ����߽�����Ϣ
        char buffer[256];
        while (true) {
            ssize_t n = read(pipe_to_parent[0], buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0'; // ȷ���� NULL ��β
                std::cout << "Received message: " << buffer << std::endl;

                // ������������
                //if (strncmp(buffer, "Goodbye", 7) == 0) {
                //    break;
                //}
            }
        }

        // �ر�д��
        close(pipe_to_parent[0]); // �رն���
        close(pipe_to_child[1]); //�ر�д��

        wait(NULL); // �ȴ��ӽ��̽���
    }

    return 0;
}