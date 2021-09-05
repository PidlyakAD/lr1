#define WINVER 0x0502
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

using namespace std;

#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define INFO_BUFFER_SIZE 32767
#define MAX_KEY_LENGTH 255

//2. Функциональность измерения производительности ЦП
//замер рабочей частоты ЦП 
__int64 f, t_1, t_2 = 0;

void CounterFreq()
{
    LARGE_INTEGER FR;
	QueryPerformanceFrequency(&FR);
    f = double(FR.QuadPart);
    printf("CPU frequency: %u Hz \n", FR);
	
    QueryPerformanceCounter(&FR);
    t_1 = FR.QuadPart;

}

//количество тактов ЦП, которое занимает выполнение программой первого пункта
double CounterTakt()
{
    LARGE_INTEGER TM;
    QueryPerformanceCounter(&TM);
    t_2 = TM.QuadPart;
    double takt = double((t_2 - t_1) * 1000000 / f);
    return takt;
}

int main()
{
    // 1.1
    // версия ОС
    OSVERSIONINFO osvi = {0};
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    printf("1.1.  OS: Windows v%d.%d  (build %d)", osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.dwBuildNumber);

    // 1.2.
    //системны каталоги
    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;
    GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE); 
    printf("\n1.2.  System directory: %s", infoBuf);

    // 1.3.
    //название компьютера
    bufCharCount = INFO_BUFFER_SIZE;
    GetComputerName(infoBuf, &bufCharCount);
    printf("\n1.3.  Computer Name: %s", infoBuf);

    //имя пользователя
    GetUserName(infoBuf, &bufCharCount);
    printf("\n      User Name: %s", infoBuf);
    
    //1.4. данные о томах
    char buffer[MAX_PATH];
    DWORD  CBufLn = MAX_PATH;
    char Names[MAX_PATH];

    __int64 total, available, free;
    
    HANDLE search = FindFirstVolume(Names, BUFSIZE);

    do {
        printf("\n      %s", Names);
        GetVolumePathNamesForVolumeName(Names, buffer, CBufLn, &CBufLn);
        char* path = buffer;
        printf("\n      First path: %s", path);
        GetDiskFreeSpaceEx(buffer, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
        if (GetDiskFreeSpaceEx(buffer, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free)==0) {
            printf("\n      No information available.\n ");   
        }
        else{
        printf("\n      Total size: %u  bytes ", total);
        printf("\n      Free space: %u  bytes\n", available);
        }
    
    } while (FindNextVolume(search, Names, BUFSIZE));
    FindVolumeClose(search);

    //1.5 список программ, запускаемых при старте системы, из реестра Windows
    HKEY hKey;
    DWORD dwIndex = 0;
    DWORD retCode;
    DWORD BufferSize = 8192;
    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
    DWORD cbData = BufferSize;
    DWORD dwSize;
    TCHAR ValueName[MAX_KEY_LENGTH];
    
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey)== ERROR_SUCCESS)
    {
		printf("\n1.5.  Startup programms:\n");
		while (1) 
		{
        dwSize = sizeof(ValueName);
        retCode = RegEnumValue(hKey, dwIndex, ValueName, &dwSize, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(hKey, ValueName, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("      %d: %s:  %s\n", dwIndex + 1, ValueName, PerfData);
            dwIndex++;
        }
        else break;
    }
    RegCloseKey(hKey);
    CounterFreq();
    cout << "2.2.  CPU clock count: " << CounterTakt()  << "  us \n";
    return 0;
}
}

