#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE 128

using namespace std;
//先读后写


int main(){
	int fdr = -1;
	int fdw = -1;
	int ret = -1;

	char buf[SIZE];

	//只读打开管道一
	fdr = open("fifo1",O_RDONLY);
	if( -1 == fdr ){
		perror("open");
		return 1;
	}
	cout << "以只读的方式打开管道1" <<endl;

	//only write 
	fdw = open("fifo2",O_WRONLY);
	if( -1 == fdw ){
		perror("open");
		return 1;
	}
	cout << "以只写的方式打开管道2" <<endl;

	pid_t pid;
	pid = fork();
	if(pid < 0){
		perror("fork");
		return 0;
	}

	// 子进程
	if(0 == pid){
		while(1){
			// read 
			// cout <<"sonA is ok" <<endl;
			memset(buf, 0 , SIZE);
			ret = read(fdr, buf, SIZE);
			if(ret <= 0){
				perror("read");
				break;
			}
			cout << "read   " << buf << endl;
		}
		exit(0);
	}

	// 父进程
	while(1){
		// cout <<"fatherA is ok" <<endl;
		//write 
		memset(buf, 0 , SIZE);
		fgets(buf, SIZE, stdin);
		//delete /n
		if('\n' == buf[strlen(buf) - 1]){
			buf[strlen(buf) - 1] = '\0';
		}
		ret = write(fdw, buf, strlen(buf));
		if(ret <= 0){
			perror("write");
			break;
		}
		cout << "write ret "<<ret << endl;
	}

	close(fdr);
	close(fdw);

	return 0;
}
