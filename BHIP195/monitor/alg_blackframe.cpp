#include "alg_blackframe.h"

#define DEF_IMAGE_MARGIN			30


// ��־λ���� �������
#define SET_BIT(val, bitIndex)		((val) | (1 << (bitIndex)))		
#define ClEAR_BIT(val, bitIndex)	((val) & (~(1 << (bitIndex))))

/* buffer definitions */
#define MININBUFS       1
#define MINOUTBUFS      1
#define MININBUFSIZE    1
#define MINOUTBUFSIZE   1


int checkBlackFrameInOsd(int x, int y, ALG_BLACKFRAME_InArgs *pInfo)
{
	int i = 0; 
	for(i = 0; i < pInfo->osd_count; i++)
	{		
		if((x >= pInfo->osd_rect[i].x_start) && (x <= (pInfo->osd_rect[i].x_start +pInfo->osd_rect[i].width)))
		{
			if((y >= pInfo->osd_rect[i].y_start) && (y <= (pInfo->osd_rect[i].y_start +pInfo->osd_rect[i].height)))
			{
				return 1;
			}
		}
	}

	return 0;
}

/*
int IsRectIntersecting(int left_1, int top_1, int right_1, int bottom_1,
					   int left_2, int top_2, int right_2, int bottom_2)
{
	// �����κϷ���
	if(left_1 >= right_1 || top_1 >= bottom_1)
		return 0;

	if(left_2 >= right_2 || top_2 >= bottom_2)
		return 0;

	// �жϾ����Ƿ��ཻ
	return ((left_1 <= left_2 && right_1 <= left_2)
			|| (left_1 >= right_2 && right_1 >= right_2)
			|| (top_1 <= top_2 && bottom_1 <= top_2)
			|| (top_1 >= bottom_2 && bottom_1 >= bottom_2))? 0 : 1;
}
*/

int RgbYuvCmp(MonitorColorFieldThrld &stThrld, unsigned char byRY, unsigned char byGU, unsigned char byBV)
{
    //printf("R:%d RDown:%d RUP:%d   G:%d GDown:%d GUP:%d   B:%d BDown:%d BUP:%d\n", 
    //    byRY, stThrld.nRedYDown, stThrld.nRedYUP, byGU, stThrld.nGreenUDown, stThrld.nGreenUUp, byBV, stThrld.nBlueVDown, stThrld.nBlueVUp);

	if ((byRY >= stThrld.nRedYDown)&&(byRY <= stThrld.nRedYUP)
		&&(byGU >= stThrld.nGreenUDown)&&(byGU <= stThrld.nGreenUUp)
		&&(byBV >= stThrld.nBlueVDown)&&(byBV<= stThrld.nBlueVUp))
	{
		//��������ֵ
		return 0;
	}
	return -1;
}

int alg_blackfreme_checkColorFeild(ALG_BLACKFRAME_InArgs *inArgs, MonitorColorFieldThrld &stThrldRed, 
    MonitorColorFieldThrld &stThrldGreen, MonitorColorFieldThrld &stThrldBlue, MonitorColorFieldThrld &stThrldBlack, 
    MonitorColorFieldThrld &stThrldGray, MonitorColorFieldThrld &stThrldWhite, unsigned short *uStatue)
{
	int w,h;
	int nY = 0, nU = 0, nV = 0; 

	int yoffset = 0;
	int findFlag = 0;


	//ȡһ��OSD����֮���16*16���
	while((inArgs->usYStart + 16 + yoffset < inArgs->nVideoHeight) && !findFlag)
	{
		findFlag = 1;
		
		for(h= inArgs->usYStart + yoffset;h< inArgs->usYStart + 16 + yoffset && findFlag;h++)
		{
			for ( w = inArgs->usXStart; w < inArgs->usXStart + 16 && findFlag; w++)
			{
				if(checkBlackFrameInOsd(h,w,inArgs))
				{
					yoffset += 16;
					findFlag = 0;
					continue;
				}
			}
		}
		
	}

	if(!findFlag)
		return 0;
	//����16*16����Y��ֵ
	for (h = inArgs->usYStart + yoffset; h < inArgs->usYStart + yoffset + 16; h++)
	{
		for ( w = inArgs->usXStart; w < inArgs->usXStart + 16; w++)
		{
			nY += *(inArgs->cur_frame+ w + h * inArgs->nVideoWidth);
		}
	}
	nY /= 256;

	//����8*8����UV��ֵ
	for (h = (inArgs->usYStart + yoffset)/2; h < (inArgs->usYStart + yoffset)/2 + 8; h++)
	{
		for ( w = inArgs->usXStart; w < inArgs->usXStart + 16; w+=2)
		{
			nV += *(inArgs->cur_CbCr_frame + w + h * inArgs->nVideoWidth);
			nU += *(inArgs->cur_CbCr_frame + w + 1 + h * inArgs->nVideoWidth);
		}
	}
	nU /= 64;
	nV /= 64;

//	printf("Corlor Y %d U %d V %d, yoffset = %d\n", nY, nU, nV, yoffset);

    // ��ղʳ���־ �����ж��Ǿ������ֲʳ�
	*uStatue = ClEAR_BIT(*uStatue, WARN_TYPE_V_COLORFRAME);

	*uStatue = ClEAR_BIT(*uStatue, WARN_TYPE_V_BLACK);
	if (0 == RgbYuvCmp(stThrldBlack, nY, nU, nV))
	{
		*uStatue = SET_BIT(*uStatue, WARN_TYPE_V_BLACK);
		return 0;
	}

	*uStatue = ClEAR_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_WHITE);
	if (0 == RgbYuvCmp(stThrldWhite, nY, nU, nV))
	{
		*uStatue = SET_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_WHITE);
		return 0;
	}

	*uStatue = ClEAR_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_GRAY);
	if (0 == RgbYuvCmp(stThrldGray, nY, nU, nV))
	{
		*uStatue = SET_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_GRAY);
		return 0;
	}

	int nR = nY + 1.4075 * (nV - 128);
	int nG = nY - 0.3455 * (nU - 128) - 0.7169 * (nV - 128);
	int nB = nY + 1.779 * (nU - 128);

	//�ɵ�����
	nR = (nR < 0)?0:nR;
	nG = (nG < 0)?0:nG;
	nB = (nB < 0)?0:nB;

    //����������������
    nR = (nR > 255)?255:nR;
    nG = (nG > 255)?255:nG;
    nB = (nB > 255)?255:nB;

    //printf("11111 nY:%d nU:%d nV:%d nR:%d nG:%d nB:%d\n", nY, nU, nV, nR, nG, nB);

	*uStatue = ClEAR_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_RED);
	if (0 == RgbYuvCmp(stThrldRed, nR, nG, nB))
	{
		*uStatue = SET_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_RED);
		return 0;
	}

	*uStatue = ClEAR_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_GREEN);
	if (0 == RgbYuvCmp(stThrldGreen, nR, nG, nB))
	{
		*uStatue = SET_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_GREEN);
		return 0;
	}

	*uStatue = ClEAR_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_BLUE);
	if (0 == RgbYuvCmp(stThrldBlue, nR, nG, nB))
	{
		*uStatue = SET_BIT(*uStatue, WARN_TYPE_V_CLORLFRAME_BLUE);
		return 0;
	}

    // ���òʳ���־ ��Ϊ��������û�о����жϳ���������ɫ�Ĳʳ� ʹ��Ĭ�ϲʳ���־λ
	*uStatue = SET_BIT(*uStatue, WARN_TYPE_V_COLORFRAME);
	return 0;
}

void alg_blackfreme_videocheck
(
	ALG_BLACKFRAME_InArgs *pIn,
	ALG_BLACKFRAME_OutArgs *pOut,  
	unsigned char *pVideoImage,
	unsigned char *pStillImageRef,
	unsigned char *pTestImageRef[]
)
{
	int nVideoRow = 0, nVideoCol = 0;			// ��ǰ��Ƶ֡�С��б���
	int nRefRow = 0, nRefCol = 0;				// �ο�ͼ��֡�С��б���
	int nVideoBlockX = 0, nVideoBlockY = 0;		// ��Ƶͼ��֡���ؿ��С��б���
	int nRefBlockX = 0, nRefBlockY = 0;			// �ο�ͼ��֡���ؿ��С��б���

	int nVideoImagePos = 0, nImageRefPos = 0;	// ��ǰ��Ƶ֡���ο�ͼ��֡����λ��
	int nVideoImagePos_Cur = 0, nImageRefPos_Cur = 0;
												// ��ǰͼ��֡���ο�ͼ��֡��ǰ����λ��

	int nValidStillRef = 0;						// ��֡�ο�ͼ���Ƿ���Ч(��ʼ��)
	int nLuminDiff = 0, nLuminDiffSum = 0, nAbsLuminDiffSum = 0;
												// ֡���������Ȳ��ۼӺ������Ȳ����ֵ���ۼӺ�
	int nLuminSum = 0, nPreLuminSum = 0;		// ͼ�����������ۼӺͼ���ǰ�����ۼӺ�

	int nBlackFlag = 0, nStillFlag = 0;			// �ڳ�����֡ͼ���־
	int nStillCount = 0;						// ��Ǿ�֡������Ŀ
	int nThredCount = 0;						// �Ǿ�֡������ޣ�������������ǵ�10%����Ϊ�Ǿ�֡��������Ϊ��֡
	int step = pIn->step;
	//nThredCount = 50;							//��֡���Ŵ�50��
	int i =0;
	int ref_pos = 0;
	if(step < 1)
		step = 1;
	if(pVideoImage && pStillImageRef)
		nValidStillRef = 1;
	else
		goto end;

	// ���ݹ��ϼ������λ�������Ƿ�����Ӧ����
	nBlackFlag = (pIn->bBlackEnable == 0)? 0 : 1;
	nStillFlag = (pIn->bStillEnable == 0)? 0 : 1;

	if(pIn->osd_count > 0)
	{
		for(i = pIn->usYStart; i < pIn->usYEnd;i++)
		{
			int ret  = 0;
			ret = checkBlackFrameInOsd(pIn->usXStart,i,pIn);
			if(ret == 0)
			{
				break;
			}
		}
		ref_pos = pIn->nVideoWidth*i+pIn->usXStart;
	}

	//����10% ��Ϊ�Ǿ�֡
	nThredCount = ((pIn->usYEnd -  pIn->usYStart)/(16*step)) * ((pIn->usXEnd -  pIn->usXStart)/(16*step)) * pIn->stillRatio; // 10%
	
	// ��16*16���ؿ�Ϊ��λ���з���
	// ע�⣺���ʵ�ʼ������(ȥ����Ե�Ͳ���ͼ��̬�����)�������κο飬��ᵼ���󱨺ڳ�������ͼ����֡�ȹ��ϡ�
	//		 ������������������������ò�������ɣ�һ��Ӧ������²�����֣����������￼�ǵ�Ч�����⣬���ټ�顣
	for(nVideoRow = pIn->usYStart, nRefRow = 0;
			nVideoRow + 16 <= pIn->usYEnd; nVideoRow += 16*step, nRefRow += 16*step)
	{
		for(nVideoCol = pIn->usXStart, nRefCol = 0;
				nVideoCol + 16 <= pIn->usXEnd; nVideoCol += 16*step, nRefCol += 16*step)
		{		
			nLuminSum = 0;
			nLuminDiffSum = 0;
			nAbsLuminDiffSum = 0;

			// ���㵱ǰ�����Ͻ�����λ��
			nVideoImagePos = nVideoRow * pIn->nVideoWidth + nVideoCol;
			nImageRefPos = nRefRow * pIn->nImageRefWidth + nRefCol;

			// ��16*16 С���ؿ�Ϊ��λ����ȡ���ϽǴ������أ�ͬʱ���о�֡���ڳ�����
			for(nVideoBlockY = 0, nRefBlockY = 0; nVideoBlockY < 16; nVideoBlockY += 1, nRefBlockY++)
			{
				for(nVideoBlockX = 0, nRefBlockX = 0; nVideoBlockX < 16; nVideoBlockX += 1, nRefBlockX++)
				{
					int ret = 0;
					nVideoImagePos_Cur = nVideoImagePos + nVideoBlockY * pIn->nVideoWidth + nVideoBlockX;
					nImageRefPos_Cur = nImageRefPos + nRefBlockY * pIn->nImageRefWidth + nRefBlockX;
					
					ret = checkBlackFrameInOsd(nVideoCol+nVideoBlockX,nVideoRow+nVideoBlockY,pIn);
					if(ret == 1)
					{
						nVideoImagePos_Cur = ref_pos;
						nImageRefPos_Cur = ref_pos;
					}
					
					// �ڳ�������
					if(nBlackFlag)
						nLuminSum += (unsigned char)pVideoImage[nVideoImagePos_Cur];

					// ��֡������
					if(nStillFlag && nValidStillRef)
					{
						//nLuminDiff = (int)pVideoImage[nVideoImagePos_Cur] - (int)pStillImageRef[nImageRefPos_Cur];
						nLuminDiff = (int)pVideoImage[nVideoImagePos_Cur] - (int)pStillImageRef[nVideoImagePos_Cur];
						
						nLuminDiffSum += nLuminDiff;
						nAbsLuminDiffSum += abs(nLuminDiff);
					}

					// �ο�֡�����洫���������Դ˴�����ֵ
					// ����ǰͼ������ֵ��Ϊ��֡�ο�
					//pStillImageRef[nImageRefPos_Cur] = pVideoImage[nVideoImagePos_Cur];
				}
			}
			
			// ��֡�ڣ���16*16���ؿ�������ۼӺ���ǰһ���ؿ�����ж��Ƿ�Ϊ�ڳ�
			if(nBlackFlag
				&& !(nRefRow == 0 && nRefCol == 0)// �ӵ�2���鿪ʼ����
				&& abs(nLuminSum - nPreLuminSum) > pIn->usBlack)// ���ȱ仯������Χ�����Ǻڳ�
			{	
				//pOut->outargs.extendedError = abs(nLuminSum - nPreLuminSum); // for test
				nBlackFlag = 0;				
			}
			else
			{
				nPreLuminSum = nLuminSum;
			}

			// ��16*16���ؿ�Ϊ��λ���Ƚ�֡�����ؿ�����ȱ仯���ж��Ƿ�Ϊ��֡
			nLuminDiffSum = abs(nLuminDiffSum);
			if(nStillFlag && nValidStillRef
				&& ((nAbsLuminDiffSum > (10*pIn->usStill)) || (nLuminDiffSum > pIn->usStill && nAbsLuminDiffSum > (2*pIn->usStill))))
			{
				pOut->outargs.extendedError = nAbsLuminDiffSum;
				pOut->outargs.currentAU = nLuminDiffSum;
				nStillCount++;
				if(nStillCount > nThredCount)
				{
					nStillFlag = 0;
				}
			}

			//add by huanglb
			if ((0 == nStillFlag) && (0 == nBlackFlag))
			{
				//����
				goto end;
			}
		}
	}
	
end:
		
	pOut->bBlackFlag = nBlackFlag;
	pOut->bStillFlag = nStillFlag;
	pOut->usStillCnt = nStillCount;
	pOut->bTestImgFlag[0] = nStillCount;
}

/*
 *  ======== IMGENC1COPY_TI_process ========
 */
int alg_blackframe_process(ALG_BLACKFRAME_InArgs *inArgs, ALG_BLACKFRAME_OutArgs *outArgs)
{
	int numInBytes = 0;
    //Uint32       thisTransferSrcAddr, thisTransferDstAddr;

	ALG_BLACKFRAME_InArgs *pCheckInArgs = inArgs;
	ALG_BLACKFRAME_OutArgs *pCheckOutArgs = outArgs;

	unsigned char *pTestImgBufs[MAX_TESTIMAGE_NUM];

   
	numInBytes = inArgs->nVideoWidth * inArgs->nVideoHeight;

	inArgs->nImageRefWidth = (pCheckInArgs->usXEnd - pCheckInArgs->usXStart);
	inArgs->nImageRefHeight = (pCheckInArgs->usYEnd - pCheckInArgs->usYStart);


	pCheckOutArgs->outargs.extendedError = 0;
	pCheckOutArgs->outargs.currentAU = 0;

	// �ڳ�����֡������ͼ���
	alg_blackfreme_videocheck(pCheckInArgs, pCheckOutArgs, 
		(unsigned char *)(inArgs->cur_frame), (unsigned char *)(inArgs->last_frame), pTestImgBufs);

    /* outArgs->bytesGenerated reports the total number of bytes encoded */
    pCheckOutArgs->outargs.bytesGenerated = numInBytes;

    return 0;
}


int alg_blackframe_printf_info(char *print_buf,int iCh,ALG_BLACKFRAME_InArgs input,ALG_BLACKFRAME_OutArgs output)
{
	char *p = NULL;
	p = print_buf;

	p += sprintf(p,"%-3d|%-11u|%-11u|%-11f|%-5u|%-5u|%-6u|%-5u|%-6u|%-5u|%-9d|%-9d|%-9d|%-14d|%-9d|%-14d\r\n"
					,iCh
					,input.bBlackEnable
					,input.bStillEnable
					,input.stillRatio
					,input.usBlack
					,input.usStill
					,input.usXStart
					,input.usXEnd
					,input.usYStart
					,input.usYEnd
					,output.bBlackFlag
					,output.bStillFlag
					,output.usStillCnt
					,output.outargs.bytesGenerated
					,output.outargs.currentAU
					,output.outargs.extendedError);
	/*p += sprintf(p,FORMAT_TITLE,"alg blackframe");
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input bBlackEnable",input.bBlackEnable);
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input bStillEnable",input.bStillEnable);
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input usBlack",input.usBlack);
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input usStill",input.usStill);
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input usXStart",input.usXStart);
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input usXEnd",input.usXEnd);
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input usYStart",input.usYStart);
	p += sprintf(p,FORMAT_32STR_UINT,"alg bf input usYEnd",input.usYEnd);
	
	p += sprintf(p,FORMAT_32STR_INT,"alg bf output bBlackFlag",output.bBlackFlag);
	p += sprintf(p,FORMAT_32STR_INT,"alg bf output bStillFlag",output.bStillFlag);
	p += sprintf(p,FORMAT_32STR_INT,"alg bf output Generated",output.outargs.bytesGenerated);
	p += sprintf(p,FORMAT_32STR_INT,"alg bf output currentAU",output.outargs.currentAU);
	p += sprintf(p,FORMAT_32STR_INT,"alg bf output extenderr",output.outargs.extendedError);*/
	
	return (p-print_buf);
}

