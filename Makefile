# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
# Changelog :
#   2010-11-05 - first version
#   2011-08-10 - added structure : sources, objects, binaries
#                thanks to http://stackoverflow.com/users/128940/beta
# ------------------------------------------------

# project name (generate executable with this name)
TARGET   = basicNumeric

CC       = clang++
# compiling flags here
CFLAGS   = -std=c++11 -Wall -I.

LINKER   = $(CC) -o
# linking flags here
LFLAGS   = -Wall -I. -lm -lreadline

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
INSTALL_PREFIX = /usr/local/bin

SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f

$(BINDIR)/$(TARGET): $(OBJDIR) $(OBJECTS)
	$(LINKER) $@ $(LFLAGS) $(OBJECTS)
	@echo "Linking complete!"

$(OBJDIR):
	@echo "Creating output dir"
	mkdir $(OBJDIR) $(BINDIR)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

.PHONY: install
install:
	install -m755 -D bin/$(TARGET) $(INSTALL_PREFIX)/$(TARGET)

.PHONY: uninstall
uninstall:
	rm $(INSTALL_PREFIX)/$(TARGET)
