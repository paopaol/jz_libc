#ifndef __ENCODING_H
#define __ENCODING_H





//hex
extern int encoding_hex_encode(char *dst, int dst_len,
			const char *src, int src_len);
extern int encoding_hex_decode(char *dst, int dst_len,
			const char *src, int src_len);





//bcd
#define BCD_LEFT  0
#define BCD_RIGHT 1

/*encoding_bcd_to_str:
 *@_lr:BCD_LEFT or BCD_RIGHT
 *@_str:out buffer
 *@_strlen:out buffer len
 *@_bcd:bcd codes
 *@_bcd_count:bcd code numbers.eg B313233 ->H0123,then _bcd_count is 3
 *@return:if out buffer is to small, return -1,else return 0
 */
extern int encoding_bcd_to_str(int _lr,//left or right
		char *_str, int _strlen, const char *_bcd, int _bcd_count);
extern int encoding_bcdr_to_str(
		char *_str, int _strlen, const char *_bcd, int _bcd_count);
extern int encoding_bcdl_to_str(
		char *_str, int _strlen, const char *_bcd, int _bcd_count);


/*encoding_str_to_bcd:
 *@_lr:BCD_LEFT or BCD_RIGHT
 *@_bcd:out buffer
 *@_bcd_len:out buffer len
 *@_str:string codes
 *@_str_len:len of _str
 *@return:if out buffer is to small, return -1,else return bcd_len bytes
 */
extern int encoding_str_to_bcd(int _lr,
		char *_bcd, int _bcd_len, const char *_str, int _str_len);
extern int encoding_str_to_bcdl(
		char *_bcd, int _bcd_len, const char *_str, int _str_len);
extern int encoding_str_to_bcdr(
		char *_bcd, int _bcd_len, const char *_str, int _str_len);


#endif
