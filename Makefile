jz_libc-src  = clist.c
jz_libc-src  += hex.c
jz_libc-src  += strtrim.c
jz_libc-src  += bcd.c

jz_libc-objs = $(patsubst %.c, %.o, $(jz_libc-src))

lib-jz_libc = libjz_libc.a

$(lib-jz_libc):$(jz_libc-objs)
	$(AR)  cr $@ $^
	@$(CP) $@ $(ROOT)/lib
$(jz_libc-objs):$(jz_libc-src)
%.o:%.c
	$(CC) -c $< -o $@
clean:
	$(RM) -rf $(lib-jz_libc)
	$(RM) -rf $(jz_libc-objs)

