/*
 * Chinese AVS video (AVS1-P2, JiZhun profile) decoder.
 * Copyright (c) 2006  Stefan Gehrer <stefan.gehrer@gmx.de>
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
/*************************************************************
chengdu WP 20070526 - 20070730
secondwang@163.com
xavs������2.0�汾

**************************************************************/

#ifndef _XAVS_GLOBE_H
#define _XAVS_GLOBE_H

#include <memory.h>
#include <math.h>
#include <limits.h>
extern uint8_t crop_table[];
extern const xavs_vector MV_NOT_AVAIL;
extern const xavs_vector MV_REF_DIR;
extern const xavs_vector MV_INTRA;
extern const xavs_mvref MVREF_NOT_AVAIL;
extern const uint8_t dequant_shift[64];
extern const uint16_t dequant_mul[64];
extern const uint8_t zigzag_progressive[64];
extern const uint8_t zigzag_field[64];
extern const uint8_t chroma_qp[64] ;
extern const xavs_vlc intra_2dvlc[7];
extern const xavs_vlc inter_2dvlc[7];
extern const xavs_vlc chroma_2dvlc[5];
extern const uint8_t partition_flags[30];
extern const int frame_rate_tab[][2];
extern const uint8_t mv_scan[4];
static inline int xavs_clip(int a, int amin, int amax)
{
     if (a < amin)      return amin;
     else if (a > amax) return amax;
     else               return a;
}
static inline uint8_t xavs_clip_uint8(int a)
{
     if (a&(~255)) return (-a)>>31;
     else          return a;
}



static inline int clip3_int( int v, int i_min, int i_max )
{
    return ( (v < i_min) ? i_min : (v > i_max) ? i_max : v );
}

static inline float clip3_float( float v, float f_min, float f_max )
{
    return ( (v < f_min) ? f_min : (v > f_max) ? f_max : v );
}
static inline int median( int a, int b, int c )
{
    int min = a, max =a;
    if( b < min )
        min = b;
    else
        max = b;    

    if( c < min )
        min = c;
    else if( c > max )
        max = c;

    return a + b + c - min - max;
}

static inline	uint32_t xavs_next_startcode(uint8_t *p_start,int i_len,int *p_ret)
{
	uint32_t i_startcode;
	uint8_t *p_buf=p_start;
	uint8_t *p_stop;
	p_stop=p_buf+i_len;
	if(i_len<4)
	{
		*p_ret=0;
		return 0;
	}
    i_startcode = p_buf[0];
	i_startcode = (i_startcode << 8) + p_buf[1];
	i_startcode = (i_startcode << 8) + p_buf[2];
	p_buf+=3;
	while(p_buf<p_stop)
	{
		i_startcode = (i_startcode << 8) + *p_buf;
		if((i_startcode&0xffffff00)==0x00000100)
		{
			*p_ret=(int)(p_buf-p_start)-3;
			return i_startcode;
		}
		p_buf++;
	}
	*p_ret=i_len-3;
	return 0;
}

static inline void copy_mvref_dir_type(xavs_mvref *mvref, xavs_block_size bsize)
{

	switch(bsize)
	{
    case BLK_16X16:
		mvref[MV_STRIDE  ].dir = mvref[0].dir;
		mvref[MV_STRIDE  ].type = mvref[0].type;
		mvref[MV_STRIDE + 1].dir = mvref[0].dir;
		mvref[MV_STRIDE + 1].type = mvref[0].type;
    case BLK_16X8:
		mvref[1].dir = mvref[0].dir;
		mvref[1].type = mvref[0].type;
		
        break;
    case BLK_8X16:
		mvref[MV_STRIDE].dir = mvref[0].dir;
		mvref[MV_STRIDE].type = mvref[0].type;
        break;
    }
}

static inline void copy_mvref(xavs_mvref *mvref, xavs_block_size bsize)
{

	switch(bsize)
	{
    case BLK_16X16:
		mvref[MV_STRIDE  ] = mvref[0];
		mvref[MV_STRIDE + 1] = mvref[0];
    case BLK_16X8:
		mvref[1] = mvref[0];
        break;
    case BLK_8X16:
		mvref[MV_STRIDE] = mvref[0];
        break;
    }
}
static inline void copy_mvs(xavs_vector *mv, enum xavs_block_size size) 
{

	//��sizeΪBLK_16X16��ʱ���൱�ڰ�X0��ֵ����X1,X2,X3
	//��BLK_16X8ʱ��X0��ֵ����X1,X2��ֵ����X3
	//��BLK_8X16ʱ��X0��ֵ����X2,X1��ֵ����X3
    switch(size)
	{
    case BLK_16X16:
        mv[MV_STRIDE  ] = mv[0];
        mv[MV_STRIDE+1] = mv[0];
    case BLK_16X8:
        mv[1] = mv[0];
        break;
    case BLK_8X16:
        mv[MV_STRIDE] = mv[0];
        break;
    }
}


typedef struct tagxavs_aec_ctx
{
	unsigned char	mps;   
	unsigned char	cycno;
	unsigned int	lgPmps; 	
}xavs_aec_ctx;


typedef struct tagxavs_aec_decoder
{
	xavs_bitstream * stream;
	xavs_aec_ctx    *ctxes;
	int              ctx_count;

	uint32_t valueS,valueT;
	uint32_t rS1,rT1;


}xavs_aec_decoder;




typedef struct tagxavs_decoder
{
	xavs_bitstream                       s;//bit����ȡ�ṹ
	xavs_video_sequence_header           vsh;//��Ƶ����ͷ
	xavs_picture_header                  ph;//ͼ��ͷ
	xavs_slice_header                    sh;//Ƭͷ
	uint8_t                             *p_user_data;//�û�����
	xavs_sequence_display_extension     *p_sde;//������ʾ��չͷ
	xavs_copyright_extension            *p_ce;//��Ȩ��չ
	xavs_camera_parameters_extension    *p_cpe;//�����������չ
	xavs_picture_display_extension      *p_pde;//ͼ����ʾ��չ
	uint8_t                              b_extention_flag;
	/*0 ��ʾ��չ����֮ǰ������ͷ����ʾ��չ֮ǰ��ͼ��ͷ*/
	uint32_t                              i_video_edit_code_flag;//˵Ʒ�༭��־


	

	uint8_t                              b_get_video_sequence_header;//���������Ƶ����ͷ�����øñ�־
	uint8_t                              b_get_i_picture_header;//�������ͼ��ͷ�����øñ�־
	uint8_t                              b_have_pred;//��ǰ�����Ƿ�����֡��򳡼�Ԥ��
	uint32_t                             i_mb_width,i_mb_height,i_mb_num,i_mb_num_half;//���ߴ�
	uint32_t                             i_mb_x,i_mb_y,i_mb_index,i_mb_offset;  //��������ƫ�Ƶ�
	uint32_t                             b_complete; // ������һ֡�ˣ������������ 
	uint32_t                             i_mb_flags; // ����־��ʾ�ú���ܱ߿��Ƿ���Եõ� 
	uint32_t                             b_first_line; //Ƭ�ĵ�һ�б�־


	xavs_image image[3];//Ԥ�ȷ����3��֡ͼ�񣬶�Ӧ��������ο�֡��һ����ǰ����֡��Ҳ���Էֽ��4���ο�������ǰ֡��2����
	xavs_image ref[4];//�ĸ��ο������������ο�֡
	//
	xavs_image *p_save[3];//���ڳ������ʱ�򱣴�֡��ʹ��
	xavs_image cur;//������ĳ�
	uint8_t *p_y,*p_cb,*p_cr,*p_edge;//��ǰ���YUV��ַ��p_edgeΪ֡��Ԥ��ʱ��ʱ��ű�Ե���ص�1/2��1/4���ص�����ֵ
//
	//��Щ���Ǽ����˶�ʸ����ϵ�������ڼӿ��ٶ�
    int i_sym_factor;    ///<����B��ĶԳ�ģʽ
    int i_direct_den[4]; ///< ����B���ֱ��ģʽ
    int i_scale_den[4];  ///< �����ٽ����Ԥ���˶�ʸ���ļ���
	int i_ref_distance[4];

    int i_left_qp;
    uint8_t *p_top_qp;//���ں��Ļ�·�˲�
	int i_qp;//��ǰʹ�õ�qp��
    int b_fixed_qp;//�̶�qp��־��������slice�иı�
    int i_cbp;//��ǰ����cbp
	int i_luma_offset[4];//�ĸ��������X0�ĵ�ַƫ��
    /** 
       0:    D3  B2  B3  C2
       4:    A1  X0  X1   -
       8:    A3  X2  X3   - */
	//24������ǰ���������˶�ʸ��
    xavs_vector mv[24];
	xavs_vector *p_top_mv[2];//����һ�п���˶�ʸ��
    xavs_vector *p_col_mv;//�ο�λ�õĶ�Ӧ�˶�ʸ��

	
	xavs_mvref mvref[24];//����24������ǰ���������˶�ʸ�� ��Ϊaec���������Ĳο�
	xavs_mvref *p_top_mvref[2];//����һ�п���˶�ʸ�� ��Ϊaec���������Ĳο�

	xavs_mvd    mvd_a[4]; //���aecʹ��,/��׼��a�鱣���˶�ʸ�����Ϊaec���������Ĳο�
	
    /** luma pred mode cache
       0:    D3  B2  B3
       3:    A1  X0  X1
       6:    A3  X2  X3   */
    int i_intra_pred_mode_y[9];//������ʱ��������Ÿ��������Ԥ��ģʽ
    int *p_top_intra_pred_mode_y;//��������һ�е�Ԥ��ģʽ

	int i_intra_pred_mode_chroma_a;//��׼��a��λ�õ�ɫ��֡��Ԥ��ģʽ
	int *p_top_intra_pred_mode_chroma;//��������һ�е�Ԥ��ģʽ


	uint8_t   i_cbp_a;//��׼��a��λ�õ�cbp, ��Ϊaec���������Ĳο�
	uint8_t * p_top_cbp;//��������һ�е�Ԥ��ģʽ

	uint8_t   i_mb_type_a;//��׼��a��ĺ�����ͣ���Ϊaec���������Ĳο�
	uint8_t  *p_top_mb_type;//��һ�п������� 


	int       i_last_qp;//��һ��qp ��Ϊaec���������Ĳο�
	int       i_last_qp_delta;//��һ��qp delta����Ϊaec���������Ĳο�


    
	//�������ڲ�ͬ�Ŀ��Ӧ�ı�Ե��һ�������õ��ս�������ݣ�����������Щ�ұ߿鲢δ��������������һЩԤ��ģʽ����ʹ��
	//���е���Щ��������deblockǰ��������
	//���ڱ���֡��Ԥ��ĵ�ǰ����ٽ��������е�y������׼��c[0~16]
    uint8_t *p_top_border_y,*p_top_border_cb,*p_top_border_cr; 
	;//���ڱ���֡��Ԥ��ĵ�ǰ����ٽ�������е�y����
	uint8_t i_left_border_y[26],i_left_border_cb[10],i_left_border_cr[10];
	//�����м�߽������ı��棬����X1,X3��Ԥ��
    uint8_t i_internal_border_y[26];
    uint8_t i_topleft_border_y,i_topleft_border_cb,i_topleft_border_cr;//�����׼�е�c[0]��r[0]
    uint8_t *p_col_type_base;
	//���ڱ���������ͣ���ʵֻ��B_SKIP��B_Direct���õ�������ֻ��Ҫ����P֡�ľͿ�����
    uint8_t *p_col_type;//��ǰ������͵�ƫ�Ƶ�ַ	
//����в�����
    DCTELEM *p_block;


	uint8_t *p_wqM88;//��Ȩ��������
	//
	//
	unsigned char *data_buf;//���뻺��������
	int  data_buf_len;//������ʵ����Ч�ֽ�
	int  data_buf_size;//����������Ĵ�С
	int  data_buf_pos;//������������Ч������ʼλ��

	xavs_aec_decoder *aec_decoder;//aec������

	
	int i_frame_skip_mode;//��֡ģʽ
}xavs_decoder;
int xavs_init_slice(xavs_decoder *p);
//int xavs_check_stream_end(xavs_decoder *p);


//vlc
int xavs_get_all_mbs(xavs_decoder *p);

///aec decoder
int xavs_aec_decoder_init(xavs_decoder *p);
int xavs_aec_decoder_get_all_mbs(xavs_decoder *p);
void xavs_aec_decoder_destroy(xavs_decoder *p);

void xavs_mb_init(xavs_decoder *p);
void xavs_mb_filter(xavs_decoder *p, int i_mb_type);
int  xavs_mb_next(xavs_decoder *p);



static int inline xavs_check_index(xavs_decoder *p)
{
	if(p->i_mb_index >= p->i_mb_num_half && p->i_mb_offset == 0 &&!p->ph.b_picture_structure)
	{
		return 1;
	}
	if(p->i_mb_index>=p->i_mb_num)
	{
		p->b_complete=1;
		return 1;
	}
	return 0;

}

static int inline xavs_check_stream_end(xavs_decoder *p)
{

	if(xavs_check_index(p))
	{
		return 1;
	}
	

	//if((p->i_mb_index%p->i_mb_width)==0)
	{
		if(xavs_bitstream_eof(&p->s))
		{
			return 1;
		}
	}
	return 0;
}


#endif 
