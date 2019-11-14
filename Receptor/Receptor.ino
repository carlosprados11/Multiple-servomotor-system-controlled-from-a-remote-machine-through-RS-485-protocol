// Programa a subir en el Mega para la comunicación con Matlab

#define pinLect 2

#define RX_BUFFER_SIZE 100000
#define TX_BUFFER_SIZE 100000

void Secuencia_Inicio();

float t_act;
float pos;
int dest;
int listos;
char tipo;

String string;
String msg;

int ite;
int num_datos;
char borrar;

int counter[4] = {0, 0, 0, 0};

void setup()
{
  // Iniciamos el puerto serie
  Serial.begin(250000);
  Serial1.begin(250000);

  // Definición del pin emisor/receptor
  pinMode(pinLect, OUTPUT);

  pinMode(13, OUTPUT);
}

void loop()
{

  Secuencia_Inicio();

  // RS485 como receptor
  //digitalWrite(pinLect, LOW);

  delay(10);
  while (Serial1.available())
  {
    char borrar = Serial1.read();
  }

  bool salir = false;
  int turno = 1;
  int contador = 0;
  
  while ((!salir)&&(contador<1000))
  {
    delay(8);

    // RS485 como emisor
    digitalWrite(pinLect, HIGH);
    delay(1);

    switch (turno)
    {
      case 1:
        Serial1.println('n');
        break;
      case 2:
        Serial1.println('o');
        break;
      case 3:
        Serial1.println('p');
        break;
      case 4:
        Serial1.println('q');
        break;
    }
    Serial1.flush();

    // RS485 como receptor
    digitalWrite(pinLect, LOW);

    turno++;
    if (turno > 4)
      turno = 1;
    
    if (Serial1.available())
    {
      contador = 0;
      tipo = Serial1.read();

      if (tipo == 'f')
      {
        salir = true;

        // Señal de finalización
        Serial.println("s,0,0,0");

      }
      else if ((tipo == 'a') || (tipo == 'b') || (tipo == 'c') || (tipo == 'd'))
      {

        string = "";
        string = tipo;
        string += Serial1.readStringUntil('k');
        Serial.println(string);
        //Serial.flush();
        
        while (Serial1.available())
        {
          borrar = Serial1.read();
        }

      }
    }
    else
    {
      contador++;
    }
  }

  // Señal de finalización
  Serial.println("s,0,0,0");

}



void Secuencia_Inicio()
{
  // RS485 como receptor
  digitalWrite(pinLect, LOW);

  // Esperamos inicio del programa (desde el ordenador)
  unsigned long t_now;
  unsigned long t_ant = 0;
  while (Serial.read() != 'g')
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

  // RS485 como emisor
  digitalWrite(pinLect, HIGH);

  // Ponemos a Matlab en espera
  Serial.println("e");

  listos = 0;

  /*while (listos < 1)
    {*/
  // RS485 como emisor
  digitalWrite(pinLect, HIGH);

  // Enviamos comando de inicialización a los controladores
  for (int i = 1; i <= 5; i++)
    Serial1.println("i");

  // RS485 como receptor
  digitalWrite(pinLect, LOW);

  // Esperamos confirmación de los 4 micros
  /*while (Serial1.read() != 'r')
    {
    }
    listos++;*/

  // TODOS LOS RECEPTORES ESTÁN CONECTADOS
  // RS485 como emisor
  //digitalWrite(pinLect, HIGH);

  // Mandamos confirmación a Matlab
  //digitalWrite(13, LOW);
  Serial.println("l");
  Serial.flush();

}
