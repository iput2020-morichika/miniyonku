  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
  server.on("/", handleRoot);
  server.on("/home/",handleRoot);                      //"home"は追加項目、入れないとWi-Fi通信できない場合がある。：ルートへのアクセス信号 
  server_8080.on("/stop", handle_stop);
  server_8080.on("/forward", handle_forward);
  server_8080.on("/back", handle_back);
  server_8080.on("/left", handle_left);
  server_8080.on("/right", handle_right);
  server_8080.on("/leftforward", handle_f_left);
  server_8080.on("/rightforward", handle_f_right);
  server_8080.on("/leftback", handle_b_left);
  server_8080.on("/rightback", handle_b_right);
 
  server.begin();
  server_8080.begin();
  
  Serial.println("HTTP server started");
  delay(2000);
