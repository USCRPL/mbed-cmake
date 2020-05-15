# Edit this file to configure the upload methods for your processor.
# An example configuration is shown here for RPL's boards based around the LPC1768.

# General config options
# -------------------------------------------------------------
set(UPLOAD_METHOD_DEFAULT MBED)

# use a higher numbered port to allow use without root on Linux/Mac
set(GDB_PORT 23331)

# Config options for MBED
# -------------------------------------------------------------

set(MBED_UPLOAD_ENABLED TRUE)

# Config options for JLINK
# -------------------------------------------------------------

set(JLINK_UPLOAD_ENABLED TRUE)

if(JLINK_UPLOAD_ENABLED)
    set(JLINK_CPU_NAME LPC1768)
    set(JLINK_JTAG_SPEED 4000)
    set(JLINK_RESET_TARGET TRUE)
endif()

