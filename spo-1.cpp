#define WINVER 0x0502
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#define SIZE 32767
#define TOTALBYTES 8192
#define BSIZE MAX_PATH
#define MAX_VALUE_NAME 16383
using namespace std;

void ch_takt()
{
    LARGE_INTEGER chas,time1, time2;
    double takt;
    QueryPerformanceCounter(&time1);
    QueryPerformanceFrequency(&chas);
    QueryPerformanceCounter(&time2);
    takt = (double(time2.QuadPart - time1.QuadPart) * 1000000 / chas.QuadPart);
    printf("Frequency:%lld Hz \n", chas);
    printf("Kolich taktov: %1.1f mcs\n", takt);

}

int main()
{
    setlocale(LC_ALL, "RUS");
    
    //1)version
    OSVERSIONINFO OS_ver;
    OS_ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&OS_ver);
    printf("1.1) Versia Windows OC:%i.%i\n", OS_ver.dwMajorVersion, OS_ver.dwMinorVersion);
    
    //2)directory
    TCHAR  Buf[SIZE];
    GetSystemDirectory(Buf, SIZE);
    printf("System directory: %s\n", Buf);

    //3)compname
    DWORD siz=sizeof(Buf);
    GetComputerName(Buf, &siz);
    printf("Computer name: %s\n", Buf);

    //username
    GetUserName(Buf, &siz);
    printf("Username: %s\n", Buf);

    //4)disks

    char buffer[MAX_PATH];
    DWORD BufferLengthe= MAX_PATH;
    char Names[MAX_PATH];
    ULARGE_INTEGER total, available, free;

    HANDLE search = FindFirstVolume(buffer, sizeof(buffer));


    do {
        printf("\n%s\n", buffer);
        GetVolumePathNamesForVolumeName(Names, buffer, BufferLengthe, &BufferLengthe);
        
        printf("First path: %s\n", Names);
        
        if (GetDiskFreeSpaceEx(buffer, &available, &total, &free) != 0)
        {
            printf("Total size: %I64u  bytes\n", total.QuadPart);
            printf("Free space: %I64u  bytes\n", free.QuadPart);
        }
        else
        {
            printf("Information unavailable\n");
        }

    } while (FindNextVolume(search, buffer, sizeof(buffer)));

    FindVolumeClose(search);

    //5)Startup programs
	cout << "Run programs:" << endl;
    HKEY hKey = HKEY_CURRENT_USER;
    LPCTSTR Sub = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    long Res = RegOpenKeyEx(hKey, Sub, 0, KEY_READ, &hKey);

    DWORD i = 0;
    TCHAR tchValue[MAX_VALUE_NAME];
    DWORD dwValue;
    if (Res == ERROR_SUCCESS) {
        while(1) {
            dwValue = MAX_VALUE_NAME;
            if (RegEnumValue(hKey, i, tchValue, &dwValue, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
                cout << tchValue << endl;
                i++;
            } else {
                break;
            }
        }
    } else {
        cout << "Error" << endl;
    }
    RegCloseKey(hKey);


    ch_takt();
    return 0;
}
