#ifndef CLSNOTIFYICONHANDLER_H
#define CLSNOTIFYICONHANDLER_H
#include "clsShellServiceHandler.h"
#include "clsNotificationIcon.h"
#include "nid.h"
#include <list>
#include <map>
#include <utility>

namespace ShellServices
{

class NotificationIcon;

typedef std::list<NotificationIcon *> IconList;

/** @internal
  * @brief Types of information which can be retrieved from a notification icon
  */
enum eNotificationIconInfo
{
	NI_HWND, ///< @brief The owning window
	NI_ID, ///< @brief The id associated with this icon
	NI_CALLBACKMESSAGE, ///< @brief The message to send to the owning window
	NI_ICON, ///< @brief The icon to display in the tray
	NI_TIP, ///< @brief The tooltip text
	NI_INFOTITLE, ///< @brief The title for the balloon tooltip
	NI_INFOTEXT, ///< @brief The text for the balloon tooltip
	NI_INFOTIMEOUT, ///< @brief The timeout value for the balloon tooltip
	NI_INFOFLAGS, ///< @brief Flags with information on the balloon tooltip
	NI_INFOICON, ///< @brief The icon to use in the balloon tooltip
	NI_VERSION, ///< @brief The version of desired tray behaviour
	NI_LEGACY, ///< @brief The legacy data item of the icon (implementation dependant)
	NI_ENUMSIZE ///< @brief Number of elements in this enumeration
};

enum eTrayCallbackType {TCALLBACK_ADD, TCALLBACK_MOD, TCALLBACK_DEL};

typedef void (*NotificationIconCallback)(void *);
typedef LegacyNotificationIcon *(*LegacyNotficationIconFactory)();

typedef std::pair<HWND, UINT> iconPair_t;

class NotifyIconHandler : public ShellServiceHandler
{
public:
	NotifyIconHandler(LegacyNotficationIconFactory p_legacyFactory, bool p_enableProxy);
	virtual ~NotifyIconHandler();

	void RegisterCallback(eTrayCallbackType p_type, NotificationIconCallback p_callback);
	virtual HRESULT ProcessMessage(DWORD p_cbData, PVOID p_lpData);

	bool GetNotificationIconInfo(UINT p_index, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);
	bool GetNotificationIconInfo(HWND p_hWnd, UINT p_uID, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);
	bool GetNotificationIconInfo(NotificationIcon *p_icon, PVOID p_return[], eNotificationIconInfo p_info[], UINT p_count);

	UINT GetTraySize();
	void CleanTray();

	NotificationIcon *LookupIcon(HWND p_hWnd, UINT p_uID);
	NotificationIcon *LookupIcon(UINT p_index);

	NotificationIcon *FindIconToShare(HICON p_icon);
	eUpdateResult DeleteIcon(HWND p_hWnd, UINT p_uID);
private:
	eUpdateResult UpdateIcon(NID_INTERNAL &p_nid);
	eUpdateResult DeleteIcon(NID_INTERNAL &p_nid);
	eUpdateResult VersionIcon(NID_INTERNAL &p_nid);
	eUpdateResult FocusIcon(NID_INTERNAL &p_nid);

	IconList m_IconList;

	LegacyNotficationIconFactory m_legacyFactory;

	NotificationIconCallback createdCallback;
	NotificationIconCallback modifiedCallback;
	NotificationIconCallback deletedCallback;

	bool m_proxyEnabled;
	std::map<UINT, iconPair_t> m_iconLookup;
	std::map<iconPair_t, UINT> m_iconLookupReverse;
	UINT currentHash;
	HWND m_proxyWnd;
	UINT m_lastPopup;
	std::pair<HWND, UINT> m_lastTarget;
	UINT m_lastMessage;

	static LRESULT CALLBACK ExtProxyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK ProxyWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

}

#endif // CLSNOTIFYICONHANDLER_H
