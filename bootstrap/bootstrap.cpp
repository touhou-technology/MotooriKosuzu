#include <iostream>

void replace() {
	system("killall Project.out");

	//更新好的文件路径
	system("rm -f ./Project.out");
	system("cp ./MotooriKosuzu/src/Project.out ./");
}

int main() {
	while (1) {
		replace();

		system("./Project.out");
	}
}