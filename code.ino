#include <ESP8266WiFi.h>

void mostrarInfoRede() {
  Serial.println();
  Serial.println("===== CONECTADO =====");
  Serial.print("SSID (nome da rede): "); Serial.println(WiFi.SSID());
  Serial.print("BSSID (MAC do roteador): "); Serial.println(WiFi.BSSIDstr());
  Serial.print("Canal: "); Serial.println(WiFi.channel());
  Serial.print("Sinal (RSSI): "); Serial.print(WiFi.RSSI()); Serial.println(" dBm");
  Serial.print("IP do ESP: "); Serial.println(WiFi.localIP());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("Máscara: "); Serial.println(WiFi.subnetMask());
  Serial.print("DNS: "); Serial.println(WiFi.dnsIP());
  Serial.print("MAC do ESP: "); Serial.println(WiFi.macAddress());
  Serial.print("Hostname: "); Serial.println(WiFi.hostname());
  Serial.println("=====================");
}

void listarRedesProximas() {
  Serial.println();
  Serial.println("Procurando redes próximas...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("Nenhuma rede encontrada.");
    return;
  }
  for (int i = 0; i < n; i++) {
    Serial.printf("%2d: %-32s  %4d dBm  canal %2d  %s\n",
                  i + 1,
                  WiFi.SSID(i).c_str(),
                  WiFi.RSSI(i),
                  WiFi.channel(i),
                  WiFi.encryptionType(i) == ENC_TYPE_NONE ? "aberta" : "protegida");
  }
  WiFi.scanDelete();
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("Iniciando...");

  WiFi.mode(WIFI_STA);

  // Mostra a rede salva na flash (mesmo antes de conectar)
  Serial.print("Rede salva na memória: ");
  String salva = WiFi.SSID();
  Serial.println(salva.length() ? salva : "(nenhuma)");

  // Tenta conectar com as credenciais salvas
  WiFi.begin();

  Serial.print("Conectando");
  unsigned long inicio = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - inicio < 30000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    mostrarInfoRede();
  } else {
    Serial.println("Não está conectado a nenhuma rede.");
    listarRedesProximas();
  }
}

void loop() {
  // A cada 10 s mostra o status atual
  static unsigned long ultimo = 0;
  if (millis() - ultimo > 10000) {
    ultimo = millis();
    if (WiFi.status() == WL_CONNECTED) {
      Serial.printf("Conectado em \"%s\" | RSSI %d dBm | IP %s\n",
                    WiFi.SSID().c_str(), WiFi.RSSI(),
                    WiFi.localIP().toString().c_str());
    } else {
      Serial.println("Desconectado.");
    }
  }
}
