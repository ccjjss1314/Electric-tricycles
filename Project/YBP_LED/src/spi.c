#include "spi.h"

#define ONE_WIRE_ENABLE 1            //��������һ��ͨ����

//���ϱ�־λ
uint8_t Error_Reverse_flag,
        Error_StopCar_flag =0;
								
uint8_t OdoTrip_unit = 0;
uint8_t Speed_LED[5]={0};

//��ʾLED��Ż���
uint8_t All_LED_num[256] = {0};

uint8_t showing_arr_Aip1[16]={0};
uint8_t showing_arr_Aip2[16]={0};



/***************************************�ٶ���ʾ**************************************/
 const uint8_t Speed_num1[98] = {
	33,34,35,37,38,39,49,50,51,53,54,55,                 // 0-11:0
  35,38,51,54,                                         // 12-15:1 
  33,35,36,37,39,49,51,52,53,55,                       // 16-25:2
	33,35,36,38,39,49,51,52,54,55,                       // 26-35:3
	34,35,36,38,50,51,52,54,                             // 36-43:4
	33,34,36,38,39,49,50,52,54,55,                       // 44-53:5
	33,34,36,37,38,39,49,50,52,53,54,55,                  // 54-65:6
	33,35,38,49,51,54,                                    // 66-71:7
	33,34,35,36,37,38,39,49,50,51,52,53,54,55,            // 72-85:8
	33,34,35,36,38,39,49,50,51,52,54,55,                  // 86-97:9
};

 const uint8_t Speed_num2[102] = {
	65,66,67,69,70,71,81,82,83,85,86,87,         // 0-11:0
  67,70,83,86,                                 // 12-15:1 
  65,67,68,69,71,81,83,84,85,87,               // 16-25:2
	65,67,68,70,71,81,83,84,86,87,               // 26-35:3
	66,67,68,70,82,83,84,86,                     // 36-43:4
	65,66,68,70,71,81,82,84,86,87,               // 44-53:5
	65,66,68,69,70,71,81,82,84,85,86,87,         // 54-65:6
	65,67,70,81,83,86,                           // 66-71:7
	65,66,67,68,69,70,71,81,82,83,84,85,86,87,   // 72-85:8
	65,66,67,68,70,71,81,82,83,84,86,87,         // 86-97:9
	103,104,101,102                              // 98-99:km/h   100-101:mph
};

/*************************************�����************************************/
 const uint8_t kilometer_num[256] = {
	 // 0-5:0 ,         6-7:1      8-12:2,         13-17:3         18-21:4      22-26:5         27-32:6            33-35:7    36-42:8               43-48:9                               ����1
	 41,42,43,45,46,47, 43,46,     41,43,44,45,47, 41,43,44,46,47, 42,43,44,46, 41,42,44,46,47, 41,42,44,45,46,47, 41,43,46,  41,42,43,44,45,46,47, 41,42,43,44,46,47,	 
	 // 49-54:0 ,       55-56:1    57-61:2,        62-66:3         67-70:4      71-75:5         76-81:6            82-84:7    85-91:8               92-97:9                               ����2
	 57,58,59,61,62,63, 59,62,     57,59,60,61,63, 57,59,60,62,63, 58,59,60,62, 57,58,60,62,63, 57,58,60,61,62,63, 57,59,62,  57,58,59,60,61,62,63, 57,58,59,60,62,63,
	 // 98-103:0 ,      104-105:1  106-110:2,      111-115:3       116-119:4    120-124:5       125-130:6          131-133:7  134-140:8             141-146:9                             ����3
	 73,74,75,77,78,79, 75,78,     73,75,76,77,79, 73,75,76,78,79, 74,75,76,78, 73,74,76,78,79, 73,74,76,77,78,79, 73,75,78,  73,74,75,76,77,78,79, 73,74,75,76,78,79,
	 // 147-152:0 ,     153-154:1  155-159:2,      160-164:3       165-168:4    169-173:5       174-179:6          180-182:7  183-189:8             190-195:9                             ����4
	 89,90,91,93,94,95, 91,94,     89,91,92,93,95, 89,91,92,94,95, 90,91,92,94, 89,90,92,94,95, 89,90,92,93,94,95, 89,91,94,  89,90,91,92,93,94,95, 89,90,91,92,94,95,
	 // 196-201:0 ,           202-203:1  204-208:2,           209-213:3            214-217:4        218-222:5            223-228:6                229-231:7    232-238:8                    239-244:9                  ����5
	 105,106,107,109,110,111, 107,110,   105,107,108,109,111, 105,107,108,110,111, 106,107,108,110, 105,106,108,110,111, 105,106,108,109,110,111, 105,107,110, 105,106,107,108,109,110,111, 105,106,107,108,110,111,
	 // 245-249:V,        250-253:ODOTRIP   254:km     255:mile            
	 106,107,109,110,111, 25,26,27,28,      88,        72
};

/*************************************���************************************/
const uint8_t battery[21]={
	// 0-1:���ͼ��   2-15:��ؿ��                                      16:1�����  17:2�� 18:3��  19:4��  20:5��
	30,31,            29,32,48,64,80,96,112,113,115,118,120,122,125,127, 126,        124,   121,    116,    119
};

#if ONE_WIRE_ENABLE==1
/*************************************һ��ͨ����************************************/
const uint8_t fault[8]={
	//0:ECU���� 1:������� 2:ɲ������ 3:ת�ѹ���   4-5:�������� 6-7:ͣ������
	128,        123,       114,       117,         97,98,       99,100
};
#endif

/*************************************ָʾ��************************************/
const uint8_t light[61]={
	// 0-5:0 ,         6-7:1      8-12:2,         13-17:3         18-21:4      22-26:5                                        
	17,18,19,21,22,23, 19,22,     17,19,20,21,23, 17,19,20,22,23, 18,19,20,22, 17,18,20,22,23, 
//27-32:6            33-35:7    36-42:8               43-48:9                                 ��λ����
	17,18,20,21,22,23, 17,19,22,  17,18,19,20,21,22,23, 17,18,19,20,22,23,
	//49-50:��ת 51-52:��ת  53-54:Զ��� 55-56:READY 57-58:ʾ��� 59-60:���
    133,134,   181,182,    135,136,     145,146,    147,148,     149,150 	
};

/*************************************������Ϣ************************************/ 
const uint8_t guide_num[202]={
 // 0-5:0 ,               6-7:1      8-12:2,              13-17:3              18-21:4          22-26:5              27-32:6                  33-35:7      36-42:8                      43-48:9                    ����1
 137,138,139,140,142,143, 139,142,   137,139,140,141,143, 137,139,140,142,143, 138,139,140,142, 137,138,140,142,143, 137,138,140,141,142,143, 137,139,142, 137,138,139,140,141,142,143, 137,138,139,140,142,143,	                                                                                                                            
 // 49-54:0 ,             55-56:1    57-61:2,             62-66:3              67-70:4          71-75:5              76-81:6                  82-84:7      85-91:8                      92-97:9                    ����2
 153,154,155,157,158,159, 155,158,   153,155,156,157,159, 153,155,156,158,159, 154,155,156,158, 153,154,156,158,159, 153,154,156,157,158,159, 153,155,158, 153,154,155,156,157,158,159, 153,154,155,156,158,159,
 // 98-103:0 ,            104-105:1  106-110:2,           111-115:3            116-119:4        120-124:5            125-130:6                131-133:7    134-140:8                    141-146:9                  ����3
 169,170,171,173,174,175, 171,174,   169,171,172,173,175, 169,171,172,174,175, 170,171,172,174, 169,170,172,174,175, 169,170,172,173,174,175, 169,171,174, 169,170,171,172,173,174,175, 169,170,171,172,174,175,
	//147:С����  148:km  149:m  
	  160,        144,    176,
	//150-151:���� 152-153:���� 154-155:�绰 156-157:��� 158-159:�շ�վ
	  151,152,     161,162,     163,164,     165,166,     167,168,
	//160-163:�������ö�
	  215,216,245,246,
	//164-169:ֱ��             170-175:��ǰ             176-181:��ǰ             182-185:��ת     186-189:��ת     190-195:���             196-201:�Һ�
	  209,210,211,212,213,214, 193,194,195,196,212,214, 213,214,225,226,227,228, 197,198,199,200, 229,230,231,232, 199,200,241,242,243,244, 192,201,229,230,247,248
};

/*************************************��ˮ����ʾ************************************/
const uint8_t prompt[]={
	//0-9:����ɫ                   10-19:����ɫ                             20-29:���ɫ                30-39:�Һ�ɫ
    16,14,12,10,8,6,4,2,129,131, 208,206,204,202,190,188,186,184,180,178, 15,13,11,9,7,5,3,1,130,132, 207,205,203,191,189,187,185,183,179,177
};

void SPI2_Init(void)
{
	GPIO_InitType GPIO_InitStructure;
	SPI_InitType SPI_InitStructure;
	
  //��������SPIʱû����AFIOʱ��
	RCC_APB2_Peripheral_Clock_Enable(RCC_APB2_PERIPH_SPI2);   //����SPI2����ʱ��
	
	GPIO_Structure_Initialize(&GPIO_InitStructure);
  GPIO_InitStructure.Pin        = SPI_SLAVE_CLK_PIN | SPI_SLAVE_MOSI_PIN|SPI_SLAVE_NSS_PIN;
  GPIO_InitStructure.GPIO_Mode  = GPIO_MODE_AF_PP;
  GPIO_InitStructure.GPIO_Alternate = SPI_SLAVE_GPIO_ALTERNATE;
  GPIO_Peripheral_Initialize(SPI_SLAVE_GPIO, &GPIO_InitStructure);
	
	SPI_I2S_Reset(SPI_SLAVE); 																				  //��λSPI2�Ĵ���ֵ
	SPI_Initializes_Structure(&SPI_InitStructure);                      //��λSPI1�Ĵ���������Ĭ��ֵ���ÿһ����Ա
	SPI_InitStructure.DataDirection = SPI_DIR_SINGLELINE_TX;            //SPI��������ģʽ��ֻ����
	SPI_InitStructure.SpiMode       = SPI_MODE_MASTER;                  //SPIģʽΪ����ģʽ
	SPI_InitStructure.DataLen       = SPI_DATA_SIZE_8BITS;             //16λ���ݷ���
	SPI_InitStructure.CLKPOL        = SPI_CLKPOL_LOW;                   //SPI_CPOL_High=ģʽ3��ʱ�ӿ���Ϊ�� SPI_CPOL_Low=ģʽ0��ʱ�ӿ���Ϊ��
	SPI_InitStructure.CLKPHA        = SPI_CLKPHA_FIRST_EDGE;            //��һ�������ز������������ز���
	SPI_InitStructure.NSS           = SPI_NSS_SOFT;                     //ʹ��Ӳ�������Զ�Ƭѡ��ͬSPI����������1
	/* It is recommended that the SPI master mode of the C version chips should not exceed 18MHz */
	SPI_InitStructure.BaudRatePres  = SPI_BR_PRESCALER_8;               //64/8 = 8MHz
	SPI_InitStructure.FirstBit      = SPI_FB_LSB;                       //��MSB��λ��ʼ����
	SPI_InitStructure.CRCPoly       = 7;
	SPI_Initializes(SPI_SLAVE, &SPI_InitStructure);
	
	SPI_Set_Nss_Level(SPI_SLAVE,SPI_NSS_HIGH);
//  SPI_I2S_DMA_Transfer_Enable(SPI_SLAVE, SPI_I2S_DMA_TX);
	SPI_CRC_Enable(SPI_SLAVE);
	SPI_ON(SPI_SLAVE);
	
	GPIO_InitStructure.Pin        =  SPI_SLAVE_NSS_PIN;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_MODE_OUT_PP;
	GPIO_InitStructure.GPIO_Slew_Rate = GPIO_SLEW_RATE_FAST;
  GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
	GPIO_Pins_Set(GPIOB, SPI_SLAVE_NSS_PIN);
}
	
/*******************************************************************************
**�������ƣ�void Send_AddrInc(uint8_t display_cmd,uint8_t data[16],uint8_t nss)
**����������������������ģʽ
**��ڲ�����
**�������
*******************************************************************************/
void Send_AddrInc(uint8_t display_cmd,uint8_t data[16],uint8_t nss)
{
	uint8_t i= 0;
	if(nss==1)
	GPIO_Pins_Reset(GPIOA, GPIO_PIN_8);             //����STB1����
	else if(nss==2)
		GPIO_Pins_Reset(GPIOB, SPI_SLAVE_NSS_PIN);
	
	SPI_I2S_Data_Transmit(SPI2,Data_Cmd_AddrInc);   //��������ģʽ
	__nop();
	while(SPI_I2S_Flag_Status_Get(SPI2,SPI_I2S_FLAG_BUSY)==SET){};
	if(nss==1)
	{
	 GPIO_Pins_Set(GPIOA, GPIO_PIN_8);
	 GPIO_Pins_Reset(GPIOA, GPIO_PIN_8);
	}
  else if(nss==2)
	{
	 GPIO_Pins_Set(GPIOB, SPI_SLAVE_NSS_PIN);
	 GPIO_Pins_Reset(GPIOB, SPI_SLAVE_NSS_PIN);
	}
	SPI_I2S_Data_Transmit(SPI2,Addr_Cmd_00H);       //������ʼ��ַ
	__nop();
	while(SPI_I2S_Flag_Status_Get(SPI2,SPI_I2S_FLAG_BUSY)==SET){};
	for(i=0;i<16;i++)
	{
		SPI_I2S_Data_Transmit(SPI2,data[i]); 
		__nop();
	  while(SPI_I2S_Flag_Status_Get(SPI2,SPI_I2S_FLAG_BUSY)==SET){};
	}
	
	if(nss==1)
	{
	 GPIO_Pins_Set(GPIOA, GPIO_PIN_8);
	 GPIO_Pins_Reset(GPIOA, GPIO_PIN_8);
	}
  else if(nss==2)
	{
	 GPIO_Pins_Set(GPIOB, SPI_SLAVE_NSS_PIN);
	 GPIO_Pins_Reset(GPIOB, SPI_SLAVE_NSS_PIN);
	}
	
	SPI_I2S_Data_Transmit(SPI2,display_cmd);        //������ʾ����
	__nop();
	while(SPI_I2S_Flag_Status_Get(SPI2,SPI_I2S_FLAG_BUSY)==SET){};
	if(nss==1)
	GPIO_Pins_Set(GPIOA, GPIO_PIN_8);
	else if(nss==2)
		GPIO_Pins_Set(GPIOB, SPI_SLAVE_NSS_PIN);
}


/*******************************************************************************
**�������ƣ�void One_LED_num_Switch(uint16_t led_num)
**������������Ҫ������LED_num�����ӦAll_LED_num����λ
**��ڲ�����
**�������
*******************************************************************************/
void One_LED_num_Switch(uint16_t led_num)
{
	uint8_t index;
  
	if(led_num <= 128)                                         //ΪAip1���Ƶ�LED
	{
		index = led_num/8;
		if((led_num%8)==0)
		{
			index-=1;
			showing_arr_Aip1[index] |= 1 << 7;
		}
		else
			showing_arr_Aip1[index] |= 1 << (led_num%8-1);
	}
	else if((led_num > 128) && (led_num <= 256))               //ΪAip2���Ƶ�LED
	{
		led_num-=128;
		index = led_num/8;
		if((led_num%8)==0)
		{
			index-=1;
			showing_arr_Aip2[index] |= 1 << 7;
		}
		else
			showing_arr_Aip2[index] |= 1 << (led_num%8-1);
	}
}

/*******************************************************************************
**�������ƣ�void All_LED_Switch(uint8_t arr[])
**������������Ҫ�������������LED���ȫ��ת����All_LED_num
**��ڲ�����
**�������
*******************************************************************************/
void All_LED_Switch(uint8_t arr[])
{
	uint16_t i,index;
	
	for(index=0;index<256;index++)
	{
		if(All_LED_num[index] == 0 )
			break;
	}
	
	for(i=0;i<index;i++)
		One_LED_num_Switch(arr[i]);
}


/*******************************************************************************
**�������ƣ�uint16_t Current_location(void)
**��������������All_LED_num���鵱ǰԪ�ظ��������������±�
**��ڲ�����
**�������
*******************************************************************************/
uint16_t Current_location(void)
{
	uint16_t i;
	for(i=0;i<256;i++)
	{
		if(!All_LED_num[i])
			break;
	}
	return i;
}

/*******************************************************************************
**�������ƣ�void Clean_All_LED_num(void)
**������������All_LED_num������0
**��ڲ�����
**�������
*******************************************************************************/
void Clean_All_LED_num(void)
{
	uint16_t i;
		for(i=0;i<256;i++)
		  All_LED_num[i] = 0;
}


/*******************************************************************************
**�������ƣ�void Clean_show_LED_num(void)
**������������All_LED_num������0
**��ڲ�����
**�������
*******************************************************************************/
void Clean_show_LED_num(void)
{
	uint8_t i;
	for(i=0;i<16;i++)
	{
		showing_arr_Aip1[i] = 0;
		showing_arr_Aip2[i] = 0;
	}
}


/*******************************************************************************
**�������ƣ�void Transfer_Speed_to_All_LED_num(uint8_t speed,uint8_t speed_unit)
**�������������ٶȴ�����ʾ����
**��ڲ�����
**�������
*******************************************************************************/
void Transfer_Speed_to_All_LED_num(uint8_t speed,uint8_t speed_unit)
{
	uint8_t num1,num2;
	uint16_t location;
	uint8_t i;
	
	num1 = speed/10;
	num2 = speed%10;
	location = Current_location();     //���ҵ�ǰλ��
	
	switch(num1)
	{
		case 0:
			for(i=0;i<12;i++)
			All_LED_num[location++] = Speed_num1[i];break;
		case 1:
			for(i=0;i<4;i++)
			All_LED_num[location++] = Speed_num1[i+12];break;
		case 2:
			for(i=0;i<10;i++)
			All_LED_num[location++] = Speed_num1[i+16];break;
		case 3:
			for(i=0;i<10;i++)
			All_LED_num[location++] = Speed_num1[i+26];break;
		case 4:
			for(i=0;i<8;i++)
			All_LED_num[location++] = Speed_num1[i+36];break;
		case 5:
			for(i=0;i<10;i++)
			All_LED_num[location++] = Speed_num1[i+44];break;
		case 6:
			for(i=0;i<12;i++)
			All_LED_num[location++] = Speed_num1[i+54];break;
		case 7:
			for(i=0;i<6;i++)
			All_LED_num[location++] = Speed_num1[i+66];break;
		case 8:
			for(i=0;i<14;i++)
			All_LED_num[location++] = Speed_num1[i+72];break;
		case 9:
			for(i=0;i<12;i++)
			All_LED_num[location++] = Speed_num1[i+86];break;
	}
	switch(num2)
	{
		case 0:
			for(i=0;i<12;i++)
			All_LED_num[location++] = Speed_num2[i];break;
		case 1:
			for(i=0;i<4;i++)
			All_LED_num[location++] = Speed_num2[i+12];break;
		case 2:
			for(i=0;i<10;i++)
			All_LED_num[location++] = Speed_num2[i+16];break;
		case 3:
			for(i=0;i<10;i++)
			All_LED_num[location++] = Speed_num2[i+26];break;
		case 4:
			for(i=0;i<8;i++)
			All_LED_num[location++] = Speed_num2[i+36];break;
		case 5:
			for(i=0;i<10;i++)
			All_LED_num[location++] = Speed_num2[i+44];break;
		case 6:
			for(i=0;i<12;i++)
			All_LED_num[location++] = Speed_num2[i+54];break;
		case 7:
			for(i=0;i<6;i++)
			All_LED_num[location++] = Speed_num2[i+66];break;
		case 8:
			for(i=0;i<14;i++)
			All_LED_num[location++] = Speed_num2[i+72];break;
		case 9:
			for(i=0;i<12;i++)
			All_LED_num[location++] = Speed_num2[i+86];break;
	}
	if(speed_unit==0)     //ȫ��
	{
		All_LED_num[location++] = Speed_num2[98];
		All_LED_num[location++] = Speed_num2[99];
		All_LED_num[location++] = Speed_num2[100];
		All_LED_num[location++] = Speed_num2[101];
	}
	else if(speed_unit==1) //km/h
	{
		All_LED_num[location++] = Speed_num2[98];
		All_LED_num[location++] = Speed_num2[99];
	}
	else if(speed_unit==2) //mph
	{
		All_LED_num[location++] = Speed_num2[100];
		All_LED_num[location++] = Speed_num2[101];
	}
}

/*******************************************************************************
**�������ƣ�void Transfer_Trip_to_All_LED_num(uint32_t kilometer,uint8_t unit,uint8_t trip_swit)
**��������������ʻ������������ʾ����
**��ڲ�����trip_swit: 1:С�� 2:��ѹ��ʾ 0:�����
**�������
*******************************************************************************/
void Transfer_Trip_to_All_LED_num(uint32_t kilometer,uint8_t unit,uint8_t trip_swit)
{
	uint8_t num[5] = {0};
	uint8_t i,j,number;
	uint16_t location;
  
	location = Current_location();     //���ҵ�ǰλ��
	
	if(trip_swit==1 || (trip_swit==0 && odotrip_show_Flag))
	{
		num[2] = (kilometer_sub/1000)/100%10;
		num[1] = (kilometer_sub/1000)/10%10;        //ʮ
		num[0] = (kilometer_sub/1000)%10;           //��
		number = 3;
		All_LED_num[location++] = kilometer_num[252];   //trip
		All_LED_num[location++] = kilometer_num[253];   //trip
	}
	else if(trip_swit==2)
		number = 3;
	else if(trip_swit==0 && !odotrip_show_Flag)
	{
		number = 5;
		num[4] = kilometer/10000;
		num[3] = kilometer/1000%10;
		num[2] = kilometer/100%10;
		num[1] = kilometer/10%10;        //ʮ
		num[0] = kilometer%10;           //��
		for(i=0;i<4;i++)
		 All_LED_num[location++] = kilometer_num[i+250];    //odotrip
	}
	
	if(trip_swit!=2) //��Ϊ��ѹ��ʾ
	{
		for(j=0;j<number;j++)
		{
			 switch (num[j])
			 {
				case 0: 
					for(i=0;i<6;i++)
					All_LED_num[location++] = kilometer_num[i+(4-j)*49];break;
				case 1:
					for(i=0;i<2;i++)
					All_LED_num[location++] = kilometer_num[i+6+(4-j)*49];break;
				case 2:	
					for(i=0;i<5;i++)
					All_LED_num[location++] = kilometer_num[i+8+(4-j)*49];break;
				case 3:
					for(i=0;i<5;i++)
					All_LED_num[location++] = kilometer_num[i+13+(4-j)*49];break;
				case 4:
					for(i=0;i<4;i++)
					All_LED_num[location++] = kilometer_num[i+18+(4-j)*49];break;
				case 5:
					for(i=0;i<5;i++)
					All_LED_num[location++] = kilometer_num[i+22+(4-j)*49];break;
				case 6:
					for(i=0;i<6;i++)
					All_LED_num[location++] = kilometer_num[i+27+(4-j)*49];break;
				case 7:
					for(i=0;i<3;i++)
					All_LED_num[location++] = kilometer_num[i+33+(4-j)*49];break;
				case 8:
					for(i=0;i<7;i++)
					All_LED_num[location++] = kilometer_num[i+36+(4-j)*49];break;
				case 9:
					for(i=0;i<6;i++)
					All_LED_num[location++] = kilometer_num[i+43+(4-j)*49];break;
			 }
		}
		if(unit==1)      //km
		  All_LED_num[location++] = kilometer_num[254];
		else if(unit==2) //mile
			All_LED_num[location++] = kilometer_num[255];
		else if(unit==0)             //ȫ��
		{
		  All_LED_num[location++] = kilometer_num[254];
		  All_LED_num[location++] = kilometer_num[255];
		}
	}
	else
	{
		//60V
		if(!(GPIOA->PID & GPIO_PIN_5))       //60V��ͨ
		{
			for(i=0;i<6;i++)
				All_LED_num[location++] = kilometer_num[i+125];
			for(i=0;i<6;i++)
				All_LED_num[location++] = kilometer_num[i+147];
			for(i=0;i<5;i++)
				All_LED_num[location++] = kilometer_num[i+245];
		}
		else                                //60Vδ��ͨ
		{
			if(Dianping_Voltage<590)//48V
			{
				for(i=0;i<4;i++)
					All_LED_num[location++] = kilometer_num[i+116];
				for(i=0;i<7;i++)
					All_LED_num[location++] = kilometer_num[i+183];
				for(i=0;i<5;i++)
					All_LED_num[location++] = kilometer_num[i+245];
			}
			if(Dianping_Voltage>=590)//72V
			{
				for(i=0;i<3;i++)
					All_LED_num[location++] = kilometer_num[i+131];
				for(i=0;i<5;i++)
					All_LED_num[location++] = kilometer_num[i+155];
				for(i=0;i<5;i++)
					All_LED_num[location++] = kilometer_num[i+245];
			}
		}
	}
}

/*******************************************************************************
**�������ƣ�void Transfer_battery_to_All_LED_num(uint16_t voltage)
**����������������ֵ������ʾ����
**��ڲ�����
**�������
*******************************************************************************/
void Transfer_battery_to_All_LED_num(uint8_t value)                         //������Ҫ��ѹУ׼
{
	uint8_t  i;
	uint16_t location;
	location = Current_location();     //���ҵ�ǰλ��
	
	switch(value)
	{
		case 1:All_LED_num[location++] = battery[16];break;
		case 2:
			All_LED_num[location++] = battery[16];
		  All_LED_num[location++] = battery[17];
		  break;
		case 3:
			for(i=0;i<3;i++)
			All_LED_num[location++] = battery[16+i];break;
		case 4:
			for(i=0;i<4;i++)
			All_LED_num[location++] = battery[16+i];break;
		case 5:
			for(i=0;i<5;i++)
			All_LED_num[location++] = battery[16+i];break;
	}
	if(!battery_icon_blink)
	{
		All_LED_num[location++] = battery[0];
		All_LED_num[location++] = battery[1];
	}
	if(!battery_frame_blink)
	{
		for(i=0;i<14;i++)
		All_LED_num[location++] = battery[2+i];
	}
}


/*******************************************************************************
**�������ƣ�void Transfer_border_to_All_LED_num(uint8_t colour,uint8_t mode)
**��������������ʾ�߿�����ʾ����
**��ڲ���: 
**�������
*******************************************************************************/
void Transfer_border_to_All_LED_num(uint8_t colour,uint8_t mode)
{
	uint8_t  i;
	uint16_t location;
	location = Current_location();     //���ҵ�ǰλ��
	if(mode)
	{
		if(colour)         //��ɫ
		{
		  for(i=0;i<flow_water;i++)
			{
				All_LED_num[location++] = prompt[i];
				All_LED_num[location++] = prompt[i+10];
			}
		}
		else
		{
		  for(i=0;i<flow_water;i++)
			{
				All_LED_num[location++] = prompt[i+20];
				All_LED_num[location++] = prompt[i+30];
			}
		}
	}
	else
	{
		if(colour)
		{
			for(i=0;i<20;i++)
			All_LED_num[location++] = prompt[i];
		}
		else
		{
			for(i=0;i<20;i++)
			All_LED_num[location++] = prompt[i+20];
		}
	}
}

/*******************************************************************************
**�������ƣ�void Transfer_CarLed_to_All_LED_num()
**������������ledָʾ������ʾ����
**��ڲ���: 
**�������
*******************************************************************************/
void Transfer_CarLed_to_All_LED_num()
{
	uint8_t  i;
	uint16_t location;
	location = Current_location();     //���ҵ�ǰλ��
	
	
	if(car_led.Ready)
	{
		All_LED_num[location++] = light[55];
		All_LED_num[location++] = light[56];
	}
	if(car_led.Double_flash)
	{
		for(i=0;i<4;i++)
		All_LED_num[location++] = light[i+49];
	}
	if(car_led.High_beam)
	{
		All_LED_num[location++] = light[53];
		All_LED_num[location++] = light[54];
	}
	if(led_left_blink)
	{
		All_LED_num[location++] = light[49];
		All_LED_num[location++] = light[50];
	}
	if(car_led.Rear_fog_light)
	{
		All_LED_num[location++] = light[59];
		All_LED_num[location++] = light[60];
	}
	if(led_right_blink)
	{
		All_LED_num[location++] = light[51];
		All_LED_num[location++] = light[52];
	}
	if(car_led.Width_indicator)
	{
		All_LED_num[location++] = light[57];
		All_LED_num[location++] = light[58];
	}
}


/*******************************************************************************
**�������ƣ�void Transfer_onewire_to_All_LED_num()
**������������һ��ָͨʾ������ʾ����
**��ڲ���: 
**�������
*******************************************************************************/
void Transfer_onewire_to_All_LED_num()
{
	uint8_t  i;
	uint16_t location;
	location = Current_location();     //���ҵ�ǰλ��
	
	switch(OneWire_led.gear)
	{
		case 0:
			for(i=0;i<6;i++)
			All_LED_num[location++] = light[i];break;
		case 1:
			for(i=0;i<2;i++)
			All_LED_num[location++] = light[i+6];break;
		case 2:
			for(i=0;i<5;i++)
			All_LED_num[location++] = light[i+8];break;
		case 3:
			for(i=0;i<5;i++)
			All_LED_num[location++] = light[i+13];break;
		case 4:
			for(i=0;i<4;i++)
			All_LED_num[location++] = light[i+18];break;
		case 5:
			for(i=0;i<5;i++)
			All_LED_num[location++] = light[i+22];break;
		case 6:
			for(i=0;i<6;i++)
			All_LED_num[location++] = light[i+27];break;
		case 7:
			for(i=0;i<3;i++)
			All_LED_num[location++] = light[i+33];break;
		case 8:
			for(i=0;i<7;i++)
			All_LED_num[location++] = light[i+36];break;
		case 9:
			for(i=0;i<6;i++)
			All_LED_num[location++] = light[i+43];break;
	}
	if(OneWire_led.ECU_erro)  All_LED_num[location++] = fault[0];
	if(OneWire_led.brake_erro) All_LED_num[location++] = fault[2];
	if(OneWire_led.handle_erro) All_LED_num[location++] = fault[3];
	if(OneWire_led.motor_erro) All_LED_num[location++] = fault[1];
	if(OneWire_led.reserve_erro) {All_LED_num[location++] = fault[4];All_LED_num[location++] = fault[5];}
	if(OneWire_led.stop_erro)    {All_LED_num[location++] = fault[6];All_LED_num[location++] = fault[7];}
}

/*******************************************************************************
**�������ƣ�void Transfer_guide_to_All_LED_num()
**������������������Ϣ������ʾ����
**��ڲ���: 
**�������
*******************************************************************************/
void Transfer_guide_to_All_LED_num(uint8_t guide,uint32_t guide_distance,uint8_t unit)
{
	uint8_t  i,j,number;
	uint8_t num[3] = {0};
	uint16_t location;
	
	location = Current_location();     //���ҵ�ǰλ��
  	
	if(guide_distance > 99)     //�������99m��ʾ
	{
		guide_distance = guide_distance/100;
		if((0<=guide_distance)&&(guide_distance<100))
			number = 2;
		else if((100<=guide_distance)&&(guide_distance<1000))
			number = 3;
		
		num[2] = guide_distance/100;
		num[1] = guide_distance/10%10;
		num[0] = guide_distance%10;
	}
	else                         //����С��99m��ʾ 
	{
		guide_distance = guide_distance*10;
		if((0<=guide_distance)&&(guide_distance<100))
			number = 2;
		else if((100<=guide_distance)&&(guide_distance<1000))
			number = 3;
		
		num[2] = guide_distance/100;
		num[1] = guide_distance/10%10;
		num[0] = guide_distance%10;
	}
	
	if((guide && Flag_bluetooth_Connecting)||(guide==8))
	{
		for(i=0;i<4;i++)
		 All_LED_num[location++] = guide_num[i+160];
		for(j=0;j<number;j++)
		{
			 switch (num[j])
			 {
				case 0:for(i=0;i<6;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 1:for(i=6;i<8;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 2:for(i=8;i<13;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 3:for(i=13;i<18;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 4:for(i=18;i<22;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 5:for(i=22;i<27;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 6:for(i=27;i<33;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 7:for(i=33;i<36;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 8:for(i=36;i<43;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;
				case 9:for(i=43;i<49;i++)All_LED_num[location++] = guide_num[i+(2-j)*49];break;	
			 }
		}
		if(unit==1)
		All_LED_num[location++] = guide_num[149];                     //��λm
		else if(unit==2)
		All_LED_num[location++] = guide_num[148];                     //��λkm
		else if(unit==0)                                              //ȫ��
		{
			All_LED_num[location++] = guide_num[148];
			All_LED_num[location++] = guide_num[149];
		}
		All_LED_num[location++] = guide_num[147];                     //С����
	}
	switch (guide)
	{
		//���
		case 2:for(i=0;i<6;i++)All_LED_num[location++] = guide_num[i+190];break;
		//��
		case 4:for(i=0;i<4;i++)All_LED_num[location++] = guide_num[i+182];break;
		//��ǰ
		case 6:for(i=0;i<6;i++)All_LED_num[location++] = guide_num[i+170];break;
		//ǰ
		case 1:for(i=0;i<6;i++)All_LED_num[location++] = guide_num[i+164];break;
		//��ǰ
		case 7:for(i=0;i<6;i++)All_LED_num[location++] = guide_num[i+176];break;
		//��
		case 5:for(i=0;i<4;i++)All_LED_num[location++] = guide_num[i+186];break;
		//�Һ�
		case 3:for(i=0;i<6;i++)All_LED_num[location++] = guide_num[i+196];break;
		//ȫ��
		case 8:for(i=0;i<38;i++)All_LED_num[location++] = guide_num[i+164];break;
		default: break;
	}

	if(car_bluetooth.bluetooth)
	{
		All_LED_num[location++] = guide_num[150];
		All_LED_num[location++] = guide_num[151];
	}
	if(car_bluetooth.message){All_LED_num[location++] = guide_num[152];All_LED_num[location++] = guide_num[153];}
	if(car_bluetooth.phone){All_LED_num[location++] = guide_num[154];All_LED_num[location++] = guide_num[155];}
	if(car_bluetooth.spend_station){All_LED_num[location++] = guide_num[158];All_LED_num[location++] = guide_num[159];}
	if(car_bluetooth.tunnel){All_LED_num[location++] = guide_num[156];All_LED_num[location++] = guide_num[157];}
}