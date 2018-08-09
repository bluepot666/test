#ifndef CHANNEL_STRUCT_H_
#define CHANNEL_STRUCT_H_

#include "Common.h"


struct FrameBitStream
{
    unsigned char   * pData;
    unsigned int	  uLen;

    unsigned long long ullPts;

    FrameBitStream() : pData(NULL), uLen(0), ullPts(0) { }
    FrameBitStream( const FrameBitStream & node );
    FrameBitStream & operator = (const FrameBitStream & node);
};


// Monitor Out
struct MonitorOutAddr
{
    int     nSwitch;
    string  strIP;
    int     nPort;

    MonitorOutAddr(): nSwitch(0), strIP("232.0.0.100"), nPort(7001) 
    {}
};

// TS Out
struct TSOutAddr
{
    int     nSwitch;
    string  strIP;
    int     nPort;

    TSOutAddr(): nSwitch(0), strIP("233.0.0.100"), nPort(7001)
    {}
};

struct TSOutParam
{
    int     nVCode;
    int     nACode;
    int     nWidth;
    int     nHeight;
    int     nFrameRate;
    int     nVRate;
    int     nARate;

    int     nVStreamType;   // VCodeת��
    int     nAStreamType;   // ACodeת��

    TSOutParam(): nVCode(2), nACode(1), 
        nWidth(704), nHeight(576), nFrameRate(25), nVRate(1500), nARate(32),
        nVStreamType(0), nAStreamType(0)
    {}
};

// In
struct TSInParam
{
    int     nVCode;
    int     nACode;
    int     nServiceID;     // ��Ŀ��
    int     nVPID;
    vector<int> vecAPID;
    int     nServiceType;   // ��Ŀ���(0-�㲥 1-����)

    int     nVStreamType;   // VCodeת��
    int     nAStreamType;   // ACodeת��

    TSInParam();
    TSInParam(const TSInParam & node);
    TSInParam & operator = (const TSInParam & node);
};

struct TSInAddr
{
    string  strIP;
    int     nPort;

    TSInAddr(): strIP("231.0.0.100"), nPort(7001)
    {}


};

// OSD
struct OsdInfo
{
    string  strText;
    int     nFontSize;      // �����С
    int     nPosition;      // λ����Ϣ 0-���Ͻ� 1-���Ͻ� 2-���½� 3-���½�
    int     nAntiColor;     // �Ƿ�ɫ 0-�����÷�ɫ 1-������ɫ
    int     nAlign;         // ���뷽ʽ 0-����� 1-���� 2-�Ҷ���
    int     nType;          // ��ʾ���� 1-��һ��Ϊtext,�ڶ���Ϊʱ����Ϣ  2-��һ��Ϊtext,��ʱ����Ϣ  3-text��ʱ����Ϣ��һ����ʾ

    OsdInfo(): strText("osd"), nFontSize(16), nPosition(0), nAntiColor(0), nAlign(0), nType(1) 
    {}
};


// Monitor Param Start
// Monitor Enable
struct MonitorEnable
{
    int     nVideoMonitor;        // ��Ƶ���ʹ�ܿ��� ����0��ʾ�ڳ����,����1��ʾ��֡���
    int     nAudioMonitor;        // ��Ƶ���ʹ�ܿ��� ����0��ʾ��Ƶ��ʧ,����1��ʾ��������,����2��ʾ��������
    int     nAudioChannelMonitor; // ��Ƶͨ�����ʹ�ܿ��� bit0-������,bit1-������

    MonitorEnable(): nVideoMonitor(7), nAudioMonitor(7), nAudioChannelMonitor(3) 
    {}
};

// Video Threshold
struct MonitorVThrld
{
    int     nLostTime;    
    int     nSampleTime;
    int     nStillRatio;
    int     nStill;
    int     nBlack;
    int     nXStart;
    int     nYStart;
    int     nXEnd;
    int     nYEnd;

    MonitorVThrld(): nLostTime(500), nSampleTime(100), nStillRatio(3), nStill(500), nBlack(150), nXStart(30), nYStart(30), nXEnd(322), nYEnd(258) 
    {}
};

// Audio Threshold
struct MonitorAThrld
{
    int     nLostTime;
    int     nSampleTime;
    int     nLost;
    int     nHigher;
    int     nLower;
    int     nLostLower;

    MonitorAThrld(): nLostTime(500), nSampleTime(500), nLost(3), nHigher(-3), nLower(-45), nLostLower(-48) 
    {}
};

// �����������
struct MonitorColorBarThrld
{
    int     nMaxDiffValue;
    int     nMinNum;
    int     nXStart;
    int     nYStart;
    int     nXEnd;
    int     nYEnd;

    MonitorColorBarThrld(): nMaxDiffValue(150), nMinNum(4), nXStart(30), nYStart(30), nXEnd(322), nYEnd(258) 
    {}
};

// �ʳ��������
struct MonitorColorFieldThrld
{
    int     nRedYUP;
    int     nRedYDown;
    int     nGreenUUp;
    int     nGreenUDown;
    int     nBlueVUp;
    int     nBlueVDown;

    MonitorColorFieldThrld(): nRedYUP(255), nRedYDown(80), nGreenUUp(16), nGreenUDown(0), nBlueVUp(16), nBlueVDown(0) 
    {}
};

#endif

