#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "jz_type.h"
#include "jz_buffer.h"

static jz_bool jz_buffer_rewind_cap(jz_buffer_ref buffer, jz_size cap);
static jz_bool jz_buffer_rewind_size(jz_buffer_ref buffer, jz_size size);

static jz_bool jz_buffer_init(jz_buffer_ref buffer)
{
    jz_assert_return_val(buffer, false);
    buffer->p = buffer->buffer;
    buffer->len = 0;
    buffer->cap = defaultCap;
    return true;
}

static jz_bool jz_buffer_exit(jz_buffer_ref buffer)
{
    jz_assert_return_val(buffer, false);
    if (buffer->cap == defaultCap){
        return true;
    }
    free(buffer->p);
    return true;
}


static jz_bool jz_buffer_ncat(jz_buffer_ref dst, jz_buffer_ref src, jz_int len)
{
    jz_assert_return_val((dst && src), false);
    if (dst->cap < (dst->len + len)){
        jz_assert_return_val(jz_buffer_rewind_cap(dst, dst->len + len), false);
    }
    memcpy(dst->p + dst->len, src->p, len);
    dst->len += len;
    return true;
}

static jz_bool jz_buffer_ncat_string(jz_buffer_ref dst, const jz_char *src, jz_int len)
{
    jz_assert_return_val((dst && src), false);
    len = strlen((char *)src);
    if (dst->cap < (dst->len + len)){
        jz_assert_return_val(jz_buffer_rewind_cap(dst, dst->len + len), false);
    }
    memcpy(dst->p + dst->len, src, len);
    dst->len += len;
    return true;
}

static jz_bool jz_buffer_cat(jz_buffer_ref dst, jz_buffer_ref src)
{
    jz_assert_return_val((dst && src), false);
    return jz_buffer_ncat(dst, src, src->len);
}

static jz_bool jz_buffer_cat_string(jz_buffer_ref dst, const jz_char *src)
{
    jz_assert_return_val((dst && src), false);
    return jz_buffer_ncat_string(dst, src, strlen((char *)src));
}

static jz_bool jz_buffer_ncpy(jz_buffer_ref dst, jz_buffer_ref src, jz_int len)
{
    jz_assert_return_val(dst && src, false);
    jz_buffer_rewind_size(dst, 0);
    return jz_buffer_ncat(dst, src, len);
}

static jz_bool jz_buffer_cpy(jz_buffer_ref dst, jz_buffer_ref src)
{
    jz_assert_return_val(dst && src, false);
    jz_buffer_rewind_size(dst, 0);
    return jz_buffer_cat(dst, src);
}

static jz_bool jz_buffer_ncpy_string(jz_buffer_ref dst, const jz_char *src, int len)
{
    jz_assert_return_val(dst && src, false);
    jz_buffer_rewind_size(dst, 0);
    return jz_buffer_ncat_string(dst, src, len);
}

static jz_bool jz_buffer_cpy_string(jz_buffer_ref dst, const jz_char *src)
{
    jz_assert_return_val(dst && src, false);
    jz_buffer_rewind_size(dst, 0);
    return jz_buffer_cat_string(dst, src);
}


static jz_bool jz_buffer_ninsert(jz_buffer_ref buffer, jz_int index, jz_buffer_ref sub, jz_int n)
{
    jz_assert_return_val(buffer && sub, false);
    jz_assert_return_val(buffer->len > index, false);

    if (buffer->len + n > buffer->cap){
        jz_assert_return_val(jz_buffer_rewind_cap(buffer, buffer->len + n), false);
    }
    memmove(buffer->p + index + n, buffer->p + index, buffer->len - index);
    memcpy(buffer->p + index, sub->p, n);
    buffer->len += n;
    return true;
}

static jz_bool jz_buffer_insert(jz_buffer_ref buffer, jz_int index, jz_buffer_ref sub)
{
    jz_assert_return_val(buffer && sub, false);
    return jz_buffer_ninsert(buffer, index, sub, sub->len);
}

//    jz_buffer.cpy_string(&version, "j32");
//    write(1, version.p, version.len);
//    puts("");
//    //jkkk
//    jz_buffer.insert_string(&version, 1, "v1.0");
static jz_bool jz_buffer_ninsert_string(jz_buffer_ref buffer, jz_int index, const jz_char *sub, jz_int n)
{
    jz_assert_return_val(buffer && sub, false);
    jz_assert_return_val(buffer->len > index, false);
    if (buffer->len + n > buffer->cap){
        jz_assert_return_val(jz_buffer_rewind_cap(buffer, buffer->len + n), false);
    }
    memmove(buffer->p + index + n, buffer->p + index, buffer->len - index);
    memcpy(buffer->p + index + 1, sub, n);
    buffer->len += n;
    return true;
}

static jz_bool jz_buffer_insert_string(jz_buffer_ref buffer, jz_int index, const jz_char *sub)
{
    jz_assert_return_val(buffer && sub, false);
    return jz_buffer_ninsert_string(buffer, index, sub, strlen((char *)sub));
}

static jz_bool jz_buffer_nequals(jz_buffer_ref buffer1, jz_buffer_ref buffer2, jz_int len)
{
    jz_assert_return_val(buffer1 && buffer2, false);

    if (buffer1->len < len){
        return false;
    }
    if (memcmp(buffer1->p, buffer2->p, len) != 0){
        return false;
    }
    return true;
}

static jz_bool jz_buffer_equals(jz_buffer_ref buffer1, jz_buffer_ref buffer2)
{
    jz_assert_return_val(buffer1 && buffer2, false);

    return jz_buffer_nequals(buffer1, buffer2, buffer2->len);
}

static jz_bool jz_buffer_nequals_string(jz_buffer_ref buffer, const jz_char *str, jz_int len)
{
    jz_assert_return_val(buffer && str, false);
    if (buffer->len < len){
        return false;
    }
    if (memcmp(buffer->p, str, len) != 0){
        return false;
    }
    return true;
}

static jz_bool jz_buffer_equals_string(jz_buffer_ref buffer, const jz_char *str)
{
    jz_assert_return_val(buffer && str, false);
    return jz_buffer_nequals_string(buffer, str, strlen((char *)str));
}

static jz_bool jz_buffer_rewind_size(jz_buffer_ref buffer, jz_size size)
{
    jz_assert_return_val(buffer && (size >= 0), false);
    buffer->len = 0;
    return true;
}

static jz_bool jz_buffer_rewind_cap(jz_buffer_ref buffer, jz_size new_cap)
{
    jz_ptr		p;

    jz_assert_return_val(buffer && (new_cap >= 0), false);

    if (buffer->cap == defaultCap && new_cap <= defaultCap){
        return true;
    }
    if (buffer->cap >= new_cap){
        return true;
    }
    p = malloc(new_cap);
    jz_assert_return_val(p, false);
    memcpy(p, buffer->p, buffer->len);
    if (buffer->cap > defaultCap){
        free(buffer->p);
    }
    buffer->p = p;
    buffer->cap = new_cap;
    return true;
}

static jz_ptr jz_buffer_val(jz_buffer_ref buffer)
{
    jz_assert_return_val(buffer, NULL);
    jz_assert_return_val(buffer->len, NULL);
    return buffer->p;
}

static jz_int jz_buffer_len(jz_buffer_ref buffer)
{
    jz_assert_return_val(buffer, -1);
    return buffer->len;
}


const jz_buffer_interface jz_buffer = {
    jz_buffer_init,
    jz_buffer_exit,

    jz_buffer_ncat,
    jz_buffer_ncat_string,

    jz_buffer_cat,
    jz_buffer_cat_string,

    jz_buffer_ncpy,
    jz_buffer_ncpy_string,

    jz_buffer_cpy,
    jz_buffer_cpy_string,

    jz_buffer_ninsert,
    jz_buffer_ninsert_string,

    jz_buffer_insert,
    jz_buffer_insert_string,

    jz_buffer_nequals,
    jz_buffer_nequals_string,

    jz_buffer_equals,
    jz_buffer_equals_string,

    jz_buffer_val,
    jz_buffer_len
};
