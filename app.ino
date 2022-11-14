#include <ArduinoSort.h>
#include "MIDIUSB.h"

int latchPin = 9;
int clockPin = 10;
int dataPin = 8;
byte leds = 0;
int lineNum = 0;
int oldNote = 0;
int asdf[3];
int notesInLine[40];
int notesInGroup[40];
int notesInOldGroup[40];
/*int notesInOldGroup[40] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,};*/

int scale_matrix[4][8] = {
	{0,2,2,1,2,2,2,1}, // major scale
	{0,2,1,2,2,1,2,2}, // natural minor scale
	{0,2,1,2,2,2,1,2}, // dorian mode
	{0,2,2,1,2,2,1,2}  // mixolydian mode
};

int useMode = 0;
int speed = 0;
int octave = 0;
int velocity = 0;
int steps = 0;
int scaleType = 0;
int arpeggiationStyle = 0;

void setup()
{
	Serial.begin(9600);
	/*Serial.begin(115200);*/

	pinMode(3,OUTPUT);
	pinMode(4,OUTPUT);
	pinMode(5,OUTPUT);
	pinMode(6,INPUT);

	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);  
	pinMode(clockPin, OUTPUT);
}

void loop()
{
	readKeyboard();
	lineToGroup();
	readInputs(); // not done

	switch (useMode)
	{
		case 0: // keyboard
			keyboardPlayNotes();
			break;
		case 1: // arpeggiator
				// arpeggiate premade scales
			arpeggiator();
			break;	
		case 2: // arpeggiator 2
				// arpeggiate user inputed chords
			arpeggiator_2();
			break;
	}
}
void readInputs()
{
	int bsdf[3];
	// add octave, 5th, 7th, 9th etc, use buttons
	speed = 50;
	octave = 0;
	velocity = 64;
	steps = 7;
	scaleType = 0; // major, minor, dorian, mixolydian
	arpeggiationStyle = 0;
	useMode = 0;
	for (int n = 0; n <= 4; n++)
	{
		for (int bit = 2; bit >= 0; bit--)
		{
			delay(2);
			bsdf[bit] = (bitRead(n, bit));
			digitalWrite(bit+11,bsdf[bit]);
			/*Serial.print(bsdf[bit]);*/
			/*Serial.print(" ");*/
		}
		delay(5);
		Serial.print(n);
		Serial.print(": ");
		Serial.print(analogRead(A0));
		Serial.print(" ");
	}
	Serial.print("======================");
	Serial.println();
	delay(10);
}
void arpeggiator_2()
{
	for (int i = 0; i < lineNum; i++)
	{
		arpeggiatorPlayNotes(notesInGroup[i]);
	}
}
void arpeggiator()
{
	int note = 0;
	if (notesInGroup[0] != 0)
	{
		oldNote = notesInGroup[0];
	}
	else
	{
		notesInGroup[0] = oldNote;
	}
	for (int i = 0; i < steps; i++)
	{
		note += scale_matrix[scaleType][i];
		arpeggiatorPlayNotes(notesInGroup[0] + note);
	}
	emptyGroups();
}
void arpeggiatorPlayNotes(int note)
{
	noteOn(0, note, velocity); // channel, note, velocity
	MidiUSB.flush();
	delay(speed);
	noteOff(0, note, velocity);
}
void readKeyboard()
{
	int num = 0;
	for (int i = 0; i <= 4; i++)
	{
		leds = 0;	
		bitSet(leds, i);	
		updateShiftRegister();
		for (int n = 7; n >= 0; n--)
		{
			for (int bit = 2; bit >= 0; bit--)
			{
				asdf[bit] = (bitRead(n, bit));
				digitalWrite(bit+3,asdf[bit]);

				/*Serial.print(asdf[bit]);*/
				/*Serial.print(" ");*/
			}
			if (digitalRead(6))
			{
				notesInLine[num] = 1;
			}
			else
			{
				notesInLine[num] = 0;
			}
			num += 1;
			/*Serial.println();*/
		}
		/*Serial.println("=========");*/
	}
}
void lineToGroup()
{
	int num = 0;
	// convert notesInLine to notes
	for (int b = 0; b < 40; b++)
	{
		/*Serial.print(notesInLine[b]);*/
		if (notesInLine[b] == 1)
		{
			switch (b)
			{
				case 0:
					notesInGroup[num] = 84;
					break;
				case 1:
					notesInGroup[num] = 80;
					break;
				case 2:
					notesInGroup[num] = 82;
					break;
				case 3:
					notesInGroup[num] = 78;
					break;
				case 4:
					notesInGroup[num] = 83;
					break;
				case 5:
					notesInGroup[num] = 79;
					break;
				case 6:
					notesInGroup[num] = 81;
					break;
				case 7:
					notesInGroup[num] = 77;
					break;
				case 8:
					notesInGroup[num] = 76;
					break;
				case 9:
					notesInGroup[num] = 72;
					break;
				case 10:
					notesInGroup[num] = 74;
					break;
				case 11:
					notesInGroup[num] = 70;
					break;
				case 12:
					notesInGroup[num] = 75;
					break;
				case 13:
					notesInGroup[num] = 71;
					break;
				case 14:
					notesInGroup[num] = 73;
					break;
				case 15:
					notesInGroup[num] = 69;
					break;
				case 16:
					notesInGroup[num] = 68;
					break;
				case 17:
					notesInGroup[num] = 64;
					break;
				case 18:
					notesInGroup[num] = 66;
					break;
				case 19:
					notesInGroup[num] = 62;
					break;
				case 20:
					notesInGroup[num] = 67;
					break;
				case 21:
					notesInGroup[num] = 63;
					break;
				case 22:
					notesInGroup[num] = 65;
					break;
				case 23:
					notesInGroup[num] = 61;
					break;
				case 24:
					notesInGroup[num] = 60;
					break;
				case 25:
					notesInGroup[num] = 56;
					break;
				case 26:
					notesInGroup[num] = 58;
					break;
				case 27:
					notesInGroup[num] = 54;
					break;
				case 28:
					notesInGroup[num] = 59;
					break;
				case 29:
					notesInGroup[num] = 55;
					break;
				case 30:
					notesInGroup[num] = 57;
					break;
				case 31:
					notesInGroup[num] = 53;
					break;
				case 32:
					notesInGroup[num] = 52;
					break;
				case 33:
					notesInGroup[num] = 48;
					break;
				case 34:
					notesInGroup[num] = 50;
					break;
				case 35:
					/*Serial.print("Not used");*/
					break;
				case 36:
					notesInGroup[num] = 51;
					break;
				case 37:
					/*Serial.print("Not used");*/
					break;
				case 38:
					notesInGroup[num] = 49;
					break;
				case 39:
					/*Serial.print("Not used");*/
					break;
			}
			num += 1;
		}
	}
	sortArray(notesInGroup, num);
	lineNum = num;
}
void keyboardPlayNotes()
{
	compareToOff(lineNum); // offs the non played notes
	compareToOn(lineNum);
	emptyGroups();
}
void emptyGroups()
{
	for (int i = 0; i < 40; i++)
	{
		notesInOldGroup[i] == NULL;

	}
	for (int i = 0; i < 40; i++)
	{
		notesInOldGroup[i] = notesInGroup[i];
		notesInGroup[i] = NULL;
	}
}
void updateShiftRegister()
{
	digitalWrite(latchPin, LOW);
	shiftOut(dataPin, clockPin, LSBFIRST, leds);
	digitalWrite(latchPin, HIGH);
}
void noteOn(byte channel, byte pitch, byte velocity)
{
	midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
	MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity)
{
	midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
	MidiUSB.sendMIDI(noteOff);
}
void compareToOff(int num)
{
	int notesToOff[40];
	int counter = 0;
	int flag = 0;

	for (int j = 0; j < sizeof(notesInOldGroup); j++)
	{
		flag = 0;
		for (int i = 0; i < num; i++)
		{
			if (notesInGroup[i] == notesInOldGroup[j])
			{
				flag = 1;
			}
		}
		if (flag == 0)
		{
			notesToOff[counter] = notesInOldGroup[j];
			counter += 1;
		}
	}
	for (int i = 0; i < counter; i++)
	{
		noteOff(0, notesToOff[i], velocity);
		MidiUSB.flush();
		notesToOff[i] = NULL;
	}
}
void compareToOn(int num)
{
	int notesToOn[40];
	int counter = 0;
	int flag = 0;

	for (int i = 0; i < num; i++)
	{
		flag = 0;
		for (int j = 0; j < 40; j++)
		{
			if (notesInGroup[i] == notesInOldGroup[j] && notesInOldGroup[j] != 0)
			{
				flag = 1;
			}
		}
		if (flag == 0)
		{
			notesToOn[counter] = notesInGroup[i];
			counter += 1;
		}

	}
	for (int i = 0; i < counter; i++)
	{
		noteOn(0, notesToOn[i], velocity);
		MidiUSB.flush();
		notesToOn[i] = NULL;
		flag = 2;
	}
}
