# CONFIGURE_PETSC
# ---------------
# brief: Configures required package PETSc.
AC_DEFUN([CONFIGURE_PETSC],[

echo
echo "=================================="
echo "Configuring required package PETSc"
echo "=================================="

PACKAGE_SETUP_ENVIRONMENT

AC_ARG_VAR(PETSC_DIR, [location of the PETSc installation.])
AC_ARG_VAR(PETSC_ARCH, [configuration of PETSc, located in ${PETSC_DIR}/${PETSC_ARCH}.])
AC_SUBST(PETSC_DIR, $PETSC_DIR)
AC_SUBST(PETSC_ARCH, $PETSC_ARCH)

PETSC_CONF_PATH=`find ${PETSC_DIR}/${PETSC_ARCH} -name conf`
AC_SUBST(PETSC_CONF_PATH)
PETSC_VARIABLES=`find ${PETSC_DIR}/ -name variables`
AC_SUBST(PETSC_VARIABLES)

AC_MSG_NOTICE([using PETSC_DIR: ${PETSC_DIR}])
AC_MSG_NOTICE([using PETSC_ARCH: ${PETSC_ARCH}])
AC_MSG_NOTICE([using PETSC_CONF_PATH: ${PETSC_CONF_PATH}])

PETSC_CC_INCLUDES=`grep "PETSC_CC_INCLUDES =" ${PETSC_CONF_PATH}/petscvariables | sed -e 's/.*=//' -e 's/^[ \t]*//'`
PETSC_EXTERNAL_LIB_BASIC=`grep "PETSC_EXTERNAL_LIB_BASIC =" ${PETSC_CONF_PATH}/petscvariables | sed -e 's/.*=//' -e 's/^[ \t]*//'`
PETSC_WITH_EXTERNAL_LIB=`grep "PETSC_WITH_EXTERNAL_LIB =" ${PETSC_CONF_PATH}/petscvariables | sed -e 's/.*=//' -e 's/^[ \t]*//'`

CPPFLAGS_PREPEND($PETSC_CC_INCLUDES)

AC_CHECK_HEADER([petsc.h], , AC_MSG_ERROR([could not find header file petsc.h]))

AC_MSG_CHECKING([for PETSc version])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <petscversion.h>

#ifndef PETSC_VERSION_GE
#define PETSC_VERSION_GE(MAJOR, MINOR, SUBMINOR) (!PETSC_VERSION_LT(MAJOR, MINOR, SUBMINOR))
#endif
]], [[
#if (PETSC_VERSION_GE(3, 5, 0) || !PETSC_VERSION_RELEASE)
#else
asdf
#endif
]])], [PETSC_VERSION_VALID=yes], [PETSC_VERSION_VALID=no])
AC_MSG_RESULT([${PETSC_VERSION_VALID}])
if test "$PETSC_VERSION_VALID" = no; then
  AC_MSG_ERROR([invalid PETSc version detected; please use PETSc version greater than 3.5])
fi

PACKAGE_CPPFLAGS_PREPEND($PETSC_CC_INCLUDES)
PACKAGE_LIBS_PREPEND($PETSC_WITH_EXTERNAL_LIB)

PACKAGE_RESTORE_ENVIRONMENT

echo
])
