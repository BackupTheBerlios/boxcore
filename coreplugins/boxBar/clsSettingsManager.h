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

struct NoCaseCmp
{
	bool operator()(std::string p_a, std::string p_b)
	{
		return (stricmp(p_a.c_str(), p_b.c_str()) < 0);
	}
};

typedef std::map<std::string, std::pair<INT, INT>, NoCaseCmp> intKeys_t;
typedef std::map<std::string, std::pair<UINT, UINT>, NoCaseCmp > uIntKeys_t;
typedef std::map<std::string, std::pair<bool, bool>, NoCaseCmp > boolKeys_t;
typedef std::map<std::string, std::pair<std::string, std::string>, NoCaseCmp > strKeys_t;

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
	void WriteSettings();
private:
	std::string BuildEntry(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key);

	std::string m_fileName;

	intKeys_t m_intKeys;
	uIntKeys_t m_uIntKeys;
	boolKeys_t m_boolKeys;
	strKeys_t m_strKeys;
};

#endif /* CLSSETTINGSMANAGER_H_ */
