#include <string.h>
#include "encoding.h"

int encoding_bcd_to_str(int _lr,//left or right
		char *_str, int _strlen, const char *_bcd, int _bcd_count)
{
	int	i = 0;
	char	high;
	char	low;
	int 	bcd_count;

	if(_strlen + 1 < _bcd_count){
		return -1;
	}
	bcd_count = (_bcd_count % 2 == 0) ? _bcd_count : _bcd_count + 1;
	for(i = 0; i < bcd_count;){
		high = ((*_bcd & 0xf0) >> 4) | 0x30;
		low = (*_bcd & 0x0f) | 0x30;
		_str[i++] = high;
		_str[i++] = low;
		_bcd++;
	}
	_str[bcd_count] = '\0';
	if(_lr == BCD_RIGHT && _bcd_count % 2 == 1){
		memmove(_str, _str + 1, _bcd_count);
		_str[_bcd_count] = '\0';
	}else if(_lr == BCD_LEFT && _bcd_count % 2 == 1){
		_str[_bcd_count] = '\0';
	}
	return 0;
}

int encoding_bcdl_to_str(
		char *_str, int _strlen, const char *_bcd, int _bcd_count)
{
	return encoding_bcd_to_str(BCD_LEFT,
				_str, _strlen, _bcd,  _bcd_count);
}
int encoding_bcdr_to_str(
		char *_str, int _strlen, const char *_bcd, int _bcd_count)
{
	return encoding_bcd_to_str(BCD_RIGHT,
				_str, _strlen, _bcd, _bcd_count);
}

int encoding_str_to_bcd(int _lr,
		char *_bcd, int _bcd_len, const char *_str, int _str_len)
{
	char 	low;
	char	high;
	int	str_len = _str_len;
	int	i = 0;
	int	use_len;

	use_len = _str_len % 2 == 0 ? _str_len / 2 : (_str_len + 1) / 2;
	if(_bcd_len < use_len){
		return -1;
	}

	if(_str_len % 2 == 1){
		if(_lr == BCD_RIGHT){
			high = 0;
			low = *_str++ & 0x0f;
			*_bcd++ = high | low;
			str_len -= 1;
		}else {
			str_len -= 1;
		}
	}
	
	for(i = 0; i < str_len; i++){
		high = (_str[i] & 0x0f) << 4;
		i++;
		low = _str[i] & 0x0f;
		*_bcd++ = high | low;
	}
	if(_str_len % 2 == 1){
		if(_lr == BCD_LEFT){
			high = (_str[i] & 0x0f) << 4;
			low = 0;
			*_bcd = high | low;
		}
	}
	return (_str_len % 2 == 0) ? _str_len / 2 : (_str_len +1) / 2;
}
int encoding_str_to_bcdl(
		char *_bcd, int _bcd_len, const char *_str, int _str_len)
{
	return encoding_str_to_bcd(BCD_LEFT,
				_bcd, _bcd_len, _str, _str_len);
}

int encoding_str_to_bcdr(
		char *_bcd, int _bcd_len, const char *_str, int _str_len)
{
	return encoding_str_to_bcd(BCD_RIGHT,
				_bcd, _bcd_len, _str, _str_len);
}



