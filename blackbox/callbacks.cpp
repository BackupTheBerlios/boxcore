/*
 * callbacks.cpp
 *
 *  Created on: 12 Feb 2009
 *      Author: shaun
 */

#include "BBApi.h"
#include "managers.h"

void TaskAddedCallback(HWND p_window, PVOID p_extra)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_ADDTASK, reinterpret_cast<WPARAM>(p_window), 0);
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_ADDED);
	}
}

void TaskRemovedCallback(HWND p_window, PVOID p_extra)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_REMOVETASK, reinterpret_cast<WPARAM>(p_window), 0);
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_REMOVED);
	}
}

void TaskUpdatedCallback(HWND p_window, PVOID p_extra)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_REDRAWTASK, reinterpret_cast<WPARAM>(p_window), 0);
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_MODIFIED);
	}
}

void TaskActivatedCallback(HWND p_window, PVOID p_extra)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_ACTIVETASK, reinterpret_cast<WPARAM>(p_window), 0);
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_ACTIVATED);
	}
}

void TaskFlashedCallback(HWND p_window, PVOID p_extra)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_TASKSUPDATE, reinterpret_cast<WPARAM>(p_window), TASKITEM_FLASHED);
	}
}

void TaskGetRectCallback(HWND p_window, PVOID p_extra)
{
	if (g_pMessageManager)
	{
		g_pMessageManager->BroadcastMessage(BB_MINMAXTASK, reinterpret_cast<WPARAM>(p_window), reinterpret_cast<LPARAM>(p_extra));
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
