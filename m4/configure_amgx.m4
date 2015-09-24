# CONFIGURE_AMGX
# ---------------
# brief: Configures required package AmgX.
AC_DEFUN([CONFIGURE_AMGX],[

echo
echo "=================================="
echo "Configuring required package AmgX"
echo "=================================="

PACKAGE_SETUP_ENVIRONMENT

AC_ARG_VAR(AMGX_DIR, [location of the AmgX installation.])
AC_SUBST(AMGX_DIR, $AMGX_DIR)

AC_MSG_NOTICE([using AMGX_DIR: ${AMGX_DIR}])

AMGX_INC_PATH=-I`find ${AMGX_DIR} -name amgx_c.h | sed -e 's/\/amgx_c.h//g'`
AMGX_LIB_PATH=-L`find ${AMGX_DIR} -name libamgxsh.so | sed -e 's/\/libamgxsh.so//g'`
AMGX_LIBRARY=-lamgxsh

CPPFLAGS_PREPEND($AMGX_INC_PATH)
LDFLAGS_PREPEND($AMGX_LIB_PATH)
LIBS_PREPEND($AMGX_LIBRARY)

AC_CHECK_HEADER([amgx_c.h], , AC_MSG_ERROR([could not find header file amgx_c.h]))
AC_CHECK_LIB([amgxsh], [AMGX_initialize], , AC_MSG_ERROR([could not find library amgxsh]))

PACKAGE_CPPFLAGS_PREPEND($AMGX_INC_PATH)
PACKAGE_LDFLAGS_PREPEND($AMGX_LIB_PATH)
PACKAGE_LIBS_PREPEND($AMGX_LIBRARY)

PACKAGE_RESTORE_ENVIRONMENT

echo
])
