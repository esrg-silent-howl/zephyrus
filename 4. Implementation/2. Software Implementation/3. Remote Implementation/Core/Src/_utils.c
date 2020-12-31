#include "main.h"
#include "_utils.h"


float UTILS_calculateAverage (float* array, uint32_t size) {

	float avrg = 0.0;
	uint32_t it = size;

	while (it-- > 0)		
		avrg += array[it];

	return (avrg / size);
}
