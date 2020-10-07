void enviowifi(void){
  //--- programa----------------------

//    variable1=analogRead(A0);
//    variable2=3.14;
    //otras operaciones
    // . . . 
  
  
  //---------enviamos las variables al servidor---------------------
  
      //Nos conectamos con el servidor:
      
      Serial2.println("AT+CIPSTART=\"TCP\",""\"" + server + "\"" ",80");
      
      if( Serial2.find("OK"))
      {  
          Serial.println(BufEner1);
          Serial.println(BufEner2);
          Serial.println(BufEner3);
          Serial.println("\r\n\r\n\r\n ESP8266 conectado con el servidor...");             
       
      // int humidityData =20; int pulsos=21; int TemperatureC=22; 
         // int photocellReading=15;
          String peticionHTTP= "GET /pushingbox?devid=";
          peticionHTTP=peticionHTTP+devid; 
          peticionHTTP=peticionHTTP+ "&humidityData=" + (String)BufEner1 
          + "&celData=" + (String)FP1 
          + "&fehrData=" + (String)BufEner2
         + "&hicData=" + (String)FP2
        + "&hifData="  + (String)BufEner3 +" HTTP/1.1\r\n";

           peticionHTTP=peticionHTTP+"Host: api.pushingbox.com\r\n\r\n";
           peticionHTTP=peticionHTTP+"Connection: close\r\n\r\n" ;
            
          //Enviamos el tamaño en caracteres de la peticion http:  
          Serial2.print("AT+CIPSEND=");
          Serial2.println(peticionHTTP.length());
          delay(500);
          //esperamos a ">" para enviar la petcion  http
          if(Serial2.find(">")) // ">" indica que podemos enviar la peticion http
          {
            Serial.println("Enviando HTTP . . .");
            Serial2.println(peticionHTTP);
            if( Serial2.find("SEND OK"))
            {  
              Serial.println("Peticion HTTP enviada:");
              Serial.println();
              Serial.println(peticionHTTP);
              Serial.println("Esperando respuesta...");

    ///
           boolean fin_respuesta=false; 
              long tiempo_inicio=millis(); 
              cadena="";
              
              while(fin_respuesta==false)
              {
                  while(Serial2.available()>0) 
                  {
                      char c=Serial2.read();
                      Serial.write(c);
                      cadena.concat(c);  //guardamos la respuesta en el string "cadena"
                  }
                  //finalizamos si la respuesta es mayor a 500 caracteres
                  if(cadena.length()>500) //Pueden aumentar si tenen suficiente espacio en la memoria
                  {
                    Serial.println("La respuesta a excedido el tamaño maximo");
                    
                    Serial2.println("AT+CIPCLOSE");
                    if( Serial2.find("OK"))
                      Serial.println("Conexion finalizada");
                    fin_respuesta=true;
                  }
                  if((millis()-tiempo_inicio)>10000) //Finalizamos si ya han transcurrido 10 seg
                  {
                    Serial.println("Tiempo de espera agotado");
                    Serial2.println("AT+CIPCLOSE");
                    if( Serial2.find("OK"))
                      Serial.println("Conexion finalizada");
                    fin_respuesta=true;
                  }
                  if(cadena.indexOf("CLOSED")>0) //si recibimos un CLOSED significa que ha finalizado la respuesta
                  {
                    Serial.println();
                    Serial.println("Cadena recibida correctamente, conexion finalizada");         
                    fin_respuesta=true;
                  }
              }
    
              
            }
            else
            {
              Serial.println("No se ha podido enviar HTTP.....");
                
              
  ////
              }
                   
                     
          }
      }
      else
      {
        Serial.println("No se ha podido conectarse con el servidor");
      }

         
  //-------------------------------------------------------------------------------

 // delay(5000); //pausa de 10seg antes de conectarse nuevamente al servidor (opcional)
 
}

