/*
 * callbacks.h
 *
 *  Created on: 12 Feb 2009
 *      Author: shaun
 */

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

void broadcastAdd(void *p_data);
void broadcastRemove(void *p_data);
void broadcastMod(void *p_data);

void TaskAddedCallback(HWND p_window, PVOID p_extra);
void TaskRemovedCallback(HWND p_window, PVOID p_extra);
void TaskUpdatedCallback(HWND p_window, PVOID p_extra);
void TaskFlashedCallback(HWND p_window, PVOID p_extra);
void TaskActivatedCallback(HWND p_window, PVOID p_extra);
void TaskGetRectCallback(HWND p_window, PVOID p_extra);

#endif /* CALLBACKS_H_ */
