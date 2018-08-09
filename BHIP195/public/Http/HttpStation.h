#ifndef _HTTP_STATION_H_
#define _HTTP_STATION_H_

#include <vector>
#include <list>
#include <map>
using namespace std;

#include "OSSocket.h"
#include "Thread.h"
#include "OSMutex.h"
#include "RingBuffer.h"
#include "XMLParser.h"



namespace HttpStation
{
#define MAX_RECV_TH_NUM		50

#define REAPET_CHECK_NUM	0X0FFF
//#define REAPET_CHECK_NUM	0X000F

//���յ�HTTP��Ϣ�Ļص�����
typedef void (*HttpRecvCallBack) (
	const char* strContent,		//�յ���http��������
	const char* szFrom,			//�������ݵ�Դ��ַ
	int nPort,					//�������ݵ�Դ�˿�
	void *lp					//�����Ĳ���
	);

typedef void (*GetXMLParserCallBack)(void* lp, const string& strCode, CXMLParser*& pObj);

typedef std::map<int, int>	MSGIDMAP;

//������յ��̶߳���
class CHttpRecvTH : public OS_Thread
{
public:
	CHttpRecvTH();
	CHttpRecvTH(const OS_TcpSocket& sock);
	~CHttpRecvTH();

public:
	void RegFun(void *lp, GetXMLParserCallBack pFn);

	int Start();
	bool IsExit();
	void SetExit();

	bool IsSame(const std::string& strFromIP, int nFromPort);
	bool IsSame(const std::string& strFromCode);

	void SetSockTimeOut(int nSec, bool bTmOutCut = true);

private:
	void Stop();

	int Routine(TH1);

	//���������Ϣ
	void ProcessMsg(OS_TcpSocket& sock);

	bool ParseHttpMsg(std::string& strMsg, std::string& strHttp);

	void GenerateOKReply();

	bool ParseMsgID(const std::string& strMsg, std::string& strMsgID);
	bool ParseSrcCode(const std::string& strMsg, std::string& strSrcCode);
	bool ParseDstCode(const std::string& strMsg, std::string& strSrcCode);
	void CreateReplyXml(const std::string& strMsgID, std::string& strMsg);
	void WriteLog(std::string& strMsg);

private:
	//�߳�״̬���
	bool	m_bStop;
	bool	m_bExitFlag;
  
	//�����շ��������Ĳ���
	CXMLParser*	m_pParseObject;
	bool m_bParseObjEffect;

	//����Socket
	OS_TcpSocket m_RecvSock;

	std::string m_strOKReply;

	std::string m_strFromIP;
	int	m_nFromPort;

	std::string m_strFromCode;

	GetXMLParserCallBack m_pFnGetXMLParser;
	void*	m_lpContext;

	int	         m_pBufMsgID[REAPET_CHECK_NUM];

	FILE*        m_pRecvSwapFile;
	int          m_nLine;
};
typedef std::vector<CHttpRecvTH*>	PHttpRecvTHVec;

//�����͵��̶߳���
//����״̬֪ͨ�ص�����
typedef void (*HttpSendCallBack) (
								  const std::string& strToIP,	//Ŀ���ַ
								  int nToPort,
								  int nStatus,					//����״̬
								  void *lp						//�����Ĳ���
								  );

typedef std::list<std::string>		PostMsgQue;

typedef std::vector<string>		MsgVec;


//<index, buf>
typedef std::map<unsigned int, CRingBuffer<std::string, 1000>*>	IndexBufMap;

extern class BufIndex g_objBufIndex;

class BufIndex
{
public:
	BufIndex();
	~BufIndex();

public:
	unsigned int Get();

private:
	unsigned int m_unBufIndex;
	OS_Mutex m_mutexBufIndex;
};

class CHttpSendTH : public OS_Thread
{
public:
	CHttpSendTH();
	CHttpSendTH(const std::string& strToIP, int nToPort, bool bLong = true);
	~CHttpSendTH();

public:
	//ע�ᷢ��״̬�ص�
	void RegCallBackFunc(HttpSendCallBack pfnHttpSend, void *lpContext);

	int Start();
	void Stop();
	void SetExit();
	bool IsExit();

	bool IsSame(const std::string& strToIP, int nToPort);

	void AddMsg(const std::string& strMsg, unsigned int nBufIndex = 0);
	void AddMsg(const MsgVec& vecMsg);
	
	unsigned int RegBufferUser();
	void RealeseBuf(unsigned int unBufIndex);
	void SetSendTimeout(int nTimeout);
	void SetPath(std::string &strPath);

private:
	bool Connect();
	int Routine(TH1);
	int PostMsg(const std::string& strMsg);
	bool Send(const std::string& strMsg);
	bool CheckReply(const std::string& strReply);
	bool StuffMsg(std::string &strContent, const std::string& strToIP, int nToPort); 
	void WriteLog(const std::string& strMsg);

private:
	//�߳�״̬���
	bool	m_bStop;
	bool	m_bExitFlag;

	std::string m_strPath;
	//����״̬֪ͨ�ص�����
	HttpSendCallBack	m_pfnSendStatus;
	void*				m_lpContext;

	//����Socket
	OS_TcpSocket	m_SendSock;
	bool			m_bSockConn;

	std::string m_strToIP;
	int			m_nToPort;

	IndexBufMap	m_mapIndexBuf;
	CRingBuffer<unsigned int, 1000> m_bufNeedDel;		//��Ҫɾ����BUF
	unsigned int m_unBufIndex;

	int	m_nSendTimeout;
	bool m_bLongConn;

	FILE* m_pSendSwapFile;
	int   m_nLine;
};
typedef std::vector<CHttpSendTH*>	PHttpSendTHVec;


class CHttpStation : public OS_Thread
{
public:
	CHttpStation();
	~CHttpStation();

public:
	void SetLocalAddr(const std::string strLocalIP = "", int nLocalPort = 0	);

	bool Start(void);
	void Stop(void);
	void SetExit();

	bool RegParseXmlObj(const std::string& strCode, CXMLParser* pObj);


	//�������
	bool PostMsg(const MsgVec& vecMsg, 
		const std::string& strToIP, 
		int nToPort, string strPath = "");

	CHttpSendTH* CreateSender(const std::string& strToIP, int nToPort, bool bLongConn = true);
	bool PostMsg(const std::string& strMsg, void* lpSend, unsigned int nBufIndex = 0);

	void RealeseSendBuf(void* lpSend, unsigned int nBufIndex);

	void DelRecvTH(const std::string& strFromIP, int nFromPort);
	void DelRecvTH(const std::string& strFromCode);

	void DelSendTH(const std::string& strToIP, int nToPort);


private:
	int Routine(TH1);
	int Routine(TH2);

	void GetTheXMLParser(const string& strCode, CXMLParser*& pObj);
	static void GetXMLParser(void* lp, const string& strCode, CXMLParser*& pObj);
	void CheckRepeatRecvTH(const std::string& strClientIP, int nClientPort);
	void Realese();

private:
	//���ص�ַ�Ͷ˿�
	std::string		m_strIP;
	int				m_nPort;

	//�����׽���
	OS_TcpSocket m_listenSock;	

	//�Ƿ���������
	bool m_bStop; 

	//�����߳�������
	PHttpRecvTHVec m_vecPHttpRecvTH;
	OS_Mutex	m_mutexRecvTH;

	//�����߳�������
	PHttpSendTHVec m_vecPHttpSendTH;
	OS_Mutex	m_mutexSendTH;

	XMLParserMap	m_mapXMLParser;
	OS_Mutex	m_mutexXMLParserMap;
};
};
#endif

