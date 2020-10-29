pkg load control
pkg load signal 
k=0:31;
Tm=1/2000;
n=0:31;
for m=0:32
	x=sin(2*pi*m*62.5*n*Tm);
	X=fft(x);
	subplot(3,1,1);
		stem(k,x);
		titulo=sprintf("x[n]=sin(2*pi*%d*t)",62.5*m);
		title(titulo);
		axis([0,31,-1.5,1.5]);
	subplot(3,1,2);
		stem(k,real(X));
		title("Parte real de la transformada discreta de fourier");
		axis([0,31,-33,33]);
	subplot(3,1,3);
		stem(k,imag(X));
		title("Parte imaginaria de la transformada discreta de fourier");
		axis([0,31,-33,33]);
	nombre=sprintf("sin%d.jpg",m);
	print(nombre);
end

