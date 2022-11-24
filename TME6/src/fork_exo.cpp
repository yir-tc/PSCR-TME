#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main () {
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;

	int childs = 0;
	bool is_parent = true;

	for (int i=1, j=N; i<=N && j==N && fork()==0 ; i++ ) {
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				j=0;
				childs = 0;
				std::cout << " k:j " << k << ":" << j << std::endl;
			} else {
				childs ++;
			}
		}
	}


	for (int i = 0; i < childs; i++) {
		wait(nullptr);
	}

	return 0;
}
