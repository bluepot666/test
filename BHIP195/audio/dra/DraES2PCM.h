#pragma once

#include "AudioCommon.h"
#include "AudioES2PCM.h"
#include "xdra_decoder.h"


// Dra��Ƶ�̶�16bit(bitsPerSample)
class DraES2PCM: public AudioES2PCM
{
public:
    DraES2PCM();
    ~DraES2PCM();

    bool Init(int nAudioType);
    void DeInit();

    //��ES���� ����ΪPCM���� �����²��� ���뻷�� ��������
    //����Ҫ�� AudioReaderģ�� ����һ������ ES���ݰ�����ֹ��������ES���ݰ��������ݶ�ʧ
    void Decode(unsigned char* pES, int nLen, unsigned long long ullPts, unsigned long long ullDts);

private:
    bool                    m_bGetHeader;
    void                   *m_pDraDecoder;

    short                  *m_pPCMData;

    int						m_nInChannel;			//��Դ ������
    int						m_nReSamplingInRate;	//��Դ ������
};

