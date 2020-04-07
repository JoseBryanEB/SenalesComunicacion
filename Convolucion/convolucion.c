#include<stdio.h>
#include<string.h>
int convolve1D(float* in, float* out, int dataSize, float* kernel, int kernelSize);
int main(char argc, char *argv[]){
    float muestra_del_circuito[100]={
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
//fwrite(header,1,44,outputf);

if (flagread<44)return -1;

char wavfile[5]={header[8], header[9],header[10],header[11],'\0'};
if (strcmp(wavfile,"WAVE")!=0)return -1;
printf("success");
int bytesSample=header[34]/8;
unsigned int sizeRead=0;
for (int i=40;i<44;i++){
         sizeRead|=header[i]<<(8*i);
        }
sizeRead/=bytesSample;
// lectura de muestras
switch (bytesSample)
{
case 1:{
    float muestras1[sizeRead];
    // lectura de datps
    for  (int i=0 ;i<sizeRead;i++){
		unsigned char c1=0x00;
        c1=fgetc(inputf);
        muestras1[i]=c1/255.0;
    } 
    /*
    Convolucion de 1d a 1d en un solo espacio X[n]
    */ 
    float out[100+sizeRead];
    if(!convolve1D(muestras1,out,sizeRead,muestra_del_circuito,100))return 0;
    for (int i=0;i<sizeRead;i++){
        fputc(out[i]*=0xff,outputf);
    }


    }


    break;  
case 2:{
    //lectura de datos
    printf("16 bits");
    float muestras2[sizeRead];
    for  (int i=0 ;i<sizeRead;i++){
		short valor=0;
		char c1=0x00,c0=fgetc(inputf);
        c1=fgetc(inputf);
		valor=c0|c1<<8;
        muestras2[i]=valor/65535.0;
       // printf("valor: %f",muestras2[i]);
    }
    //convolucion
    float out[100+sizeRead];
    if(!convolve1D(muestras2,out,sizeRead,muestra_del_circuito,100))return 0;
    for (int i=0;i<sizeRead;i++){
        out[i]*=65535.0;
       short salida=out[i];
		salida=((salida&0xff)<<8)|((salida>>8)&0xff);
		unsigned char salfile[2];
		//printf("entrada %x| salida %x\n",c0|c1<<8,salida&0xffff);
		salfile[0]= (salida>>8)&0xff;
		salfile[1]=(salida)&0xff;
		fwrite(salfile,1,2,outputf);

    }

}
    break; 
default:
    break;
}
fclose(inputf);
fclose(outputf);
return 0;

}

int convolve1D(float* in, float* out, int dataSize, float* kernel, int kernelSize)
{
    int i, j, k;

    // check validity of params
    if(!in || !out || !kernel) return 0;
    if(dataSize <=0 || kernelSize <= 0) return 0;

    // start convolution from out[kernelSize-1] to out[dataSize-1] (last)
    for(i = kernelSize-1; i < dataSize; ++i)
    {
        out[i] = 0;                             // init to 0 before accumulate

        for(j = i, k = 0; k < kernelSize; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    // convolution from out[0] to out[kernelSize-2]
    for(i = 0; i < kernelSize - 1; ++i)
    {
        out[i] = 0;                             // init to 0 before sum

        for(j = i, k = 0; j >= 0; --j, ++k)
            out[i] += in[j] * kernel[k];
    }

    return 1;
}