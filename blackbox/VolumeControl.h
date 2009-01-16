/*
 ============================================================================

  This file is part of the bbLean source code
  Copyright � 2001-2003 The Blackbox for Windows Development Team
  Copyright � 2004 grischka

  http://bb4win.sourceforge.net/bblean
  http://sourceforge.net/projects/bb4win

 ============================================================================

  bbLean and bb4win are free software, released under the GNU General
  Public License (GPL version 2 or later), with an extension that allows
  linking of proprietary modules under a controlled interface. This means
  that plugins etc. are allowed to be released under any license the author
  wishes. For details see:

  http://www.fsf.org/licenses/gpl.html
  http://www.fsf.org/licenses/gpl-faq.html#LinkingOverControlledInterface

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.

 ============================================================================
*/

#include <windows.h>
#include <ole2.h>
#ifndef __VOLUMECONTROL_H
#define __VOLUMECONTROL_H

// volume in percents (0-100)
void volume_set(int vol);
int volume_get();
bool volume_mute(bool toggle);

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
typedef struct {
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


#endif
