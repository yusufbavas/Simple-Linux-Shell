#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 100

// getcwd ile calisilan directory stringi basilir.
int main(int argc, char *argv[]){

	
	if(argc != 1){
		write(STDOUT_FILENO,"Usage: pwd\n",11);
		return 1;	
	}

	char path[BUFFER_SIZE];

	getcwd(path,BUFFER_SIZE);

	strcat(path,"\n");
	write(STDOUT_FILENO,path,strlen(path));

	return 0;
}