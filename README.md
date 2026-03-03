# 🌤️ WeatherLib - Libreria Meteo per Energia Rinnovabile

Libreria C++ per interrogare l'API **Open-Meteo** e calcolare **pesi energetici** per solare e vento.

## 📦 Struttura progetto


## 🚀 Primo avvio (Quick Start)

### 1. Installa dipendenze (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential libssl-dev
# Il tuo progetto
g++ -std=c++17 mio_progetto.cpp forecast.cpp -lssl -lcrypto -o mio_app
