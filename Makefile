CC := g++
SRCDIR := src
BINDIR := bin
BUILDDIR := build
EXECUTABLES := $(BINDIR)/sampa2vec $(BINDIR)/findqta $(BINDIR)/linkqta $(BINDIR)/plotqta
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11 -O3
LIB := -L lib
INC := -I include

all: ${EXECUTABLES}

$(BINDIR)/sampa2vec: $(BUILDDIR)/sampa2vec.o $(BUILDDIR)/Word.o $(BUILDDIR)/Syllable.o $(BUILDDIR)/Vowel.o $(BUILDDIR)/Consonant.o $(BUILDDIR)/StringOps.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm -lnlopt

$(BINDIR)/findqta: $(BUILDDIR)/findqta.o $(BUILDDIR)/ApproximationSystem.o $(BUILDDIR)/PraatFileIO.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm -lnlopt

$(BINDIR)/linkqta: $(BUILDDIR)/linkqta.o $(BUILDDIR)/FileAnalyzer.o $(BUILDDIR)/StringOps.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

$(BINDIR)/plotqta: $(BUILDDIR)/plotqta.o $(BUILDDIR)/PlotFileGenerator.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR)"; $(RM) -r $(BUILDDIR) $(BINDIR)

test: all
	@echo " Testing...";
	bash ./learnF0qta.sh test/config.xml

.PHONY: clean test
