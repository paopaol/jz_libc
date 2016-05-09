#ifndef JZ_BUFFER_H
#define JZ_BUFFER_H

#include "jz_type.h"

typedef struct jz_buffer_interfce_t {
    jz_bool (*init)(jz_buffer_ref buffer);
    jz_bool (*exit)(jz_buffer_ref buffer);
    jz_bool (*ncat)(jz_buffer_ref dst, jz_buffer_ref src, jz_int len);
    jz_bool (*ncat_string)(jz_buffer_ref dst, const jz_char *src, jz_int len);
    jz_bool (*cat)(jz_buffer_ref dst, jz_buffer_ref src);
    jz_bool (*cat_string)(jz_buffer_ref dst, const jz_char *src);
    jz_bool (*ncpy)(jz_buffer_ref dst, jz_buffer_ref src, jz_int len);
    jz_bool (*ncpy_string)(jz_buffer_ref dst, const jz_char *src, int len);
    jz_bool (*cpy)(jz_buffer_ref dst, jz_buffer_ref src);
    jz_bool (*cpy_string)(jz_buffer_ref dst, const jz_char *src);
    jz_bool (*ninsert)(jz_buffer_ref buffer, jz_int index, jz_buffer_ref sub, jz_int n);
    jz_bool (*ninsert_string)(jz_buffer_ref buffer, jz_int index, const jz_char *sub, jz_int n);
    jz_bool (*insert)(jz_buffer_ref buffer, jz_int index, jz_buffer_ref sub);
    jz_bool (*insert_string)(jz_buffer_ref buffer, jz_int index, const jz_char *sub);
    jz_bool (*nequals)(jz_buffer_ref buffer1, jz_buffer_ref buffer2, jz_int len);
    jz_bool (*nequals_string)(jz_buffer_ref buffer, const jz_char *str, jz_int len);
    jz_bool (*equals)(jz_buffer_ref buffer1, jz_buffer_ref buffer2);
    jz_bool (*equals_string)(jz_buffer_ref buffer, const jz_char *str);
    jz_ptr (*val)(jz_buffer_ref buffer);
    jz_int (*len)(jz_buffer_ref buffer);
}jz_buffer_interface;

extern const jz_buffer_interface  jz_buffer;

#endif







