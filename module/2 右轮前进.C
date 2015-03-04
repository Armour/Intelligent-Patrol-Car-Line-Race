/********************************/
/*       右轮前进样例程序       */ 
/********************************/

#include<AT89x51.H>

#define Left_moto_pwm     P1_0	   //P1_0接驱动模块ENA(左电机) 
#define Right_moto_pwm    P1_1	   //P1_1接驱动模块ENB(右电机) 
 
#define Left_moto_go      {P3_3=0,P3_4=1;}	//P3_3,P3_4接IN1,IN2	当P3_3=0,P3_4=1时左电机前进
#define Left_moto_back    {P3_3=1,P3_4=0;}	//P3_3,P3_4接IN1,IN2	当P3_3=1,P3_4=0时左电机后退
#define Left_moto_stp     {P3_3=1,P3_4=1;}	//P3_3,P3_4接IN1,IN2	当P3_4=1,P3_5=1时左电机停转                 
#define Right_moto_go     {P3_5=0,P3_6=1;}	//P3_5,P3_6接IN3,IN4	当P3_5=0,P3_6=1时右电机前转
#define Right_moto_back   {P3_5=1,P3_6=0;}	//P3_5,P3_6接IN3,IN4	当P3_5=1,P3_6=0时右电机后退
#define Right_moto_stp    {P3_5=1,P3_6=1;}	//P3_5,P3_6接IN3,IN4	当P3_5=1,P3_6=1时右电机停转

void main(void)
{
	while(1)
	{
		Right_moto_pwm = 1;//设置右轮能转动
		Right_moto_go;//使右轮前进 
		//通过右轮前进，左轮不前进可以实现简单的小车左转 
	}
}
