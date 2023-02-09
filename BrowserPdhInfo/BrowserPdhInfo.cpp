// BrowserPdhInfo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Pdh.h>
#include <PdhMsg.h>
#include <string>
#include <set>

using std::set;

#pragma comment(lib, "Pdh.lib")

#define __DEBUG__  
#ifdef __DEBUG__  
//#define LogPrint(format,...) printf("["__FILE__" %05d "__FUNCTION__"] "format, __LINE__, ##__VA_ARGS__)
#define LogPrint(format,...) printf(format, ##__VA_ARGS__)
#else  
#define LogPrint(format,...)  
#endif 

DWORD   dwDetailLevel = PERF_DETAIL_EXPERT;

bool ShowPdhInfoA()
{
	using std::string;
	LPCSTR  szDataSource = NULL, szMachineName = NULL;
	DWORD dwBufferSize = 0;
	PZZSTR pszBuffer = NULL;
	PDH_STATUS ret = PdhEnumObjectsA(szDataSource, szMachineName, pszBuffer, &dwBufferSize, dwDetailLevel, FALSE);
	if(PDH_MORE_DATA != ret)
	{
		LogPrint("PdhEnumObjectsA fail. ret: 0x%08x\n", ret);
		return 0;
	}

	pszBuffer = (PZZSTR)malloc(dwBufferSize * sizeof(char));

	ret = PdhEnumObjectsA(szDataSource, szMachineName, pszBuffer, &dwBufferSize, dwDetailLevel, FALSE);
	if(ERROR_SUCCESS != ret)
	{
		LogPrint("ret: 0x%08x\n", ret);
		return 0;
	}

	set<string> setObejcts;

	for(char* wch = pszBuffer; 0 != *wch; wch += strlen(wch) + 1)
	{
		setObejcts.insert(wch);
	}

	free(pszBuffer);

	for (set<string>::iterator i = setObejcts.begin(); i != setObejcts.end(); ++i)
	{
		LogPrint("*********************************************%s*********************************************\n", i->c_str());
		PZZSTR  mszCounterList = NULL;
		DWORD dwCounterListLength = 0;
		PZZSTR  mszInstanceList = NULL;
		DWORD dwInstanceListLength = 0;
		ret = PdhEnumObjectItemsA(szDataSource, szMachineName, i->c_str(), mszCounterList, &dwCounterListLength, mszInstanceList, &dwInstanceListLength, dwDetailLevel, 0);
		if(PDH_MORE_DATA != ret)
		{
			LogPrint("PdhEnumObjectItemsA [%s] fail. ret: 0x%08x\n", i->c_str(), ret);
			continue;
		}

		if ((0 == dwCounterListLength) && (0 == dwInstanceListLength))
		{
			continue;
		}

		if (0 != dwCounterListLength)
		{
			mszCounterList = (PZZSTR)malloc(dwCounterListLength * sizeof(char));
			memset(mszCounterList, 0, dwCounterListLength);
		}

		if (0 != dwInstanceListLength)
		{
			mszInstanceList = (PZZSTR)malloc(dwInstanceListLength);
			memset(mszInstanceList, 0, dwInstanceListLength * sizeof(char));
		}

		ret = PdhEnumObjectItemsA(szDataSource, szMachineName, i->c_str(), mszCounterList, &dwCounterListLength, mszInstanceList, &dwInstanceListLength, dwDetailLevel, 0);
		if(ERROR_SUCCESS != ret)
		{
			LogPrint("PdhEnumObjectItemsA fail. ret: 0x%08x\n", ret);
		}
		else
		{
			LogPrint("Counter\n");
			for(char* ch = mszCounterList; 0 != *ch; ch += strlen(ch) + 1)
			{
				LogPrint("%s\\%s\n", i->c_str(), ch);
			}

			LogPrint("Instance\n");
			for(char* ch = mszInstanceList; (NULL != ch) && (0 != *ch); ch += strlen(ch) + 1)
			{
				LogPrint("%s(%s)\n", i->c_str(), ch);
			}
		}

		if(mszInstanceList)
		{
			free(mszInstanceList);
		}

		if(mszCounterList)
		{
			free(mszCounterList);
		}
	}

	return true;
}


bool ShowPdhInfoW()
{
	using std::wstring;

	LPCWSTR  szDataSource = NULL, szMachineName = NULL;
	DWORD dwBufferSize = 0;
	PZZWSTR pszBuffer = NULL;
	PDH_STATUS ret = PdhEnumObjectsW(szDataSource, szMachineName, pszBuffer, &dwBufferSize, dwDetailLevel, FALSE);
	if(PDH_MORE_DATA != ret)
	{
		LogPrint("PdhEnumObjectsW fail. ret: 0x%08x\n", ret);
		return 0;
	}

	pszBuffer = (PZZWSTR)malloc(dwBufferSize * sizeof(wchar_t));
	if(NULL == pszBuffer)
	{
		LogPrint("malloc fail\n");
		return 0;
	}

	ret = PdhEnumObjectsW(szDataSource, szMachineName, pszBuffer, &dwBufferSize, dwDetailLevel, FALSE);
	if(ERROR_SUCCESS != ret)
	{
		LogPrint("ret: 0x%08x\n", ret);
		return 0;
	}

	set<wstring> setObejcts;

	for(wchar_t* wch = pszBuffer; (NULL != wch) && (L'\0' != *wch); wch += wcslen(wch) + 1)
	{
		setObejcts.insert(wch);
	}

	free(pszBuffer);

	for (set<wstring>::iterator i = setObejcts.begin(); i != setObejcts.end(); ++i)
	{
		LogPrint("*********************************************%ws*********************************************\n", i->c_str());
		PZZWSTR  mszCounterList = NULL;
		DWORD dwCounterListLength = 0;
		PZZWSTR  mszInstanceList = NULL;
		DWORD dwInstanceListLength = 0;
		ret = PdhEnumObjectItemsW(szDataSource, szMachineName, i->c_str(), mszCounterList, &dwCounterListLength, mszInstanceList, &dwInstanceListLength, dwDetailLevel, 0);
		if(PDH_MORE_DATA != ret)
		{
			LogPrint("PdhEnumObjectItemsA [%ws] fail. ret: 0x%08x\n", i->c_str(), ret);
			continue;
		}

		if ((0 == dwCounterListLength) && (0 == dwInstanceListLength))
		{
			continue;
		}

		if (0 != dwCounterListLength)
		{
			mszCounterList = (PZZWSTR)malloc(dwCounterListLength * sizeof(wchar_t));
			memset(mszCounterList, 0, dwCounterListLength * sizeof(wchar_t));
		}

		if (0 != dwInstanceListLength)
		{
			mszInstanceList = (PZZWSTR)malloc(dwInstanceListLength * sizeof(wchar_t));
			memset(mszInstanceList, 0, dwInstanceListLength * sizeof(wchar_t));
		}

		ret = PdhEnumObjectItemsW(szDataSource, szMachineName, i->c_str(), mszCounterList, &dwCounterListLength, mszInstanceList, &dwInstanceListLength, dwDetailLevel, 0);
		if(ERROR_SUCCESS != ret)
		{
			LogPrint("PdhEnumObjectItemsA fail. ret: 0x%08x\n", ret);
		}
		else
		{
			LogPrint("Counter\n");
			for(wchar_t* ch = mszCounterList; (NULL != ch) && (L'\0' != *ch); ch += wcslen(ch) + 1)
			{
				LogPrint("%ws\\%ws\n", i->c_str(), ch);
			}

			LogPrint("Instance\n");
			for(wchar_t* ch = mszInstanceList; (NULL != ch) && (L'\0' != *ch); ch += wcslen(ch) + 1)
			{
				LogPrint("%ws(%ws)\n", i->c_str(), ch);
			}
		}

		if(mszInstanceList)
		{
			free(mszInstanceList);
		}

		if(mszCounterList)
		{
			free(mszCounterList);
		}
	}

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	ShowPdhInfoW();
	//ShowPdhInfoA();
	return 0;
}

