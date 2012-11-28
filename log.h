#if !defined(__LOG_H__)
#	define __LOG_H__
//-----------------------------------------------------------------------------

#include <stdio.h>

#define BUFSIZE 1024
//-----------------------------------------------------------------------------

// char *text[] = { "asdfasdf", "dfdfdfdff", "acjesqqq", "asdbghf", "ppppppp", "q5q5q5q5r", "atystyatw", "ccmcckddk", "yusyustd", "pp[]]", "qqqqqqqqq", "mqmqmsjjhdhihdhuus" };
FILE *fd;
//-----------------------------------------------------------------------------

int parser_init(const char *filename);
int parser_exit(void);
char *parse(void);
//-----------------------------------------------------------------------------

int parser_init(const char *filename)
{
	char *command = (char*)malloc(28+2*strlen(filename));
	char *tmpfn = (char*)malloc(5+strlen(filename));
	sprintf(tmpfn, "%s.tmp", filename);
	sprintf(command, "cat %s | cut -d\' \' -f7 > %s", filename, tmpfn);
	system(command);
	fd = fopen(tmpfn, "r");
}
//-----------------------------------------------------------------------------

int parser_exit(void)
{
	return fclose(fd);
}
//-----------------------------------------------------------------------------

char *parse(void)
{
	char buffer[BUFSIZE];
	if(fgets(buffer, BUFSIZE, fd)) 
	{
		buffer[strlen(buffer)-1]='\0';
		return buffer;
	}
	return NULL;
}
//-----------------------------------------------------------------------------

#endif /* __LOG_H__ */