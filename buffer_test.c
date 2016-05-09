#include <fcntl.h>
#include <stdio.h>
#include "jz_buffer.h"


typedef struct person {
    jz_buffer_t	 name;
    jz_buffer_t	 addr;
}person_t;



int main(void)
{
    jz_buffer_t	version;
    jz_buffer_t	old_version;

    jz_buffer.init(&version);
    jz_buffer.init(&old_version);
    //jkkk
    jz_buffer.insert_string(&version, 6, "v1.0$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
    write(1, version.p, version.len);
    puts("");



    jz_buffer.cat(&version, &old_version);
    write(1, version.p, version.len);
    puts("");
    jz_buffer.cat_string(&version, "jinzhao");
    write(1, version.p, version.len);
    puts("");

    jz_buffer.cpy(&old_version, &version);
    write(1, old_version.p, old_version.len);
    puts("");
    jz_buffer.cpy_string(&version, "j23456789");
    write(1, version.p, version.len);
    puts("");

    jz_buffer.cpy_string(&version, "jinzhao");
    if (jz_buffer.equals_string(&version, "jinzhao")){
        puts("ok");
    }
    jz_buffer.cpy_string(&old_version, "jinzhao");
    if (jz_buffer.nequals(&version, &old_version, 2)){
        puts("ok");
    }













    jz_buffer.exit(&version);
    jz_buffer.exit(&old_version);
    return 0;
}
