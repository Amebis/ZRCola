CPPFLAGS := $(CPPFLAGS) -I../lib/libZRCola/include -I../lib/stdex/include -I/usr/local/include/oatpp-1.3.0/oatpp
LDFLAGS := $(LDFLAGS) -L../lib/libZRCola/lib -L/usr/local/lib64/oatpp-1.3.0
LDLIBS := $(LDLIBS) -lZRCola -loatpp -lstdc++
SRCS := zrcolaws.cpp

include ../include/props.mak

.PHONY: all
all: zrcolad

zrcolad: ../lib/libZRCola/lib/libZRCola.a $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

../lib/libZRCola/lib/libZRCola.a:
	$(MAKE) $(MFLAGS) -C ../lib/libZRCola/build

.PHONY: install
install: zrcolad ../output/data/ZRCola.zrcdb
	install -d $(PREFIX)/bin/
	install -m 755 zrcolad $(PREFIX)/bin/
	install -d $(PREFIX)/share/zrcola/
	install -m 644 ../output/data/ZRCola.zrcdb $(PREFIX)/share/zrcola/

.PHONY: clean
clean:
	-rm -r *.{d,o} zrcolad

include ../include/targets.mak
-include $(DEPS)
