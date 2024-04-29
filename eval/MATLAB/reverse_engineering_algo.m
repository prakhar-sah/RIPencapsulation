%select file%
 data = xlsread('old_dumps/tiny_aes_O0.xlsx','Sheet1','A1:P615');
% data = xlsread('old_dumps/tiny_aes_Og.xlsx','Sheet1','A1:P1115');
% data = xlsread('old_dumps/tiny_aes_O1.xlsx','Sheet1','A1:P1200');
% data = xlsread('old_dumps/tiny_aes_O2.xlsx','Sheet1','A1:P1200');
% data = xlsread('old_dumps/tiny_aes_O3.xlsx','Sheet1','A1:P1200');
% data = xlsread('old_dumps/tiny_aes_OS.xlsx','Sheet1','A1:P1200');
% data = xlsread('old_dumps/tiny_aes_Ofast.xlsx','Sheet1','A1:P1200');
% data = xlsread('old_dumps/rsa_O0.xlsx','Sheet1','A1:P300');
% data = xlsread('old_dumps/rsa_Og.xlsx','Sheet1','A1:P300');
% data = xlsread('old_dumps/rsa_O1.xlsx','Sheet1','A1:P300');
% data = xlsread('old_dumps/rsa_O2.xlsx','Sheet1','A1:P300');
% data = xlsread('old_dumps/rsa_O3.xlsx','Sheet1','A1:P300');
% data = xlsread('old_dumps/rsa_Os.xlsx','Sheet1','A1:P300');
% data = xlsread('old_dumps/rsa_Ofast.xlsx','Sheet1','A1:P300');
% data = xlsread('old_dumps/saddi_sha_O0.xlsx','Sheet1','A1:P7000');
% data = xlsread('old_dumps/saddi_sha_Og.xlsx','Sheet1','A1:P4000');
% data = xlsread('old_dumps/saddi_sha_O1.xlsx','Sheet1','A1:P4000');
% data = xlsread('old_dumps/saddi_sha_O2.xlsx','Sheet1','A1:P4000');
% data = xlsread('old_dumps/saddi_sha_O3.xlsx','Sheet1','A1:P4000');
% data = xlsread('old_dumps/saddi_sha_Os.xlsx','Sheet1','A1:P4000');
% data = xlsread('old_dumps/saddi_sha_Ofast.xlsx','Sheet1','A1:P4000');
% data = xlsread('old_dumps/gladman_sha_O0.xlsx','Sheet1','A1:P7000');
% data = xlsread('old_dumps/gladman_sha_Og.xlsx','Sheet1','A1:P4500');
% data = xlsread('old_dumps/gladman_sha_O1.xlsx','Sheet1','A1:P4500');
% data = xlsread('old_dumps/gladman_sha_O2.xlsx','Sheet1','A1:P4500');
% data = xlsread('old_dumps/gladman_sha_O3.xlsx','Sheet1','A1:P4500');
% data = xlsread('old_dumps/gladman_sha_Os.xlsx','Sheet1','A1:P4500');
% data = xlsread('old_dumps/gladman_sha_Ofast.xlsx','Sheet1','A1:P4500');

counter = 0;
register_count = 0;
guess_count = 0;
opd2 = [""];

for i = 1:614
    diff = data(i+1,:) - data(i,:);
    if diff(1,1) ~= 0
        register_i = 0;
        counter = counter + 1;
        gPCo = data(i,1);
        gPCn = data(i+1,1);
        gPCd = diff(1,1);
        gSRd = diff(1,3);
        gSRo = data(i,3);
        gRd = [diff(1,4) diff(1,5) diff(1,6) diff(1,7) diff(1,8) diff(1,9) diff(1,10) diff(1,11) diff(1,12) diff(1,13) diff(1,14) diff(1,15) diff(1,16)];
        gRo = [data(i,4) data(i,5) data(i,6) data(i,7) data(i,8) data(i,9) data(i,10) data(i,11) data(i,12) data(i,13) data(i,14) data(i,15) data(i,16)];
        gRn = [data(i+1,4) data(i+1,5) data(i+1,6) data(i+1,7) data(i+1,8) data(i+1,9) data(i+1,10) data(i+1,11) data(i+1,12) data(i+1,13) data(i+1,14) data(i+1,15) data(i+1,16)];
        gSPd = diff(1,2);
        
        switch gSRo
            case 8
                gV = 0;
                gN = 0;
                gZ = 0;
                gC = 0;
            case 9
                gV = 0;
                gN = 0;
                gZ = 0;
                gC = 1;
            case 10
                gV = 0;
                gN = 0;
                gZ = 1;
                gC = 0;
            case 11
                gV = 0;
                gN = 0;
                gZ = 1;
                gC = 1;
            case 12
                gV = 0;
                gN = 1;
                gZ = 0;
                gC = 0;
            case 13
                gV = 0;
                gN = 1;
                gZ = 0;
                gC = 1;
            case 14
                gV = 0;
                gN = 1;
                gZ = 1;
                gC = 0;
            case 15
                gV = 0;
                gN = 1;
                gZ = 1;
                gC = 1;
            case 264
                gV = 1;
                gN = 0;
                gZ = 0;
                gC = 0;
            case 265
                gV = 1;
                gN = 0;
                gZ = 0;
                gC = 1;
            case 266
                gV = 1;
                gN = 0;
                gZ = 1;
                gC = 0;
            case 267
                gV = 1;
                gN = 0;
                gZ = 1;
                gC = 1;
            case 268
                gV = 1;
                gN = 1;
                gZ = 0;
                gC = 0;
            case 269
                gV = 1;
                gN = 1;
                gZ = 0;
                gC = 1;
            case 270
                gV = 1;
                gN = 1;
                gZ = 1;
                gC = 0;
            case 271
                gV = 1;
                gN = 1;
                gZ = 1;
                gC = 1;
            otherwise
                disp('some other SR');
        end
        
        for n = 1:13
            if gRd(1,n) ~= 0
                register_count = register_count + 1;
                op2i(1,n) = 1;
            else
                op2i(1,n) = 0;
            end
        end
        
        if register_count == 1
            for n = 1:13
                if op2i(1,n) == 1
                    op2 = [n+2];
                    op2o = gRo(1,n);
                    op2n = gRn(1,n);
                    opd2 = ["Rm"];
                    break;
                end
            end
        elseif register_count >= 3
            op22 = zeros(1,register_count);
            for n = 1:13
                if op2i(1,n) == 1
                    register_i = register_i + 1;
                    op22(1,register_i) = n+2;
                    op2o = gRo(1,n);
                    op2n = gRn(1,n);
                    opd2 = ["Rpop"];
                    opt = ["popm.a"];
                    opd1 = ["#N"];
                    op1 = register_count;
                end
            end
        end
        
        if opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o*2
            op1 = 1;
            opd1 = ["#N"];
            opt = ["rlam[.a]"];
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o*4
            op1 = 2;
            opd1 = ["#N"];
            opt = ["rlam[.a]"];
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o*8
            op1 = 3;
            opd1 = ["#N"];
            opt = ["rlam[.a]"];
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o*16
            op1 = 4;
            opd1 = ["#N"];
            opt = ["rlam[.a]"]; 
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o/2
            op1 = 1;
            opd1 = ["#N"];
            opt = ["rrum.w/rram.a"]; 
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o/4
            op1 = 2;
            opd1 = ["#N"];
            opt = ["rrum.w/rram.a"]; 
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o/8
            op1 = 3;
            opd1 = ["#N"];
            opt = ["rrum.w/rram.a"]; 
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2 && op2n == op2o/16
            op1 = 4;
            opd1 = ["#N"];
            opt = ["rrum.w/rram.a"]; 
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "Rm" && gPCd == 2
        guess_count = guess_count + 1;
            for j = 1:13
                if op2n == gRo(1,j) 
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["mov[.b],mova"];
                    break;
                elseif op2n == op2o + 1
                    opd1 = [""];
                    opt = ["inc"];
                    break;
                elseif op2n == op2o + 2
                    opd1 = [""];
                    opt = ["incd"];
                    break;
                elseif op2n == op2o + gRo(1,j)
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["add[.b],adda"];
                    break;
                elseif op2n == op2o + gRo(1,j) + gC
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["addc[.b]"];
                    break;
                elseif op2n == op2o - gRo(1,j)
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["sub[.b]"];
                    break;
                elseif op2n == op2o - gRo(1,j) + gC - 1
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["subc[.b]"];
                    break;
                elseif op2n == op2o && ~gRo(1,j)
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["bic[.b]"];
                    break;
                %elseif op2n == op2o || gRo(1,j)
                    %op1 = j+2;
                    %opd1 = ["Rn"];
                    %opt = ["bis"];
                    %break;
                elseif op2n == xor(op2o,gRo(1,j))
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["xor[.b]"];
                    break;
                elseif op2n == op2o && gRo(1,j)
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["and[.b]"];
                    break;
                else
                    opd1 = ["(Rn)","(Rn)+"];
                    opt = ["format 1 (double operand)[.b]/mova"];
                end
            end
        elseif opd2(1,1) == "Rm" && gPCd == 4
        guess_count = guess_count + 0.5;
            for j = 1:13
                if op2n == gRo(1,j) 
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["movx[.b]"];
                    guess_count = guess_count + 0.5;
                    break;
                elseif op2n == xor(op2o,gRo(1,j))
                    op1 = j+2;
                    opd1 = ["Rn"];
                    opt = ["xorx[.b]"];
                    guess_count = guess_count + 0.5;
                    break;
                else
                    opd1 = ["1:x(Rn)/ADDR/&ADDR/#N, 2:(Rn)/(Rn)+, 3:#N"];
                    opt = ["1:format 1 (double operand)[.b]/mova, 2:movx[.b]/xorx[.b], 3:adda"];
                end
            end
        elseif opd2(1,1) == "Rm" && gPCd == 6
            opd1 = ["x(Rn)","ADDR","&ADDR","#N"];
            opt = ["movx[.b]/xorx[.b]"];
            guess_count = guess_count + 0.5;
        elseif opd2(1,1) == "" && gPCd == 4 && gSPd == 0
            opd1 = ["1:Rn/(Rn)/(Rn)+, 2:Rn"];
            opd2 = ["1:x(Rn)/ADDR/&ADDR, 2:x(Rn)/ADDR/&ADDR"];
            opt = ["1:format 1 (double operand)[.b], 2:mova"];
            guess_count = guess_count + 0.5;
        elseif opd2(1,1) == "" && gPCd == 6 && gSPd == 0
            opd1 = ["1:x(Rn)/ADDR/&ADDR/#N, 2:Rn/(Rn)/(Rn)+, 3:&ADDR"];
            opd2 = ["1:x(Rn)/ADDR/&ADDR, 2:x(Rn)/ADDR/&ADDR, 3:none"];
            opt = ["1:format 1 (double operand)[.b], 2:movx[.b]/xorx[.b], 3:incx/clrx"];
            guess_count = guess_count + 0.5;
        elseif opd2(1,1) == "" && gPCd == 8 && gSPd == 0
            opd1 = ["x(Rn)","ADDR","&ADDR","#N"];
            opd2 = ["x(Rn)","ADDR","&ADDR"];
            opt = ["movx[.b]/xorx[.b]"];
            guess_count = guess_count + 0.5;
        elseif opd2(1,1) == "" && (gPCd >= 9 || gPCd <= -1) && gSPd == 0
            opd1 = ["jump label"];
            opd2 = ["none"];
            opt = ["jump/bra instruction"];
            guess_count = guess_count + 1;
        elseif opd2(1,1) == "" && gSPd ~= 0
            if gSPd == -4 && (gPCd >= 9 || gPCd <= -1)
                opd1 = ["#N"];
                opd2 = ["none"];
                opt = ["calla"];
                op1 = gPCn;
                guess_count = guess_count + 1;
            elseif gSPd == 4 && (gPCd >= 9 || gPCd <= -1) 
                opd1 = ["none"];
                opd2 = ["none"];
                opt = ["reta"];
                guess_count = guess_count + 1;
            elseif gSPd <= -4 && gPCd == 2  
                opd1 = ["#N"];
                opd2 = ["Rp"];
                opt = ["pushm.a"];
                op1 = -gSPd/4;
                guess_count = guess_count + 1;
            elseif gPCd == 4
                opd1 = ["#N"];
                opd2 = ["none"];
                opt = ["adda/suba"];
                op1 = gSPd;
                guess_count = guess_count + 1;
            end
        elseif opd2(1,1) == "Rpop"
                opt = ["popm.a"];
                guess_count = guess_count + 1;
        else
            opd1 = ["(Rn)","(Rn)+","#N","x(Rn)","ADDR","&ADDR"];
            opd2 = ["PC", "x(Rm)", "ADDR", "&ADDR"];
            opt = ["no guesses"];
        end
        gPCo_h = dec2hex(gPCo);
        gPCn_h = dec2hex(gPCn);
        fprintf('Guesses for instruction %d @PC = 0x%s:\n',counter,gPCo_h);
        fprintf('operation: ');
        for k = 1:length(opt)
            fprintf('%s',opt(1,k));
        end
        fprintf('\n');
        fprintf('operand 1: ');
        if opd1(1,1) == "Rn"
            fprintf('R%d',op1);
        elseif opd1(1,1) == ""
            fprintf('none');
        elseif opd1(1,1) == "#N"
            fprintf('#%d',op1);
        elseif opd1(1,1) == "jump label"
            fprintf('(0x%s)',gPCn_h);
        else
            for k = 1:length(opd1)
                fprintf('%s, ',opd1(1,k));
            end
        end
        fprintf('\n');
        fprintf('operand 2: ');
        if opd2(1,1) == "Rm"
            fprintf('R%d',op2(1,1));
        elseif opd2(1,1) == "Rpop"
            fprintf('R%d',op22(1,op1));
            fprintf('\n registers affected: ');
            for k = 1:op1
                fprintf('R%d, ',op22(1,k));
            end
        elseif opd2(1,1) == "Rp"
            fprintf('Rp');
        else
            for k = 1:length(opd2)
                fprintf('%s, ',opd2(1,k));
            end
        end
        fprintf('\n');
        opd2 = [""];
        register_count = 0;
    end
end
