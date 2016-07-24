#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

void updateWeatherInfo(void);
void parseWeatherInfo(String payload);

//Definir o SSID da rede WiFi
const char* ssid = "SMART";
//Definir a senha da rede WiFi
const char* password = "redeiot123";

ESP8266WiFiMulti WiFiMulti;

#define JSON_BUFFER 2048
//Definir a chave do site openweathermap.org
char id[] = "4590cd396eda3b53dda0643b4c09f1e8";
char city[] = "SaoPaulo";

void setup() {
  Serial.begin(115200);
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  updateWeatherInfo();
  ESP.deepSleep(10 * 1000000);

}

void loop() {
}

void parseWeatherInfo(String payload) {
  StaticJsonBuffer<JSON_BUFFER> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(payload);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  double temp = root["main"]["temp"];
  int humd = root["main"]["humidity"];
  double temp_m = root["main"]["temp_min"];
  double temp_p = root["main"]["temp_max"];
  int press = root["main"]["pressure"];

  Serial.print("Temp: ");
  Serial.println(temp - 273.15, 2);
  Serial.printf("Umidade: %d%\n", humd);
  Serial.printf("Temperatura Maxima: ");
  Serial.println(temp_p - 273.15, 2);
  Serial.printf("Temperatura Minima: ");
  Serial.println(temp_m - 273.15, 2);
  Serial.printf("Pressao Atm: %d\n", press);
}

void updateWeatherInfo(void) {
  HTTPClient http;

  String httpAsk = "http://api.openweathermap.org/data/2.5/weather?q=";
  httpAsk += city;
  httpAsk += ",uk&id=524901&APPID=";
  httpAsk += id;

  http.begin(httpAsk);

  int httpCode = http.GET();

  if(httpCode > 0) {
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      //Serial.println(payload);
      parseWeatherInfo(payload);
    }
  } else {
    Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}
