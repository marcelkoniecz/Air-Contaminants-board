

#include "meas.h"
#include "adc.h"

TEMP_SENSOR temp_sensor;
GAS_SENSOR gas_sensor;

double V_REF = 0;


void init_sensors(){

	uint16_t ref_val;
//	double vdda;

	//ADC calibration

	HAL_ADCEx_Calibration_Start(&hadc);
	  ADC1->CHSELR = ADC_CHSELR_CHANNEL(ADC_CHANNEL_VREF);
	  HAL_ADC_Start(&hadc);
	  HAL_ADC_PollForConversion(&hadc, 10000);
	  ref_val = HAL_ADC_GetValue(&hadc);
	  V_REF = 3.3 *(*VREFINT_CAL_ADDR) /ref_val;

	//Initialization of gas sensor
	gas_sensor.adc_channel = ADC_CHANNEL_GAS;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,1);

	//Initialization of temperature sensor
	temp_sensor.adc_channel = ADC_CHANNEL_TEMP;
	temp_sensor.updated = 0;
}

void measure(sensor_t sensor){

	switch(sensor){
		case GAS:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,0);
			HAL_Delay(50);
			ADC1->CHSELR = ADC_CHSELR_CHANNEL(gas_sensor.adc_channel);
			HAL_ADC_Start(&hadc);
			HAL_ADC_PollForConversion(&hadc, 10000);
			write_sens_raw(sensor,HAL_ADC_GetValue(&hadc));
			calculate(sensor);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5,1);
			break;
		case TEMP:
			ADC1->CHSELR = ADC_CHSELR_CHANNEL(temp_sensor.adc_channel);
			HAL_ADC_Start(&hadc);
			HAL_ADC_PollForConversion(&hadc, 10000);
			write_sens_raw(sensor,HAL_ADC_GetValue(&hadc));
			calculate(sensor);
			break;
	}

}


void calculate(sensor_t sensor){
	static double temp = 0;
	switch(sensor){
		case GAS:
			temp = ((float)gas_sensor.raw_val/(float)ADC_RES_VAL)*V_REF;
			break;
		case TEMP:
			if(temp_sensor.updated){

								//  a * R1 + b
				temp = ((float)temp_sensor.raw_val/(float)ADC_RES_VAL)*V_REF;
				temp_sensor.temp = A_REG_GAS*((RES_x_VOL_TEMP)/(temp)-(double)DIV_RES_TEMP)+B_REG_TEMP;
				temp_sensor.updated = 0;
			}
			break;
	}
}


double read_sens_val(sensor_t sensor){
	switch(sensor){
		case GAS:

			break;
		case TEMP:
			return temp_sensor.temp;
			break;
	}
}

void write_sens_raw(sensor_t sensor, uint16_t raw_val){
	switch(sensor){
		case GAS:
			gas_sensor.raw_val = raw_val;
			gas_sensor.updated = 1;
			break;
		case TEMP:
			temp_sensor.raw_val = raw_val;
			temp_sensor.updated = 1;
			break;
	}
}
