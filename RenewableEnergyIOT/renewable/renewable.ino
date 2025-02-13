#include <LiquidCrystal.h>
#include <GSM.h>
const int rs = 7, en = 8, d4 = 3, d5 = 4, d6 = 5, d7 = 6;
int bs=0;
const int battery = A0;
const int ct1 = A1,ct2=A2;
int t=0,m=0,n=0;  
const int r1 =12,r2=13,r3=2,r4=A0;   
int i = 0,j=0,cts1=0,cts2=0;
float i2=0,j2=0;     
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() 
{
  lcd.begin(16, 2);
   Serial.begin(9600);
     pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
   digitalWrite(10, LOW);
   digitalWrite(9, LOW);
   
   lcd.setCursor(4,0);
  lcd.print("WELCOME");
  delay(1000);
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("RENEWABLE ENERGY");
  delay(1000);

  delay(1000);
  gprs_init();
  lcd.clear();
}
void loop()
{


   bs = analogRead(battery);
   bs=bs*0.0122;
    
  
 if(bs<5)
 {
  digitalWrite(10, HIGH);
   digitalWrite(9, HIGH);
  n=1;
  if(m==1)
  {
    m=0;
    i2=i*0.016;
       cts1 = analogRead(ct1);
       i2=cts1*i;
       i2=i2*0.1428;
       i2=i2/1000;
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Invr Unit=");
   lcd.setCursor(11,0);
  lcd.print(i2);
  lcd.setCursor(15,0);
  lcd.print("U");
  sendsms1();
  delay(500);
  gprs_update(i2,0);

  i=0;
  i2=0;
    lcd.clear();
  }

   lcd.setCursor(0,0);
  lcd.print("B");
   lcd.setCursor(0,1);
  lcd.print("  ");
   lcd.setCursor(0,1);
  lcd.print(bs);
  delay(200);
    lcd.setCursor(3,0);
  lcd.print("INVER");
   lcd.setCursor(3,1);
    lcd.print("         ");
       lcd.setCursor(3,1);
  lcd.print(i);
  lcd.setCursor(10 ,0);
  lcd.print("GRID ");
   lcd.setCursor(10,1);
    lcd.print("         ");
       lcd.setCursor(10,1);
  lcd.print(j);
    delay(1000);
     j++;
  }
  else if(bs>5)
  {
    m=1;
    digitalWrite(10, LOW);
   digitalWrite(9, LOW);
    if(n==1)
  {
    n=0;
    j2=j*0.016;
    cts2 = analogRead(ct2);
    j2=cts2*j;
    j2=j2*0.000196;
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Grid Unit=");
   lcd.setCursor(11,0);
  lcd.print(j2);
  lcd.setCursor(15,0);
  lcd.print("U");
  sendsms2();
  delay(500);
    gprs_update(0,j2);
  j=0;
  j2=0;
    lcd.clear();
  }
    
 
    lcd.setCursor(0,0);
  lcd.print("B");
   lcd.setCursor(0,1);
  lcd.print("  ");
   lcd.setCursor(0,1);
  lcd.print(bs);
  delay(200);
     lcd.setCursor(3,0);
  lcd.print("INVER");
   lcd.setCursor(3,1);
    lcd.print("         ");
       lcd.setCursor(3,1);
  lcd.print(i);
  lcd.setCursor(10 ,0);
  lcd.print("GRID ");
   lcd.setCursor(10,1);
    lcd.print("         ");
       lcd.setCursor(10,1);
  lcd.print(j);
    delay(1000);
     i++;
  }
}
 void sendsms1()
{
  Serial.println("AT\r");
  delay(1000);
  Serial.println("AT+CMGF=1\r");
  delay(1000);
  Serial.println("AT+CMGS=\"7025474346\"\r");
  delay(1000);
  Serial.println("ENERGY USAGE\n");
  Serial.println("Inverter =");
  Serial.println(i2);
  Serial.println(" Unit");
  delay(1000);
  Serial.println((char)26);
  delay(100);
}
 void sendsms2()
{
  Serial.println("AT\r");
  delay(1000);
  Serial.println("AT+CMGF=1\r");
  delay(1000);
  Serial.println("AT+CMGS=\"7025474346\"\r");
  delay(1000);
  Serial.println("ENERGY USAGE\n");
  Serial.println("Grid =");
  Serial.println(j2);
  Serial.println(" Unit");
  delay(1000);
  Serial.println((char)26);
  delay(100);
}
void gprs_init()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GPRS configuring");
  lcd.setCursor(0,1);
  lcd.print("Please wait...");
  delay(2000);
  Serial.println("AT\r");
 // Serial.println(0X0D);
  delay(1000);
  delay(1000);
  Serial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  //  Serial.println(0X0D);
  delay(1000);
  delay(1000);
  delay(1000);
  Serial.println("AT+SAPBR=3,1,\"APN\",\"CMNET\"");
 //   Serial.println(0X0D);
  delay(1000);
  delay(1000);
  delay(1000);
   Serial.println("AT+SAPBR=3,1,\"APN\",\"airtelgprs.com\"");
    // Serial.println(0X0D);
  delay(1000);
  delay(1000);
  delay(1000);
   Serial.println("AT+SAPBR=1,1");
   //  Serial.println(0X0D);
  delay(1000);
  delay(1000);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GPRS configured");
  lcd.setCursor(0,1);
  lcd.print("Successfully...");
  delay(2000);
}

void gprs_update(float r,float n)
{
                 
    Serial.println("AT\r");
         delay(1000);
     Serial.println("AT+SAPBR=2,1");
        delay(2000);
        delay(2000);
       Serial.println("AT+HTTPINIT");
      delay(2000);
        delay(2000);
       Serial.println("AT+HTTPPARA=\"CID\",1");
     delay(2000);
        delay(2000);
        delay(1000);
        Serial.print("AT+HTTPPARA=\"URL\",\"http://www.tamilgurukulam.com/iotmakers/upload.php?id=960iql6t&data1=");
         Serial.print(r);
        Serial.print("&data2="); 
        Serial.print(n);
        Serial.print("\"\r\n");
        delay(2000);
        delay(2000);
        delay(2000);
       Serial.println("AT+HTTPPARA=\"CID\",1");
      delay(2000);
        delay(2000);
        delay(1000);
      Serial.println("AT+HTTPACTION=0");
      delay(2000);
        delay(2000);
        delay(2000);
         delay(2000);
        delay(1000);
        delay(1000);
        Serial.println("AT+HTTPREAD");
       delay(2000);
        delay(2000);
        delay(2000);
      lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GPRS Updated");
  lcd.setCursor(0,1);
  lcd.print("Successfully...");
  delay(2000);
       
}



