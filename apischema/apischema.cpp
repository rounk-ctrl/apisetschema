// apischema.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "apisetv2.h"
#include "apisetv4.h"
#include "apisetv6.h"


void PrintSectionHeadersv2(LPCWSTR filePath) {

    HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    LPVOID lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpFileBase + dosHeader->e_lfanew);
    
    // get the section headers
    PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);
    int numberOfSections = ntHeaders->FileHeader.NumberOfSections;

	PIMAGE_SECTION_HEADER pApiSetSection = NULL;

    for (int i = 0; i < numberOfSections; ++i) {
		std::string name = std::string(reinterpret_cast<char*>(sectionHeader[i].Name), 8);
		if (name.find(".apiset") != std::string::npos)
		{
			pApiSetSection= &sectionHeader[i];
		}
    }
	if (pApiSetSection != NULL)
	{
		DWORD_PTR sectionAddress = (DWORD_PTR)lpFileBase + pApiSetSection->PointerToRawData;
		DWORD version = *(DWORD*)sectionAddress;
		if (version != 2) return;

		DWORD count = *(DWORD*)(sectionAddress+4);
		//BYTE* sectionData = (BYTE*)sectionAddress+4;
		//printf("%02X %02X %02X %02X\n", sectionData[0], sectionData[1], sectionData[2], sectionData[3]);
		
		std::wofstream outfile;
		outfile.open("apisetv2.txt", std::ios::trunc);

		outfile << "apisetschema version: " << version << std::endl;
		outfile << "no of redirects: " << count << std::endl << std::endl;

		// dllhostdescriptor		
		std::vector<DLLHOSTDESCRIPTOR> dllhost;
		int oldi=4;
		for (int i = 0; i < count; ++i)
		{
			DLLHOSTDESCRIPTOR di;
			di.OffsetDllString = *(DWORD*)(sectionAddress+oldi+(4*1));
			di.StringLength = *(DWORD*)(sectionAddress+oldi+(4*2));
			di.OffsetDllRedirector = *(DWORD*)(sectionAddress+oldi+(4*3));
			dllhost.push_back(di);
			oldi+=12;
		}

		for (int z = 0; z < count; ++z)
		{
			//std::cout << "0x"<<dllhost[z].OffsetDllString << ": "<< dllhost[z].StringLength << std::endl;

			// divide by sizeof(wchar_t) to fix weird issues
			std::wstring stringBuffer(reinterpret_cast<wchar_t*>(sectionAddress + dllhost[z].OffsetDllString), dllhost[z].StringLength / sizeof(wchar_t));
	
			DWORD redCount =  *(DWORD*)(sectionAddress+dllhost[z].OffsetDllRedirector);
			//outfile << "no of redirects: " << redCount << std::endl;

			outfile << stringBuffer << L" -> ";
			std::vector<std::wstring> dlls;
			for (int k = 0; k < redCount; ++k)
			{
				int structLength = 16;      // 4 members, each size 4
				REDIRECTION re;
				re.OffsetRedirection1 = *(DWORD*)(sectionAddress+dllhost[z].OffsetDllRedirector + 16*k + (4*1));
				re.RedirectionLength1 = *(DWORD*)(sectionAddress+dllhost[z].OffsetDllRedirector + 16*k + (4*2));
				re.OffsetRedirection2 = *(DWORD*)(sectionAddress+dllhost[z].OffsetDllRedirector + 16*k + (4*3));
				re.RedirectionLength2 = *(DWORD*)(sectionAddress+dllhost[z].OffsetDllRedirector + 16*k + (4*4));

				if (re.RedirectionLength1 != 0)
				{
					std::wstring value(reinterpret_cast<wchar_t*>(sectionAddress + re.OffsetRedirection1), re.RedirectionLength1 / sizeof(wchar_t));			
					if (std::find(dlls.begin(), dlls.end(), value) == dlls.end())
						dlls.push_back(value);
				}
				if (re.RedirectionLength2 != 0)
				{
					std::wstring value(reinterpret_cast<wchar_t*>(sectionAddress + re.OffsetRedirection2), re.RedirectionLength2 / sizeof(wchar_t));			
					if (std::find(dlls.begin(), dlls.end(), value) == dlls.end())
						dlls.push_back(value);
				}
			}
			outfile << "[";
			for (int l=0; l < dlls.size(); ++l)
			{
				outfile << dlls[l];
				if (l != dlls.size()-1)
					outfile << ", ";
			}
			outfile << "]" << std::endl;
		}
	}

    // Clean up
    UnmapViewOfFile(lpFileBase);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);
}

void PrintSectionHeadersv4(LPCWSTR filePath) {

    HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    LPVOID lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((DWORD_PTR)lpFileBase + dosHeader->e_lfanew);
    
    // get the section headers
    PIMAGE_SECTION_HEADER sectionHeader = IMAGE_FIRST_SECTION(ntHeaders);
    int numberOfSections = ntHeaders->FileHeader.NumberOfSections;

	PIMAGE_SECTION_HEADER pApiSetSection = NULL;

    for (int i = 0; i < numberOfSections; ++i) {
		std::string name = std::string(reinterpret_cast<char*>(sectionHeader[i].Name), 8);
		if (name.find(".apiset") != std::string::npos)
		{
			pApiSetSection= &sectionHeader[i];
		}
    }
	if (pApiSetSection != NULL)
	{
		DWORD_PTR sectionAddress = (DWORD_PTR)lpFileBase + pApiSetSection->PointerToRawData;
		DWORD version = *(DWORD*)sectionAddress;
		if (version != 4) return;

		std::wofstream outfile;
		outfile.open("apisetv4.txt", std::ios::trunc);

		DWORD size = *(DWORD*)(sectionAddress+4);
		DWORD count = *(DWORD*)(sectionAddress+12);

		outfile << "apisetschema version: " << version << std::endl;
		outfile << "size: " << size << std::endl;
		outfile << "no of redirects: " << count << std::endl << std::endl;

		// api namespace
		std::vector<API_SET_NAMESPACE_ENTRY_v4> dllhost;
		int oldi=12;
		for (int i = 0; i < count; ++i)
		{
			API_SET_NAMESPACE_ENTRY_v4 di;
			di.flags = *(DWORD*)(sectionAddress+oldi+(4*1));
			di.nameOffset = *(DWORD*)(sectionAddress+oldi+(4*2));
			di.nameLength = *(DWORD*)(sectionAddress+oldi+(4*3));
			di.aliasOffset = *(DWORD*)(sectionAddress+oldi+(4*4));
			di.aliasLength = *(DWORD*)(sectionAddress+oldi+(4*5));
			di.dataOffset = *(DWORD*)(sectionAddress+oldi+(4*6));
			dllhost.push_back(di);
			oldi+=24;
		}
		for (int z = 0; z < count; ++z)
		{
			API_SET_VALUE_ARRAY_v4 ar;
			ar.flags = *(DWORD*)(sectionAddress+dllhost[z].dataOffset);
			ar.count = *(DWORD*)(sectionAddress+dllhost[z].dataOffset+4);
			//std::cout << "count: "<< ar.count << std::endl;
			
			std::wstring stringBuffer(reinterpret_cast<wchar_t*>(sectionAddress + dllhost[z].nameOffset), dllhost[z].nameLength / sizeof(wchar_t));
			outfile << stringBuffer << L" -> ";

			std::vector<std::wstring> dlls;
			for (int k = 0; k < ar.count; ++k)
			{
				int structLength = 20;      // 5 members, each size 4
				API_SET_VALUE_ENTRY_v4 entry;
				entry.Flags = *(DWORD*)(sectionAddress+dllhost[z].dataOffset + 20*k + 8);
				entry.NameOffset = *(DWORD*)(sectionAddress+dllhost[z].dataOffset + 20*k + 12);
				entry.NameLength = *(DWORD*)(sectionAddress+dllhost[z].dataOffset + 20*k + 16);
				entry.ValueOffset = *(DWORD*)(sectionAddress+dllhost[z].dataOffset + 20*k + 20);
				entry.ValueLength = *(DWORD*)(sectionAddress+dllhost[z].dataOffset + 20*k + 24);
				
				std::wstring value(reinterpret_cast<wchar_t*>(sectionAddress + entry.ValueOffset), entry.ValueLength / sizeof(wchar_t));
				if (std::find(dlls.begin(), dlls.end(), value) == dlls.end())
						dlls.push_back(value);
			}
			outfile << "[";
			for (int l=0; l < dlls.size(); ++l)
			{
				outfile << dlls[l];
				if (l != dlls.size()-1)
					outfile << ", ";
			}
			outfile << "]" << std::endl;
		}
	}

    // Clean up
    UnmapViewOfFile(lpFileBase);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::cout << "\nDLL: apisetschema.dll" << std::endl << "=======================\n";
    PrintSectionHeadersv2(L"C:\\Windows\\system32\\apisetschema.dll");
    PrintSectionHeadersv4(L"C:\\Users\\rounak\\Desktop\\apisetschema8.1.dll");
	std::cout << std::endl;
	return 0;
}

