#include "adc.h"
uint8_t Power_48V_flag,Power_72V_flag = 0;


__IO float TempValue;

//NTC阻温特性表
const uint16_t  wRTtable[RT_NUM] =
{
 /*  0     1     2     3     4     5     6     7     8     9  */   
     994,  992, 991,   989,  987,  985,  983,  981,  979,  977,
 /*  10    11    12    13    14    15    16    17    18    19 */   
     975,  973,  970,  968,  965,  962,  960,  957,  954,  951,  
 /*  20    21    22    23    24    25    26    27    28    29 */
     947,  944,  941,  937,  934,  930,  926,  922,  918,  913,
 /*  30    31    32    33    34    35    36    37    38    39 */
     909,  905,  900,  895,  890,  885,  880,  875,  870,  864,     
 /*  40    41    42    43    44    45    46    47    48    49 */
     858,  853,  847,  841,  835,  828,  822,  816,  809,  802, 
 /*  50    51    52    53    54    55    56    57    58    59 */
     795,  788,  781,  774,  767,  760,  752,  745,  737,  729, 
 /*  60    61    62    63    64    65    66    67    68    69 */
     721,  713,  705,  697,  689,  681,  673,  665,  656,  648, 
 /*  70    71    72    73    74    75    76    77    78    79 */  
     640,  631,  623,  614,  606,  597,  589,  581,  572,  564,  
 /*  80    81    82    83    84    85    86    87    88    89 */ 
     555,  547,  538,  530,  521,  513,  505,  497,  488,  480,  
 /*  90    91    92    93    94    95    96    97    98    99 */ 
     472,  464,  456,  448,  440,  433,  425,  417,  410,  402, 
 /*  100   101   102   103   104   105   106   107   108   109 */ 
     395,  388,  381,  374,  366,  360,  353,  346,  339,  333, 
 /*  110   111   112   113   114   115   116   117   118   119 */ 
     326,  320,  300,  308,  302,  296,  290,  284,  278,  273, 
 /*  120   121   122   123   124   125   126   127   128   129 */ 
     267,  262,  257,  251,  246,  241,  236,  232,  227,  222, 
 /*  130   131   132   133   134   135   136   137   138   139 */
     218,  213,  209,  205,  200,  196,  192,  188,  184,  181, 
 /*  140   141   142   143   144   145   146   147   148   149 */
     177,  173,  170,  166,  163,  159,  156,  153,  150,  147, 
 /*  150   151   152   153   154   155   156   157   158   159 */    
     144,  141,  138,  135,  132,  130,  127,  124,  122,  119, 
};



void ADC_Init(void)
{
	  GPIO_InitType GPIO_InitStructure;
	  ADC_InitType  ADC_InitStructure;
	
	  RCC_AHB_Peripheral_Clock_Enable(RCC_AHB_PERIPH_ADC);                    //开启ADC时钟
    ADC_Clock_Mode_Config(ADC_CKMOD_AHB, RCC_ADCHCLK_DIV16);
    RCC_ADC_1M_Clock_Config(RCC_ADC1MCLK_SRC_HSE, RCC_ADC1MCLK_DIV8);       //selsect HSE as RCC ADC1M CLK Source
	  
	  //   PB1:速度   PA0:电池
	  GPIO_Structure_Initialize(&GPIO_InitStructure);
    GPIO_InitStructure.Pin       = GPIO_PIN_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
    GPIO_Peripheral_Initialize(GPIOB, &GPIO_InitStructure);
	
	  GPIO_InitStructure.Pin       =  GPIO_PIN_0;
	  GPIO_InitStructure.GPIO_Mode = GPIO_MODE_ANALOG;
	  GPIO_Peripheral_Initialize(GPIOA, &GPIO_InitStructure);
	
	  ADC_Initializes_Structure(&ADC_InitStructure);                             //默认值填充
    ADC_InitStructure.MultiChEn      = DISABLE;                                //多通道连续转换使能
    ADC_InitStructure.ContinueConvEn = DISABLE;                                //单次转换
    ADC_InitStructure.DatAlign       = ADC_DAT_ALIGN_R;                        //ADC数据右对齐
    ADC_InitStructure.ExtTrigSelect  = ADC_EXT_TRIGCONV_REGULAR_SWSTRRCH;      //软件控制触发
    ADC_InitStructure.ChsNumber      = ADC_REGULAR_LEN_1;
    ADC_Initializes(&ADC_InitStructure);
		
		ADC_ON();
		/* Check ADC Ready */
		while(ADC_Flag_Status_Get(ADC_RD_FLAG, ADC_FLAG_JENDCA, ADC_FLAG_RDY) == RESET);
		/* Start ADC calibration */
		ADC_Calibration_Operation(ADC_CALIBRATION_ENABLE);                                 //开始校准
		/* Check the end of ADC calibration */
		while (ADC_Calibration_Operation(ADC_CALIBRATION_STS) == SET);                    
		/* Start ADC Software Conversion */
		ADC_Regular_Channels_Software_Conversion_Operation(ADC_EXTRTRIG_SWSTRRCH_ENABLE);   //软件触发开始

//    ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel_01_PA0, ADC_REGULAR_NUMBER_1);
//	  ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel_11_PB2, ADC_REGULAR_NUMBER_11);
//	  ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel_12_PB10, ADC_REGULAR_NUMBER_12);
}




uint16_t ADC_GetData(uint8_t ADC_Channel)
{
	  uint8_t  i;
    uint16_t dat,sum = 0;
	  /* 通道采样时间配置 */
    ADC_Channel_Sample_Time_Config(ADC_Channel ,ADC_SAMP_TIME_55CYCLES5);
	  /* 常规通道配置 */
    ADC_Regular_Sequence_Conversion_Number_Config(ADC_Channel, ADC_REGULAR_NUMBER_1);
	  
		for(i=0;i<10;i++)
		{
			ADC_Regular_Channels_Software_Conversion_Operation(ADC_EXTRTRIG_SWSTRRCH_ENABLE);   //Start ADC Software Conversion
			while(ADC_Flag_Status_Get(ADC_RUN_FLAG ,ADC_FLAG_ENDC ,ADC_FLAG_RDY)==0){
			}
			ADCConvertedValue[i]=ADC_Regular_Group_Conversion_Data_Get();
			ADC_Flag_Status_Clear(ADC_FLAG_ENDC);
			ADC_Flag_Status_Clear(ADC_FLAG_STR);
		}
		bubble_sort(ADCConvertedValue,10);                //排序
		for(i=1;i<9;i++)
		  sum += ADCConvertedValue[i];
		dat = sum/8;                                 
    return dat;
}



/*******************************************************************************
**函数名称：uint8_t Speed_get(void)
**功能描述：根据电压值返回速度大小
**入口参数：
**输出：无
*******************************************************************************/
uint8_t Speed_get(void)
{
	  uint8_t SPEED = 0;
	  uint16_t Speed_Voltage = 0;
	
    Speed_Voltage = ((ADC_GetData(ADC_Channel_10_PB1)*825)>>10)*21.6/100;                //Dianping_Voltage = ADC_GetData(ADC_Channel_12_PB10)*3300/4096;
	  if(Speed_Voltage > 33)
    SPEED = (Speed_Voltage-33)/6;
		
		if(SPEED>99)
			SPEED = 99;
	  return SPEED;
}


/*******************************************************************************
**函数名称：uint8_t Battery_Voltage_get(void)
**功能描述：根据电压值返回电量值
**入口参数：
**输出：无
*******************************************************************************/
uint8_t Battery_Voltage_get(void)
{
    Dianping_Voltage =((ADC_GetData(ADC_Channel_01_PA0)*825)>>10)*31/100+4;                //Dianping_Voltage = ADC_GetData(ADC_Channel_12_PB10)*3300/4096;
	  if(!(GPIOA->PID & GPIO_PIN_5)) //60v
		{
			if((Dianping_Voltage < Value_60V_1_2) && (Dianping_Voltage > Value_60V_poor))
			{
				battery_icon = 1;
				battery_frame = 0;
				return 1;
			}				
			if((Dianping_Voltage < Value_60V_2_3) && (Dianping_Voltage >= Value_60V_1_2))
			{
				if(Dianping_Voltage < Value_60V_charge)
				{
						battery_icon = 1;
					  battery_frame = 0;
				}else {battery_icon = battery_frame =0;}
				return 2;
			}				   
			if((Dianping_Voltage < Value_60V_3_4) && (Dianping_Voltage >= Value_60V_2_3))    {battery_icon = battery_frame =0; return 3;}
			if((Dianping_Voltage < Value_60V_4_5) && (Dianping_Voltage >= Value_60V_3_4))   {battery_icon = battery_frame =0; return 4;}
			if(Dianping_Voltage >= Value_60V_4_5)                                   {battery_icon = battery_frame =0; return 5;}
			if(Dianping_Voltage <= Value_60V_poor)
			{
				battery_icon = battery_frame = 1;
				return 1;
			}
		}
		else if(((Dianping_Voltage < DP_48_72V_change)&& !Power_72V_flag)||Power_48V_flag) //48V
		{
			Power_48V_flag = 1;
			if((Dianping_Voltage < Value_48V_1_2) && (Dianping_Voltage > Value_48V_poor))
			{
				battery_icon = 1;
				battery_frame = 0;
				return 1;
			}				
			if((Dianping_Voltage < Value_48V_2_3) && (Dianping_Voltage >= Value_48V_1_2))
			{
				if(Dianping_Voltage < Value_48V_charge)
				{
						battery_icon = 1;
					  battery_frame = 0;
				}else {battery_icon = battery_frame =0;}
				return 2;
			}				   
			if((Dianping_Voltage < Value_48V_3_4) && (Dianping_Voltage >= Value_48V_2_3))    {battery_icon = battery_frame =0; return 3;}
			if((Dianping_Voltage < Value_48V_4_5) && (Dianping_Voltage >= Value_48V_3_4))     {battery_icon = battery_frame =0; return 4;}
			if(Dianping_Voltage >= Value_48V_4_5)                                  {battery_icon = battery_frame =0; return 5;}
			if(Dianping_Voltage <= Value_48V_poor)
			{
				battery_icon = battery_frame = 1;
				return 1;
			}
		}
		else if(((Dianping_Voltage >= DP_48_72V_change)&& !Power_48V_flag)||Power_72V_flag) //72V
		{
			Power_72V_flag = 1;
			if((Dianping_Voltage < Value_72V_1_2)&&(Dianping_Voltage > Value_72V_poor))
			{
				battery_icon = 1;
				battery_frame = 0;
				return 1;
			}				
			if((Dianping_Voltage < Value_72V_2_3) && (Dianping_Voltage >= Value_72V_1_2))
			{
				if(Dianping_Voltage < Value_72V_charge)
				{
						battery_icon = 1;
					  battery_frame = 0;
				}
				else {battery_icon = battery_frame =0;}
				return 2;
			}				   
			if((Dianping_Voltage < Value_72V_3_4) && (Dianping_Voltage >= Value_72V_2_3))    {battery_icon = battery_frame =0; return 3;}
			if((Dianping_Voltage < Value_72V_4_5) && (Dianping_Voltage >= Value_72V_3_4))    {battery_icon = battery_frame =0; return 4;}
			if(Dianping_Voltage >= Value_72V_4_5)                                           {battery_icon = battery_frame =0; return 5;}
			if(Dianping_Voltage <= Value_72V_poor)
			{
				battery_icon = battery_frame = 1;
				return 1;
			}
		}
}



/*******************************************************************************
**函数名称：void FindAdcTemp(uint16_t wAdcVal, uint16_t wTempData)
**功能描述：查NTC温度表
**入口参数：wAdcVal：ADC通道值，wTempData：温度值
**输出：无
*******************************************************************************/
void FindAdcTemp(uint16_t wAdcVal, uint16_t wTempData)
{
  uint8_t nBegin = 0;
  uint8_t nEnd= 0;
  uint8_t nMiddle = 0;
	static uint16_t wDiffAdcVal = 0;
  /* 首先判断是否超出表格的最大最小值 */
  nEnd = RT_NUM -1;
  if (wAdcVal <= *(wRTtable + nEnd))              //adc超出表格最小值
  {
    wTempData = RT_NUM + MINTEMP;                 //赋表格最高温度     
  }
  else if (wAdcVal >= *(wRTtable + nBegin))        //adc超出表格最大值
  {
    wTempData = MINTEMP;                           //赋表格最低温度
  }
  else /* 当温度在表格范围内 */
  {
    while ((nBegin < nEnd) && (wAdcVal != *(wRTtable + nMiddle)))
    {
      nMiddle = nBegin + (nEnd - nBegin)>>1;                              
      if (wAdcVal > *(wRTtable + nMiddle))
      {
        nEnd = nMiddle - 1;
      }
      else
      {
        nBegin = nMiddle + 1;
      }
    }
    wTempData = MINTEMP + (nBegin -1);
    /* 接下来要计算当前的值更偏向于前一个还是后一个数 */
    if (wAdcVal < *(wRTtable + nBegin -1))
    {
      wDiffAdcVal = (*(wRTtable + nBegin -1) - wAdcVal)*10;
      wDiffAdcVal /= *(wRTtable + nBegin -1) - *(wRTtable + nBegin);
      if (wDiffAdcVal >= 5) // 四舍五入
      {
        wTempData = MINTEMP + nBegin;
      }
    }
  }
}





/*******************************************************************************
**函数名称：void bubble_sort(int arr[], int len)
**功能描述：冒泡排序
**入口参数：
**输出：无
*******************************************************************************/
void bubble_sort(uint16_t arr[], uint8_t len)
{
	uint8_t i, j;
	uint16_t temp;
	for(i = 0; i < len - 1; i++)
			for(j = 0; j < len - 1 - i; j++)
					if(arr[j] > arr[j + 1]) 
					{
						temp = arr[j];
						arr[j] = arr[j + 1];
						arr[j + 1] = temp;
					}
}
