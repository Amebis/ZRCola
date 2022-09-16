ifeq ($(PREFIX),)
PREFIX := /usr/local
endif
CPPFLAGS := $(CPPFLAGS) -MMD -MP -DPREFIX='"$(PREFIX)"'
ifeq ($(CFG),Debug)
CPPFLAGS := $(CPPFLAGS) -D_DEBUG
CFLAGS := $(CFLAGS) -Og -g
CXXFLAGS := $(CXXFLAGS) -Og -g
else
CPPFLAGS := $(CPPFLAGS) -DNDEBUG
CFLAGS := $(CFLAGS) -O3
CXXFLAGS := $(CXXFLAGS) -O3
endif

OBJS := $(SRCS:%=%.o)
DEPS := $(OBJS:.o=.d)
