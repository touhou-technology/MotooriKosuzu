#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>

int pipe_to_child[2]; // �ӽ����ߵ������ߵĹܵ�
int pipe_to_parent[2]; // �ӷ����ߵ������ߵĹܵ�

void fn() {
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

int main(int argc, char** argv) {

	// ���������ܵ�
	if (pipe(pipe_to_child) == -1 || pipe(pipe_to_parent) == -1) {
		perror("pipe");
		return EXIT_FAILURE;
	}

	bool run = 1;

	//������
	while (run) {
		pid_t pid = fork();

		if (pid == -1) {
			perror("fork");
			return EXIT_FAILURE;
		}

		if (pid == 0) {
			fn();
		}
		else {
			close(pipe_to_child[0]);
			close(pipe_to_parent[1]);

			// �ӷ����߽�����Ϣ
			char buffer[256];

			//while
			ssize_t n = read(pipe_to_parent[0], buffer, sizeof(buffer) - 1);

			buffer[n] = '\0'; // ȷ���� NULL ��β
			std::cout << buffer << std::endl;

			std::string msg = buffer;

			std::cout << msg << std::endl;

			if (msg == "update")
				run = 0;

			// �ر�д��
			close(pipe_to_parent[0]); // �رն���
			close(pipe_to_child[1]); //�ر�д��

			//wait(NULL);
			fn();
		}
	}

	std::cout << "\nupdate" << std::endl;



	return 0;
}