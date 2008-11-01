#ifndef GUIDSOURCES_H
#define GUIDSOURCES_H

#include <string>
#include <windows.h>

using std::string;



class clsidSource
{
public:
	virtual CLSID getNextCLSID()=0;
protected:
private:
};

class clsidRegValues : public clsidSource
{
public:
	clsidRegValues(const string &);
	~clsidRegValues();
	virtual CLSID getNextCLSID();
private:
	HKEY regKey;
	string keyName;
	bool keyValid;
	int index;
};

class clsidRegKeys : public clsidSource
{
	public:
		clsidRegKeys(const string &);
		~clsidRegKeys();
		virtual CLSID getNextCLSID();
	private:
		HKEY regKey;
		string keyName;
		bool keyValid;
		int index;
};

class clsidInjected : public clsidSource
{
	public:
		clsidInjected(const string &);
		~clsidInjected();
		virtual CLSID getNextCLSID();
	private:
		CLSID inject;
		bool used;
};

#endif
