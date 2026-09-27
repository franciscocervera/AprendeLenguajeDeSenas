#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

constexpr int SERVOMIN = 125;
constexpr int SERVOMAX = 575;
constexpr unsigned long TIEMPO_LETRA = 1000;

int inicioPulgar = 0;
int inicioIndice = 0;
int inicioMedio = 0;
int inicioAnular = 0;
int inicioMenique = 0;

bool letraActiva = false;
unsigned long tiempoInicioLetra = 0;

int anguloPWM(int angulo) {
  return map(constrain(angulo, 0, 180), 0, 180, SERVOMIN, SERVOMAX);
}

void posicionInicial() {
  pca.setPWM(0, 0, anguloPWM(inicioPulgar));
  pca.setPWM(1, 0, anguloPWM(inicioIndice));
  pca.setPWM(2, 0, anguloPWM(inicioMedio));
  pca.setPWM(3, 0, anguloPWM(inicioAnular));
  pca.setPWM(4, 0, anguloPWM(inicioMenique));
  letraActiva = false;
}

void moverMano(int pulgar, int indice, int medio, int anular, int menique) {
  pca.setPWM(0, 0, anguloPWM(pulgar));
  pca.setPWM(1, 0, anguloPWM(indice));
  pca.setPWM(2, 0, anguloPWM(medio));
  pca.setPWM(3, 0, anguloPWM(anular));
  pca.setPWM(4, 0, anguloPWM(menique));
  tiempoInicioLetra = millis();
  letraActiva = true;
}

void letraA() { moverMano(0, 180, 180, 180, 180); }
void letraB() { moverMano(180, 0, 0, 0, 0); }
void letraC() { moverMano(90, 90, 90, 90, 90); }
void letraD() { moverMano(40, 0, 90, 180, 180); }
void letraE() { moverMano(140, 140, 140, 140, 140); }
void letraF() { moverMano(90, 90, 0, 0, 0); }
void letraG() { moverMano(0, 0, 180, 180, 180); }
void letraH() { moverMano(0, 0, 0, 180, 180); }
void letraI() { moverMano(180, 180, 180, 180, 0); }
void letraJ() { moverMano(180, 180, 180, 180, 0); }
void letraK() { moverMano(0, 40, 90, 180, 180); }
void letraL() { moverMano(0, 0, 180, 180, 180); }
void letraM() { moverMano(180, 0, 0, 0, 180); }
void letraN() { moverMano(180, 0, 0, 180, 180); }
void letraO() { moverMano(90, 90, 90, 90, 90); }
void letraP() { moverMano(180, 0, 90, 180, 180); }
void letraQ() { moverMano(100, 0, 180, 180, 180); }
void letraR() { moverMano(180, 0, 0, 180, 180); }
void letraS() { moverMano(180, 180, 180, 180, 180); }
void letraT() { moverMano(0, 180, 180, 180, 180); }
void letraU() { moverMano(180, 0, 0, 180, 180); }
void letraV() { moverMano(180, 0, 0, 180, 180); }
void letraW() { moverMano(180, 0, 0, 0, 180); }
void letraX() { moverMano(140, 100, 180, 180, 180); }
void letraY() { moverMano(0, 180, 180, 180, 0); }
void letraZ() { moverMano(180, 0, 180, 180, 180); }

void ejecutarComando(char comando) {
  switch (comando) {
    case 'A': letraA(); break;
    case 'B': letraB(); break;
    case 'C': letraC(); break;
    case 'D': letraD(); break;
    case 'E': letraE(); break;
    case 'F': letraF(); break;
    case 'G': letraG(); break;
    case 'H': letraH(); break;
    case 'I': letraI(); break;
    case 'J': letraJ(); break;
    case 'K': letraK(); break;
    case 'L': letraL(); break;
    case 'M': letraM(); break;
    case 'N': letraN(); break;
    case 'O': letraO(); break;
    case 'P': letraP(); break;
    case 'Q': letraQ(); break;
    case 'R': letraR(); break;
    case 'S': letraS(); break;
    case 'T': letraT(); break;
    case 'U': letraU(); break;
    case 'V': letraV(); break;
    case 'W': letraW(); break;
    case 'X': letraX(); break;
    case 'Y': letraY(); break;
    case 'Z': letraZ(); break;
    case '0': posicionInicial(); break;
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_Mano");
  Wire.begin(21, 22);
  pca.begin();
  pca.setPWMFreq(50);
  delay(100);
  posicionInicial();
}

void loop() {
  while (SerialBT.available()) {
    char comando = SerialBT.read();

    if (comando >= 'a' && comando <= 'z') {
      comando -= 32;
    }

    if ((comando >= 'A' && comando <= 'Z') || comando == '0') {
      ejecutarComando(comando);
    }
  }

  if (letraActiva && millis() - tiempoInicioLetra >= TIEMPO_LETRA) {
    posicionInicial();
  }
}
