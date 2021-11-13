//https://forum.arduino.cc/index.php?topic=122039.0


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}
float number = 0.000000000022222222222;

union u_tag {
  byte b[4];
  float fval;
} u;

void loop() {
  // put your main code here, to run repeatedly:
u.fval = number;

   Serial.print("Byte 1: ");
   Serial.println(u.b[0], DEC );

   Serial.print("Byte 2: ");
   Serial.println(u.b[1], DEC );
 
   Serial.print("Byte 3: ");
   Serial.println(u.b[2], DEC );
 
   Serial.print("Byte 4: ");
   Serial.println(u.b[3], DEC );
   
   Serial.print("velocity: ");
   Serial.println(u.fval, 32);
   Serial.println (number, 32);
 
   Serial.println();


   delay(3000); // allow some time for the Serial data to be sent
}
