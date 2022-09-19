CPPFLAGS := $(CPPFLAGS) -I../lib/libZRCola/include -I../lib/stdex/include -I/usr/local/include/oatpp-1.3.0/oatpp -I/usr/local/include/oatpp-1.3.0/oatpp-swagger
LDFLAGS := $(LDFLAGS) -L../lib/libZRCola/lib -L/usr/local/lib64/oatpp-1.3.0
LDLIBS := $(LDLIBS) -lZRCola -loatpp-swagger -loatpp -lstdc++
SRCS := zrcolaws.cpp

include ../include/props.mak

.PHONY: all
all: zrcolaws

zrcolaws: ../lib/libZRCola/lib/libZRCola.a $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

../lib/libZRCola/lib/libZRCola.a:
	$(MAKE) $(MFLAGS) -C ../lib/libZRCola/build

.PHONY: install
install: zrcolaws ../output/data/ZRCola.zrcdb
	install -d $(PREFIX)/bin/
	install -m 755 zrcolaws $(PREFIX)/bin/
	install -d $(PREFIX)/share/zrcola/
	install -m 644 ../output/data/ZRCola.zrcdb $(PREFIX)/share/zrcola/

.PHONY: clean
clean:
	-rm -r *.{d,o} zrcolaws

include ../include/targets.mak
-include $(DEPS)
