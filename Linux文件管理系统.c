#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//定义一个结构体，包括一个路径、名称，以及种类（文件h还是文件夹，若是文件夹，则还有文件内容），三个路径指针。 
typedef struct FileFolder{
	char pwd[50];
	struct FileFolder* Child;
	struct FileFolder* Father;
	struct FileFolder* Brother;
	char name[20];
	char type[10];
	char content[50];
}FileFolder;





void split(char *src,const char *separator,char **dest) {

     char *pNext;
     int count = 0;
     if (src == NULL || strlen(src) == 0) 
        return;
     if (separator == NULL || strlen(separator) == 0) 
        return;
     pNext = (char *)strtok(src,separator); 
     while(pNext != NULL) {
          *dest++ = pNext;
         pNext = (char *)strtok(NULL,separator);  
    }  

} 

//初始化根目录 
FileFolder* init(){
	
	FileFolder* rootFileFolder = (FileFolder *)malloc(1 * sizeof(FileFolder));
	strcpy(rootFileFolder->pwd,".../");
	rootFileFolder->Brother = NULL;
	rootFileFolder->Child = NULL;
	rootFileFolder->Father = rootFileFolder;
	strcpy(rootFileFolder->name,"root");
	strcpy(rootFileFolder->type,"FileFolder");
	return rootFileFolder;
}

//查找该目录下最后一个子文件/文件夹 
FileFolder* putpToEndBrother(FileFolder* p) {
	FileFolder* q = p;
	while(q->Brother != NULL) {
		q = q->Brother;
	}
	return q;
}

//创建文件夹 
void makdir(FileFolder* now,char** command) {
	FileFolder* p = now;
	FileFolder* x = (FileFolder *)malloc(1 * sizeof(FileFolder));
	strcpy(x->name,command[1]);
	strcpy(x->type,"FileFolder");
	strcpy(x->pwd,now->pwd);
	strcat(x->pwd,x->name);
	strcat(x->pwd,"/");
	x->Brother = NULL;
	x->Father = now;
	x->Child = NULL;
	if(p->Child == NULL){
		p->Child = x;
		
	}
	else {
		p = putpToEndBrother(p->Child);
		p->Brother = x;
	}
	
	
}

//创建文件 
void touch(FileFolder* now,char** command) {
	FileFolder* p = now;
	FileFolder* x = (FileFolder *)malloc(1 * sizeof(FileFolder));
	strcpy(x->name,command[1]);
	strcpy(x->type,"File");
	strcpy(x->content,"");
	x->Father = now;
	x->Brother = NULL;
	if(p->Child == NULL){
		p->Child = x;
		
	}
	else {
		FileFolder* q = putpToEndBrother(p);
		q->Brother = x;
	}
}

//按创建顺序展示该目录下的文件、文件夹 
void ls(FileFolder* now) {
	FileFolder* p = now;
	if(p->Child == NULL) {
		printf("\n");
	}
	else {
		p = p->Child;
		while(p != NULL) {
			printf("%s ",p->name);
			p = p->Brother; 
		}
		printf("\n");
	}
}

//cd操作 
FileFolder* cd(char cmd[50],FileFolder* now,char** command,FileFolder* root) {
	FileFolder* p = now;
	FileFolder* rootp = root;
	if(strcmp(cmd,"cd") == 0) {
		while(strcmp(p->name,"root") != 0){
			p = p->Father;
		}
		return p;
	}
	else if(strcmp(cmd,"cd ..") == 0) {
		p = p->Father;
		return p;
	}
	else if(command[1][3] == '/') {
		char* path[20];
		int l = strlen(cmd);
		char* cmdf;
		strcpy(cmdf,cmd);
		split(cmd,"/",path);
		int i = 0;
		int time = 0;
		for(i = 0; i < l; i++) {
			if(cmdf[i] == '/') {
				time++;
			}
		}
		for(i = 1; i < 1 + time; i++) {
			rootp = rootp->Child;
			while(rootp && strcmp(rootp->name,path[i]) != 0){		
				rootp = rootp->Brother; 
			}
			if(rootp == NULL || strcmp(rootp->type,"File") == 0) {
				printf("error\n");
				return now;
			}
		}
		return rootp;
	}
	else {
		p = p->Child;
		while(p && strcmp(p->name,command[1]) != 0) {
			p = p->Brother; 
		}
		if(p == NULL || strcmp(rootp->type,"File") == 0) {
			printf("error");
			return now;
		}
		else {
			return p;
		}
	}
}

//显示路径 
void pwd(FileFolder* p){
	if(strcmp(p->type,"FileFolder") == 0) {
		printf("%s",p->pwd);
		printf("\n");
	}
	else {
		printf("%s",p->Father->pwd);
		printf("\n");
	}
	
}

//查看一个指定文件的内容 
void cat(char cmd[50], char **command,FileFolder* now, FileFolder* root) {
	
	if(cmd[5] == '/' || cmd[7] == '/') {
		FileFolder* p;
		if(cmd[5] == '/'){
			p = now;
		}
		else if(cmd[7] == '/'){
			p = root;
		}
		char cmdf[50];
		strcpy(cmdf,cmd);
		int i = 0,time = 0;
		char* path[20];
		int l = strlen(cmd);
		split(cmd,"/",path);
		for(i = 0; i < l; i++) {
			if(cmdf[i] == '/') {
				time++;
			}
		}
		for(i = 1; i < time; i++) {
			p = p->Child;
			while(p && strcmp(p->name,path[i]) != 0){		
				p = p->Brother; 
			}
			if(p == NULL || strcmp(p->type,"File") == 0) {
				printf("error");
				return;
			}
		}
		p = p->Child;
		while(p && strcmp(p->name,path[time]) != 0){		
				p = p->Brother; 
		}
		if(p == NULL || strcmp(p->type,"FileFolder") == 0) {
			printf("error");
			return;
		}
		else {
			printf("%s\n" ,p->content);
		}
	}	
	else {
		printf("error\n");
	}
	
}

//修改一个指定文件的内容 
void tac(char cmd[50], char **command,FileFolder* now, FileFolder* root) {
	if(cmd[5] == '/' || cmd[7] == '/') {
		FileFolder* p;
		if(cmd[5] == '/'){
			p = now;
		}
		else if(cmd[7] == '/'){
			p = root;
		}
		char cmdf[50];
		strcpy(cmdf,cmd);
		int i = 0,time = 0;
		char* path[20];
		int l = strlen(cmd);
		split(cmd,"/",path);
		for(i = 0; i < l; i++) {
			if(cmdf[i] == '/') {
				time++;
			}
		}
		for(i = 1; i < time; i++) {
			p = p->Child;
			while(p && strcmp(p->name,path[i]) != 0){		
				p = p->Brother; 
			}
			if(p == NULL || strcmp(p->type,"File") == 0) {
				printf("error");
				return;
			}
		}
		p = p->Child;
		char filename[20];
		char content[50];
		int len = strlen(path[time]);
		int k = 0;
		for(i = 0; i < len; i++) {
			if(path[time][i] == ' ') {
				k = i;
				break;
			}
		}
		int y = 0;
		for(i = k + 1; i < len; i++) {
			content[y] = path[time][i];
			y++;
		}
		char* res[10];
		split(path[time]," ",res);
		strcpy(filename,res[0]);
		while(p && strcmp(p->name,filename) != 0){		
				p = p->Brother; 
		}
		if(strcmp(p->type,"FileFolder") == 0) {
			printf("error\n");
		}
		else{
			strcpy(p->content,content);
		}
		
	}
}

//与用户进行交互操作 
void getcommand(FileFolder* root) {
	FileFolder* now = root;
	char cmd[50];
	char cmdf[50];
	char** command;
	int i = 0;
	command = (char**)malloc(sizeof(char *) * 100);
	for( i=0;i<100;i++ ) {
		command[i]= (char*)malloc(sizeof(char)*100);
		memset(command[i],'\0',sizeof(command[i]));
	}
	gets(cmd);
	strcpy(cmdf,cmd);
	split(cmd," ",command);
	while(1) {
		if(strcmp(command[0],"pwd") == 0) {
			pwd(now);
		}
		else if(strcmp(command[0],"makdir") == 0) {
			makdir(now,command);
		}
		else if(strcmp(command[0],"touch") == 0) {
			touch(now,command);
		}
		else if(strcmp(command[0],"ls") == 0) {
			ls(now);
		}
		else if(strcmp(command[0],"cd") == 0) {
			now = cd(cmdf,now,command,root);
		}
		else if(strcmp(command[0],"cat") == 0) {
			cat(cmdf,command,now,root);
		}
		else if(strcmp(command[0],"tac") == 0) {
			tac(cmdf,command,now,root);
		}
		else{
			printf("error");
		}
		gets(cmd);
		strcpy(cmdf,cmd);
		split(cmd," ",command);
	}
}

int main() {
	FileFolder* root = init();
	getcommand(root);
} 







































