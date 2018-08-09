#pragma once

#include "Common.h"
#include "Markup.h"					//xml process
#include "BHXml.h"
#include "OSMutex.h"
#include "OSSocket.h"
#include "OSTime.h"
#include "ZThread.h"
#include "HttpStation.h"
#include "SysConfig.h"

#include "RingBuf.h"
#include "RingBuffer.h"

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace std;

//64λ��ʱ�ӣ���1900��1��1�տ�ʼ������
struct NtpTimePacket
{
	unsigned int m_unInteger;			//�������֣���λs
	unsigned int m_unFractional;		//С������
};

//SNTPЭ��ڵ�ڵ�
typedef struct SntpNode 
{
	unsigned char Mode	: 3;		
	unsigned char VN	: 3;
	unsigned char LI	: 2;

	unsigned char Stratum;
	unsigned char Poll;
	char Precision;
	float RootDelay;
	float RootDispersion;
	char RefIndentifier[4];
	struct NtpTimePacket RefTimestamp;				//���һ��У����ʱ��
	struct NtpTimePacket OrgTimestamp;				//
	struct NtpTimePacket ReceiveTimestamp;			//����ʱ��
	struct NtpTimePacket TransTimestamp;			//����ʱ��
}SntpNode;

class PlatformInterface;

struct ToPlatformMsg;
typedef bool (PlatformInterface::*pfnDoCmd)(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
typedef std::map<string, pfnDoCmd>	DoCmdFnMap; 

class PlatformInterface: public CXMLParser
{
public:
    PlatformInterface(void);
	~PlatformInterface(void);
	
    static PlatformInterface *GetInstance();
    static void DelInstance();

	bool Start();
	void Stop();

    void ParseXML(const string& strXML);

	// ��ӷ��͸�ƽ̨��Э����Ϣ
    void AddMsgToPlatform(const char * strXml, string &strPlatformIP, int nPlatformPort);
	int SntpRecv(unsigned char *pData,struct SntpNode *pSendNode);
	int SntpCreatSocket(string strIP);
	
private:
    // �����߳�
	static Tool::TH_RET TH_Work(LPVOID lpContext);
	void Work();

	//Уʱ������
	static Tool::TH_RET TH_Sntp(LPVOID lpContext);
	void* SntpProcess();

	//�忨����
	static Tool::TH_RET UpdateCmdCreateThread(LPVOID lpContext);

	// Update���̣߳������������󣬵���process
	void *UpdateCommunication();
	int  process();

    void InitCRC();
    void CalcCRC(unsigned char *pData, unsigned int uDataLen);

private:
    // HTTP�����Э��
    void RegParseInterface();

    bool CmdGetDeviceInfo(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdSetOutputInfo(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdGetOutputInfo(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdUpdate(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdRestart(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdFactory(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdSaveAllCfg(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdSetTimeServerAddr(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdSetTcorderInfo(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdGetTcorderInfo(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdSetOSDInfo(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);
    bool CmdGetOSDInfo(rapidxml::xml_node<char> * pData, string &strMsgID, string& strSendtoIP, int nSendtoPort);

    // ���������� ����Ƿ���Ҫ�ϱ�Э��
    void RestartReport();

private:
    // �������鲥��������Э��
    void MultiBoardCheck(const SysConfig &stSysConfig);
    void MultiChannelResource(const SysConfig &stSysConfig);
    void MultiAVErrorStatus(const SysConfig &stSysConfig);

private:
    bool	        m_bRun;

    Tool::TH_HANDLE	m_hWorkThread;			// �����߳�

	Tool::TH_HANDLE	m_hSntpThread;			// Уʱ�������߳�  SntpProcess

	Tool::TH_HANDLE	m_hUpdateThread;		//�����߳�
	
    DoCmdFnMap	m_mapDoCmdFn;
    HttpStation::CHttpStation   m_objHttpStation;
	
    OS_Mutex                          m_mutexRingBuf;
    CRingBuffer<ToPlatformMsg, 200>	  m_ringToPlatform;

private:
    OS_UdpSocket    m_sockMulitPost;        // �鲥����socket
    unsigned int	m_arrCrc32Table[256];   // CRC32У��

	int             m_SocketFd;	            //Уʱ�׽���
	char            m_byaServerIp[32];	
	int             m_SNTPPort;	            //SNTP�˿�
	unsigned int    m_uTimeout10S_Count; 
	unsigned int    m_uUpdateTime;	        //ͬ��ʱ��
	unsigned int    m_uGap;		            //10 min

};

struct ToPlatformMsg
{
    string  strMsg;

    string  strPlatformIP;
    int     nPlatformPort;

    ToPlatformMsg(): strMsg(""), strPlatformIP(""), nPlatformPort(0)
    {}

    ToPlatformMsg(string msg, string platformIP, int platformPort): strMsg(msg), strPlatformIP(platformIP), nPlatformPort(platformPort)
    {}
};

