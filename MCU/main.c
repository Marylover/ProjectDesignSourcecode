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

#define L0 GPIO_PIN_0   //卡片过敏信息，对应PL0,PL1,PL2,PL3
#define L1 GPIO_PIN_1
#define L2 GPIO_PIN_2
#define L3 GPIO_PIN_3


#define LED GPIO_PIN_0

#define M0 GPIO_PIN_0   //电机输出信号，对应PK0,PK1,PK2,PK3,PK7
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

//int foods[6];  //被选食物及其数量
//int foodsNum[6]; //剩余食物及其数量
char receive [8];
//char foodsToPC[8];
//*********************************************************
//延时函数
//*********************************************************
void Delay1ms(uint32_t nDelay1ms)
{
    //
    //时钟频率120MHZ，机器周期为0.1us
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
    if(receive[0]>='0'&&receive[0]<='9')  //数组首位为0-9时为合法食物数量，进入出货函数
    {
        FoodOut();
    }


    if(receive[0] == 'A')  //维护模式，电机转1
    {
       
        Strike();
      
        UARTprintf("run 1   ");
    }
    if(receive[0] == 'C')   //维护模式，读取距离传感器
    {
        int diatance = 0;

        diatance = DistanceRead();

        UARTprintf("%8d",diatance);
    }
    if(receive[0] == 'B')  //维护模式，电机转2
    {
    
        GPIOPinWrite(GPIO_PORTK_BASE,M4,0);
              
    UARTprintf("run 2   ");
    }

    if(receive[0] == 'D') //维护模式，读取颜色传感器
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

    if(receive[0] == 'E')  //读取过敏信息，当PC端按下食物列表时触发
       {
        ReadCard ();
       }
}


//******************************************************************
//初始化串口
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
//           全部不过敏
            break;
        case 12:
            UARTprintf("01111111");
//            对蓝色过敏
            break;
        case 9:
            UARTprintf("10111111");
//           对红色过敏
            break;
        case 2:
            UARTprintf("11011111");
//            对绿色过敏
            break;
        case 8:
            UARTprintf("11101111");
//            对黄色过敏
            break;
        case 10:
            UARTprintf("11110111");
//            对粉色过敏
            break;
        case 1:
            UARTprintf("11111011");
//            对紫色过敏
            break;
        default:
            UARTprintf("00000000");
//           全都过敏
            break;
    }
    DelayMS(50);
}


void RunMotor(int i)

{



    switch (i)

    {

                case 0:    //0001，23°

                    GPIOPinWrite(GPIO_PORTK_BASE, M0,0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);

                    DelayMS(10000);

                    break;

                case 1 :  //0010，45°

                   // GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);
                    DelayMS(10000);



                    break;

                case 2:  //0011，68°


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);

                    DelayMS(10000);



                    break;

                case 3:  //0100，90°


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    DelayMS(10000);



                    break;

                case 4:   //0101，113°


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);

                    DelayMS(10000);



                    break;

                case 5:  //0110，135°


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    DelayMS(10000);



                    break;

                case 6:  //0111，158°



                      GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                      GPIOPinWrite(GPIO_PORTK_BASE, M1, M1);

                      GPIOPinWrite(GPIO_PORTK_BASE, M2, M2);

                      GPIOPinWrite(GPIO_PORTK_BASE, M3, M3);
                      DelayMS(10000);

                      break;

                case 7:   //1000，180°


                    GPIOPinWrite(GPIO_PORTK_BASE, M0, M0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    DelayMS(10000);

                    break;

                default:   // 0000,复原

                    GPIOPinWrite(GPIO_PORTK_BASE, M3, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M0, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M1, 0);

                    GPIOPinWrite(GPIO_PORTK_BASE, M2, 0);

                    break;

     }
}
void  Strike() //打击电机先关上，再打开，再关上
{
    GPIOPinWrite(GPIO_PORTK_BASE,M4,0);  //关
           /* GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_0,0);
            GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_1,0);
            GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,0);  */
    DelayMS(30000);
    GPIOPinWrite(GPIO_PORTK_BASE,M4,M4);  //开
                   /* GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_0,GPIO_PIN_0);
                    GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_1,GPIO_PIN_1);
                    GPIOPinWrite(GPIO_PORTK_BASE,GPIO_PIN_2,GPIO_PIN_2);  */
    DelayMS(30000);
    GPIOPinWrite(GPIO_PORTK_BASE,M4,0);   //关
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

    int distance = 0;         //小于10cm时才能出货

       int a =0; //char到int循环变量
       int b =0; //电机转动循环变量
       int c =0; //int到char循环变量


      int color = 8;
      for( a = 0; a <= 5; a++)   //把PC端发的八位ASCII码转为六位数字码，分别代表每种食物数量
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
              {  //当颜色传感器颜色为蓝到紫其中一种，且该种颜色所需数量不为时开始打击判断
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
      RunMotor(8);  //回到原点
     for( c = 0; c<=5;c++)    //把6位int数组转化为8位char数组，作为发送给PC的剩余食物数量
     {
         foodsToPC[c] = '0'+ foodsNum[c];
     }
    /* foodsToPC[6] = '0';
     foodsToPC[7] = '\n';  */
     UARTprintf("%s",foodsToPC);
}



//*******************************************************************
//主函数
//*******************************************************************



int main(void)
{


    //
    //设置时钟为16MHZ
    //

    ui32SysClkFreq =SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |SYSCTL_CFG_VCO_480), 120000000);

    SysTickPeriodSet(SysCtlClockGet());
           SysTickEnable();
    //
    //启动延时10ms
    //
    Delay1ms(10);

    //
    //使能I2C模块
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);  //使能GPIOK,作为电机信号的输出
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, M0|M1|M2|M3|M4);  //把PK0,PK1,PK2作为数字量输出


    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE,LED);

    GPIOPinConfigure(GPIO_PN5_I2C2SCL);
    GPIOPinConfigure(GPIO_PN4_I2C2SDA);
    GPIOPinTypeI2C(GPIO_PORTN_BASE,GPIO_PIN_4);
    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE,GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);

    //
    //使能GPIOB模块
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    //设置PB2为I2C的SCL功能
    //
    GPIOPinConfigure(GPIO_PB2_I2C0SCL);

    //
    //设置PB3为I2C的SDA功能
    //
    GPIOPinConfigure(GPIO_PB3_I2C0SDA);

    //
    //进行PB3引脚配置
    //
    GPIOPinTypeI2C(GPIO_PORTB_BASE,GPIO_PIN_3);

    //
    //进行PB2引脚配置
    //
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE,GPIO_PIN_2);

    //
    //配置I2C0模块的通信时钟，false代表传输速率为100kbit/s,true代表速率为400kbit/s
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
         if(distance>100)  //当距离大于10厘米时，LED灯亮
         GPIOPinWrite(GPIO_PORTN_BASE, LED, LED);
         else
         GPIOPinWrite(GPIO_PORTN_BASE, LED, 0);*/

     }

     return 0;
}

