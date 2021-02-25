 
void setup() {
  Serial.begin(115200);
  String START = "BEGIN";
  String END = "END";
}
 
void loop() {
    int eco2 = random(0, 5000);
    int voc = random(1, 300);
    int humidity = random(1, 100);
    int pressure = random(1, 3000);    
    int temp = random(1, 100);
    
    serialize_data("START", 1);
    serialize_data("ECO2", eco2);
    serialize_data("VOC", voc);
    serialize_data("HUM", humidity);
    serialize_data("PSI", pressure);
    serialize_data("TMP", temp);                
    serialize_data("END", 1);
    delay(1000);
}

void serialize_data(String prop_name, int value){

    String data = prop_name+":"+String(value)+"_";
    int n = data.length();
    char char_array[n + 1];
    strcpy(char_array, data.c_str());
    Serial.write(char_array, n+1);
    Serial.println();
    delay(100);
  }



