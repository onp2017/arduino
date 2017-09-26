#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = { 0x01, 0xAD, 0xBE, 0xEF, 0xFE, 0xAD };

IPAddress server_addr(192,168,20,212);  // IP of the MySQL *server* here

char user[] = "arduino";
char password[] = "adminadmin";
EthernetClient client;
MySQL_Connection conn((Client *)&client);
File myFile;

//parse data vals
String dataarr[120] = {};     //array of parsed data
int arrpointer = 0;       //array pointer
int column = 4;           //column to parse
int columncount = 1;      //start at column 1 not 0
int rowincrement = 2;     //increment the number of parse rows - set to 1 for all rows
int rowcount = 1;         //start at row 1 not 0
String datatemp;          //temp to store datapoint
char *fecha =new char[10];
char *hora =new char[10];
char *valor =new char[10];
char *unidad =new char[10];
char *estado =new char[10];
//String fecha;
//String hora;
//String valor;
//String unidad;
//String estado;
bool flag = false;
String input;


void setup(){
   
  Serial.begin(115200);
  Serial.println("Empezando!");
  Serial.println("Configurando MAC!");

  Serial.println("Configurando SD!");
  Ethernet.begin(mac_addr);
  pinMode(53, OUTPUT);
  if (!SD.begin(4)) {
    Serial.println("Initialization failed!");
    return;
  }
  myFile = SD.open("data.csv");
  
  if(myFile) {
    
    String l_line = "";
    //open the file here
    while (myFile.available() != 0) {  
        //A inconsistent line length may lead to heap memory fragmentation        
        input = myFile.readStringUntil('\n');  
        //Serial.println(input);  
        //LINEA LISTA
        if (flag == false){
        int i = 0;
        const int numberOfPieces = 5;
        String pieces[numberOfPieces];
        int counter = 0;
        int lastIndex = 0;
        for (i; i < input.length(); i++) {
        if (input.substring(i, i+1) == "," || input.substring(i, i+1) == '\n') {
          // Grab the piece from the last index up to the current position and store it
          pieces[counter] = input.substring(lastIndex, i);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          Serial.println(pieces[counter]);
          Serial.println(counter);
          counter++;
          }
 
      }
       pieces[counter] = input.substring(lastIndex, i);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          Serial.println(pieces[counter]);
          Serial.println(counter);
          flag = true;
        }else{
        int i = 0;
        const int numberOfPieces = 5;
        String pieces[numberOfPieces];
        int counter = 0;
        int lastIndex = 0;
        for (i; i < input.length(); i++) {
        if (input.substring(i, i+1) == "," || input.substring(i, i+1) == '\n') {
          // Grab the piece from the last index up to the current position and store it
          pieces[counter] = input.substring(lastIndex, i);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          Serial.println(pieces[counter]);
          Serial.println(counter);
          counter++;
 }
 
      }
       pieces[counter] = input.substring(lastIndex, i);
          // Update the last position and add 1, so it starts from the next character
          lastIndex = i + 1;
          if ( pieces[3] == "c" ){
            Serial.println("Es una c");
            char INSERT_DATA[] = "INSERT INTO Onp.temperatura (fecha, hora, valor, unidad, 1) VALUES (%s , %s, %s , %s)";
            char query[128];
            conn.connect(server_addr, 3306, user, password);
            if (conn.connect(server_addr, 3306, user, password)) {
              MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
              pieces[0].toCharArray(fecha, 10);
              pieces[1].toCharArray(hora, 10);
              pieces[2].toCharArray(valor, 10);
              pieces[3].toCharArray(unidad, 10);
              sprintf(query, INSERT_DATA, fecha, hora, valor, unidad);
              Serial.println(query);
              cur_mem->execute(query);
              delete cur_mem;
              Serial.println("Datos guardados.");
              conn.close();
            }else{
              Serial.println("Error de coneccion.");
              conn.close();
            }
          }
          if ( pieces[3] == "lux" ){
            Serial.println("Es una lux");
          }
           if ( pieces[3] == "ml" ){
            Serial.println("Es una ml");
          }
          Serial.println(pieces[counter]);
          Serial.println(counter);
        }

            
        if (input == "") //no blank lines are anticipated        
          break;      
        //  
    
       //parse l_line here
  } 
    
}
  
  
  myFile.close();
  
    //Ethernet.begin(mac_addr);
    Serial.println("Initialization done.");
}



void loop(void) {
  
}
