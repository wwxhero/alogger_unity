#pragma once
#include "stdio.h"
#include <windows.h>
#include <assert.h>
#include <iostream>
#include <map>
#define ROUND_UP_SIZE(Value,Pow2) ((SIZE_T) ((((ULONG)(Value)) + (Pow2) - 1) & (~(((LONG)(Pow2)) - 1))))
#define LOG2PHY(p_log) (c_base + (p_log % c_cap))
#ifdef TEST_OVERFLOW
#define LogPtrMax UCHAR_MAX
typedef unsigned char LogPtr;
#else
#define LogPtrMax UINT_MAX
typedef unsigned int LogPtr;
#endif
class QueueMem
{
public:
	enum State {overflow = 0, fine};
	QueueMem() : c_base(NULL)
			   , c_cap(0)

	{
		m_range[0] = 0;
		m_range[1] = 0;
	}
	void Init(void *mem, unsigned int size)
	{
		c_base = (char*)mem;
		c_cap = size;
		m_range[0] = 0;
		m_range[1] = 0;
	}

	inline unsigned int size()
	{
		return m_range[head] - m_range[rear];
	}

	inline State push_front(const char* data, unsigned int len)
	{
		if (len + size() > c_cap)
			return overflow;
		else
		{
			volatile LogPtr& p_dst = m_range[head];
			if (p_dst > LogPtrMax - len)
			{
				m_range[rear] = m_range[rear] % c_cap;
				m_range[head] = m_range[head] % c_cap;
			}
			const char* p_src = data;
			const char* src_end = data + len;
			for (; p_src < src_end; p_src ++, p_dst ++)
			{
				*(LOG2PHY(p_dst)) = *p_src;
			}
			return fine;
		}
	}

	inline unsigned int pop_back(char* dst, unsigned int len)
	{
		unsigned int ret = m_range[head] - m_range[rear];
		if (len < ret)
			ret = len;
		volatile LogPtr& p_src = m_range[rear];
		char* dst_end = dst + ret;
		for (char* p_dst = dst; p_dst < dst_end; p_dst ++, p_src ++)
		{
			*p_dst = *(LOG2PHY(p_src));
		}
		return ret;
	}

	inline char* base() const
	{
		return c_base;
	}

	inline unsigned int cap() const
	{
		return c_cap;
	}

private:
	char *c_base;
	unsigned int c_cap;
	enum {rear = 0, head};
	volatile LogPtr m_range[2];
};

class CLogger
{
private:
	static VOID WINAPI CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED);
	CLogger(const wchar_t* path);
	virtual ~CLogger();
	void DumpLogInSeq();
public:
	static CLogger* CreateLogger(const wchar_t* path);
	static void DeleteLogger(CLogger* logger);
	void Logout(const wchar_t* logItem, unsigned int cap);
	void Dump();
private:
	const int c_nBuckets;
	DWORD c_secsize;
	QueueMem m_queue;
	DWORD m_threashold;
	char* m_pThresholder;
	HANDLE m_hFile;

	struct OVERLAPPED_ex : OVERLAPPED
	{
		CLogger* attach;
	};

	OVERLAPPED_ex m_aycOver;
};

