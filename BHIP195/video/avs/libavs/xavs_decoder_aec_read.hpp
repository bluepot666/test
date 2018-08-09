

//////////////////////////////////////////////////

//offset  = 0 - 3
static inline int  aec_read_skip_run(xavs_decoder *p)
{
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 0) == 1)
	{
		return 0;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 1) == 1)
	{
		return 1;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 2) == 1)
	{
		return 2;
	}

	int value = 3;
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 3) == 0)
	{
		value++;
	}
	return value;
}
//offset  = 4 - 8 ����P ��I֡�ڶ���
static inline int aec_read_mb_type_p(xavs_decoder *p)
{
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 4) == 1)
	{
		return 0;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 5) == 1)
	{
		return 1;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 6) == 1)
	{
		return 2;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 7) == 1)
	{
		return 3;
	}

	int value = 4;
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 8) == 0)
	{
		value++;
		
	}
	return value;
}

static inline int is_mb_type_skip(int i_mb_type)
{
	return i_mb_type == P_SKIP || i_mb_type == B_SKIP || i_mb_type == B_DIRECT ;
}
//offset  = 9 - 18 ��B֡
static inline int aec_read_mb_type_b(xavs_decoder *p)
{

	int offset = 9 ;
	if((p->i_mb_flags & A_AVAIL) && is_mb_type_skip(p->i_mb_type_a) == 0)
	{
		offset++;
	}
	if((p->i_mb_flags & B_AVAIL) && is_mb_type_skip(p->p_top_mb_type[p->i_mb_x])  == 0)
	{
		offset++;
	}
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 0)
	{
		return 0;
	}
//
	int value = 0;
	for( offset = 12; offset < 19; offset++)
	{
		value++;
		if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 1)
		{
			return value;
		}
	}
//��ʱoffset = 19������Ҫ��һλ;
	offset--;
	value++;
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 0)
	{
		value++;
		
	}
	return value;
}

//offset = 19 - 21
static inline int aec_read_mb_part_type(xavs_decoder *p)
{
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 19) == 0)
	{
		return aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 20);
	}
	else
	{
		return aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 21) + 2;
	}
	
}


//offset = 22 - 25
static inline int  aec_read_intra_luma_pred_mode(xavs_decoder *p)
{
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 22) == 1)
	{
		return 0;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 23) == 1)
	{
		return 1;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 24) == 1)
	{
		return 2;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 25) == 1)
	{
		return 3;
	}
	return 4;
}

//offset = 26 - 29
static inline int  aec_read_intra_chroma_pred_mode(xavs_decoder *p)
{
	int index = 26;
	if((p->i_mb_flags & A_AVAIL) && p->i_intra_pred_mode_chroma_a != 0)
	{
		index++;
	}
	if((p->i_mb_flags & B_AVAIL) && p->p_top_intra_pred_mode_chroma[p->i_mb_x] != 0)
	{
		index++;
	}
		
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + index) == 0)
	{
		return 0;
	}
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 29) == 0)
	{
		return 1;
	}
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 29) == 0)
	{
		return 2;
	}

	return 3;
}
//offset = 30 - 35 
static inline int aec_read_ref_p(xavs_decoder *p, xavs_mvref *mvref)
{

	//���ձ�׼�ǲ���Ҫ�жϺ�������Ƿ�ΪP_SKIP�ģ����ǲο���������Ҫ�ж�
	int offset = 30;
	if(mvref[-1].ref > 0 && mvref[-1].type != 0)
	//if(mvref[-1].ref > 0)
	{
		offset++;
	}
	if(mvref[-4].ref > 0  && mvref[-4].type != 0)
	//if(mvref[-4].ref > 0)
	{
		offset += 2;
	}
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 1)
	{
		return 0;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 34) == 1)
	{
		return 1;
	}
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 35) == 1)
	{
		return 2;
	}

	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 35) == 0)
	{
		break;
	}
	return 3;
}
static int is_block_weighting( xavs_mvref *fmvref, int offset)
{
	if(fmvref[offset].type != 0)	
	{
		if( (((fmvref[offset].dir& fmvref[0].dir) & 1) == 1  && (fmvref[offset + 0           ].ref & 1) > 0) 
		 || ((fmvref[offset].dir == fmvref[0].dir && fmvref[0].dir == 2) && (fmvref[offset + MV_BWD_OFFS ].ref    ) > 0))
		{
			return 1;
		}
	}
	return 0;
}
static inline int aec_read_ref_b(xavs_decoder *p,   xavs_mvref *fmvref)
{
	
		
	int offset = 30 + is_block_weighting(fmvref, -1);
	if(is_block_weighting(fmvref, -4))	
	{
		offset += 2;
		
	}

	
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}




static inline int aec_read_mv_abs_value(xavs_decoder *p, int table_offset, int first_offset)
{
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + first_offset) == 0)
	{
		return 0;
	}
	
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + table_offset + 3) == 0)
	{
		return 1;
	}
		
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + table_offset + 4) == 0)
	{
		return 2;
	}
			
	int t =  aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + table_offset + 5);
	int i = 0, j ,value = 0;
	while(aec_decode_bypass(p->aec_decoder) == 0)
	{
		i++;
	}
	for(j = 0; j < i;  j++)
	{
		value <<= 1;
		value += aec_decode_bypass( p->aec_decoder );	
	}
 
	return (( 1 << i) - 1 + value) * 2 + 3 + t;
}

static inline int aec_read_mv_value(xavs_decoder *p, int table_offset, int first_offset)
{
	int value = aec_read_mv_abs_value(p, table_offset, first_offset);
	if(value)
	{
		if(aec_decode_bypass(p->aec_decoder))
		{
			return - value;

		}
		else
		{
			return value;
		}
	}
	return 0;

}
//offset = 36 - 47
//����xΪ36 - 41 ,yΪ42 -47
static inline int aec_read_mv(xavs_decoder *p, xavs_mvd *mvd, int enable, int table_offset, int value)
{
	
	int first_offset = table_offset;
	if(enable)
	{
		if(value > 15)
		{
			first_offset += 2;
		}
		else if(value >1)
		{
			first_offset++;
		}
	}
	
	return aec_read_mv_value(p, table_offset, first_offset);
}

//offset = 48 - 53
static inline int  aec_read_cbp(xavs_decoder *p)
{
	int cbp = 0;
	int index = 48;

	if((p->i_mb_flags & A_AVAIL) && (p->i_cbp_a & 0x2) == 0)
	{
		index++;
	}
	if((p->i_mb_flags & B_AVAIL) && (p->p_top_cbp[p->i_mb_x] & 0x4) == 0)
	{
		index += 2;
	}
	xavs_aec_ctx *ctx = p->aec_decoder->ctxes + index;
	cbp += aec_decode_decision(p->aec_decoder, ctx) << 0;

	index = 48;
	if(cbp == 0)
	{
		index++;
	}
	if((p->i_mb_flags & B_AVAIL) && (p->p_top_cbp[p->i_mb_x] & 0x8) == 0)
	{
		index += 2;
	}
	ctx = p->aec_decoder->ctxes + index;
	cbp += aec_decode_decision(p->aec_decoder, ctx) << 1;	

	index = 48;
	if((p->i_mb_flags & A_AVAIL) && (p->i_cbp_a & 0x8) == 0)
	{
		index++;
	}
	if((cbp & 0x1) == 0)
	{
		index += 2;
	}
	ctx = p->aec_decoder->ctxes + index;
	cbp += aec_decode_decision(p->aec_decoder, ctx) << 2;

	index = 48;
	if((cbp & 0x4) == 0)
	{
		index++;
	}
	if((cbp & 0x2) == 0)
	{
		index += 2;
	}
	ctx = p->aec_decoder->ctxes + index;
	cbp += aec_decode_decision(p->aec_decoder, ctx) << 3;


	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 52) == 0)
	{
		return cbp;
	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 53) == 1)
	{
		cbp += 48;
		return cbp;

	}

	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 53) == 1)
	{
		cbp += 32;
	}
	else
	{
		cbp += 16;
	}
	return cbp;
}


static inline void  aec_read_stuffing_bit(xavs_decoder *p)
{

	aec_decode_stuffing_bit(p->aec_decoder);
}







//offset 54 - 57
static inline int  aec_read_qp_delta_real(xavs_decoder *p)
{
	if(p->i_last_qp_delta == 0)
	{
		if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 54) == 1)
		{
			return 0;
		}
	}
	else
	{
		if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 55) == 1)
		{
			return 0;
		}
	}
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 56) == 1)
	{
		return 1;
	}

	int value = 2;
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 57) == 0)
	{
		value++;
	}
	
	return value;
}

//offset 58 -321
//����֡ģʽ����  58 - 123
//����֡ģʽɫ�� 124 - 189
//���г�ģʽ���� 190 - 255
//���г�ģʽɫ�� 256 - 321

static inline int  aec_read_qp_delta(xavs_decoder *p)
{
	int delta = aec_read_qp_delta_real(p) + 1;
	if(delta & 1)
	{
		delta = -(delta >> 1);
	}
	else
	{
		delta = (delta >> 1);
	}
	p->i_last_qp_delta = delta;
	return delta;
	
}



static inline int  aec_read_trans_coefficent_level_max_0(xavs_decoder *p, int offset)
{
	//��ʱlMax = 0��Ӧ��priIdx = 0 �÷Ǽ�Ȩ��ʽ,���Զ��ڵ�һbit����binIndex = 0��ʱ��
	//ctxIdxInc = priIdx * 3 + secIdx - (priIdx != 0) = 0 + 0 - 0 = 0;
	//�ο�������ʹ��1 ��Ҫʹ��offset++
//binIdx = 0 secIdx = 0
#if 0
	
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 1)
	{
		return 0;
	}
//binIdx = 1  secIdx = 1
	offset++;
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 1)
	{
		return 1;
	}
//binIndex >= 2 secIdx = 2
	offset++;
	int value = 2;
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 0)
	{
		value++;
		
	}
#else

	
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 1)
	{
		return 0;
	}
	offset++;
	int value = 1;
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 0)
	{
		value++;
		
	}
#endif
	return value;
}

				   
static inline int  aec_read_trans_coefficent_level_max_other(xavs_decoder *p,  int i_max,int offset, int pos)
{
	//��ʱlMax ��1 - 4֮��priIdx�� 1-3֮�� �÷Ǽ�Ȩ��ʽ,���Զ��ڵ�һbit����binIndex = 0��ʱ��
	//�ü�Ȩ��ʽ��
	static int prefIdx3_1[6] ={0,2,5,8,8,11};
	int offsetW = offset + 14 + ((pos >> 5) << 4) + ((pos >> 1) & 0x0F);
	offset += prefIdx3_1[i_max > 4 ? 5 : i_max ];
	//binIdx = 0 secIdx = 0
	if(aec_decode_decision_weighting(p->aec_decoder, p->aec_decoder->ctxes + offset,p->aec_decoder->ctxes + offsetW ) == 1)
	{
		return 0;
	}
//binIndex = 1 secIdx = 1
	offset++;
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 1)
	{
		return 1;
	}
//binIndex >= 2 secIdx = 2
	offset++;
	int value = 2;
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 0)
	{
		value++;		
	}
	return value;

}


static inline int  aec_read_trans_coefficent_sign(xavs_decoder *p)
{
	return aec_decode_bypass(p->aec_decoder);
}

static inline int  aec_read_trans_coefficent_run(xavs_decoder *p, int i_max, int offset, int level)
{
	//ctxIdxInc = priIdx * 4 + secIdx
	static int preIdx_4_46[6] ={46,50,54, 58,58, 62};
	int value = 0;
	offset += preIdx_4_46[i_max > 4 ? 5 : i_max];	
	if(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset + (level > 1 ? 2 : 0)) == 1)
	{
		return value;
	}

	value++;
	offset += (level > 1 ? 3 : 1);
	while(aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + offset) == 0)
	{
		value++;
	}
	return value;
}


//offset = 322 - 322
static inline int aec_read_weighting_prediction(xavs_decoder *p)
{
	return aec_decode_decision(p->aec_decoder, p->aec_decoder->ctxes + 322);
}



