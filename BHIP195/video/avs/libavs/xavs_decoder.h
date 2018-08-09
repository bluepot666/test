#ifndef _XAVS_DECODER_H
#define _XAVS_DECODER_H

typedef struct tagxavs_param
{
	
	unsigned  int i_color_space;
	unsigned char *p_in;//ѹ��֡����������,����������ͷ��ʼ������
	unsigned  int  i_in_length;//ѹ��֡����
	unsigned char *p_out;//�����ͼ�󻺳���
	unsigned  int i_out_pitch;//������ͼ�󻺳���pitchֵ

	long           l_result;
}xavs_param;
typedef struct tagxavs_seq_info
{
	int iWidth;
	int iHeight;
	int iAspectRatioX;
	int iAspectRatioY;
	int iFrameRateX;
	int iFrameRateY;
	int iProgressiveSequence;
	  
}xavs_seq_info;

#define XAVS_SEQ_HEADER  1
#define XAVS_FRAME_OUT  2


typedef struct AVSFrame 
{
	int nWidth;      //ͼ����
		
	int nHeight;
		
	unsigned char *data[4];   //0��1��2�ֱ��Y��U��V
	
	int linesize[4];     //0��1��2�ֱ��Y��U��V�Ŀ��
	
	int nFrameType;   //֡���� 0I 1P 2B
	
	int nBitrate;     //����

	int nFrameCoded;
	int nTopFieldFirst;
	int nFrameIndex;
	void *param0;
	void *param1;
} AVSFrame;

//����decoder
int xavs_decoder_create(void **pp_decoder);

//����ѹ������
int xavs_decoder_put_data(void *p_decoder,unsigned char *data,int len);
//��ȡ�������ݣ����got_frame��ý���֡�������ȡ��ֱ��got_frame����Ϊ0
int xavs_decoder_get_decode_video(void *p_decoder,int *got_frame,AVSFrame *data);
//��λ������
int xavs_decoder_reset(void *p_decoder);
//�ͷŽ�����
void xavs_decoder_destroy(void *p_decoder);
//��λ����֡buffer
int xavs_decoder_reset_buffer(void *p_decoder);
//
//must a complete frame
int xavs_decoder_decode_frame(void *p_decoder,unsigned char *p_buf,int i_len,int *have_frame,AVSFrame *p_param);
////�����������ڲ�ʹ�ã��ⲿ������ʹ��
int xavs_decoder_get_seq(void *p_decoder,xavs_seq_info *p_si);

//�������ڣ��������ö�֡ģʽ��0����֡��ΪĬ��ֵ�� 1��B֡�ȷǲο�֡��2ֻ����I֡�����øò�������ܻḴλ������
int xavs_decoder_set_frame_skip_mode(void *p_decoder,int skip_mode);

//���ݻ��������֡����
int xavs_decoder_get_frame_type(void *p_decoder, unsigned char *data,int len);
int xavs_get_frame_type(unsigned char *data,int len, int profile_id);



void *xavs_malloc(int);
void *xavs_realloc(void *p, int i_size);
void  xavs_free(void *);

typedef int  (* xavs_decoder_create_t)(void **pp_decoder);
typedef int  (* xavs_decoder_put_data_t)(void *p_decoder,unsigned char *data,int len);
typedef int  (* xavs_decoder_get_decode_video_t)(void *p_decoder,int *got_frame,AVSFrame *data);
typedef int  (* xavs_decoder_reset_t)(void *p_decoder);
typedef void (* xavs_decoder_destroy_t)(void *p_decoder);
typedef int  (* xavs_decoder_reset_buffer_t)(void *p_decoder);
typedef int  (* xavs_decoder_decode_frame_t)(void *p_decoder,unsigned char *p_buf,int i_len,int *have_frame,AVSFrame *p_param);
typedef int  (* xavs_decoder_get_seq_t)(void *p_decoder,xavs_seq_info *p_si);
typedef int  (* xavs_get_frame_tye_t)(unsigned char *data,int len, int profile_id);
typedef int  (* xavs_decoder_set_frame_skip_mode_t)(void *p_decoder,int skip_mode);
typedef int  (* xavs_decoder_get_frame_type_t)(void *p_decoder, unsigned char *data,int len);

typedef int  (* xavs_get_frame_type_t)(unsigned char *data,int len, int profile_id);
typedef struct tag_xavs_decoder_funs
{
	xavs_decoder_create_t                   create;
	xavs_decoder_set_frame_skip_mode_t      set_frame_skip_mode;
	xavs_decoder_put_data_t                 put_data;
	xavs_decoder_get_decode_video_t         get_decode_video;
	xavs_decoder_reset_t                    reset;
	xavs_decoder_destroy_t                  destroy;
	xavs_decoder_reset_buffer_t             reset_buffer;
	xavs_decoder_decode_frame_t             decode_frame;
	xavs_decoder_get_seq_t                  get_seq;
	xavs_decoder_get_frame_type_t           get_frame_type;
	//xavs_get_frame_tye_t                    xavs_get_frame_type;
}xavs_decoder_funs;
typedef int  (* xavs_decoder_load_funs_t)(xavs_decoder_funs *funs, int mode);
int xavs_decoder_load_funs(xavs_decoder_funs *funs, int mode);

#ifdef WIN32
#define __LoadLibrary LoadLibrary
#define __GetProcAddress GetProcAddress

#else
typedef void * HMODULE;
#define __LoadLibrary(filename) dlopen(filename,RTLD_LAZY)
#define __GetProcAddress dlsym
#endif

#define XAVS_LOAD_LIB(filename, funs, mode) \
	HMODULE handle = LoadLibrary(filename); \
	xavs_decoder_load_funs_t load_fun =  (xavs_decoder_load_funs_t)GetProcAddress(handle,"xavs_decoder_load_funs"); \
    if(load_fun != NULL) \
	{\
	     load_fun(&funs, mode);\
	}	

#endif