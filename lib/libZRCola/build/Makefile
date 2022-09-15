CPPFLAGS := -I../../stdex/include
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

../lib/libZRCola.a : ../src/pch.h.gch $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -r ../src/*.h.gch ../src/*.cpp.o ../lib/libZRCola.a

include ../../../include/targets.mak
-include $(DEPS)
