# CONFIGURE_CUDA65
# ---------------
# brief: Configures required package CUDA 6.5.
AC_DEFUN([CONFIGURE_CUDA65],[

echo
echo "=================================="
echo "Configuring required package CUDA 6.5"
echo "=================================="

PACKAGE_SETUP_ENVIRONMENT

AC_ARG_VAR(CUDA65_DIR, [location of the CUDA 6.5 installation.])
AC_SUBST(CUDA65_DIR, $CUDA65_DIR)

AC_MSG_NOTICE([using CUDA65_DIR: ${CUDA65_DIR}])

CUDA65_INC_PATH=-I${CUDA65_DIR}/include
CUDA65_LIB_PATH=-L${CUDA65_DIR}/lib64
CUDA65_LIBRARY=-lcudart\ -lcublas\ -lcusparse

CPPFLAGS_PREPEND($CUDA65_INC_PATH)
LDFLAGS_PREPEND($CUDA65_LIB_PATH)
LIBS_PREPEND($CUDA65_LIBRARY_PATH)

AC_CHECK_HEADER([cuda_runtime.h], , AC_MSG_ERROR([could not find header file cuda_runtime.h]))

AC_MSG_CHECKING([for CUDA version])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <cuda_runtime.h>

#define CUDART_VERSION_EQ65 (CUDART_VERSION==6050)
]], [[
#if (CUDART_VERSION_EQ65)
#else
asdf
#endif
]])], [CUDART_VERSION_VALID=yes], [CUDART_VERSION_VALID=no])
AC_MSG_RESULT([${CUDART_VERSION_VALID}])
if test "$CUDART_VERSION_VALID" = no; then
  AC_MSG_ERROR([invalid CUDA version detected; please use CUDA equal to 6.5])
fi

AC_CHECK_LIB([cudart], [cudaRuntimeGetVersion], , AC_MSG_ERROR([could not find library cudart]))
AC_CHECK_LIB([cublas], [cublasGetVersion], , AC_MSG_ERROR([could not find library cublas]))
AC_CHECK_LIB([cusparse], [cusparseGetVersion], , AC_MSG_ERROR([could not find library cusparse]))

PACKAGE_CPPFLAGS_PREPEND($CUDA65_INC_PATH)
PACKAGE_LDFLAGS_PREPEND($CUDA65_LIB_PATH)
PACKAGE_LIBS_PREPEND($CUDA65_LIBRARY)

PACKAGE_RESTORE_ENVIRONMENT

echo
])
