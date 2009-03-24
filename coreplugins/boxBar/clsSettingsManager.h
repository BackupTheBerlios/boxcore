/*
 * clsSettingsManager.h
 *
 *  Created on: 24 Mar 2009
 *      Author: shaun
 */

#ifndef CLSSETTINGSMANAGER_H_
#define CLSSETTINGSMANAGER_H_
#include <map>
#include <string>
#include <utility>
#include <windows.h>

typedef std::map<std::string, std::pair<INT, INT> > intKeys_t;
typedef std::map<std::string, std::pair<UINT, UINT> > uIntKeys_t;
typedef std::map<std::string, std::pair<bool, bool> > boolKeys_t;
typedef std::map<std::string, std::pair<std::string, std::string> > strKeys_t;

class SettingsManager
{
public:
	SettingsManager();
	virtual ~SettingsManager();

	void AttachFile(const char *p_file);
	INT &AssociateInt(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, INT p_default);
	UINT &AssociateUInt(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, UINT p_default);
	bool &AssociateBool(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, bool p_default);
	std::string &AssociateString(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, LPCSTR p_default);

	void ReadSettings();
	void WriteSetting(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key);
private:
	std::string m_fileName;

	intKeys_t m_intKeys;
	uIntKeys_t m_uIntKeys;
	boolKeys_t m_boolKeys;
	strKeys_t m_strKeys;
};

#endif /* CLSSETTINGSMANAGER_H_ */
