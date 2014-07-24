#include <stdio.h>
#include <string.h>

#include "encoding.h"


int main()
{
	char	*str = "12345";
	char	bcd[30];
	int	len;
	int 	i;	
	char	str_arr[60];


	printf("%s -->encoding_str_to_bcdl\n", str);
	len = encoding_str_to_bcdl(bcd, sizeof(bcd), str, strlen(str));	
	if(len < 0){
		printf("encoding_str_to_bcdl err\n");
		return 1;
	}
	printf("%d\n", len);
	for(i = 0; i < len; i++){
		printf("%x", bcd[i]);
	}
	puts("");
	puts("");

	printf("encoding_bcdl_to_str\n");
	len = encoding_bcdl_to_str(str_arr, sizeof(str_arr), bcd, 5);	
	if(len < 0){
		printf("encoding_bcdl_to_str err\n");
		return 1;
	}
	puts(str_arr);






	printf("%s -->encoding_str_to_bcdr\n", str);
	len = encoding_str_to_bcdr(bcd, sizeof(bcd), str, strlen(str));	
	if(len < 0){
		printf("encoding_str_to_bcdr err\n");
		return 1;
	}
	printf("%d\n", len);
	for(i = 0; i < len; i++){
		printf("%02x", bcd[i]);
	}
	puts("");
	puts("");



	printf("encoding_bcdr_to_str\n");
	len = encoding_bcdr_to_str(str_arr, sizeof(str_arr), bcd, 5);	
	if(len < 0){
		printf("encoding_bcdr_to_str err\n");
		return 1;
	}
	puts(str_arr);



	return 0;
}
