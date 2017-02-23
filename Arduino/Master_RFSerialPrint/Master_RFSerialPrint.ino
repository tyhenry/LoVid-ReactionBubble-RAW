/*
* Master RF24Network Serial Printer for LoVid Reaction Bubble Project
* by Tyler Henry
* 
* Designed to work with Atmega328 (Uno)
*
* uses (TMRh20) RF24Network + RF24 libraries to receive messages over RF24Network
* prints payload (unsigned long) and transmitter node to serial
* 
*/

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

/* RF24Network setup */
/*--------------------------------------------------------*/

// node address of this arduino for RF24Network:
const uint16_t rfNode = 00;

// Master receiver Arduino Uno is base node: 00
// RF Relays are addressed 01 (IR), 02 (CAP)
// IR beams are addressed: 011, 021, 031, 041, 051 (children of RF Relay 01)
// Cap Sensors are addressed: 012, 022, 032, 042, 052, 03 (children of RF Relay 02, and 00)

RF24 radio(9,10); // RF module on pins 9,10
RF24Network network(radio); // create the RF24Network object

struct payload_t { // Structure of our payload
  unsigned long rfCode;
};
/*--------------------------------------------------------*/


void setup() {
  SPI.begin();
  radio.begin(); // start the RF24 module
  network.begin(90, rfNode); // start the RF24 network on channel 90 with rfNode address

  Serial.begin(115200);
  Serial.println("\nLOVID REACTION BUBBLE: MASTER RF SERIAL PRINT\n");
}

void loop() {

  network.update(); // check for packets on network
  
  while ( network.available() ) { // anything on the network for us?
    
    RF24NetworkHeader header;   // if so, grab it and print to serial
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    serialOut(header.from_node, payload.rfCode);
  }
}

void serialOut(uint16_t node, unsigned long code){
  // convert node address to header char ('A','B'...)
  char nChar = 0;
  
  // infrared beams
  if (node == 011) nChar = 'A';
  else if (node == 021) nChar = 'B';
  else if (node == 031) nChar = 'C';
  else if (node == 041) nChar = 'D';
  else if (node == 051) nChar = 'E';
  
  // cap bowls
  else if (node == 012) nChar = 'F';
  else if (node == 022) nChar = 'G';
  else if (node == 032) nChar = 'H';
  else if (node == 042) nChar = 'I';
  else if (node == 052) nChar = 'J';
  else if (node == 0152) nChar = 'K';
  else if (node == 03) nChar = 'K';

  // RF relays
  else if (node == 01) { nChar = 'R'; code = 1; } // IR
  else if (node == 02) { nChar = 'R'; code = 2; } // CAP

  // error
  else nChar = 'x';
    
  Serial.print(nChar);
  Serial.print(code);
  Serial.print('\n');
}


