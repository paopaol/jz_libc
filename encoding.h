#ifndef __ENCODING_H
#define __ENCODING_H
extern int encoding_hex_encode(char *dst, int dst_len,
			const char *src, int src_len);
extern int encoding_hex_decode(char *dst, int dst_len,
			const char *src, int src_len);

#endif
