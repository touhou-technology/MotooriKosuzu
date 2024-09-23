#include <iostream>

void replace() {
	system("killall Project.out");
	//更新好的文件路径
	system("cp /home/awalwa/projects/Project/bin/ARM64/Debug/Project.out ./");
}

int main() {
	while (1) {
		replace();

		system("./Project.out");
	}
}