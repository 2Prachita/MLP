#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<math.h>

#define OP 1
#define ATT 10
#define CASE_train 94
#define CASE_test 22
#define Class 2
#define EPOCH 645
#define bias 1

float normalise (float x, float minn, float maxx, float a)
{
	return((x-a)/(maxx-minn));
}
float activation (float x)
{
	float ret;
	ret= 1.0/(1.0 + +exp(-x));
	if(x > 0 )
	{
		ret = 1.0;
	}
	else
	{
		ret= -1.0;
	}
	return(ret);
}

float delta(float o)
{
	float ret;
	ret = o * (1.0 - o);
	return(o);
}

int main()
{
	float test_data[CASE_test][ATT];
	float train_data[CASE_train][ATT];
	float out_d_test[CASE_test][Class],out_d_train[CASE_train][Class];
	
	FILE *fp,*out;		
	char name[10];
	float c;
	
	fp = fopen("train.txt","r");								//READING DATA TRAINING
	
	if(fp==NULL)
	{
		printf("ERROR READING INPUT FILE\n");
		return(1);
	}

	for(int i=0; i<CASE_train; i++)
	{
		for(int j = 0; j<ATT + 1; j++)
		{
			if(j == 0)
			{
				train_data[i][j] = 1.0;
			}
			else if(j < ATT )
			{	
				fscanf(fp,"%f",&c);
				train_data[i][j]=c;
			}
			else
			{
				fscanf(fp,"%f",&c);
				if(c == 1)
				{
					out_d_train[i][0]=0;
					out_d_train[i][1]=0;
					out_d_train[i][2]=1;
				}
				else if(c == 2)
				{
					out_d_train[i][0]=0;
					out_d_train[i][1]=1;
					out_d_train[i][2]=0;
				}
				else if(c == 3)
				{
					out_d_train[i][0]=1;
					out_d_train[i][1]=0;
					out_d_train[i][2]=0;
				}
			}
		}
	}
	
	if(!(feof(fp)))
	{
		fclose(fp);
	}
							
	fp = fopen("test.txt","r");									//TESTING DATA READ
	
	if(fp==NULL)
	{
		printf("ERROR READING INPUT FILE\n");
		return(1);
	}

	for(int i=0; i<CASE_test; i++)
	{
		for(int j = 0; j<ATT + 1; j++)
		{
			if(j == 0)
			{
				test_data[i][j] = 1.0;
			}
			else if(j < ATT )
			{	
				fscanf(fp,"%f",&c);
				test_data[i][j]=c;	
			}
			else
			{
				fscanf(fp,"%f",&c);
				if(c == 1)
				{
					out_d_test[i][0]=0;
					out_d_test[i][1]=0;
					out_d_test[i][2]=1;
				}
				else if(c == 2)
				{
					out_d_test[i][0]=0;
					out_d_test[i][1]=1;
					out_d_test[i][2]=0;
				}
				else if(c == 3)
				{
					out_d_test[i][0]=1;
					out_d_test[i][1]=0;
					out_d_test[i][2]=0;
				}
			}
		}
	}
	
	if(!(feof(fp)))
	{
		fclose(fp);
	}
	
							
	float test[CASE_test][ATT], train[CASE_train][ATT];					//NORMALISING DATA
	float min=10000.0, max=0.0, avg=0.0;
	
	for(int i=1; i <ATT; i++)
	{
		for(int j = 0; j < CASE_train ; j++ )
		{
			avg=avg+train_data[j][i];
			if(train_data[j][i] > max)
			{
				max = train_data[j][i];
			}
			else if ( train_data[j][i] < min)
			{
				min = train_data[j][i];
			}
		}
		for(int j = 0; j < CASE_test; j++ )
		{
			avg=avg+test_data[j][i];
			if(test_data[j][i] > max)
			{
				max = test_data[j][i];
			}
			else if ( test_data[j][i] < min)
			{
				min = test_data[j][i];
			}
		}
		
		avg=avg/(CASE_test+CASE_train);

		for(int j = 0; j < CASE_train; j++)
		{
			train[j][i] = normalise(train_data[j][i] , min, max, avg);
		}

		for(int j = 0; j < CASE_test; j++)
		{
			test[j][i] = normalise(test_data[j][i] , min, max, avg);
		}
	}
	
	
	for(int i = 0 ; i < CASE_train; i++)
	{
		for(int j = 0; j < ATT ; j++)
		{
			if (j==0)
			{
				train[i][j] = 1 ;
			}
			else if(train[i][j] > 0 )
			{
				train[i][j] = 1 ;
			}
			else
			{
				train[i][j] = -1 ;
			}
		}
	}
	
	for(int i = 0; i < CASE_test; i++)
	{
		for(int j = 0 ; j < ATT; j++)
		{
			if (j==0)
			{
				test[i][j] = 1 ;
			}
			else if(test[i][j] > 0 )
			{
				test[i][j] = 1 ;
			}
			else
			{
				test[i][j] = -1 ;
			}
		}
	}
		
		
		
		////TRAINING
		
		
		
	int HL1 = ATT-1 ;
	int OL = Class ;												
	float WHL[HL1][ATT], WOL[OL][HL1+1] ;
	float HL1_Out [ HL1 + 1],OL_Out[OL],v=0;
	float EHL[HL1], EOL[OL];
	float Learning = 0.0245;
	float Eavg=0.0;
	
	
	
		//INTIALISING WTS
	
	out = fopen("traning_output.txt","w");
	fp = fopen ("graph.txt","w");
	
	for(int z = 0; z < HL1; z++)
	{
		for(int y= 0; y < ATT; y++)
		{
			WHL[z][y] = ((rand()%10)/10); 	
		} 
		EHL[z]=0.0;
	}
	for(int z = 0; z < OL; z++)
	{
		for(int y= 0; y < HL1+1 ; y++)
		{
			WOL[z][y] = ((rand()%10))/10; 
		}
		EOL[z]=0.0;
	}
	
	for(int a = 0; a < EPOCH; a++ )
	{
		for(int i = 0 ; i< CASE_train; i++)
		{
			
			
			//FORWARD PROPAGATION
			
			
			for(int x = 0; x < HL1; x++)
			{
				for (int j = 0 ; j < ATT; j++)
				{
					v = v + train[i][j] * WHL [x][j];
					
				}
				HL1_Out[x+1] = activation(v);
				v=0;
			}
		
			HL1_Out[0] = bias;
			v=0;
			
			for(int x = 0; x < OL; x++)
			{
				for (int j = 0 ; j < HL1+1; j++)
				{
					v = v + HL1_Out[j] * WOL [x][j]; 
				}
				OL_Out[x] = activation(v);
				v=0;
			}
		
				
			//BACK PROPAGATION ALGORITHM
			
			
			for(int x = 0; x < OL; x++)												//CALCULATING ERROR
			{
				EOL[x] = (	out_d_train[CASE_train][x] - OL_Out[x] ) * (	delta (OL_Out[x]));
			}	
		
			for(int x = 1; x < HL1 ; x++)
			{
				for(int j = 0; j < OL; j++)
				{
					EHL[x-1] += ( EOL[j] * WOL[j][x] )*(	delta (OL_Out[j]));	
				}
				
			}
			
			
			for(int x = 0; x < OL; x++)								//UPDATING WTS
			{
				for (int j = 0 ; j < HL1+1; j++)
				{
					WOL [x][j] = WOL [x][j] + Learning * EOL[x] * HL1_Out[j] ; 
				}
			}													
			for(int x = 0; x < HL1 +1; x++)
			{
				for (int j = 0 ; j < ATT; j++)
				{
					WHL [x][j] = WHL [x][j] + Learning * EHL[x] * train[i][j];
				}
			}
			float e1=0.0,e2=0.0,e3=0.0;
			e1 = (1.0/2.0)*(pow(OL_Out[0],2));
			e2 = (1.0/2.0)*(pow(OL_Out[1],2));
			e3 = (1.0/2.0)*(pow(OL_Out[2],2));
			Eavg = Eavg +e1+e2+e3;	
		}
		Eavg = Eavg /(float)(CASE_train);
		fprintf(out,"EPOCH  %d :   ",a+1);
		fprintf(out," Learning Rate : %f   ",Learning);
		fprintf(out," Error Avg : %f ", Eavg);
		fprintf(out,"\n");
		
		fprintf(fp,"%d\t",a+1);
		fprintf(fp,"%f\t",Eavg);
		fprintf(fp,"\n");
	}
	fclose(fp);
	fclose(out);
	
	float count= 0;
	out = fopen("testing_output.txt","w");
	for(int i =0; i < CASE_test; i++)								//TESTING
	{
		int flag = 0;
		for(int x = 0; x < HL1; x++)
		{
			for (int j = 0 ; j < ATT; j++)
			{
				v = v + train[i][j] * WHL [x][j];			
			}
			HL1_Out[x+1] = activation(v);
			v=0;
		}
		HL1_Out[0] = bias;
		v=0;
			
		for(int x = 0; x < OL; x++)
		{
			for (int j = 0 ; j < HL1+1; j++)
			{
				v = v + HL1_Out[j] * WOL [x][j]; 
			}
			OL_Out[x] = activation(v);
			v=0;
		}
		
		for(int x = 0; x < Class ; x++)
		{
			if(OL_Out[x] == out_d_test[i][x] )
			{
				flag=0;
			}
			else
			{
				flag=1;
			}
		}
		fprintf(out,"TEST : %d\n\n", i);
		fprintf(out,"EXPECTED : %f %f %f	OUTPUT : %f %f %f",OL_Out[0],OL_Out[1],OL_Out[2],out_d_test[i][0],out_d_test[i][1],out_d_test[i][2]);
		if(flag==0)
		{
			fprintf(out,"   ------------  MATCH !   -----------\n\n");
			count ++;
		}
		else if(flag == 1)
		{
			fprintf(out,"\n\n   ----------   NOT A MATCH !  --------------\n\n");
		}	
	}
	fprintf(out,"\n\n");
	fprintf(out,"\t\t\t\t\t\tACCURACY : %f ",count/CASE_test);
	fclose(out);
	return(0);
}
