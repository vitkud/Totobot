#ifndef TotobotTinyFont_h
#define TotobotTinyFont_h

#include <Arduino.h>

const byte font4x4[256 * 2] PROGMEM = {
	//0, 1
	0b00000001,
	0b00000000,
	0b00000000,
	0b00000000,
	//2, 3
	0b00100011,
	0b00000000,
	0b00000000,
	0b00000000,
	//4, 5
	0b01000101,
	0b00000000,
	0b00000000,
	0b00000000,
	//6, 7
	0b01100111,
	0b00000000,
	0b00000000,
	0b00000000,
	//8, 9
	0b10001001,
	0b00000000,
	0b00000000,
	0b00000000,
	//10, 11
	0b10101011,
	0b00000000,
	0b00000000,
	0b00000000,
	//12, 13
	0b11001101,
	0b00000000,
	0b00000000,
	0b00000000,
	//14, 15
	0b11101111,
	0b00000000,
	0b00000000,
	0b00000000,
	//16, 17
	0b00000001,
	0b00010001,
	0b00000000,
	0b00000000,
	//18, 19
	0b00100011,
	0b00010001,
	0b00000000,
	0b00000000,
	//20, 21
	0b01000101,
	0b00010001,
	0b00000000,
	0b00000000,
	//22, 23
	0b01100111,
	0b00010001,
	0b00000000,
	0b00000000,
	//24, 25
	0b10001001,
	0b00010001,
	0b00000000,
	0b00000000,
	//26, 27
	0b10101011,
	0b00010001,
	0b00000000,
	0b00000000,
	//28, 29
	0b11001101,
	0b00010001,
	0b00000000,
	0b00000000,
	//30, 31
	0b11101111,
	0b00010001,
	0b00000000,
	0b00000000,
	//' ', '!'
	0b00000100,
	0b00000100,
	0b00000000,
	0b00000100,
	//'"', '#'
	0b10100101,
	0b00001110,
	0b00000111,
	0b00001010,
	//'$', '%'
	0b01001001,
	0b11100010,
	0b11100100,
	0b01001001,
	//'&', '''
	0b11100100,
	0b10100000,
	0b11110000,
	0b11010000,
	//'(', ')'
	0b00100100,
	0b01000010,
	0b01000010,
	0b00100100,
	//'*', '+'
	0b00000000,
	0b10100100,
	0b01001110,
	0b10100100,
	//',', '-'
	0b00000000,
	0b00000000,
	0b01001110,
	0b10000000,
	//'.', '/'
	0b00000010,
	0b00000100,
	0b00000100,
	0b01001000,
	//'0', '1'
	0b11110110,
	0b10110010,
	0b10010010,
	0b11110111,
	//'2', '3'
	0b11111111,
	0b00110111,
	0b11000001,
	0b11111111,
	//'4', '5'
	0b10011111,
	0b10011000,
	0b11111111,
	0b00010111,
	//'6', '7'
	0b10001111,
	0b11110001,
	0b10010001,
	0b11110001,
	//'8', '9'
	0b11111111,
	0b10011001,
	0b11111111,
	0b11110001,
	//':', ';'
	0b00000100,
	0b01000000,
	0b00000100,
	0b01001000,
	//'<', '='
	0b00000000,
	0b00101110,
	0b01000000,
	0b00101110,
	//'>', '?'
	0b00001110,
	0b01000110,
	0b00100000,
	0b01000100,
	//'@', 'A'
	0b11111111,
	0b10111001,
	0b10001111,
	0b11001001,
	//'B', 'C'
	0b11101111,
	0b11111000,
	0b10011000,
	0b11111111,
	//'D', 'E'
	0b11101111,
	0b10011110,
	0b10011000,
	0b11101111,
	//'F', 'G'
	0b11111111,
	0b10001000,
	0b11101001,
	0b10001111,
	//'H', 'I'
	0b10011110,
	0b10010100,
	0b11110100,
	0b10011110,
	//'J', 'K'
	0b01111011,
	0b00101100,
	0b10101010,
	0b11101001,
	//'L', 'M'
	0b10001111,
	0b10001011,
	0b10001001,
	0b11111001,
	//'N', 'O'
	0b10011111,
	0b11011001,
	0b10111001,
	0b10011111,
	//'P', 'Q'
	0b11111111,
	0b10011001,
	0b11111011,
	0b10001111,
	//'R', 'S'
	0b11111111,
	0b10011100,
	0b11110011,
	0b10101111,
	//'T', 'U'
	0b11111001,
	0b01001001,
	0b01001001,
	0b01001111,
	//'V', 'W'
	0b10011001,
	0b10011001,
	0b10011011,
	0b01101111,
	//'X', 'Y'
	0b10011001,
	0b01101001,
	0b01101111,
	0b10010100,
	//'Z', '['
	0b11110110,
	0b00100100,
	0b01000100,
	0b11110110,
	//'\', ']'
	0b10000110,
	0b01000010,
	0b01000010,
	0b00100110,
	//'^', '_'
	0b01000000,
	0b10100000,
	0b00000000,
	0b00001111,
	//'`', 'a'
	0b01001100,
	0b00100110,
	0b00001110,
	0b00000000,
	//'b', 'c'
	0b10001110,
	0b11101000,
	0b11101110,
	0b00000000,
	//'d', 'e'
	0b00101110,
	0b11101110,
	0b11101100,
	0b00000000,
	//'f', 'g'
	0b01110110,
	0b11101110,
	0b01000010,
	0b00001100,
	//'h', 'i'
	0b10000100,
	0b11100100,
	0b10100100,
	0b00000000,
	//'j', 'k'
	0b01001010,
	0b01001100,
	0b01001010,
	0b10000000,
	//'l', 'm'
	0b10001110,
	0b10001110,
	0b01101010,
	0b00000000,
	//'n', 'o'
	0b11001110,
	0b10101010,
	0b10101110,
	0b00000000,
	//'p', 'q'
	0b11101110,
	0b10101010,
	0b11101110,
	0b10000010,
	//'r', 's'
	0b11000110,
	0b10000100,
	0b10001100,
	0b00000000,
	//'t', 'u'
	0b01001010,
	0b11101010,
	0b01000110,
	0b00000000,
	//'v', 'w'
	0b10101010,
	0b10101110,
	0b01001110,
	0b00000000,
	//'x', 'y'
	0b10101010,
	0b01000110,
	0b10100010,
	0b00000100,
	//'z', '{'
	0b11000010,
	0b01001100,
	0b01101100,
	0b00000010,
	//'|', '}'
	0b01000100,
	0b01000011,
	0b01000011,
	0b01000100,
	//'~', 127
	0b00001111,
	0b01111111,
	0b10101111,
	0b00001111,
	//128, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//130, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//140, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//150, ...
	0b11111111,
	0b11111000,
	0b11110001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//160, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//170, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//180, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//190, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//200, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b01111000,
	0b11100001,
	0b11111111,
	//210, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//220, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//230, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//240, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//250, ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., ...
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
	//..., 255
	0b11111111,
	0b00011000,
	0b10000001,
	0b11111111,
};

#endif