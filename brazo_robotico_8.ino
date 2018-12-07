#include <Servo.h>
#include <Stepper.h>

#define ledN 10
#define ledV 11
#define ledA 12
#define ledR 13
#define botonAbortar 22
#define buzzer 24 // buzzer


Servo servo01;
Servo servo02;
Servo servo03;
Servo servo04;
Servo servo05;

int servo1Pos, servo2Pos, servo3Pos, servo4Pos, servo5Pos; // current position
int servo1PPos, servo2PPos, servo3PPos, servo4PPos, servo5PPos; // previous position
int servo01SP[50], servo02SP[50], servo03SP[50], servo04SP[50], servo05SP[50]; // for storing positions/steps
int index = 0;
int retardo = 5;
int num_pasos = 0;
int dato_rx;
String readString;
char a;


void setup() {
  pinMode(2, OUTPUT); //in4 Verde-Negro-Verde
  pinMode(3, OUTPUT); //in3 Blanco-Rojo-Blanco
  pinMode(4, OUTPUT); //in2 Naranja-Rojo-Naranja
  pinMode(5, OUTPUT); //in1 Amariilo-Morado-Amarillo

  servo01.attach(12);
  servo02.attach(6);//Nedro-Amarillo-Cafe
  servo03.attach(7);
  servo04.attach(8);//Cafe-Negro-Verde
  servo05.attach(9);//Azul-Blanco-Blanco

  pinMode(ledN, OUTPUT); //Guardar-Naranja
  pinMode(ledV, OUTPUT); //Reproducir -Verde
  pinMode(ledA, OUTPUT); //Pausa -Amarillo
  pinMode(ledR, OUTPUT); //Resetear -Rojo
  pinMode(botonAbortar, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(botonAbortar), abortar, RISING);
  Serial.begin(9600);
  // Robot arm initial position
  servo1PPos = 50;
  while (servo1PPos > num_pasos) { // Giro hacia la izquierda en grados
    paso_izq();
    num_pasos = num_pasos + 1;
  }
  while (servo1PPos < num_pasos) { // Giro hacia la derecha en grados
    paso_der();
    num_pasos = num_pasos - 1;
  }

  servo2PPos = 170;
  servo02.write(servo2PPos);
  servo3PPos = 120;
  servo03.write(servo3PPos);
  servo4PPos = 90;
  servo04.write(servo4PPos);
  servo5PPos = 30;
  servo05.write(servo5PPos);
  delay(10);
  digitalWrite(ledN , LOW);
  digitalWrite(ledV , LOW);
  digitalWrite(ledA , LOW);
  digitalWrite(ledR , LOW);

}

void loop() {
  /*en Arduino, al utilizamos ña condicion  a=='A',
    verificamos el prefijo de cada información entrante y sabemos qué hacer a continuación. Por ejemplo, si el prefijo es "A",
    sabemos que necesitamos mover el número uno del servo. Usando char c = Serial.read()  obtenemos el texto restante,
    o ese es el valor de posición, lo convertimos en entero y usamos el valor para mover el servo a esa posición.*/

  //El mismo método se utiliza para conducir cada eje del brazo del robot.
  if (Serial.available()) {
    a = Serial.read();

    if (a == 'A') { //BASE  
      delay(30);

      while (Serial.available()) {
        char c = Serial.read();
        readString += c;
      }
      if (readString.length() > 0) {
        Serial.println(readString.toInt());
        dato_rx = readString.toInt();
        delay(retardo);
        dato_rx = (dato_rx * 1.4222222222);
      }

      while (dato_rx > num_pasos) { // Girohacia la izquierda en grados
        paso_izq();
        num_pasos = num_pasos + 1;
      }
      while (dato_rx < num_pasos) { // Giro hacia la derecha en grados
        paso_der();
        num_pasos = num_pasos - 1;
      }
      servo1PPos = dato_rx;
      readString = "";   // Inicializamos la cadena de caracteres recibidos

    }

    if (a == 'B') { //BRAZO
      delay(30);
      while (Serial.available()) {
        char c = Serial.read();
        readString += c;
      }
      if (readString.length() > 0) {
        Serial.println(readString.toInt());
        servo2Pos = readString.toInt();

        // Aquí podemos simplemente llamar a la función write () y el servo irá a esa posición, pero de esa manera el servo funcionará a su velocidad máxima,
        servo02.write(servo2Pos);

      }
      servo2PPos = servo2Pos;
      Serial.println(servo2PPos);
      readString = "";
    }


    if (a == 'C') {  //CODO
      delay(30);
      while (Serial.available()) {
        char c = Serial.read();
        readString += c;
      }
      if (readString.length() > 0) {
        Serial.println(readString.toInt());
        servo3Pos = readString.toInt();
        servo03.write(servo3Pos);

      }
      servo3PPos = servo3Pos;
      Serial.println(servo3PPos);
      readString = "";
    }

    if (a == 'D') {  //MUÑECA
      delay(30);
      while (Serial.available()) {
        char c = Serial.read();
        readString += c;
      }
      if (readString.length() > 0) {
        Serial.println(readString.toInt());
        servo4Pos = readString.toInt();
        servo04.write(servo4Pos);


      }
      servo4PPos = servo4Pos;
      Serial.println(servo4PPos);
      readString = "";
    }

    if (a == 'E') { //PINZA
      delay(30);
      while (Serial.available()) {
        char c = Serial.read();
        readString += c;
      }
      if (readString.length() > 0) {
        Serial.println(readString.toInt());
        servo5Pos = readString.toInt();
        servo05.write(servo5Pos);


      }
      servo5PPos = servo5Pos;
      Serial.println(servo5PPos);
      readString = "";

    }

    //Debajo de ellos está el botón GUARDAR (S). Si presionamos el botón GUARDAR, la posición de cada servomotor se almacena en una matriz.
    //Con cada pulsación, el índice aumenta, por lo que la matriz se llena paso a paso.
    if (a == 'S') {
      digitalWrite(ledN , HIGH);
      digitalWrite(ledV , LOW);
      digitalWrite(ledA , LOW);
      digitalWrite(ledR , LOW);
      delay(2000);
      digitalWrite(ledN , LOW);
      tone(buzzer, 423.25, 500);   //do 5  // Lanza señal a 523.25Hz por el pin 3 durante 500ms.
      delay(550);
      Serial.println("POSICIONES");
      servo01SP[index] = servo1PPos; 
      Serial.println(servo01SP[index]);// save position into the array
      servo02SP[index] = servo2PPos;
      Serial.println( servo02SP[index]);
      servo03SP[index] = servo3PPos;
      Serial.println(servo03SP[index]);
      servo04SP[index] = servo4PPos;
      Serial.println(servo04SP[index]);
      servo05SP[index] = servo5PPos;
      Serial.println(servo05SP[index]);

      index++;                        // Increase the array index
    }
    //Luego, si presionamos el botón RUN (R), llamamos a la función personalizada runservo () que ejecuta los pasos almacenados y se encendera el led verde.
    if (a == 'R') {
      //digitalWrite(ledN , LOW);
      //digitalWrite(ledV , HIGH);
      //digitalWrite(ledA , LOW);
      //digitalWrite(ledR , LOW);
      //delay(2000);
      //digitalWrite(ledV , LOW);
      runservo();  // Automatic mode - run the saved steps
      //delay(1000);

    }

    if ( a == 'Z') {
      abortar();
    }
  }
}

/*quí ejecutamos los pasos almacenados una y otra vez hasta que presionamos el botón RESETEAR (Z). Usando el bucle FOR,
  recorremos todas las posiciones almacenadas en los arreglos y, al mismo tiempo, verificamos si tenemos datos entrantes desde el teléfono inteligente.
  Estos datos pueden ser el botón EJECUTAR / PAUSA, que detiene el robot y, si se hace clic nuevamente, continúa con los movimientos automáticos.
  Además, si cambiamos la posición del control deslizante de velocidad, usaremos ese valor para cambiar el tiempo de retardo entre cada iteración
  en los bucles FOR de abajo, que controlan la velocidad de los servomotores. */
void runservo() {
  Serial.println("run");
  tone(buzzer, 523.25, 500);   //do 5   // Lanza señal a 523.25Hz por el pin 3 durante 500ms.
  delay(550);
  tone(buzzer, 523.25, 500);   //do 5  // Lanza señal a 523.25Hz por el pin 3 durante 500ms.
  delay(550);
  while (a != 'Z') {
    for (int i = 0; i <= index - 1; i++) {
      if (Serial.available() > 0) {      // Check for incomding data
        a = Serial.read();

        // Wait until "RUN" is pressed again
        while ((a != 'R')) {
          if (Serial.available() > 0) {      // Check for incomding data
            a = Serial.read();

            if ( a == 'Z') {
               Serial.println("abortando");
              abortar();
              break;
            }
            if ( a == 'P') {           // If button "PAUSE" is pressed
              digitalWrite(ledN , LOW);
              digitalWrite(ledV , LOW);
              digitalWrite(ledA , HIGH);
              digitalWrite(ledR , LOW);
              delay(2000);
              digitalWrite(ledA , LOW);
            }

          }
        }

        if ( a == 'Z') {
          abortar();
          break;
        }
      }

      while (servo01SP[i] > num_pasos) { // Giro hacia la izquierda en grados
        paso_izq();
        num_pasos = num_pasos + 1;
      }
      while (servo01SP[i] < num_pasos) { // Giro hacia la derecha en grados
        paso_der();
        num_pasos = num_pasos - 1;
      }
      //se ejecutan los pasos guardados en el arreglo
      int j = servo02SP[i];

      Serial.println(j);
      servo02.write(j);
      delay(500);

      int q = servo03SP[i];
      Serial.println(q);
      servo03.write(q);
      delay(500);

      int t = servo04SP[i];
      Serial.println(t);
      servo04.write(t);
      delay(500);

      int y = servo05SP[i];
      Serial.println(y);
      servo05.write(y);
      delay(500);

    }
  }
}


void paso_der() {        // motos a pasos Pasos a la derecha
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(retardo);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(retardo);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(retardo);
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(retardo);
}

void paso_izq() {        //motor a pasos Pasos a la izquierda
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
  delay(retardo);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(retardo);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  delay(retardo);
  digitalWrite(2, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  delay(retardo);
}

//metodo de abortar este metodo se dispara cuando se presiona el boton reset en la aplicaion
void abortar() {
  digitalWrite(ledN , LOW);
  digitalWrite(ledV , LOW);
  digitalWrite(ledA , LOW);
  digitalWrite(ledR , HIGH);
  tone(buzzer, 523.25, 500);   //do 5   // Lanza señal a 523.25Hz por el pin 3 durante 500ms.
  delay(550);
  tone(buzzer, 523.25, 500);   //do 5  // Lanza señal a 523.25Hz por el pin 3 durante 500ms.
  delay(550);

  if (index != 0) {
    memset(servo01SP, 0, sizeof(servo01SP)); // Clear the array data to 0
    memset(servo02SP, 0, sizeof(servo02SP));
    memset(servo03SP, 0, sizeof(servo03SP));
    memset(servo04SP, 0, sizeof(servo04SP));
    memset(servo05SP, 0, sizeof(servo05SP));
    index = 0;  // Index to 0
  }
  servo1PPos = 50;

  while (servo1PPos > num_pasos) { // Giro hacia la izquierda en grados
    paso_izq();
    num_pasos = num_pasos + 1;
  }
  while (servo1PPos < num_pasos) { // Giro hacia la derecha en grados
    paso_der();
    num_pasos = num_pasos - 1;
  }
  //cuando se presiona reset en la aplicaion el brazo robotico vuelve a la posicion original
  servo2PPos = 170;
  servo02.write(servo2PPos);
  servo3PPos = 120;
  servo03.write(servo3PPos);
  servo4PPos = 90;
  servo04.write(servo4PPos);
  servo5PPos = 10;
  servo05.write(servo5PPos);

  delay(5000);
  digitalWrite(ledR , LOW);
}
