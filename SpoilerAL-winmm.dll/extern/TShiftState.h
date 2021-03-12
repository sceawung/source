#pragma once

enum Classes__1
{
	ssShift  = 0x01,
	ssAlt    = 0x02,
	ssCtrl   = 0x04,
	ssLeft   = 0x08,
	ssRight  = 0x10,
	ssMiddle = 0x20,
	ssDouble = 0x40,
};

typedef struct ShiftState
{
	bool ssShift  : 1;
	bool ssAlt    : 1;
	bool ssCtrl   : 1;
	bool ssLeft   : 1;
	bool ssRight  : 1;
	bool ssMiddle : 1;
	bool ssDouble : 1;
	__int8 : 0;
} TShiftState;
