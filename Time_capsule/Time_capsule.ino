int open_button=2;
int close_button=3;
int reed_switch=8;
int motor_direction1=12;
int motor_direction2=13;
int state=0;

// state=0, abierto
// state=1, verificar sise puiede cerrar
// 

void setup() {

  pinMode(motor_direction1,OUTPUT);
  pinMode(motor_direction2,OUTPUT);
  pinMode(open_button,INPUT);
  pinMode(close_button,INPUT);
   pinMode(reed_switch,INPUT);
  Serial.begin(9600);
  Serial.println("Start Time Capsule---");
  
}

void loop() {


  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  
  //Serial.print(sensorValue1);
  //Serial.print("    ");
  //Serial.println(sensorValue2);
  
  
  if(digitalRead(open_button)==HIGH){
    state=0;
  }

   if(digitalRead(close_button)==HIGH){ 
    state=1;
   }

   if(state==0){
    Serial.println("Abriendo Capsula");
    open_capsule();
    state=3;
   }

   if(state==1){
    //Verificar si se puede cerrar
    if(digitalRead(reed_switch)==LOW){
      Serial.println("Debe cerrarse la capsula para poner el seguro, vuelve a intentarlo cuando la cierres");
      state=0;
    }
    else{
      state=2;
    }
    
   }

   if (state==2){
     Serial.println("Cerrando Capsula");
    close_capsule();
    state=3;
   }

   
   
  
  delay(50);
}

void close_direction(){
  
  digitalWrite(motor_direction1,HIGH);
  digitalWrite(motor_direction2,LOW);
}


void open_direction(){
  
  digitalWrite(motor_direction1,LOW);
  digitalWrite(motor_direction2,HIGH);

  
}

void stop_motor(){
    digitalWrite(motor_direction1,LOW);
  digitalWrite(motor_direction2,LOW);
}

void open_capsule(){
  int sensorValue1 = analogRead(A0);
   while(sensorValue1>500){
   sensorValue1 = analogRead(A0);
   open_direction();
    }
    stop_motor();
}

void close_capsule()
{
  int sensorValue2 = analogRead(A1);
    while(sensorValue2>500){
    sensorValue2 = analogRead(A1);
    close_direction();
    }
    
    stop_motor();
}
