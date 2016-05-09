#ifndef JZ_TYPE_H
#define JZ_TYPE_H

#include "jz_assert.h"

typedef int	jz_int;
typedef unsigned int jz_uint;
typedef char jz_int8;
typedef unsigned char jz_uint8;
typedef short jz_int16;
typedef unsigned short jz_uint16;
typedef long jz_long;
typedef unsigned long jz_ulong;
typedef long long jz_llong;
typedef unsigned long long jz_ullong;
typedef void* jz_ptr;
typedef jz_uint	jz_size;
typedef jz_uint	jz_bool;
typedef char jz_char;

#define false 	0
#define true 	1




#define defaultCap	32

typedef struct jz_buffer_t {
    jz_ptr		p;
    jz_size		len;
    jz_size		cap;
    jz_int8		buffer[	defaultCap];
}jz_buffer_t, *jz_buffer_ref;


#endif
