#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

Process process;

void setup() {
  Bridge.begin();
}

void loop() {
//  String s = "curl -X POST -H \"Content-Type: application/json\" -H \"Accept: application/json\" -d  '{\"serial\":\"raspi\", \"latitude\":38.258595, \"longitude\":139.6950221}' http://192.168.11.143:3000/positions";
  String s = "ping www.google.com";
  process.runShellCommand(s);
  Serial.println("send");
  delay(1000);
}
