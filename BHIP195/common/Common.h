#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <bitset>         // std::bitset
#include <cmath>
#include "sample_defs.h"

using namespace std;


#define CHANNEL_MAX_NUM 32      // һ��忨֧�ֵ�ͨ���������
#define TRANSCODE_CARD_TYPE 1   // ת�뿨����Ϊ1


// Э�鴦������붨��
enum PROTOCOL_ERROR_CODE
{
    PROTOCOL_SUCCESS        = 0,   
    PROTOCOL_FORMAT_ERROR   = 1000,
    PROTOCOL_PARAM_ERROR    = 1001,
    PROTOCOL_UPDATE_CHECK_ERROR = 2000,
    PROTOCOL_VERSION_ERROR  = 2001,
    PROTOCOL_OTHER_ERROR    = 2002
};

enum AVCodec_Enum
{
    AV_CODEC_UNKNOWN                    = 0,
    AV_CODEC_V_MPEG1                    = 1,
    AV_CODEC_V_MPEG2                    = 2,
    AV_CODEC_V_MPEG4                    = 16,
    AV_CODEC_V_H263                     = 26,
    AV_CODEC_V_H264                     = 27,
    AV_CODEC_V_HEVC                     = 36,  // H265
    AV_CODEC_V_AVS                      = 66,
    AV_CODEC_V_PRESENTATION_GRAPHICS    = 144,
    AV_CODEC_V_INTERACTIVE_GRAPHICS     = 145,
    AV_CODEC_V_SUBTITLE                 = 146,
    AV_CODEC_V_VC1                      = 234,
    AV_CODEC_V_JPEG                     = 500,

    AV_CODEC_A_MPEG1                    = 3,
    AV_CODEC_A_MPEG2                    = 4,
    AV_CODEC_A_AC3_PRIVATE_STREAM       = 6,
    AV_CODEC_A_AAC                      = 15,
    AV_CODEC_A_MPEG4                    = 17,
    AV_CODEC_A_AVS                      = 67,
    AV_CODEC_A_DRA                      = 96,
    AV_CODEC_A_LPCM                     = 128,
    AV_CODEC_A_AC3                      = 129,
    AV_CODEC_A_DTS                      = 130,
    AV_CODEC_A_AC3_TRUE_HD              = 131,
    AV_CODEC_A_AC3_PLUS                 = 132,
    AV_CODEC_A_DTS_HD                   = 133,
    AV_CODEC_A_DTS_HD_MASTER            = 134,
    AV_CODEC_A_SECONDARY_AC3_PLUS       = 161,
    AV_CODEC_A_SECONDARY_DTS_HD         = 162,
};



#if (defined( _WIN32 ) || defined ( _WIN64 )) && !defined (__GNUC__)
typedef __int64  INT64;
typedef unsigned __int64 UINT64;
#define __INT64		INT64
#define __UINT64	UINT64
#endif

typedef unsigned char byte;
typedef void* LPVOID;
typedef unsigned int		UINT;
typedef int					INT;

typedef long				LONG;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef const char*			LPCSTR;
typedef BYTE*				PBYTE;

#define TRUE 1
#define FALSE 0


#if !((defined _WIN32) || (defined _WINDOWS_))
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) if(p){delete p; p = NULL;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if(p){delete [] p; p = NULL;}
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(p) if(p){free(p); p = NULL;}
#endif


#define TS_PACKET_LEN 188
#define MFX_BITSTREAM_LEN 10240000


#define log_printf(out, ls, fn, ln, ...) do { \
    char log[2048]; \
    snprintf(log, sizeof(log), __VA_ARGS__); \
    fprintf(out, "%s %s()@%s:%d, %s\n", ls, __FUNCTION__, fn , ln, log); \
} while (0);

#define log_info(...)       log_printf(stdout, "Info:", __FILE__, __LINE__, __VA_ARGS__)
#define log_warning(...)    log_printf(stderr, "Warning:", __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)      log_printf(stdout, "Error:", __FILE__, __LINE__, __VA_ARGS__)



#define MAX_DISPLAY_WIDTH_HD   	4096
#define MAX_DISPLAY_HEIGHT_HD	2160

#define MIN_DISPLAY_WIDTH_HD   	352
#define MIN_DISPLAY_HEIGHT_HD	288

#define MAX_OSD_NUM    		    2

struct OSD_RECT_INFO
{
    int x_start;
    int y_start;
    int width;
    int height;

    OSD_RECT_INFO(): x_start(0), y_start(0), width(0), height(0)
    {}
};

//������Ƶ������
enum PCM_CHANNELS_E
{
    PCM_CHANNEL_L=0,				//������
    PCM_CHANNEL_R,					//������
    PCM_CHANNEL_C,					//����
    PCM_CHANNEL_LS,					//����
    PCM_CHANNEL_RS,					//�һ���
    PCM_CHANNEL_D,					//�ص���
    PCM_CHANNEL_LR,					//�����
    PCM_CHANNEL_RR,					//�Һ���
    MAX_PCM_CHANNELS=8				//���֧��7.1���������ƣ�һ��8������
};

//�������ͽṹ��
enum WARN_TYPE_V_E
{
    WARN_TYPE_V_BLACK = 0,          // �ڳ�
    WARN_TYPE_V_STILL,			    // ��֡
    WARN_TYPE_V_COLORBAR,		    // ����
    WARN_TYPE_V_COLORFRAME,		    // �ʳ�
    WARN_TYPE_V_CLORLFRAME_RED,     // �쳡
    WARN_TYPE_V_CLORLFRAME_BLUE,    // ����
    WARN_TYPE_V_CLORLFRAME_GREEN,   // �̳�
    WARN_TYPE_V_CLORLFRAME_GRAY,    // �ҳ�
    WARN_TYPE_V_CLORLFRAME_WHITE,   // �׳�
    WARN_TYPE_V_DEC,			    // ��Ƶ�����쳣
    WARN_TYPE_V_LOST,			    // ��Ƶ��ʧ
    WARN_TYPE_V_SCRAMB,			    // ��Ƶ�����쳣
    WARN_TYPE_V_TEST,               // ���Կ�

    WARN_TYPE_V_MAX                 // ���������������ֵ
};

enum WARN_TYPE_A_E
{
    WARN_TYPE_A_LOST = 0,       // ��Ƶ��ʧ
    WARN_TYPE_A_NO_SOUND,       // ��Ƶ�ް���

    WARN_TYPE_A_MAX             // ���������������ֵ
};


// ���澯���
struct MonitorWarnStatus
{
    // �ڳ� | ��֡  | ����	| �쳡	| ����	| �̳�	| �ҳ�	| �׳�	|   �������	|   ��Ƶ��ʧ    |   ����ʧ��    |  
    //	0	|	1	|	2	|	3	|	4	|	5	|	6	|	7	|		8		|		9		|		10		|
    unsigned short          sWarnVideoStatus;	  // ��Ƶ���ݼ��״̬

    unsigned short          sWarnAudioStatus;     // ��Ƶ���ݼ��״̬
    unsigned short          sWarnAudioStatusL;    // ��������Ƶ���ݼ��״̬  bit0-��Ƶ��ʧ   bit1-��������   bit2-��������
    unsigned short          sWarnAudioStatusR;    // ������

    MonitorWarnStatus(): sWarnVideoStatus(0), sWarnAudioStatus(0), sWarnAudioStatusL(0), sWarnAudioStatusR(0)
    {}

    void Clear()
    {
        sWarnVideoStatus = 0;
        sWarnAudioStatus = 0;
        sWarnAudioStatusL = 0;
        sWarnAudioStatusR = 0;
    }
};

#endif

