/********************************/
/*       左轮前进样例程序       */ 
/********************************/


//小车单片机对应的头文件中定义了P1_0,P1_1等单片机的端口， 
//比赛用的小车上，这些端口与电机驱动模块、四路巡线模块等硬件相连接， 
//通过这些值即可设置或读取该端口连接的硬件的状态， 
//具体的连接对应方式请看后面的各条注释。 
#include<AT89x51.H>

//P1_0和P1_1是左右电机的使能端，为1时电机能转动，为0时电机不能转。 
#define Left_moto_pwm     P1_0	   //P1_0接驱动模块ENA(左电机) 
#define Right_moto_pwm    P1_1	   //P1_1接驱动模块ENB(右电机) 

//P3_3至P3_6是用来控制左右电机的运动方向的。 
#define Left_moto_go      {P3_3=0,P3_4=1;}	//P3_3,P3_4接IN1,IN2	当P3_3=0,P3_4=1时左电机前进
#define Left_moto_back    {P3_3=1,P3_4=0;}	//P3_3,P3_4接IN1,IN2	当P3_3=1,P3_4=0时左电机后退
#define Left_moto_stp     {P3_3=1,P3_4=1;}	//P3_3,P3_4接IN1,IN2	当P3_4=1,P3_5=1时左电机停转                 
#define Right_moto_go     {P3_5=0,P3_6=1;}	//P3_5,P3_6接IN3,IN4	当P3_5=0,P3_6=1时右电机前转
#define Right_moto_back   {P3_5=1,P3_6=0;}	//P3_5,P3_6接IN3,IN4	当P3_5=1,P3_6=0时右电机后退
#define Right_moto_stp    {P3_5=1,P3_6=1;}	//P3_5,P3_6接IN3,IN4	当P3_5=1,P3_6=1时右电机停转

void main(void)
{
	while(1)//通过一个无限循环实现小车的持续运动 
	{ 	
		Left_moto_pwm = 1;//设置左轮能转动 
		Left_moto_go;//使左轮前进
		//通过左轮前进，右轮不前进可以实现简单的小车右转
	}
}
