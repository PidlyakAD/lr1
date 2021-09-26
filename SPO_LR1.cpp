#pragma hdrstop
#pragma argsused
#include <windows.h>

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdio.h>
#include <conio.h>
#include <ctype.h>

int _tmain(int argc, _TCHAR* argv[])
{
char svBuffer[512];
DWORD dwBufSize;
char svReply[512];

dwBufSize = MAX_COMPUTERNAME_LENGTH+1;
if(!GetComputerName(svBuffer, &dwBufSize))
printf("Could not retrieve machine name.\n");
else
 {
wsprintf(svReply, "System info for machine '%.400s'\n", svBuffer);
printf(svReply);
 }

dwBufSize = 512;
if(!GetUserName(svBuffer, &dwBufSize))
printf("Could not retrieve user name.\n");
else
 {
wsprintf(svReply, "Current user: '%.400s'\n", svBuffer);
printf(svReply);
 }

// Send back processor info
SYSTEM_INFO sysInfo;
lstrcpy(svReply, "Processor: ");

GetSystemInfo(&sysInfo);
switch (sysInfo.dwProcessorType)  {
case PROCESSOR_INTEL_386:
lstrcat(svReply, "I386\n");
break;
case PROCESSOR_INTEL_486:
lstrcat(svReply, "I486\n");
break;
case PROCESSOR_INTEL_PENTIUM:
lstrcat(svReply, "I586\n");
break;
case PROCESSOR_MIPS_R4000:
lstrcat(svReply, "MIPSR4000\n");
break;
default:
lstrcat(svReply, "UNKNOWN\n");
break;
 }

printf(svReply);

OSVERSIONINFO osvi;// Send back OS version info

osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
if (GetVersionEx(&osvi) == FALSE)
lstrcpy(svReply, "Could not get version info.\n");
else  {
switch(osvi.dwPlatformId)  {
case VER_PLATFORM_WIN32s:
lstrcpy( svBuffer, "Win32s on Windows 3.1");
break;
case VER_PLATFORM_WIN32_WINDOWS:
lstrcpy( svBuffer, "Win32 on Windows 95");
break;
case VER_PLATFORM_WIN32_NT:
lstrcpy( svBuffer, "Windows NT");
break;
default:
lstrcpy( svBuffer, "Windows?");
break;
 }
wsprintf( svReply, "%s v%d.%d build %d\n", svBuffer, (int)osvi.dwMajorVersion, (int)osvi.dwMinorVersion, (int)LOWORD(osvi.dwBuildNumber));
if(lstrlen(osvi.szCSDVersion))  {
lstrcat(svReply, " - ");
lstrcat(svReply, osvi.szCSDVersion);
lstrcat(svReply, "\n");
 }
 }
printf(svReply);

// Send back global memory usage
MEMORYSTATUS memstat;
DWORD dw,dw2,dw3,dw4;
char c;
int x;

memstat.dwLength = sizeof(memstat);

GlobalMemoryStatus(&memstat);
wsprintf(svReply, "Memory: %dM in use: %d%% Page file: %dM free: %dM\n", memstat.dwTotalPhys/1024/1024, memstat.dwMemoryLoad, memstat.dwTotalPageFile/1024/1024, memstat.dwAvailPageFile/1024/1024 );
printf(svReply);

for (c = 'C'; c <= 'Z'; c++)  {
wsprintf(svReply, "%c:\\", c);
x = GetDriveType(svReply);
lstrcat( svReply, " - ");
switch (x)  {
case 0:
lstrcat(svReply, "Unable to determine.\n");
break;
case 1:
svReply[0]='\0';
break;
case DRIVE_REMOVABLE:
lstrcat(svReply, "Removable\n");
break;
case DRIVE_FIXED:
lstrcat(svReply, "Fixed");
wsprintf(svBuffer, "%c:\\", c);
if (GetDiskFreeSpace(svBuffer, &dw, &dw2, &dw3, &dw4))  {
wsprintf(svBuffer, " Sec/Clust: %u Byts/Sec: %u, Bytes free: %u/%u\n", (unsigned int)dw, (unsigned int)dw2, (unsigned int)(dw3*dw2*dw), (unsigned int)(dw4*dw2*dw));
lstrcat(svReply, svBuffer);
 }  else lstrcat(svReply,"\n");
break;
case DRIVE_REMOTE:
lstrcat(svReply, "Remote\n");
break;
case DRIVE_CDROM:
lstrcat(svReply, "CD-ROM\n");
break;
case DRIVE_RAMDISK:
lstrcat(svReply, "Ramdisk\n");
break;
default:
lstrcat(svReply, "Unknown type!\n");
break;
 }
if(lstrlen(svReply))
printf(svReply);
 }
getch();
	system("pause");

}
