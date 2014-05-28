#include <stdio.h>


static const char *hextable = "0123456789abcdef";


int encoding_hex_encode(char *dst, int dst_len,
			const char *src, int src_len)
{
	int	i;

	//ff
	if(src_len * 2 > dst_len){
		return -1;
	}
	for(i = 0; i < src_len; i++){
		//in mingwin: 0xf3 >> 4 is not 0x0f, it is 0xff. 
		dst[i * 2] = hextable[(src[i] >> 4) & 0x0f];
		dst[i * 2 + 1] = hextable[src[i] & 0x0f];
	}
	dst[src_len * 2] = '\0';
	return src_len * 2;
}

static int _from_hexchar(char c)
{
	if(c >= '0' && c <= '9'){
		return c - '0';
	}
	if(c >= 'a' && c <= 'f'){
		return c - 'a' + 10;
	}
	if(c >= 'A' && c <= 'F'){
		return c - 'A' + 10;
	}
	return -1;
}


int encoding_hex_decode(char *dst, int dst_len,
			const char *src, int src_len)
{
	int	high, low;
	int	i;


	if(src_len % 2 == 1){
		return -1;
	}
	if(src_len / 2 > dst_len){
		return -1;
	}
	for(i = 0; i < src_len / 2; i++){
		high = _from_hexchar(i * 2);
		low = _from_hexchar(i * 2 + 1);
		dst[i] = ((high << 4) & 0xf0) | (low & 0x0f);
	}
	return src_len / 2;
}


