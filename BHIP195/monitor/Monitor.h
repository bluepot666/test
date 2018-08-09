#pragma once

#include "ChannelStruct.h"

#include "alg_colorbar.h"
#include "alg_blackframe.h"
#include "alg_snd.h"


#define MAX_WIDTH_PIXEL_NUM 4096


class Monitor
{
public:
    Monitor();
    ~Monitor();

    int VideoDataMonitor(unsigned char *pYaddr, unsigned char *pUVaddr, int width, int height, unsigned short &sMonitorWarnStatus);
    int AudioDataMonitor(unsigned char *pPCMData, int nChannels, int nPcmSamplesPerFrame, int nBitPerSample, unsigned short &sMonitorWarnStatus);

private:
    int BlackFrameSetParam(unsigned char *pYaddr, unsigned char *pUVaddr, int width, int height, int ChromaFomart);
    int ColorbarSetParam(unsigned char *pYaddr, int width, int height);


public:
    // ��������� Э������δ���� ʹ�������ļ��е�Ĭ�ϲ���
    MonitorEnable               m_stMonitorEnable;
    MonitorVThrld               m_stThrldVideo;
    MonitorAThrld               m_stThrldAudio;
    MonitorColorBarThrld        m_stThrldColorBar;      // ����
    MonitorColorFieldThrld      m_stThrldRed;           // ��ɫ�ʳ�
    MonitorColorFieldThrld      m_stThrldGreen;         // ��ɫ�ʳ�
    MonitorColorFieldThrld      m_stThrldBlue;          // ��ɫ�ʳ�
    MonitorColorFieldThrld      m_stThrldBlack;         // ��ɫ�ʳ�
    MonitorColorFieldThrld      m_stThrldGray;          // ��ɫ�ʳ�
    MonitorColorFieldThrld      m_stThrldWhite;         // ��ɫ�ʳ�


    // ��Ƶ
private:
    // ����������
    unsigned int			m_arrAverage[MAX_WIDTH_PIXEL_NUM];    // �洢���ؾ�ֵ
    ALG_COLORBAR_InArgs     m_stColorBarInArgs;
    ALG_COLORBAR_OutArgs    m_stColorBarOutArgs;


    // �ʳ�������
    ALG_BLACKFRAME_InArgs	m_stBlackframeInArgs;
    ALG_BLACKFRAME_OutArgs	m_stBlackframeOutArgs;
    unsigned char		    m_arrLastFrameYaddr[MAX_DISPLAY_WIDTH_HD * MAX_DISPLAY_HEIGHT_HD];


    unsigned long long      m_ullLastMonitorTime;       // ��һ�μ��ʱ��
    // ���澯���
    unsigned long long      m_ullStillHappendTime;      // ��֡����ʱ��


private:
    // ��Ƶ
    ALG_AUDIO_Infos         m_stAudioInfos;
    ALG_AUDIO_Infos  		m_stAudioPrintInfo;

    int                     m_arrCalcCnt[2][3];		// ���������ڵĲ�������
    int                     m_arrWarnCnt[2][3];     // ���������ڵĹ��ϴ���
};

