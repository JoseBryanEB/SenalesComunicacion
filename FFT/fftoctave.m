N=512;
t=linspace(-1,1,N);
T=2/N; %intervalo del muestreo
y=cos(2*pi*10*t);
figure (1),plot(t,y)
fcorte=1/(2*T); 
paso=2*fcorte/N;
f=-fcorte:paso:fcorte-paso;
Y=fftshift(fft(y));
figure(2),plot3(f,real(Y),imag(Y)), ylabel("real"),xlabel("eje k")
