/* 
    back_emf_test.c - put accelerometer into test mode and then read x,y,z accelerations
      
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


	printf("Battery test...\n");


	// open the i2c bus
	file = i2c_open_device("0", filename, sizeof(filename), 0);
	if (file < 0) return EXIT_FAILURE;

	// check compatability
	if (i2c_compatability_test(file,size,pec) < 0){
		return EXIT_FAILURE;
	}

	// choose the fpga
	if  (i2c_pick_slave(file, "0x1e") < 0){
		return EXIT_FAILURE;
	}


//                     addy, len, msb, lsb, mode, desc
//
//'adc_go'          : (0x46, 1, 4, 4, 'rw', 'sample ADC data'),
//'adc_chan'        : (0x46, 1, 3, 0, 'rw', 'set ADC channel to sample'),
//'adc_valid'       : (0x83, 1, 0, 0,  'ro', 'ADC input value up to date'),
//'adc_in'          : (0x81, 2, 9, 0,  'ro', 'ADC input value'),


	// write 1 byte of data
	//if (i2c_write_byte(file, daddress, value, 1) < 0){
	//	fprintf(stderr, "Warning - write failed\n");
	//	return EXIT_FAILURE;
	//}	

	// adcChan = 8;
	unsigned char adc_chan_sel = 8;
	unsigned char adc_chan = 0; 

    //adcChan = kovanGet('adc_chan')
	adc_chan = i2c_read_byte(file, 0x46);  
	printf("adc chan was 0x%0x\n",adc_chan);


	//   anapu = kovanGet('ana_pu')

	//   kovanSet('ana_pu', anapu)

	//   kovanSet('dig_update', 0)

	//   kovanSet('dig_update', 1)

	//   kovanSet('dig_update', 0)



	unsigned char adc_go_bit = 16;


	char res;
	int c = 0;
	while(1){
		c+=1;

		//   kovanSet('adc_chan', adcChan)
		adc_chan = adc_chan & ~15; // clear last 4 bits
		adc_chan = adc_chan | adc_chan_sel;  // set adc channel
//    	//   kovanSet('adc_chan', adcChan)
//		if (i2c_write_byte(file, 0x46, adc_chan, 0) < 0){
//			fprintf(stderr, "Error - write failed\n");
//			return EXIT_FAILURE;
//		}	
	

		//   kovanSet('adc_go', 1 )
		adc_chan = adc_chan | adc_go_bit; // set "go bit" active

		if (i2c_write_byte(file, 0x46, adc_chan, 0) < 0){
			fprintf(stderr, "Error - write failed\n");
			return EXIT_FAILURE;
		}

		adc_chan = adc_chan & ~adc_go_bit; // clear "go bit"

		//  kovanSet('adc_go', 0 )
		if (i2c_write_byte(file, 0x46, adc_chan, 0) < 0){
			fprintf(stderr, "Error - write failed\n");
			return EXIT_FAILURE;
		}


		// while kovanGet('adc_valid') == 0:
		//     dummy = 1  # just wait
		res = 0;
		while (res == 0){
			res = i2c_read_byte(file, 0x83);  
			//printf("res = %d\n",res);
			usleep(1);
		}

		// val = kovanGet('adc_in')
		unsigned char high = i2c_read_byte(file, 0x82); //TODO: 81?
		unsigned char low = i2c_read_byte(file, 0x81); //TODO 82?
		unsigned short volt_raw = (high << 8) | low;

		if((c%100)==0)printf("raw 10bit battery reading = %d\n",volt_raw);
	}

	//	accels[i] =  i2c_read_byte(file,  6 + i);


	// close i2c
	i2c_close_device(file);

	return EXIT_SUCCESS;
}
