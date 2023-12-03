void handleRoot() {
  String response;
  response = "<a href='/start' class=button> Sequence is stopped</a>";

  if (ProgramActive){response = "<a href='/start' class=button> Firework sequence is running</a>";}
  
  String style;
  style = "<style>"
        ".button {"
            "display: block; /* Change to block to make it full-width */"
            "width: 85vw;    /* Set width to 100% to stretch across the screen */"
			"padding: 10px 20px;"
            "text-align: center;"
            "text-decoration: none;"
            "font-size: 5vh;"
            "border: 1px solid #ccc;"
            "border-radius: 4px;"
            "background-color:" + String(ProgramActive ? "#f44444;" : "#44f444;") + 
            "color: #333;"
"}</style>\n";
  
  server.send(200, "text/html",style + response);
}
void handleStart() {
  ProgramActive = !ProgramActive;
  Serial.print(ProgramActive);
  server.sendHeader("Location","/"); 
  server.send(303, "text/html", "<h1>You are starting</h1>");
  Serial.println("Starting....");
}
