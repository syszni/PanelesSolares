
#define PIN_GUVAS12SD A0 // Pin al que se conecta el módulo sensor de radiación solar
#define ESPERA_ENTRE_LECTURAS 100 // Leer cada 100 ms. la lectura demora 100 µs para cada una
#define ESPERA_ENTRE_PRESENTACIONES 60000 // Mostrar la lectura cada minuto
#define COEFICIENTE_VOLTAJE 1.07526881720430107527 // 1100/1023≃1.07526881720430107527 La lectura máxima es de 1023 que corresponde a 1100 mV 
 
unsigned int lectura_sensor;
unsigned int contador_lecturas=1;
float total_lecturas=0.0;
float media_lecturas;
long cronometro_lecturas;
long cronometro_presentaciones;
long tiempo_transcurrido;
 
void setup()
{
  analogReference(INTERNAL); // Referencia interna de 1100 mV El GUVA-S12SD mide de 0 a 1170 mV que corresponde con el índice UV 11
  //pinMode(PIN_GUVAS12SD,INPUT); // La lectura analógica no necesita inicialización
  Serial.begin(9600);
  #if defined (__AVR_ATmega32U4__)
    while(!Serial);// Esperar a Arduino Leonardo
  #endif
  lectura_sensor=analogRead(PIN_GUVAS12SD); // La primera lectura es incorrecta (normalmente cero) y necesita unos 100 µs para cada lectura analógica
  cronometro_lecturas=millis(); // Esperar un ciclo de lectura para estabilizar el sensor y la entrada analógica
  cronometro_presentaciones=millis();
}
void loop()
{
  tiempo_transcurrido=millis()-cronometro_lecturas;
  if(tiempo_transcurrido>ESPERA_ENTRE_LECTURAS)
  {
    cronometro_lecturas=millis();
    lectura_sensor=analogRead(PIN_GUVAS12SD);
    total_lecturas+=lectura_sensor;
    media_lecturas=total_lecturas/contador_lecturas++;
  }
  tiempo_transcurrido=millis()-cronometro_presentaciones;
  if(tiempo_transcurrido>ESPERA_ENTRE_PRESENTACIONES)
  {
    cronometro_presentaciones=millis();
    Serial.print("Lectura sensor: "+String(media_lecturas,DEC));
    Serial.print(" (media de "+String(contador_lecturas,DEC)+")");
    Serial.print(" Tensión (mV): "+String(media_lecturas*COEFICIENTE_VOLTAJE,DEC)+"\n\n");
    contador_lecturas=1;
    total_lecturas=0.0;
  }
}
