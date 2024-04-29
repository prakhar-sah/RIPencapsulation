device = serialport("/dev/ttyACM1",115200);
for i = 1:4500
    data(i,:) = read(device,32,"uint8");
end

for r = 1:4500
    for c = 1:2:32
        b1 = dec2hex(data(r,c),2);
        b2 = dec2hex(data(r,c+1),2);
        temp_h = strcat(b1,b2);
        dump_h(r,(c+1)/2) = string(temp_h);
        dump(r,(c+1)/2) = hex2dec(temp_h);
    end
end


