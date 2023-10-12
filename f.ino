#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "senWiFi_Free";        // your network SSID (name)
const char* password = "sen2021!wi";      // your network password

WebServer server(80);

const int led = 13;
String InName = "Default Name";
String InNum = "Default Num";

String btn10 = "red";
String btn11 = "green";

int inbtn = 0;
#define BUFFER_SIZE 1024

void handleRoot() {
  digitalWrite(led, 1);
  
  char temp[BUFFER_SIZE];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  int day = hr / 24;

////////////////////////////////////////////////////////////
  String argname[5];
  String arg[5];
  for (uint8_t i = 0; i < server.args(); i++) {
    argname[i] = server.argName(i); 
    arg[i] = server.arg(i);
  }
  InName = arg[0];
  InNum = arg[1];

  Serial.println(argname[0]);
  Serial.println(argname[1]);
  Serial.println(InName);
  Serial.println(InNum);
////////////////////////////////////////////////////////////

  snprintf(temp, BUFFER_SIZE - 1,
           "<html>\
<head>\
<meta charset='UTF-8'>\
<!-- <meta http-equiv='refresh' content='5'/> -->\
<title>ESP32 Server 12</title>\
<style>\
body { background-color: #ffffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
</style>\
</head>\
<body>\
<h1>Hello from ESP32 Server</h1>\
<h3>동작 WiFiWebServer</h3>\
	<form action=\"/\" method=\"post\" id=\"sForm\">\
		이름 : <input type=\"text\" name=\"n1\" value=\"%s\"><br>\
		번호 : <input type=\"text\" name=\"n2\" value=\"%s\"><br>\
	</form>\
	<button type=\"submit\" form=\"sForm\">제출하기</button><br><br>\
	<form action=\"/\" method=\"post\" id=\"tForm1\">\
  		<button style=\"background-color:%s\" type=\"submit\" form=\"tForm1\" name=\"b1\" value=\"1\">On</button> <br>\
	</form>\
	<form action=\"/\" method=\"post\" id=\"tForm2\">\
  		<button style=\"background-color:%s\" type=\"submit\" form=\"tForm2\" name=\"b2\" value=\"0\">Off</button> <br>\
	</form>\
<p>Uptime: %d d %02d:%02d:%02d   num:%d</p>\
</body>\
</html>", InName, InNum, btn10, btn11, day, hr, min % 60, sec % 60, inbtn);

  server.send(200, F("text/html"), temp);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String argname[5];
  String arg[5];


  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    argname[i] = server.argName(i); 
    arg[i] = server.arg(i);
  }
  InName = arg[0];
  InNum = arg[1];

  handleRoot();

//  server.send(404, "text/plain", message);
  digitalWrite(led, 0);


  Serial.println(argname[0]);
  Serial.println(argname[1]);
  Serial.println(InName);
  Serial.println(InNum);

}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.on("/test", []() {
    server.send(200, "text/plain", "Test Message.");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
//  delay(2);//allow the cpu to switch to other tasks
}
