#include <iostream>

void replace() {
	system("killall Project.out");

	//���ºõ��ļ�·��
	system("rm -f /root/Project.out");
	system("cp /root/MotooriKosuzu/src/Project.out /root/");
	//system("cp /home/awalwa/projects/Project/bin/ARM64/Debug/Project.out ./");
}

int main() {
	while (1) {
		replace();

		system("/root/Project.out");
	}
}