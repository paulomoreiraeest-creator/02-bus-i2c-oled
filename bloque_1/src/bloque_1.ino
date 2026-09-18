#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SERIAL_BAUD 115200

#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
#define I2C_CLOCK_SPEED 400000 

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET_PIN -1
#define OLED_I2C_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

// Configuración de los ojos
int radioOjo = 15;
int radioPupila = 6;
int ojoIzquierdoX = 50;
int ojoDerechoX = 88;
int ojoY = 32;

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    Serial.println("\n========================================================");
    Serial.println("  [BLOQUE 1] ESCÁNER DE DIRECCIONES DE HARDWARE I2C     ");
    Serial.println("========================================================");

    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    Wire.setClock(I2C_CLOCK_SPEED);
    Serial.println("[I2C] Bus configurado en SDA:GPIO21, SCL:GPIO22 a 400kHz.\n");
    Serial.println("--- INICIANDO BARRIDO DE DIRECCIONES (0x01 .. 0x7E) ---");

    int devicesFound = 0;
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("[OLED] ERROR: Pantalla no detectada.");
        while (true);
    }
    
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
       
        if (error==0) {
          Serial.printf("[I2C] Dispositivo detectado en: 0x%02X ", address);
          if (address == OLED_I2C_ADDR) {
            Serial.println("➔ [Display OLED SSD1306] [OK]");
          } else {
            Serial.println("➔ [Periférico Desconocido]");
          }
          devicesFound++;
        }
    }

    if (devicesFound == 0) {
        Serial.println("[I2C] ❌ No se detectaron dispositivos en el bus.");
        Serial.println("[DIAGNÓSTICO] Revisa que SDA esté en GPIO21, SCL en GPIO22, 3V3 y GND.");
    } else {
        Serial.printf("\n--- BARRIDO FINALIZADO: %d dispositivo(s) encontrado(s) ---\n", devicesFound);
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.display();
    
    
    
}

void loop() {
    // 1. Mirar al frente por 2 segundos
  dibujarOjos(0, 0); 
  delay(2000);

  // 2. Parpadear
  parpadeo();
  
  // 3. Mirar a la derecha por 1.5 segundos
  dibujarOjos(5, 0); 
  delay(1500);

  // 4. Mirar a la izquierda por 1.5 segundos
  dibujarOjos(-5, 0); 
  delay(1500);
  
  // 5. Parpadear de nuevo antes de reiniciar el ciclo
  parpadeo();
}

// Función principal para renderizar los ojos en una posición
void dibujarOjos(int offsetPupilaX, int offsetPupilaY) {
  display.clearDisplay(); // Borra la pantalla antes de dibujar el nuevo cuadro
  int grosor = 2;
  for (int i = 0; i < grosor; i++) {
    // Ojo Izquierdo (un círculo por cada píxel de grosor)
    display.drawCircle(ojoIzquierdoX, ojoY, radioOjo - i, SSD1306_WHITE);
    
    // Ojo Derecho
    display.drawCircle(ojoDerechoX, ojoY, radioOjo - i, SSD1306_WHITE);
  }
  // Ojo Izquierdo (Contorno y Pupila)
  display.drawCircle(ojoIzquierdoX, ojoY, radioOjo, SSD1306_WHITE);
  display.fillCircle(ojoIzquierdoX + offsetPupilaX, ojoY + offsetPupilaY, radioPupila, SSD1306_WHITE);

  // Ojo Derecho (Contorno y Pupila)
  display.drawCircle(ojoDerechoX, ojoY, radioOjo, SSD1306_WHITE);
  display.fillCircle(ojoDerechoX + offsetPupilaX, ojoY + offsetPupilaY, radioPupila, SSD1306_WHITE);

  display.display(); // Aplica los cambios en la pantalla
}
// Función que recrea el efecto de cerrar y abrir los ojos
void parpadeo() {
  // Ojos cerrándose (se dibujan líneas horizontales)
  display.clearDisplay();
  display.drawFastHLine(ojoIzquierdoX - radioOjo, ojoY, radioOjo * 2, SSD1306_WHITE);
  display.drawFastHLine(ojoDerechoX - radioOjo, ojoY, radioOjo * 2, SSD1306_WHITE);
  display.display();
  delay(150); // Tiempo que duran cerrados

  // Regresa a mirar al frente (se vuelven a abrir)
  dibujarOjos(0, 0);
}