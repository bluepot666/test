#include "alg_colorbar.h"


/* buffer definitions */
#define MININBUFS       2
#define MINOUTBUFS      0
#define MININBUFSIZE_0    (MAX_WIDTH * MAX_HEIGHT)
#define MININBUFSIZE_1    (MAX_WIDTH * 4)
#define MINOUTBUFSIZE   0

#define MAX_COLOR_BAR_NUM	8


#define MAX_WIDTH	MAX_DISPLAY_WIDTH_HD
#define MAX_HEIGHT	MAX_DISPLAY_HEIGHT_HD

#define MIN_TRAP	10 //��ֵ��С��������ֵ

typedef struct node{
	//�û�����
	unsigned int uMaxThredValue;	// ������ز�ֵ��ֵ Ĭ��150, ģ�������ֵ80-100 < 150 
	unsigned int uMinCorlorBarNum;	// ��С������ Ĭ��3
	unsigned int uHeight;			// ֡�ĸ߶�MAX_WIDTH
	unsigned int uWidth;			// ֡�Ŀ��MAX_HEIGHT
	unsigned int uCutLeftWidth;		// �ü�����
	unsigned int uCutRightWidth;
	unsigned int uCutUpHeight;
	unsigned int uCutLowHeight;

	//˽�б���
	unsigned char *pbyData;		//Yֵ��ַ
	unsigned int *puAverage;	//��ֵ�Ĵ洢��ַ
	unsigned int uAverageVal;	//������ľ�ֵ
	unsigned int osd_count;
	OSD_RECT_INFO osd_rect[2];
}colorBarInfo;


int checkColorBarInOsd(int x, int y, colorBarInfo *pInfo)
{
	unsigned int i = 0; 

	for(i = 0; i < pInfo->osd_count; i++)
	{
		if((x>pInfo->osd_rect[i].x_start) && (x < (pInfo->osd_rect[i].x_start +pInfo->osd_rect[i].width)))
		{
			if((y>pInfo->osd_rect[i].y_start) && (y < (pInfo->osd_rect[i].y_start +pInfo->osd_rect[i].height)))
			{
				return 1;
			}
		}
	}

	return 0;
}

int checkColorBar(colorBarInfo *pInfo)
{
	unsigned int nRow,nCol;
	int nSum = 0;
	int iCountCol = 0;
	int iCountRow = 0;
	int iInterValRow = 2;   //������Ŀ
	int iInterValCol = 1;   //���п��
    int nColorber_cnt = 0;

	if(pInfo->uHeight >= 576)
	{
		iInterValRow = 2;   //������Ŀ,��Ը���
	}

	if(pInfo->uWidth >= 720)
	{
		iInterValCol = 2;   //����1�У������2�У�checkAverage����4��ȡһ��ֵ
	}

	for(nCol = pInfo->uCutLeftWidth; nCol < (pInfo->uWidth - pInfo->uCutRightWidth); nCol+= iInterValCol)
	{
		int nAverage = 0, nDiff = 0;
		iCountRow = 0;
		for(nRow = pInfo->uCutUpHeight; nRow < (pInfo->uHeight - pInfo->uCutLowHeight); nRow+=iInterValRow)
		{
			//���н���ͳ��
			int nCur = 0;
			int ret = 0;
			ret = checkColorBarInOsd(nCol,nRow,pInfo);
			if(ret!= 0)
            {
				continue;
            }
			nCur = pInfo->pbyData[nRow * pInfo->uWidth + nCol];
			nAverage += nCur;
			iCountRow++;
		}

		nAverage /= iCountRow;
		pInfo->puAverage[nCol] = nAverage;

		for(nRow = pInfo->uCutUpHeight; nRow < (pInfo->uHeight - pInfo->uCutLowHeight); nRow+=iInterValRow)
		{
			int nCur = 0;
			int ret = 0;
			ret = checkColorBarInOsd(nCol,nRow,pInfo);
			if(ret != 0)
            {
				continue;
            }
			nCur = pInfo->pbyData[nRow * pInfo->uWidth + nCol];
			nDiff += abs(nCur - nAverage);
		}
		if((0 == nDiff)&&(nAverage >16))
		{
			nColorber_cnt++;
			if(nColorber_cnt > ( pInfo->uWidth - pInfo->uCutRightWidth-pInfo->uCutLeftWidth)/iInterValCol-5)
            {
				return 0;
            }
		}
		nSum += nDiff;
		iCountCol++;
		/*if(0 == iCountCol%100) //1920/2/100=10 time,��ʡCPUռ����
		{
			pInfo->uAverageVal = nSum /iCountCol*100/iCountRow;

			if (pInfo->uAverageVal > pInfo->uMaxThredValue)
			{
				//�ǲ���
				return pInfo->uAverageVal;
			}
		}*/
	}

	//@clr abnormal nAverage;
	nCol = pInfo->uCutLeftWidth;
	unsigned int nAverage;
	nAverage = pInfo->puAverage[nCol];
	for(nCol = pInfo->uCutLeftWidth; nCol < (pInfo->uWidth - pInfo->uCutRightWidth); nCol+= iInterValCol)
	{
		if(abs(nAverage - pInfo->puAverage[nCol]) >MIN_TRAP/2)
		{
			if(abs(pInfo->puAverage[nCol] - pInfo->puAverage[nCol+iInterValCol])>MIN_TRAP/2)
			{
				pInfo->puAverage[nCol] = nAverage;
			}
			nAverage = pInfo->puAverage[nCol];
		}
	}

    //printf("nSum:%d iCountCol:%d iCountRow:%d   Info H:%u W:%u LeftWidth:%u RightWidth:%u UpHeight:%u LowHeight:%u\n", nSum, iCountCol, iCountRow, 
    //    pInfo->uHeight, pInfo->uWidth, pInfo->uCutLeftWidth, pInfo->uCutRightWidth, pInfo->uCutUpHeight, pInfo->uCutLowHeight);

	pInfo->uAverageVal = nSum / iCountCol * 100 / iCountRow;

	if (pInfo->uAverageVal > pInfo->uMaxThredValue)
	{
		//�ǲ���
		//__D("uAverageVal = %d\r\n",pInfo->uAverageVal);
		return pInfo->uAverageVal;
	}
	
	return 0; //��ֵ�ж��ǲ���
}


int checkAverage(colorBarInfo *pInfo)
{
	unsigned int nCol;
	int iCount = 0;
	unsigned int nAverage;
	

	nCol = pInfo->uCutLeftWidth;
	nAverage = pInfo->puAverage[nCol];

	for(nCol = pInfo->uCutLeftWidth + 4; nCol < (pInfo->uWidth - pInfo->uCutRightWidth); nCol+= 4)
	{
		//printf("puAverage[%d] = %u\r\n",nCol,pInfo->puAverage[nCol]);

		if (abs(nAverage - pInfo->puAverage[nCol]) > MIN_TRAP)
		{
			iCount++;
			//@��׼������������ Yֵ�ǵݼ���;
            //printf("Aver:%d  InfoAver:%d nCol:%d\n", nAverage, pInfo->puAverage[nCol], nCol);
			if(nAverage < pInfo->puAverage[nCol])
			{
                int nDiff = pInfo->puAverage[nCol] - nAverage;
                if(nDiff > 3)
                {
                    return 0;//@not color bar
                }
                else
                {
                    iCount--;
                }
			}
			nAverage = pInfo->puAverage[nCol];
		}
	
	}

	return iCount; 
}


int checkColorBarWidth(colorBarInfo *pInfo)
{
	unsigned int nCol;
	unsigned int nLastCol = 0;
	unsigned int nMaxColWidth = 0;
	unsigned int nMinColWidth = 0;
	int iColWidth = 0;
	unsigned int nAverage;
	

	nCol = pInfo->uCutLeftWidth;
	nAverage = pInfo->puAverage[nCol];
	
	for(nCol = pInfo->uCutLeftWidth + 4; nCol < (pInfo->uWidth - pInfo->uCutRightWidth); nCol+= 4)
	{
		if (abs(nAverage - pInfo->puAverage[nCol]) > MIN_TRAP)
		{
			if(0 != nLastCol)
			{
				iColWidth = nCol - nLastCol;
				nMaxColWidth = (iColWidth > nMaxColWidth)?(iColWidth):(nMaxColWidth);
				nMinColWidth = (iColWidth < nMinColWidth)?(iColWidth):(nMinColWidth);
			}
			nLastCol = nCol;
			nAverage = pInfo->puAverage[nCol];
		}
	}

	float ratio = (double)nMinColWidth/(double)nMaxColWidth;

	//@As stardand color bar, this ratio is great to 0.8;
	if(ratio > 0.8)
	{
		return 0;
	}else{
		return (int)(ratio*100); 
	}
}


///* process ���õĲ����ж����*/
int isColorBar(colorBarInfo *pInfo)
{
	int iRet = 0;
	//�ж�ÿ��ɫ������ɫ�Ƿ����һ��
	iRet = checkColorBar(pInfo);
	if (iRet > 0)
	{
		iRet = -5000 - iRet;
		return iRet; //�ǲ���
	}

	//�жϸ���ɫ������Ƿ����
	iRet = checkColorBarWidth(pInfo);
	if(0 != iRet)
	{
		iRet = -700000 - iRet;
		return iRet; //�ǲ���
	}
	
	 //�ж���ɫ������
	iRet = checkAverage(pInfo);  
	if (iRet < pInfo->uMinCorlorBarNum || iRet > MAX_COLOR_BAR_NUM)
	{
		iRet = -60000 - iRet;
		return iRet; //�ǲ���
	}

	return 0;
}

/*
*  ======== IMGENC1COPY_TI_process ========
*/
/*
	����ֵ˵����
	-1���������ݲ�ȫ
	0�� ����������ȫ����ɼ��

	�������ֵ˵����colorBarCheck
	-1���������ݲ�ȫ
	-2�����ǲ���
	0�� �ǲ���
*/
int alg_colorbar_process(ALG_COLORBAR_InArgs *pAlgcolorbar_input, ALG_COLORBAR_OutArgs *pAlgcolorbar_output)
{
    int flag = 0;
    colorBarInfo cb ;

	if(NULL == pAlgcolorbar_input)
	{
		return -1;
	}
			
	if((NULL == pAlgcolorbar_input->puAverage)||(NULL == pAlgcolorbar_input->pbyData))
	{
		return -2;
	}
		
    //���ýṹ���еĲ���
    cb.uMaxThredValue	= pAlgcolorbar_input->uMaxThredValue;//���������ķ�Χֵ
    cb.uMinCorlorBarNum	= pAlgcolorbar_input->uMinCorlorBarNum;//���е�����
    cb.uWidth			= pAlgcolorbar_input->uWidth;//֡���
    cb.uHeight			= pAlgcolorbar_input->uHeight;//֡�߶�
    cb.uCutLeftWidth	= pAlgcolorbar_input->uCutLeftWidth;//�ü�����
    cb.uCutRightWidth	= pAlgcolorbar_input->uCutRightWidth;
    cb.uCutLowHeight	= pAlgcolorbar_input->uCutLowHeight;
    cb.uCutUpHeight		= pAlgcolorbar_input->uCutUpHeight;

    if(cb.uMaxThredValue <= 50) 
	{//Ŀǰ���Գ̶��е������ֵΪ150
        cb.uMaxThredValue = 150;//��ʼ��Ӧ�ĳ�ʼ��
    }

    if(cb.uMinCorlorBarNum <= 3) 
	{
        cb.uMinCorlorBarNum = 3;
    }

    if(cb.uHeight > MAX_HEIGHT) 
    {
        cb.uHeight = MAX_HEIGHT;
    }

    if(cb.uWidth > MAX_WIDTH) 
    {
        cb.uWidth = MAX_WIDTH;       //��ʼ��֡�Ĵ�С
    }

    if(cb.uCutLeftWidth + cb.uCutRightWidth + 100 >= cb.uWidth) 
    {
        cb.uCutLeftWidth  = 30;
        cb.uCutRightWidth = 30;
    } 

    if(cb.uCutLowHeight + cb.uCutUpHeight + 100 >= cb.uHeight) 
    {
        cb.uCutLowHeight = 30;
        cb.uCutUpHeight = 30;
    } 

    //���ýṹ����ָ��ָ����ڴ�λ��
    cb.pbyData		= (unsigned char *)pAlgcolorbar_input->pbyData;//y����ֵ�Ķ�ά������׵�ַ������
    cb.puAverage	= (unsigned int *)pAlgcolorbar_input->puAverage;//���������Ҷ�ֵ֮��Ĳ�ֵ
    cb.osd_count 	= pAlgcolorbar_input->osd_count;
    memcpy(cb.osd_rect, pAlgcolorbar_input->osd_rect, sizeof(OSD_RECT_INFO)*2);

    //�����жϺ������д���
    flag = isColorBar(&cb);  //��������Ҫ��������

    pAlgcolorbar_output->colorBarCheck = flag;
    pAlgcolorbar_output->colorAverage = cb.uAverageVal;

    return 0;
}


int alg_colorbar_printf_info(char *print_buf, int iCh, ALG_COLORBAR_InArgs input, ALG_COLORBAR_OutArgs output)
{
	char *p = NULL;
	p = print_buf;

	p += sprintf(p,"%-3d|%-6u|%-6u|%-8u|%-8u|%-10u|%-10u|%-8u|%-8u|%-8d|%-8d\r\n"
					,iCh
					,input.uWidth
					,input.uHeight
					,input.uCutLeftWidth
					,input.uCutRightWidth
					,input.uCutLowHeight
					,input.uCutUpHeight
					,input.uMinCorlorBarNum
					,input.uMaxThredValue
					,output.colorAverage
					,output.colorBarCheck);
					
	return (p-print_buf);
}

