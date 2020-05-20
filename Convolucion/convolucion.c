/**
 *Autor Estrada Bernal José Bryan
 *Programa escrito en Visual Strudio Code y compilado con gcc v7
 *Teoria de Sañales con el profesor Eduardo Aldana del IPN ESCOM
 *Proposito: Usar la convolucion entre la respuesta al impulso h(t) con una señal x{t}
 *en su forma discreta es decir h[n] y x[k] para obtener la salida de un filtro pasa bajas
 *con funcion de transferencia igual a 1/(s+1/RC) con la frecuencia de corte (Fc)se obtiene
 *1/(s+2piFc) con h(t)=L^-1{1/(s+2piFc)} = e^(-2piFct) con 100 muestras para h[t]
 */
#include<stdio.h>
#include<string.h>
#include<math.h>
int convolve1D(double* in, double* out, int dataSize, double* kernel, int kernelSize,double *max);
int main(int argc, char *argv[]){
    double muestra_del_circuito[100]={
0.99997 ,0.75201 ,0.56555 ,0.42532 ,0.31985 ,0.24054 ,0.18091 ,0.13605 ,
0.10229 ,0.07693 ,0.05786 ,0.04352 ,0.03271 ,0.02460 ,0.01849 ,0.01392 ,0.01047 ,0.00787 ,
0.00592 ,0.00443 ,0.00333 ,0.00250 ,0.00189 ,0.00140 ,0.00107 ,0.00079 ,0.00058 ,0.00043 ,
0.00034 ,0.00024 ,0.00018 ,0.00012 ,0.00009 ,0.00006 ,0.00006 ,0.00003 ,
0.00003 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,
0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,
0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,
0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,
0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,
0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,
0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,
0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000 ,0.00000};
//lectura del archivo de entrada
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
//printf("success");
int bytesSample=header[34]/8;
unsigned int sizeRead=0;
for (int i=40,c=0;i<44;i++,c++){
         sizeRead|=header[i]<<(8*c);
        }
sizeRead/=bytesSample;
// lectura de muestras
switch (bytesSample)
{
case 1:{
    char a= 20;
    printf("%d",(unsigned char)a);
    double muestras1[sizeRead];
    // lectura de datps
    for  (int i=0 ;i<sizeRead;i++){
		int c1=0x0;
        c1=fgetc(inputf);
        muestras1[i]=(c1-128)/128.0;
    } 
    /*
    Convolucion de 1d a 1d en un solo espacio X[n]
    */ 
   // printf("resultados\n");
    double out[100+sizeRead];
    //revisar max
    double max=0.0;
    if(!convolve1D(muestras1,out,sizeRead,muestra_del_circuito,100,&max))return 0;
    for (int i=0;i<sizeRead;i++){
        fputc((char)((out[i])*128/max)+128,outputf);
       //printf("%f\n",(out[i])*128/max);
    }


    }


    break;  
case 2:{
    //lectura de datos
    //printf("16 bits");
    double muestras2[sizeRead];
    for  (int i=0 ;i<sizeRead;i++){
		short valor=0;
		unsigned char c1=0x00,c0=fgetc(inputf);
        c1=fgetc(inputf);
		valor=c0|c1<<8;
        muestras2[i]=valor/32767.0;
    }
    //convolucion
    double out[100+sizeRead];
    double max=0;
    if(!convolve1D(muestras2,out,sizeRead,muestra_del_circuito,100,&max))return 0;
    for (int i=0;i<sizeRead;i++){
        unsigned int aux=(out[i]*32767.0)/max;
		unsigned char signal[2]={(unsigned char)aux,aux>>8};
        fwrite(signal,1,2,outputf);
        //printf("salida : %d\n",aux); 

    }

}
    break; 
default:
    break;
}

 while (!feof(inputf)){
		fputc(fgetc(inputf),outputf);
		}
fclose(inputf);
fclose(outputf);
return 0;

}

int convolve1D(double* in, double* out, int dataSize, double* kernel, int kernelSize,double *max)
{
    int i, j, k;
    double abs=0;

    // check validity of params
    if(!in || !out || !kernel) return 0;
    if(dataSize <=0 || kernelSize <= 0) return 0;

    // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
    for(i = kernelSize-1; i < dataSize; ++i)
    {
        out[i] = 0;                             // init to 0 before accumulate

        for(j = i, k = 0; k < kernelSize; --j, ++k)
            out[i] += in[j] * kernel[k];
        abs=fabs(out[i]);
        if(*max<abs)*max=abs;
    }

    // convolution from out[0] to out[kernelSize-2]
    for(i = 0; i < kernelSize - 1; ++i)
    {
        out[i] = 0;                             // init to 0 before sum

        for(j = i, k = 0; j >= 0; --j, ++k)
            out[i] += in[j] * kernel[k];
       abs=fabs(out[i]);
        if(*max<abs)*max=abs;
    }
    return 1;
}   