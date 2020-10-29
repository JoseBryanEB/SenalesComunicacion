#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#define M_PI 3.14159
int FFT(float *xr,float *xi,int N,int inverse);
void SWAP(float *xp, float *yp) 
{ 
    float temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 
typedef struct complejo{
float real;
float imaginario;} complex;
//void IDFT (complex *in, complex*out,int n);
int main(int argc,char *argv[]){
    if (argc!=3){perror("Exit reuturn: ");return 0;}
FILE *inputf,*outputf;
inputf=fopen(argv[1],"rb");
outputf=fopen(argv[2],"wb");
if (inputf==NULL || outputf==NULL)return -1;
unsigned char header[44];
int flagread=fread(header,1,44,inputf);
fwrite(header,1,44,outputf);

if (flagread<44)return -1;

char wavfile[5]={header[8], header[9],header[10],header[11],'\0'};
if (strcmp(wavfile,"WAVE")!=0)return -1;

int bytesSample=header[34]/8;
unsigned int sizeRead=0;
for (int i=40,c=0;i<44;i++,c++){
         sizeRead|=header[i]<<(8*c);
        }
sizeRead/=bytesSample;
sizeRead/=2;
switch (bytesSample)
{
case 1:{
    float *muestras1=malloc(sizeRead*sizeof(float)),
    *muestras2=malloc(sizeRead*sizeof(float));
    
    // lectura de datps
    for  (int i=0 ;i<sizeRead;i++){
		int c1=0x0;
        c1=fgetc(inputf);
        muestras1[i]=(c1-128)/128.0;
	c1=fgetc(inputf);
	muestras2[i]=(c1-128)/128.0;
    } 
    /*
    Convolucion de 1d a 1d en un solo espacio X[n]
    */ 
   // printf("resultados\n");
    // complex *out=malloc(sizeRead*sizeof(complex));
    //DFT
    FFT(muestras1,muestras2,sizeRead,1);
    
    for (int i=0;i<sizeRead;i++){
        fputc((char)((muestras1[i])*128)+128,outputf);
        fputc((char)((muestras2[i])*128)+128,outputf);
      // printf("%f\n",out[i].real);   
    }


    }


    break;  
case 2:{
    //lectura de datos
    //printf("16 bits");
    float  *muestras2=malloc(sizeRead*sizeof(float)),
    *muestrasI=malloc(sizeRead*sizeof(float));
    for  (int i=0 ;i<sizeRead;i++){
		short valor=0;
		unsigned char c1=0x00,c0=fgetc(inputf);
        c1=fgetc(inputf);
		valor=c0|c1<<8;
        muestras2[i]=valor/32767.0;
	c0=fgetc(inputf);
	c1=fgetc(inputf);
		valor=c0|c1<<8;
	muestrasI[i]=valor/32767.0;

    }
    //DFT
    //complex out[sizeRead];
    FFT(muestras2,muestrasI,sizeRead,1);
   
    for (int i=0;i<sizeRead;i++){
        unsigned int aux=(muestras2[i]*32767.0);
		unsigned char signal[2]={(unsigned char)aux,aux>>8};
        fwrite(signal,1,2,outputf);
         aux=(muestrasI[i]*32767.0);
		unsigned char signal2[2]={(unsigned char)aux,aux>>8};
        fwrite(signal2,1,2,outputf);
        //printf("salida : %d\n",aux); 

    }

}
    break; 
default:
    break;
}
/*
 while (!feof(inputf)){
		fputc(fgetc(inputf),outputf);
		}*/
fclose(inputf);
fclose(outputf);
return 0;

}

int FFT(float *xr,float *xi,int N,int inverse) {
	int i,j,k,j1,m,n;
	float arg,s,c,w,tempr,tempi;
	m=log((float)N)/log(2.0);
	//bit reversal,intercambiando los datos
	for (i=0; i<N;++i){
		j=0;
		for (k=0;k<m;++k)
			j=(j<<1)| (1 & (i>>k));
		if (j<i){
		SWAP(&xr[i],&xr[j]); SWAP(&xi[i],&xi[j]);	
			}
	}
	for (i=0;i<m;i++){
		n=w=pow(2.0,(float)i);
		w=M_PI/n;
		if (inverse) w=-w;
		k=0;
		while (k<N-1){
			for (j=0;j<n;j++){
			arg=-j*w; c=cos(arg); s=sin(arg);
			j1=k+j;
			tempr=xr[j1+n]*c-xi[j1+n]*s;
			tempi=xi[j1+n]*c+xr[j1+n]*s;
			xr[j1+n]=xr[j1]-tempr;
			xi[j1+n]=xi[j1]-tempi;
			xr[j1]=xr[j1]+tempr;
			xi[j1]=xi[j1]+tempi;
			}
			k+=2*n;
		}
	}
	for (i=0;i<N;i++){
		if (xi[i]>1.0)xi[i]=1;
		if (xr[i]>1.0)xr[i]=1;
		if (xi[i]<-1.0)xi[i]=-1;
		if (xr[i]<-1.0)xr[i]=-1;
	}	
	 
	return 1;
}
