
/*
 * Capacitive Sensor and Radio Tx for LoVid Reaction Bubble Project
 * by Tyler Henry
 * 
 * 10M Ohm resistor between pins 4 (send) and 8 (receive)
 * pin 8 is sensor pin
 * 
 */

#include <CapacitiveSensor.h>

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

/* RF24 NODE ADDRESS GOES HERE */
/*--------------------------------------------------------*/

// node address of this IR beam for RF24Network:
const uint16_t rfNode = 042;

// CAP sensors are addressed: 012, 022, 032, 042, 052, 03 (6th cap is child of 00)
// IR beams are addressed: 011, 021, 031, 041, 051
// Relays are addressed: 01 (IR), 02 (CAP)
// Master receiver Arduino Uno is base node: 00
/*--------------------------------------------------------*/

/* RF24Network setup */
/*--------------------------------------------------------*/
RF24 radio(9,10); // RF module on pins 9,10
RF24Network network(radio); // create the RF24Network object
/*--------------------------------------------------------*/


CapacitiveSensor cap = CapacitiveSensor(4,8);

void setup()                    
{

  SPI.begin();
  radio.begin(); // start the RF24 module
  network.begin(90, rfNode); // start the RF24 network on channel 90 with rfNode address

//  cap.reset_CS_AutoCal();
//  cap.set_CS_AutocaL_Millis(3000);

  cap.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate

}

void loop()                    
{

//    if (millis() > 10000 && millis() < 11000) {
//      cap.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate
//    }

    network.update();
    
    long val =  cap.capacitiveSensor(30); // 50 is # samples - can experiment w/ this

    if (val > 200){ // 200 is threshold, experiment w/ this
     sendRF(val); // send reading to master node
    }
    delay(50); // delay to limit Tx data
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



