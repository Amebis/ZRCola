CPPFLAGS := $(CPPFLAGS) -I../lib/libZRCola/include -I../lib/stdex/include -I/usr/local/include/oatpp-1.3.0/oatpp
LDFLAGS := $(LDFLAGS) -lstdc++ -L../lib/libZRCola/lib -lZRCola -L/usr/local/lib64/oatpp-1.3.0 -loatpp
SRCS := zrcolaws.cpp

include ../include/props.mak

.PHONY: all
all: zrcolad

zrcolad: ../lib/libZRCola/lib/libZRCola.a $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

../lib/libZRCola/lib/libZRCola.a:
	$(MAKE) $(MFLAGS) -C ../lib/libZRCola/build

.PHONY: clean
clean:
	-rm -r *.{d,o} zrcolad

include ../include/targets.mak
-include $(DEPS)
