# This file is called by mbed-src/CMakeLists.txt when unit tests are active
# and we only want to build a stub that contains all the headers.

project(mbed-os)


# Unit test stub include dirs (list adapted from mbed-src/UNITTESTS/CMakeLists.txt)
set(unittest-includes-base
	"UNITTESTS/target_h"
	"UNITTESTS/target_h/events"
	"UNITTESTS/target_h/events/equeue"
	"UNITTESTS/target_h/platform"
	"UNITTESTS/target_h/platform/cxxsupport"
	"UNITTESTS/target_h/drivers"
	"UNITTESTS/stubs"
	"UNITTESTS/.."
	"UNITTESTS/../features"
	"UNITTESTS/../features/netsocket"
	"UNITTESTS/../platform"
	"UNITTESTS/../drivers"
	"UNITTESTS/../hal"
	"UNITTESTS/../events"
	"UNITTESTS/../events/source"
	"UNITTESTS/../events/internal"
	"UNITTESTS/../rtos"
	"UNITTESTS/../rtos/TARGET_CORTEX"
	"UNITTESTS/../rtos/TARGET_CORTEX/rtx5/Include"
	"UNITTESTS/../cmsis"
	"UNITTESTS/../features/frameworks"
	"UNITTESTS/../features/frameworks/mbed-trace"
	"UNITTESTS/../features/frameworks/nanostack-libservice"
	"UNITTESTS/../features/frameworks/nanostack-libservice/mbed-client-libservice"
	"UNITTESTS/../features/filesystem/fat"
	"UNITTESTS/../features/filesystem/fat/ChaN"
	"UNITTESTS/../features/filesystem/bd"
	"UNITTESTS/../features/filesystem/"
	"UNITTESTS/../features/filesystem/littlefs"
	"UNITTESTS/../features/filesystem/littlefs/littlefs"
	"UNITTESTS/../features/cellular/framework/API"
	"UNITTESTS/../features/cellular/framework/AT"
	"UNITTESTS/../features/cellular/framework/device"
	"UNITTESTS/../features/cellular/framework"
	"UNITTESTS/../features/cellular/framework/common"
	"UNITTESTS/../features/lorawan"
	"UNITTESTS/../features/lorawan/lorastack"
	"UNITTESTS/../features/lorawan/lorastack/mac"
	"UNITTESTS/../features/lorawan/lorastack/phy"
	"UNITTESTS/../features/lorawan/system"
	"UNITTESTS/../features/mbedtls"
	"UNITTESTS/../features/mbedtls/inc"
	"UNITTESTS/../features/storage/kvstore/conf"
	"UNITTESTS/../features/storage/blockdevice"
	)

# create final library target which refers to MBed OS stubs
add_library(mbed-os INTERFACE)
target_compile_options(mbed-os INTERFACE -include ${MBED_CMAKE_SOURCE_DIR}/mbed-conf-unittests.h)
target_include_directories(mbed-os INTERFACE ${unittest-includes-base})

# fix error with missing types when building for MinGW
set(CMAKE_EXTRA_INCLUDE_FILES "sys/types.h")
check_all_types(uid_t gid_t nlink_t)
if(NOT HAVE_UID_T)
	target_compile_definitions(mbed-os INTERFACE uid_t=int)
endif()
if(NOT HAVE_GID_T)
	target_compile_definitions(mbed-os INTERFACE gid_t=int)
endif()
if(NOT HAVE_NLINK_T)
	target_compile_definitions(mbed-os INTERFACE nlink_t=int)
endif()