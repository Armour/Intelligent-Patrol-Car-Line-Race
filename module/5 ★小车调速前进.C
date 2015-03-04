/********************************/
/*     小车调速前进样例程序     */ 
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

/************************************************************************************************/
unsigned char push_val_left =0;//左电机占空比,1为最慢,10是最快,通过这个值可以改变其速度
unsigned char push_val_right=0;//右电机占空比,1为最慢,10是最快,通过这个值可以改变其速度
unsigned char pwm_val_left  =0;
unsigned char pwm_val_right =0;
bit Left_moto_stop =1;
bit Right_moto_stop=1;
unsigned int time=0;

//以下三个函数实现了电机调速运行的功能
// 
//使用方法： 
//如果需要使用调速运行的功能，只需将以下三个函数和前面的变量定义直接拷贝到你的代码中即可，
//使用的时候可以方便地通过设置push_val_left和push_val_right来控制占空比控制速度。
// 
//调速原理（以左轮为例）： 
//时间中断服务函数timer0()会每隔1ms自动运行，
//调用pwm_out_left_moto()使pwm_val_left每毫秒加1，加到10时再归零，
//如果你将push_val_left设置成了3，那么pwm_val_left为1-3时轮子转，4-10时不转
//也就是说，以下三个函数可以使轮子转3ms，停7ms，转3ms，停7ms...
//
//因此，通过push_val_left这个值就可以设置轮子在10ms中转的毫秒数，从而控制前进速度，
//设为1时转的最慢，设为10最快。
//具体设置为多少比较好可以参考最后一个样例程序“小车巡线程序.C”，也可以在赛前几天来调试（我们到时候会通知调试时间地点）。 
// 
void pwm_out_left_moto(void)//左电机调速
{  
	if(Left_moto_stop){
	if(pwm_val_left<=push_val_left)
		Left_moto_pwm=1; 
	else 
		Left_moto_pwm=0;
	if(pwm_val_left>=10)
	pwm_val_left=0;
	}
	else  Left_moto_pwm=0;
}
void pwm_out_right_moto(void)//右电机调速
{ 
	if(Right_moto_stop){ 
    if(pwm_val_right<=push_val_right)
		Right_moto_pwm=1; 
	else 
		Right_moto_pwm=0;
	if(pwm_val_right>=10)
	pwm_val_right=0;
	}
	else    Right_moto_pwm=0;
}
void timer0()interrupt 1 using 2 //TIMER0中断服务子函数产生PWM信号
{
	TH0=0XF8;//1Ms定时
	TL0=0X30;
	time++;
	pwm_val_left++;
	pwm_val_right++;
	pwm_out_left_moto();
	pwm_out_right_moto();
}
/************************************************************************************************/


void run(unsigned int speed)//前进函数，以speed的占空比前进 
{
	push_val_left=speed;	//调节左电机占空比
	push_val_right=speed;	//调节右电机占空比
	Left_moto_go;		//左电机前进
	Right_moto_go;		//右电机前进
}

void delay(unsigned int k)//延时函数，可用于使小车在一段时间内保持一种运动状态 
{    
    unsigned int x,y;
	for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++); //通过一个循环实现延时
}


void main(void)
{
	//如果用了前面的调速函数，就需要以下几行代码设置时间中断函数的时间 
	TMOD=0X01;
	TH0= 0XF8;//1ms定时
 	TL0= 0X30;
	TR0= 1;
	ET0= 1;
	EA = 1;

	while(1)
	{ 	
	  run(3);
	  delay(50);
	  run(6);
	  delay(50);
	  run(10);
	  delay(50);
	  run(6);
	  delay(50);
	 }
}
	
