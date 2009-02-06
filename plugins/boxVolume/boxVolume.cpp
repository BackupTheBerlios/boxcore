/*
 * boxVolume.cpp
 *
 *  Created on: 22 Jan 2009
 *      Author: shaun
 */

#include "BBApi.h"
#include <mmsystem.h>
#include <ole2.h>

LPCSTR g_pluginName = "boxVolume";
HWND g_pluginWnd = NULL;
HINSTANCE g_pluginInstance = NULL;

static int VistaGetVolume();
static void VistaSetVolume(int nVol);
static void VistaSetMute(bool bMute);
static bool VistaGetMute();

static int GetVolume();
static void SetVolume(int nVol);
static void SetMute(bool bMute);
static bool GetMute();

static bool IsVista();

extern "C" void BBSetVolume(int nVol)
{
	if (IsVista())
		VistaSetVolume(nVol);
	else
		SetVolume(nVol);
}

extern "C" int BBGetVolume()
{
	if (IsVista())
		return VistaGetVolume();
	else
		return GetVolume();
}

extern "C" void BBToggleMute()
{
	if (IsVista())
		VistaSetMute(!VistaGetMute());
	else
		SetMute(!GetMute());
}

extern "C" void BBSetMute(bool bMute)
{
	if (IsVista())
		VistaSetMute(bMute);
	else
		SetMute(bMute);
}

extern "C" bool BBGetMute()
{
	if (IsVista())
		return VistaGetMute();
	else
		return GetMute();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT pluginMessages[] = {BB_BROADCAST, 0};
	switch (message)
	{
	case BB_BROADCAST:
	{
		LPCSTR msgString = reinterpret_cast<LPCSTR>(lParam);
		msgString++;
		if (!strnicmp(msgString, g_pluginName, strlen(g_pluginName)))
		{
			msgString += strlen(g_pluginName) + 1;
			LPCSTR element = NULL;
			if ((element = "ChangeVolume") && !strnicmp(msgString, element, strlen(element)))
			{
				msgString += strlen(element);
				BBSetVolume(BBGetVolume() + atoi(msgString));
				return 0;
			}
			else if ((element = "SetVolume") && !strnicmp(msgString, element, strlen(element)))
			{
				msgString += strlen(element);
				BBSetVolume(atoi(msgString));
				return 0;
			}
			else if ((element = "ToggleMute") && !strnicmp(msgString, element, strlen(element)))
			{
				BBToggleMute();
				return 0;
			}
			else if ((element = "Mute") && !strnicmp(msgString, element, strlen(element)))
			{
				BBSetMute(true);
				return 0;
			}
			else if ((element = "UnMute") && !strnicmp(msgString, element, strlen(element)))
			{
				BBSetMute(false);
				return 0;
			}
		}
	}

	case WM_CREATE:
		SendMessage(GetBBWnd(), BB_REGISTERMESSAGE, (WPARAM)hwnd, (LPARAM)pluginMessages);
		break;
	case WM_DESTROY:
		SendMessage(GetBBWnd(), BB_UNREGISTERMESSAGE, (WPARAM)hwnd, (LPARAM)pluginMessages);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

int beginPlugin(HINSTANCE hInstance)
{
	return beginPluginEx(hInstance, NULL);
}

int beginSlitPlugin(HINSTANCE hInstance, HWND hSlit)
{
	return beginPluginEx(hInstance, hSlit);
}

int beginPluginEx(HINSTANCE hInstance, HWND hSlit)
{
	g_pluginInstance = hInstance;
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = g_pluginName;
	RegisterClass(&wc);
	g_pluginWnd = CreateWindowEx(WS_EX_TOOLWINDOW, g_pluginName, g_pluginName, WS_POPUP, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
	return 0;
}

void endPlugin(HINSTANCE hInstance)
{
	DestroyWindow(g_pluginWnd);
	UnregisterClass(g_pluginName, g_pluginInstance);
}

LPCSTR pluginInfo(int index)
{
	switch (index)
	{
	case PLUGIN_NAME:
		return "boxVolume";
	case PLUGIN_VERSION:
		return "1.0.0";
	case PLUGIN_AUTHOR:
		return "Carsomyr";
	case PLUGIN_RELEASEDATE:
		return "2009/01/22";
	case PLUGIN_LINK:
		return "None";
	case PLUGIN_EMAIL:
		return "carsomyr@users.berlios.de";
	default:
		return "boxVolume 1.0.0";
	}
}

static bool IsVista()
{
	OSVERSIONINFO osInfo;
	ZeroMemory(&osInfo, sizeof(osInfo));
	osInfo.dwOSVersionInfoSize = sizeof(osInfo);
	GetVersionEx(&osInfo);
	bool typeNT = osInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
	return typeNT && osInfo.dwMajorVersion >= 6;
}

enum EDataFlow { eRender, eCapture, eAll, EDataFlow_enum_count };
enum ERole {eConsole, eMultimedia, eCommunications, ERole_enum_count};

#define DEVICE_STATE_ACTIVE 0x00000001;
#define DEVICE_STATE_DISABLED 0x00000002;
#define DEVICE_STATE_NOTPRESENT 0x00000004;
#define DEVICE_STATE_UNPLUGGED 0x00000008;
#define DEVICE_STATEMASK_ALL 0x0000000F;

struct IMMNotificationClient : public IUnknown
{
};

#ifndef PROPERTYKEY_DEFINED
#define PROPERTYKEY_DEFINED
typedef struct
{
	GUID fmtid;
	DWORD pid;
} PROPERTYKEY;
#endif
typedef PROPERTYKEY &REFPROPERTYKEY;

#ifdef __MINGW32__
typedef PROPVARIANT &REFPROPVARIANT;
#endif

#ifndef __IPropertyStore_INTERFACE_DEFINED__
#define __IPropertyStore_INTERFACE_DEFINED__
struct IPropertyStore : public IUnknown
{
	virtual HRESULT GetCount(DWORD *cProps);
	virtual HRESULT GetAt(DWORD iProp, PROPERTYKEY *pkey);
	virtual HRESULT GetValue(REFPROPERTYKEY key,PROPVARIANT *pv);
	virtual HRESULT SetValue(REFPROPERTYKEY key,REFPROPVARIANT propvar);
	virtual HRESULT Commit();
};
#endif

struct IMMDeviceCollection : public IUnknown
{
};

struct IAudioEndpointVolumeCallback : public IUnknown
{
};

struct IAudioEndpointVolume : public IUnknown
{
public:
	virtual HRESULT WINAPI RegisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify);
	virtual HRESULT WINAPI UnregisterControlChangeNotify(IAudioEndpointVolumeCallback *pNotify);
	virtual HRESULT WINAPI GetChannelCount(UINT *pnChannelcount);
	virtual HRESULT WINAPI SetMasterVolumeLevel(float fLevelDB, LPCGUID pguidEventContext);
	virtual HRESULT WINAPI SetMasterVolumeLevelScalar(float fLevel, LPCGUID pguidEventContext);
	virtual HRESULT WINAPI GetMasterVolumeLevel(float *pfLevelDB);
	virtual HRESULT WINAPI GetMasterVolumeLevelScalar(float *pfLevel);
	virtual HRESULT WINAPI SetChannelVolumeLevel(UINT nChannel, float fLevelDB, LPCGUID pguidEventContext);
	virtual HRESULT WINAPI SetChannelVolumeLevelScalar(UINT nChannel, float fLevel, LPCGUID pguidEventContext);
	virtual HRESULT WINAPI GetChannelVolumeLevel(UINT nChannel, float *pfLevelDB);
	virtual HRESULT WINAPI GetChannelVolumeScalar(UINT nChannel, float*pfLevel);
	virtual HRESULT WINAPI SetMute(BOOL bMute, LPCGUID pguidEventContext);
	virtual HRESULT WINAPI GetMute(BOOL *pMute);
	virtual HRESULT WINAPI GetVolumeStepInfo(UINT *pnStep, UINT *pnStepCount);
	virtual HRESULT WINAPI VolumeStepUp(LPCGUID pguidEventContext);
	virtual HRESULT WINAPI VolumeStepDown(LPCGUID pguidEventContext);
	virtual HRESULT WINAPI QueryHardwareSupport(DWORD *pdwHardwareSupportMask);
	virtual HRESULT WINAPI GetVolumeRange(float *pfLevelMinDB, float *pfLevelMaxDB, float *pfLevelIncrementDB);
};

struct IMMDevice : public IUnknown
{
public:
	virtual HRESULT WINAPI Activate(REFIID iid, DWORD dwClsCtx, PROPVARIANT *pActivationParams, void **ppInterface);
	virtual HRESULT WINAPI OpenPropertyStore(DWORD stgmAccess, IPropertyStore *ppProperties);
	virtual HRESULT WINAPI GetId(LPWSTR *pwstrId);
	virtual HRESULT WINAPI GetState(DWORD *pwdState);
};

struct IMMDeviceEnumerator : public IUnknown
{
public:
	virtual HRESULT WINAPI EnumAudioEndPoints(EDataFlow dataFlow, DWORD dwStateMask, IMMDeviceCollection **ppDevices);
	virtual HRESULT WINAPI GetDefaultAudioEndpoint(EDataFlow dataFlow, ERole role, IMMDevice **ppDevice);
	virtual HRESULT WINAPI GetDevice(LPCWSTR pwstrId, IMMDevice **ppDevice);
	virtual HRESULT WINAPI RegisterEndpointNotificationCallback(IMMNotificationClient *pNotify);
	virtual HRESULT WINAPI UnregisterEndpointNotificationCallback(IMMNotificationClient *pNotify);

};

static int VistaGetVolume()
{
	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CLSID CLSID_MMDeviceEnumerator =
		{ 0xBCDE0395, 0xE52F, 0x467C,
		  { 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E }
		};
	IID IID_IMMDeviceEnumerator =
		{ 0xA95664D2, 0x9614, 0x4F35,
		  { 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 }
		};
	IID IID_IAudioEndpointVolume =
		{ 0x5CDF2C82, 0x841E, 0x4546,
		  { 0x97, 0x22, 0x0C, 0xF7, 0x40, 0x78, 0x22, 0x9A }
		};
	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER,
						  IID_IMMDeviceEnumerator, (LPVOID *) &deviceEnumerator);

	IMMDevice *defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole,
			&defaultDevice);

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(IID_IAudioEndpointVolume,
								 CLSCTX_INPROC_SERVER, NULL, (LPVOID *) &endpointVolume);

	float volume;
	hr = endpointVolume->GetMasterVolumeLevelScalar(&volume);
	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();
	return volume * 100;
}

static void VistaSetVolume(int nVol)
{
	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CLSID CLSID_MMDeviceEnumerator =
		{ 0xBCDE0395, 0xE52F, 0x467C,
		  { 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E }
		};
	IID IID_IMMDeviceEnumerator =
		{ 0xA95664D2, 0x9614, 0x4F35,
		  { 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 }
		};
	IID IID_IAudioEndpointVolume =
		{ 0x5CDF2C82, 0x841E, 0x4546,
		  { 0x97, 0x22, 0x0C, 0xF7, 0x40, 0x78, 0x22, 0x9A }
		};
	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER,
						  IID_IMMDeviceEnumerator, (LPVOID *) &deviceEnumerator);

	IMMDevice *defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole,
			&defaultDevice);

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(IID_IAudioEndpointVolume,
								 CLSCTX_INPROC_SERVER, NULL, (LPVOID *) &endpointVolume);

	float volume;
	volume = ((double) nVol) / 100;
	endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();
}

static void VistaSetMute(bool bMute)
{
	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CLSID CLSID_MMDeviceEnumerator =
		{ 0xBCDE0395, 0xE52F, 0x467C,
		  { 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E }
		};
	IID IID_IMMDeviceEnumerator =
		{ 0xA95664D2, 0x9614, 0x4F35,
		  { 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 }
		};
	IID IID_IAudioEndpointVolume =
		{ 0x5CDF2C82, 0x841E, 0x4546,
		  { 0x97, 0x22, 0x0C, 0xF7, 0x40, 0x78, 0x22, 0x9A }
		};
	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER,
						  IID_IMMDeviceEnumerator, (LPVOID *) &deviceEnumerator);

	IMMDevice *defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole,
			&defaultDevice);

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(IID_IAudioEndpointVolume,
								 CLSCTX_INPROC_SERVER, NULL, (LPVOID *) &endpointVolume);
	endpointVolume->SetMute(bMute, NULL);

	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();
}

static bool VistaGetMute()
{
	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CLSID CLSID_MMDeviceEnumerator =
		{ 0xBCDE0395, 0xE52F, 0x467C,
		  { 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E }
		};
	IID IID_IMMDeviceEnumerator =
		{ 0xA95664D2, 0x9614, 0x4F35,
		  { 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 }
		};
	IID IID_IAudioEndpointVolume =
		{ 0x5CDF2C82, 0x841E, 0x4546,
		  { 0x97, 0x22, 0x0C, 0xF7, 0x40, 0x78, 0x22, 0x9A }
		};
	CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_INPROC_SERVER,
						  IID_IMMDeviceEnumerator, (LPVOID *) &deviceEnumerator);

	IMMDevice *defaultDevice = NULL;
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole,
			&defaultDevice);

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(IID_IAudioEndpointVolume,
								 CLSCTX_INPROC_SERVER, NULL, (LPVOID *) &endpointVolume);
	BOOL muteStatus;
	endpointVolume->GetMute(&muteStatus);

	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();
	return muteStatus;
}

static int vol2int(int vol)
{
	if (20 > vol)
		return vol * 5;
	if (50 > vol)
		return 100 + (vol - 20) * 10;
	return 400 + (vol - 50) * 12;
}

static int int2vol(int i)
{
	if (100 > i)
		return i / 5;
	if (400 > i)
		return 20 + (i - 100) / 10;
	return 50 + (i - 400) / 12;
}

static int GetVolume()
{
	int vol;

	HMIXER m_hMixer;
	MIXERCAPS m_mxcaps;
	DWORD m_dwMinimum, m_dwMaximum;
	DWORD m_dwVolumeControlID;

	m_hMixer = NULL;
	ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));
	if (MMSYSERR_NOERROR != mixerOpen(&m_hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER))
		return 0;

	if (MMSYSERR_NOERROR!=mixerGetDevCaps((UINT_PTR) m_hMixer, &m_mxcaps,
										  sizeof(MIXERCAPS)))
		return 0;

	// get dwLineID
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if (MMSYSERR_NOERROR!=mixerGetLineInfo((HMIXEROBJ) m_hMixer, &mxl,
										   MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE))
		return 0;

	// get dwControlID
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if (MMSYSERR_NOERROR!=mixerGetLineControls((HMIXEROBJ) m_hMixer, &mxlc,
			MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE))
		return 0;
	m_dwMinimum = mxc.Bounds.dwMinimum;
	m_dwMaximum = mxc.Bounds.dwMaximum;
	m_dwVolumeControlID = mxc.dwControlID;


	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = &mxcdVolume;
	if (MMSYSERR_NOERROR!=mixerGetControlDetails((HMIXEROBJ) m_hMixer,
			&mxcd, MIXER_OBJECTF_HMIXER| MIXER_GETCONTROLDETAILSF_VALUE))
		return 0;

	vol = int2vol(1000 * (mxcdVolume.dwValue - m_dwMinimum) / (m_dwMaximum
				  - m_dwMinimum));
	mixerClose(m_hMixer);
	return vol;
}

static void SetVolume(int nVol)
{
	HMIXER m_hMixer;
	MIXERCAPS m_mxcaps;
	DWORD m_dwMinimum, m_dwMaximum;
	DWORD m_dwVolumeControlID;

	m_hMixer = NULL;
	ZeroMemory(&m_mxcaps, sizeof(MIXERCAPS));
	if (MMSYSERR_NOERROR != mixerOpen(&m_hMixer, 0, 0, 0, MIXER_OBJECTF_MIXER))
		return;

	if (MMSYSERR_NOERROR!=mixerGetDevCaps((UINT_PTR) m_hMixer, &m_mxcaps,
										  sizeof(MIXERCAPS)))
		return;

	// get dwLineID
	MIXERLINE mxl;
	mxl.cbStruct = sizeof(MIXERLINE);
	mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
	if (MMSYSERR_NOERROR!=mixerGetLineInfo((HMIXEROBJ) m_hMixer, &mxl,
										   MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_COMPONENTTYPE))
		return;

	// get dwControlID
	MIXERCONTROL mxc;
	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct = sizeof(MIXERLINECONTROLS);
	mxlc.dwLineID = mxl.dwLineID;
	mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
	mxlc.cControls = 1;
	mxlc.cbmxctrl = sizeof(MIXERCONTROL);
	mxlc.pamxctrl = &mxc;
	if (MMSYSERR_NOERROR!=mixerGetLineControls((HMIXEROBJ) m_hMixer, &mxlc,
			MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE))
		return;
	m_dwMinimum = mxc.Bounds.dwMinimum;
	m_dwMaximum = mxc.Bounds.dwMaximum;
	m_dwVolumeControlID = mxc.dwControlID;

	DWORD dwVol = m_dwMinimum + vol2int(nVol) * (m_dwMaximum - m_dwMinimum)
				  / 1000;
	MIXERCONTROLDETAILS_UNSIGNED mxcdVolume =
		{ dwVol };
	MIXERCONTROLDETAILS mxcd;
	mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
	mxcd.dwControlID = m_dwVolumeControlID;
	mxcd.cChannels = 1;
	mxcd.cMultipleItems = 0;
	mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	mxcd.paDetails = &mxcdVolume;
	if (MMSYSERR_NOERROR != mixerSetControlDetails((HMIXEROBJ) m_hMixer,
			&mxcd, MIXER_OBJECTF_HMIXER| MIXER_SETCONTROLDETAILSF_VALUE))
		return;

	mixerClose(m_hMixer);
}

static void SetMute(bool bMute)
{
	int nOut;
	int nDev;
	unsigned int i, j;
	MIXERCAPS mxcaps;
	MIXERCONTROLDETAILS mxcd;
	MIXERLINE mxl;
	MIXERLINECONTROLS mxlc;
	HMIXER hmx;
	MMRESULT mmr;
	MIXERCONTROLDETAILS_UNSIGNED pmxcd_u[2];

	if (0 >= (nDev = mixerGetNumDevs()))
		return;
	for (nOut = 0; nOut < nDev; ++nOut)
	{
		if (MMSYSERR_NOERROR!=mixerGetDevCaps(nOut, &mxcaps, sizeof(MIXERCAPS)))
			return;
		mmr = mixerOpen(&hmx, nOut, 0, 0, MIXER_OBJECTF_MIXER);
		if (MMSYSERR_NOERROR != mmr)
			continue;
		for (i = 0; i < mxcaps.cDestinations; ++i)
		{
			mxl.cbStruct = sizeof(mxl);
			mxl.dwDestination = i;
			mxl.dwSource = 0;
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
			mmr = mixerGetLineInfo((HMIXEROBJ) hmx, &mxl,
								   MIXER_GETLINEINFOF_DESTINATION);
			if (MMSYSERR_NOERROR != mmr || MIXERLINE_LINEF_ACTIVE
					!= mxl.fdwLine)
				continue;

			mxlc.cbStruct = sizeof(mxlc);
			mxlc.dwLineID = mxl.dwLineID;
			mxlc.cControls = mxl.cControls;
			mxlc.cbmxctrl = sizeof(MIXERCONTROL);
			mxlc.pamxctrl = new MIXERCONTROL[mxl.cControls];
			mmr = mixerGetLineControls((HMIXEROBJ) hmx, &mxlc,
									   MIXER_GETLINECONTROLSF_ALL);
			for (j = 0; j < mxl.cControls; ++j)
			{
				if (mxlc.pamxctrl[j].dwControlType
						== MIXERCONTROL_CONTROLTYPE_MUTE)
					break;
			}
			if (j >= mxl.cControls)
			{
				delete[] mxlc.pamxctrl;
				continue;
			}

			mxcd.cbStruct = sizeof(mxcd);
			mxcd.dwControlID = mxlc.pamxctrl[j].dwControlID;
			mxcd.cChannels = 1; // Uniform Control
			mxcd.cMultipleItems = mxlc.pamxctrl[j].cMultipleItems;
			mxcd.cbDetails = sizeof(pmxcd_u);
			mxcd.paDetails = pmxcd_u;
			mixerGetControlDetails((HMIXEROBJ) hmx, &mxcd,
								   MIXER_GETCONTROLDETAILSF_VALUE);
			pmxcd_u[0].dwValue = bMute;
			mixerSetControlDetails((HMIXEROBJ) hmx, &mxcd,
								   MIXER_GETCONTROLDETAILSF_VALUE);

			delete[] mxlc.pamxctrl;
		}
		mixerClose(hmx);
	}
}

static bool GetMute()
{
	int nOut;
	int nDev;
	unsigned int i, j;
	MIXERCAPS mxcaps;
	MIXERCONTROLDETAILS mxcd;
	MIXERLINE mxl;
	MIXERLINECONTROLS mxlc;
	HMIXER hmx;
	MMRESULT mmr;
	MIXERCONTROLDETAILS_UNSIGNED pmxcd_u[2];

	if (0 >= (nDev = mixerGetNumDevs()))
		return pmxcd_u[0].dwValue;
	for (nOut = 0; nOut < nDev; ++nOut)
	{
		if (MMSYSERR_NOERROR!=mixerGetDevCaps(nOut, &mxcaps, sizeof(MIXERCAPS)))
			return pmxcd_u[0].dwValue;
		mmr = mixerOpen(&hmx, nOut, 0, 0, MIXER_OBJECTF_MIXER);
		if (MMSYSERR_NOERROR != mmr)
			continue;
		for (i = 0; i < mxcaps.cDestinations; ++i)
		{
			mxl.cbStruct = sizeof(mxl);
			mxl.dwDestination = i;
			mxl.dwSource = 0;
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
			mmr = mixerGetLineInfo((HMIXEROBJ) hmx, &mxl,
								   MIXER_GETLINEINFOF_DESTINATION);
			if (MMSYSERR_NOERROR != mmr || MIXERLINE_LINEF_ACTIVE
					!= mxl.fdwLine)
				continue;

			mxlc.cbStruct = sizeof(mxlc);
			mxlc.dwLineID = mxl.dwLineID;
			mxlc.cControls = mxl.cControls;
			mxlc.cbmxctrl = sizeof(MIXERCONTROL);
			mxlc.pamxctrl = new MIXERCONTROL[mxl.cControls];
			mmr = mixerGetLineControls((HMIXEROBJ) hmx, &mxlc,
									   MIXER_GETLINECONTROLSF_ALL);
			for (j = 0; j < mxl.cControls; ++j)
			{
				if (mxlc.pamxctrl[j].dwControlType
						== MIXERCONTROL_CONTROLTYPE_MUTE)
					break;
			}
			if (j >= mxl.cControls)
			{
				delete[] mxlc.pamxctrl;
				continue;
			}

			mxcd.cbStruct = sizeof(mxcd);
			mxcd.dwControlID = mxlc.pamxctrl[j].dwControlID;
			mxcd.cChannels = 1; // Uniform Control
			mxcd.cMultipleItems = mxlc.pamxctrl[j].cMultipleItems;
			mxcd.cbDetails = sizeof(pmxcd_u);
			mxcd.paDetails = pmxcd_u;
			mixerGetControlDetails((HMIXEROBJ) hmx, &mxcd,
								   MIXER_GETCONTROLDETAILSF_VALUE);
			delete[] mxlc.pamxctrl;
		}
		mixerClose(hmx);
	}
	return pmxcd_u[0].dwValue;
}


