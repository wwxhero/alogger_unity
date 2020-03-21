// dllmain.h : Declaration of module class.

class CloggerSrvModule : public ATL::CAtlDllModuleT< CloggerSrvModule >
{
public :
	DECLARE_LIBID(LIBID_loggerSrvLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LOGGERSRV, "{360ce664-a160-41c6-86f8-d76bdcbb479d}")
	CloggerSrvModule()
	{
		m_hEventSource = RegisterEventSource(NULL, c_logSrc);
	}
	virtual ~CloggerSrvModule()
	{
		DeregisterEventSource(m_hEventSource);
	}
	void LogEventEx(
		_In_ int id,
		_In_opt_z_ LPCTSTR pszMessage=NULL,
		_In_ WORD type = EVENTLOG_INFORMATION_TYPE) throw()
	{
			/* Get a handle to use with ReportEvent(). */
			if (m_hEventSource != NULL)
			{
				/* Write to event log. */
				ReportEvent(m_hEventSource,
							type,
							(WORD)0,
							id,
							NULL,
							(WORD)(pszMessage != NULL ? 1 : 0),
							0,
							pszMessage != NULL ? &pszMessage : NULL,
							NULL);
			}
	}
private:
	HANDLE m_hEventSource;
	static const TCHAR c_logSrc[];
};

extern class CloggerSrvModule _AtlModule;
