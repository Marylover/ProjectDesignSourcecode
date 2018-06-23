#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "hw_types.h"
#include "tm4c1294ncpdt.h"
#include "pin_map.h"
#include "sysctl.h"
#include "gpio.h"
#include "i2c.h"
#include "uart.h"
#include "interrupt.h"
#include "vl6180x_i2c.h"
#include "uartstdio.h"
#include "TCS34725_file.h"
#include "systick.h"

#include <softi2c.h>
#define SLAVE_ADDRESS 0x29

#define L0 GPIO_PIN_0   //��Ƭ������Ϣ����ӦPL0,PL1,PL2,PL3
#define L1 GPIO_PIN_1
#define L2 GPIO_PIN_2
#define L3 GPIO_PIN_3


#define LED GPIO_PIN_0

#define M0 GPIO_PIN_0   //�������źţ���ӦPK0,PK1,PK2,PK3,PK7
#define M1 GPIO_PIN_1
#define M2 GPIO_PIN_2
#define M3 GPIO_PIN_3
#define M4 GPIO_PIN_7


int ReadColor (void);
int DistanceRead(void);
void ReadCard (void);
void FoodOut();
void  Strike();
void RunMotor(int i);
uint32_t ui32SysClkFreq;
int sysrange_read;

//int foods[6];  //��ѡʳ�Ｐ������
//int foodsNum[6]; //ʣ��ʳ�Ｐ������
char receive [8];
//char foodsToPC[8];
//*********************************************************
//��ʱ����
//*********************************************************
void Delay1ms(uint32_t nDelay1ms)
{
    //
    //ʱ��Ƶ��120MHZ����������Ϊ0.1us
    //
    unsigned int i;
    uint32_t Delay;
    Delay=10000*nDelay1ms;
    for(i=0;i<Delay;i++);
}



void UARTIntHandler(void)
{
    uint32_t ui32Status;

        //
        // Get the interrrupt status.
        //
        ui32Status = UARTIntStatus(UART0_BASE, true);

        //
        // Clear the asserted interrupts.
        //
        UARTIntClear(UART0_BASE, ui32Status);

    UARTgets(receive,8);
    if(receive[0]>='0'&&receive[0]<='9')  //������λΪ0-9ʱΪ�Ϸ�ʳ�������������������
    {
        FoodOut();
    }


    if(receive[0] == 'A')  //ά��ģʽ�����ת1
    {
       
        Strike();
      
        UARTprintf("run 1   ");
    }
    if(receive[0] == 'C')   //ά��ģʽ����ȡ���봫����
    {
        int diatance = 0;

        diatance = DistanceRead();

        UARTprintf("%8d",diatance);
    }
    if(receive[0] == 'B')  //ά��ģʽ�����ת2
    {
    
        GPIOPinWrite(GPIO_PORTK_BASE,M4,0);
              
    UARTprintf("run 2   ");
    }

    if(receive[0] == 'D') //ά��ģʽ����ȡ��ɫ������
    {
        int color = -1;
        color = ReadColor ();
        switch(color)
        {
        case 0:
        UARTprintf("Blue    ");
        break;
        case 1:
        UARTprintf("Red     ");
                break;
        case 2:
        UARTprintf("Green   ");
                break;
        case 3:
        UARTprintf("Yellow  ");
                break;
        case 4:
        UARTprintf("Pink    ");
                break;
        case 5:
        UARTprintf("Violet  ");
                break;
        default:
        UARTprintf("ERROR   ");
            break;
        }
    }

    if(receive[0] == 'E')  //��ȡ������Ϣ����PC�˰���ʳ���б�ʱ����
       {
        ReadCard ();
       }
}


//******************************************************************
//��ʼ������
//******************************************************************
void UARTInit(void)
{
    //
      // Set the clocking to run directly from the crystal at 120MHz.
      //
    ui32SysClkFreq = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                               SYSCTL_OSC_MAIN |
                                               SYSCTL_USE_PLL |
                                               SYSCTL_CFG_VCO_480), 120000000);


      GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);

      //
      // Enable the peripherals used by this example.
      //
      SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
      SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

      //
      // Enable processor interrupts.
      //
      IntMasterEnable();

      //
      // Set GPIO A0 and A1 as UART pins.
      //
      GPIOPinConfigure(GPIO_PA0_U0RX);
      GPIOPinConfigure(GPIO_PA1_U0TX);
      GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

      //
      // Configure the UART for 115,200, 8-N-1 operation.
      //
      UARTConfigSetExpClk(UART0_BASE, ui32SysClkFreq, 115200,
                              (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                               UART_CONFIG_PAR_NONE));

      //
      // Enable the UART interrupt.
      //
      IntEnable(INT_UART0);
      UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

}

void ReadCard (void)
 {
    int infra_code[4];
    int tran_code;
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE,L0|L1|L2|L3);
    if(GPIOPinRead(GPIO_PORTL_BASE,L0))
    {
         infra_code[0] = 0;
    }
    else
    {
         infra_code[0] = 1;
    }
    if(GPIOPinRead(GPIO_PORTL_BASE,L1))
    {
         infra_code[1] = 0;
    }
    else
    {
         infra_code[1] = 1;
    }
    if(GPIOPinRead(GPIO_PORTL_BASE,L2))
    {
         infra_code[2] = 0;
    }
    else
    {
         infra_code[2] = 1;
    }
    if(GPIOPinRead(GPIO_PORTL_BASE,L3))
    {
         infra_code[3] = 0;
    }
    else
    {
         infra_code[3] = 1;
    }
    tran_code = infra_code[0]*8+infra_code[1]*4+infra_code[2]*2+infra_code[3]*1;
    switch(tran_code)
    {
        case 0:
           UARTprintf("11111111");
//           ȫ��������
            break;
        case 12:
            UARTprintf("01111111");
//            ����ɫ����
            break;
        case 9:
            UARTprintf("10111111");
//           �Ժ�ɫ����
            break;
        case 2:
            UARTprintf("11011111");
//            ����ɫ����
            break;
        case 8:
            UARTprintf("11101111");
//            �Ի�ɫ����
            break;
        case 10:
            UARTprintf("11110111");
//            �Է�ɫ����
            break;
        case 1:
            UARTprintf("11111011");
//            ����ɫ����
            break;
        default:
            UARTprintf("00000000");
//           ȫ������
            break;
    }
    DelayMS(50);
}


void RunMotor(int i)

{



    switch (i)

    {

                case 0:    //0001��23��

                    GPIOPinWrite(GPIO_PORTK_BASE, M0,0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);

                    DelayMS(10000);

                    break;

                case 1 :  //0010��45��

                   // GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);
                    DelayMS(10000);



                    break;

                case 2:  //0011��68��


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);

                    DelayMS(10000);



                    break;

                case 3:  //0100��90��


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    DelayMS(10000);



                    break;

                case 4:   //0101��113��


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);

                    DelayMS(10000);



                    break;

                case 5:  //0110��135��


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    DelayMS(10000);



                    break;

                case 6:  //0111��158��



                      GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                      GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                      GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                      GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);
                      DelayMS(10000);

                      break;

                case 7:   //1000��180��


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, M0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    DelayMS(10000);

                    break;

                default:   // 0000,��ԭ

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    break;

     }
}
void  Strike() //�������ȹ��ϣ��ٴ򿪣��ٹ���
{
    GPIOPinWrite(GPIO_PORTK_BASE,M4,0);  //��
           /* GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_0,0);
            GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_1,0);
            GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,0);  */
    DelayMS(30000);
    GPIOPinWrite(GPIO_PORTK_BASE,M4,M4);  //��
                   /* GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_0,GPIO_PIN_0);
                    GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_1,GPIO_PIN_1);
                    GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,GPIO_PIN_2);  */
    DelayMS(30000);
    GPIOPinWrite(GPIO_PORTK_BASE,M4,0);   //��
              /*  GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_0,0);
                GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_1,0);
                GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,0);  */
}

int ReadColor ()
{
    uint8_t readdata[3];
                             // SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    int colorNum;
    I2CMasterReadTCS34725(readdata,SLAVE_ADDRESS,RED_ADDR,1);//RED_ADDR BLUE_ADDR GREEN_ADDR
    Delay_ms(1000);
    I2CMasterReadTCS34725(&readdata[1],SLAVE_ADDRESS,GREEN_ADDR,1);//RED_ADDR BLUE_ADDR GREEN_ADDR
    Delay_ms(1000);
    I2CMasterReadTCS34725(&readdata[2],SLAVE_ADDRESS,BLUE_ADDR,1);//RED_ADDR BLUE_ADDR GREEN_ADDR
    Delay_ms(1000);
    float readdata1[3];
    readdata1[0]=readdata[0];
    readdata1[1]=readdata[1];
    readdata1[2]=readdata[2];
    readdata1[0]=(readdata1[0])/(readdata1[0]+readdata1[1]+readdata1[2])*400;
    readdata1[1]=(readdata1[1])/(readdata1[0]+readdata1[1]+readdata1[2])*400;
    readdata1[2]=(readdata1[2])/(readdata1[0]+readdata1[1]+readdata1[2])*400;
    readdata[0]=readdata1[0];
    readdata[1]=readdata1[1];
    readdata[2]=readdata1[2];
    UARTprintf("r: %d     ",readdata[0]);
    UARTprintf("g: %d     ",readdata[1]);
    UARTprintf("b: %d     ",readdata[2]);
    if((readdata[0]>155)&(readdata[0]<175)&(readdata[1]>120)&(readdata[1]<135)&(readdata[2]>115)&(readdata[2]<135))
        colorNum = 0;
   // UARTprintf("Blue    ");
    if((readdata[0]>198)&(readdata[0]<215)&(readdata[1]>100)&(readdata[1]<115)&(readdata[2]>95)&(readdata[2]<115))
        colorNum = 1;
   // UARTprintf("Red     ");
    if((readdata[0]>0)&(readdata[0]<40)&(readdata[1]>=215)&(readdata[1]<235)&(readdata[2]>130)&(readdata[2]<145))
        colorNum = 3;
   // UARTprintf("Yellow  ");
    if((readdata[0]>=0)&(readdata[0]<40)&(readdata[1]>180)&(readdata[1]<210)&(readdata[2]>145)&(readdata[2]<165))
        colorNum = 4;
   // UARTprintf("Pink    ");
    if((readdata[0]>170)&(readdata[0]<185)&(readdata[1]>=130)&(readdata[1]<145)&(readdata[2]>100)&(readdata[2]<115))
        colorNum = 2;
    //UARTprintf("Green   ");
    if((readdata[0]>170)&(readdata[0]<185)&(readdata[1]>105)&(readdata[1]<120)&(readdata[2]>120)&(readdata[2]<140))
        colorNum = 5;
    //UARTprintf("Violet  ");
    return colorNum;
}
int DistanceRead()
   {VL6180X_Init();
        Delay1ms(10);
        VL6180X_WriteBytes(DEV_ADDR,VL6180X_SYSRANGE_START,0x01);
        sysrange_read=VL6180x_ReadBytes(DEV_ADDR,VL6180X_RESULT_RANGE_VAL);
        Delay_ms(1000);
       return sysrange_read;
   }


void FoodOut()
{
    int foods[6] = {0,0,0,0,0,0};

   int  foodsNum[6] = {0,0,0,0,0,0};

   char foodsToPC[8] = {'0','0','0','0','0','0','0','0'};

    int distance = 0;         //С��10cmʱ���ܳ���

       int a =0; //char��intѭ������
       int b =0; //���ת��ѭ������
       int c =0; //int��charѭ������


      int color = 8;
      for( a = 0; a <= 5; a++)   //��PC�˷��İ�λASCII��תΪ��λ�����룬�ֱ����ÿ��ʳ������
      {
          foods[a] = receive[a] - '0';
      }
      for ( b = 0; b<=7; b++)
      {
          switch(b){
          case 0:
          {
             RunMotor(0);
             color = ReadColor();
             if(color>=0 && color<=5)
                 foodsNum[color] = foodsNum[color] +1;
              if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
              {  //����ɫ��������ɫΪ����������һ�֣��Ҹ�����ɫ����������Ϊʱ��ʼ����ж�
                  distance = DistanceRead();
                  if(distance<=100)
                  {
                  Strike();
                  foodsNum[color] = foodsNum[color] -1;
                  foods[color]=foods[color]-1;
                  DelayMS(30000);
                  }
              }
              break;
          }
          case 1:
                 {
              RunMotor(1);
                    color = ReadColor();
                    if(color>=0 && color<=5)
                        foodsNum[color] = foodsNum[color] +1;
                    if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
                               {
                                   distance = DistanceRead();
                                   if(distance<=100)
                                   {
                                   Strike();
                                   foodsNum[color] = foodsNum[color] -1;
                                   foods[color]=foods[color]-1;
                                   DelayMS(30000);
                                   }
                               }
                    break;
                 }
          case 2:
                 {
              RunMotor(2);
                    color = ReadColor();
                    if(color>=0 && color<=5)
                        foodsNum[color] = foodsNum[color] +1;
                    if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
                               {
                                   distance = DistanceRead();
                                   if(distance<=100)
                                   {
                                   Strike();
                                   foodsNum[color] = foodsNum[color] -1;
                                   foods[color]=foods[color]-1;
                                   DelayMS(30000);
                                   }
                               }
                    break;
                 }
          case 3:
                 {
              RunMotor(3);
                    color = ReadColor();
                    if(color>=0 && color<=5)
                        foodsNum[color] = foodsNum[color] +1;
                    if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
                               {
                                   distance = DistanceRead();
                                   if(distance<=100)
                                   {
                                   Strike();
                                   foodsNum[color] = foodsNum[color] -1;
                                   foods[color]=foods[color]-1;
                                   DelayMS(30000);
                                   }
                               }
                    break;
                 }
          case 4:
                 {
              RunMotor(4);
                    color = ReadColor();
                    if(color>=0 && color<=5)
                        foodsNum[color] = foodsNum[color] +1;
                    if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
                               {
                                   distance = DistanceRead();
                                   if(distance<=100)
                                   {
                                   Strike();
                                   foodsNum[color] = foodsNum[color] -1;
                                   foods[color]=foods[color]-1;
                                   DelayMS(30000);
                                   }
                               }
                    break;
                 }
          case 5:
                 {
              RunMotor(5);
                    color = ReadColor();
                    if(color>=0 && color<=5)
                        foodsNum[color] = foodsNum[color] +1;
                    if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
                               {
                                   distance = DistanceRead();
                                   if(distance<=100)
                                   {
                                   Strike();
                                   foodsNum[color] = foodsNum[color] -1;
                                   foods[color]=foods[color]-1;
                                   DelayMS(30000);
                                   }
                               }
                    break;
                 }
          case 6:
                        {
                     RunMotor(6);
                           color = ReadColor();
                           if(color>=0 && color<=5)
                               foodsNum[color] = foodsNum[color] +1;
                           if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
                                      {
                                          distance = DistanceRead();
                                          if(distance<=100)
                                          {
                                          Strike();
                                          foodsNum[color] = foodsNum[color] -1;
                                          foods[color]=foods[color]-1;
                                          DelayMS(30000);
                                          }
                                      }
                           break;
                        }
          case 7:
          {
                            RunMotor(7);
                                  color = ReadColor();
                                  if(color>=0 && color<=5)
                                      foodsNum[color] = foodsNum[color] +1;
                                  if(((color==0)&&foods[0])||((color==1)&&foods[1])||((color==2)&&foods[2])||((color==3)&&foods[3])||((color==4)&&foods[4])||((color==5)&&foods[5]))
                                             {
                                                 distance = DistanceRead();
                                                 if(distance<=100)
                                                 {
                                                 Strike();
                                                 foodsNum[color] = foodsNum[color] -1;
                                                 foods[color]=foods[color]-1;
                                                 DelayMS(30000);
                                                 }
                                             }
                                  break;
                               }
          default:
              break;
          }
      }
      RunMotor(8);  //�ص�ԭ��
     for( c = 0; c<=5;c++)    //��6λint����ת��Ϊ8λchar���飬��Ϊ���͸�PC��ʣ��ʳ������
     {
         foodsToPC[c] = '0'+ foodsNum[c];
     }
    /* foodsToPC[6] = '0';
     foodsToPC[7] = '\n';  */
     UARTprintf("%s",foodsToPC);
}



//*******************************************************************
//������
//*******************************************************************



int main(void)
{


    //
    //����ʱ��Ϊ16MHZ
    //

    ui32SysClkFreq =SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 120000000);

    SysTickPeriodSet(SysCtlClockGet());
           SysTickEnable();
    //
    //������ʱ10ms
    //
    Delay1ms(10);

    //
    //ʹ��I2Cģ��
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);  //ʹ��GPIOK,��Ϊ����źŵ����
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, M0|M1|M2|M3|M4);  //��PK0,PK1,PK2��Ϊ���������


    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,LED);

    GPIOPinConfigure(GPIO_PN5_I2C2SCL);
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);
    GPIOPinTypeI2C(GPIO_PORTN_BASE,GPIO_PIN_4);
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE,GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //
    //ʹ��GPIOBģ��
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    //����PB2ΪI2C��SCL����
    //
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);

    //
    //����PB3ΪI2C��SDA����
    //
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    //
    //����PB3��������
    //
    GPIOPinTypeI2C(GPIO_PORTB_BASE,GPIO_PIN_3);

    //
    //����PB2��������
    //
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE,GPIO_PIN_2);

    //
    //����I2C0ģ���ͨ��ʱ�ӣ�false����������Ϊ100kbit/s,true��������Ϊ400kbit/s
    //
    I2CMasterInitExpClk(I2C0_BASE,SysCtlClockGet(),false);
    I2CMasterInitExpClk(I2C2_BASE,SysCtlClockGet(),false);
    TCS34725_Enable();
    TCS34725_SetIntegrationTime(TCS34725_INTEGRATIONTIME_24MS);
    TCS34725_SetGain(TCS34725_GAIN_4X);
    UARTInit();
    UARTStdioConfig(0,115200,ui32SysClkFreq);

     while(1)
     {
        /* int distance = 0;
         distance = DistanceRead();
         if(distance>100)  //���������10����ʱ��LED����
         GPIOPinWrite(GPIO_PORTN_BASE, LED, LED);
         else
         GPIOPinWrite(GPIO_PORTN_BASE, LED, 0);*/

     }

     return 0;
}

