#include <math.h>
#include <stdint.h>
#include <stdio.h>

void doubleToDegrees(double inValue, int16_t *grad, uint8_t * min, uint8_t *sec)
{
	double dblMin, dblSec;
	int sign = (inValue < 0) ? -1:1;

	*grad = (uint16_t)floor(inValue*sign);

	dblMin  = (inValue - floor(inValue)) * 60;
	dblSec  = (dblMin - floor(dblMin)) * 60;
	
	*grad = (uint16_t)*grad*sign;
	*min  = (uint8_t) floor(dblMin);
	*sec  = (uint8_t) floor(dblSec);
	
	printf("inValue=%f\n", inValue);
	printf(" %+.3d grad, %.2d min, %.2d sec\n", *grad, *min, *sec);

}
