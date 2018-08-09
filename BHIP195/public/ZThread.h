/*******************************************************************************
* ��Ȩ���� (C) 2010
* 
* �ļ����ƣ� 	ZThread.h
* �ļ���ʶ�� 	
* ����ժҪ�� 	�̸߳�������������Windows��linux��ͨ�������
* ����˵���� 	
* ��ǰ�汾�� 	V1.0
* ��    �ߣ� 	�ܷ�
* ������ڣ� 	2010-06-08
*******************************************************************************/
#ifndef Z_THREAD_H_7589432758974389257894385849328590438508439589058904328590430
#define Z_THREAD_H_7589432758974389257894385849328590438508439589058904328590430

#if (defined _WIN32) || (defined _WINDOWS_)
#include <Windows.h>
#else
#include <pthread.h>
#endif


namespace Tool
{


#if (defined _WIN32) || (defined _WINDOWS_)
	typedef HANDLE	TH_HANDLE;
	typedef UINT TH_RET;
#else
	typedef pthread_t	TH_HANDLE;
	typedef void  *TH_RET;
	typedef unsigned long       DWORD;
#endif

	typedef TH_RET (*ThreadFuncType) (void *lpContext);

class CZThread
{
private:
	CZThread(void){}
	~CZThread(void){}


public:

	static const TH_HANDLE INVALID_HANDLE;

	//�����߳�
	static TH_HANDLE BeginThread(ThreadFuncType pfnFunc, void *lpContext, bool bHighPriority = false);

	//�ȴ��߳̽���(dwTime��λΪms����Ϊ0��ʾ�����Ƶȴ�)
	static bool WaitThreadEnd(TH_HANDLE &handle, DWORD dwTime = 0);
};

}

#endif

