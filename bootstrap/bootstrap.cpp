#include <iostream>

void replace() {
	system("killall Project.out");

	//���ºõ��ļ�·��
	system("rm -f ./Project.out");
	system("cp ./MotooriKosuzu/src/Project.out ./");
}

int main() {
	while (1) {
		replace();

		system("./Project.out");
	}
}