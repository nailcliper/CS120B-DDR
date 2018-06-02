#ifndef __tabula_h__
#define __tabula_h__

float get_Tabula_Tone(unsigned char track, unsigned char index);
unsigned long get_Tabula_Duration(unsigned char track, unsigned char index);
unsigned char get_Tabula_Size(unsigned char track);
unsigned char get_Tabula_Map(unsigned char index);
unsigned short get_Tabula_Map_Size();

#endif