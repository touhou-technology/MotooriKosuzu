#include <iostream>

void replace() {
	system("killall Project.out");
	//���ºõ��ļ�·��
	system("cp /home/awalwa/projects/Project/bin/ARM64/Debug/Project.out ./");
}

int main() {
	while (1) {
		replace();

		system("./Project.out");
	}
}