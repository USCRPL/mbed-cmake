

# When in unit testing mode, we want buildfiles to still process OK.
# However, executables can't link correctly because mbed-os is stubbed out as headers.
# So, while libraries build normally, add_mbed_executable is a no-op.
function(add_mbed_executable EXECUTABLE)

	# unit tests active -- create a dummy target that only exists so setting properties on it doesn't fail.
	add_executable(${EXECUTABLE} ${MBED_CMAKE_SOURCE_DIR}/cmake/dummy.cpp)

	# prevent this target from being built during normal make commands
	set_property(TARGET ${EXECUTABLE} PROPERTY EXCLUDE_FROM_ALL TRUE)

endfunction(add_mbed_executable)

 
