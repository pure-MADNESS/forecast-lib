//#include "httplib.h"
//#include "json.hpp"

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

double SolarWheight(double cloud_cover, double direct_normal_irradiance) {
    // Soglie reali FV:
    // DNI < 100 W/m² = produzione trascurabile
    // Nuvole > 80% = quasi zero anche con sole
    
    double dni_factor = min(1.0, max(0.0, direct_normal_irradiance / 1000.0));  // 0-1000 W/m²
    double cloud_factor = max(0.0, (100.0 - cloud_cover) / 100.0);  // 0-1
    
    return dni_factor * cloud_factor;  // Prodotto = entrambi importanti

    // NB: Questo peso non è ancpora normalizzato a 0-1.
}

double WindWheight(double wind_speed_10m) {
    // Semplice formula: più vento = più peso

    if (wind_speed_10m < 5.0) return 0.0; // vento troppo debole per essere utile (CUT-IN)
    if (wind_speed_10m > 50.0) return 0.0; // vento troppo forte, rischio danni (CUT-OUT)

    return wind_speed_10m / 50.0; // Normalizza a 0-1
}

double PrecipitationWheight(double precipitation) {
    if (precipitation < 0.5) return 0.3;      // Portata minima
    if (precipitation < 5.0) return 0.8;      // Buon flusso  
    if (precipitation < 20.0) return 1.0;     // Ottimale
    return 0.9;                           // >20mm = rischio piena


    // Fatto cosi è abba stupido, ma è un inizio.
    // Per farlo meglio servirebbe un modello più complesso che tenga conto di:
    // - Durata e intensità (es. 1mm/h vs 10mm/h)
    // - Giorni precendenti 
    // Quindi una " running average" che tiene conto dei giorni pasati, non solo dell'istante attuale.
}   