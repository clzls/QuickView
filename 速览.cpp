#include "stdafx.h"

FILE *FTedb,*FCedb,*cfg;

//IDB:Internal DataBase
//EDB:External DataBase 
bool findFTInIDB(char *str,char *fileSuffix)
{
	for(int i = 0;i < fileTypeLibSize;++i)
		if (strcmp(fileTypes[i]fileType_Suffix,fileSuffix) == 0)
		{
			sprintf(str,"%s(.%s)\n\t打开方式：%s",
				fileTypes[i]fileType_Dest,
				fileSuffix,
				fileTypes[i]fileType_OM);
			return true;
		}
	return false;
}

bool findFTInEDB(char *str,char *fileSuffix)
{
	if (FTedb == NULL) return false;
	char dest[330],ext[150],om[330];
	for(;!feof(FTedb);)
	{
		do fgets(ext ,150,FTedb);while ((strlen(ext ) <= 1 || ext [0] == '/') && !feof(FTedb));
		do fgets(om  ,330,FTedb);while ((strlen(om  ) <= 1 || om  [0] == '/') && !feof(FTedb));
		do fgets(dest,330,FTedb);while ((strlen(dest) <= 1 || dest[0] == '/') && !feof(FTedb));
	//	printf("Checking %s(%d).%s.%s.\n",ext,ext[0],om,dest);
		ext [strlen(ext ) - 1] = '\0';
		om  [strlen(om  ) - 1] = '\0';
		dest[strlen(dest) - 1] = '\0';
		if (strcmp(ext,fileSuffix) == 0)
		{
			sprintf(str,"%s(.%s)\n\t打开方式：%s",dest,fileSuffix,om);
			rewind(FTedb);
			return true;
		}
	}
	rewind(FTedb);
	return false;
}

bool getFileType(char *str,char *fileName)
{
	//Get suffix(external name)
	//截取扩展名
	char *fileSuffix = strchr(fileName,'.'),*tmp;
	if (fileSuffix == NULL)
	{
		sprintf(str,"未知文件 无扩展名");
		return false; 
	}
	while ((tmp = strchr(fileSuffix+1,'.')) != NULL) fileSuffix = tmp;
	++fileSuffix;
	
	sprintf(str,"未知文件(.%s)",fileSuffix);
	return findFTInIDB(str,fileSuffix) || findFTInEDB(str,fileSuffix);
}

bool getContentType(char *str,char *fileContent)
{
	while (strlen(fileContent) > 1 && (fileContent[0] <= 20 || fileContent[0] >= 127)) ++fileContent;
	sprintf(str,"未知 或 未收录(%s)",fileContent);
	return false;
}

void openEDB()
{
	Eopen(FTedb,"FTedb.ini")
	Eopen(FCedb,"FCedb.ini")
	Eopen(cfg,"QVcfg.ini")
}

int main()
{
	//quickView
	if (!system(NULL))
	{
		perror("命令行启动失败");
		exit(EXIT_FAILURE);
	}
	openEDB();
	printf("请输入文件名（或拖入文件）：");
	char tmp[1000],fileContent[bytesToRead+5];
	char *fileName;
	gets(tmp);
	
	//去除多余的冒号 
	fileName = (tmp[0] == '"')?(tmp[strlen(tmp) - 1] = '\0',tmp+1):tmp;
	
	printf("文件名：%s\n",fileName);
	FILE* fin = fopen(fileName,"rb");
	if (fin == NULL)
	{
		perror("文件打开失败");
		
		system("ping /n 3 127.1>nul 2>nul");
		exit(EXIT_FAILURE);
	}
	int result = fread(fileContent,1,bytesToRead,fin);
	
	char fileType[100],contentType[100];
	
	bool known = getFileType(fileType,fileName);
	known = getContentType(contentType,fileContent) || known;
	//强制执行两个函数，而不是在gFT成功后不执行gCT 
	
	if (!known)
	{
		printf("文件的前%d个字节：\n%s\n",result,fileContent);
		puts("字节码：\n");
		for(int i = 0;i < result;++i) printf("#%d:%d\n",i,fileContent[i]);
	}
	
	printf("文件类型推测：%s\n",fileType);
	printf("内容类型推测：%s\n",contentType); 
	
	puts("程序成功结束。");
	system("ping /n 3 127.1>nul 2>nul");
	return 0;
}
