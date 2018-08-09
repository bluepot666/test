#ifndef __SYSTEM_CONFIG_H__
#define __SYSTEM_CONFIG_H__

#include <string>
#include <vector>
#include <map>

#include "ChannelStruct.h"
#include "OSSocket.h"
#include "OSMutex.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace std;


#define MAXCORES  128
#define MAXCARDS  32

#define NCPUSTATES 7
#define NMEMSTATS 6
#define NNETSTATS 4


struct SystemInfo
{
    int    last_pid;
    double load_avg[3];
    int    core_cnt; /* including summary "cpu" with index 0 */
    /* "user", 
     * "nice", 
     * "system", 
     * "idle" 
     **/
    int    (*cpustats)[NCPUSTATES];

    /* "total mem",
     * "free mem",
     * "buffers", 
     * "cached", 
     * "total swap", 
     * "free swap" 
     **/
    int    *memory;
};

struct NetInfo
{
    bool    bActive;
    string  strName;
    string  strIP;
    string  strNetmask;
    string  strGateway;
    string  strMAC;

    NetInfo(): bActive(true), strName("eth0"), strIP("192.168.15.124"), strNetmask("255.255.255.0"), strGateway("192.168.15.1"), strMAC("00:00:00:00:00:00")
    {
    }
};

// ϵͳ�����ļ�
struct SysConfig
{
    string  strCtrlNet;
    int     nCtrlPort;
    string  strDataNet;

    string  strCtrlNetIP;
    string  strDataNetIP;

    // map<NetcardName, NetInfo>
    map<string, NetInfo> mapNetInfo;

    string  strOutBoardCheckIP;
    int     nOutBoardCheckPort;
    string  strOutResourceIP;
    int     nOutResourcePort;

    // ת�뿨��Դ�����Ϣ SD���� HD����
    int     nResourceSum;
    int     nResourceMpeg2SDNeeds;
    int     nResourceMpeg2HDNeeds;

    int     nResourceH264SDNeeds;
    int     nResourceH264HDNeeds;

    int     nResourceH265SDNeeds;
    int     nResourceH265HDNeeds;

    int     nResourceAVSSDNeeds;
    int     nResourceAVSHDNeeds;

    // �忨�̼���Ϣ
    string  strOEM;     // ������Ϣ
    string  strCardID;  // �忨���к�

    // Уʱ��������ַ��Ϣ
    string  strTimeServerIP;
    int     nTimeServerPort;
	int		nEnable;            // ToDo

    // ϵͳ״̬
    int					nTemperature;		//�¶�
    int 				nCpuUsage[MAXCORES+1];//0 ��CPUʹ����  1-N CPU1-Nʹ���� ��Χ��(0-10000)
    int					nCpuSize;			  //��ЧCPU����

    int					nMemTotal;			//�ڴ�����(KB)
    int					nMemUsage;			//�ڴ�ʹ���� ��Χ��(0-10000)
    int					nMemUse;			//�ڴ�ʹ����(KB)

    SysConfig();
    SysConfig(const SysConfig &node);
    SysConfig & operator = (const SysConfig & node);
};

// ϵͳChannel��Դ�����ļ�
struct SysChannel
{
    bool        bStart;

    int         nIndex;

    TSInAddr    stTSInAddr;
    TSInParam   stTSInParam;
    TSOutAddr   stTSOutAddr;
    TSOutParam  stTSOutParam;

    MonitorOutAddr  stMonitorOutAddr;
    OsdInfo         stOSD;

    MonitorEnable           stMonitorEnable;
    MonitorVThrld           stThrldVideo;
    MonitorAThrld           stThrldAudio;
    MonitorColorBarThrld    stThrldColorBar;      // ����
    MonitorColorFieldThrld  stThrldRed;           // ��ɫ�ʳ�
    MonitorColorFieldThrld  stThrldGreen;         // ��ɫ�ʳ�
    MonitorColorFieldThrld  stThrldBlue;          // ��ɫ�ʳ�
    MonitorColorFieldThrld  stThrldBlack;         // ��ɫ�ʳ�
    MonitorColorFieldThrld  stThrldGray;          // ��ɫ�ʳ�
    MonitorColorFieldThrld  stThrldWhite;         // ��ɫ�ʳ�

    
    SysChannel(): bStart(false), nIndex(0) {}
};


class CConfig
{
public:
	CConfig(void);
	~CConfig(void);

    static CConfig *GetInstance();
    static void DelInstance();

public:
	// ��ȡ������Ϣ
	SysConfig &GetConfig() { return m_stSysCfg; }
	bool	LoadConfig();
	bool	SaveConfig();

    // ��ȡChannel������Ϣ
    SysChannel &GetChannel(int nIndex) { return m_arrSysChannel[nIndex]; }
    bool    LoadChannel();
    bool    LoadMonitor();
    bool    SaveChannel();

    bool    Restore(); //�ָ���������

private:
    void GetMoreColorThrld(rapidxml::xml_node<char> * pThrld, MonitorColorFieldThrld &stThrld);
    bool ReadFile(const char * strFile, char * &pData);

public:
	// ������������״̬ �����浽�����ļ���
	bool	SetNetcardStatus(std::string strNetcardName, bool bActive);

	// ��ȡϵͳ״̬ CPU �ڴ�ʹ�����
	int		UpdateSystemInfo();

protected:
	SysConfig	    m_stSysCfg;		                    // ϵͳ����
    SysChannel      m_arrSysChannel[CHANNEL_MAX_NUM];   // ϵͳͨ������        

	OS_Mutex	    m_mtxLock;		// ϵͳ���÷�����
};

#endif

