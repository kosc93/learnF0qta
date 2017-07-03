CC := g++
SRCDIR := src
BINDIR := bin
BUILDDIR := build
EXECUTABLES := $(BINDIR)/sampa2vec $(BINDIR)/findqta $(BINDIR)/linkqta $(BINDIR)/plotqta $(BINDIR)/trainsvm $(BINDIR)/predictqta
 
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c++11
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

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(BINDIR)"; $(RM) -r $(BUILDDIR) $(BINDIR)
	@echo " $(RM) -r test/plots test/resynthesis test/svm ... "; $(RM) -r test/plots test/resynthesis test/svm test/data/FEATURES.csv test/data/TARGETS.csv test/data/TRAINING.csv test/output.txt test/data/corpus.data test/data/corpus.test test/data/corpus.training

test: all
	@echo " Testing...";
	bash ./learnF0qta.sh test/config.xml

.PHONY: clean test
