#include <stdio.h>
#include <math.h>

float date_to_jd(int year, int month, int day);

int main()
{

	float jd = date_to_jd(1970, 1, 1);
	printf("JD: %f\n", jd);
	return 0;
}

/**
 * parameters - year, month, day, universal time in hours
 * return - gregorian date converted to julian date
 */
float date_to_jd(int year, int month, int day)
{
    float jd = (int)367*year-(7*(year+(month+9)/12))/4+(275*month)/9+day+1721013.5f+12/24;
    return roundf(jd);
}
