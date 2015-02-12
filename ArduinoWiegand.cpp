#include "ArduinoWiegand.h" //include the declaration for this class

#define MAX_BITS 36                  // max number of bits 
#define WEIGAND_WAIT_TIME  3000      // time to wait for another weigand pulse.  

unsigned char _databits[MAX_BITS];    // stores all of the data bits
unsigned char _bitCount;              // number of bits currently captured
unsigned char _flagDone;              // goes low when data is currently being captured
unsigned int _weigand_counter;        // countdown until we assume there are no more bits
unsigned long _facilityCode = 0;        // decoded facility code
unsigned long _cardCode = 0;            // decoded card code

//<<constructor>>
ArduinoWiegand::ArduinoWiegand(){
}


void ArduinoWiegand::begin(){
	pinMode(2, INPUT); //make that pin an INPUT (INT0)
	pinMode(3, INPUT); //make that pin an INPUT (INT1)
	attachInterrupt(0, ISR_INT0, FALLING); //PIN 2
	attachInterrupt(1, ISR_INT1, FALLING); //PIN 3
	_weigand_counter = WEIGAND_WAIT_TIME;
	Serial.println("ArduinoWiegand - Ready");

}


//<<destructor>>
ArduinoWiegand::~ArduinoWiegand(){/*nothing to destruct*/}

bool ArduinoWiegand::read(){
	// This waits to make sure that there have been no more data pulses before processing data
	if (!_flagDone) {
		if (--_weigand_counter == 0){
			_flagDone = 1;
			return false;
		}
		return false;
	} else {
	// if we have bits and we the weigand counter went out
		if (_bitCount > 0 && _flagDone) {
			//cleanup before reading
			_facilityCode = 0;
			_cardCode = 0;

			unsigned char i;

			if (_bitCount == 36){
				for (i=2; i<14; i++){
					_facilityCode <<=1;
					_facilityCode |= _databits[i];
				}
				for (i=14; i<34; i++){
					_cardCode <<=1;
					_cardCode |= _databits[i];
				}
				// cleanup after reading
				_bitCount = 0;
				for (i=0; i<MAX_BITS; i++){
					_databits[i] = 0;
				}
				return true;
			}
			else{
				Serial.println("Unable to decode.");
				// cleanup after reading
				_bitCount = 0;
				for (i=0; i<MAX_BITS; i++){
					_databits[i] = 0;
				}
				return false;
			}
		}
	}
}

unsigned long ArduinoWiegand::getFacilityCode(){
	return _facilityCode;
}

unsigned long ArduinoWiegand::getCardCode(){
	return _cardCode;
}


// interrupt that happens when INTO goes low (0 bit)
void ArduinoWiegand::ISR_INT0()
{
	Serial.print("0");
	_bitCount++;
	_flagDone = 0;
	_weigand_counter = WEIGAND_WAIT_TIME;  

}

// interrupt that happens when INT1 goes low (1 bit)
void ArduinoWiegand::ISR_INT1()
{
	Serial.print("1");
	_databits[_bitCount] = 1;
	_bitCount++;
	_flagDone = 0;
	_weigand_counter = WEIGAND_WAIT_TIME;  
}