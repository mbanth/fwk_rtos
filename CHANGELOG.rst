RTOS Framework change log
=========================

3.3.0
-----

  * ADDED: Support for MSOS2.0 descriptor to automatically assign the WinUSB drivers to the DFU interface
    during enumeration on Windows. This enhancement is included in the USB descriptors of the USB tests.
  * ADDED: Support for multiread and multiwrite functions to the I2C master drivers.

3.2.0
-----

  * UPDATED: Tested against fwk_io v3.5.1 updated from v3.3.0
  * FIXED: Windows host issue with pre-2013 Visual Studio Compiler and stdbool.h.
  * UPDATED: libusb host drivers for Windows to v1.0.27. The device must now use WinUSB drivers on Windows.
  * UPDATED: To tinyusb_src commit c61f5f4, it includes official support for USB test mode.
  * FIXED: Adopted common format in error printouts of device control host drivers.

3.1.0
-----

  * UPDATED: To latest tinyusb_src
  * ADDED: Support for transitioning to USB test mode in tinyusb_src
  * UPDATED: Increase SPI tx/rx buffer sizes from 64 to 256 bytes

3.0.5
-----

  * FIXED: Added dummy definitions of vTaskCoreAffinitySet() and vTaskCoreAffinitySet() functions to
    avoid warnings when configNUM_CORES is set to 1.
  * UPDATED: Updated xmos_cmake_toolchain to v1.0.0 from untagged commit e577fbc
  * UPDATED: Tested against fwk_io v3.3.0 updated from v3.0.1
  * UPDATED: Tested against fwk_core v1.0.2 updated from v1.0.0
  * ADDED: Runtime check for I2S RPC functions called for I2S Slave

3.0.4
-----

  * FIXED: Issue seen with EP0 becoming unresponsive when lots of volume control and EP0 vendor specific
    control commands are issued at the same time.

3.0.3
-----

  * FIXED: Now fully supports overriding implementations for FatFS IO functions.

3.0.2
-----

  * UPDATED: To test again fwk_io v3.0.1
  * UPDATED: To test again lib_qspi_fast_read v1.0.1
  * ADDED: Retry connect behavior to lib_quadflash portion of RTOS flash driver
  * FIXED: Fixed device control host for Windows.

3.0.1
-----

  * FIXED: Fix to stop dest_ctrl_buffer being overwritten when xfers on EPs other than EP0 are initiated

3.0.0
-----

  * REMOVED: QSPI IO based RTOS flash driver
  * ADDED: lib_qspi_fast_read and lib_quadflash based RTOS flash driver
  * ADDED: RTOS flash driver function to set core affinity for QSPI transactions
  * ADDED: New API to specify raw or nibble swapped individual QSPI flash reads
  * UPDATED: Mbed TLS to version 2.28.3

2.0.0
-----

  * CHANGE: USB driver now supports XUD v2.2.2

1.1.0
-----

  * ADDED: Improved SPI device control support

1.0.0
-----

  * Initial version
