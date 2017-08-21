const int analog_pins[] = {A0, A1, A2, A3, A4};
const int threshold = 25;  // threshold value to decide when the detected sound is a knock or not
const int boton1 = 2;
const int boton2 =  3;

// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int buttonState1 = 0;       // botones para pedales o modificadores
int buttonState2 = 0;
int cooldown[6] = {0, 0, 0, 0, 0, 0};  // cuantos iteraciones quedan para volver a leer el sensor
bool noteIsOn[5] = {false};
bool prev1 = 0;
bool prev2 = 0;
bool modificador1 = false;
bool modificador2 = false;
int vel;
int pico;

void setup() {
  Serial.begin(115200);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
}

void loop() {
  for (int i = 0; i <= 4; i++) {
    vel = maximo(i) * 0.124;

    //BOTONES
    buttonState1 = digitalRead(boton1);
    buttonState2 = digitalRead(boton2);
    if (buttonState1 == 1 && prev1 == 0) {
      modificador1 = true;
    }
    if (buttonState1 == 0 && prev1 == 1) {      //este pedal seria del platillo, que modifica su sonido al abrirse, y suena al soltarse
      modificador1 = false;
      //notaON(6, 100);
    }
    if (buttonState2 == 1 && prev2 == 0) {
      notaON(7, 100);
    }
    if (buttonState2 == 0 && prev2 == 1) {

    }


    //SENSORES
    if (cooldown[i] == 0) {
      if (vel > threshold && i == 4) {
        notaON(i, 100);
        cooldown[i] = 90;                                               //cooldown modificado para sonido especifico
      }
      else if (vel > threshold && i == 3 && modificador1 == true) {
        notaON(i + 2, 100);                                             //indice modificado por el pedal, la nota se corre 5 medio tonos para arriba
        cooldown[i] = 60;                                           //cooldown modificado para sonido especifico
      }
      else if (vel > threshold) {
        notaON(i, 100);
        cooldown[i] = 50;
      }
    } else {
      cooldown[i]--;
    }
    prev1 = buttonState1;
    prev2 = buttonState2;
  }

}

void notaON(int i, int velocidad) {
  Serial.write(153);
  Serial.write(i + 60);
  Serial.write(velocidad);
  noteIsOn[i] = true;
}

int maximo(int i) {
  int lectura = analogRead(analog_pins[i]);
  pico = 0;
  do {
    if (pico < lectura) {
      pico = lectura;
    };
    lectura = analogRead(analog_pins[i]);
  } while (sensorReading > pico );
  return (pico);
}


