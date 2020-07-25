```
 _       _    ____      ______   ___                      ____    _       _         _        _      _   ______ 
| \     / |  /  _  \   /  ____) /  _ \_                 / ____)  | \     / |       / \      / \    / | /  ____)
|  \   /  |  | |_|  |  | |____  | | \_ \    ______    /  /       |  \   /  |      / _ \     | |   / /  | |____ 
|   \ /   |  |     /   |  ____) | |   | |  (______)  |  |        |   \ /   |     / /_\ \    |  _ / /   |  ____)
| |\ - /| |  |  _   \  | |      | |  _| |             \  \       | |\ - /| |    / _____ \   |  _  \    | |     
| | \_/ | |  | |_|   | | |____  | |_/ _/                \ \___   | | \_/ | |   / /     \ \  | |  \ \   | |____ 
\_/     \_/  \_____ /  \______) \___ /                   \____)  \_/     \_/  |_/       \_| \_/   \_/  \______)
```

MBed-cmake is a build system template that lets you use the descriptive power of CMake to create build systems that use MBed OS.  

MBed-cmake was designed to optimize the common case of embedded development: where a program and an environment are configured once for a specific processor, and then left in that configuration forever.  Normally, MBed OS's build system forces you to reconfigure things like the target, the toolchain, and the ignore files each time a project is set up, as MBed OS and all of its dependencies are downloaded by the build system on your computer at build time.  In contrast, MBed-cmake allows you to check the entire OS and configuration into version control.  This makes builds easy, repeatable, and dependable as there are no external dependencies.

Ordinarily, MBed OS can't be built easily using CMake.  The source files to build vary by processor and are generated automatically according to a complex set of rules, and additional flags and compile definitions are scattered in JSON files all around MBed OS's source tree.  

A stopgap solution is to use MBed's "export" functionality to create a Makefile or CMake build system and then develop that into a full build system.  However, this has major disadvantages: MBed itself is exported as a compiled lib, so you can't debug it or view its source code as easily.  Even worse, you're stuck using a generated build system, so if you need to switch to a different processor you'll have to re-export a new build system and make all of your modifications again.

MBed-cmake fixes this properly.  We've created an automatic scraper script that runs MBed OS's Python build scripts once to collect all of the source files and flags needed for a specific processor.  It then writes these into config files which are read by the CMake build system.  CMake can then build MBed OS (and your applications that use it) using this information.  This lets you build your MBed application easily using the full power of CMake scripts.

## Advantages 
- **Completely expandable**: You can add any and all custom build options, commands, flags, and configurations in your CMake scripts.
- **Multiple targets**: You can create multiple main programs in a single project, and selecting them is as easy as `make <program>`.
- **Custom flash logic**: MBed-cmake includes an automatic code uploader similar to MBed CLI.  However, since the logic is in CMake scripts inside the project, you can easily change it or add new upload methods to suit your project.
- **IDE support**: MBed-cmake lets you use any and all IDEs supported by CMake to develop your project.  This provides improved code-assist functionality and many other conveniences (and prevents wars between people with different IDE preferences).
- **Automatic debugging**: Certain upload methods also provide debugging capability.  MBed-cmake provides convenient build system targets to automatically start a GDB server and run GDB for a specific program.  This functionality gets even more powerful when paired with an IDE that provides a graphical debugger.
- **Easy configuration switching**: MBed-cmake maps MBed's debug, develop, and release configurations to CMake's Debug, RelWithDebInfo, and Release configurations respectively.  Switching is as easy as a single CMake flag or menu option.
- **Use standard MBed OS configurations**: MBed OS is configured exactly like normal, using mbed_app.json and .mbedignore files.  CMake will load the final MBed OS data after these files have been processed.
- **Completely reliable**: All code for your target is stored under a single Git repository, so there's no danger of code in external repositories failing to download, being modified, or causing problems.
- **Fewer dependencies**: MBed OS's build system has a zillion Python dependencies that can be difficult to install correctly.  Though you will need to install these for the initial configuration, they aren't needed after that point, so you only need to install them on one system.

## Getting Started
Interested in trying mbed-cmake?  Follow these steps:
1. Clone [the example project](https://github.com/USCRPL/mbed-cmake-example-project): `git clone --recursive https://github.com/USCRPL/mbed-cmake-example-project.git`
1. Update the mbed-cmake submodule to the latest version: `git submodule foreach git pull origin master`
2. [Set up the toolchain and other dependencies on your machine](https://github.com/USCRPL/mbed-cmake/wiki/Toolchain-Setup).
3. [Configure mbed-cmake for your processor target](https://github.com/USCRPL/mbed-cmake/wiki/Project-Configuration).
4. [Set up the upload methods you need](https://github.com/USCRPL/mbed-cmake/wiki/Upload-Methods), then test them with the included hello world program.
5. Add your own code and [write CMake buildfiles to build it](https://github.com/USCRPL/mbed-cmake/wiki/CMake-Cookbook).

Note: mbed-cmake can be used either as a Git submodule, or by just downloading the files and including them directly in your repo.  If submodules aren't your thing, then just download this repo and the example repo as zips instead of the first two steps above.  Don't forget to check back here for updates!

## Example
Simple MBed OS applications can be built with a single CMake command:
```cmake
add_mbed_executable(hello_world HelloWorld.cpp)
```

Libraries are also quite straightforward:
```cmake
set(MAX31856_SOURCES
        MAX31856.h
        MAX31856.cpp)

add_library(max31856 STATIC ${MAX31856_SOURCES})
target_include_directories(max31856 PUBLIC .)
target_link_libraries(max31856 mbed-os)
```

## Bundled MBed
This repository includes MBed OS 6.1.0 stable. 
You should be able to use a different version of MBed by simply replacing the `mbed-src` folder, just be sure to copy CMakeLists.txt into the new mbed-src folder.  Note that unignore rules in the .mbedignore is currently an mbed-cmake extension and this functionality will be lost if you replace mbed-src.  We submitted a PR to upstream though!

## Limitations
- Currently MBed-cmake only supports the GNU Arm Embedded (`-t GCC_ARM`) and ARMClang (`-t ARMC6`) toolchains.  
- CLion does not work with ARMClang -- the IDE is not able to scan compiler options so the project doesn't load.  It seems to be a CLion bug.
- The full range of MBed CLI's upload methods are not supported yet.
- LTO cannot be supported for GCC_ARM due to [a gcc bug](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=83967)

MBed-cmake was designed for use with a single processor target per project, and this is the most convenient way to use it.  However, you can also create a project that builds for multiple processors as long as you're OK with a few extra steps each time you switch processors (see [the instructions](https://github.com/USCRPL/mbed-cmake/wiki/Project-Configuration)).
