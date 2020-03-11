// Pin Definitions
// Rows are connected to
const int row1 = 4;
const int row2 = 5;
const int row3 = 6;
const int row4 = 7;

// The 74HC595 uses a serial communication 
// link which has three pins
const int clock = 10;
const int latch = 9;
const int data = 8;


uint8_t keyToMidiMap[32];

boolean keyPressed[32];

int noteVelocity = 127;


// use prepared bit vectors instead of shifting bit left everytime
int bits[] = { B00000001, B00000010, B00000100, B00001000, B00010000, B00100000, B01000000, B10000000 };


// 74HC595 shift to next column
void scanColumn(int value) {
	digitalWrite(latch, LOW); //Pulls the chips latch low
	shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
	digitalWrite(latch, HIGH); //Pulls the latch high displaying the data
}

void setup() {
	
	// Map scan matrix buttons/keys to actual Midi note number. Lowest num 41 corresponds to F MIDI note.
	keyToMidiMap[0] = 60;
	keyToMidiMap[1] = 53;
	keyToMidiMap[2] = 54;
	keyToMidiMap[3] = 55;
	keyToMidiMap[4] = 56;
	keyToMidiMap[5] = 57;
	keyToMidiMap[6] = 58;
	keyToMidiMap[7] = 59;

	keyToMidiMap[8] = 52;
	keyToMidiMap[1 + 8] = 45;
	keyToMidiMap[2 + 8] = 46;
	keyToMidiMap[3 + 8] = 47;
	keyToMidiMap[4 + 8] = 48;
	keyToMidiMap[5 + 8] = 49;
	keyToMidiMap[6 + 8] = 50;
	keyToMidiMap[7 + 8] = 51;

	keyToMidiMap[16] = 44;
	keyToMidiMap[1 + 16] = 37;
	keyToMidiMap[2 + 16] = 38;
	keyToMidiMap[3 + 16] = 39;
	keyToMidiMap[4 + 16] = 40;
	keyToMidiMap[5 + 16] = 41;
	keyToMidiMap[6 + 16] = 42;
	keyToMidiMap[7 + 16] = 43;

	keyToMidiMap[24] = 36;
	keyToMidiMap[1 + 24] = 29;
	keyToMidiMap[2 + 24] = 30;
	keyToMidiMap[3 + 24] = 31;
	keyToMidiMap[4 + 24] = 32;
	keyToMidiMap[5 + 24] = 33;
	keyToMidiMap[6 + 24] = 34;
	keyToMidiMap[7 + 24] = 35;

	// setup pins output/input mode
	pinMode(data, OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(latch, OUTPUT);

	pinMode(row1, INPUT);
	pinMode(row2, INPUT);
	pinMode(row3, INPUT);
	pinMode(row4, INPUT);

    Serial.begin(9600);

	delay(1000);

}

void loop() {

	for (int col = 0; col < 8; col++) {
		
		// shift scan matrix to following column
		scanColumn(bits[col]);

		// check if any keys were pressed - rows will have HIGH output in this case corresponding
		int groupValue1 = digitalRead(row1);
		int groupValue2 = digitalRead(row2);
		int groupValue3 = digitalRead(row3);
		int groupValue4 = digitalRead(row4);

		// process if any combination of keys pressed
		if (groupValue1 != 0 || groupValue2 != 0 || groupValue3 != 0
				|| groupValue4 != 0) {

			if (groupValue1 != 0 && !keyPressed[col]) {
				keyPressed[col] = true;
				noteOn(0x91, keyToMidiMap[col], noteVelocity,col," row1 ");
			}

			if (groupValue2 != 0 && !keyPressed[col + 8]) {
				keyPressed[col + 8] = true;
				noteOn(0x91, keyToMidiMap[col + 8], noteVelocity,col," row2 ");
			}

			if (groupValue3 != 0 && !keyPressed[col + 16]) {
				keyPressed[col + 16] = true;
				noteOn(0x91, keyToMidiMap[col + 16], noteVelocity,col," row3 ");
                                

			}

			if (groupValue4 != 0 && !keyPressed[col + 24]) {
				keyPressed[col + 24] = true;
				noteOn(0x91, keyToMidiMap[col + 24], noteVelocity,col," row4 ");
			}

		}

		//  process if any combination of keys released
		if (groupValue1 == 0 && keyPressed[col]) {
			keyPressed[col] = false;
			noteOn(0x91, keyToMidiMap[col], 0,col," row1 ");
		}

		if (groupValue2 == 0 && keyPressed[col + 8]) {
			keyPressed[col + 8] = false;
			noteOn(0x91, keyToMidiMap[col + 8], 0,col," row2 ");
		}

		if (groupValue3 == 0 && keyPressed[col + 16]) {
			keyPressed[col + 16] = false;
			noteOn(0x91, keyToMidiMap[col + 16], 0,col," row3 ");
		}

		if (groupValue4 == 0 && keyPressed[col + 24]) {
			keyPressed[col + 24] = false;
			noteOn(0x91, keyToMidiMap[col + 24], 0,col," row4 ");
		}

	}

}


void noteOn(int cmd, int midiNote, int midiVelocity, int col, String row) {
        char buf1 [64]; // must be large enough for the whole string
        char buf2 [64]; 

  	Serial.write(cmd); // send note on, or note off
	Serial.write(midiNote);    // send pitch data
	Serial.write(midiVelocity);  // send velocity data
       // sprintf (buf1, "The col is: %d  and the row is ", col);
       
       
       // Serial.print(col);
       // Serial.print (row);
       // Serial.print("\r\n");
       // if(col == 1 && row == "row3" )
      //  {
        
          //  midiNote = 29;
          //  Serial.write(midiNote);
      //  }
                              

        
}
