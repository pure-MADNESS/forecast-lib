#pragma once 

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