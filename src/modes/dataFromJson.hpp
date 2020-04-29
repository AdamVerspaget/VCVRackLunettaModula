//----------------------------------------------------------------------------
//	Lunetta Modula Plugin for VCV Rack by Count Modula
//	common  functionality for handling loading of selected I/O mode
//  Copyright (C) 2020  Adam Verspaget
//----------------------------------------------------------------------------

json_t* jsonIOMode = json_object_get(root, "ioMode");

if (jsonIOMode)
	ioMode = json_integer_value(jsonIOMode);
else
	ioMode = VCVRACK_STANDARD;

setIOMode(ioMode);