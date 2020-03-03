#include<stdio.h>
#include<math.h>
int main (int argc, char * argv[]){
if(argc!=2)printf ("El modo de uso es: volumen archivo.wav\n");
/*
 * Lectura de la cabecera de un archivo wave, lo que importa es cuantos canales se usan mono(uno) o estereo(dos),
 * el numero de bits por muestra 1 (8bits) y 2(16bits)
 * Tambien interesa reconocer que efectivamente se trate de una cabecera de archivo wave y no solo
 * guiarse por la extencion .wav
 * */
FILE *file1=NULL;
file1=fopen(argv[1],"rb");
unsigned char c;
unsigned char cabecera[45];
if (file1!=NULL){
    int leidos=0,flagBits=0;
    leidos=fread(cabecera,1,44,file1);
    //for (int i=0;i<leidos;i++)
    int size=0;
    unsigned char cabecera2[4]={0x00,0x08,0x00,0x00};
    for (int i=0,c=3;i<4;i++){
         size+=((cabecera2[i]))*(int)pow((int)10,(int)c--);
         printf("%d",(unsigned char*)"\b\0\0");
        }
  printf(" %d",size);

    if (cabecera[34]==16)flagBits=1;    
    while  (!feof(file1)){
        unsigned char c1=0x00,c0=fgetc(file1);
        if (!feof(file1))c1=fgetc(file1);

    }  



}
else return -1;
fclose(file1);
return 0;
}
