CC := g++
SRCDIR := src
BINDIR := bin
BUILDDIR := build
EXECUTABLES := $(BINDIR)/sampa2vec $(BINDIR)/findqta $(BINDIR)/linkqta $(BINDIR)/plotqta $(BINDIR)/trainsvm $(BINDIR)/predictqta $(BINDIR)/initf0stat $(BINDIR)/validatesvm
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11 -fopenmp
LIB := -L lib
INC := -I include

all: ${EXECUTABLES}

$(BINDIR)/sampa2vec: $(BUILDDIR)/_sampa2vec.o $(BUILDDIR)/Word.o $(BUILDDIR)/Syllable.o $(BUILDDIR)/Vowel.o $(BUILDDIR)/Consonant.o $(BUILDDIR)/utilities.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm -lnlopt

$(BINDIR)/findqta: $(BUILDDIR)/_findqta.o $(BUILDDIR)/SystemTA.o $(BUILDDIR)/PraatFileIO.o $(BUILDDIR)/utilities.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm -lnlopt

$(BINDIR)/linkqta: $(BUILDDIR)/_linkqta.o $(BUILDDIR)/TrainingFileGenerator.o $(BUILDDIR)/utilities.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

$(BINDIR)/plotqta: $(BUILDDIR)/_plotqta.o $(BUILDDIR)/PlotFileGenerator.o $(BUILDDIR)/utilities.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

$(BINDIR)/trainsvm: $(BUILDDIR)/_trainsvm.o $(BUILDDIR)/SVMTrainer.o $(BUILDDIR)/utilities.o $(BUILDDIR)/svm.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

$(BINDIR)/predictqta: $(BUILDDIR)/_predictqta.o $(BUILDDIR)/SystemTA.o $(BUILDDIR)/PraatFileIO.o $(BUILDDIR)/utilities.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm

$(BINDIR)/initf0stat: $(BUILDDIR)/_initf0stat.o $(BUILDDIR)/utilities.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

$(BINDIR)/validatesvm: $(BUILDDIR)/_validatesvm.o $(BUILDDIR)/SVMValidater.o $(BUILDDIR)/utilities.o $(BUILDDIR)/svm.o
	@echo " Linking" $@ "... "
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB) -lm -fopenmp

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
