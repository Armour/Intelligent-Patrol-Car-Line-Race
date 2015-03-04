#include<AT89x51.H>

#define Left_moto_pwm     P1_0	   //P1_0接驱动模块ENA(左电机) 
#define Right_moto_pwm    P1_1	   //P1_1接驱动模块ENB(右电机) 
#define Left_O_led        P2_0	   //P2_0接四路寻迹模块接口第一路	左外
#define Left_I_led        P2_1	   //P2_1接四路寻迹模块接口第二路	左内
#define Right_I_led       P2_2	   //P2_2接四路寻迹模块接口第三路	右内
#define Right_O_led       P2_3	   //P2_3接四路寻迹模块接口第四路	右外
#define Left_moto_go      {P3_3=0,P3_4=1;}	//P3_3,P3_4接IN1,IN2	当P3_3=0,P3_4=1时左电机前进
#define Left_moto_back    {P3_3=1,P3_4=0;}	//P3_3,P3_4接IN1,IN2	当P3_3=1,P3_4=0时左电机后退
#define Left_moto_stp     {P3_3=1,P3_4=1;}	//P3_3,P3_4接IN1,IN2	当P3_3=1,P3_4=1时左电机停转                 
#define Right_moto_go     {P3_5=0,P3_6=1;}	//P3_5,P3_6接IN3,IN4	当P3_5=0,P3_6=1时右电机前转
#define Right_moto_back   {P3_5=1,P3_6=0;}	//P3_5,P3_6接IN3,IN4	当P3_5=1,P3_6=0时右电机后退
#define Right_moto_stp    {P3_5=1,P3_6=1;}	//P3_5,P3_6接IN3,IN4	当P3_5=1,P3_6=1时右电机停转

unsigned char push_val_left =0;//左电机占空比,1为最慢,10是最快,通过这个值可以改变其速度
unsigned char push_val_right=0;//右电机占空比,1为最慢,10是最快,通过这个值可以改变其速度
unsigned char pwm_val_left  =0;
unsigned char pwm_val_right =0;
bit Left_moto_stop =1;
bit Right_moto_stop=1;
unsigned int time=0;

/************************************************************************************************/

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
	TL0=0X29;
	time++;
	pwm_val_left++;
	pwm_val_right++;
	pwm_out_left_moto();
	pwm_out_right_moto();
}
/************************************************************************************************/

void run(void)//前进函数
{
	push_val_left=2;	//调节左电机占空比
	push_val_right=2;	//调节右电机占空比
	Left_moto_go;		//左电机前进
	Right_moto_go;		//右电机前进
}
void runlow(void)//慢速前进函数
{
    push_val_left=1;
	push_val_right=1;
	Left_moto_go;		//左电机前进
	Right_moto_go;		//右电机前进
}
void left(void)//左转函数
{
    push_val_left=4;
	push_val_right=4;
	Left_moto_stp;		//左电机停走
	Right_moto_go;		//右电机继续
}
void right(void)//右转函数
{
    push_val_left=4;
	push_val_right=4;
	Left_moto_go;		//左电机继续
	Right_moto_stp;		//右电机停走
}
void leftmax(void)//快速左转函数
{
    push_val_left=7;
	push_val_right=7;
	Left_moto_stp;	 	//左电机停走
	Right_moto_go;		//右电机继续
}
void rightmax(void)//快速右转函数
{
    push_val_left=7;
	push_val_right=7;
	Left_moto_go;		//左电机继续
	Right_moto_stp;		//右电机停走
}
void delay(unsigned int k)//延时函数
{    
    unsigned int x,y;
	for(x=0;x<k;x++) 
	   for(y=0;y<2000;y++);
}

/************************************************************************************************/

void main(void)
{
	int i,flag,step = 0;

	TMOD=0X01;
	TH0= 0XF8;//1ms定时
 	TL0= 0X29;
	TR0= 1;
	ET0= 1;
	EA = 1;
	run();
	while(1)
	{ 
	   
		if(Left_O_led==1&&Right_O_led==1){					//无路口
			if(Left_I_led==1&&Right_I_led==1){run();}		//左右均未压线（车身正）	直走
			if(Left_I_led==0&&Right_I_led==1){left();}		//左压线（车身右歪）		左转
			if(Left_I_led==1&&Right_I_led==0){right();}		//右压线（车身左歪）		右转
			if(Left_I_led==0&&Right_I_led==0){runlow();}	//左右均压线（传感器没调好）慢速直走   
		}  
		
		if(Left_O_led==0||Right_O_led==0){
			flag=1;
			for(i=0;i<=100;i++)
				if(Left_O_led==1&&Right_O_led==1){flag=0;break;}//防止偶然干扰信号 
			if(flag){							//如果不是干扰信号
				step++;
				switch(step){
					case 3: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 4: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 5: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 7: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 15: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 21: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 22: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 24: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 25: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 26: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 28: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 34: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 35: 
						leftmax();						//左转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向

					case 8: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 9: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 10: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 14: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 16: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 17: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 20: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 23: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 27: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 29: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 30: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 31: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向
					case 36: 
						rightmax();						//右转
						delay(29);						//转向延时
						break; //TODO:在第2个路口走的方向

					default:	
						if(Left_I_led==1&&Right_I_led==1){run();}		//左右均未压线（车身正）	直走
						if(Left_I_led==0&&Right_I_led==1){left();}		//左压线（车身右歪）		左转
						if(Left_I_led==1&&Right_I_led==0){right();}		//右压线（车身左歪）		右转
						if(Left_I_led==0&&Right_I_led==0){runlow();}	//左右均压线（传感器没调好）慢速直走   				
						break; //TODO:在第1个路口走的方向
				};
			}
		}
	}
}
	