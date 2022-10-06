#include <Keypad.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(5, 4, 3, 2, A4, A5);


const byte filas = 4;            //filas
const byte columnas = 4;         //columnas
String contrasena = "1234";      //contrasena
String obtener_contrasena = "";  //obtener contraseña ingresada



#define ledAmarillo 13
#define ledVerde A3
#define ledRojo 12

//int vectorLeds[] = { ledAmarillo, ledVerde, ledRojo };  //arreglo de leds
int cursor = 0;
int num_intentos = 0;    //numero de intetos del usuario
bool resultado = false;  //estado de la comparacion de las cadenas contraseña ingresada y contraseña

char keys[filas][columnas] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte pin_filas[filas] = { A0, A1, 11, 10 };    //connect to the row pinouts of the keypad
byte pin_columnas[columnas] = { 9, 8, 7, 6 };  //connect to the column pinouts of the keypad
int LCDCol = 0;
int LCDFila = 0;

Keypad keypad = Keypad(makeKeymap(keys), pin_filas, pin_columnas, filas, columnas);


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  pinMode(ledAmarillo, OUTPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  //////////mensaje/////////////////
  lcd.setCursor(LCDCol, LCDFila);
  lcd.setCursor(0, 0);
  lcd.print("***bienvenido***");

  digitalWrite(ledAmarillo, HIGH);
  digitalWrite(ledRojo, HIGH);
  digitalWrite(ledVerde, HIGH);
  delay(3000);
  digitalWrite(ledAmarillo, LOW);
  digitalWrite(ledRojo, LOW);
  digitalWrite(ledVerde, LOW);
}
//////////////////////        funciones      ////////////////////////////7
void pedir_clave();
void validar_clave();
void secuenciaLeds();
//////////////////////////////////        void loop    /////////////////////////////
void loop() {
  if (resultado == false) {
    pedir_clave();
    if (cursor == 4 && num_intentos < 3)
      validar_clave();
    if (num_intentos == 3) {
      digitalWrite(ledRojo, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   bloqueado");
      num_intentos = 0;
      delay(10000);
      digitalWrite(ledRojo, LOW);
    }
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("  CLAVE VALIDA");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    Sistema");
    lcd.setCursor(0, 0);
    lcd.print("  Desbloqueado");
    digitalWrite(ledVerde, HIGH);
    delay(3000);
    secuenciaLeds();
    obtener_contrasena = "";
    num_intentos = 0;
    resultado = false;
    digitalWrite(ledVerde, LOW);
  }
}

////////////////////////////////////////////////////////////////////////////////////
void pedir_clave() {
  char key;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" INGRESE ClAVE");

  do {
    key = keypad.getKey();
    obtener_contrasena.concat(String(key));  //agregamos la contraseña ingresada a un string

    if (key) {
      Serial.println(obtener_contrasena);
      lcd.setCursor(cursor, 1);
      lcd.print("*");
      cursor++;
    }

  } while (cursor < 4);
}
////////////////////////////////////////////////////////////////////////////////////////

void validar_clave() {

  int comparacion = contrasena.compareTo(obtener_contrasena);
  if (comparacion == 0)
    resultado = true;
  else {
    lcd.clear();
    lcd.setCursor(0, 0);

    digitalWrite(ledAmarillo, HIGH);
    if (num_intentos < 2)
      lcd.print("Intente de nuevo");

    num_intentos++;
    lcd.setCursor(0, 1);
    lcd.print("num intentos:");
    lcd.setCursor(14, 1);
    lcd.print(num_intentos);

    resultado = false;
    delay(3000);
    digitalWrite(ledAmarillo, LOW);
  }
  cursor = 0;
  obtener_contrasena = "";
  Serial.println(num_intentos);
}
/////////////////////////////////////////////////////////////////////////////
void secuenciaLeds() {
  unsigned long tiempoInicio = millis();  //tiempo de arranque de la funcion
  do {
    digitalWrite(ledAmarillo, HIGH);
    delay(200);
    digitalWrite(ledRojo, HIGH);
    delay(200);
    digitalWrite(ledVerde, HIGH);
    delay(200);
    digitalWrite(ledAmarillo, LOW);
    delay(200);
    digitalWrite(ledRojo, LOW);
    delay(200);
    digitalWrite(ledVerde, LOW);
    delay(200);
  } while (millis() - tiempoInicio <=10000);
}


///////////////////////////////////////////////////////////////////////////