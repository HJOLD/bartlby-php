dnl $Id$
dnl config.m4 for extension bartlby

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(bartlby, for bartlby support,
dnl Make sure that the comment is aligned:
dnl [  --with-bartlby             Include bartlby support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(bartlby, whether to enable bartlby support,
dnl Make sure that the comment is aligned:
dnl [  --enable-bartlby           Enable bartlby support])

if test "$PHP_BARTLBY" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-bartlby -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/bartlby.h"  # you most likely want to change this
  dnl if test -r $PHP_BARTLBY/; then # path given as parameter
  dnl   BARTLBY_DIR=$PHP_BARTLBY
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for bartlby files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BARTLBY_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BARTLBY_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the bartlby distribution])
  dnl fi

  dnl # --with-bartlby -> add include path
  dnl PHP_ADD_INCLUDE($BARTLBY_DIR/include)

  dnl # --with-bartlby -> check for lib and symbol presence
  dnl LIBNAME=bartlby # you may want to change this
  dnl LIBSYMBOL=bartlby # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BARTLBY_DIR/lib, BARTLBY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BARTLBYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong bartlby lib version or lib not found])
  dnl ],[
  dnl   -L$BARTLBY_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(BARTLBY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(bartlby, bartlby.c, $ext_shared)
  
fi
