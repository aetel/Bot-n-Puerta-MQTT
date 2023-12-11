#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configura tus credenciales WiFi y MQTT
const char *ssid = "TuSSID";
const char *password = "TuClaveWiFi";
const char *mqtt_server = "broker.mqtt.com";  // Reemplaza con la dirección IP de tu servidor MQTT

const char *mqtt_topic = "cmnd/PUERTA/POWER";

int buttonState = 0;
int lastButtonState = 0;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(D0, INPUT); // Reemplaza con el pin al que conectaste el botón
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      // Botón presionado, enviar mensaje MQTT
      sendMqttMessage("1");
    }
    delay(50);  // Retardo para evitar rebotes del botón
  }

  lastButtonState = buttonState;
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado a WiFi");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void sendMqttMessage(const char *message) {
  Serial.println("Enviando mensaje MQTT...");
  if (client.publish(mqtt_topic, message)) {
    Serial.println("Mensaje enviado correctamente");
  } else {
    Serial.println("Error al enviar el mensaje");
  }
}
