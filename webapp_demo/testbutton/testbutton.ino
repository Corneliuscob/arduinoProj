
int pbutton = D2;
int ledpin = D3;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledpin,OUTPUT);
  pinMode(pbutton,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState  = digitalRead(pbutton);
  if(buttonState == HIGH){
    digitalWrite(ledpin,HIGH);
  }
  else{
    digitalWrite(ledpin,LOW);
  }
}
