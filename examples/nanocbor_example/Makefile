APPLICATION = nanocbor_example
# name of the application

# If no BOARD is found in the environment, use this default:
# BOARD ?= arduino-nano-33-ble
BOARD ?= nrf52840dk 


# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1
# Features required
# FEATURES_REQUIRED += cpp # basic C++ support
# FEATURES_REQUIRED += libstdcpp # libstdc++ support (for #include <cstdio>)
# USEMODULE += cpp_new_delete
USEPKG += nanocbor
USEMODULE += printf_float
CFLAGS += -DTHREAD_STACKSIZE_MAIN=\(6*THREAD_STACKSIZE_DEFAULT\)


include $(RIOTBASE)/Makefile.include
