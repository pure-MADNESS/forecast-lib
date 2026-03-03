#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include "json.hpp"

#include "forecast.hpp"

using json = nlohmann::json;

WeatherData fetchWeather(double lat, double lon, int forecast_days){
    WeatherData data;

    httplib::Client cli("https://api.open-meteo.com");
    // GIUSTO ✅
string url = "/v1/forecast?latitude=" + to_string(lat) + "&longitude=" + to_string(lon) + 
             "&hourly=temperature_2m,wind_speed_10m,cloud_cover,direct_normal_irradiance,precipitation&forecast_days=" + to_string(forecast_days);
auto res = cli.Get(url.c_str());

    if (res && res->status == 200) {
        auto json_data = nlohmann::json::parse(res->body);
        data.temperature_2m = json_data["hourly"]["temperature_2m"].get<vector<double>>();
        data.wind_speed_10m = json_data["hourly"]["wind_speed_10m"].get<vector<double>>();
        data.cloud_cover = json_data["hourly"]["cloud_cover"].get<vector<double>>();
        data.direct_normal_irradiance = json_data["hourly"]["direct_normal_irradiance"].get<vector<double>>();
        data.precipitation = json_data["hourly"]["precipitation"].get<vector<double>>();
    } else {
        throw runtime_error("Failed to fetch weather data");
    }

    return data;
}