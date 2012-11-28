#if !defined(__LOG_H__)
#	define __LOG_H__

char *text[] = { "asdfasdf", "dfdfdfdff", "acjesqqq", "asdbghf", "ppppppp", "q5q5q5q5r", "atystyatw", "ccmcckddk", "yusyustd", "pp[]]", "qqqqqqqqq", "mqmqmsjjhdhihdhuus" };

char *parse(int line);

char *parse(int line)
{
	if(line > 1000000) return 0;
	else 
	{
		printf("PARSE: %d\n", line);
		return text[rand()%12];
		line++;
	}
}

#endif /* __LOG_H__ */