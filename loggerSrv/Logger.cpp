// Logger.cpp : Implementation of CLoggerCOM

#include "stdafx.h"
#include "Logger.h"
#include "dllmain.h"

STDMETHODIMP CLoggerCOM::Create(BSTR path)
{
	if (NULL != m_pLogger)
		CLogger::DeleteLogger(m_pLogger);
	_bstr_t pathLogger(path, false);
	m_pLogger = CLogger::CreateLogger(pathLogger);
#ifdef _DEBUG
	m_strPath = LPCTSTR(pathLogger);
	CString strLog;
	strLog.Format(_T("Create Log File:%s"), m_strPath);
	_AtlModule.LogEventEx(1, strLog, EVENTLOG_WARNING_TYPE);
#endif
	return NULL != m_pLogger ? S_OK : S_FALSE;
}

STDMETHODIMP CLoggerCOM::Close()
{
	if (NULL != m_pLogger)
		CLogger::DeleteLogger(m_pLogger);
	m_pLogger = NULL;
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("Create Log File:%s"), m_strPath);
	_AtlModule.LogEventEx(1, strLog, EVENTLOG_WARNING_TYPE);
#endif
	return S_OK;
}
STDMETHODIMP CLoggerCOM::LogOut(BSTR logItem)
{
	ATLASSERT(NULL != m_pLogger);
	_bstr_t item(logItem, false);
	m_pLogger->Logout(item, item.length());
#ifdef _DEBUG
	CString strLog;
	strLog.Format(_T("LogOut:%s"), (LPCTSTR)item);
	_AtlModule.LogEventEx(1, strLog, EVENTLOG_WARNING_TYPE);
#endif
	return S_OK;
}
STDMETHODIMP CLoggerCOM::Dump()
{
	ATLASSERT(NULL != m_pLogger);
	m_pLogger->Dump();
#ifdef _DEBUG
	_AtlModule.LogEventEx(1, _T("Dump"), EVENTLOG_WARNING_TYPE);
#endif
	return S_OK;
}
// CLoggerCOM

