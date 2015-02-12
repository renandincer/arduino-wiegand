#ifndef ARDUINOWIEGAND_H
#define ARDUINOWIEGAND_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#define MAX_BITS 40	// max number of bits
#define WAIT_TIME 3000  // wait time for an other tag

class ArduinoWiegand {
public:
	ArduinoWiegand();
	~ArduinoWiegand();
	void begin();
	bool read();
	unsigned long getFacilityCode();
	unsigned long getCardCode();



private:
	static void ISR_INT0();					//
	static void ISR_INT1();					//
	static unsigned char databits[MAX_BITS];	// stores all of the data bits
	static unsigned char bitCount;				// number of bits currently captured
	static unsigned char flagDone;				// goes low when data is currently being captured
	static unsigned int weigand_counter;		// countdown until we assume there are no more bits
	static unsigned long facilityCode;		// decoded facility code
	static unsigned long cardCode;			// decoded card code
};

#endif