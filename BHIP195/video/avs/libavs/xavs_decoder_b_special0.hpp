
#ifdef XAVS_INIT_REF_DISTANCE

//���ں���ο�ͼ��Ϊ֡���룬�����˶�ʸ��ָ��Ŀ�ҲӦ���ǲ�������ĳ��������DistanceIndexRefӦ��ʼ��Ϊż��
//���ں���ο�ͼ��Ϊ�����룬�����˶�ʸ��ָ��Ŀ�϶�������ĳ��������DistanceIndexRef����Ϊ����Ҳ����Ϊż��
//����curΪ����ο�ͼ��,refΪ�˶�ʸ��ָ��Ŀ����ڵ�֡��
void set_field_image_ref_info(xavs_image *cur, xavs_image *ref, int offset, int num, int next_field)
{
	if(ref == NULL)
	{
		return ;
	}
	int * ref_top_field = !next_field ? cur->i_first_field_ref_top_field : cur->i_next_field_ref_top_field;
	uint32_t *ref_distance = !next_field ? cur->i_first_field_ref_distance : cur->i_next_field_ref_distance;
	if(cur->b_picture_structure)
	{

		ref_distance = cur->i_frame_ref_distance;
	}
	
	if(ref->b_picture_structure)
	{
		//ʼ��Ϊż��
		ref_distance[0 + offset] = ref->i_distance_index ;	
		ref_top_field[0 + offset] = !ref->b_top_field_first;
		if(num > 1)
		{
			ref_distance[1 + offset] = ref->i_distance_index ;	
			ref_top_field[1 + offset] = ref->b_top_field_first;
		}
	}
	else
	{
		ref_distance[0 + offset] = ref->i_distance_index  + 1 ;	
		ref_top_field[0 + offset] = !ref->b_top_field_first;
		if(num > 1)
		{
			ref_distance[1 + offset] = ref->i_distance_index + 0 ;	
			ref_top_field[1 + offset] = ref->b_top_field_first;
		}
	}

}

static inline void init_ref_distatnce(xavs_decoder *p, uint32_t b_next_field)
{
	
	if(p->ph.i_picture_coding_type == XAVS_B_PICTURE)
	{
		return ;
	}
	if(p->vsh.b_progressive_sequence)
	{//�������У�ֻ��P֡����ǰ��ο�֡
		if(p->ph.i_picture_coding_type == XAVS_I_PICTURE)
		{
			return ;
		}
		if(p->p_save[1])
		{
			p->p_save[0]->i_frame_ref_distance[0]  = p->p_save[1]->i_distance_index;	
		}
		if(p->p_save[2])
		{
			p->p_save[0]->i_frame_ref_distance[1]  = p->p_save[2]->i_distance_index;

		}
		return ;
	}

	if(p->ph.i_picture_coding_type == XAVS_I_PICTURE)
	{
		if(p->ph.b_picture_structure == 0 && b_next_field)
		{
			p->p_save[0]->i_next_field_ref_distance[0] = p->p_save[0]->i_distance_index ;	
			p->p_save[0]->i_next_field_ref_top_field[0] = p->p_save[0]->b_top_field_first;
		}
	}
	else
	{ // P Image
		if(p->ph.b_picture_structure == 0 )
		{
			if(b_next_field)
			{
				p->p_save[0]->i_next_field_ref_distance[0] = p->p_save[0]->i_distance_index ;	
				p->p_save[0]->i_next_field_ref_top_field[0] = p->p_save[0]->b_top_field_first;

				set_field_image_ref_info(p->p_save[0], p->p_save[1] , 1, 2, b_next_field);
				set_field_image_ref_info(p->p_save[0], p->p_save[2] , 3, 1, b_next_field);
			}
			else
			{// first field
				set_field_image_ref_info(p->p_save[0], p->p_save[1] , 0, 2, b_next_field);
				set_field_image_ref_info(p->p_save[0], p->p_save[2] , 2, 2, b_next_field);
			}
		}
		else
		{
			//��ǰΪ֡���룬��ʹ�ο�Ϊ�����ο�����������Ҳ���������κ�һ�������Զ���֡��distance
			if(p->p_save[1])
			{
				p->p_save[0]->i_frame_ref_distance[0]  = p->p_save[1]->i_distance_index;	
			}
			if(p->p_save[2])
			{
				p->p_save[0]->i_frame_ref_distance[1]  = p->p_save[2]->i_distance_index;
			}
		}
	}
}
#else

//��ľ���DistatnceIndex��������Ϊ
//�������������ض��������ڸ���ɨ��ͼ��ĵڶ������߶���������ɨ��ĵ׳�����DistanceIndexΪpicture_distance * 2 + 1
//����DistanceIndexΪpicture_distance * 2 

//���ȵ�ǰ��Ϊ������飬�뵱ǰ��ָ��λ��(һ�������Ͻ���ͬ���ߴ�λ��)��Ӧ�ĺ���֡���Ӧ�Ŀ飬�Լ��ö�Ӧ���˶�ʸ��ָ��Ŀ�
//���Ƿֱ�λ�ڵ�ǰ֡�򳡣� ��Ӧ���֡�򳡣�ָ����֡���߳�
//iDistanceIndexRefΪָ���ľ�������
//iDistanceIndexColΪ��Ӧ��ľ�������
//����ο�֡�ж�Ӧ���λ��һ������������
//(x0, y0)
//(x0, y0 - 2* top_field_first + 1)
//������λ�ö�Ӧ�����������ڵı���飬���!!!!

//���ڵ�ǰͼ��Ϊ֡����
//���ں���ο�ͼ��Ϊ֡���룬�����λ�õı����϶����ڸ�֡����DistanceIndexColӦ��ʼ��Ϊż��
//�������ο�ͼ��Ϊ�������Ӧλ�õı����϶�����ĳ��������DistanceIndexCol����Ϊ����Ҳ����Ϊż��

//���ڵ�ǰͼ��Ϊ������,����b_pb_field_enhanced_flag��־Ϊ0
//�������ο���֡���룬���Ӧλ�õı����ʼ����֡�ڣ�����DistanceIndexColӦ��ʼ��Ϊż��
//�������ο��ǳ����룬���Ӧλ�õı����ʼ���ڳ��ڣ���DistanceIndexCol����Ϊ����Ҳ����Ϊż��

//���ڵ�ǰͼ��Ϊ������,b_pb_field_enhanced_flag��־Ϊ1
//�������ο�Ϊ֡���룬���Ӧλ�õı����ʼ����֡�ڣ�����DistanceIndexColӦ��ʼ��Ϊż��
//�������ο�Ϊ�����룬ʼ��ָ���һ��������DistanceIndexColӦ��ʼ��Ϊż��


//�����ǰΪ֡���룬����ָ��Ŀ�϶����ֶ����͵׳�
//�����ǰΪ�����룬��ʹ����ο��ο�Ϊ֡��Ҳ���Էֶ����͵׳�
static inline void get_col_info(xavs_decoder *p,uint8_t *p_col_type,xavs_vector **pp_mv,int block,int *i_col_distance, int *i_col_top_field)
{
	
	int i_mb_offset = 0;

	*i_col_top_field = p->p_save[1]->b_top_field_first;
	*i_col_distance = p->p_save[1]->i_distance_index;
	//��ǰ���Ϊ�������Ӧ��Ҳͬ����֡�����е�ĳһ�������Լ�ʹ�ú���ο�Ϊ֡���룬Ҳ�����³�
	/*if(p->p_save[1]->b_picture_structure)
	{
		*i_col_top_field = -1;
	}*/
	if(p->ph.b_picture_structure)
	{//��ǰ֡Ϊ֡�ṹ
		if(p->p_save[1]->b_picture_structure)
		{//����ο�ҲΪ֡
			i_mb_offset = p->i_mb_y  * p->i_mb_width + p->i_mb_x;
			*p_col_type = p->p_col_type_base[i_mb_offset];
			*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + block];
			*i_col_top_field = -1;
		}
		else
		{//����ο�Ϊ���ṹ
			//���ں�����Ͻ�y����ʼ��Ϊż�������Զ�Ӧ��ʼ��Ϊ����
			
			i_mb_offset = p->i_mb_y / 2 * p->i_mb_width + p->i_mb_x;
			if(p->p_save[1]->b_top_field_first == 0)
			{//����Ϊ�ڶ���
				 i_mb_offset += p->i_mb_num_half;
				 *i_col_distance = p->p_save[1]->i_distance_index + 1;
			}
			*i_col_top_field = 1;
			
			//��ǰ8*8���ϽǶ�Ӧ�Ŀ���Ϊ���Ĺ�ϵ��
			//�������p->i_mb_yΪż�� ���� p->i_mb_y = 16 ��ǰ��2���λ��Ϊ (x, 16 * 16  + 8) 
			//�ں���ο�֡�ж�Ӧ�Ķ���λ��Ϊ(x, 8 * 16 + 4)��������Ӧ��λ�õ�block�����ڵ�ǰ��0���Ӧ��λ�ã�
			//����p->i_mb_yΪ���� p->i_mb_y = 17 ��ǰ��2���λ��Ϊ(x,  17 * 16 + 8),
			//�ں���ο�֡�ж�Ӧ�Ķ���λ��Ϊ(x, 8 * 16 + 12) ������Ӧ��λ�õ�block�����ڵ�ǰ��2���Ӧ��λ�ã�
			*p_col_type = p->p_col_type_base[i_mb_offset];
			*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + 2 * (p->i_mb_y % 2) + (block % 2)];
		}

	}
	else
	{
		
		if(p->ph.b_pb_field_enhanced_flag)
		{

			//�е�ǰ��λ��(x0,y0)
			//����p->i_mb_index >= p->i_mb_num_half 
			//���ݶ�Ӧ��λ��(x0,  y0 - 2 * top_field_first + 1)����֪����
			//��top_field_firstΪ0�����ڵ�ǰΪ�ڶ���������Ϊ���������ж�Ӧ��(x0, y0 + 1),��Ϊ�׳� Ϊ��һ��
			//��top_field_firstΪ1�����ڵ�ǰΪ�ڶ���������Ϊ�׳������ж�Ӧ��(x0, y0 - 1),��Ϊ���� Ϊ��һ��
			//���Բ��ܵڶ���Ϊ�׳����Ƕ��� ��Ӧ��ʼ���ں���ο�֡�ĵ�һ����������ο���0�ο���
			//Ϊ���ο�

			//������p->i_mb_index < p->i_mb_num_half ���Ӧ��λ��ҲΪ(x0, y0)����ʼ��Ϊ��һ����������һ��
			//���ﲻ���ǵ�ǰͼ���b_top_field_first�����ο�ͼ���b_top_field_first��ͬ,����ʵ����

			if(p->i_mb_index >= p->i_mb_num_half )
			{//��ǰΪ�ڶ���
				
				if(p->p_save[1]->b_picture_structure)
				{//����ο�Ϊ֡,

					//i_mb_offset = (p->i_mb_y - p->i_mb_height / 2) * 2 * p->i_mb_width + p->i_mb_x  ;
					i_mb_offset = p->i_mb_y * 2 * p->i_mb_width - p->i_mb_num + p->i_mb_x  ;
					if(block > 1)
					{//�ڶ��еĿ飬Ҫ����һ�к��,������ b_top_field_firstΪ 0��
						//��ǰ�����Ͻ�Ϊy = 32����ʱp->i_mb_y = p->i_mb_height / 2 + 1,��Ӧ��Ϊy = 33,����֡������Ϊp->i_mb_y = 2,
						//���ڶ��п�Ϊy = 48;��ʱ��Ȼp->i_mb_y = p->i_mb_height / 2 + 1,��Ӧ��Ϊy = 49,����֡������Ϊp->i_mb_y = 3,
						//���Ժ��ƫ��Ӧ�ü��� p->i_mb_width
						i_mb_offset += p->i_mb_width;
					}
					//ʼ�ղο���һ�����������p->p_save[1]->b_top_field_firstΪtrue,��һ��ҲΪ����
					*i_col_top_field = p->p_save[1]->b_top_field_first;
					*p_col_type = p->p_col_type_base[i_mb_offset];
					//(block % 2)��ʾʼ��Ϊ�ο�֡��Ӧ���ĵ�һ�п�
					*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + (block % 2)];
					
				}
				else
				{//����ο�Ϊ�� 
					//�������������
					i_mb_offset = (p->i_mb_y  - p->i_mb_height / 2 ) * p->i_mb_width + p->i_mb_x;
					*p_col_type = p->p_col_type_base[i_mb_offset];	
					*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + block];
					*i_col_top_field = p->p_save[1]->b_top_field_first;
				}
			}
			else
			{//��һ�� ��Ӧλ�õ�ҲӦ���ǵ�һ�� 
				if(p->p_save[1]->b_picture_structure)
				{//����ο�Ϊ֡
					i_mb_offset = p->i_mb_y * 2 * p->i_mb_width  + p->i_mb_x;
					if(block > 1)
					{
						i_mb_offset += p->i_mb_width;
					}
					*p_col_type = p->p_col_type_base[i_mb_offset];
					*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + (block % 2)];
					*i_col_top_field = p->p_save[1]->b_top_field_first;
				}
				else
				{//����ο�Ϊ��
				
					i_mb_offset = p->i_mb_y  * p->i_mb_width + p->i_mb_x;
					*p_col_type = p->p_col_type_base[i_mb_offset];
					*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + block ];
					*i_col_top_field = p->p_save[1]->b_top_field_first;
				}	
			}

		}
		else
		{//!p->ph.b_pb_field_enhanced_flag
			//�е�ǰ��λ��(x0,y0) ���Ӧ��λ��ҲΪ(x0, y0)��
			//�������ͼ��Ϊ֡
			if(p->p_save[1]->b_picture_structure)
			{
				i_mb_offset = p->i_mb_y * 2 * p->i_mb_width + p->i_mb_x;
				if(block > 1)
				{
					i_mb_offset += p->i_mb_width;
				}
				if(i_mb_offset >= (int)p->i_mb_num)
				{
					i_mb_offset -= p->i_mb_num;
				}
				
				*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + (block % 2)];

				*p_col_type = p->p_col_type_base[i_mb_offset];
				*i_col_top_field = p->p_save[1]->b_top_field_first;
				
				
			}
			else
			{//����ο�Ϊ����
			//���ﲻ���ǵ�ǰ֡��b_top_field_first�����ο�֡��b_top_field_first��ͬ,����ʵ����
				i_mb_offset = p->i_mb_index;
				*p_col_type = p->p_col_type_base[i_mb_offset];
				*pp_mv      = &p->p_col_mv[i_mb_offset * 4 + block];

				*i_col_top_field = p->ph.b_top_field_first;
				
				if(p->i_mb_index >= p->i_mb_num_half)	
				{//�ڶ���
					*i_col_distance = p->p_save[1]->i_distance_index + 1;
				}
			}
			

		}
	}
	

}




static inline void get_b_direct_skip_sub_mb(xavs_decoder *p,int block,xavs_vector *p_mv,int col_distance,int i_col_top_field)
{
	xavs_vector temp_mv = p_mv[0];
	int delta1 = 0, deltaFw = 0, deltaBw = 0;
	uint32_t b_next_field = p->i_mb_index >= p->i_mb_num_half;
	int iDistanceIndexFw,iDistanceIndexBw,iDistanceIndexRef,iDistatnceIndexCol;
	int iDistanceIndexFw0,iDistanceIndexFw1,iDistanceIndexBw0,iDistanceIndexBw1;
	int i_ref_top_field;//mvRefָ��Ŀ��Ƿ�Ϊ����
	if(i_col_top_field ^ p->p_save[1]->b_top_field_first)
	{
		i_ref_top_field = p->p_save[1]->i_next_field_ref_top_field[temp_mv.ref];
	}
	else
	{
		i_ref_top_field = p->p_save[1]->i_first_field_ref_top_field[temp_mv.ref];
	}
	xavs_vector *p_fw_mv = &p->mv[mv_scan[block]];
	xavs_vector *p_bw_mv = &p->mv[mv_scan[block] + MV_BWD_OFFS];

	iDistatnceIndexCol = col_distance;
	if(p->p_save[1]->b_picture_structure)
	{//�ο�ָ֡��Ŀ�Ӧ��Ҳ��֡�飬��ͬʱ�����κ�һ��������Ӧ����ż����
		//�����ʱtemp_mv.refӦ��ֻΪ0��1
		iDistanceIndexRef = p->p_save[1]->i_frame_ref_distance[temp_mv.ref];
	}
	else
	{
		if(i_col_top_field ^ p->p_save[1]->b_top_field_first)
		{
			
			iDistanceIndexRef = p->p_save[1]->i_next_field_ref_distance[temp_mv.ref];
		}
		else
		{
			iDistanceIndexRef = p->p_save[1]->i_first_field_ref_distance[temp_mv.ref];
		}
	}
	
	if(p->ph.b_picture_structure)
	{
		//�ο�ͼ��Ϊȱʡ�ο�����ǰ��0������Ϊ0����DistatnceIndexӦ����ż������Ϊ��һ������֡
		iDistanceIndexFw = p->p_save[2]->i_distance_index;
		iDistanceIndexBw = p->p_save[1]->i_distance_index;
		if(p->p_save[1]->b_picture_structure == 0)
		{
		   temp_mv.y *= 2;
		}
		p_fw_mv->ref = 1;
		p_bw_mv->ref = 0;
		
	}	
	else 
	{
		if(p->p_save[1]->b_picture_structure == 1)
		{	
			temp_mv.y /= 2;
		}
		iDistanceIndexFw0 = p->p_save[2]->i_distance_index + 1;
		iDistanceIndexFw1 = p->p_save[2]->i_distance_index;
		iDistanceIndexBw0 = p->p_save[1]->i_distance_index;
		iDistanceIndexBw1 = p->p_save[1]->i_distance_index + 1;
		
		
		if(iDistanceIndexRef == iDistanceIndexFw0)
		{
			iDistanceIndexFw = iDistanceIndexFw0;
			p_fw_mv->ref = 2;
		}
		else
		{
			iDistanceIndexFw = iDistanceIndexFw1;
			p_fw_mv->ref = 3;
		}


		if(b_next_field == 0)
		{
			iDistanceIndexBw = iDistanceIndexBw0;
			p_bw_mv->ref = 0;
		}
		else
		{
			iDistanceIndexBw = iDistanceIndexBw1;
			p_bw_mv->ref = 1;
		}
			
		
		if(p->ph.b_pb_field_enhanced_flag && b_next_field)
		{
			iDistanceIndexFw = iDistanceIndexFw0;
			p_fw_mv->ref = 2;
		}
		
		if(p->ph.b_pb_field_enhanced_flag)
		{
			iDistanceIndexBw = iDistanceIndexBw0;
			p_bw_mv->ref = 0;
		}

		//iDistatnceIndexCol = iDistanceIndexBw;
		
	}
	
	

	
	if(p->ph.b_pb_field_enhanced_flag && p->ph.b_picture_structure == 0)
	{
		int bw_top_field,fw_top_field, cur_top_field;
		cur_top_field = b_next_field ^ p->ph.b_top_field_first;
		//p_bw_mv->refΪ0��ʱ���ǵ�һ����Ϊ1��ʱ���ǵڶ���
		//p_bw_mv->refȡֵӦ��Ϊ1����0
		bw_top_field = (p_bw_mv->ref ^ p->p_save[1]->b_top_field_first);
		if(!p->p_save[1]->b_picture_structure)
		{//�������ο�Ϊ֡������mvRefָ��Ŀ϶��Ǹ�֡�������ж����͵׳�֮��

			if(bw_top_field && i_ref_top_field== 0) 
			{
				delta1 = 2;
			}
			else if(!bw_top_field && i_ref_top_field == 1) 
			{
				delta1 = -2;
			}
		}
		if(cur_top_field && bw_top_field  == 0 )
		{
			deltaBw = 2;
		}
		else if(cur_top_field == 0 && bw_top_field )
		{
			deltaBw = -2;
		}


		//p_bw_mv->refΪ2��ʱ���ǵڶ�����Ϊ3��ʱ���ǵ�һ��
		//p_bw_mv->refȡֵӦ��Ϊ2����3
		fw_top_field = !((p_fw_mv->ref - 2)^p->p_save[2]->b_top_field_first);
		if(cur_top_field && fw_top_field  == 0)
		{
			deltaFw = 2;
		}
		else if(cur_top_field == 0 && fw_top_field)
		{
			deltaFw = -2;
		}

			
		
	}
	
	mv_pred_direct(p,&p->mv[mv_scan[block]],&temp_mv,
			iDistatnceIndexCol, iDistanceIndexRef,iDistanceIndexFw,iDistanceIndexBw,
			delta1, deltaFw, deltaBw);
	
}

#endif
