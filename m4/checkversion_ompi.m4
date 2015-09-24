# CHECKVERSION_OPENMPI
# ---------------
# brief: Check whether the version of OpenMPI is 1.8
AC_DEFUN([CHECKVERSION_OPENMPI],[

echo
echo "=================================="
echo "Check whether the version of OpenMPI is 1.8"
echo "=================================="

PACKAGE_SETUP_ENVIRONMENT

AC_CHECK_HEADER([mpi.h], , AC_MSG_ERROR([could not find header file mpi.h]))

AC_MSG_CHECKING([for OpenMPI])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <mpi.h>

]], [[
#ifdef OMPI_MPI_H
#else
asdf
#endif
]])], [IS_OMPI=yes], [IS_OMPI=no])
AC_MSG_RESULT([${IS_OMPI}])
if test "$IS_OMPI" = no; then
  AC_MSG_ERROR([Must used OpenMPI, because AmgX only accept OpenMPI])
fi

AC_MSG_CHECKING([for version of OpenMPI])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <mpi.h>

# define CHECK (OMPI_MAJOR_VERSION == 1 && OMPI_MINOR_VERSION == 8)
]], [[
#if (CHECK)
#else
asdf
#endif
]])], [OMPI_VERSION=yes], [OMPI_VERSION=no])
AC_MSG_RESULT([${OMPI_VERSION}])
if test "$OMPI_VERSION" = no; then
  AC_MSG_ERROR([OpenMPI is not 1.8!! AmgX currently only accept OpenMPI 1.8.])
fi

PACKAGE_RESTORE_ENVIRONMENT

echo
])
