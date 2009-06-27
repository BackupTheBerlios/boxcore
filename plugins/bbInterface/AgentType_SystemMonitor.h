/*===================================================

	AGENTTYPE_SYSTEMMONITOR HEADERS

===================================================*/

//Multiple definition prevention
#ifndef BBInterface_AgentType_SystemMonitor_h
#define BBInterface_AgentType_SystemMonitor_h

//Includes
#include "AgentMaster.h"

#if defined(_MSC_VER) || defined(_WIN64)
#include <winternl.h>
#else
typedef enum _SYSTEM_INFORMATION_CLASS
{
	SystemBasicInformation = 0,SystemPerformanceInformation = 2,SystemTimeOfDayInformation = 3,SystemProcessInformation = 5,
	SystemProcessorPerformanceInformation = 8,SystemInterruptInformation = 23,SystemExceptionInformation = 33,SystemRegistryQuotaInformation = 37,
	SystemLookasideInformation = 45
} SYSTEM_INFORMATION_CLASS;

typedef struct _SYSTEM_BASIC_INFORMATION
{
	BYTE Reserved1[24];
	PVOID Reserved2[4];
	CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION,*PSYSTEM_BASIC_INFORMATION;

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION
{
	LARGE_INTEGER IdleTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER Reserved1[2];
	ULONG Reserved2;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION,*PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

typedef struct _SYSTEM_TIMEOFDAY_INFORMATION
{
	BYTE Reserved1[48];
} SYSTEM_TIMEOFDAY_INFORMATION,*PSYSTEM_TIMEOFDAY_INFORMATION;
#endif

//Define these structures
struct agenttype_systemmonitor_details
{
	char *internal_identifier;
	int monitor_type;
};

struct SYSTEM_TIME_INFORMATION
{
	LARGE_INTEGER liKeBootTime;
	LARGE_INTEGER liKeSystemTime;
	LARGE_INTEGER liExpTimeZoneBias;
	ULONG uCurrentTimeZoneId;
	BYTE Reserved1[48];
};

//Define these function pointer types
typedef BOOL (__stdcall *agenttype_systemmonitor_NtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

//Defines from the original
const int agenttype_systemmonitor_SystemBasicInformation = 0;
const int agenttype_systemmonitor_SystemPerformanceInformation = 2;
const int agenttype_systemmonitor_SystemTimeInformation = 3;

//Define these functions internally

int agenttype_systemmonitor_startup();
int agenttype_systemmonitor_shutdown();

int     agenttype_systemmonitor_create(agent *a, char *parameterstring);
int     agenttype_systemmonitor_destroy(agent *a);
int     agenttype_systemmonitor_message(agent *a, int tokencount, char *tokens[]);
void    agenttype_systemmonitor_notify(agent *a, int notifytype, void *messagedata);
void*   agenttype_systemmonitor_getdata(agent *a, int datatype);
void    agenttype_systemmonitor_menu_set(Menu *m, control *c, agent *a,  char *action, int controlformat);
void    agenttype_systemmonitor_menu_context(Menu *m, agent *a);
void    agenttype_systemmonitor_notifytype(int notifytype, void *messagedata);

#endif
/*=================================================*/
