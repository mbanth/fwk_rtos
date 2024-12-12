############
Memory Usage
############

This section describes how to check the memory usage for a given application.
The user can get this information at compile time or after compilation.

************
Compile Time
************

The XTC Tools provide compile time information to aid developers in creating and testing their application.
One of these features is the `-report` option, which will display a summary of resource usage.
This report includes information on memory usage, split into the stack, code, and data requirements of the program.

Unlike most XC applications, FreeRTOS makes heavy use of dynamic memory allocation.
The FreeRTOS heap will appear as `Data` in the XTC Tools report.
The heap size is determined by the compile time definition `configTOTAL_HEAP_SIZE`, which can be found in an application's FreeRTOSConfig.h file.

Typically, a xcore FreeRTOS-based application will consist of multiple executable files, one per tile.
The cmake build system includes a macro, `merge_binaries()`, which combines the multiple executable files into one.
Consequently, the console output from the XTC Tools will contain information for each of the individual executable files.

As an example, consider a hypothetical `example_freertos_explorer_board` target which operates over two tiles.
Building it with the `-report` option produces the memory usage summary shown below.

.. code-block:: console

    Constraint check for tile[0]:
      Memory available:       524288,   used:      318252 .  OKAY
        (Stack: 5260, Code: 42314, Data: 270678)
    Constraints checks PASSED WITH CAVEATS.
    Constraint check for tile[1]:
      Memory available:       524288,   used:       4060 .  OKAY
        (Stack: 356, Code: 3146, Data: 558)
    Constraints checks PASSED.

    Constraint check for tile[0]:
      Memory available:       524288,   used:       4836 .  OKAY
        (Stack: 356, Code: 3802, Data: 678)
    Constraints checks PASSED.
    Constraint check for tile[1]:
      Memory available:       524288,   used:      319476 .  OKAY
        (Stack: 14740, Code: 30730, Data: 274006)
    Constraints checks PASSED WITH CAVEATS.


One report appears for each of the initial executable files, `tile0_example_freertos_explorer_board.xe` and `tile1_example_freertos_explorer_board.xe`.
The executable focussed on tile[0] includes a small amount of code on tile[1] which contains functionality not needed in the final file.
Likewise,, the executable focussed on tile[1] includes a small amount of code on tile[0] that can be discarded.

The make file created by cmake contains the command:

.. code-block:: cmake

    merge_binaries(example_freertos_explorer_board tile0_example_freertos_explorer_board tile1_example_freertos_explorer_board 1)

This command combines the two initial executable files into a single one, `example_freertos_explorer_board.xe`, discarding the unnecessary portions.
Consequently, the correct interpretation of the reported memory usage for the final application appears below:

.. code-block:: console

    Constraint check for tile[0]:
      Memory available:       524288,   used:      318252 .  OKAY
        (Stack: 5260, Code: 42314, Data: 270678)
    Constraints checks PASSED WITH CAVEATS.
    Constraint check for tile[1]:
      Memory available:       524288,   used:      319476 .  OKAY
        (Stack: 14740, Code: 30730, Data: 274006)
    Constraints checks PASSED WITH CAVEATS.

*****************
After Compilation
*****************

The XTC Tools also provide a method to examine the resource usage of a binary post-build.
This method will only work if used on the intermediate executable files.

.. code-block:: console

    $ xobjdump --resources tile0_example_freertos_explorer_board.xe
    $ xobjdump --resources tile1_example_freertos_explorer_board.xe


.. note::
    Because the resulting `example_freertos_explorer_board.xe` binary was created by merging into `tile0_example_freertos_explorer_board.xe`,
    the results of `xobjdump --resources example_freertos_explorer_board.xe` will be the exact same as `xobjdump --resources tile0_example_freertos_explorer_board.xe`
    and will not account for the actual tile 1 requirements.

