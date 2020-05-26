#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#define M_PI 3.14159
typedef struct complejo{
double real;
double imaginario;} complex;
void IDFT (complex *in, complex*out,int n);
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

switch (bytesSample)
{
case 1:{
    complex *muestras1=malloc(sizeRead*sizeof(complex));
    
    // lectura de datps
    for  (int i=0 ;i<sizeRead;i++){
		int c1=0x0;
        c1=fgetc(inputf);
        muestras1[i].real=(c1-128)/128.0;
	c1=fgetc(inputf);
	muestras1[i++].imaginario=(c1-128)/128.0;
    } 
    /*
    Convolucion de 1d a 1d en un solo espacio X[n]
    */ 
   // printf("resultados\n");
     complex *out=malloc(sizeRead*sizeof(complex));
    //DFT
    IDFT(muestras1,out,sizeRead);
    
    for (int i=0;i<sizeRead;i++){
        fputc((char)((out[i].real)*128)+128,outputf);
        fputc((char)((out[i].imaginario)*128)+128,outputf);
      // printf("%f\n",out[i].real);   
    }


    }


    break;  
case 2:{
    //lectura de datos
    //printf("16 bits");
    complex *muestras2=malloc(sizeRead*sizeof(complex));
    for  (int i=0 ;i<sizeRead;i++){
		short valor=0;
		unsigned char c1=0x00,c0=fgetc(inputf);
        c1=fgetc(inputf);
		valor=c0|c1<<8;
        muestras2[i].real=valor/32767.0;
	c0=fgetc(inputf);
	c1=fgetc(inputf);
		valor=c0|c1<<8;
	muestras2[i++].imaginario=valor/32767.0;

    }
    //DFT
    complex out[sizeRead];
    IDFT(muestras2,out,sizeRead);
   
    for (int i=0;i<sizeRead;i++){
        unsigned int aux=(out[i].real*32767.0);
		unsigned char signal[2]={(unsigned char)aux,aux>>8};
        fwrite(signal,1,2,outputf);
         aux=(out[i].imaginario*32767.0);
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
void IDFT (complex *in, complex *out, int n){
 for (int k = 0; k < n; k++) {  // For each output element
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < n; t++) {  // For each input element
            double angle = 2 * M_PI * t * k / n;
	    double c=cos(angle),s=sin(angle);
            sumreal += in[t].real*c-in[t].imaginario*s;
            sumimag += in[t].imaginario*c+in[t].real*s;
        }
        if (sumreal>1.0)out[k].real=1;
        else out[k].real= sumreal;
        if (sumimag>1.0)out[k].imaginario=1;
        else out[k].imaginario = sumimag;
    }
    

} 