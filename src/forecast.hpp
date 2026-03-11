#ifndef __FORECAST_HPP__
#define __FORECAST_HPP__

// #define CPPHTTPLIB_OPENSSL_SUPPORT

#include <httplib.h>
#include <nlohmann/json.hpp>
#include<vector>

using namespace std;

struct WeatherData{

    vector<double> temperature_2m;
    vector<double> wind_speed_10m;
    vector<double> cloud_cover;
    vector<double> direct_normal_irradiance;
    vector<double> precipitation;
};

WeatherData fetchWeather(double lat, double lon, int forecast_days = 1);

double SolarWheight(double cloud_cover, double direct_normal_irradiance);
double WindWheight(double wind_speed_10m);
double PrecipitationWheight(double precipitation);

#endif
