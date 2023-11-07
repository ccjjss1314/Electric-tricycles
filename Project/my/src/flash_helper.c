#define FLASH_HELPER_C_

#include "flash_helper.h"
#include "stmflash.h"


flash_helper_typedef flash_helper_struct;

/**Flash��������,�����Լ��ĺ����޸ĺ�����**/

//ָ����ַ��ȡһ��������u32������
void FlashHelperReadDataEx(u32 addr,u32 *data, u32 datalen)
{
	STMFLASH_Read(addr,data,datalen);
}
//ָ����ַд��һ��������u32������
int FlashHelperWriteWordsEx(u32 addr,u32 *pBuffer, u32 NumToWrite)
{
	 return FlashWriteWords(addr,pBuffer,NumToWrite);
}
//����ĳһ������
char FlashHelperEraseSectorEx(u32 sector_start_addr,u32 sector_stop_addr)
{
	return FlashEraseSector(sector_start_addr,sector_stop_addr);
}


/**
* @brief  flash_helper_init
* @param  sectors_addr  ʹ����һ��������������
* @param  None
* @param  None
* @retval 0:�ɹ�
* @example 
* @waing   
**/
int flash_helper_init(u32 sectors_addr)
{
	u32 i=0;
	
	flash_helper_struct.FlashHelperDataSize = FlashHelperDataLen*4;          //������Ա�����ֽ���
	/*�����Ŀ�ʼ��ַ*/
	flash_helper_struct.FlashHelperStartAddr=0;
	/*�����Ľ�����ַ*/
	flash_helper_struct.FlashHelperEndAddr=0;
	/*��¼��ǰ�����ĵ�ַ*/	
	flash_helper_struct.FlashHelperSaveAddr=0;
	
		//��������鳬���˴�����,2KB
		if(FlashHelperDataLen>2*1024/4) {return 1;}
		//����������ַ
		flash_helper_struct.FlashHelperEndAddr = sectors_addr+2*1024;
		
		#if FlashHelperDebug
		printf("\r\nsectors_addr_start: 0x%x\r\n",sectors_addr);
		printf("\r\nsectors_addr_stop: 0x%x\r\n",flash_helper_struct.FlashHelperEndAddr);
		#endif
	
  if(sectors_addr < 0x08000000 || sectors_addr > 0x0800FFFF)
	{
		#if FlashHelperDebug
		printf("\r\nsectors_addr_err=4\r\n");
		#endif
		return 4;//����ʹ�õ�ĳ���������׵�ַ
	}
	
	//������ʼ��ַ
	flash_helper_struct.FlashHelperStartAddr = sectors_addr;
	
	
	//�ҵ���ǰ���浽���ĸ�λ��
	for(i=flash_helper_struct.FlashHelperStartAddr;i<=flash_helper_struct.FlashHelperEndAddr-flash_helper_struct.FlashHelperDataSize;i+=flash_helper_struct.FlashHelperDataSize)
	{
		//��ȡ���������
		FlashHelperReadDataEx(i,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
		
		//�б�ʶ
//		if(flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] == FlashHelperUUID)
	  if(flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] != 0xffffffff )
		{
			//��ǰ���ݱ���ĵ�ַ
			flash_helper_struct.FlashHelperSaveAddr=i;
			
			#if FlashHelperDebug
			printf("\r\ndata:0x55555555 addr: 0x0%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
			#endif
		}
		else if(flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] == 0xffffffff)
		{
			//û�л�������
			if(i == flash_helper_struct.FlashHelperStartAddr)
			{
				//��ǰ���ݱ���ĵ�ַ
				flash_helper_struct.FlashHelperSaveAddr =  flash_helper_struct.FlashHelperStartAddr;
//				flash_helper_struct.FlashHelperSaveAddr=i;
				//���ý�β��ʶ��       //��һ����д0
//				 flash_data_set_count(111);
				memset(flash_helper_struct.FlashHelperData,0,sizeof(flash_helper_struct.FlashHelperData));
//				flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] = FlashHelperUUID;
				FlashHelperWriteWordsEx(flash_helper_struct.FlashHelperSaveAddr,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
			}
			else
			{
				//������һ�δ洢�����ݵ�����
				FlashHelperReadDataEx(i-flash_helper_struct.FlashHelperDataSize,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
			}
			
			#if FlashHelperDebug
			printf("\r\ndata:0xFFFFFFFF addr: 0x0%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
			#endif
			break;
		}
		else
		{
			//�����������
			FlashHelperEraseSectorEx(sectors_addr,sectors_addr);
			//��ǰ���ݱ���ĵ�ַ
			flash_helper_struct.FlashHelperSaveAddr =  flash_helper_struct.FlashHelperStartAddr;
			
			#if FlashHelperDebug
			printf("\r\ndata:err addr: 0x0%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
			#endif
			break;
		}
	}
	return 0;
}


/**
* @brief  ���������������ֵ,���ô˺�������������д��flash
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
* @waing  
**/
void flash_helper_update(void)
{
	//��ǰ�������ݵ��׵�ַ + �����ݵĸ��� = ��α������ݵ��׵�ַ
		flash_helper_struct.FlashHelperSaveAddr = flash_helper_struct.FlashHelperSaveAddr + flash_helper_struct.FlashHelperDataSize;
	
	#if FlashHelperDebug
	printf("SaveAddr:%x\r\n",flash_helper_struct.FlashHelperSaveAddr);
	#endif
	
	//��α������ݵ�β��ַ�����˸�����β��ַ
	//������Χ,�����������,���¿�ʼ�洢
	if(flash_helper_struct.FlashHelperSaveAddr + flash_helper_struct.FlashHelperDataSize -1 > flash_helper_struct.FlashHelperEndAddr)
	{
		#if FlashHelperDebug
		printf("EraseSector\r\n");
		#endif
		//�����������
		FlashHelperEraseSectorEx(flash_helper_struct.FlashHelperStartAddr,flash_helper_struct.FlashHelperStartAddr);
		//���õ�ǰ����ĵ�ַΪ�����׵�ַ
		flash_helper_struct.FlashHelperSaveAddr =  flash_helper_struct.FlashHelperStartAddr;
	}
	//�ٴ�����һ�½�β��ʶ��,��ֹ�û�������������.
//	flash_helper_struct.FlashHelperData[FlashHelperDataLen-1] = FlashHelperUUID;
	FlashHelperWriteWordsEx(flash_helper_struct.FlashHelperSaveAddr,flash_helper_struct.FlashHelperData,FlashHelperDataLen);
}


/**
* @brief   ��ȡ���������
* @param   index ��������±��Ӧ
* @param   data  ���ص�����
* @param   datalen ��ȡ�ĸ���
* @retval  0:�ɹ�
* @warning 
* @example 
**/
int flash_helper_get(u32 index,u32 *data, u32 datalen)
{
	//����ȡ�ĸ������ܳ���ʵ������ܷ��صĸ���
	if(datalen > FlashHelperDataLen - index) datalen = FlashHelperDataLen - index;
	FlashHelperReadDataEx(flash_helper_struct.FlashHelperSaveAddr+(index<<2),data,datalen);
	return 0;
}



/*******************************************************flash_data*****************************************************/
//���ü���ֵ
void flash_data_set_count(u32 data)
{
	flash_helper_struct.FlashHelperData[0] = data;
}

//��ȡ���õļ���ֵ
u32 flash_data_get_count(char mode)
{
	u32 value;
	if(mode){//��falsh�����ȡ
		flash_helper_get(0,&value,1);
	}
	else{
		value = flash_helper_struct.FlashHelperData[0];
	}
	return value;
}




