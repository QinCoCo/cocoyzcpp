#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <cstring>


int ping(char *svrip)
{
	int i = 0;
	while (i < 3)
	{
		pid_t pid;
		if ((pid = vfork()) < 0)
		{
			exit(1);
		}
		else if (pid == 0)
		{
			if (execlp("ping", "ping", "-c", "1", svrip, (char *)0) < 0)
			{
				exit(1);
			}
		}

		int stat;
		//waitpid(pid, &stat, 0);

		if (stat == 0)
		{
			return 0;
		}
		sleep(3);
		i++;
	}
	return -1;
}

char *Path_str(char *path)
    {
    	FILE *fp;
        if ((fp = fopen(path, "r")) == NULL)
    	{
		return NULL;
    	}
    	fseek(fp, 0, SEEK_END);
    	int filesize = ftell(fp);
    	char *str;
    	str = (char *)malloc(filesize);
    	rewind(fp);
    	fread(str, 1, filesize, fp);
    	str[filesize] = '\0';
    	fclose(fp);
    	return str;
    }

char *post(char *url, char *str)
{
	char cmd[512] = "curl -sd '";
	strcat(cmd, str);
	strcat(cmd, "' ");
	strcat(cmd, url);
	FILE *fp = popen(cmd, "r");
	if (fp == NULL)
	{
		return "NULL";
	}
	char *ret;
	ret = (char *)malloc(sizeof(char) * 1024);
	char buf[1024];
	while (fgets(buf, 1024, fp) != NULL)
		strcat(ret, buf);
	if (pclose(fp) == -1)
	{
		return "NULL";
	}
	return ret;
}

char*  json_str(char* str, char* front, char* rear){
	if(!str || !front || !rear)return NULL;char* s;char* t;while(*str) {s = str;t = front;while (*s == *t) {s++;t++;if (!*t) {str = s;char* old = str;do{s = str;t = rear;while (*s == *t) {s++;t++;if (!*t) {int charlen = str - old;char* newstr = (char*)malloc(charlen + 1);strncpy(newstr, old, charlen);newstr[charlen] = '\0';return newstr;}}str++;}while(*str);return NULL;}}str++;}
	return NULL;
}

void del_chr(char *s, char ch)
{
	char *t = s;				// 目标指针先指向原串头
	while (*s != '\0')			// 遍历字符串s
	{
		if (*s != ch)			// 如果当前字符不是要删除的，则保存到目标串中
			*t++ = *s;
		s++;					// 检查下一个字符
	}
	*t = '\0';					// 置目标串结束符。
}

long long GetTick(char *str_time)
{
   /* struct tm stm;
    int iY, iM, iD, iH, iMin, iS;
 
    memset(&stm,0,sizeof(stm));
 
    iY = atoi(str_time);
    iM = atoi(str_time+5);
    iD = atoi(str_time+8);
    iH = atoi(str_time+11);
    iMin = atoi(str_time+14);
    iS = atoi(str_time+17);
 
    stm.tm_year=iY-1900;
    stm.tm_mon=iM-1;
    stm.tm_mday=iD;
    stm.tm_hour=iH;
    stm.tm_min=iMin;
    stm.tm_sec=iS;
    return mktime(&stm);*/
   struct tm timeinfo;
    strptime(str_time, "%Y-%m-%d %H:%M:%S",  &timeinfo);
    time_t timeStamp = mktime(&timeinfo);
    return timeStamp;
}

