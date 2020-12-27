#ifndef uss_H
#define uss_H

#include "main.h"

typedef struct USS_Sensor {
	
	float distance;
	bool _cap_ready;
	uint32_t _cap_start;
	uint32_t _cap_end;
	float _correction_factor;
	uint32_t _timer_frequency;
	
} USS_Sensor_t;

void USS_init(USS_Sensor_t* sensor, float correction_factor, uint32_t timer_frequency);

float USS_calculateDistance(USS_Sensor_t* sensor);
void USS_startCount(USS_Sensor_t* sensor, uint32_t ccr);
void USS_stopCount(USS_Sensor_t* sensor, uint32_t ccr);

#endif /* uss_H */