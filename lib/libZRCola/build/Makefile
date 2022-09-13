CPPFLAGS := $(CPPFLAGS) -MMD -MP -I../../stdex/include
ifeq ($(CFG),Debug)
CPPFLAGS := $(CPPFLAGS) -D_DEBUG
CXXFLAGS := -Og
else
CPPFLAGS := $(CPPFLAGS) -DNDEBUG
CXXFLAGS := -O3
endif

SRCS := \
	../src/character.cpp \
	../src/common.cpp \
	../src/highlight.cpp \
	../src/language.cpp \
	../src/mapping.cpp \
	../src/pch.cpp \
	../src/tag.cpp \
	../src/translate.cpp
OBJS := $(SRCS:%=%.o)
DEPS := $(OBJS:.o=.d)

../lib/libZRCola.a : ../src/pch.h.gch $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%.h.gch: %.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++-header -o $@ -c $<

%.cpp.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	-rm -r ../src/*.h.gch ../src/*.cpp.o ../lib/libZRCola.a

-include $(DEPS)
