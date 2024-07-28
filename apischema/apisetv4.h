#pragma once
#include "stdafx.h"

// https://www.geoffchappell.com/studies/windows/win32/apisetschema/index.htm

typedef struct _API_SET_NAMESPACE_ENTRY_v4{
    DWORD flags;
    DWORD nameOffset;
	DWORD nameLength;
	DWORD aliasOffset;
	DWORD aliasLength;
    DWORD dataOffset; 
} API_SET_NAMESPACE_ENTRY_v4, *PAPI_SET_NAMESPACE_ENTRY_v4;

typedef struct _API_SET_VALUE_ARRAY_v4{
    DWORD flags;	// always 0
	DWORD count;
} API_SET_VALUE_ARRAY_v4, *PAPI_SET_VALUE_ARRAY_v4;

typedef struct _API_SET_VALUE_ENTRY_v4 {
	DWORD Flags;
	DWORD NameOffset;
	DWORD NameLength;
	DWORD ValueOffset;
	DWORD ValueLength;
} API_SET_VALUE_ENTRY_v4, *PAPI_SET_VALUE_ENTRY_v4;