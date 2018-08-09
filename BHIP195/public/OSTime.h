#ifndef __OSAPI_TIME_H__
#define __OSAPI_TIME_H__

/* ΢�뼶ʱ�䴦��
    �����뼶�Ĵ�����ʹ�ñ�׼C�ĺ���
*/
#include <time.h>
#include <string.h>
#include <string>
using namespace std;

typedef unsigned long long ticks;

struct OS_TimeVal
{
	int tv_sec;
	int tv_usec;

	OS_TimeVal To(const OS_TimeVal& other)
	{
		OS_TimeVal delta;
		delta.tv_usec = other.tv_usec - tv_usec;
		delta.tv_sec = other.tv_sec - tv_sec;
		if(delta.tv_usec < 0)
		{
			delta.tv_usec += 1000000; // ��΢�벿�ֻ������ֵ
			delta.tv_sec -= 1;
		}
		return delta;
	}

	int MsTo(const OS_TimeVal& other)
	{
		return (other.tv_usec - tv_usec) / 1000 + (other.tv_sec - tv_sec) * 1000;
	}
};

class OS_Time
{
public:
	static void GetLocalTime(OS_TimeVal* tv);

	static ticks GetTicks();

	//��ȡcpuƵ��
	static double GetCpuFreq();

#ifndef _WIN32
    static void GetWallTime(OS_TimeVal* tv);
    static void SetWallTimeDiff(int diff);
#endif
};

unsigned long long OS_SysTime_Ms();
unsigned long long OS_WallTime_Ms();
string GenerateReportTime(unsigned long long ullTm);
string GenerateTimeForOsd(unsigned long long ullTm);

#endif

