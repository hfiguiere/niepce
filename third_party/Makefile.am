

GTEST_FILES = googletest/README.md \
googletest/googletest/include/gtest/gtest.h \
googletest/googletest/include/gtest/gtest-printers.h \
googletest/googletest/include/gtest/gtest_pred_impl.h \
googletest/googletest/include/gtest/gtest-death-test.h \
googletest/googletest/include/gtest/gtest-typed-test.h \
googletest/googletest/include/gtest/gtest_prod.h \
googletest/googletest/include/gtest/gtest-spi.h \
googletest/googletest/include/gtest/gtest-param-test.h \
googletest/googletest/include/gtest/gtest-message.h \
googletest/googletest/include/gtest/gtest-param-test.h.pump \
googletest/googletest/include/gtest/gtest-test-part.h \
googletest/googletest/include/gtest/internal/gtest-death-test-internal.h \
googletest/googletest/include/gtest/internal/gtest-param-util-generated.h \
googletest/googletest/include/gtest/internal/gtest-tuple.h \
googletest/googletest/include/gtest/internal/gtest-filepath.h \
googletest/googletest/include/gtest/internal/gtest-type-util.h \
googletest/googletest/include/gtest/internal/custom/gtest.h \
googletest/googletest/include/gtest/internal/custom/gtest-printers.h \
googletest/googletest/include/gtest/internal/custom/gtest-port.h \
googletest/googletest/include/gtest/internal/gtest-param-util.h \
googletest/googletest/include/gtest/internal/gtest-param-util-generated.h.pump \
googletest/googletest/include/gtest/internal/gtest-port.h \
googletest/googletest/include/gtest/internal/gtest-string.h \
googletest/googletest/include/gtest/internal/gtest-type-util.h.pump \
googletest/googletest/include/gtest/internal/gtest-port-arch.h \
googletest/googletest/include/gtest/internal/gtest-tuple.h.pump \
googletest/googletest/include/gtest/internal/gtest-internal.h \
googletest/googletest/include/gtest/internal/gtest-linked_ptr.h \
googletest/googletest/src/gtest-printers.cc \
googletest/googletest/src/gtest.cc \
googletest/googletest/src/gtest-death-test.cc \
googletest/googletest/src/gtest_main.cc \
googletest/googletest/src/gtest-test-part.cc \
googletest/googletest/src/gtest-typed-test.cc \
googletest/googletest/src/gtest-internal-inl.h \
googletest/googletest/src/gtest-filepath.cc \
googletest/googletest/src/gtest-port.cc \
googletest/googletest/src/gtest-all.cc \
googletest/googletest/README.md \
googletest/googletest/CONTRIBUTORS \
googletest/googletest/CHANGES \
googletest/googletest/Makefile.am \
googletest/googletest/LICENSE

EXTRA_DIST = $(GTEST_FILES)

noinst_LIBRARIES = libgtest.a

libgtest_a_SOURCES = googletest/googletest/src/gtest-all.cc \
	googletest/googletest/src/gtest_main.cc
libgtest_a_CPPFLAGS = -I@top_srcdir@/@GTEST_ROOT@/include -I@top_srcdir@/@GTEST_ROOT@
