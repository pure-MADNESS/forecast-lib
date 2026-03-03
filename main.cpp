#include <iostream>
#include "forecast.hpp"

using namespace std;

int main() {
    try {
        auto data = fetchWeather(46.0691, 11.1212, 1); // Trento
        cout << "First temp = " << data.temperature_2m.front() << " °C" << endl;
        cout << "First wind speed = " << data.wind_speed_10m.front() << " km/h" << endl;
        cout << "First cloud cover = " << data.cloud_cover.front() << " %" << endl;
        cout << "First direct normal irradiance = " << data.direct_normal_irradiance.front() << " W/m²" << endl;
        cout << "First precipitation = " << data.precipitation.front() << " mm" << endl;
    } catch (const exception& e) {
        cerr << "Errore: " << e.what() << endl;
    }
}
