
#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

/* RF24 NODE ADDRESS GOES HERE */
/*--------------------------------------------------------*/

// node address of this Arduino for RF24Network:
const uint16_t rfNode = 01;

// RF Relays are addressed 01 (IR), 02 (CAP)
// IR beams are addressed: 011, 021, 031, 041, 051 (children of RF Relay 01)
// Cap Sensors are addressed: 012, 022, 032, 042, 052, 0152 (children of RF Relay 02)
// Master receiver Arduino Uno is base node: 00
/*--------------------------------------------------------*/

/* RF24Network setup */
/*--------------------------------------------------------*/
RF24 radio(9,10); // RF module on pins 9,10
RF24Network network(radio); // create the RF24Network object
/*--------------------------------------------------------*/

unsigned long timer = 0;

void setup() {
  SPI.begin();
  radio.begin(); // start the RF24 module
  network.begin(90, rfNode); // start the RF24 network on channel 90 with rfNode address
}

void loop() {
   network.update();

   if (millis() - timer > 10000) {
     sendRF(1);
     timer = millis();
   }
}

/* RF24 Network */
struct payload_t { // structure of message payload
  unsigned long rfCode;
};

// sends RF packet to base node (address: 00)
void sendRF(unsigned long _rfCode){

  payload_t payload = {_rfCode};
  RF24NetworkHeader header(00); // node to send to (base node 00)
  network.write(header,&payload,sizeof(payload));
  
}
