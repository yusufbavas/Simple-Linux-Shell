#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>

#define BUFFER_SIZE 1000

void signalHandler(int signal);

int sizeWithoutSpaces(char str[]);

char * strWithoutSpaces(char str[]);

char * deleteFullSpaces(char str[]);

char** makeArgumentList(char *buffer,char *path);

int main(int argc, char const *argv[]){

	if(argc != 1){
		write(STDOUT_FILENO,"Usage: ./gtushell\n",18);
		return 1;	
	}

	int pid;
	char buffer[BUFFER_SIZE];
	char args1[BUFFER_SIZE];
	char com[BUFFER_SIZE];

	char* commands[BUFFER_SIZE];
	int commandSize=0;
	
	int pipefd[2];

	char path[BUFFER_SIZE];
	getcwd(path,BUFFER_SIZE);

	signal(SIGTERM, signalHandler);
	
	while(1){

		int flag = 0;
		int flag2 = 0;
		
		write(STDOUT_FILENO,"161044018@CSE222_HW3:",22);

		int count = read(STDIN_FILENO,buffer,BUFFER_SIZE);
		buffer[count -1 ] = '\0';

		if (sizeWithoutSpaces(buffer) == 0)
			continue;
		// ! operasyonu gerceklestirilir.
		if (strchr(buffer,'!') != NULL){
			int c;
			char junk;
			sscanf(buffer,"%c%s",&junk,args1);
			c = atoi(args1);
			if(c == 0){
				write(STDOUT_FILENO,"Usage: !X\n",10);
				continue;
			}
			else if (c <= commandSize)
				strcpy(buffer,commands[commandSize - c]);
			
		}
		// ! operasyonu icin tum komutlar tutulur.
		commands[commandSize] = (char *) malloc(strlen(buffer) +1);
		strcpy(commands[commandSize++],buffer);
			

		sscanf(buffer,"%s",args1);
		
		if (strcmp(args1,"exit") == 0)
			break;
		else if (strcmp(args1,"help") == 0){

			write(STDOUT_FILENO,"Supported Commands:\n",20);
			write(STDOUT_FILENO,"-lsf -> list files in directory\n",32);
			write(STDOUT_FILENO,"-pwd -> print current directory\n",32);
			write(STDOUT_FILENO,"-cd -> change directory\n",24);
			write(STDOUT_FILENO,"-cat -> print file content or process argument\n",47);
			write(STDOUT_FILENO,"-wc -> print number of lines in file or argument\n",49);
			write(STDOUT_FILENO,"-bunedu -> print sizes of directories and subdirectories\n",57);
			write(STDOUT_FILENO,"-exit -> Terminate shell\n",25);
			continue;	
		}
		else if (strcmp(args1,"cd") == 0)
		{

			if (chdir(&buffer[strlen(args1)+1]) == -1){
				sprintf(args1,"No such a directory : %s.\n",&buffer[strlen(args1)+1]);
				write(STDOUT_FILENO,args1,strlen(args1));
			}			
			continue;
		}
		// Eger | veya > varsa oraya kadar olan string buffer'a sonrasi args1'e atilir ve flaglar 1 yapilir.
		strcpy(com,args1);
		if(strchr(buffer,'|') != NULL){
			flag = 1;
			pipe(pipefd);
			strtok(buffer,"|");
			strcpy(args1,strtok(NULL,"|"));
		}
		else if(strchr(buffer,'>') != NULL){
			flag2 = 1;
			strtok(buffer,">");
			strcpy(args1,strtok(NULL,">"));
			strcpy(args1,&args1[1]);
			strWithoutSpaces(args1);
		}

		char **args = makeArgumentList(buffer,path);

		switch(pid = fork()){
			case -1:
				printf("Fork Failed!\n");
				exit(0);
			case 0:
				// Eger | varsa ilk tarafin sonucu ikinci kismin inputu olarak verilir. 
				// Fonksiyonun stdout u pipe'in yazma bacagi yapilir. Bu sayede orada yapilacak olan print pipe a yazmis olur. 
				if(flag){
					dup2(pipefd[1], STDOUT_FILENO);
					close(pipefd[0]);
					close(pipefd[1]);
				}
				// Eger > varsa komutun ciktisi dosyaya yazilir yani stdout dosya olur.
				else if(flag2){
					int fd = open(args1,O_WRONLY | O_CREAT | O_TRUNC,0777);
					dup2(fd,STDOUT_FILENO);
					close(fd);
				}
				// execv de bir hata durumunda free islemleri yapilip chield process bitirilir.
				if(execv(args[0],args) == -1){
					if(flag)
						printf("-1");
					printf("unnown command : %s\n",com);
					for(int i=0;i<commandSize;++i)
						free(commands[i]);
					exit(1);
				}
				
			default:
				waitpid(pid,NULL,0);
				// | olmasi durumunda ikinci bir process calistirilir. Arguman pipe dan alinir.
				if(flag){
					int count = read(pipefd[0],buffer,BUFFER_SIZE);
					buffer[count-1] = '\0';
					if (buffer[0] == '-'){
						printf("%s\n",&buffer[2]);
						continue;
					}
					
					strcpy(com,args1);
					strcat(args1," ");
					strcat(args1,buffer);

					args = makeArgumentList(args1,path);

					close(pipefd[0]);
					close(pipefd[1]);
					
					if((pid = fork()) == -1){
						printf("Fork Failed!\n");
						exit(0);	
					}
					else if(pid == 0){
						if(execv(args[0],args) == -1){
							printf("unnown command : %s\n",com);
							for(int i=0;i<commandSize;++i)
								free(commands[i]);
							exit(1);
						}
					}
					waitpid(pid,NULL,0);
				}
		}
		
	}
	for(int i=0;i<commandSize;++i)
		free(commands[i]);
	return 0;
}

// SIGTERM sinyali geldiginde shell kapatilir.
void signalHandler(int signal){

  if (signal == SIGTERM){
  	printf("\ngtushell Terminated\n");
  	exit(0);
  }
}

// stringin bosluk karakterleri haric size'ini return eder.
int sizeWithoutSpaces(char str[]){
	int res = 0;
	
	for(int i=0;i< strlen(str);++i)
		if (str[i] != ' ')
			res++;
		
	return res;
}

// stringin ilk basindaki bosluklari siler
char * strWithoutSpaces(char str[]){
	
	while(str[0] == ' '){
		for(int i=0;i<strlen(str);++i)
			str[i] = str[i+1];
	}
	return str;
}

// stringin icerisindeki tum bosluklari siler
char * deleteFullSpaces(char str[]){

	for(int i=0;i<strlen(str);++i){
		if (str[i] == ' ')
		{
			for(int j = i;j<strlen(str);++j){
				str[j] = str[j+1];
			}
		}
	}
	return str;
}

// Arguman listesi hazirlanir.
char** makeArgumentList(char *buffer,char *path){

	char args1[BUFFER_SIZE] = "";
	char args2[BUFFER_SIZE] = "";
	char args3[BUFFER_SIZE] = "";

	sscanf(buffer,"%s",args1);

	if(sizeWithoutSpaces(buffer) != sizeWithoutSpaces(args1)){

		sscanf(&buffer[strlen(args1)+1],"%s",args2);
		// Eger < var ise input dosyadan okunur.
		if (strcmp(args2,"<") == 0){
			
			sscanf(&buffer[strlen(args1) + strlen(args2) +1],"%s",args2);
			int fd = open(args2, O_RDONLY,0777);
			if (fd == -1){
				printf("No such a file : %s\n",args2);
				return NULL;
			}
			int c = read(fd,args2,BUFFER_SIZE);
				
			args2[c-1] = '\0';
			close(fd);
		}
		else if((strchr(args2,'-') == NULL))
			strcpy(args2,deleteFullSpaces(&buffer[strlen(args1)+1]));

		else if(sizeWithoutSpaces(&buffer[strlen(args1)] +1) != 0){
			sscanf(&buffer[strlen(args1) + strlen(args2) + 2],"%s",args3);

			if (strcmp(args3,"<") == 0){
			
				sscanf(&buffer[strlen(args1) + strlen(args2) + strlen(args3) +3],"%s",args3);
				int fd = open(args3, O_RDONLY,0777);
				if (fd == -1){
					printf("No such a file : %s\n",args3);
					return NULL;
				}
				int c = read(fd,args3,BUFFER_SIZE);
					
				args3[c-1] = '\0';
				close(fd);
			}		
		}
	}

	strcpy(buffer,path);
	strcat(buffer,"/");
	strcat(buffer,args1);

	return (char*[]){buffer,(strlen(args2) == 0) ? NULL : args2,(strlen(args3) == 0) ? NULL : args3,NULL};
}