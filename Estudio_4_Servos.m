close all;
clear all;
clc;

% Inicialize the serial port
delete(instrfind({'Port'},{'COM7'}));
puerto_serial = serial('COM7');
puerto_serial.BaudRate = 250000;
warning('off','MATLAB:serial:fscanf:unsuccessfulRead');

% Open the serial port
fopen(puerto_serial);

% Variables to store the information
counter = [1 1 1 1];
temp = zeros(4,1);
dest = zeros(4,1);
pos = zeros(4,1);
salir = 0;

tipo = "z";
repetir = 0;
tic;
% Inicialize the secuence
while (tipo~="e")
    if (repetir + 2 < toc) 
        fprintf(puerto_serial,'%c','g');
        repetir = toc;
    end
    tipo = fscanf(puerto_serial,'%s');
end

while (tipo~="l")
    tipo = fscanf(puerto_serial,'%s');
end

fprintf(1,"\nREGISTRANDO DATOS\n\n");

% cuenta_ro=0;
% while cuenta_ro<20
% 
%     cadena = fscanf(puerto_serial,'%s');
% 
%     C = strsplit(cadena,',')
% 
% end
while salir==0

    cadena = fscanf(puerto_serial,'%s');
    C = strsplit(cadena,',');
    tipo = C{1};
    
    if (length(C) == 4)
        tiempo = str2num(C{2});
        destino = str2num(C{3});
        posicion = str2num(C{4});  
    end
    
	if (tipo=="s")
		salir = 1;
    elseif (tipo=="a")
            temp(1,counter(1)) = tiempo;
            dest(1,counter(1)) = destino;
            pos(1,counter(1)) = posicion;
            counter(1) = counter(1) + 1;
    elseif (tipo=="b")
            temp(2,counter(2)) = tiempo;
            dest(2,counter(2)) = destino;
            pos(2,counter(2)) = posicion;
            counter(2) = counter(2) + 1;
    elseif (tipo=="c")
            temp(3,counter(3)) = tiempo;
            dest(3,counter(3)) = destino;
            pos(3,counter(3)) = posicion;
            counter(3) = counter(3) + 1;
    elseif (tipo=="d")
            temp(4,counter(4)) = tiempo;
            dest(4,counter(4)) = destino;
            pos(4,counter(4)) = posicion;
            counter(4) = counter(4) + 1;
    end
        
end

%% Represent
final = [0,0,0,0];

for i = 1:size(temp,2)
    for j = 1:4
        if ((temp(j,i)==0)&&(final(j)==0))
            final(j) = i-1;
        end
    end
end
    
for i = 1:4
    if (final(i)==0)
        final(i) = length(temp);
    end
end

x_max = 0;
tam_t_max = 0;
for i = 1:4
    if (tam_t_max < final(i))
        x_max = temp(i,final(i));
        tam_t_max = final(i);
    end
end

figure(1)
for i = 1:4
    if (final(i)>2)
        subplot(2,2,i)
        str = sprintf('Comportamiento del servomotor %d', i);
        title(str)
        xlabel('time(ms)')
        ylabel('Angle(º)')
        grid on
        hold on
        xlim([0 x_max]);
        ylim([-5 185]);
        plot(temp(i,1:final(i)),dest(i,1:final(i)),'b')
        plot(temp(i,1:final(i)),pos(i,1:final(i)),'r')
        legend('Conmutada','Real')
    end
end

% subplot(2,1,2)
% title('Comportamiento del servomotor 2')
% xlabel('time(ms)')
% ylabel('Angle(º)')
% grid on
% hold on
% xlim([0 temp(2,end)]);
% ylim([-5 185]);
% plot(temp(2,:),dest(2,:),'b')
% plot(temp(2,:),pos(2,:),'r')
% legend('Conmutada','Real')

% subplot(2,2,3)
% title('Comportamiento del servomotor 3')
% xlabel('time(ms)')
% ylabel('Angle(º)')
% grid on
% hold on
% xlim([0 temp(3,end)]);
% ylim([-5 185]);
% plot(temp(3,:),dest(3,:),'b')
% plot(temp(3,:),pos(3,:),'r')
% legend('Conmutada','Real')
% 
% subplot(2,2,4)
% title('Comportamiento del servomotor 1')
% xlabel('time(ms)')
% ylabel('Angle(º)')
% grid on
% hold on
% xlim([0 temp(4,end)]);
% ylim([-5 185]);
% plot(temp(4,:),dest(4,:),'b')
% plot(temp(4,:),pos(4,:),'r')
% legend('Conmutada','Real')
% 
% figure(2)
% title('Comportamiento de los servomotores')
% xlabel('time(ms)')
% ylabel('Angle(º)')
% grid on
% hold on
% xlim([0 temp(1,end)]);
% ylim([-5 185]);
% plot(temp(1,:),pos(1,:))
% plot(temp(1,:),pos(2,:))
% plot(temp(1,:),pos(3,:))
% plot(temp(1,:),pos(4,:))
% legend('Servo 1','Servo 2','Servo 3','Servo 4')

% Close the serial port
fclose(puerto_serial);
delete(puerto_serial);
delete(instrfind({'Port'},{'COM7'}));