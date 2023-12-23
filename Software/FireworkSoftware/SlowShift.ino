void shiftOut2(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST) {
			digitalWrite(dataPin, val & 1);
			val >>= 1;
		} else {	
			digitalWrite(dataPin, (val & 128) != 0);
			val <<= 1;
		}
		delay(SHIFT_DELAY);	
		digitalWrite(clockPin, HIGH);
    delay(SHIFT_DELAY);
		digitalWrite(clockPin, LOW);		
	}
}
