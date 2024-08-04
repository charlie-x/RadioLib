/*
   RadioLib Non-Arduino Raspberry Pi Example

   This example shows how to use RadioLib without Arduino.
   In this case, a Raspberry Pi with WaveShare SX1302 LoRaWAN Hat
   using the lgpio library
   https://abyz.me.uk/lg/lgpio.html

   Can be used as a starting point to port RadioLib to any platform!
   See this API reference page for details on the RadioLib hardware abstraction
   https://jgromes.github.io/RadioLib/class_hal.html

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

// include the library
#include <RadioLib.h>

// include the hardware abstraction layer
#include "PiHal.h"
#include "hidapi.h"

hid_device *handle;
// create a new instance of the HAL class
// use SPI channel 1, because on Waveshare LoRaWAN Hat,
// the SX1261 CS is connected to CE1
PiHal* hal = new PiHal(1);

// now we can create the radio module
// pinout corresponds to the Waveshare LoRaWAN Hat
// NSS pin:   7
// DIO1 pin:  17
// NRST pin:  22
// BUSY pin:  not connected
SX1262 radio = new Module(hal, 7, 17, 22, RADIOLIB_NC);

// the entry point for the program
int main(int argc, char** argv) {

  {

    struct hid_device_info *devs, *cur_dev;


    devs = hid_enumerate ( 0x0, 0x0 );
    cur_dev = devs;

    printf("looking for hid devices\n");
    while ( cur_dev ) {

        printf ( "Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number );
        printf ( "\n" );
        printf ( "  Manufacturer: %ls\n", cur_dev->manufacturer_string );
        printf ( "  Product:      %ls\n", cur_dev->product_string );
        printf ( "  Release:      %hx\n", cur_dev->release_number );
        printf ( "  Interface:    %d\n",  cur_dev->interface_number );
        printf ( "\n" );
        cur_dev = cur_dev->next;
    }

    hid_free_enumeration ( devs );

}



  // initialize just like with Arduino
  printf("[SX1261] Initializing ... ");
  int state = radio.begin(915.0, 125.0, 9, 7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 10, 8, 0, false);
  if (state != RADIOLIB_ERR_NONE) {
    printf("failed, code %d\n", state);
    return(1);
  }
  printf("success!\n");

  // loop forever
  int count = 0;
  for(;;) {
    // send a packet
    printf("[SX1261] Transmitting packet ... ");
    char str[64];
    sprintf(str, "Hello World! #%d", count++);
    state = radio.transmit(str);
    if(state == RADIOLIB_ERR_NONE) {
      // the packet was successfully transmitted
      printf("success!\n");

      // wait for a second before transmitting again
      hal->delay(1000);

    } else {
      printf("failed, code %d\n", state);

    }

  }

  return(0);
}
