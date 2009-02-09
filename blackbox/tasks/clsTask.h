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

class Task
{
public:
	Task(HWND p_hWnd);
	virtual ~Task();

	void ReplaceTask(HWND p_hWnd);
	void Flash(bool p_status);
	void Activate(bool p_status);
	void Update();
private:
	HWND m_hWnd;
	HICON m_smallIcon;
	HICON m_largeIcon;
	WCHAR m_caption[256];
	bool m_active;
	bool m_flashing;

	HICON m_origSmallIcon;
	HICON m_origLargeIcon;
};

}

#endif /* CLSTASK_H_ */
