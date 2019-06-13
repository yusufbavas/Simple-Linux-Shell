#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define BUFFER_SIZE 100

int main(int argc, char *argv[]){
	
	if(argc != 1){
		write(STDOUT_FILENO,"Usage: lsf\n",11);
		return 1;	
	}

	char path[BUFFER_SIZE];

	struct dirent *direntptr;
	struct stat statbuf;
	DIR *dirptr;

	getcwd(path,BUFFER_SIZE);

	dirptr = opendir(path);
	// Directory gezilir. Regular veya special file oldugu yazilir. Erisim izinleri, boyut ve isim yazilir.
	// Directoryler alinmaz.
	while (dirptr != NULL &&  errno != EACCES && (direntptr = readdir(dirptr)) != NULL){

		char *str = (char*) malloc(strlen(path) + strlen(direntptr->d_name) + 2);
		strcpy(str,path);
		if( str[strlen(str)-1] != '/')
			strcat(str,"/");
		strcat(str,direntptr->d_name);

		lstat(str,&statbuf);
		if(!S_ISDIR(statbuf.st_mode)){

			if (S_ISREG(statbuf.st_mode))
				write(STDOUT_FILENO,"R 	",strlen("R 	"));
			else
				write(STDOUT_FILENO,"S 	",strlen("S 	"));

		    write(STDOUT_FILENO,(statbuf.st_mode & S_IRUSR) ? "r" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IWUSR) ? "w" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IXUSR) ? "x" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IRGRP) ? "r" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IWGRP) ? "w" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IXGRP) ? "x" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IROTH) ? "r" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IWOTH) ? "w" : "-",1);
		    write(STDOUT_FILENO,(statbuf.st_mode & S_IXOTH) ? "x" : "-",1);
		    
		    sprintf(str," 	%ld 	",statbuf.st_size / 1024 );
		    write(STDOUT_FILENO,str,strlen(str));

		    sprintf(str,"%s\n",direntptr->d_name);
			write(STDOUT_FILENO,str,strlen(str));
		}
		
		free(str);
	}

	return 0;
}