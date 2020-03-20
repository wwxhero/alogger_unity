#include "pch.h"
#include "Logger.h"
#define TEST_CONSISTENCY_INPUT
#define OPTIMISED_DISK_IO
//#define TEST_CONSISTENCY_OUTPUT
std::map<OVERLAPPED*, CLogger*> CLogger::m_overlap2logger;

CLogger::CLogger(const wchar_t* szPath) 
#ifdef TEST_OVERFLOW
		: c_nBuckets(1)
#else
		: c_nBuckets(4)
#endif
{
#ifdef TEST_OVERFLOW
	c_secsize = 128;
#else
	GetDiskFreeSpaceA(NULL, NULL, &c_secsize, NULL, NULL);
#endif
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	DWORD buketsize;
#ifdef TEST_OVERFLOW
	DWORD pagebytes = 128;
#else
	DWORD pagebytes = sysInfo.dwPageSize;
#endif
	if (pagebytes > c_secsize)
		buketsize = ROUND_UP_SIZE(c_secsize, pagebytes);
	else
		buketsize = ROUND_UP_SIZE(pagebytes, c_secsize);
	DWORD cap = c_nBuckets * buketsize;
	char *p = (char*)VirtualAlloc(NULL, cap, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	m_queue.Init(p, cap);
#ifdef OPTIMISED_DISK_IO
	m_threashold = c_secsize;
#else
	m_threashold = 128;
#endif
	m_pThresholder = (char *)malloc(c_secsize);
	m_hFile = ::CreateFileW(szPath
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_READ
		, NULL
		, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED
		, NULL);
	m_aycOver.hEvent = CreateEvent(
		NULL,    // default security attribute
		TRUE,    // manual-reset event
		TRUE,    // initial state = signaled
		NULL);   // unnamed event object
}

CLogger::~CLogger()
{
	Dump();
	VirtualFree(m_queue.base(), m_queue.cap(), MEM_RELEASE);
	CloseHandle(m_aycOver.hEvent);
	CloseHandle(m_hFile);
	free(m_pThresholder);
}

void CLogger::Dump()
{
	while (m_queue.size() > 0)
		DumpLogInSeq();
	::WaitForSingleObjectEx(m_aycOver.hEvent, INFINITE, TRUE); //wait till the asyc io is done
}

void CLogger::Logout(const wchar_t* logItem, unsigned int cap)	
{
	unsigned int delta = (cap << 1);
	bool overflow = (QueueMem::overflow == m_queue.push_front((const char*)logItem, delta));

	if ((m_queue.size() > m_threashold
		&& WAIT_OBJECT_0 == ::WaitForSingleObjectEx(m_aycOver.hEvent, 0, TRUE)) //wait till the asyc io is done
		|| overflow)
		DumpLogInSeq();

	while (overflow)
	{
		::SleepEx(5, TRUE); //yield cpu in order for the asyc routinue proceees data dump
#ifdef TEST_CONSISTENCY_INPUT
		printf("\n>>>>>>>>>>>overflow>>>>>>>>>>>>");
#endif
		overflow = (QueueMem::overflow == m_queue.push_front((const char*)logItem, delta));
	}
}

void CLogger::DumpLogInSeq()
{
	char *p = m_pThresholder;
	unsigned int size = 0;
	if ((size = m_queue.pop_back(p, c_secsize)) > 0)
	{
		m_aycOver.Offset = m_aycOver.OffsetHigh = 0xFFFFFFFF;
		ResetEvent(m_aycOver.hEvent);
		WriteFileEx(m_hFile, p, size, &m_aycOver, CompletedWriteRoutine);
	}
#ifdef TEST_CONSISTENCY_OUTPUT
	wchar_t *wp = (wchar_t *)p;
	unsigned int wsize = (size >> 1);
	wp[wsize] = L'\0';
	wprintf(L"DumpLogInSeq%d:%s\n", wsize, wp);
#endif
}

VOID WINAPI CLogger::CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten, LPOVERLAPPED lpOverLap)
{
	CLogger* this_logger = m_overlap2logger[lpOverLap];
	assert(NULL != this_logger);
	if (this_logger->m_queue.size() > this_logger->m_threashold)
		this_logger->DumpLogInSeq();
	else
		SetEvent(lpOverLap->hEvent);
}

CLogger* CLogger::CreateLogger(const wchar_t* path)
{
	//to create a file for the path and be prepared to write log (asynchronously) to that file
	CLogger* ret = new CLogger(path);
	m_overlap2logger[&ret->m_aycOver] = ret;
	return ret;
}

void CLogger::DeleteLogger(CLogger* logger)
{
	OVERLAPPED* overlap = &logger->m_aycOver;
	delete logger;
	m_overlap2logger.erase(overlap);
}

