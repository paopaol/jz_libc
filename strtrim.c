#include <string.h>

/*
 *函数名称：ltrim 
 *函数功能：除去字符串开头的空白符。 
 *输入参数：s:输入字符串。 
 *输出参数：s:输出字符串。 
 *返回值：空 
 */
static void _ltrim(char *s)  
{  
	char 	*p = s;

	while(*p == ' ' || *p == '\t'){
		p++;
	}  
    	memmove(s,p, strlen(p) + 1);  
}  
  
/*
 *函数名称：rtrim 
 *函数功能：除去字符串末尾的空白符。 
 *输入参数：s:输入字符串。 
 *输出参数：s:输出字符串。 
 *返回值：空 
 */  
static void _rtrim(char *s)  
{  
	char	*p;

	p = s + strlen(s) - 1;
	while(1){
		if((*p == ' ' || *p == '\t') && *(p + 1) == '\0'){
			*p-- = '\0';
		}else {
			break;
		}
	}
}  
  
/*
 *函数名称：trim 
 *函数功能：除去字符串开头和末尾的空白符。 
 *输入参数：s:输入字符串。 
 *输出参数：s:输出字符串。 
 *返回值：空 
 */  
void strtrim(char *s)  
{  
	_ltrim(s);  
	_rtrim(s);  
}  
