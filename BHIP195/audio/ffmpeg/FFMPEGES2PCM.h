#pragma once 


#include "AudioCommon.h"
#include "AudioES2PCM.h"

#ifdef __cplusplus  
extern "C"  
{  
#endif  
#include <libavutil/samplefmt.h>
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#ifdef __cplusplus  
};  
#endif  


// ʹ��ffmpeg����Ƶ���и�ʽת��
class FFMPEGES2PCM : public AudioES2PCM
{
public:
    FFMPEGES2PCM();
    ~FFMPEGES2PCM();

    bool Init(int nAudioType);
    void DeInit();


    //��ES���� ����ΪPCM���� �����²��� ���뻷�� ��������
    //����Ҫ�� AudioReaderģ�� ����һ������ ES���ݰ�����ֹ��������ES���ݰ��������ݶ�ʧ
    void Decode(unsigned char* pES, int nLen, unsigned long long ullPts, unsigned long long ullDts);


private:
    bool		FfmpegInit();
    bool		FfmpegDeInit(bool bFreeSWR = true);


private:
    //ffmpeg ��ȡ���� �ز���˽�б���
    enum AVCodecID			m_enAudioType;		//��Ƶ���� AAC(1024) MP3(1152)
    AVFormatContext	      * m_pFormatCtx;		//ffmpegȫ��������
    AVCodecContext		  * m_pCodecCtx;		//�����������ģ�������Ƶ�� ������Ϣ ȫ������������б��棺�����ʡ�������ʽ��������
    AVCodec				  * m_pCodec;			//������ָ��
    AVCodecParserContext  * m_pAvParser;		//ES���ݽ�����
    AVPacket			  * m_pPacket;			//��Ƶ ѹ������
    AVFrame				  * m_pFrame;			//��Ƶ δѹ������

    //�ز��� ����
    struct SwrContext	  * m_AvConvertCtx;	//�ز��� ������

    //�ز��� Ŀ�����(ͨ�������ļ�����)
    enum AVSampleFormat		m_enReSamplingOutFormat;	//������ʽ
    int						m_nReSamplingOutRate;		//������
    unsigned long			m_ulReSamplingOutChannel;	//������ ������ 4 ������ 3
    int						m_nOutChannel;				//������ ������ 1 ������ 2

    bool					m_bSwrInited;				//�ز������Ƿ��Ѿ���ʼ����
    

private:
    bool					m_bFfmpegInit;		    //ffmpeg    
    int						m_nInChannel;			//��Դ ������
    int						m_nReSamplingInRate;	//��Դ ������

    unsigned char		  * m_pSWRPcmData;         //ES�����ز���֮�������,��ŵ�����Ļ��λ�������
};

