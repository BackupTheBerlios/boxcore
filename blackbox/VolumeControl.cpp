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

#include "VolumeControl.h"
#include "../debug/debug.h"
#include "managers.h"

//================================================
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

//================================================
static int getsetVolume(int x)
{
	int vol = 0;
	if (0 <= x)
		vol = x;

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

	if (0 <= x)
	{ // set
		DWORD dwVol = m_dwMinimum + vol2int(vol) * (m_dwMaximum - m_dwMinimum)
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
			return 0;
	}
	else
	{
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
	}
	mixerClose(m_hMixer);
	return vol;
}

static int getsetVolumeVista(int x)
{
	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CLSID CLSID_MMDeviceEnumerator =
	{ 0xBCDE0395, 0xE52F, 0x467C,
	{ 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E } };
	IID IID_IMMDeviceEnumerator =
	{ 0xA95664D2, 0x9614, 0x4F35,
	{ 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 } };
	IID IID_IAudioEndpointVolume =
	{ 0x5CDF2C82, 0x841E, 0x4546,
	{ 0x97, 0x22, 0x0C, 0xF7, 0x40, 0x78, 0x22, 0x9A } };
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
	if (x == -1)
	{
		hr = endpointVolume->GetMasterVolumeLevelScalar(&volume);
	}
	else
	{
		volume = ((double) x) / 100;
		endpointVolume->SetMasterVolumeLevelScalar(volume, NULL);
	}
	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();
	return volume * 100;
}

// volume in percents (0-100)
void volume_set(int vol)
{
	if (0 > vol)
		vol = 0;
	if (100 < vol)
		vol = 100;
	if (SystemInfo.isOsVista())
		getsetVolumeVista(vol);
	else
		getsetVolume(vol);
}

int volume_get()
{
	if (SystemInfo.isOsVista())
		return getsetVolumeVista(-1);
	else
		return getsetVolume(-1);
}

//================================================
// volume mute
static bool volume_mute_vista(bool toggle)
{
	HRESULT hr;
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CLSID CLSID_MMDeviceEnumerator =
	{ 0xBCDE0395, 0xE52F, 0x467C,
	{ 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E } };
	IID IID_IMMDeviceEnumerator =
	{ 0xA95664D2, 0x9614, 0x4F35,
	{ 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 } };
	IID IID_IAudioEndpointVolume =
	{ 0x5CDF2C82, 0x841E, 0x4546,
	{ 0x97, 0x22, 0x0C, 0xF7, 0x40, 0x78, 0x22, 0x9A } };
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
	if (toggle)
		muteStatus = !muteStatus;
	endpointVolume->SetMute(muteStatus, NULL);

	endpointVolume->Release();
	defaultDevice->Release();
	deviceEnumerator->Release();
	CoUninitialize();
	return muteStatus;
}

static bool volume_mute_other(bool toggle)
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
			toggle ? pmxcd_u[0].dwValue = !pmxcd_u[0].dwValue
					: pmxcd_u[0].dwValue = pmxcd_u[0].dwValue; // toggle mute
			mixerSetControlDetails((HMIXEROBJ) hmx, &mxcd,
					MIXER_GETCONTROLDETAILSF_VALUE);

			delete[] mxlc.pamxctrl;
		}
		mixerClose(hmx);
	}
	return pmxcd_u[0].dwValue;
}

bool volume_mute(bool toggle)
{
	if (SystemInfo.isOsVista())
		return volume_mute_vista(toggle);
	else
		return volume_mute_other(toggle);
}
