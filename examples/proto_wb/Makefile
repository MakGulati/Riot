APPLICATION = proto_wb
# name of the application

# If no BOARD is found in the environment, use this default:
# BOARD ?= arduino-nano-33-ble
BOARD ?= native
# BOARD ?= nrf52840dk

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1
# Features required
FEATURES_REQUIRED += cpp # basic C++ support
FEATURES_REQUIRED += libstdcpp # libstdc++ support (for #include <cstdio>)
USEMODULE += cpp_new_delete
USEMODULE += printf_float
USEPKG += nanopb
# SRC += simple.c
# SRC += pb_encode.c pb_decode.c pb_common.c 
CFLAGS += -DTHREAD_STACKSIZE_MAIN=\(6*THREAD_STACKSIZE_DEFAULT\)
# BLOBS+=iris_data/iris_aug_test.csv iris_data/iris_aug_train.csv

# Add include path for Eigen library
CXXINCLUDES += -I/usr/include/eigen3
INCLUDES += -I$(PKGDIRBASE)/nanopb
include $(RIOTBASE)/Makefile.include
