CC := g++
SRCDIR := src
BUILDDIR := build
EXECUTABLES := bin/sampa2vec bin/findqta bin/linkqta bin/plotqta
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11 -O3
LIB := -L lib
INC := -I include

all: ${EXECUTABLES}

bin/sampa2vec: $(BUILDDIR)/sampa2vec.o $(BUILDDIR)/Word.o $(BUILDDIR)/Syllable.o $(BUILDDIR)/Vowel.o $(BUILDDIR)/Consonant.o $(BUILDDIR)/StringOps.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm -lnlopt

bin/findqta: $(BUILDDIR)/findqta.o $(BUILDDIR)/ApproximationSystem.o $(BUILDDIR)/PraatFileIO.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm -lnlopt

bin/linkqta: $(BUILDDIR)/linkqta.o $(BUILDDIR)/FileAnalyzer.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

bin/plotqta: $(BUILDDIR)/plotqta.o $(BUILDDIR)/PlotFileGenerator.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) ${EXECUTABLES}"; $(RM) -r $(BUILDDIR) ${EXECUTABLES}

test:
	@echo " Testing...";

.PHONY: clean test
