//#include "httplib.h"
//#include "json.hpp"

#include "forecast.hpp"
#include<vector>
#include<cmath>
#include<algorithm>

using json = nlohmann::json;
using namespace std;


struct BasinParameters {
    double area_km2 = 170.0; // area in km² del Fersina
    double C_secco = 0.2; // coeff di runoff per terreno secco
    double C_bagnato = 0.7; // coeff di runoff per terreno bagnato
    double Q_base_m3s = 1.5; // flusso de fiume a secco
    int tc_h = 6; // tempo di concentraz.
};



double EstimatedFlow(double <vector>precipitation_mm,double precipitation_7d_mm,const BasinParameters& params) {

double wetness_f = min(1.0, precipitation_7d_mm / 100.0); // più piove nei giorni precedenti, più il terreno è bagnato
double C_eff = params.C_secco + (params.C_bagnato - params.C_secco) * wetness_f; // interpolazione lineare

double P_eff = 0.0;
int n_hours = min(24, params.tc_h); // consideriamo solo le ultime tc_h ore
for(int i = 0; i < n_hours; ++i) {
    P_eff += precipitation_mm[i]; // semplificazione: tutta la pioggia contribuisce, in realtà dovrebbe essere decrescente nel tempo
}
P_eff /= static_cast<double>(n_hours); // media oraria

    double Q_runoff = (C_eff * P_eff * params.area_km2 * 1e6) / 3.6e6;
    
    return params.Q_base_m3s + Q_runoff;

}



double getPast7DaysPrecipSum(const std::vector<double>& daily_precip_sum) {
    if (daily_precip_sum.empty()) return 0.0;
    double sum = 0.0;
    size_t n = daily_precip_sum.size();
    size_t start = std::max(0, static_cast<int>(n) - 8);  // 7 gg prima di ieri
    size_t end = std::max(0, static_cast<int>(n) - 1);    // fino a ieri
    for (size_t i = start; i < end; ++i) {
        sum += daily_precip_sum[i];
    }
    return sum;
}





WeatherData fetchWeather(double lat, double lon, int forecast_days){
    WeatherData data;

    httplib::Client cli("http://api.open-meteo.com");
    // GIUSTO ✅
string url = "/v1/forecast?latitude=" + to_string(lat) + "&longitude=" + to_string(lon) + 
             "&hourly=temperature_2m,wind_speed_10m,cloud_cover,direct_normal_irradiance,precipitation&forecast_days=" + "&daily=precipitation_sum&" +"past_days=7"+"forecast_days=" + to_string(forecast_days);
auto res = cli.Get(url.c_str());

    if (res && res->status == 200) {
        auto json_data = nlohmann::json::parse(res->body);
        data.temperature_2m = json_data["hourly"]["temperature_2m"].get<vector<double>>();
        data.wind_speed_10m = json_data["hourly"]["wind_speed_10m"].get<vector<double>>();
        data.cloud_cover = json_data["hourly"]["cloud_cover"].get<vector<double>>();
        data.direct_normal_irradiance = json_data["hourly"]["direct_normal_irradiance"].get<vector<double>>();
        data.precipitation = json_data["hourly"]["precipitation"].get<vector<double>>();
        data.past_day_precipitation = json_data["daily"]["precipitation_sum"].get<vector<double>>(); 



        data.past_7d_precip_mm = getPast7DaysPrecipSum(data.past_day_precipitation);
        data.estimated_flow_m3s = EstimatedFlow(data.precipitation, data.past_7d_precip_mm, BasinParameters());



    } else {
        throw runtime_error("Failed to fetch weather data");
    }

    return data;
}


