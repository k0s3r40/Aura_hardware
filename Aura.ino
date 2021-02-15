void setup() {
  Serial.begin(115200);
}
 
void loop() {
    float eco2 = random(0, 500000)/100.0;
    float voc = random(1, 300) / 100.0;
    float humidity = random(1, 10000)/100.0;
    float pressure = random(1, 300000) / 100.0;    
    float temp = random(1, 10000)/100.0;
    String data =  String(eco2)+";"+String(voc)+";"+String(humidity)+";"+String(pressure)+";"+String(temp)+";";
    int n = data.length();
    char char_array[n + 1];
    strcpy(char_array, data.c_str());
    Serial.write(char_array, n+1);

     Serial.println();
 
    delay(1000);
}
