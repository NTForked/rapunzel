# Package name, version and support mail
AC_INIT(Rapunzel, 1.0, andrei.pavel@cti.pub.ro)

# Pass foreign parameter to all Makefile.am in the tree.
AM_INIT_AUTOMAKE(foreign)

# Make sure autoconf is version 2.69 or higher.
AC_PREREQ([2.69])

# Enable libtool.
LT_INIT()

# Make sure libtool is version 2.4.6 or higher.
LT_PREREQ([2.4.6])

# Use C++ language.
AC_LANG([C++])

# File for preprocessor defines
AC_CONFIG_HEADERS([config.h])

# m4 macros directory
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX([g++ c++ cc++ clang++ nvcc])

# Convert Makefile.in to Makefile
AC_CONFIG_FILES([Makefile src/Makefile])

# Finish
AC_OUTPUT
