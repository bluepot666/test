#ifndef __XDRA_DECODER_H
#define __XDRA_DECODER_H
typedef struct tagxdra_frame_header
{
	int frame_header_type;
	int num_word;
	int num_block_per_frame;
	int sample_rate_index;//����Ƶ������
	int num_normal_channel;//����ͨ����
	int num_lfe_channel;//��Ƶ��ǿ������
	int aux_data;
	int use_sum_diff;
	int use_jic;
	int jic_cb;
}xdra_frame_header;
//�������ߺ���
//Ѱ��֡ͷ�������������������ڴ��뻺������ַ��ƫ��,
//û�ҵ�����-1
int xdra_next_sync_word(unsigned char *buf, int len);
//����֡ͷ�����������0x7FFF��ʼ�Ļ�������size����С��4
//����ʧ�ܷ���-1
int xdra_parse_header(unsigned char *buf, int len,  xdra_frame_header *header);
//���ݽ���������֡ͷ�������õ�������,index��ΧΪ0��12
int xdra_get_sample_rate(int index);
//////
//����������,��������ַ����ΪNULL,Ҳ������ͬ���ֿ�ʼ��һ֡����
void *xdra_decoder_create(unsigned char *buf, int len);
//���֡ͷ��Ϣ���˺���Ҫ�ɹ�������put_data���ؽ���һ֡�ɹ��Ժ�
int xdra_decoder_get_frame_header(void *decoder, xdra_frame_header *header);
//���������ͻ�������������֡���룬Ҳ���Բ���֡���룬�ڲ��Զ����ж�,
int xdra_decoder_put_data(void *decoder, unsigned char *data, int len);
//������һ֡���ڵ���xdra_decoder_decode_begin�󣬵���xdra_decoder_decode_next_frameֱ������ʧ�ܣ�ʧ��Ϊ�������ɹ�Ϊ0
int xdra_decoder_decode_next_frame(void *decoder);

//����һ֡���ݣ��������������һ֡����������ͬ���ֿ�ʼ��ǰ�����ֽ�Ϊ0x7F 0xFF
int xdra_decoder_decode_frame(void *decoder, unsigned char *frame, int len);
//���xdra_decoder_decode_next_frame��xdra_decoder_decode_frame����ɹ�,�������������������ý�����PCM���ݣ�����ɹ����ǵķ���ֵΪ����������
//xdra_decoder_get_channel_f32��xdra_decoder_get_channel_s16�̶�Ϊ1024��xdra_decoder_get_stero_s16�̶�Ϊ2048���ʧ��Ϊ-1
//���������Ļ�ø����ͽ������ݣ�
int xdra_decoder_get_channel_f32(void *decoder, int channel, float *buf, int size);
//�����������з��ŵĶ����ͽ�������
int xdra_decoder_get_channel_s16(void *decoder, int channel, short *buf, int size);
//����˫ͨ���ĵ��з��ŵĶ����ͽ������ݣ������������ֻ��һ���������ݣ����Ƹ��������γ�˫������
int xdra_decoder_get_stero_s16(void *decoder, short *buf, int size);
//��������λ
void xdra_decoder_reset(void *decoder);
//�������ͷ���Դ
void xdra_decoder_destroy(void *decoder);

#endif
