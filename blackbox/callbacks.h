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

void TaskAddedCallback(HWND p_window);
void TaskRemovedCallback(HWND p_window);
void TaskUpdatedCallback(HWND p_window);
void TaskFlashedCallback(HWND p_window);

#endif /* CALLBACKS_H_ */
