/*
Programa de la tranformada discreta de fourier con archivos wav
Escrito  por: Estrada Bernal José Bryan de la Escuela Superior de Computo
para la clase de Señales con el profesor Eduardo Aldana.
Escrtito en: Visual Code, GCC 7.0
Fecha de Finalización: 20 de Mayo del 2020
*/ 

#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
//#define M_PI 3.14159
typedef struct complejo{
double real;
double imaginario;
} complex;
unsigned int obtenerNumero (unsigned char *header,int lowerLimit, int upperLimit);
void mandarNumero(unsigned char *header, int lowerLimit, int upperLimit,unsigned int num);
int DFT (double *x,complex *out,int n);

int main(int argc,char *argv[]){
    if (argc!=3){perror("Exit reuturn: ");return 0;}
FILE *inputf,*outputf;
inputf=fopen(argv[1],"rb");
outputf=fopen(argv[2],"wb");
if (inputf==NULL || outputf==NULL)return -1;
unsigned char header[44];
int flagread=fread(header,1,44,inputf);


if (flagread<44)return -1;

char wavfile[5]={header[8], header[9],header[10],header[11],'\0'};
if (strcmp(wavfile,"WAVE")!=0)return -1;

int bytesSample=header[34]/8;
unsigned int sizeRead=0;
for (int i=40,c=0;i<44;i++,c++){
         sizeRead|=header[i]<<(8*c);
        }
sizeRead/=bytesSample;
header[22]*=2;
//modificacion del byte 28 al 31 
unsigned int numero=0; 
numero = obtenerNumero(header,28,32);
numero*=2;
mandarNumero(header,28,32,numero);
numero=obtenerNumero(header,32,34);
numero*=2; 
mandarNumero(header,32,34,numero);
//modificación del sizechunk despues de "data"
numero = sizeRead*2*bytesSample;
mandarNumero(header,40,44,numero);
mandarNumero(header,4,8,numero+36);
fwrite(header,1,44,outputf);
switch (bytesSample)
{
case 1:{
    double *muestras1=malloc(sizeRead*sizeof(double));
    // lectura de datps
    for  (int i=0 ;i<sizeRead;i++){
		int c1=0x0;
        c1=fgetc(inputf);
        muestras1[i]=(c1-128)/128.0;
    } 
    /*
    Convolucion de 1d a 1d en un solo espacio X[n]
    */ 
    printf("resultados\n");
     complex *out=malloc(sizeRead*sizeof(complex));
    //DFT
    DFT(muestras1,out,sizeRead);
    
    for (int i=0;i<sizeRead;i++){
        fputc((char)((out[i].real)*128/sizeRead)+128,outputf);
        fputc((char)((out[i].imaginario)*128/sizeRead)+128,outputf);
      // printf("%f\n",out[i].real);   
    }


    }


    break;  
case 2:{
    //lectura de datos
    //printf("16 bits");
    double *muestras2=malloc (sizeRead*sizeof(double));
    for  (int i=0 ;i<sizeRead;i++){
		short valor=0;
		unsigned char c1=0x00,c0=fgetc(inputf);
        c1=fgetc(inputf);
		valor=c0|c1<<8;
        muestras2[i]=valor/32767.0;
    }
    //DFT
    complex *out=malloc(sizeRead*sizeof(complex));
    DFT(muestras2,out,sizeRead);
   
    for (int i=0;i<sizeRead;i++){
        unsigned int aux=(out[i].real*32767.0)/sizeRead;
		unsigned char signal[2]={(unsigned char)aux,aux>>8};
        fwrite(signal,1,2,outputf);
         aux=(out[i].imaginario*32767.0)/sizeRead;
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

int DFT(double *inreal,complex * out,int n) {
    for (int k = 0; k < n; k++) {  // For each output element
        double sumreal = 0;
        double sumimag = 0;
        for (int t = 0; t < n; t++) {  // For each input element
            double angle = 2 * M_PI * t * k / n;
            sumreal +=  inreal[t] * cos(angle);
            sumimag += -inreal[t] * sin(angle);
        }
        out[k].real= sumreal;
        out[k].imaginario = sumimag;
    }
    return 1;
}
//invierte la entrada de numeros de la cabecera
unsigned int obtenerNumero (unsigned char *header,int lowerLimit, int upperLimit){
    unsigned int sal=0;
    for (int i=lowerLimit,c=0;i<upperLimit;i++,c++){
         sal|=header[i]<<(8*c);
    }
    return sal;
}
//invierte un numero para mandarlo a la cabecera
void mandarNumero(unsigned char *header, int lowerLimit, int upperLimit, unsigned int num){
    for (int i=lowerLimit,c=0;i<upperLimit;i++,c++){
         header[i]=(num>>(8*c));
    }
}