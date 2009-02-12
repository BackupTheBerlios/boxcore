/*
 * callbacks.cpp
 *
 *  Created on: 12 Feb 2009
 *      Author: shaun
 */

#include "BBApi.h"
#include "managers.h"

void TaskAddedCallback(HWND p_window)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_ADDTASK, reinterpret_cast<WPARAM>(p_window), 0);
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_ADDED);
	}
}

void TaskUpdatedCallback(HWND p_window)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_REDRAWTASK, reinterpret_cast<WPARAM>(p_window), 0);
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_MODIFIED);
	}
}

void TaskFlashedCallback(HWND p_window)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_FLASHED);
	}
}

void broadcastAdd(void *p_data)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_TRAYUPDATE, reinterpret_cast<WPARAM>(p_data), TRAYICON_ADDED);
	}
}

void broadcastRemove(void *p_data)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_TRAYUPDATE, reinterpret_cast<WPARAM>(p_data), TRAYICON_REMOVED);
	}
}

void broadcastMod(void *p_data)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_TRAYUPDATE, reinterpret_cast<WPARAM>(p_data), TRAYICON_MODIFIED);
	}
}
