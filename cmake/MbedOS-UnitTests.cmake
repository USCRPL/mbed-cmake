# This file is called by mbed-src/CMakeLists.txt when unit tests are active
# and we only want to build a stub that contains all the headers.

project(mbed-os)


# Unit test stub include dirs (list adapted from mbed-src/UNITTESTS/CMakeLists.txt)
set(unittest-includes-base
	"UNITTESTS/target_h"
	"events/tests/UNITTESTS/target_h"
	"events/tests/UNITTESTS/target_h/equeue"
	"UNITTESTS/target_h/platform"
	"UNITTESTS/target_h/platform/cxxsupport"
	"UNITTESTS/target_h/drivers"
	"UNITTESTS/target_h/rtos/include"
	"UNITTESTS/stubs"
	"."
	"features"
	"platform/include"
	"platform/include/platform"
	"platform/mbed-trace/include"
	"storage/filesystem/littlefs/include"
	"storage/filesystem/fat/include"
	"storage/blockdevice/include"
	"storage/filesystem/include"
	"storage/kvstore/include"
	"storage/kvstore/kv_config"
	"storage/kvstore/kv_config/include"
	"drivers"
	"drivers/include"
	"drivers/include/drivers"
	"drivers/include/drivers/internal"
	"hal"
	"hal/include"
	"events/include"
	"events/include/events/internal"
	"events/source"
	"rtos/include"
	"features/frameworks"
	"connectivity/libraries/nanostack-libservice"
	"connectivity/libraries/nanostack-libservice/mbed-client-libservice"
	"connectivity/netsocket/include"
	"features/filesystem/fat"
	"features/filesystem/fat/ChaN"
	"features/filesystem/bd"
	"features/filesystem/"
	"features/filesystem/littlefs"
	"features/filesystem/littlefs/littlefs"
	"connectivity/cellular/include/cellular/framework/API"
	"connectivity/cellular/include/cellular/framework/AT"
	"connectivity/cellular/include/cellular/framework/device"
	"connectivity/cellular/include/cellular/framework"
	"connectivity/cellular/include/cellular/framework/common"
	"connectivity"
	"connectivity/lorawan/include/lorawan"
	"connectivity/lorawan/lorastack"
	"connectivity/lorawan/lorastack/mac"
	"connectivity/lorawan/lorastack/phy"
	"connectivity/lorawan"
	"connectivity/mbedtls"
	"connectivity/mbedtls/include"
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