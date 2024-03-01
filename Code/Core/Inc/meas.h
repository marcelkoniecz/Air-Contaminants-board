/*
 * meas.h
 *
 *  Created on: Jan 17, 2024
 *      Author: maroc
 */

#ifndef INC_MEAS_H_
#define INC_MEAS_H_

#include "main.h"

//ADC
//#define V_REF 3.29		// ADC_VREF [V] -> global variable computing at the beginning
#define ADC_RES_VAL	4095
#define DIV_VOL	3.02		// [V]
#define ADC_CHANNEL_VREF ADC_CHANNEL_17
#define VREFINT_CAL_ADDR ((uint16_t*)((uint32_t)0x1FFFF7BA))


//TEMP DIV - linear aproximation
#define ADC_CHANNEL_TEMP	ADC_CHANNEL_1
#define DIV_RES_TEMP 9960							// [Ohm]
#define RES_x_VOL_TEMP (DIV_VOL*DIV_RES_TEMP)
#define A_REG_TEMP 0.06340592						//y=ax+b
#define B_REG_TEMP -102.41

//GAS DIV
#define ADC_CHANNEL_GAS		ADC_CHANNEL_2
#define DIV_RES_GAS 33000					//[Ohm]
#define A_REG_GAS 0.06340592				//
#define B_REG_GAS 1


typedef enum sensor{
	TEMP,
	GAS
}sensor_t;

typedef struct TEMP_SENSOR{
	uint16_t adc_channel;
	uint16_t raw_val;		//Here is saved value read from adc
	uint8_t updated;		// if there is a new raw value
	double temp;			//temperature in C
}TEMP_SENSOR;


typedef struct GAS_SENSOR{
		uint16_t adc_channel;
		uint16_t raw_val;		//Here is saved value read from adc
		uint8_t updated;		// if there is a new raw value
		double temp;			//temperature in C
}GAS_SENSOR;


extern TEMP_SENSOR tmp_sensor;
extern GAS_SENSOR gas_sensor;


void init_sensors();
double read_sens_val(sensor_t );
void write_sens_raw(sensor_t, uint16_t);
void measure(sensor_t);
void calculate(sensor_t);


#endif /* INC_MEAS_H_ */
