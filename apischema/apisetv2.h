#pragma once
#include "stdafx.h"

// https://blog.quarkslab.com/runtime-dll-name-resolution-apisetschema-part-ii.html

typedef struct _APISETMAP{
    DWORD Version;                    // dummy name (this field is never used)
    DWORD NumberOfHosts;              // number of DLLHOSTDESCRIPTOR structures following.
} APISETMAP, *PAPISETMAP;

typedef struct _DLLHOSTDESCRIPTOR{
    DWORD OffsetDllString;
    DWORD StringLength;
    DWORD OffsetDllRedirector; // offset to DLLREDIRECTOR
} DLLHOSTDESCRIPTOR, *PDLLHOSTDESCRIPTOR;
typedef struct _DLLREDIRECTOR{
    DWORD NumberOfRedirections; // Number of REDIRECTION structs.
} DLLREDIRECTOR, *PDLLREDIRECTOR;

typedef struct _REDIRECTION{
    DWORD OffsetRedirection1;
    USHORT RedirectionLength1;
    USHORT _pad1;
    DWORD OffsetRedirection2;
    USHORT RedirectionLength2;
    USHORT _pad2;
}REDIRECTION, *PREDIRECTION;