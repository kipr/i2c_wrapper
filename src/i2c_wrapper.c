/* 
    I2C_Wrapper is a convenience wrapper of the i2c-tools package
 
    Author: Joshua Southerland
  
    Based on i2cset.c from i2c-tools

*/

#include "i2c_wrapper.h"

#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include "i2cbusses.h"//i2c-tools/src
#include "version.h" //i2c-tools
#include <stdio.h>
#include <stdlib.h>

#include "check_funcs.h" // check_funcs()


int i2c_open_device(const char* i2c_bus_name, char *filename, size_t size, int quiet)
{
	int i2cbus = lookup_i2c_bus(i2c_bus_name);
	int file = open_i2c_dev(i2cbus, filename, size, quiet);
	
	if(!quiet) printf("i2cbus=%d and dev=%s\n",i2cbus,filename);

	return file;
}


unsigned char i2c_read_byte(int file, int daddress)
{
	return i2c_smbus_read_byte_data(file, daddress);
}


void i2c_close_device(int file)
{
	close(file);
}

//TODO: value is only a byte (unsigned char or __u8)
int i2c_write_byte(int file, unsigned char daddress, unsigned char value, int readback)
{
	int res = i2c_smbus_write_byte_data(file, daddress,value);

	if(readback) {
		res = i2c_smbus_read_byte_data(file, daddress);

		if (res < 0) {
			printf("Warning - readback failed\n");
			return -1;
		} else if (res != value) {
			printf("Warning - data mismatch - wrote "
				   "0x%0*x, read back 0x%0*x\n", 2, value, 2, res);
			return -1;
		}
	}

	return 0;
}

 
/** wrapper for check_funcs, a common function in i2c-tools
 *  can short-circuit as an error if the i2c bus isn't open
 */
int i2c_compatability_test(int file, int size, int pec)
{
	// error
	if (file < 0 || check_funcs(file, size, pec)) return -1;

	// success   
	return 0;
}


// address name is something like "0x1d"
int i2c_pick_slave(int file, const char* address_name)
{
	int address = parse_i2c_address(address_name);
	return set_slave_addr(file, address, 0); // don't force
}

