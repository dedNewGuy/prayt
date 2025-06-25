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

typedef struct {
    int hours;
    int minutes;
} metime_t;

metime_t to_civil_time(double hours);

/* Get Approx Prayer Time */
double dhuhr(double lon, double eqT, double loc_time_zone);

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

	double zuhur = dhuhr(101.7099999, astro_const.eq_of_time, 8);

	log("%f", "Zuhur", zuhur);

	metime_t metime = to_civil_time(zuhur);

	printf("------ PRAYER TIME -------\n");
	printf("Zuhur: %d:%d\n", metime.hours, metime.minutes);

	return 0;
}

/**
 * parameters - year, month, day, universal time in hours
 * return - gregorian date converted to julian date
 */
float date_to_jd(int year, int month, int day)
{
    float second_last_exp = (100 * year) + month - 190002.5;
    float jd = (int)367*year-(7*(year+(month+9)/12))/4+(275*month)/9+day+1721013.5f+12/24
	- 0.5 * (second_last_exp / second_last_exp) + 0.5;
    return jd;
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
	
	double g = fmod(357.529 + 0.98560028 * D, 360);
	double q = fmod(280.459 + 0.98564736 * D, 360);

	double L = fmod(q + 1.915 * to_degrees(sin(to_radians(g))) + 0.020 * to_degrees(sin(to_radians(2 * g))), 360);

	log("%f", "g", g);
	log("%f", "q", q);
	log("%f", "L", L);
	
	double e = fmod(23.439 - 0.00000036 * D, 360);

	log("%f", "e", e);

	double kos_e = cos(to_radians(e));
	double sin_e = sin(to_radians(e));
	double sin_l = sin(to_radians(L));
	double kos_l = cos(to_radians(L));
	double sun_right_ascen = to_degrees(atan2(kos_e * sin_l, kos_l));
	double declination = to_degrees(asin(sin_e * sin_l));

	log("%f", "RA BFORE DIV", sun_right_ascen);

	double ra_in_hours = fmod(sun_right_ascen / 15, 24);

	log("%f", "RA IN HOURS", ra_in_hours);
	
	double eqt_of_time = q / 15 - ra_in_hours;

	astro_const.sun_declination = declination;
	astro_const.eq_of_time = eqt_of_time;

	return astro_const;
}

double dhuhr(double lon, double eqT, double loc_time_zone)
{
    return 12.0 + loc_time_zone - (lon / 15) - (eqT / 60);
}

metime_t to_civil_time(double hours)
{
    metime_t metime;

    int le_hours = (int)hours;

    log("%d", "HOURS", le_hours);

    double le_rest = hours - (double)le_hours;
    double minutes = le_rest * 60;
    log("%f", "MINUTE", minutes);

    metime.hours = le_hours;
    metime.minutes = minutes;

    return metime;
}
