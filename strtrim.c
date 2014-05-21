/*
 *函数名称：ltrim 
 *函数功能：除去字符串开头的空白符。 
 *输入参数：s:输入字符串。 
 *输出参数：s:输出字符串。 
 *返回值：空 
 */
static void ltrim(char *s)  
{  
    char *p;  
    p = s;  
    while(*p == ' ' || *p == '\t'){*p++;}  
    strcpy(s,p);  
}  
  
/*
 *函数名称：rtrim 
 *函数功能：除去字符串末尾的空白符。 
 *输入参数：s:输入字符串。 
 *输出参数：s:输出字符串。 
 *返回值：空 
 */  
static void rtrim(char *s)  
{  
    int i;  
    i = strlen(s)-1;  
    while((s[i] == ' ' || s[i] == '\t') && i >= 0){i--;};  
    s[i+1] = '\0';  
}  
  
/*
 *函数名称：trim 
 *函数功能：除去字符串开头和末尾的空白符。 
 *输入参数：s:输入字符串。 
 *输出参数：s:输出字符串。 
 *返回值：空 
 */  
void trim(char *s)  
{  
    ltrim(s);  
    rtrim(s);  
}  
