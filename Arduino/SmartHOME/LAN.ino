#include <EtherCard.h>

// ethernet interface mac address
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

static byte session;
byte Ethernet::buffer[700];
Stash stash;

const char website[] PROGMEM = WEBSITE;
const char webpath[] PROGMEM = WEBPATH;

void LANsetup () {
  Serial.println("\nLAN init");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
  
  ether.printIp("My IP: ", ether.myip);
  // ether.printIp("Netmask: ", ether.mymask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);

  if (!ether.dnsLookup(website)) {
    Serial.println(F("DNS failed"));
  }
  ether.printIp("ServerIP: ", ether.hisip);
  
}

static void sendToServer (String message) {
  Serial.println("Sending...");

  byte sd = stash.create();
  stash.print(message);
  stash.print("&apikey=");
  stash.println(APIKEY);
  stash.save();
  int stash_size = stash.size();

  // Compose the http POST request, taking the headers below and appending
  // previously created stash in the sd holder.
  Stash::prepare(PSTR("POST http://$F/$F HTTP/1.0" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  website, webpath, website, stash_size, sd);

  // send the packet - this also releases all stash buffers once done
  // Save the session ID so we can watch for it in the main loop.
  session = ether.tcpSend();

  for (int i=0; i <= 1000; i++){
    delay(10);
    LANloop();
  }
  
}

void LANloop () {
  ether.packetLoop(ether.packetReceive());

  const char* reply = ether.tcpReply(session);
  if (reply != 0) {
    Serial.println("Got a response!");
    Serial.println(reply);
  }
}
