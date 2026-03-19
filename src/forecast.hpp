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
    vector<double> past_day_precipitation;

    double past_7d_precip_mm = 0.0;      
    vector<double> estimated_flow_m3s = 0.0;  
    
};

WeatherData fetchWeather(double lat, double lon, int forecast_days = 1);


#endif
