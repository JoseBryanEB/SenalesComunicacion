#include<stdio.h>
#include<math.h>
int main (int argc, char * argv[]){
if(argc!=3)printf ("El modo de uso es: volumen archivo.wav\n");
/*
 * Lectura de la cabecera de un archivo wave, lo que importa es cuantos canales se usan mono(uno) o estereo(dos),
 * el numero de bits por muestra 1 (8bits) y 2(16bits)
 * Tambien interesa reconocer que efectivamente se trate de una cabecera de archivo wave y no solo
 * guiarse por la extencion .wav
 * */
FILE *file1=NULL,*file2=NULL;
file1=fopen(argv[1],"rb");
file2=fopen(argv[2],"wb");
//unsigned char c;
unsigned char cabecera[44];
if (file1!=NULL){
    int leidos=0,flagBits=0;
    leidos=fread(cabecera,1,44,file1);
    //for (int i=0;i<leidos;i++)
    int size=0;
    for (int i=40;i<44;i++){
         size|=(cabecera[i])<<(8*i);
        }
  printf(" %d\n",size);
	int valor;
    if (cabecera[34]==16)flagBits=1;    
    fwrite(cabecera,1,44,file2);
    while  (!feof(file1) && size--){
		char c1=0x00,c0=fgetc(file1);
        if (!feof(file1))c1=fgetc(file1);
		valor=c0|c1<<8;
		valor=valor>>1;
		int salida=0x00;
		salida=((valor&0xff)<<8)|((valor>>8)&0xff);
		char salfile[2];
		printf("entrada %d| salida %d\n",c0|c1<<8,valor);
		salfile[0]= (salida>>8)&0xff;
		salfile[1]=(salida)&0xff;
		fwrite(salfile,1,2,file2);
    }  



}
else return -1;
fclose(file1);
return 0;
}
