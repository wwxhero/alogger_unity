// dllmain.h : Declaration of module class.

class CloggerSrvModule : public ATL::CAtlDllModuleT< CloggerSrvModule >
{
public :
	DECLARE_LIBID(LIBID_loggerSrvLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LOGGERSRV, "{360ce664-a160-41c6-86f8-d76bdcbb479d}")
};

extern class CloggerSrvModule _AtlModule;
