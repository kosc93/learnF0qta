CC := g++
SRCDIR := src
BUILDDIR := build
EXECUTABLES := sampa2vec findqta linkqta plotqta
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11 -O3
LIB := -L lib
INC := -I include

all: ${EXECUTABLES}

sampa2vec: $(BUILDDIR)/sampa2vec.o $(BUILDDIR)/Word.o $(BUILDDIR)/Syllable.o $(BUILDDIR)/Vowel.o $(BUILDDIR)/Consonant.o $(BUILDDIR)/StringOps.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o bin/$@ $(LIB)"; $(CC) $^ -o bin/$@ $(LIB) -lm -lnlopt

findqta: $(BUILDDIR)/findqta.o $(BUILDDIR)/ApproximationSystem.o $(BUILDDIR)/PraatFileIO.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o bin/$@ $(LIB)"; $(CC) $^ -o bin/$@ $(LIB) -lm -lnlopt

linkqta: $(BUILDDIR)/linkqta.o $(BUILDDIR)/FileAnalyzer.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o bin/$@ $(LIB)"; $(CC) $^ -o bin/$@ $(LIB)

plotqta: $(BUILDDIR)/plotqta.o $(BUILDDIR)/PlotFileGenerator.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o bin/$@ $(LIB)"; $(CC) $^ -o bin/$@ $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(foreach BIN,$(EXECUTABLES),bin/$(BIN))"; $(RM) -r $(BUILDDIR) $(foreach BIN,$(EXECUTABLES),bin/$(BIN))

test: $(TARGET)
	@echo " Testing...";

.PHONY: clean test
