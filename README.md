# MBed-cmake
MBed-cmake is a build system template that lets you use the descriptive power of CMake to create build systems that use MBed OS.  

MBed-cmake was designed to optimize the common case of embedded development: where a program and an environment are configured once for a specific processor, and then left in that configuration forever.  Normally, MBed OS's build system forces you to reconfigure things like the target, the toolchain, and the ignore files each time a project is set up, as MBed OS and all of its dependencies are downloaded by the build system on your computer at build time.  In contrast, MBed-cmake allows you to check the entire OS and configuration into version control.  This makes build easy, repeatable, and dependable as there are no external dependencies.

Ordinarily, MBed OS can't be built easily using CMake.  The source files to build vary by processor and are generated automatically according to a complex set of rules, and additional flags and compile definitions are scattered in JSON files all around MBes OS's source tree.  

A stopgap solution is to use MBed's "export" functionality to create a Makefile or CMake build system and then develop that into a full build system.  However, this has major disadvantages: MBed itself is exported as a compiled lib, so you can't debug it or view its source code as easily.  Even worse, you're stuck using a generated build system, so if you need to switch to a different processor you'll have to re-export a new build system and make all of your modifications again.

MBed-cmake fixes this properly.  We've created an automatic scraper script that runs MBed OS's Python build scripts once to collect all of the source files and flags needed for a specific processor.  It then writes these into config files which are read by the CMake build system.  CMake can then build MBed OS (and your applications that use it) using this information.  This lets you build your MBed application easily using the full power of CMake scripts.

## Comparison to MBed's Build System
MBed-cmake has a number of advantages compared to MBed-CLI:
- **Completely expandable**: You can add any and all custom build options, commands, flags, and configurations in your CMake scripts.
- **Multiple targets**: You can create multiple main programs in a single project, and selecting them is as easy as `make <program>`.
- **Custom flash logic**: MBed-cmake includes an automatic code flasher similar to MBed CLI.  However, since the logic is in CMake scripts inside the project, you can easily change it or add new upload methods to suit your project.
- **IDE support**: MBed-cmake lets you use any and all IDEs supported by CMake to develop your project.  This provides improved code-assist functionality and many other conveniences (and prevents wars between people with different IDE preferences).
- **Automatic debugging**: Certain upload methods also provide debugging capability.  MBed-cmake provides convenient build system targets to automatically start a GDB server and run GDB for a specific program.  This functionality gets even more powerful when paired with an IDE that provides a graphical debugger.
- **Easy configuration switching**: MBed-cmake maps MBed's debug, develop, and release configurations to CMake's Debug, RelWithDebInfo, and Release configurations respectively.  Switching is as easy as a single CMake flag or menu option.
- **Compatible with MBed configurations**: MBed OS is configured exactly like normal, using mbed_app.json and .mbedignore files.  CMake will load the final MBed OS data after these files have been processed.
- **Completely reliable**: All code for your target is stored under a single Git repository, so there's no danger of code in external repositories failing to download or causing problems.
- **Fewer dependencies**: MBed OS's build system has a zillion Python dependencies that can be difficult to install correctly.  Though you will need to install these for the initial configuration, they aren't needed after that point, so you only need to install them on one system.

## Limitations
Currently MBed-cmake does not support toolchains other than GCC-ARM.  However this could be fixed without any major redesign if it was a desirable feature.

Additionally, the full range of MBed CLI's upload methods are not supported yet.

MBed-cmake was designed for use with a single processor target per project, and this is the most convenient way to use it.  However, you can also create a project that builds for multiple processors as long as you're OK with a few extra steps each time you switch processors:
 - Run `configure_for_target` passing your new processor name
 - Delete the build directory or use a different one.  Or, in CLion, use Tools > CMake > Reset Cache and Reload Project.
   - If you don't do this step you will end up using the wrong compiler flags for your processor!
