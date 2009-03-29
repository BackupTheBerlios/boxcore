/*
 * clsSettingsManager.cpp
 *
 *  Created on: 24 Mar 2009
 *      Author: shaun
 */

#include "clsSettingsManager.h"
#include <cstring>
#include "BBApi.h"
#include "../../debug/debug.h"

SettingsManager::SettingsManager()
{
}

SettingsManager::~SettingsManager()
{
}

void SettingsManager::AttachFile(const char *p_file)
{
	m_fileName = p_file;
}

INT & SettingsManager::AssociateInt(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, INT p_default)
{
	std::string entry;
	if (p_plugin)
	{
	entry = p_plugin;
	}
	if (p_component)
	{
		entry += ".";
		entry += p_component;
	}
	if (p_key)
	{
		entry += ".";
		entry += p_key;
	}
	entry += ":";
	intKeys_t::iterator it = m_intKeys.find(entry);
	if (it != m_intKeys.end())
	{
		return it->second.second;
	}
	else
	{
		m_intKeys[entry].first = p_default;
		if (m_fileName.size())
		{
			m_intKeys[entry].second = ReadInt(m_fileName.c_str(), entry.c_str(), m_intKeys[entry].first);
		}
		else
		{
			m_intKeys[entry].second = m_intKeys[entry].first;
		}
		return m_intKeys[entry].second;
	}
}

UINT & SettingsManager::AssociateUInt(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, UINT p_default)
{
	std::string entry = p_plugin;
	if (p_component)
	{
		entry += ".";
		entry += p_component;
	}
	if (p_key)
	{
		entry += ".";
		entry += p_key;
	}
	entry += ":";
	uIntKeys_t::iterator it = m_uIntKeys.find(entry);
	if (it != m_uIntKeys.end())
	{
		return it->second.second;
	}
	else
	{
		m_uIntKeys[entry].first = p_default;
		if (m_fileName.size())
		{
			m_uIntKeys[entry].second = ReadInt(m_fileName.c_str(), entry.c_str(), m_uIntKeys[entry].first);
		}
		else
		{
			m_uIntKeys[entry].second = m_uIntKeys[entry].first;
		}
		return m_uIntKeys[entry].second;
	}
}

bool & SettingsManager::AssociateBool(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, bool p_default)
{
	std::string entry = p_plugin;
	if (p_component)
	{
		entry += ".";
		entry += p_component;
	}
	if (p_key)
	{
		entry += ".";
		entry += p_key;
	}
	entry += ":";
	boolKeys_t::iterator it = m_boolKeys.find(entry);
	if (it != m_boolKeys.end())
	{
		return it->second.second;
	}
	else
	{
		m_boolKeys[entry].first = p_default;
		if (m_fileName.size())
		{
			m_boolKeys[entry].second = ReadBool(m_fileName.c_str(), entry.c_str(), m_boolKeys[entry].first);
		}
		else
		{
			m_boolKeys[entry].second = m_boolKeys[entry].first;
		}
		return m_boolKeys[entry].second;
	}
}

std::string & SettingsManager::AssociateString(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key, LPCSTR p_default)
{
	std::string entry = p_plugin;
	if (p_component)
	{
		entry += ".";
		entry += p_component;
	}
	if (p_key)
	{
		entry += ".";
		entry += p_key;
	}
	entry += ":";
	strKeys_t::iterator it = m_strKeys.find(entry);
	if (it != m_strKeys.end())
	{
		return it->second.second;
	}
	else
	{
		m_strKeys[entry].first = p_default;
		if (m_fileName.size())
		{
			m_strKeys[entry].second = ReadString(m_fileName.c_str(), entry.c_str(), m_strKeys[entry].first.c_str());
		}
		else
		{
			m_strKeys[entry].second = m_strKeys[entry].first;
		}
		return m_strKeys[entry].second;
	}
}

void SettingsManager::ReadSettings()
{
	for(intKeys_t::iterator i = m_intKeys.begin(); i != m_intKeys.end(); ++i)
	{
		i->second.second = ReadInt(m_fileName.c_str(), i->first.c_str(), i->second.first);
	}
	for(uIntKeys_t::iterator i = m_uIntKeys.begin(); i != m_uIntKeys.end(); ++i)
		{
			i->second.second = ReadInt(m_fileName.c_str(), i->first.c_str(), i->second.first);
		}
	for(boolKeys_t::iterator i = m_boolKeys.begin(); i != m_boolKeys.end(); ++i)
		{
			i->second.second = ReadBool(m_fileName.c_str(), i->first.c_str(), i->second.first);
		}
	for(strKeys_t::iterator i = m_strKeys.begin(); i != m_strKeys.end(); ++i)
		{
			i->second.second = ReadString(m_fileName.c_str(), i->first.c_str(), i->second.first.c_str());
		}
}

void SettingsManager::WriteSetting(LPCSTR p_plugin, LPCSTR p_component, LPCSTR p_key)
{
	if (m_fileName.size())
	{
		std::string entry = p_plugin;
		if (p_component)
		{
			entry += ".";
			entry += p_component;
		}
		if (p_key)
		{
			entry += ".";
			entry += p_key;
		}
		entry += ":";
		intKeys_t::iterator intIt = m_intKeys.find(entry);
		if (intIt != m_intKeys.end())
		{
			WriteInt(m_fileName.c_str(), entry.c_str(), intIt->second.second);
		}
		else
		{
			uIntKeys_t::iterator uIntIt = m_uIntKeys.find(entry);
			if (uIntIt != m_uIntKeys.end())
			{
				WriteInt(m_fileName.c_str(), entry.c_str(), uIntIt->second.second);
			}
			else
			{
				boolKeys_t::iterator boolIt = m_boolKeys.find(entry);
				if (boolIt != m_boolKeys.end())
				{
					WriteBool(m_fileName.c_str(), entry.c_str(), boolIt->second.second);
				}
				else
				{
					strKeys_t::iterator strIt = m_strKeys.find(entry);
					if (strIt != m_strKeys.end())
					{
						WriteString(m_fileName.c_str(), entry.c_str(), strIt->second.second.c_str());
					}
				}
			}
		}
	}
}














