#ifndef __ALG_BLACK_FREME_H__
#define __ALG_BLACK_FREME_H__

#include "Common.h"
#include "ChannelStruct.h"


#define MAX_TESTIMAGE_NUM		2

typedef struct _TESTImage_Info
{
	unsigned char bTestImgEnable;
	int		nActiveRgnLeft;
	int		nActiveRgnTop;
	int		nActiveRgnRight;
	int		nActiveRgnBottom;	
} TESTImage_Info;

typedef struct _ALG_BLACKFRAME_InArgs
{
	//IIMGENC1_InArgs inargs;
	unsigned char nCh;		//ͨ����
	unsigned char *cur_frame;
	unsigned char *cur_CbCr_frame;
	unsigned char *last_frame;
	unsigned char bBlackEnable;
	unsigned char bStillEnable;
	float stillRatio;			//�Ǿ�֡����ռ��������			
	unsigned short usStill;		//��֡�ж������ȷ�ֵ��Ĭ��500
	unsigned short usBlack;		//�ڳ��ж������ȷ�ֵ��Ĭ��150
	unsigned short usXStart;	//��Ƶ��������X�᷽����ʼ�㣬Ĭ��30
	unsigned short usYStart;	//��Ƶ��������Y�᷽����ʼ�㣬Ĭ��30
	unsigned short usXEnd;		//��Ƶ��������X�᷽������㣬Ĭ��292
	unsigned short usYEnd;		//��Ƶ��������Y�᷽������㣬Ĭ��228
	unsigned short step;		//16*step
	TESTImage_Info stTestInfo[MAX_TESTIMAGE_NUM];

	int		nVideoWidth;		// ��Ƶͼ����(����)
	int		nVideoHeight;		// ��Ƶͼ��߶�(����)
	int		nVideoFormat;		// ��Ƶͼ���ʽ

	int		nImageRefWidth;
	int		nImageRefHeight;

	unsigned int osd_count;
	OSD_RECT_INFO osd_rect[MAX_OSD_NUM+1];
} ALG_BLACKFRAME_InArgs;

typedef struct _IMAGEOutArgs
{
	int extendedError;
	int currentAU;
	int bytesGenerated;
} IMAGEOutArgs;

typedef struct _ALG_BLACKFRAME_OutArgs
{
	IMAGEOutArgs outargs;

	unsigned char bBlackFlag;
	unsigned char bStillFlag;
	unsigned short usStillCnt;
	unsigned char bTestImgFlag[MAX_TESTIMAGE_NUM];
} ALG_BLACKFRAME_OutArgs;

typedef struct _ALG_BLACKFRAME_INFO
{
	ALG_BLACKFRAME_InArgs	in;
	ALG_BLACKFRAME_OutArgs	out;
} ALG_BLACKFRAME_INFO;


int alg_blackframe_process(ALG_BLACKFRAME_InArgs *inArgs, ALG_BLACKFRAME_OutArgs *outArgs);
int alg_blackfreme_checkColorFeild(ALG_BLACKFRAME_InArgs *inArgs, MonitorColorFieldThrld &stThrldRed, 
                MonitorColorFieldThrld &stThrldGreen, MonitorColorFieldThrld &stThrldBlue, MonitorColorFieldThrld &stThrldBlack, 
                MonitorColorFieldThrld &stThrldGray, MonitorColorFieldThrld &stThrldWhite, unsigned short *uStatue);

int alg_blackframe_printf_info(char *print_buf, int iCh, ALG_BLACKFRAME_InArgs input, ALG_BLACKFRAME_OutArgs output);

#endif

