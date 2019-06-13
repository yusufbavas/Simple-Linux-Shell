#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]){

	
	if(argc != 2){
		write(STDOUT_FILENO,"Usage: cat <values>\n",21);
		return 1;	
	}

	int count;
	int fd;
	struct stat statbuf;
	stat(argv[1],&statbuf);

	// Eger input bir file ise icerigi, degil ise kendisi print edilir.
	if(S_ISDIR(statbuf.st_mode) || (fd =open(argv[1],O_RDONLY)) == -1){
		strcat(argv[1],"\n");
		write(STDOUT_FILENO,argv[1],strlen(argv[1]));
	}
	else{
		do{
			char str[BUFFER_SIZE] = "";
			count = read(fd,str,BUFFER_SIZE);
			str[count] = '\0';
			if (count == -1)
				exit(0);
			write(STDOUT_FILENO,str,strlen(str));

		}while(count == BUFFER_SIZE);
	}

	return 0;
}