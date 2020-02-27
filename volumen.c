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
FILE *file1=open(argv[1],"rb");


return 0;
}
