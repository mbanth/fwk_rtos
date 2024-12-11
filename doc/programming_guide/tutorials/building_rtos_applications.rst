##########################
Building RTOS Applications
##########################

Applications using the RTOS Framework are built using `CMake`.
The RTOS framework provides many libraries, drivers and software services, all of which can be included by the application's ``CMakeLists.txt`` file.
The application's CMakeLists can specify precisely which drivers and software services within the SDK should be included through the use of various `CMake` target aliases.

See the :ref:`Build System Guide <build_system_guide>` for more information on the build system.

See the :ref:`Build System Guide - Targets <build_system_targets>` for more information on the build system target aliases.
