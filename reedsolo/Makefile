#Can be changed to C++
CC=cc 
#Compilation flags to build object files
CFLAGS = -c -g -Isrc/ -Wall -Wextra -Wno-deprecated-declarations
#Linker flags when linking to build executables
#To use profiler, you have to include -Wl,-no_pie to turn off adress randomization
LFLAGS = -Wl,-no_pie -g -Wall -Wextra -lcrypto -lprofiler -Isrc/ -L/opt/local/lib/
#Sets debug level
DEBUGFLAGS = -DDEBUG4
#Local libs for linking
LIBS = ./gf_complete.a

#Destination for binaries, and source folder
OUT = bin
SRC = ./

##### BUILD LIST #####

ALL_OBJS = #

TEST_EXECS = #

FINAL_EXECS = vmondmatrix_test

##### BUILD LIST #####

#List of object files prefixed with output folder
ALL_DEPS = $(addprefix $(OUT)/, $(ALL_OBJS))
ALL_TEST = $(addprefix $(OUT)/, $(TEST_EXECS))  
ALL_EXES = $(addprefix $(OUT)/, $(FINAL_EXECS)) 

all:	bin $(ALL_DEPS) $(ALL_EXES)
test:	bin $(ALL_TEST)

bin:
	mkdir -p $(OUT)
	mkdir -p $(OUT)/test
	@echo

##### BUILD TARGETS ######

#Template:
#$(OUT)/mysrc.o:	$(SRC)/mysrc.c
#	$(CC) $(CFLAGS) $< -o $@


##### BUILD TARGETS ######

#These are some handy default cases, for things not specified above.
#If they don't work you may need to add an explicit target above.
$(OUT)/%.o:	$(SRC)/%.c
	@echo "Default target for building $<"
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $< -o $@
	@echo

$(OUT)/%:	$(SRC)/%.c $(ALL_DEPS)
	@echo "Default target for building/linking $@"
	$(CC) $(LFLAGS) $(DEBUGFLAGS) $(LIBS) $^ -o $@
	@echo

clean:
	rm -rf $(OUT)
