#include <stdio.h>
#include <math.h>

#define to_degrees(radians)			\
	radians * (180.0 / M_PI)

#define to_radians(degrees)			\
    degrees * M_PI / 180.0

#define log(format, name, __VA_ARGS__)		\
    printf("[LOG] %s = ", name);		\
    printf(format, __VA_ARGS__);		\
    printf("\n");				\

typedef struct {
	double eq_of_time;
	double sun_declination;
} astro_const_t;

float date_to_jd(int year, int month, int day);
astro_const_t init_astro_constant(double jd);


int main()
{

	float jd = date_to_jd(2025, 6, 25);
	log("%f", "JD", jd);

	/* Just testing to see if macros works */
	/* log("%f", "TEST", to_degrees(M_PI / 2)); */
	/* log("%f", "TEST", to_radians(90.0)); */

	astro_const_t astro_const = init_astro_constant((double)jd);

	log("%f", "EqT", astro_const.eq_of_time);
	log("%f", "Sun Decline", astro_const.sun_declination);

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


astro_const_t init_astro_constant(double jd)
{


	astro_const_t astro_const;

	/* the number of days and fraction (+ or –) from the epoch referred to as
	 * "J2000.0", which is 2000 January 1.5, Julian date 2451545.0 
	 * D = JD – 2451545.0
	 * */
	double D = jd - 2451545.0; 

	log("%f", "D", D);
	
	double g = 357.529 + 0.98560028 * D;
	double q = 280.459 + 0.98564736 * D;
	double L = q + 1.915
		* to_degrees(sin(g)) + 0.020 * to_degrees(sin(2 * g));

	log("%f", "g", g);
	log("%f", "q", q);
	log("%f", "L", L);
	
	double e = 23.439 - 0.00000036 * D;

	log("%f", "e", e);

	double kos_e = cos(e);
	double sin_e = sin(e);
	double sin_l = sin(L);
	double kos_l = cos(L);
	double sun_right_ascen = to_degrees(atan2(kos_e * sin_l, kos_l));
	double declination = to_degrees(asin(sin_e * sin_l));

	double ra_in_hours = sun_right_ascen / 15;
	double eqt_of_time = q / 15 - ra_in_hours;

	astro_const.sun_declination = declination;
	astro_const.eq_of_time = eqt_of_time;

	return astro_const;
}

