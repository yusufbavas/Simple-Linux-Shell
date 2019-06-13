#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]){

	if(argc != 2){
		write(STDOUT_FILENO,"usage: wc <values>\n",20);
		return 1;	
	}
	
	int result = 0;
	int count;
	int fd;
	char str[BUFFER_SIZE] = "";
	struct stat statbuf;
	stat(argv[1],&statbuf);

	// Eger input bir dosya degil ise argumanin line sayisi hesaplanir.
	if(S_ISDIR(statbuf.st_mode) || (fd =open(argv[1],O_RDONLY)) == -1){

		char * cat = (char *) malloc(strlen(argv[1]));
		strcpy(cat,argv[1]);
		if (strlen(cat) != 0)
			result++;
		for (; (cat = strchr(cat,'\n')) != NULL;cat++,result++);
		free(cat);
	}
	else{
		// Eger input bir file ise file okunur ve iceriginin line sayisi hesaplanir
		do{
			count = read(fd,str,BUFFER_SIZE);
			str[count] = '\0';
			if (count == -1)
				exit(0);
			
			for (char *temp = str; (temp = strchr(temp,'\n')) != NULL;temp++,result++);

		}while(count == BUFFER_SIZE);
	}
	sprintf(str,"%d\n",result);
	write(STDOUT_FILENO,str,strlen(str));
	return 0;
}