#include <iostream>
#include <iomanip>
#include <vector>
#include "forecast.hpp"

using namespace std;

// Funzione helper per stampare vettori
template<typename T>
void printVector(const string& name, const vector<T>& vec) {
    cout << "\n" << name << " (" << vec.size() << " valori): ";
    if (vec.empty()) {
        cout << "VUOTO";
    } else {
        for (size_t i = 0; i < min(10ul, vec.size()); ++i) {
            cout << fixed << setprecision(1) << vec[i];
            if (i < vec.size() - 1) cout << ", ";
        }
        if (vec.size() > 10) cout << "... (" << vec.size() - 10 << " altri)";
    }
    cout << endl;
}

int main() {
    try {
        cout << "🌤️ Fetch meteo Trento (46.0691, 11.1212)..." << endl;
        auto data = fetchWeather(46.0691, 11.1212, 1);
        
        printVector("Temperature 2m (°C)", data.temperature_2m);
        printVector("Wind speed 10m (km/h)", data.wind_speed_10m);
        printVector("Cloud cover (%)", data.cloud_cover);
        printVector("Direct Normal Irradiance (W/m²)", data.direct_normal_irradiance);
        printVector("Precipitation (mm)", data.precipitation);
       
    } catch (const exception& e) {
        cerr << "❌ Errore: " << e.what() << endl;
    }
    
    cout << "\n\n✅ Controllo completato!" << endl;
    return 0;
}
