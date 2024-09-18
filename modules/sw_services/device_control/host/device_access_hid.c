// Copyright 2016-2024 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.
#if USE_HID
#include <stdio.h>
#if !defined(_MSC_VER) || (_MSC_VER >= 1800) // !MSVC or MSVC >=VS2013
#include <stdbool.h>
#else
typedef enum { false = 0, true = 1} bool;
#endif // MSC
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <hidapi.h>
#include "device_control_host.h"
#include "control_host_support.h"
#include "util.h"

static unsigned num_commands = 0;

static hid_device *devh = NULL;

static const int sync_timeout_ms = 500;

/* Control query transfers require smaller buffers */
#define VERSION_MAX_PAYLOAD_SIZE 63
#define HEADER_SIZE 4 // HID report ID, resid, cmd, payload_len

control_ret_t control_query_version(control_version_t *version)
{
  control_ret_t ret;
  uint8_t request_data[VERSION_MAX_PAYLOAD_SIZE];

  ret = control_read_command(CONTROL_SPECIAL_RESID, CONTROL_GET_VERSION, request_data, sizeof(control_version_t));

  memcpy(version, request_data, sizeof(control_version_t));
  DBG(printf("version returned: 0x%X\n", *version));

  return CONTROL_SUCCESS;
}


static bool payload_len_exceeds_control_packet_size(size_t payload_len)
{
  if (payload_len > HID_TRANSACTION_MAX_BYTES-HEADER_SIZE) {
    printf("control transfer of %zd bytes requested\n", payload_len);
    printf("maximum control packet size is %d\n", HID_TRANSACTION_MAX_BYTES-4);
    return true;
  }
  else {
    return false;
  }
}

control_ret_t
control_write_command(control_resid_t resid, control_cmd_t cmd,
                      const uint8_t payload[], size_t payload_len)
{
  uint8_t buf[HID_TRANSACTION_MAX_BYTES];
  int res;
  if (payload_len_exceeds_control_packet_size(payload_len))
    return CONTROL_DATA_LENGTH_ERROR;
  control_build_hid_data(buf, resid, CONTROL_CMD_SET_WRITE(cmd), payload, (unsigned int)payload_len);
  DBG(printf("%u: send HID output report: 0x%04x 0x%04x 0x%04x ",
    num_commands, resid, CONTROL_CMD_SET_WRITE(cmd), payload_len));
  DBG(print_bytes(&payload[4], payload_len));

  // Send the output report
  res = hid_write(devh, buf, sizeof(buf));
  if (res < 0) {
    PRINT_ERROR("%ls\n", hid_error(devh));
  } else {
    DBG(printf("Data sent successfully\n"));
  }
  num_commands++;
  // TODO: Read back write command status

  return CONTROL_SUCCESS;

}

control_ret_t
control_read_command(control_resid_t resid, control_cmd_t cmd,
                     uint8_t payload[], size_t payload_len)
{
  uint8_t buf[HID_TRANSACTION_MAX_BYTES] = {0};
  memset(payload, 0, payload_len);
  int res;
  if (payload_len_exceeds_control_packet_size(payload_len))
    return CONTROL_DATA_LENGTH_ERROR;

  control_build_hid_data(buf, resid, CONTROL_CMD_SET_READ(cmd), payload, (unsigned int)payload_len);
  DBG(printf("%u: send HID output report: ", num_commands));
  DBG(print_bytes(buf, payload_len));

  // Send the output report
  res = hid_write(devh, buf, sizeof(buf));
  if (res < 0) {
    PRINT_ERROR("%ls\n", hid_error(devh));
  } else {
    DBG(printf("Data sent successfully\n"));
  }

  // Wait for the device to process the first request
  #ifdef _WIN32
    Sleep(1); // Sleep takes milliseconds
  #else
    usleep(20000); // usleep takes microseconds
  #endif

  control_build_hid_data(buf, resid, CONTROL_CMD_SET_READ(cmd), payload, (unsigned int)payload_len);

  res = hid_get_input_report(devh, buf, sizeof(buf));
  if (res < 0) {
    PRINT_ERROR("%ls\n", hid_error(devh));
  } else {
    DBG(printf("Data received successfully\n"));
  }
  DBG(printf("%u: read HID input report: ", num_commands));
  DBG(print_bytes(buf, HID_TRANSACTION_MAX_BYTES));
  memcpy(payload, &buf[1], payload_len);
  num_commands++;

  return CONTROL_SUCCESS;
}

control_ret_t control_init_hid(int vendor_id, int product_id)
{

  // Initialize the HIDAPI library
  if (hid_init()) {
    PRINT_ERROR("Failed to initialize HIDAPI\n");
    return CONTROL_ERROR;
  }

  // Open the device using the VID, PID,
  // and optionally the Serial number.
  devh = hid_open(vendor_id, product_id, NULL);
  if (!devh) {
    PRINT_ERROR("Unable to open device\n");
    return CONTROL_ERROR;
  }

  hid_set_nonblocking(devh, 1);
  return CONTROL_SUCCESS;
}

control_ret_t control_cleanup_hid(void)
{
  // Close the device
  hid_close(devh);

  // Finalize the HIDAPI library
  hid_exit();

  return CONTROL_SUCCESS;
}

#endif // USE_HID
