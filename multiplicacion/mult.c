#include<stdio.h>
#include<string.h>
#include<math.h>
#include<malloc.h>
#define M_PI 3.14159

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

typedef struct complejo{
double real;
double imaginario;} complex;
void MULT (complex in, complex out,complex *n);

int main(int argc,char *argv[]){

    if (argc!=4){perror("Exit reuturn: ");return 0;}
FILE *inputf1,*outputf,*inputf2;
inputf1=fopen(argv[1],"rb");
inputf2=fopen(argv[2],"rb");
outputf=fopen(argv[3],"wb");
if (inputf1==NULL || outputf==NULL|| inputf2==NULL)return -1;

unsigned char header[44],header2[44];
int flagread=fread(header,1,44,inputf1);
int flagread2=fread(header2,1,44,inputf2);
//fwrite(header2,1,44,outputf);
if (flagread<44 && flagread2<44)return -1;

char wavfile[5]={header[8], header[9],header[10],header[11],'\0'};
if (strcmp(wavfile,"WAVE")!=0)return -1;
char wavfile2[5]={header2[8], header2[9],header2[10],header2[11],'\0'};
if (strcmp(wavfile2,"WAVE")!=0)return -1;
unsigned int SizeRead[2],canales[2],byteSample[2]; 
SizeRead[0]=obtenerNumero(header,40,44);
SizeRead[1]=obtenerNumero(header2,40,44);
canales[0]=obtenerNumero(header,22,24);
canales[1]=obtenerNumero(header2,22,24);
byteSample[0]=obtenerNumero(header,34,36)/8;
byteSample[1]=obtenerNumero(header2,34,36)/8;
SizeRead[0]/=byteSample[0];
SizeRead[1]/=byteSample[1];
if (SizeRead[0]>SizeRead[1])fwrite(header,1,44,outputf);
else fwrite(header2,1,44,outputf);
while (SizeRead[0]>0 || SizeRead[1]>0){
    complex entrada[2],salida;
    for (int i=0;i<2 ;i++){entrada[i].real=0;entrada[i].imaginario=0;}
    //Extraer muestras del primer archivo
    for (int i=0;i<canales[0] && SizeRead[0]>0;i++){
        //1byte = 8 bites
        if (byteSample[0]==1){
            int c1=0x0;
            c1=fgetc(inputf1);
            if (i==0){
                
                entrada[0].real=(c1-128)/128.0;
            }
            else{
                entrada[0].imaginario=(c1-128)/128.0;
            }
        }
        else {
            short valor=0;
		unsigned char c1=0x00,c0=fgetc(inputf1);
        c1=fgetc(inputf1);
		valor=c0|c1<<8;
           if (i==0){
                
                entrada[0].real=valor/32767.0;
            }
            else{
                entrada[0].imaginario=valor/32767.0;
            } 
        SizeRead[0]--;
        }
        SizeRead[0]--;
    }
    //Extraer el valor del 2do archivo
    for (int i=0;i<canales[1] && SizeRead[1]>0;i++){
        //1byte = 8 bites
        if (byteSample[1]==1){
            int c1=0x0;
            c1=fgetc(inputf2);
            if (i==0){
                
                entrada[1].real=(c1-128)/128.0;
            }
            else{
                entrada[1].imaginario=(c1-128)/128.0;
            }
        }
        else {
            short valor=0;
		unsigned char c1=0x00,c0=fgetc(inputf2);
        c1=fgetc(inputf2);
		valor=c0|c1<<8;
           if (i==0){
                
                entrada[1].real=valor/32767.0;
            }
            else{
                entrada[1].imaginario=valor/32767.0;
            } 
        SizeRead[1]--;
        }
        SizeRead[1]--;
    }
    MULT(entrada[0],entrada[1],&salida);
    if (byteSample[0]==1){
        if (canales[0]==1){
    fputc((char)((salida.real)*128)+128,outputf);
       
        }
        else{
             fputc((char)((salida.imaginario)*128)+128,outputf);
        }
    }
    else {
         if (canales[0]==1){
             unsigned int aux=(salida.real*32767.0);
		unsigned char signal[2]={(unsigned char)aux,aux>>8};
        fwrite(signal,1,2,outputf);
         }
        else
        {
         unsigned int aux=salida.imaginario*32767.0;
		unsigned char signal2[2]={(unsigned char)aux,aux>>8};
        fwrite(signal2,1,2,outputf);

        }
    }

}



fclose(inputf1);
fclose(inputf2);
fclose(outputf);
return 0;

}
void MULT (complex n1, complex n2, complex *n){
 n->real=n1.real*n2.real-n1.imaginario*n2.imaginario;
 n->imaginario=n1.imaginario*n2.real+n1.real*n2.imaginario;
} 




