# Makefile que facilmente se adapta a qualquer tipo de exercicio
# author Vitor Carreira
# date 2009-09-23
#

# Bibliotecas a incluir
LIBS=#-pthread 

# Flags para o compilador
CFLAGS=-Wall -W -g -Wmissing-prototypes

# Flags para a indentacao de codigo 
IFLAGS=-br -brs -npsl -ce -cli4


# nome do executavel 
SERVIDOR=servidor

# Nome do ficheiro de opcoes do gengetopt (caso exista)
SERVIDOR_OPT=server_opt

# Lista de objectos necessarios para criar o executavel
SERVIDOR_OBJS=servidor.o ${SERVIDOR_OPT}.o  #semaforos.o debug.o

# nome do executavel 
CLIENTE=cliente

# Nome do ficheiro de opcoes do gengetopt (caso exista)
CLIENTE_OPT=client_opt

# Lista de objectos necessarios para criar o executavel
CLIENTE_OBJS=cliente.o ${CLIENTE_OPT}.o  #semaforos.o debug.o

.PHONY: clean

all: ${SERVIDOR} ${CLIENTE}

# compilar com depuracao
depuracao: CFLAGS += -D SHOW_DEBUG 
depuracao: ${SERVIDOR} ${CLIENTE}

${SERVIDOR}: ${SERVIDOR_OBJS}
	${CC} -o $@ ${SERVIDOR_OBJS} ${LIBS}

${CLIENTE}: ${CLIENTE_OBJS}
	${CC} -o $@ ${CLIENTE_OBJS} ${LIBS}

# Dependencias 
servidor.o: servidor.c comum.h ${SERVIDOR_OPT}.h #semaforos.h debug.h
${SERVIDOR_OPT}.o: ${SERVIDOR_OPT}.c ${SERVIDOR_OPT}.h

cliente.o: cliente.c comum.h ${CLIENTE_OPT}.h #semaforos.h debug.h
${CLIENTE_OPT}.o: ${CLIENTE_OPT}.c ${CLIENTE_OPT}.h

#debug.o: debug.c debug.h
semaforos.o: semaforos.c semaforos.h

#como compilar .o a partir de .c
.c.o:
	${CC} ${CFLAGS} -c $<

# Como gerar os ficheiros do gengetopt 
${SERVIDOR_OPT}.h: ${SERVIDOR_OPT}.ggo
	gengetopt < ${SERVIDOR_OPT}.ggo --file-name=${SERVIDOR_OPT}

${CLIENTE_OPT}.h: ${CLIENTE_OPT}.ggo
	gengetopt < ${CLIENTE_OPT}.ggo --file-name=${CLIENTE_OPT}

clean:
	rm -f *.o core.* *~ ${SERVIDOR} ${CLIENTE} *.bak ${SERVIDOR_OPT}.h ${SERVIDOR_OPT}.c ${CLIENTE_OPT}.h ${CLIENTE_OPT}.c

docs: Doxyfile
	doxygen Doxyfile

Doxyfile:
	doxygen -g Doxyfile

indent:
	dos2unix *.c *.h
	indent ${IFLAGS} *.c *.h
