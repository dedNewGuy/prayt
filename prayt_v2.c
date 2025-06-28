#include <stdio.h>
#include <math.h>

#define DAYS_SINCE_JAN 178
#define YEAR 2025

#define KL_LAT 3.13900300
#define KL_LONG 101.68685500
#define KL_TZ 8

#define to_degrees(radians)			\
	radians * (180.0 / M_PI)

#define to_radians(degrees)			\
    degrees * M_PI / 180.0

#define log(format, name, __VA_ARGS__)		\
    printf("[LOG] %s = ", name);		\
    printf(format, __VA_ARGS__);		\
    printf("\n");				\

#define m_to_h(m)				\
    m / 60;					\

double wiki_eq_of_time(int year, int days_since_jan);
double sun_decline(int days_since_jan);
double sunrise_eqt(double angle, double latitude, double sun_dec);
double asr_eqt();

typedef struct {
    int hours;
    int minutes;
} metime_t;

metime_t to_civil_time(double hours);

double zuhur(double longitude, double eq_of_time, int timeZ);

int main()
{

    double wiki_eqT = wiki_eq_of_time(YEAR, DAYS_SINCE_JAN);
    log("%f", "WIKI eqT", wiki_eqT);
    
    double sunD = sun_decline(DAYS_SINCE_JAN);
    log("%f", "SUN DEC", sunD);
    
    // Calculate dhuhr
    double zuhur_time = zuhur(KL_LONG, wiki_eqT, KL_TZ);
    log("%f", "ZUHUR_TIME", zuhur_time);
    metime_t zuhur = to_civil_time(zuhur_time);

    // Calculate fajr
    double fajr_angle = sunrise_eqt(17, KL_LAT, sunD);
    double fajr_time = zuhur_time - fajr_angle;
    log("%f", "FAJR_TIME", fajr_time);    
    metime_t fajr = to_civil_time(fajr_time);

    // Calculate Isha
    double isha_angle = sunrise_eqt(18, KL_LAT, sunD);
    double isha_time = zuhur_time + isha_angle;
    log("%f", "ISHA_TIME", isha_time);
    metime_t isha = to_civil_time(isha_time);

    printf("------ PRAYER TIME -------\n");
    printf("Zuhur: %d:%d\n", zuhur.hours, zuhur.minutes);
    printf("Fajr: %d:%d\n", fajr.hours, fajr.minutes);
    printf("Isha: %d:%d\n", isha.hours, isha.minutes);    

    
    return 0;
}

// Return value is in hours. Original formula return in minutes
double wiki_eq_of_time(int year, int days_since_jan)
{
    double D = 6.24004077 + 0.01720197 * (365.25 * (year - 2000) + days_since_jan);
    double eq_time = -7.659 * to_degrees(sin(to_radians(D)))
	+ 9.863 * to_degrees(sin(to_radians(2 * D + 3.5932)));
    return m_to_h(eq_time);
}

/* double sun_decline(int days_since_jan) */
/* { */
/*     double cos_v = to_radians((360 * (days_since_jan + 10)) / 365); */
/*     double sin_v = -23.45; */
/*     log("%f", "COS_V", cos_v); */
/*     log("%f", "SIN_V", sin_v); */
/*     return sin_v * cos(cos_v); */
/* } */

double sun_decline(int days_since_jan)
{
    double B = to_radians((360.0 / 365.0) *
			  (days_since_jan - 81)); // 81 ≈ March 21 (equinox reference)
    return 23.45 * sin(B);
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

double zuhur(double longitude, double eq_of_time, int timeZ)
{
    return 12.0 + timeZ - (longitude / 15) - eq_of_time / 60;
}

double sunrise_eqt(double angle, double latitude, double sun_dec)
{
    double angle_rad = sin(to_radians(angle));
    log("%f", "ANGLE_RAD", angle_rad);
    double lat_rad = sin(to_radians(latitude));
    log("%f", "LAT_RAD", lat_rad);
    double sun_rad = sin(to_radians(sun_dec));
    log("%f", "SUN_RAD", sun_rad);
    double lat_cos_rad = cos(to_radians(latitude));
    log("%f", "LAT_COS", lat_cos_rad);
    double sun_cos_rad = cos(to_radians(sun_dec));
    log("%f", "SUN_COS", sun_cos_rad);

    double x = -angle_rad - lat_rad * sun_rad;
    log("%f", "X", x);
    double y = lat_cos_rad * sun_cos_rad;
    log("%f", "Y", y);

    double x_div_y = x / y;
    log("%f", "X / Y", x_div_y);

    double acosit = acos(x_div_y);
    log("%f", "ACOSIT", acosit);

    double degreesIt = to_degrees(acosit);
    log("%f", "ACOSIT DEG", degreesIt);    
    
    return degreesIt / 15.0;

}
