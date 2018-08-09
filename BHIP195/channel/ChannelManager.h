#ifndef CHANNEL_MANAGER_H_
#define CHANNEL_MANAGER_H_

#include "Common.h"
#include "Channel.h"
#include "ChannelStruct.h"


#define A_CODE_SUPPORT_NUM  6   // 5+1
#define V_CODE_SUPPORT_NUM  5   // 4+1

#define		LATTICE_LIB_PATH_HZK_16			"Config/lattice/HZK16S"
#define		LATTICE_LIB_PATH_ASC_16			"Config/lattice/ASC16"
#define		LATTICE_LIB_PATH_HZK_48			"Config/lattice/HZK48S"
#define		LATTICE_LIB_PATH_ASC_48			"Config/lattice/ASC48"

// ͨ����Դ����ģ��
class ChannelManager
{
public:
    ChannelManager(void);
    ~ChannelManager(void);

    static ChannelManager *GetInstance();
    static void DelInstance();

	//�ͷ��ڴ�
	//void CaptureFreeFile(char ** pBuf);
	
	//�����ļ�
	//char* CaptureLoadFile(char * fileName, char* pBuf);
   
	bool Init();
    void DeInit();


public:
    // ����ͨ��
    bool ReStartChannel(int nIndex);

private:
    // ��ȡһ��δʹ��Channel
    int GetFreeChannel();

    // ����A/V Code�޸�TSParam�е�A/V StreamType
    void ProcessTSParamStreamType(TSInParam &stTSInParam, TSOutParam &stTSOutParam);


public:
    // ҵ�����  Channel Index��Ҫ��1 ��Ϊ�Ǵ�1��ʼ��
    // ����/��ȡͨ��ת�����  ToDoЭ�鴦��ģ����Ҫ�Բ������л������
    bool SetChannelTransParam(vector<int> &vecChannelID, TSInAddr &stTSInAddr, TSInParam &stTSInParam, TSOutParam &stTSOutParam);
    void GetChannelTransParam(int nIndex, TSInAddr &stTSInAddr, TSInParam &stTSInParam, TSOutParam &stTSOutParam)
        { m_arrChannel[nIndex]->GetTransParam(stTSInAddr, stTSInParam, stTSOutParam); }

    // ����/��ȡͨ��OSD����
    void SetChannelOSDParam(int nIndex, OsdInfo &stOSD) { m_arrChannel[nIndex]->SetOSDParam(stOSD); }
    void GetChannelOSDParam(int nIndex, OsdInfo &stOSD) { m_arrChannel[nIndex]->GetOSDParam(stOSD); }

    // ����/��ȡͨ������鲥��ַ������
    void SetChannelMultiAddr(int nIndex, TSOutAddr &stTSOutAddr, MonitorOutAddr &stMonitorOutAddr);
    void GetChannelMultiAddr(int nIndex, TSOutAddr &stTSOutAddr, MonitorOutAddr &stMonitorOutAddr) { m_arrChannel[nIndex]->GetMultiAddr(stTSOutAddr, stMonitorOutAddr); }

    bool GetMonitorStatus(int nIndex, unsigned short &sWarnVedioStatus, unsigned short &sWarnAudioStatus, string &strOutIP, int &nOutPort)
        {  return m_arrChannel[nIndex]->GetWarnStatus(sWarnVedioStatus, sWarnAudioStatus, strOutIP, nOutPort); }

private:
    // �忨����ͨ����Դ
    Channel *m_arrChannel[CHANNEL_MAX_NUM];

    int     m_arrACode2StreamType[A_CODE_SUPPORT_NUM];
    int     m_arrVCode2StreamType[V_CODE_SUPPORT_NUM];
};

#endif

