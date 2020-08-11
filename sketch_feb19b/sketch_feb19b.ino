void setup() {
   Serial.begin(9600);
}

void loop() {
 /// digital read from pin 2
 byte a;
 if ( Serial.available() > 0 )
 {
    a = Serial.read(); // get the byte of data from the GPS
    Serial.write(a);
 }
}
