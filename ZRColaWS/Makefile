CPPFLAGS := $(CPPFLAGS) -I../lib/libZRCola/include -I../lib/stdex/include -I../lib/oatpp-swagger/src -I../lib/oatpp/src
LDFLAGS := $(LDFLAGS) -L../lib/libZRCola/lib -L../lib/oatpp-swagger/build/src -L../lib/oatpp/build/src
LDLIBS := $(LDLIBS) -lZRCola -loatpp-swagger -loatpp -lstdc++
SRCS := zrcolaws.cpp

include ../include/props.mak

.PHONY: all
all: zrcolaws

zrcolaws: \
	../lib/oatpp/build/src/liboatpp.a \
	../lib/oatpp-swagger/build/src/liboatpp-swagger.a \
	../lib/libZRCola/lib/libZRCola.a \
	$(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

../lib/oatpp/build/src/liboatpp.a: ../lib/oatpp/build/Makefile
	$(MAKE) $(MFLAGS) -C ../lib/oatpp/build

../lib/oatpp/build/Makefile: ../lib/oatpp/CMakeLists.txt
	cmake -D OATPP_INSTALL=OFF -D OATPP_BUILD_TESTS=OFF -D OATPP_LINK_TEST_LIBRARY=OFF -S ../lib/oatpp -B ../lib/oatpp/build

../lib/oatpp-swagger/build/src/liboatpp-swagger.a: ../lib/oatpp-swagger/build/Makefile
	$(MAKE) $(MFLAGS) -C ../lib/oatpp-swagger/build

../lib/oatpp-swagger/build/Makefile: ../lib/oatpp-swagger/CMakeLists.txt
	cmake -D OATPP_INSTALL=OFF -D OATPP_BUILD_TESTS=OFF -D OATPP_MODULES_LOCATION=CUSTOM -D OATPP_DIR_SRC=${CURDIR}/../lib/oatpp/src -D OATPP_DIR_LIB=${CURDIR}/../lib/oatpp/build/src -S ../lib/oatpp-swagger -B ../lib/oatpp-swagger/build

../lib/libZRCola/lib/libZRCola.a:
	$(MAKE) $(MFLAGS) -C ../lib/libZRCola/build

.PHONY: install
install: zrcolaws ../output/data/ZRCola.zrcdb
	install -d $(PREFIX)/bin/
	install -m 755 zrcolaws $(PREFIX)/bin/
	install -d $(PREFIX)/share/zrcola/
	install -m 644 ../output/data/ZRCola.zrcdb $(PREFIX)/share/zrcola/
	install -d $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/favicon-16x16.png $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/favicon-32x32.png $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/index.html $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/oauth2-redirect.html $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-bundle.js $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-bundle.js.map $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-es-bundle-core.js $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-es-bundle-core.js.map $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-es-bundle.js $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-es-bundle.js.map $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-standalone-preset.js $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui-standalone-preset.js.map $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui.css $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui.css.map $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui.js $(PREFIX)/share/zrcola/res/
	install -m 644 ../lib/oatpp-swagger/res/swagger-ui.js.map $(PREFIX)/share/zrcola/res/
	install -d $(PREFIX)/etc/sysconfig/
	{	echo '#!/bin/sh'; \
		echo ''; \
		echo '# Interface to listen on. Default: localhost'; \
		echo '#HOST=0.0.0.0'; \
		echo ''; \
		echo '# Port to listen on. Default: 54591'; \
		echo '#PORT=54591'; \
		echo ''; \
		echo '# Additional zrcolaws command line options'; \
		echo '#OPTIONS=-4'; \
	} > $(PREFIX)/etc/sysconfig/zrcolaws
	install -d $(PREFIX)/lib/systemd/system/
	{	echo '[Unit]'; \
		echo 'Description=ZRCola Web Service'; \
		echo 'After=network.target'; \
		echo ''; \
		echo '[Service]'; \
		echo 'Environment="HOST=localhost" "PORT=54591"'; \
		echo 'EnvironmentFile=-$(PREFIX)/etc/sysconfig/zrcolaws'; \
		echo 'DynamicUser=yes'; \
		echo 'ExecStart=/usr/local/bin/zrcolaws --host $$HOST --port $$PORT $$OPTIONS'; \
		echo 'Type=exec'; \
		echo 'Restart=always'; \
		echo ''; \
		echo '[Install]'; \
		echo 'WantedBy=multi-user.target'; \
		echo 'RequiredBy=network.target'; \
	} > $(PREFIX)/lib/systemd/system/zrcolaws.service

.PHONY: clean
clean:
	-rm -r *.{d,o} zrcolaws

include ../include/targets.mak
-include $(DEPS)
