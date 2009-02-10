/*
 * clsTask.h
 *
 *  Created on: 08 Feb 2009
 *      Author: shaun
 */

#ifndef CLSTASK_H_
#define CLSTASK_H_

#include <windows.h>

namespace TaskManagement
{

class LegacyTask;

class Task
{
public:
	Task(HWND p_hWnd, LegacyTask *p_legacy);
	virtual ~Task();

	void ReplaceTask(HWND p_hWnd);
	void Flash(bool p_status);
	void Activate(bool p_status);
	void Update();
	LegacyTask *getLegacy() const
	{
		return m_legacy;
	}

	HWND getHWnd() const
	{
		return m_hWnd;
	}

	HICON getSmallIcon() const
	{
		return m_smallIcon;
	}

	HICON getLargeIcon() const
	{
		return m_largeIcon;
	}

	LPCWSTR getCaption() const
	{
		return m_caption;
	}

	bool getActive() const
	{
		return m_active;
	}

	bool getFlashing() const
	{
		return m_flashing;
	}

private:
	HWND m_hWnd;
	HICON m_smallIcon;
	HICON m_largeIcon;
	WCHAR m_caption[256];
	bool m_active;
	bool m_flashing;
	LegacyTask *m_legacy;

	HICON m_origSmallIcon;
	HICON m_origLargeIcon;

	void UpdateLegacy();

	friend class TaskManager;
};

}

#endif /* CLSTASK_H_ */
