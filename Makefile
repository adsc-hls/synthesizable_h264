###
###     Makefile for H.26L encoder
###
###             generated for UNIX/LINUX environments
###             by H. Schwarz
###



NAME=   ldecod

### include debug information: 1=yes, 0=no
#DBG= 0

DEPEND= dependencies

BINDIR= bin
INCDIR= tpdn_opt
SRCDIR= tpdn_opt
OBJDIR= obj


CC=clang

LIBS=   -lm
FLAGS=  #-ffloat-store -Wall -I$(INCDIR)


FLAGS+= -g


OBJSUF= .o$(SUFFIX)

SRC=    $(wildcard $(SRCDIR)/*.c) 
ADDSRC= $(wildcard $(ADDSRCDIR)/*.c)
OBJ=    $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o$(SUFFIX)) $(ADDSRC:$(ADDSRCDIR)/%.c=$(OBJDIR)/%.o$(SUFFIX)) 
BIN=    $(BINDIR)/$(NAME)$(SUFFIX)


default: depend bin 

dependencies:
	@echo "" >dependencies

clean:
	@echo remove bin/ and obj/
	@rm -rf $(OBJDIR) $(BINDIR) *.yuv
	@rm -rf $(OBJDIR) $(BINDIR) testresult.yuv


bin:    $(OBJ)
	@echo
	@echo 'creating binary "$(BIN)"'
	@$(CC) -o $(BIN) $(OBJ) $(LIBS)
	@echo '... done'
	@echo

depend:
	@echo
	@echo 'checking dependencies'
	@$(SHELL) -ec '$(CC) -MM $(CFLAGS) -I$(INCDIR) -I$(ADDINCDIR) $(SRC) $(ADDSRC)                  \
         | sed '\''s@\(.*\)\.o[ :]@$(OBJDIR)/\1.o$(SUFFIX):@g'\''               \
         >$(DEPEND)'
	@echo

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir -p obj bin

$(OBJDIR)/%.o$(SUFFIX): $(SRCDIR)/%.c
	@echo 'compiling object file "$@" ...'
	@$(CC) -c -o $@ $(FLAGS) $<

$(OBJDIR)/%.o$(SUFFIX): $(ADDSRCDIR)/%.c
	@echo 'compiling object file "$@" ...'
	@$(CC) -c -o $@ $(FLAGS) $<


include $(DEPEND)

