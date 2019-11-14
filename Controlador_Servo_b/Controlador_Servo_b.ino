// Programa a subir en los micros que controlan los motores

#include <Servo.h>
#include <avr/wdt.h>

Servo servoMotor;

#define pinLect 2

#define paso_peq 2
#define paso_grande 30

#define motor 'b'
#define activacion 'o'

float pos;
float t_ini;
float t_act;

String cadena;

void Move_Motor(int, int);

void setup()
{
  // Iniciamos el puerto serie
  Serial.begin(250000);

  // Iniciamos el servo para que empiece a trabajar con el pin 9
  servoMotor.attach(9);

  // Inicializamos al ángulo 0 el servomotor
  servoMotor.write(0);

  // Definición del pin emisor/receptor
  pinMode(pinLect, OUTPUT);
  digitalWrite(pinLect, LOW);

  pinMode(13, OUTPUT);

  MCUSR = 0;

}

void loop()
{
  Secuencia_Inicio();

  t_ini = millis();

  // Vuelve a la posición inicial
  Move_Motor(0, 400);

  // Realiza diferentes trayectorias con diferentes velocidades
  for (int t_pausa = 5; t_pausa <= 20; t_pausa += 5)
  {
    // En sentido positivo de extremo a extremo
    for (int i = 0; i <= 180; i = i + paso_peq)
    {
      Move_Motor(i, t_pausa);
    }

    // En sentido negativo de extremo a extremo
    for (int i = 180; i >= 0; i = i - paso_peq)
    {
      Move_Motor(i, t_pausa);
    }
  }

  // Vuelve a la posición inicial
  Move_Motor(0, 400);

  // Hace parones
  for (int t_pausa = 100; t_pausa <= 220; t_pausa += 40)
  {
    // En sentido positivo
    for (int i = 0; i <= 180; i += paso_grande)
    {
      Move_Motor(i, t_pausa);
    }
    // En sentido negativo
    for (int i = 180; i >= 0; i -= paso_grande)
    {
      Move_Motor(i, t_pausa);
    }
  }
  
  // Escalón en ambos sentidos
  Move_Motor(0, 2000);
  Move_Motor(180, 2000);
  Move_Motor(0, 2000);


  // RS485 como emisor
  digitalWrite(pinLect, LOW);
  /*bool salir = false;

  while (!salir)
  {
    // RS485 como emisor
    digitalWrite(pinLect, LOW);

    if (Serial.available())
    {
      char senal = 'j';
      senal = Serial.read();
      if (senal == activacion)
      {
        salir = true;
        delay(10);
        // RS485 como emisor
        digitalWrite(pinLect, HIGH);
        delay(1);

        // Send the values
        Serial.println("f");
        Serial.flush();

        // RS485 como emisor
        digitalWrite(pinLect, LOW);
      }
    }
  }*/


}


// Move the servomotor
void Move_Motor(int dest, int t_pausa)
{
  // Desplazamos al ángulo correspondiente
  servoMotor.write(dest);

  float tiempo_pausa = millis() - t_ini;
  t_act = millis() - t_ini;

  char senal;

  while (t_act <= (tiempo_pausa + t_pausa))
  {
    // Calculate values
    pos = analogRead(A0);
    pos = pos * 5 / 1023; // Value in voltage
    pos = pos * 180 / 5;
    if (pos > 180)
    {
      pos = 180;
    }
    else if (pos < 0)
    {
      pos = 0;
    }
    t_act = millis() - t_ini;

    if (Serial.available())
    {
      senal = Serial.read();
      if (senal == activacion)
      {
        // RS485 como emisor
        
        digitalWrite(pinLect, HIGH);
        delay(1);

        // Send the values
        cadena = "";
        cadena += motor;
        cadena += ",";
        cadena += t_act;
        cadena += ",";
        cadena += dest;
        cadena += ",";
        cadena += pos;
        cadena += "k";
        
        // Send the values
        wdt_enable(WDTO_15MS);
        Serial.println(cadena);
        Serial.flush();
        wdt_disable();

        // RS485 como emisor
        digitalWrite(pinLect, LOW);
        //delay(1);
      }
    }

  }

}

void Secuencia_Inicio()
{
  // RS485 como receptor
  digitalWrite(pinLect, LOW);

  // Esperamos inicio del programa (desde el maestro)
  unsigned long t_now;
  unsigned long t_ant = 0;
  while (Serial.read() != 'i')
  {

    t_now = millis();
    if (t_now > t_ant + 1000)
    {
      t_ant = t_now;
      if (digitalRead(13))
      {
        digitalWrite(13, LOW);
      }
      else
      {
        digitalWrite(13, HIGH);
      }
    }
  }

}
