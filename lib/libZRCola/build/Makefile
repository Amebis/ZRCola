CPPFLAGS := $(CPPFLAGS) -I../../stdex/include
SRCS := \
	../src/character.cpp \
	../src/common.cpp \
	../src/highlight.cpp \
	../src/language.cpp \
	../src/mapping.cpp \
	../src/pch.cpp \
	../src/tag.cpp \
	../src/translate.cpp

include ../../../include/props.mak

.PHONY: all
all: ../lib/libZRCola.a

../lib/libZRCola.a: ../src/pch.h.gch $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: test
test: ../test/test
	../test/test

../test/test: ../lib/libZRCola.a
	$(CXX) $(CPPFLAGS) -I../../stdex/include -I../include $(CXXFLAGS) -L../lib -o $@ ../test/test.cpp -lstdc++ -lZRCola

.PHONY: clean
clean:
	-rm -r ../src/*.{d,gch,o} ../lib/libZRCola.a ../test/*.d ../test/test

include ../../../include/targets.mak
-include $(DEPS)
-include ../test/test.d
