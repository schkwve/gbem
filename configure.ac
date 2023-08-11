AC_PREREQ([2.69])
AC_INIT([GbEM], [0.1], [schkwve@gmail.com])
AC_CONFIG_AUX_DIR(build-aux)
AM_INIT_AUTOMAKE([foreign])

AC_PROG_MAKE_SET
AC_PROG_CC
AC_LANG(C)

CFLAGS="-O2 -g3 -Wall -Wextra -Werror -pedantic -std=c99"

AC_CHECK_LIB([SDL2], [SDL_Init], [], [
			echo "The SDL2 library is required for this program."
			exit -1])

AC_CHECK_LIB([SDL2_ttf], [TTF_Init], [], [
			echo "The SDL2_ttf library is required for this program."
			exit -1])

AC_CONFIG_MACRO_DIR([m4])

LT_PREREQ([2.4.7])
LT_INIT([dlopen shared])

AC_CONFIG_FILES([Makefile inc/Makefile lib/Makefile src/Makefile])

AC_OUTPUT