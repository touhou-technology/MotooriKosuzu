#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>

int pipe_to_child[2]; // 从接收者到发送者的管道
int pipe_to_parent[2]; // 从发送者到接收者的管道

void fn() {
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

int main(int argc, char** argv) {

	// 创建两个管道
	if (pipe(pipe_to_child) == -1 || pipe(pipe_to_parent) == -1) {
		perror("pipe");
		return EXIT_FAILURE;
	}

	bool run = 1;

	//监测更新
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

			// 从发送者接收消息
			char buffer[256];

			//while
			ssize_t n = read(pipe_to_parent[0], buffer, sizeof(buffer) - 1);

			buffer[n] = '\0'; // 确保以 NULL 结尾
			std::cout << buffer << std::endl;

			std::string msg = buffer;

			std::cout << msg << std::endl;

			if (msg == "update")
				run = 0;

			// 关闭写端
			close(pipe_to_parent[0]); // 关闭读端
			close(pipe_to_child[1]); //关闭写端

			//wait(NULL);
			fn();
		}
	}

	std::cout << "\nupdate" << std::endl;



	return 0;
}