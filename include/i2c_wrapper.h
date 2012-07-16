/* 
    I2C_Wrapper is a convenience wrapper of the i2c-tools package
 
    Author: Joshua Southerland
  
    Based on i2cset.c from i2c-tools

*/



#ifndef I2C_WRAPPER_H
#define I2C_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <linux/i2c-dev.h>

int i2c_open_device(const char* i2c_bus_name, char *filename, size_t size, int quiet);

void i2c_close_device(int file);

unsigned char i2c_read_byte(int file, int daddress);


int i2c_write_byte(int file, unsigned char daddress, unsigned char value, int readback);
 


/** wrapper for check_funcs, a common function in i2c-tools
 *  can short-circuit as an error if the i2c bus isn't open
 */
int i2c_compatability_test(int file, int size, int pec);

// address name is something like "0x1d"
int i2c_pick_slave(int file, const char* address_name);

#ifdef __cplusplus
}
#endif

#endif //I2C_WRAPPER_H
