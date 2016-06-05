#*===============================[[ beg-code ]]===============================*#

#*---(base name)----------------------*#
BASE    = helios
DEBUG   = helios_debug



#*---(standard variables)-------------*#
# must have "-x c" on gcc line so stripped files work with alternate extensions
COMP    = gcc -c -std=gnu89 -x c -g -pg -Wall -Wextra
INCS    = -I/usr/local/include 
LINK    = gcc 
LIBS    = -L/usr/local/libs -lySTR  -lm
LIBD    = -L/usr/local/libs -lySTR  -lm -lyLOG
OBJS    = ${BASE}_main.os ${BASE}_prog.os ${BASE}_entry.os ${BASE}_rptg.os ${BASE}_sort.os ${BASE}_file.os ${BASE}_drive.os
OBJD    = ${BASE}_main.o  ${BASE}_prog.o  ${BASE}_entry.o  ${BASE}_rptg.o  ${BASE}_sort.o  ${BASE}_file.o  ${BASE}_drive.o



#*---(make variables)-----------------*#
COPY    = cp -f  
CLEAN   = rm -f
PRINTF  = @printf
STRIP   = @grep -v -e " DEBUG_" -e " yLOG_" 



#*---(MAIN)---------------------------*#
all                : ${BASE}



#*---(executables)--------------------*#
${BASE}            : ${OBJD}
	${LINK}  -o ${BASE}        ${OBJS}   ${LIBS}
	${LINK}  -o ${DEBUG}       ${OBJD}   ${LIBD}



#*---(normal objects)-----------------*#
${BASE}_main.o     : ${BASE}.h    ${BASE}_main.c
	${COMP}    ${BASE}_main.c                        ${INC}
	${STRIP}   ${BASE}_main.c    > ${BASE}_main.cs
	${COMP}    ${BASE}_main.cs  -o ${BASE}_main.os   ${INC}

${BASE}_prog.o     : ${BASE}.h    ${BASE}_prog.c
	${COMP}    ${BASE}_prog.c                        ${INC}
	${STRIP}   ${BASE}_prog.c    > ${BASE}_prog.cs
	${COMP}    ${BASE}_prog.cs  -o ${BASE}_prog.os   ${INC}

${BASE}_entry.o    : ${BASE}.h    ${BASE}_entry.c
	${COMP}    ${BASE}_entry.c                       ${INC}
	${STRIP}   ${BASE}_entry.c   > ${BASE}_entry.cs
	${COMP}    ${BASE}_entry.cs -o ${BASE}_entry.os  ${INC}

${BASE}_sort.o     : ${BASE}.h    ${BASE}_sort.c
	${COMP}    ${BASE}_sort.c                        ${INC}
	${STRIP}   ${BASE}_sort.c    > ${BASE}_sort.cs
	${COMP}    ${BASE}_sort.cs  -o ${BASE}_sort.os   ${INC}

${BASE}_rptg.o     : ${BASE}.h    ${BASE}_rptg.c
	${COMP}    ${BASE}_rptg.c                        ${INC}
	${STRIP}   ${BASE}_rptg.c    > ${BASE}_rptg.cs
	${COMP}    ${BASE}_rptg.cs  -o ${BASE}_rptg.os   ${INC}

${BASE}_file.o     : ${BASE}.h    ${BASE}_file.c
	${COMP}    ${BASE}_file.c                        ${INC}
	${STRIP}   ${BASE}_file.c    > ${BASE}_file.cs
	${COMP}    ${BASE}_file.cs  -o ${BASE}_file.os   ${INC}

${BASE}_drive.o    : ${BASE}.h    ${BASE}_drive.c
	${COMP}    ${BASE}_drive.c                       ${INC}
	${STRIP}   ${BASE}_drive.c   > ${BASE}_drive.cs
	${COMP}    ${BASE}_drive.cs -o ${BASE}_drive.os  ${INC}



#*---(housekeeping)-------------------*#
bigclean           :
	${CLEAN} .*.swp

clean              :
	${CLEAN} ${BASE}
	${CLEAN} ${BASE}*.o
	${CLEAN} ${BASE}*.cs
	${CLEAN} ${BASE}*.os
	${CLEAN} ${BASE}*.Sc
	${CLEAN} ${BASE}*.So
	${CLEAN} *~
	${CLEAN} temp*
	${CLEAN} ${BASE}_unit
	${CLEAN} ${BASE}_unit.c
	${CLEAN} ${DEBUG}

remove             : ${BASE}
	#---(executable)----------------------#
	rm -f /usr/bin/${BASE}
	rm -f /usr/local/bin/${DEBUG}
	rm -f /usr/bin/${BASE}_update
	rm -f /usr/bin/updatedb
	rm -f /usr/bin/locate

install            : ${BASE}
	#---(executable)----------------------#
	${COPY}  ${BASE}         /usr/bin/
	chmod    0755           /usr/bin/${BASE}
	sha1sum  ${BASE}
	#---(debug version)-------------------#
	${COPY}  ${DEBUG}        /usr/local/bin/
	chmod    0755           /usr/local/bin/${DEBUG}
	#---(provide other links)-------------#
	ln --force --physical /usr/bin/${BASE}  /usr/bin/helios_update
	ln --force --physical /usr/bin/${BASE}  /usr/bin/locate
	ln --force --physical /usr/bin/${BASE}  /usr/bin/updatedb
	#---(documentation)-------------------#
	rm -f       /usr/share/man/man1/${BASE}.1.bz2
	cp -f       ${BASE}.1    /usr/share/man/man1/${BASE}.1
	bzip2       /usr/share/man/man1/${BASE}.1
	chmod 0644  /usr/share/man/man1/${BASE}.1.bz2
	#---(provide other links)-------------#
	ln --force --physical /usr/share/man/man1/${BASE}.1.bz2  /usr/share/man/man1/${DEBUG}.1.bz2
	ln --force --physical /usr/share/man/man1/${BASE}.1.bz2  /usr/share/man/man1/locate.1.bz2
	ln --force --physical /usr/share/man/man1/${BASE}.1.bz2  /usr/share/man/man1/updatedb.1.bz2


#*===============================[[ end-code ]]===============================*#
