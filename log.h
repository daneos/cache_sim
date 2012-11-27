#if !defined(__LOG_H__)
#	define __LOG_H__

char text[] = "asdfasdf";

char *parse(int line);

char *parse(int line)
{
	if(line > 1000) return 0;
	else 
	{
		printf("PARSE: %d\n", line);
		return text;
		line++;
	}
}

#endif /* __LOG_H__ */