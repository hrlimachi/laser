
#include "SD_lib.h"

void setup() {
  Serial.begin(115200);
  
  if (!SD.begin()) {
    Serial.println("Error al inicializar la tarjeta SD.");
    return;
  }

  int fileCount = 0;
  File root = SD.open("/");  // Abre la raíz de la tarjeta SD

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      // No hay más archivos en la raíz
      break;
    }
    // Incrementar el contador de archivos
    fileCount++;
    entry.close();
  }

  Serial.print("Número de archivos en la tarjeta SD: ");
  Serial.println(fileCount);
}

void loop() {
  // Tu código principal
}
