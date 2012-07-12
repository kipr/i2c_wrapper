/* 
    accel_test.c - put accelerometer into test mode and then read x,y,z accelerations
      
    Device: "Kovan" board
 
    Author: Joshua Southerland
  
    Based on i2cset.c from i2c-tools

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "i2c_wrapper.h"





int main(int argc, char** argv){


	int size, file;
	int value, daddress = 0;
	int pec = 0;
	int yes = 0, readback = 0;
	int len;
	char filename[20];
	yes = 1; // don't ask to confirm
	readback = 1; // check value
	size = I2C_SMBUS_BYTE_DATA; //from <linux/i2c-dev.h>
	len = 0; /* Must always initialize len since it is passed to confirm() */


	printf("Accelerometer test...\n");



	// set device in measurement mode
	//
	// i2cset -y 1 0x1d 0x16 0x05
	// -y so "yes = 1"
	// bus 1
	// chip address 0x1d
	// data address 0x16
	// data value 0x5

	daddress = 0x16; // data address
	value = 0x05; // data value

	// open the i2c bus
	file = i2c_open_device("1", filename, sizeof(filename), 0);
	if (file < 0) return EXIT_FAILURE;

	// check compatability
	if (i2c_compatability_test(file,size,pec) < 0){
		return EXIT_FAILURE;
	}

	// choose the accelerometer
	if  (i2c_pick_slave(file, "0x1d") < 0){
		return EXIT_FAILURE;
	}


	// write 1 byte of data
	if (i2c_write_byte(file, daddress, value, 1) < 0){
		fprintf(stderr, "Warning - write failed\n");
		return EXIT_FAILURE;
	}	





	// read x,y,z accelerations
	//
	// i2cdump -y -r 6-8 1 0x1d
	// -y so "yes = 1"
	// -r so address range = 6-8
	// bus 1
	// chip address = 0x1d


	int i;
	int c = 0;
	char accels[3];
	while(1){
		c++;
		// read x,y,z accels from registers 6-8
		for (i = 0; i < 3; i++){ 	
			accels[i] =  i2c_read_byte(file,  6 + i);
		}
		if ((c%100) == 0)printf("< x:0x%x, y:0x%x, z:0x%x >\n", accels[0], accels[1], accels[2]);
	}


	// close i2c
	i2c_close_device(file);

	return EXIT_SUCCESS;
}
