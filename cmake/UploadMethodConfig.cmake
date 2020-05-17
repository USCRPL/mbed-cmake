# Edit this file to configure the upload methods for your processor.
# An example configuration is shown here for RPL's boards based around the LPC1768.

# See here for documentation about these options: https://github.com/USCRPL/mbed-cmake/wiki/Upload-Methods

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

set(JLINK_UPLOAD_ENABLED FALSE)

if(JLINK_UPLOAD_ENABLED)
    set(JLINK_CPU_NAME LPC1768)
    set(JLINK_JTAG_SPEED 4000)
endif()

# Config options for PYOCD
# -------------------------------------------------------------

set(PYOCD_UPLOAD_ENABLED FALSE)

if(PYOCD_UPLOAD_ENABLED)
    set(PYOCD_TARGET_NAME LPC1768)
    set(PYOCD_JTAG_SPEED 4000k)
endif()

# Config options for OPENOCD
# -------------------------------------------------------------

set(OPENOCD_UPLOAD_ENABLED FALSE)

if(OPENOCD_UPLOAD_ENABLED)
    set(OPENOCD_CHIP_CONFIG_COMMANDS
        -f ${OpenOCD_SCRIPT_DIR}/interface/cmsis-dap.cfg
        -f ${OpenOCD_SCRIPT_DIR}/target/lpc17xx.cfg
        -c "gdb_memory_map disable")
endif()

# Config options for NXPPROG
# -------------------------------------------------------------

set(NXPPROG_UPLOAD_ENABLED FALSE)

if(NXPPROG_UPLOAD_ENABLED)
    set(NXPPROG_OSCFREQ 14748)
    set(NXPPROG_BAUD 115200)
endif()

