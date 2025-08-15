#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Paramètres du point d'accès (AP) */
const char* ssid = "ESP8266-LM35";  // Nom du réseau Wi-Fi
const char* password = "12345678"; // Mot de passe du réseau Wi-Fi (optionnel)

/* Serveur HTTP sur le port 80 */
ESP8266WebServer server(80);

/* Broche pour le capteur LM35 */
const int LM35_PIN = A0;
float temperature = 0.0;

void setup() {
  Serial.begin(115200);

  // Initialisation du point d'accès Wi-Fi
  WiFi.softAP(ssid, password);  // Démarre l'ESP8266 en mode point d'accès avec SSID et mot de passe
  Serial.println("Point d'accès lancé");
  Serial.print("Adresse IP du point d'accès : ");
  Serial.println(WiFi.softAPIP());  // Affiche l'adresse IP locale du point d'accès

  // Définition de la route du serveur pour la page d'accueil
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  // Démarrer le serveur
  server.begin();
  Serial.println("Serveur HTTP démarré");
}

void loop() {
  temperature = lireTemperature();  // Lire la température du capteur LM35
  server.handleClient();  // Gère les requêtes du serveur
}

// Fonction pour lire la température du LM35
float lireTemperature() {
  int analogValue = analogRead(LM35_PIN); // Lecture de la valeur analogique du capteur
  float voltage = analogValue * (3.3 / 1023.0); // Conversion en tension (ESP8266 fonctionne à 3.3V)
  float tempC = voltage * 100.0; // LM35 donne 10 mV par °C (1V = 100°C)
  return tempC;
}

// Fonction pour gérer la connexion à la page web
void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(temperature));
}

// Fonction pour gérer les requêtes non trouvées (page introuvable)
void handle_NotFound() {
  server.send(404, "text/plain", "Page non trouvée");
}

// Générer le HTML à envoyer au client (page web)
String SendHTML(float temp) {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Température LM35</title>";
  html += "<style>body{font-family:Arial;text-align:center;padding-top:50px;}</style>";
  html += "</head><body>";
  html += "<h1>Température Actuelle</h1>";
  html += "<h2>" + String(temp, 2) + " &deg;C</h2>";
  html += "<p>Actualiser la page pour mettre à jour</p>";
  html += "</body></html>";
  return html;
}
