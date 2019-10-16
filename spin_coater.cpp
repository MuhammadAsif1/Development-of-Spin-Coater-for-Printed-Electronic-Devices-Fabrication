#include "mbed.h"
#include "Servo.h"

Serial pc(USBTX, USBRX);
BusOut data(p22,p23,p24,p25,p26,p27,p28,p29);
DigitalOut r1(p5);
DigitalOut r2(p6);
DigitalOut r3(p7);
DigitalOut r4(p8);
DigitalIn c1(p9);
DigitalIn c2(p10);
DigitalIn c3(p11);
DigitalIn c4(p12);
///////////////////PWM and sensor veriables
Servo myservo(p21);
InterruptIn sensor(p13); 
int rpm_counter=0;
int Sensor_value; ///////////Sensor value
PwmOut ledf(LED1);
PwmOut ledr(LED2);
DigitalOut ss(p18); /////red lower
DigitalOut start(p19);   ///blue upper
DigitalOut ramp(p20); /////center green
///////////////////
DigitalOut rs(p30);
DigitalOut en(p14);
  float Ramp_Fraction;  
 int dcount11,dcount12,dcount13,dcount14; ////////// for digit count
 int sdigits[10];
 int size;
 float factor = 0.05; ////// start duty cycle
 int new_reference = 0;
 int j=0;
 int d;  //////////d---->digit
 int p; //////////for conversion process
 int Ramp_RPM,Ramp_Time,SS_RPM,SS_Time;
 int Save_RPM;
 int Error;/////////////Error in feedbackloop
 int RampRPM_Array[3];
 int RampTime_Array[3];
 int SSRPM_Array[3];
 int SSTime_Array[3];
 float Matlabdata; //////
 Ticker timeup;
 Ticker over;
 void comandwrt()
 {
     rs=0;
     en=1;
     wait(0.0023);
     en=0;
     }
     void datawrt()
     {
         rs=1;
         en=1;
         wait(0.0023);
         en=0;
         }
         //////////////////////////////////////////////
         //////////////////////////////////////////////
         int keypadN()
         {
        
          int value;
          if(c1==0)
          {
              r1=1; r2=0; r3=0; r4=0;
                 
              //wait(0.001);
              if(c1==1)
              {
                       value=49;
                       ///1 is pressed
              }
              //////////////
              r1=0; r2=1; r3=0; r4=0;
                 
              //wait(0.001);
              if(c1==1)
              {
                       value=52;
                       ///4 is pressed
              }
              ///////////////r=3/
              r1=0; r2=0; r3=1; r4=0;
                 
              //wait(0.001);
              if(c1==1)
              {
                       value=55;
                       ///7 is pressed
              }
              ///////////////r=4
              r1=0; r2=0; r3=0; r4=1;
                 
              ///wait(0.0000001);
              if(c1==1)
              {
                       value=49;
                       ///* is pressed
              }
              
            }
            
                  

                 
             ///////////////////////////////////////// for c2
            else if(c2==0)
          {
              r1=1; r2=0; r3=0; r4=0;
                 
              //wait(0.001);
              if(c2==1)
              {
                       value=50;
                       ///2is pressed
              }
              //////////////
              r1=0; r2=1; r3=0; r4=0;
                 
              //wait(0.001);
               if(c2==1)
              {
                       value=53;
                       ///5 s pressed
              }
              ///////////////r=3/
              r1=0; r2=0; r3=1; r4=0;
                 
              //wait(0.001);
              if(c2==1)
              {
                       value=56;
                       ///8 s pressed
              }
              ///////////////r=4
              r1=0; r2=0; r3=0; r4=1;
                 
              //wait(0.001);
              if(c2==1)
              {
                       value=50;
                       ///0 is pressed
              }
              
            }
            ////////////////////////////////////////for c=3
            else if(c3==0)
          {
              r1=1; r2=0; r3=0; r4=0;
                 
              //wait(0.001);
              if(c3==1)
              {
                       value=51;
                       ///3 is pressed
              }
              //////////////
              r1=0; r2=1; r3=0; r4=0;
                 
              //wait(0.001);
              if(c3==1)
              {
                       value=54;
                       ///6 s pressed
              }
              ///////////////r=3/
              r1=0; r2=0; r3=1; r4=0;
                 
              //wait(0.001);
              if(c3==1)
              {
                       value=57;
                       ///9 s pressed
              }
              ///////////////r=4
              r1=0; r2=0; r3=0; r4=1;
                 
              //wait(0.001);
              if(c3==1)
              {
                       value=51;
                       ///Actually key in keypad is # but i assign ascii of A
              }
              
            }
                        
        
            
             ////////////////////
/////////////////////////////////////////c4
         else if(c4==0)
          {
              r1=1; r2=0; r3=0; r4=0;
                 
              //wait(0.001);
              if(c4==1)
              {
                       value=65;
                       ///A is pressed
              }
              //////////////
              r1=0; r2=1; r3=0; r4=0;
                 
              //wait(0.001);
              if(c4==1)
              {
                       value=48;
                       ///B s pressed
              }
              ///////////////r=3/
              r1=0; r2=0; r3=1; r4=0;
                 
              //wait(0.001);
              if(c4==1)
              {
                       value=48;
                       ///0 is pressed
              }
              ///////////////r=4
              r1=0; r2=0; r3=0; r4=1;
                 
              //wait(0.001);
              if(c4==1)
              {
                       value=65;
                       ///Actually key in keypad is * but i assign ascii of A
              }
              
            }

////////////////////////////////////////////               
    
              return value;
             
     }
     ////////////////////////////////////
     int Converter(int dcount)           //////////////////Ascii to decimal conversion
     {
         
         if(data==48)
         d=0;
         if(data==49)
         d=1;
         if(data==50)
         d=2;
         if(data==51)
         d=3;
         if(data==52)
         d=4;
         if(data==53)
         d=5;
         if(data==54)
         d=6;
         if(data==55)
         d=7;
         if(data==56)
         d=8;
         if(data==57)
         d=9;
         
         if(dcount==0)
          p=d;              ////////e.g  9
         if(dcount==1)
         {  
            p=p*10;         ////////for second digit   p=9x10=90,  90+2=92   here i pick 2 as example
            p=p+d;
          }
         if(dcount==2)
         {  
            p=p*10;     ///////////so on for others
            p=p+d;
          }
         
         if(dcount==3)
         {  
            p=p*10;
            p=p+d;
          }
          return p;   ////////////return decimal digit
    }////////end of function
    ////////////////////////
    void SSTime_Display()
    {
             
             data='S';
             datawrt();
             data='S';
             datawrt();
             data=' ';
             datawrt();
             data='T';
             datawrt();
             data='i';
             datawrt();
             data='m';
             datawrt();
             data='e';
             datawrt();
             data=' ';
             datawrt();
             data=':';
             datawrt();
             data=' ';
             datawrt();
     
     }
     ////////////////////
     void SSTime()
     {       p=0;
             d=0;
             dcount14=0;
             data=0X94;
             comandwrt();
             SSTime_Display();
             data=0;
             while(data!=65)
             {
                 if(c1==0 | c2==0 | c3==0 | c4==0)
                 {
                     data=keypadN();
                     if(data==65)
                     {
                         wait(0.5);
                         break;
                         }
                         else
                         {
                            datawrt();
                            wait(0.5);
                            SSTime_Array[dcount14]=data;
                            SS_Time=Converter(dcount14);
                            dcount14++;
                            }
                     }////end of if condition
                     
             }/////end of while
      }
      ////////////////////////////
      void SSRPM_Display()
      {
             data=0XC0;
             comandwrt();
             
             data='S';
             datawrt();
             data='S';
             datawrt();
             data=' ';
             datawrt();
             data='R';
             datawrt();
             data='P';
             datawrt();
             data='M';
             datawrt();
             data=' ';
             datawrt();
             data=':';
             datawrt();
             data=' ';
             datawrt();
         }
         ////////////////////////
         
       void SSData()
        {
             data=' ';
             datawrt();
             data=' ';
             datawrt();
             data=' ';
             datawrt();
             data=' ';
             datawrt();
             data='S';
             datawrt();
             data='S';
             datawrt();
            
             datawrt();
             data=' ';
             datawrt();
             data='D';
             datawrt();
             data='a';
             datawrt();
             data='t';
             datawrt();
             data='a';
             datawrt();
            }
     void SSRPM()
     {
             p=0;
             d=0;
             dcount13=0;
             
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X80;
             comandwrt();
             SSData();
             SSRPM_Display();
             
             data=0;
             while(data!=65)
             {
                 if(c1==0 | c2==0 | c3==0 | c4==0)
                 {
                     data=keypadN();
                     if(data!=65)
                     {
                        datawrt();
                        wait(0.5);
                        SSRPM_Array[dcount13]=data; //////////to save ascii of a digit in array
                        SS_RPM=Converter(dcount13); ///////ascii to decimal conversion
                        dcount13++; ///////////////digit counter
                        }
                      else
                     {
                        wait(0.5);
                        SSTime();
                        }
                     
                  }////end of if condition
                     
             }/////end of while
      }
      ///////////////////////////////
      void RampTime_Display()
      {
             data=0X94; //////must be on third line
             comandwrt();
             
             data='R';
             datawrt();
             data='a';
             datawrt();
             data='m';
             datawrt();
             data='p';
             datawrt();
             data=' ';
             datawrt();
             data='T';
             datawrt();
             data='i';
             datawrt();
             data='m';
             datawrt();
             data='e';
             datawrt();
             data=' ';
             datawrt();
             data=':';
             datawrt();
             data=' ';
             datawrt();
             
             }
             /////////////////////////////////////////////////
     void RampTime()
     {       
             p=0;
             d=0;
             dcount12=0;
             RampTime_Display();
             
             data=0;
             while(data!=65)
             {
                 if(c1==0 | c2==0 | c3==0 | c4==0)
                 {
                     data=keypadN();
                     if(data!=65)
                     {
                        datawrt();
                        wait(0.5);
                        RampTime_Array[dcount12]=data;
                        Ramp_Time=Converter(dcount12);
                        dcount12++;
                        }
                      else
                     {
                         wait(0.5);
                         SSRPM();  /////////after returning from this function system will check 65(while given above)
                         }
                     }////end of if condition
                     
             }/////end of while
      }
      ////////////////////////////////////////////////
      void Rampdata()/////Ramp data heading
      {
             data=' ';
             datawrt();
             data=' ';
             datawrt();
             data=' ';
             datawrt();
             data='R';
             datawrt();
             data='A';
             datawrt();
             data='M';
             datawrt();
             data='P';
             datawrt();
             data=' ';
             datawrt();
             data='D';
             datawrt();
             data='a';
             datawrt();
             data='t';
             datawrt();
             data='a';
             datawrt();
             
       
       }
       void RampRPM_Display()
      {      
             data=0XC0;
             comandwrt();
             data='R';
             datawrt();
             data='a';
             datawrt();
             data='m';
             datawrt();
             data='p';
             datawrt();
             data=' ';

             data='R';
             datawrt();
             data='P';
             datawrt();
             data='M';
             datawrt();
             data=' ';
             datawrt();
             data=':';
             datawrt();
             data=' ';
             datawrt();
          }
      ///////////////////////////////////////////////
             
     void RampRPM()
     {
             d=0; /////no digit at start
             p=0; ////// for conversion process
             dcount11=0;
            
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X80;
             comandwrt();
             Rampdata();
            
             RampRPM_Display();
             data=0;
             
             while(data !=65)
             {
                 if(c1==0 | c2==0 | c3==0 | c4==0)
                 {
                     data=keypadN();
                     if(data!=65)
                     {
                        datawrt();
                        wait(0.4);
                        RampRPM_Array[dcount11]=data; //////////////To save ascii RampRPM ascii
                        Ramp_RPM=Converter(dcount11); /////////Ascii to decimal digit converter function
                     
                        dcount11=dcount11+1;
                        }
                         else
                        {
                          wait(0.5);
                          RampTime();
                          }
                     
                 }////end of if condition
                     
                 }/////end of while
                 
      }
      ///////////////////////////
     
          ///////////////////////////////
          ////////////////////////////////
      void spincoater()
      {
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X86;
             comandwrt();
             
             
             data='W';
             datawrt();   //////////////Wellcome
             data='E';
             datawrt();
             data='L';
             datawrt();
             data='L';
             datawrt();
             data='C';
             datawrt();
             data='O';
             datawrt();
             data='M';
             datawrt();
             data='E';
             datawrt();
             
             data=0XC4;          //Next 
             comandwrt();
        

             data='S';
             datawrt();
             data='P';
             datawrt();
             data='I';
             datawrt();
             data='N';
             datawrt();
             data=' ';
             datawrt();
             data='C';
             datawrt();
             data='O';
             datawrt();
             data='A';
             datawrt();
             data='T';
             datawrt();
             data='E';
             datawrt();
             data='R';
             datawrt();
             
             data=0XD6;
             comandwrt();
             data='A';
             datawrt();
             data='-';
             datawrt();
             data='-';
             datawrt();
             data='-';
             datawrt();
             data='-';
             datawrt();
             data='>';
             datawrt();
             data='F';
             datawrt();
             data='o';
             datawrt();
             data='r';
             datawrt();
             data=' ';
             datawrt();
             data='M';
             datawrt();
             data='e';
             datawrt();
             data='n';
             datawrt();
             data='u';
             datawrt();
             

          }
          int DecimalToAscii(int x)
          {
              int y;
              if(x==0)
              y=48;
              if(x==1)
              y=49;
              if(x==2)
              y=50;
              if(x==3)
              y=51;
              if(x==4)
              y=52;
              if(x==5)
              y=53;
              if(x==6)
              y=54;
              if(x==7)
              y=55;
              if(x==8)
              y=56;
              if(x==9)
              y=57;
              
              return y;
              }////////////end of function
              
              ///////////////////////////////
              void Ramp_Stage()
              {
                  data='R';
             datawrt();
             data='a';
             datawrt();
             data='m';
             datawrt();
             data='p';
             datawrt();
             data=' ';
             datawrt();
             data='S';
             datawrt();
             data='t';
             datawrt();
             data='a';
             datawrt();
             data='g';
             datawrt();
             data='e';
             datawrt();
                  
          }
          
          /////////////////////////////////////
          void SS_Stage()
              {
                  data='S';
             datawrt();
             data='S';
             datawrt();
             data=' ';
             datawrt();
             data='S';
             datawrt();
             data='t';
             datawrt();
             data='a';
             datawrt();
             data='g';
             datawrt();
             data='e';
             datawrt();
                  
          }
       ///////////////////////////////
              /////////////////////////////////////////////////////
     void RunTimeDisplay()           //////////////////////IS Function man masla ha
       {
        data = 0;
        }
 //////////////////////////////////////////////
 /////////////////////////////////////////////
                          
/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
        void RunTimeDisplay2()           //////////////////////IS Function man masla ha
       {
             int x,y,i=0;
             int Time1_Array[3];
             //float Ramp_Fraction;
             ///Ramp_Fraction=Ramp_RPM/Ramp_Time; /////////////RPM increment veriable
             
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X85;
             comandwrt();
             SS_Stage();
             
             data=0XC0;
             comandwrt();
             SSRPM_Display();
             for(int t=0; t<dcount13; t++)
             {
                 
                   data=SSRPM_Array[t];
                   datawrt();
                }
             data=0;
             while(SS_Time>=0)
             {
                 
             ///PWM1=Ramp_Fraction/1000; //////////////////Duty Cycle
             i=0;
             y=SS_Time;
             while(data!=65) //////////// complex logic to get out from this loop
               {
                   x=y%10;
                   y=y/10;      ////Conversion from decimal number to separte digits
                   Time1_Array[i]=DecimalToAscii(x);
                   i++;
                   if(y<10)
                   {
                       Time1_Array[i]=DecimalToAscii(y);
                       data=65;
                       }
              } ///////////////////end of while
              
              
             data=0X94;
             comandwrt();
             SSTime_Display();
              
              for(int j=i; j>-1; j--)
              {
                      data=Time1_Array[j];   ////////Display of characters
                      datawrt();
              }
              
              SS_Time=SS_Time-1;
              wait(1);        
            } ///////////////Ramp_Time  end loop 
             wait(1);/////////////Faz Khate man
             data=65;///////// to get exit path from while loop in main 
                                      
        }////////End of RunTimeDisplay
        /////////////////////////////////////////////////////
        ////////////////////////////////////////////////////

  void Process_Complete()
  {
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0XC6;
             comandwrt();
             
             data='P';
             datawrt();
             data='r';
             datawrt();
             data='o';
             datawrt();
             data='c';
             datawrt();
             data='e';
             datawrt();
             data='s';
             datawrt();
             data='s';
             datawrt();
             
             data=0X9A;
             comandwrt();
    
             data='C';
             datawrt();
             data='o';
             datawrt();
             data='m';
             datawrt();
             data='p';
             datawrt();
             data='l';
             datawrt();
             data='e';
             datawrt();
             data='t';
             datawrt();
             data='e';
             datawrt();
             wait(3);
             data=65;
             
     } ////////////End of process complete
             ////////////////////////////////////
             ////////////ISR
            void RampCalculations()
             {
                    int local_rpm=0;
                    factor = 0.08;
                    myservo = factor;
                    j = 0;
                    new_reference = new_reference + Ramp_Fraction; ////////////
                    while (j == 0) 
                        {
                            wait(0.3);
                            local_rpm = rpm_counter;
                            if(((local_rpm*60/(2*0.3)) - (local_rpm*70)) < new_reference)
                                {
                                    factor = factor + (0.001*Ramp_Fraction/90);
                                    myservo = factor;
                                 }
                             ////////////////////
                              wait(0.3);
                              local_rpm = rpm_counter;
                              if(((local_rpm*60/(2*0.6)) - (local_rpm*20)) < new_reference)
                                   {
                                      factor = factor + (0.001*Ramp_Fraction/90);
                                      myservo = factor;
                                    }
                                  ////////////////////////
                                  wait(0.3);
                                  local_rpm = rpm_counter;
                                  if(((local_rpm*60/(2*0.9)) - (local_rpm*3.333)) < new_reference)
                                  {
                                     factor = factor + (0.001*Ramp_Fraction/90);
                                     myservo = factor;
                                   }
                                     wait(0.06);
          
                     } //////end of while loop
          factor = factor - 0.02;
          myservo = factor;
         }////////end of function loop
         //////////////////////////
         //////////////////////////
 void rpm()
     {
         rpm_counter++;
         
     }
     //////////////////////////////////
     /////////////////////////////////
     void Digits_Display()
 {           
     
     for(int j=size; j>=0; j--)
     {
         data=sdigits[j];
         datawrt();
         }
    }//////////////end of function
     
     /*int DecimalToAscii(int x)
          {
              int y;
              if(x==0)
              y=48;
              if(x==1)
              y=49;
              if(x==2)
              y=50;
              if(x==3)
              y=51;
              if(x==4)
              y=52;
              if(x==5)
              y=53;
              if(x==6)
              y=54;
              if(x==7)
              y=55;
              if(x==8)
              y=56;
              if(x==9)
              y=57;
              
              return y;
     }*/ ////////////end of function
     ///////////////////////////////////
    void separate_digits(int y)
     {
        size=0;
        do{
         sdigits[size]=DecimalToAscii(y%10);
         y=y/10;
         size++;
         }while(y!=0);
         size=size-1; ///////due to one extra count
     }
     ///////////////////////////////////////////////////////////////////////////
     ///////////////////////////////////////////////////////////////////////////
     void display()
    {
             Matlabdata = (rpm_counter*60)/2;
             printf("%f\n", Matlabdata);
             Save_RPM = Matlabdata; 
            if(Ramp_Time == 0)
             {
                 
                  j = 1;
                  ledr = 1;
                  }
                  
            else
            {
                new_reference = new_reference + Ramp_Fraction;/////if previous is 2100 next will be 2100 + increment
             }
             
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X85;
             comandwrt();
              
            Ramp_Stage();
             
            separate_digits((rpm_counter*60)/2); /////because we are taking 1 after one second reading
            RampRPM_Display();
            data=0XCD;
            comandwrt();
            Digits_Display();
            
            RampTime_Display();
            separate_digits(Ramp_Time);
            Digits_Display();
            //////////////////////13th
              
            rpm_counter=0;
            Ramp_Time = Ramp_Time -1;
            
        }
        /////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////
        void display1()
       {
             
             if(SS_Time == 0)
             {
                 
                  j = 1;
                  
                }
             ////////////////////
             Matlabdata = (rpm_counter*60)/2;
             printf("%f\n", Matlabdata);
             ///////////////////    
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X85;
             comandwrt();
              
             SS_Stage();
            
            separate_digits((rpm_counter*60)/2); /////because we are taking 1 after one second reading
            
            SSRPM_Display();
            data=0XCA;
            comandwrt();
            Digits_Display();
            data=0X94; //////must be on third line
            comandwrt();
            SSTime_Display();
            separate_digits(SS_Time);
            Digits_Display();
            
            
            Sensor_value = (rpm_counter*60)/2;
            Error = SS_RPM - Sensor_value;
            if(Error != 0)
            {
                factor = factor + (Error*0.00001);
                //factor = (SS_RPM*0.00002677419);
                }
                    else
                    {
                        factor = factor;
                        }
                        
                        myservo = factor;
                        ss = 1;
            /////////////////////
            rpm_counter=0;
            SS_Time = SS_Time - 1;
             
        }
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void system_settings()
{
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X85;
             comandwrt();
             data='W';
             datawrt();
             data='a';
             datawrt();
             data='i';
             datawrt();
             data='t';
             datawrt();
             data='.';
             datawrt();
             data='.';
             datawrt();
             data='.';
             datawrt();
             
             data=0XC0;
             comandwrt();
             
             data='H';
             datawrt();
             data='i';
             datawrt();
             data=',';
             datawrt();
             data=' ';
             datawrt();
             data='w';
             datawrt();
             data='e';
             datawrt();
             data=' ';
             datawrt();
             data='a';
             datawrt();
             data='r';
             datawrt();
             data='e';
             datawrt();
             data=' ';
             datawrt();
             
             data=0X94; //////must be on third line
             comandwrt();
             
             data='g';
             datawrt();
             data='e';
             datawrt();
             data='t';
             datawrt();
             data='t';
             datawrt();
             data='i';
             datawrt();
             data='n';
             datawrt();
             data='g';
             datawrt();
             data=' ';
             datawrt();
             data='t';
             datawrt();
             data='h';
             datawrt();
             data='i';
             datawrt();
             data='n';
             datawrt();
             data='g';
             datawrt();
             data='s';
             datawrt();
             
             data=0XD8;
             comandwrt();
             
             data=' ';
             datawrt();
             data='r';
             datawrt();
             data='e';
             datawrt();
             data='a';
             datawrt();
             data='d';
             datawrt();
             data='y';
             datawrt();
             //Hi, we're getting things ready for you
    }
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////         
int main() 
  {
    c1.mode(PullUp);
    c2.mode(PullUp);
    c3.mode(PullUp);
    c4.mode(PullUp);
    start = 1;
    ////////////////////////////
    system_settings();
    ////////////////////////////
    ///////ESC Settings////////////////
   /* myservo = 0.0;
    ledf = 1;
    wait(0.5); //detects signal
//Required ESC Calibration/Arming sequence  
//sends longest and shortest PWM pulse to learn and arm at power on
    myservo = 0.13; //send longest PWM
    ledf = ledr = 0;
    wait(8);*/
    myservo = 0.0; //send shortest PWM
    wait(8);
  
    
    while(1)
    {
             r1=0;
             r2=0;
             r3=0;
             r4=0;
             
             data=0X01;
             comandwrt();             
             data=0X0E;
             comandwrt();
             data=0X06;
             comandwrt();
             data=0X38;
             comandwrt();
             data=0X86;
             comandwrt();
             
             ///////////////
    
             data='E';
             datawrt();   //////////////Wellcome
             data='n';
             datawrt();
             data='t';
             datawrt();
             data='D';
             datawrt();
             data=' ';
             datawrt();
             data=':';
             datawrt();
             data=' ';
             datawrt();
             
            /* while(data!=65)
             {
             if(c1==0 | c2==0 | c3==0 | c4==0 )
              {
                  data=keypadN();
                  datawrt();
                  if(data==65)
                  break;
                  wait(0.5);
              }///end of keychecker if
            }*/
             
             
             spincoater();
             while(data!=65)
             {
              
                if(c1==0 | c2==0 | c3==0 | c4==0)
              {
                  data=keypadN();
                  wait(0.5);
                  if(data==65)
                  {
                     RampRPM();
                     wait(0.001);
                     timeup.attach(&display,1);
                     sensor.rise(&rpm);/////ISR
                     NVIC_SetPriority(EINT2_IRQn, 1);
                     //////////////////////////////////code from sspid section////
                     //////////////////////////////////////////////////
                     //SS_RPM = 5500;
                     //SS_Time = 30;    
                     data=0X01;
                     comandwrt();             
                     data=0X0E;
                     comandwrt();
                     data=0X06;
                     comandwrt();
                     data=0X38;
                     comandwrt();
                     data=0X85;
                     comandwrt();
                     Ramp_Stage();
             

                     //Ramp_RPM = 5000;
                     //Ramp_Time=10;
                     
                     start = 0; /////start led off
                     if(SS_RPM>6000)
                     SS_RPM = 6000;
                     
                     int scale = SS_RPM/30; ///////e.g 4000/30 = 133 instead of 133.33 
                     SS_RPM = scale*30; ///////133*30 = 3990
                     
                     if(Ramp_RPM>6000)
                     Ramp_RPM = 6000;  ////putting limit for max rpm
                     if(Ramp_RPM>=5700)
                     {
                         Ramp_RPM = Ramp_RPM + 30; ///////adding offset
                         }
                         if(Ramp_RPM>=5300 && Ramp_RPM<5700)
                         {
                             Ramp_RPM = Ramp_RPM + 100; //////adding offset
                         }

                     Ramp_Fraction = (Ramp_RPM-2800)/Ramp_Time; //////because motor starts from 2000 rpm
                     factor = 0.047;
                     myservo = factor;
                     ramp = 1;
                             
                     //int i=0.5;
                     int local_rpm=0;
                     j = 0;
                     new_reference = new_reference + Ramp_Fraction; ////////////
                     while (j == 0) 
                     {
                         wait(1);
                         local_rpm = rpm_counter;
                         if(Ramp_Time != 0)
                         {
                             if(((local_rpm*60/(2*0.3)) - (local_rpm*70)) < new_reference)
                            {
                                factor = factor + (0.00002677419*Ramp_Fraction); ///////0.00003677419
                                myservo = factor;
                             }
                         }
                    ////////////////////
                    /* wait(0.3);
                     local_rpm = rpm_counter;
                     if(((local_rpm*60/(2*0.6)) - (local_rpm*20)) < new_reference)
                         {
                            factor = factor + (0.00002677419*Ramp_Fraction/10);
                            myservo = factor;
                         }
                    ////////////////////////
                     wait(0.3);
                     local_rpm = rpm_counter;
                     if(((local_rpm*60/(2*0.9)) - (local_rpm*3.333)) < new_reference)
                       {
                          factor = factor + (0.00002677419*Ramp_Fraction/10);
                          myservo = factor;
                       }*/
                     //wait(0.08);
                     
          
                  }
                  ramp = 0; ////ramp led of
                  //////end of while
          // __disable_irq();
          //NVIC_DisableIRQ(TIMER2_IRQn);
                  timeup.detach();
                  //factor = factor - 0.002677419;
                  //myservo = factor;
          //////////////////////
                  over.attach(&display1,1);
                  ledr = 0;
                  j = 0;
                  rpm_counter = 0;
                  wait(SS_Time+1);
                  over.detach();
                  ledr = 1;
                  myservo = 0;
                     //RampCalculations(); ////
                     ///////////////////////////////////
                     /////////////////////////////////// 
                     //RunTimeDisplay2();
                     ss = 1;
                     start = 1;
                     ramp = 1;
                     Process_Complete();
                     rpm_counter = 0;
                     ss = 0;
                     ramp = 0;
                     start = 1;
                   }  
    
              }///end of keychecker if
             }  //end of internal while   
            }///////////end of while

}///end of main


