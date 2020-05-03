#================================[[ beg-code ]]================================#



#===(user defined)=========================================#
# basename of executable, header, and all code files
NAME_BASE  = helios
# additional standard and outside libraries
LIB_STD    = -lm
# all heatherly libraries used, debug versions whenever available
LIB_MINE   = -lySTR_debug -lySORT_debug -lyREGEX_debug
# directory for production code, no trailing slash
INST_DIR   = /usr/bin



#===(post-processing)======================================#
# create a rule for...
#install_prep       :
#remove_prep        :



#===(master template)======================================#
include /usr/local/sbin/make_program



#===(post-processing)======================================#
# create a rule for...

install_post       :
	@ln --force --physical /usr/bin/$(NAME_BASE)  /usr/bin/$(NAME_BASE)_update
	@sha1sum   /usr/bin/$(NAME_BASE)_update
	@ln --force --physical /usr/bin/$(NAME_BASE)  /usr/bin/locate
	@sha1sum   /usr/bin/locate
	@ln --force --physical /usr/bin/$(NAME_BASE)  /usr/bin/updatedb
	@sha1sum   /usr/bin/updatedb
	@ln --force --physical /usr/share/man/man1/$(NAME_BASE).1.bz2  /usr/share/man/man1/locate.1.bz2
	@printf "installing manual ----------------------  /usr/share/man/man1/locate.1.bz2\n"
	@ln --force --physical /usr/share/man/man1/$(NAME_BASE).1.bz2  /usr/share/man/man1/updatedb.1.bz2
	@printf "installing manual ----------------------  /usr/share/man/man1/updatedb.1.bz2\n"

remove_post        :
	rm -f /usr/bin/$(NAME_BASE)_update
	rm -f /usr/bin/updatedb
	rm -f /usr/bin/locate



#================================[[ end-code ]]================================#
