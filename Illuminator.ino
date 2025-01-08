#include <LiquidCrystal.h>

int luz;
int modo;
int estadoBoton;
int estadoLeds;
bool encLeds;
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);


class Componente {
public:
    void iniciar() {}
    void escribir() {}
    int leer() {}
};

class Led : public Componente {
public:
    void iniciar(){
      pinMode(8, OUTPUT);
    }

    void escribir(bool option){
        if (option){
        digitalWrite(8, HIGH);
        } else {
          digitalWrite(8, LOW); 
        }
    }
};
  
class Sensor : public Componente {
public:
    int leer() {
      return analogRead(A0);
    }
};

class Pantalla : public Componente {
public:
    void iniciar(){
      lcd.begin(16, 2);
  }
  
    void escribir(int option){
      lcd.clear();
        lcd.setCursor(0, 0);
        switch (option){
      case 0:
            lcd.print("Min");
            break;
            case 1:
              lcd.print("Max");
              break;
            case 2:
              lcd.print("Sensor");
              break;
            case 3:
              lcd.print("Fiesta");
              break;
        }
    }
};

class Boton : public Componente {
public:
    void iniciar(){
      pinMode(9, INPUT);
    }

    int leer() {
      return digitalRead(9);
    }
};


Led leds;
Pantalla pantalla;
Sensor sensor;
Boton boton;

void cambiarModo(){
    modo = ((modo + 1) % 4);
  pantalla.escribir(modo);
}

void setup() {
    Serial.begin(9600);
    leds.iniciar();
    pantalla.iniciar();
    boton.iniciar();
    modo = 0;
    estadoBoton = 0;
    estadoLeds = 0;
    encLeds = false;
}

void loop() {
    if (estadoBoton > 0){
      estadoBoton -= 5;
    } else if (boton.leer() == HIGH){
      estadoBoton = 100;
        cambiarModo();
    }

    if (modo == 2){
      luz = sensor.leer();
        if (luz < 300){
        leds.escribir(true);
        } else {
          leds.escribir(false);
        }
    } else if (modo == 0){
      leds.escribir(false);
    } else if (modo == 1){
      leds.escribir(true);
    } else if (modo == 3){
        if (estadoLeds > 0){
      estadoLeds -= 5;
      } else if (encLeds){
        estadoLeds = 300;
            encLeds = false;
            leds.escribir(false);
        } else {
            estadoLeds = 300;
            encLeds = true;
          leds.escribir(true);
        }
    }

    delay(5);
}
