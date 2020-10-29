clear all;clc;close all;
[x,fs]=audioread('/home/slayer/Documentos/Señales/Programas/DTMF/prueba.wav');
% [S,F,T] = spectrogram(x, 1024, 512, 256*3, fs, 'yaxis');
[S,F,T] = spectrogram(x,round(length(x)/12), 0, 256*3, fs, 'yaxis');
Sa = abs(S);
[r, c] = find(Sa >= 30);
Fr = F(r);
Tc = T(c)';
FT = [Tc  Fr];
[C, ia, ic] = unique(FT(:,1));                                              % Find Unique Times
for k1 = 1:size(C,1)                                                        % Create Cell Array By Time
    FrqTime{k1} = FT(FT(:,1) == C(k1),:);                                   % Time & Frequency Cell
end
original_f = [697  770  852  941  1209  1366  1477];                        % DTMF Frequencies
dtmf_dcd = [1 5; 1 6; 1 7; 2 5; 2 6; 2 7; 3 5; 3 6; 3 7; 4 5; 4 6; 4 7];    % Combination Codes w.r.t. ‘original_f’
nbr_map = ['1' '2' '3' '4' '5' '6' '7' '8' '9' '*' '0' '#'];                % Number Key Map
for k1 = 1:size(C,1)
    freq_dist = abs(bsxfun(@minus, FrqTime{k1}(:,2), original_f));          % Distance Of ‘FrqTime’ Frequencies From ‘original_f’ Frequencies
    [~,freq_pos(:,k1)] = min(freq_dist,[],2);                               % Frequency Positions Of ‘FrqTime’ In ‘original_f’
    num_pad(k1) = nbr_map(ismember(dtmf_dcd, freq_pos(:,k1)', 'rows'));     % Map To Number Key Pad
end