#ifndef __ALG_COLORBAR_H__
#define __ALG_COLORBAR_H__

#include "Common.h"


struct ALG_COLORBAR_InArgs{
	//�û�����
	unsigned int uMaxThredValue;	// ������ز�ֵ��ֵ Ĭ��150, ģ�������ֵ80-100 < 150 
	unsigned int uMinCorlorBarNum;	// ��С������ Ĭ��3
	unsigned int uHeight;			// ֡�ĸ߶�MAX_WIDTH
	unsigned int uWidth;			// ֡�Ŀ��MAX_HEIGHT
	unsigned int uCutLeftWidth;		// �ü�����
	unsigned int uCutRightWidth;    // �����ұߵľ���
	unsigned int uCutUpHeight;
	unsigned int uCutLowHeight;     // �����±ߵľ���

	unsigned char *pbyData;		    // Yֵ��ַ
	unsigned int *puAverage;	    // ��ֵ�Ĵ洢��ַ
	unsigned int osd_count;
	OSD_RECT_INFO osd_rect[MAX_OSD_NUM];

    ALG_COLORBAR_InArgs(): uMaxThredValue(0), uMinCorlorBarNum(0), uHeight(0), uWidth(0),
        uCutLeftWidth(0), uCutRightWidth(0), uCutUpHeight(0), uCutLowHeight(0), pbyData(NULL),
        puAverage(NULL), osd_count(0)
    {
        
    }
};

struct ALG_COLORBAR_OutArgs{
	int colorBarCheck;	/*The result of colorbar check*/
	int colorAverage;	/*The result of colorbar Average*/

    ALG_COLORBAR_OutArgs(): colorAverage(0), colorBarCheck(0)
    {

    }
};


int alg_colorbar_process(ALG_COLORBAR_InArgs *pAlgcolorbar_input, ALG_COLORBAR_OutArgs *pAlgcolorbar_output);
int alg_colorbar_printf_info(char *print_buf, int iCh, ALG_COLORBAR_InArgs input, ALG_COLORBAR_OutArgs output);

#endif

