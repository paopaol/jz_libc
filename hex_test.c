#include <stdio.h>
#include "encoding.h"


int main(void)
{
	unsigned char	src[6] = {0x3f, 0xf3, 0xff, 0xef, 0xfe, 0xcd};
	char	dst[256];
	int	n;

	n = encoding_hex_encode(dst, sizeof(dst),
					src, 6);
	if(n < 0){
		return 1;
	}
	printf("%d\n", n);
	puts(dst);
	return 0;
}

