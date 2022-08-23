#include <LiquidCrystal.h>
#include <DHT.h>

#define TONO_ERROR 200
#define TONO_ERROR1 800
#define INTERVALO_TIEMPO 1000
#define INTERVALO_TIEMPO1 1000
#define DHT_11 A14
#define DHTTYPE DHT11

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // Salidas Arduino de la pantalla LCD principal
DHT dht(DHT_11, DHTTYPE);

int motor =  8;         // Controlador de los motores de refrigeración
int calefactor = 9;     // Controlador de los motores calefactables
int ledTiempoA = 11;    // Led Verde - tiempo EJECUCIÓN
int ledTiempoD = 10;    // Led Parpadeante - tiempo FINALIZADO
int buzzer = 12;        // Aviso Auditivo
int buzzer1 = 13;       // Aviso Auditivo
int ledMotorA = 14;     // Led - Motor ACTIVO
int ledCalefA = 16;     // Led - Calefactor ACTIVO
int ledPhA = 18;        // Led - pH ALTO
int ledPhB = 19;        // Led - pH BAJO

// ----------------- Temporizador y Constantes ------------------

int mostrarSeg = 0;      // Variable que muestra los SEGUNDOS en el LCD
int mostrarMin = 0;      // Variable que muestra los MINUTOS en el LCD
int mostrarHrs = 0;      // Variable que muestra las HORAS en el LCD
int segundosTotal = -1;   // Tiempo Total
int cont = 0;            // Variable contadora para el Menu
int selecOpciones = 0;   // Variable que selecciona en el Menu
float temperatura = 0;   // Variable para la Temperatura Ambiente
float pHVal = 0;         // Variable para el Potencial de Hidrógeno
float temperaturaInt = 0;// Variable para la Temperatura Interior
float humedad = 0;       // Variable para la Humedad Interior
float humedadCafe = 0;       // Variable para la Humedad del café

// ----------------- Entradas ------------------
int LM35 = A0;           // Entrada de temperatura sensor LM35
int pH = A1;             // Entrada de valores del pH
int botMenu = A2;        // Pulsador de Menu
int botA = A3;
int botB = A5;
int botC = A4;
int humedadC = A11;


void setup()
{
  //Inicializamos comunicación serial - con el ordenador
  Serial.begin(9600);

  // Inicializador del Controlador
  lcd.begin(20, 4);
  dht.begin();

  lcd.setCursor(4, 0);
  lcd.print(" BIENVENIDO!");
  lcd.setCursor(5, 1);
  lcd.print("CONTROL DE");
  lcd.setCursor(0, 2);
  lcd.print("FERMENTACION DE CAFE");
  delay(200);
  lcd.setCursor(0, 3);
  lcd.print("Inicializando Modulo");
  delay(500);
  lcd.setCursor(0, 3);
  lcd.print("..Por favor espere..");
  delay(500);

  pinMode(buzzer, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  pinMode(ledTiempoA, OUTPUT);
  pinMode(ledTiempoD, OUTPUT);
  pinMode(ledMotorA, OUTPUT);
  pinMode(ledCalefA, OUTPUT);
  pinMode(ledPhA, OUTPUT);
  pinMode(ledPhB, OUTPUT);
  pinMode(motor, OUTPUT);
  pinMode(calefactor, OUTPUT);
  pinMode(humedadC, INPUT);

  // ----------------- Temporizador ------------------
  pinMode(botA, INPUT);
  pinMode(botB, INPUT);
  pinMode(botC, INPUT);
  pinMode(botMenu, INPUT);

  //arranque = 1024;
  lcd.clear();
}

void loop()
{
  // --------------------------------------------------------
  // ----------------------- MENU ---------------------------
  // --------------------------------------------------------
  selecOpciones = 0;
  segundosTotal = -1;
  digitalWrite(ledTiempoA, LOW);
  digitalWrite(ledTiempoD, HIGH);
  while (cont == 0)
  {
    while (selecOpciones == 0) {
      lcd.setCursor(0,0);
      lcd.print("MENU: Asub Bbaj Csel");
      lcd.setCursor(0,1);
      lcd.print(" ~ Ver Temp Ambiente");
      lcd.setCursor(0,2);
      lcd.print("   Ver pH");
      lcd.setCursor(0,3);
      lcd.print("   Ver Humedad");
      controlParametros();

      if (analogRead(botA) == LOW) {
        selecOpciones = 5;
        delay(350);
        lcd.clear();
      }

      if (analogRead(botC) == LOW) {
        selecOpciones == -1;
        cont = 1;
        delay(350);
        lecturaParametros(cont);
      }
      if (analogRead(botB) == LOW) {
        selecOpciones = 1;
        delay(350); //Delay humano
        lcd.clear();
      }
    }
    
    cont = 0;
    while (selecOpciones == 1) {
      lcd.setCursor(0,0);
      lcd.print("MENU: Asub Bbaj Csel");
      lcd.setCursor(0,1);
      lcd.print("   Ver Temp Ambiente");
      lcd.setCursor(0,2);
      lcd.print(" ~ Ver pH");
      lcd.setCursor(0,3);
      lcd.print("   Ver Humedad");
      controlParametros();

      if (analogRead(botA) == LOW) {
        selecOpciones = 0;
        delay(350);
        lcd.clear();
      }

      if (analogRead(botC) == LOW) {
        cont = 2;
        delay(350);
        lecturaParametros(cont);
      }
      if (analogRead(botB) == LOW) {
        selecOpciones = 2;
        delay(350); //Delay humano
        lcd.clear();
      }
    }
    
    cont = 0;
    while (selecOpciones == 2) {
      lcd.setCursor(0,0);
      lcd.print("MENU: Asub Bbaj Csel");
      lcd.setCursor(0,1);
      lcd.print("   Ver Temp Ambiente");
      lcd.setCursor(0,2);
      lcd.print("   Ver pH");
      lcd.setCursor(0,3);
      lcd.print(" ~ Ver Humedad");
      controlParametros();

      if (analogRead(botA) == LOW) {
        selecOpciones = 1;
        delay(350);
        lcd.clear();
      }

      if (analogRead(botC) == LOW) {
        cont = 3;
        delay(350);
        lecturaParametros(cont);
      }
      if (analogRead(botB) == LOW) {
        selecOpciones = 3;
        delay(350); //Delay humano
        lcd.clear();
      }
    }

    cont = 0;
    while (selecOpciones == 3) {
      lcd.setCursor(0,0);
      lcd.print("MENU: Asub Bbaj Csel");
      lcd.setCursor(0,1);
      lcd.print(" ~ Ver Temp del Cafe");
      lcd.setCursor(0,2);
      lcd.print("   Ajustar Tiempo");
      lcd.setCursor(0,3);
      lcd.print("   Instrucciones Uso");
      controlParametros();

      if (analogRead(botA) == LOW) {
        selecOpciones = 2;
        delay(350);
        lcd.clear();
      }

      if (analogRead(botC) == LOW) {
        cont = 4;
        delay(350);
        lecturaParametros(cont);
      }
      if (analogRead(botB) == LOW) {
        selecOpciones = 4;
        delay(350); //Delay humano
        lcd.clear();
      }
    }

    cont = 0;
    while (selecOpciones == 5) {
      lcd.setCursor(0,0);
      lcd.print("MENU: Asub Bbaj Csel");
      lcd.setCursor(0,1);
      lcd.print("   Ver Temp del Cafe");
      lcd.setCursor(0,2);
      lcd.print("   Ajustar Tiempo");
      lcd.setCursor(0,3);
      lcd.print(" ~ Instrucciones Uso");
      controlParametros();

      if (analogRead(botA) == LOW) {
        selecOpciones = 4;
        delay(350);
        lcd.clear();
      }

      if (analogRead(botC) == LOW) {
        delay(350);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("AjusteTiempo deseado");
        lcd.setCursor(0,1);
        lcd.print("para fermentacion de");
        lcd.setCursor(0,2);
        lcd.print("cafe. ElColor de los");
        lcd.setCursor(0,3);
        lcd.print("led significa:  ");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("LED PARA TIEMPO:");
        lcd.setCursor(0,1);
        lcd.print("LedAzul: Tiempo Ejec");
        lcd.setCursor(0,2);
        lcd.print("LedRojo: Tiempo Stop");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("LED PARA pH:");
        lcd.setCursor(0,1);
        lcd.print("LedRojo: pH ALTO");
        lcd.setCursor(0,2);
        lcd.print("LedAzul: pH BAJO");
        lcd.setCursor(0,3);
        lcd.print("Apagados: pH NORMAL");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("LED PARA TEMPERAT C:");
        lcd.setCursor(0,1);
        lcd.print("LedAzul: ventilad ON");
        lcd.setCursor(0,2);
        lcd.print("LedRojo: calefact ON");
        delay(2000);
        lcd.clear();
      }
      if (analogRead(botB) == LOW) {
        selecOpciones = 0;
        delay(350); //Delay humano
        lcd.clear();
      }
    }

    while (selecOpciones == 4) {
      lcd.setCursor(0,0);
      lcd.print("MENU: Asub Bbaj Csel");
      lcd.setCursor(0,1);
      lcd.print("   Ver Temp del Cafe");
      lcd.setCursor(0,2);
      lcd.print(" ~ Ajustar Tiempo");
      lcd.setCursor(0,3);
      lcd.print("   Instrucciones Uso");
      controlParametros();

      if (analogRead(botA) == LOW) {
        selecOpciones = 3;
        delay(350);
        lcd.clear();
      }

      if (analogRead(botC) == LOW) {
        cont = 5;
        selecOpciones = -1;
        delay(350);
        lcd.clear();
      }
      if (analogRead(botB) == LOW) {
        selecOpciones = 5;
        delay(350); //Delay humano
        lcd.clear();
      }
    }
  }

  // --------------------------------------------------------
  // --- Lectura de los Botones y Programación del Tiempo ---
  // --------------------------------------------------------

  while (cont == 5) {

    // Boton Segundos
    if (analogRead(botA) == LOW) {
      mostrarSeg = mostrarSeg + 1;
      if (mostrarSeg == 60) {
        mostrarSeg = 0;
        mostrarMin = mostrarMin + 1;
      }
      delay(350);
    }

    // Boton Minutos
    if (analogRead(botB) == LOW) {
      mostrarMin = mostrarMin + 1;
      if (mostrarMin == 60) {
        mostrarSeg = 0;
        mostrarMin = 0;
        mostrarHrs = mostrarHrs + 1;
      }
      delay(350);
    }
    controlParametros();
    lcd.setCursor(0, 0);
    lcd.print("Programe el tiempo: ");
    lcd.setCursor(0, 1);
    lcd.print(" A:seg  B:min  C:hrs");
    lcd.setCursor(0, 3);
    lcd.print("Oprima MENU Empezar");
    lcd.setCursor(6, 2);

    if (analogRead(botMenu) == LOW) {
      cont = 0;
    }

    if (mostrarHrs < 10) lcd.print("0");  // Anteponer un 0 a las horas menores de 10
    lcd.print(mostrarHrs);
    lcd.print(":");

    if (mostrarMin < 10) lcd.print("0");  // Anteponer un 0 a las horas menores de 10
    lcd.print(mostrarMin);
    lcd.print(":");

    if (mostrarSeg < 10) lcd.print("0");  // Anteponer un 0 a las horas menores de 10
    lcd.print(mostrarSeg);

    // Boton Horas
    if (analogRead(botC) == LOW) {
      mostrarHrs = mostrarHrs + 1;
      delay(350);
    }
  }
  // Convertir el tiempo programado en segundos
  segundosTotal = mostrarSeg + (mostrarMin * 60) + (mostrarHrs * 60 * 60);

  // --------------------------------------------------------------------
  // --- Cuenta regresiva que no finaliza hasta que termine el tiempo ---
  // --------------------------------------------------------------------
  lcd.clear();
  while (segundosTotal > 0)
  {
    delay(500);         // Se descuenta el tiempo en un segundo, ajustado
    segundosTotal--;
    controlParametros();
    digitalWrite(ledTiempoA, HIGH);
    digitalWrite(ledTiempoD, LOW);


    mostrarHrs = (( segundosTotal / 60 ) / 60); // Convertir los segundos en horas
    mostrarMin = (( segundosTotal / 60 ) % 60); // Convertir los segundos en minutos
    mostrarSeg = segundosTotal % 60;  // Convertir los segundos en periodos de 60 seg

    lcd.setCursor(2, 0);
    lcd.print("Tiempo Restante:");
    lcd.setCursor(6, 1);

    if (mostrarHrs < 10) lcd.print("0");
    lcd.print(mostrarHrs);
    lcd.print(":");

    if (mostrarMin < 10) lcd.print("0");
    lcd.print(mostrarMin);
    lcd.print(":");

    if (mostrarSeg < 10) lcd.print("0");
    lcd.print(mostrarSeg);

    lcd.setCursor(0, 2);
    lcd.print("Oprima A Condiciones");
    lcd.setCursor(0, 3);
    lcd.print("Oprima MENU Cancelar");

    // Boton Menu
    if (analogRead(botMenu) == LOW) {
      segundosTotal = 0;
      mostrarHrs = 0;
      mostrarMin = 0;
      mostrarSeg = 0;
      delay(350);
    }
    // Boton Revisar Condiciones
    if (analogRead(botA) == LOW) {
      delay(350);
      lecturaParametros(9);              // Un caso especial para mostrar todo
      segundosTotal = segundosTotal - 10; // Para que el tiempo siga contando, cuando se revisen condiciones
    }
    cont = -1;

  }

  lcd.clear();
  if (segundosTotal == 0) {
    //lcd.print("                    ");
    digitalWrite(ledTiempoA, LOW);
    digitalWrite(ledTiempoD, HIGH);  
    lcd.setCursor(1, 0);
    lcd.print("TIEMPO FINALIZADO");
    lcd.setCursor(0, 1);
    lcd.print("..Por favor espere..");
    sonarTono(TONO_ERROR, 600);
    cont = 0;
  }
}




// ---------------------------------------
// Función para hacer sonar la alarma
// ---------------------------------------
void sonarTono(int tono, int duracion) {
  if (tono == TONO_ERROR) {
    tone(buzzer, tono, duracion);
  }
  if (tono == TONO_ERROR1) {
    tone(buzzer1, tono, duracion);
  }
  delay(duracion);
}


// --------------------------------------------------------
// ----------------- Lectura de Parámetros ----------------
// --------------------------------------------------------
void lecturaParametros(int cont) {
  if (cont == 1 || cont == 9) {
    temperatura = analogRead(LM35);         // Lectura sensor
    temperatura = (temperatura / 1024) * 5 * 100;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TEMPERATURA AMBIENTE");
    lcd.setCursor(4, 1);
    lcd.print(temperatura);
    lcd.print(" C");
    delay(1500);    
  }
  
  // ----------------- Lectura del Potencial de Hidrógeno -----------------
  if (cont == 2 || cont == 9) {
    pHVal = analogRead(pH);
    pHVal = pHVal / 10;
    pHVal = (pHVal * 14) / 100; // Conversión a valores de ph (0-14)

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NUMERO DEL PH CAFE");
    lcd.setCursor(4, 1);
    lcd.print("pH: ");
    lcd.print(pHVal);

    if (pHVal < 3.7) {
      lcd.setCursor(0, 2);
      lcd.print("ALERTA pH BAJO!");
      digitalWrite(ledPhB, HIGH);
      digitalWrite(ledPhA, LOW);
      sonarTono(TONO_ERROR1, 25);
    }

    if (pHVal > 4.2) {
      lcd.setCursor(0, 2);
      lcd.print("ALERTA pH ALTO!");
      digitalWrite(ledPhA, HIGH);
      digitalWrite(ledPhB, LOW);
      sonarTono(TONO_ERROR1, 25);
    }

    if (pHVal >= 3.7 && pHVal <= 4.2) {
      lcd.setCursor(0, 2);
      lcd.print("CONDICIONES");
      lcd.setCursor(12, 2);
      lcd.print("NORMALES");
      digitalWrite(ledPhB, LOW);
      digitalWrite(ledPhA, LOW);
    }
    delay(1500);
    lcd.clear();
  }

  // ----------------- Lectura de la Humedad Relativa y la Temperatura Interior -----------------
  if (cont == 3 || cont == 4 || cont == 9) {
    temperaturaInt = dht.readTemperature();
    humedad = dht.readHumidity();
    humedadCafe = analogRead(humedadC);   
    humedadCafe = humedadCafe / 10; 

    if (cont == 4 || cont == 9) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("TEMPERATURA DEL CAFE");
      lcd.setCursor(4, 1);
      lcd.print(temperaturaInt);
      lcd.print(" C ");

      if (temperaturaInt > 35) {
        lcd.setCursor(0, 2);
        lcd.print("REFRIGERANDO");
        lcd.setCursor(0, 3);
        lcd.print("ALERTA Temp Alta");
        digitalWrite(motor, HIGH);
        sonarTono(TONO_ERROR, 25);
        digitalWrite(calefactor, LOW);
        digitalWrite(calefactor, LOW);
        digitalWrite(ledCalefA, LOW);
        digitalWrite(ledMotorA, HIGH);
      }

      if (temperaturaInt > 28 and temperaturaInt < 35) {
        lcd.setCursor(0, 2);
        lcd.print("COND NORMALES ");
        lcd.setCursor(0, 3);
        lcd.print("Temperatura Ideal");
        digitalWrite(motor, LOW);
        digitalWrite(calefactor, LOW);
        digitalWrite(ledCalefA, LOW);
        digitalWrite(ledMotorA, LOW);
      }

      if (temperaturaInt < 28) {
        lcd.setCursor(0,2);
        lcd.print("CALENTANDO");
        lcd.setCursor(0,3);
        lcd.print("ALERTA Temp Baja");
        digitalWrite(motor, LOW);
        digitalWrite(calefactor, HIGH);
        sonarTono(TONO_ERROR, 25);
        digitalWrite(ledCalefA, HIGH);
        digitalWrite(ledMotorA, LOW);
      }
      delay(1000);
    }

    if (cont == 3 || cont == 9) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("HUMEDAD DEL CAFE");
        lcd.setCursor(4, 1);
        lcd.print(" H: ");
        lcd.print(humedadCafe);
        lcd.print(" %");
        lcd.setCursor(0, 2);
        lcd.print("HUMEDAD RECINTO");
        lcd.setCursor(4, 3);
        lcd.print(" H: ");
        lcd.print(humedad);
        lcd.print(" %");
        delay(1500);
      }
    lcd.clear();
   } 
}


// --------------------------------------------------------
// ------------------- Control Parámetros -----------------
// --------------------------------------------------------
void controlParametros() {
  // ----------------- Lectura de la Temperatura Exterior -----------------
  temperatura = analogRead(LM35);         // Lectura sensor
  temperatura = (temperatura / 1024) * 5 * 100;

  // ----------------- Lectura de la Temperatura Interior -----------------
  temperaturaInt = dht.readTemperature();

  if (temperaturaInt > 35) {
    digitalWrite(motor, HIGH);
    digitalWrite(calefactor, LOW);
    digitalWrite(ledCalefA, LOW);
    digitalWrite(ledMotorA, HIGH);
  }

  if (temperaturaInt > 28 and temperaturaInt < 35) {
    digitalWrite(motor, LOW);
    digitalWrite(calefactor, LOW);
    digitalWrite(ledCalefA, LOW);
    digitalWrite(ledMotorA, LOW);
  }

  if (temperaturaInt < 28) {
    digitalWrite(motor, LOW);
    digitalWrite(calefactor, HIGH);
    digitalWrite(ledCalefA, HIGH);
    digitalWrite(ledMotorA, LOW);
  }

  pHVal = analogRead(pH);
  pHVal = pHVal / 10;
  pHVal = (pHVal * 14) / 100; // Conversión a valores de ph (0-14)


  if (pHVal < 3.7) {
    lcd.setCursor(0, 2);
    digitalWrite(ledPhB, HIGH);
    digitalWrite(ledPhA, LOW);
  }

  if (pHVal > 4.2) {
    lcd.setCursor(0, 2);
    digitalWrite(ledPhA, HIGH);
    digitalWrite(ledPhB, LOW);
  }

  if (pHVal >= 3.7 && pHVal <= 4.2) {
    lcd.setCursor(0, 2);
    digitalWrite(ledPhB, LOW);
    digitalWrite(ledPhA, LOW);
  }
}
