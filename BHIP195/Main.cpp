#include <signal.h>
#include <sys/time.h> 
#include <stdio.h>

#include "SysConfig.h"
#include "NetcardInfo.h"
#include "Thread.h"
#include "PlatformInterface.h"
#include "ChannelManager.h"
#include "IntelCommon.h"


const char * g_strHardwareVersion = "1.0.0.1";
const char * g_strSoftwareVersion = "1.0.0.1";
const char * g_strDevName = "BHIP195";
bool g_bCtrlC = false;


static void sigIntHandler(int sig)
{
	g_bCtrlC = true;
	signal(SIGINT, SIG_DFL);
}

int main(int argc, char * argv[])
{
    printf("#******************************************************************************#\n");
    printf("#                              BHIP195    INFORMATION                          #\n");
    printf("#                              COPYRIGHT  :  BOHUI                             #\n");
    printf("#                              VERSION    :  %s                          #\n", g_strSoftwareVersion);
    printf("#******************************************************************************#\n");

    // �ȴ�Ctrl+C���룬�������
    signal(SIGINT, sigIntHandler);
    signal(SIGPIPE, SIG_IGN);

    IMSDK_INIT();

	// ���ر�������
    CConfig *pConfig = CConfig::GetInstance();
	if(!pConfig->LoadConfig())
    {
        printf("Config LoadConfig Failed\n");
        return -1;
    }

    // ����ͨ������
    if(!pConfig->LoadChannel())
    {
        printf("Config LoadChannel Failed\n");
        return -1;
    }

    // ���ؼ���������
    if(!pConfig->LoadMonitor())
    {
        printf("Config LoadMonitor Failed\n");
        return -1;
    }

	// ��ʼ��������Ϣ
	CNetcardInfo::GetInstance()->Init();
    ChannelManager::GetInstance()->Init();

    PlatformInterface *pPlatformInterface = PlatformInterface::GetInstance();
    if(!pPlatformInterface || !pPlatformInterface->Start())
    {
        printf("Platform Interface Start Failed\n");
        return -1;
    }


	while (!g_bCtrlC)
	{
		char szCmd[1024];
		string strcmd = std::gets(szCmd);

        if(strcmd == "quit")
		{
			break;
		}

		OS_Thread::Sleep(1);
	}


	//ֹͣ��Ϣ����
	pPlatformInterface->Stop();

	PlatformInterface::DelInstance();
    CNetcardInfo::DelInstance();
    ChannelManager::DelInstance();
    CConfig::DelInstance();

	return 0;
}

