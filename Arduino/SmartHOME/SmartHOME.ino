//Config

#define DHTPIN 9
#define DHTTYPE DHT11

#define WEBSITE "your_site.com"
#define WEBPATH "POST_endpoint_path"
#define APIKEY "your_api_key"

void setup() {
  Serial.begin(9600);

  DHTsetup();
  LANsetup();
}

void loop() {
  Wait(30);

  String temp_hum = GetTemp();
  if (temp_hum != "") {
    sendToServer(temp_hum);
  }
}
