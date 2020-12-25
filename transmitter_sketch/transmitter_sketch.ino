
int led = 13;
// the setup routine runs once when you press reset:
void setup() {
 // initialize the digital pin as an output.
 pinMode(led, OUTPUT);
}
// the loop routine runs over and over again forever:
void loop() {
 digitalWrite(led, HIGH); // turn the LED on (HIGH is the voltage level)
 delay(100); // wait for a 1/10 second
 digitalWrite(led, LOW); // turn the LED off by making the voltage LOW
 delay(100); // wait for a 1/10 second
 delay(2000);
 tone(8,880,300); //start
 delay(300);
 tone(8,440,200); // 0 -
 delay(200);
 tone(8,880,200); // 1 -
 delay(200);
 tone(8,440,200); // 0 -
 delay(200);
 tone(8,880,200); // 1 -
 delay(200);
 tone(8,440,200); // 0 bit 1
 delay(200);
 tone(8,880,200); // 1 bit 2
 delay(200);
 tone(8,440,200); // 0 bit 3
 delay(200);
 tone(8,440,200); // 0 bit 4
 delay(200);
 tone(8,440,200); // 0 bit 5
 delay(200);
 tone(8,880,200); // 1 bit 6
 delay(200);
 tone(8,440,200); // 0 bit 7
 delay(200);
 tone(8,440,200); // 0 bit 8
 delay(200);
 tone(8,880,200); // 1 parity
 delay(200);
 tone(8,880,300); // stop
 delay(300);
 delay(200);
 digitalWrite(led, HIGH); // turn the LED on (HIGH is the voltage level)
 delay(100); // wait for a 1/10 second
 digitalWrite(led, LOW); // turn the LED off by making the voltage LOW
 delay(7000); // wait for 7 second
}
