dnl -*- m4 -*-
dnl
dnl libz - zlib compression library
dnl
AC_REVISION([$Id: configure.ac,v 1.3 2003/06/05 02:14:09 rob Exp $])dnl

dnl Process this file with autoconf to produce a configure script.

AC_INIT([zlib], [1.1.4], [], [libz])
AC_PREREQ(2.53)
AC_CONFIG_SRCDIR([zlib.h])

AM_INIT_AUTOMAKE

dnl System type.

AC_CANONICAL_HOST

dnl Checks for programs.

AC_PROG_CC

dnl Compiler options.

if test "$GCC" = yes
then
    case "$host" in
	*-*-mingw*)
	    case "$build" in
		*-*-cygwin*)
		    CPPFLAGS="$CPPFLAGS -mno-cygwin"
		    LDFLAGS="$LDFLAGS -mno-cygwin"
		    ;;
	    esac
    esac

dnl    case "$host" in
dnl	*-*-cygwin* | *-*-mingw*)
dnl	    LDFLAGS="$LDFLAGS -no-undefined -mdll"
dnl	    ;;
dnl    esac
fi

dnl Support for libtool.

dnl AC_DISABLE_SHARED
dnl AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_SUBST(LIBTOOL_DEPS)

dnl Create output files.

AC_CONFIG_FILES([Makefile msvc++/Makefile])
AC_OUTPUT
