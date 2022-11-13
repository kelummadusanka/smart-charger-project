
#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4// Reset pin not used but needed for library
#define ANALOG_IN_PIN A1//volatage sensor
Adafruit_SSD1306 display(OLED_RESET);


typedef struct chargePar
{
  char device[10];
  char vol[10];
  char cur[10];

} chargePar;

chargePar array[3];

uint16_t items;

// Variables for Measured Voltage and Calculated Current
double Vout = 0;
double Current = 0;
const double scale_factor = 0.018; 
const double vRef = 5.00;
const double resConvert = 1024;
double resADC = vRef/resConvert;
double zeroPoint = vRef/2.014;
double chooseratedcurrent=2.2;
double chooseratedvoltage=5.1;
int relay_pin = 12;


// Floats for ADC voltage & Input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;
 
// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0; 
 
// Float for Reference Voltage
float ref_voltage = 5.0;
 
// Integer for ADC value
int adc_value = 0;


void write_charPara(int k, chargePar chp);
void Read_charPara();


void setup() {
  Wire.begin();
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);  // Delay to allow sensor to stabalize
  pinMode(relay_pin,OUTPUT);
  digitalWrite(relay_pin, LOW);
  
  
}

void displayVoltageCurrent(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1.2);
  display.setCursor(0,0);
  display.print("Group 13");
  display.setCursor(0,10); 
  display.print("Voltage:    "); 
  display.setCursor(100,10);
  display.print(" V");
  display.setCursor(0,20);
  display.print("Current: ");
  display.setCursor(100,20);
  display.print(" A");
  display.setCursor(60,10);
  display.print(in_voltage);
  display.setCursor(60,20); 
  display.print(Current); 
}


void loop() {
  for(int i = 0; i < 1000; i++) {
    Vout = (Vout + (resADC * analogRead(A0)));   
    delay(1);
  }
 
  Vout = Vout /1000;
 
  // Convert Vout into Current using Scale Factor
  Current = (Vout - zeroPoint)/ scale_factor;

  // Read the Analog Input
  adc_value = analogRead(ANALOG_IN_PIN);
   
  // Determine voltage at ADC input
  adc_voltage  = (adc_value * ref_voltage) / 1024.0; 
   
  // Calculate voltage at divider input
  in_voltage = adc_voltage / (R2/(R1+R2)); 
                    
  Serial.print("Current = ");                  
  Serial.print(Current,2);
  Serial.println(" Amps");                             
  Serial.print("Input Voltage = ");
  Serial.println(in_voltage, 2);
  
  displayVoltageCurrent();
  display.display();
  relay();
  //digitalWrite(12,HIGH);
  //delay(5000);
   //digitalWrite(12,LOW);
}


void write_charPara(int k, chargePar chp)
{
  items = eeprom_read_word((uint16_t*)1000);
  int currentAddress = items*sizeof(chargePar);
  eeprom_write_block(&chp,(void*)(k*sizeof(chargePar)),sizeof(chargePar));
  items++;
  eeprom_update_word((uint16_t*)1000,items);
  
};

void Read_charPara()
{

  for(int i =0;i<3;i++ ){
    int current = i*sizeof(chargePar);
    eeprom_read_block(&array[i],(void*)current,sizeof(chargePar));
    _delay_ms(1000);
  }


};
void relay(){
  if ((chooseratedcurrent<Current)||(chooseratedvoltage<Vout)){
    digitalWrite(relay_pin, HIGH);
    delay(10000);

    if (Vout<chooseratedvoltage)
    {
      digitalWrite(relay_pin, LOW);
      }
    } 
};
void buttoninput()
{
  
  };
