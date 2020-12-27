#include "_uss.h"

void USS_init(USS_Sensor_t* sensor, float correction_factor, uint32_t timer_frequency) {
	
	sensor->distance = 0;
	sensor->_cap_start = 0;
	sensor->_cap_end = 0;
	sensor->_cap_ready = FALSE;
	sensor->_correction_factor = correction_factor;
	sensor->_timer_frequency = timer_frequency;
}

float USS_calculateDistance(USS_Sensor_t* sensor) {

#define USS_CORRECTION_FACTOR	2.2
#define SPEED_SOUND						340		
#define CAPTURE_MASK					0x0FFFFFFF
	float cap;
	
	if (!sensor->_cap_ready)
		return 30.0;
	
	cap = (float)((sensor->_cap_end - sensor->_cap_start) & CAPTURE_MASK);
	sensor->distance = USS_CORRECTION_FACTOR * (cap / sensor->_timer_frequency) * SPEED_SOUND / 2;
	
	sensor->_cap_ready = FALSE;

	return sensor->distance;
}


void USS_startCount(USS_Sensor_t* sensor, uint32_t ccr) {
	
	sensor->_cap_start = ccr;
	sensor->_cap_ready = FALSE;
}

void USS_stopCount(USS_Sensor_t* sensor, uint32_t ccr) {
	
	sensor->_cap_end = ccr;
	sensor->_cap_ready = TRUE;
}