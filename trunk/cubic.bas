100'  NAME--QUBIC
110'
120'  DESCRIPTION--PLAYS 3-DIMENSIONAL TIC TAC TOE.
130'
140'  SOURCE--UNKNOWN
150'
160'  INSTRUCTIONS--TYPE "RUN" AND FOLLOW INSTRUCTIONS.
170'
180'
190'  *  *  *  *  *  *   MAIN PROGRAM   *  *  *  *  *  *  *  *  *  *  *
200'
210 PRINT "DO YOU WANT INSTRUCTIONS";
220 INPUT C$
230 IF C$="NO" THEN 310
240 IF C$="YES" THEN 270
250 PRINT "INCORRECT ANSWER.  PLEASE TYPE 'YES' OR 'NO'";
260 GOTO 220
270 PRINT "THE GAME IS TIC-TAC-TOE IN A 4 X 4 X 4 CUBE."
280 PRINT "EACH MOVE IS INDICATED BY A 3 DIGIT NUMBER, WITH EACH"
290 PRINT "DIGIT BETWEEN 1 AND 4 INCLUSIVE.  THE DIGITS INDICATE THE"
300 PRINT "LEVEL, COLUMN, AND ROW, RESPECTIVELY, OF THE OCCUPIED PLACE."
310 DIM X(64),L(76),M(76,4),Y(16)
320 FOR I = 1 TO 16
330 READ Y(I)
340 NEXT I
350FOR I=1 TO 76
360FOR J = 1 TO 4
370 READM(I,J)
380 NEXT J
390 NEXT I
400 FOR I = 1 TO 64
410 LET X (I) =0
420 NEXT I
430 LET Z=1
440 PRINT "DO YOU WANT TO MOVE FIRST";
450 INPUT S$
460 IF S$="NO" THEN 630
470 IF S$="YES" THEN 500
480 PRINT "INCORRECT ANSWER.  PLEASE TYPE 'YES' OR 'NO'.";
490 GOTO 450
500 PRINT " "
505 gosub 3000
510 PRINT "YOUR MOVE";
520INPUTJ1
530 GOSUB 2500
540 LETK1=INT(J1/100)
550 LET J2=(J1-K1*100)
560 LET K2=INT(J2/10)
570 LET K3= J1 - K1*100 -K2*10
580 LET M=16*K1+4*K2+K3-20
590 IF X(M)=0 THEN 620
600 PRINT "THAT SQUARE IS USED, TRY AGAIN"
610 GOTO 500
620 LET X(M)=1
630 GOSUB 1640
640 FOR J=1 TO 3
650 FOR I=1 TO 76
660 IF J=1 THEN 720
670 IF J=2 THEN 790
680 IF J=3 THEN 930
690 NEXT I
700 NEXTJ
710 GOTO 1300
720 IF L(I)<>4 THEN 690
730 PRINT "YOU WIN AS FOLLOWS";
740 FOR J=1 TO 4
750 LET M=M(I,J)
760 GOSUB 1570
770 NEXT J
780 GOTO 1490
790 IF L(I)<>15 THEN 690
800 FOR J=1 TO 4
810 LET M=M(I,J)
820 IF X(M)<>0 THEN 860
830 LET X(M)=5
840 PRINT "MACHINE MOVES TO";
850 GOSUB 1570
860 NEXT J
870 PRINT ", AND WINS AS FOLLOWS"
880 FOR J=1 TO 4
890 LET M=M(I,J)
900 GOSUB 1570
910 NEXT J
920 GOTO 1490
930 IF L(I)<>3 THEN 690
940 PRINT "NICE TRY MACHINE MOVES TO";
950 FOR J=1 TO 4 
960 LET M=M(I,J)
970 IF X(M)<>0 THEN 1010
980 LET X(M)=5
990 GOSUB 1570
1000 GOTO 500
1010 NEXT J
1020 GOTO 1300
1030 FOR I = 1 TO 76
1040 LET L(I)=X(M(I,1))+X(M(I,2))+X(M(I,3))+X(M(I,4))
1050 LET L = L(I)
1060 IF L <2 THEN 1130
1070 IF L>=3 THEN 1130
1080 IF L>2 THEN 2230
1090 FOR J = 1 TO 4
1100 IF X(M(I,J))<>0 THEN 1120
1110 LET X(M(I,J))=1/8
1120 NEXT J
1130 NEXT I
1140 GOSUB 1640
1150 FOR I = 1 TO 76
1160 IF L(I)=1/2 THEN 2360
1170 IF L(I)=1+3/8 THEN 2360
1180 NEXT I
1190 GOTO 1830
1200LET Z = 1
1210 IF X(Y(Z))=0 THEN 1250
1220 LET Z=Z+1
1230 IF Z<>17 THEN 1210
1240 GOTO 1720
1250 LET M=Y(Z)
1260 LET X(M)=5
1270 PRINT "MACHINE MOVES TO";
1280 GOSUB 1570
1290 GOTO 500
1300 LET X=X
1310 FOR I=1 TO 76
1320 LET L(I)=X(M(I,1))+X(M(I,2))+X(M(I,3))+X(M(I,4))
1330 LET L=L(I)
1340 IF L<10 THEN 1410
1350 IF L>=11 THEN 1410
1360 IF L>10 THEN 2230
1370 FOR J=1 TO 4
1380 IF X(M(I,J))<>0 THEN 1400
1390 LET X(M(I,J))=1/8
1400 NEXT J
1410 NEXT I
1420 GOSUB 1640
1430 FOR I=1 TO 76
1440 IF L(I)=.5 THEN 2360
1450 IF L(I)=5+3/8 THEN 2360
1460 NEXT I
1470 GOSUB 2500
1480 GOTO 1030
1490 PRINT " "
1495 gosub 3000
1500 PRINT "DO YOU WANT TO TRY ANOTHER GAME";
1510 INPUT X$
1520 IF X$="YES" THEN 400
1530 IF X$="NO" THEN 1560
1540 PRINT "INCORRECT ANSWER. PLEASE TYPE 'YES' OR 'NO'";
1550 GOTO 1510
1560 STOP
1570 LET K1=INT((M-1)/16)+1
1580 LET J2=M-16*(K1-1)
1590 LET K2=INT((J2-1)/4)+1
1600 LET K3=M-(K1-1)*16-(K2-1)*4
1610 LET M=K1*100+K2*10+K3
1620 PRINT M;
1630 RETURN
1640 FOR S=1 TO 76
1650 LET J1 = M(S,1)
1660 LET J2=M(S,2)
1670 LET J3=M(S,3)
1680 LET J4=M(S,4)
1690 LET L(S)=X(J1)+X(J2)+X(J3)+X(J4)
1700 NEXT S
1710 RETURN
1720 FOR I=1 TO 64
1730 IF X(I)<>0 THEN 1800
1740 LET X(I)=5
1750 LET M=I
1760 PRINT "MACHINE LIKES";
1770 GOSUB 1570
1780 PRINT " "
1790 GOTO 500
1800 NEXT I
1810 PRINT "THE GAME IS A DRAW"
1820 GOTO 1490
1830 FOR K=1 TO 18
1840 LET P=0
1850 FOR I=4*K-3 TO 4*K
1860 FOR J=1 TO 4
1870 LET P=P+X(M(I,J))
1880 NEXT J
1890 NEXT I
1900 IF P<4 THEN 1940
1910 IF P<5 THEN 1970
1920 IF P<9 THEN 1940
1930 IF P<10 THEN 1970
1940 NEXT K
1950 GOSUB 2500
1960 GOTO 1200
1970 LET S=1/8
1980 FOR I=4*K-3 TO 4*K
1990 GOTO 2370
2000 NEXT I
2010 LET S=0
2020 GOTO 1980
2030 DATA 1,49,52,4,13,61,64,16,22,39,23,38,26,42,27,43
2040DATA 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20
2050 DATA 21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38
2060 DATA 39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56
2070 DATA 57,58,59,60,61,62,63,64
2080DATA1,17,33,49,5,21,37,53,9,25,41,57,13,29,45,61
2090 DATA 2,18,34,50,6,22,38,54,10,26,42,58,14,30,46,62
2100DATA 3,19,35,51,7,23,39,55,11,27,43,59,15,31,47,63
2110 DATA 4,20,36,52,8,24,40,56,12,28,44,60,16,32,48,64
2120 DATA 1,5,9,13,17,21,25,29,33,37,41,45,49,53,57,61
2130 DATA 2,6,10,14,18,22,26,30,34,38,42,46,50,54,58,62
2140 DATA 3,7,11,15,19,23,27,31,35,39,43,47,51,55,59,63
2150 DATA4,8,12,16,20,24,28,32,36,40,44,48,52,56,60,64
2160 DATA1,6,11,16,17,22,27,32,33,38,43,48,49,54,59,64
2170 DATA 13,10,7,4,29,26,23,20,45,42,39,36,61,58,55,52
2180 DATA1,21,41,61,2,22,42,62,3,23,43,63,4,24,44,64
2190DATA 49,37,25,13,50,38,26,14,51,39,27,15,52,40,28,16
2200DATA 1,18,35,52,5,22,39,56,9,26,43,60,13,30,47,64
2210DATA 49,34,19,4,53,38,23,8,57,42,27,12,61,46,31,16
2220DATA 1,22,43,64,16,27,38,49,4,23,42,61,13,26,39,52
2230 FOR J=1 TO 4
2240 IF X(M(I,J))<>1/8 THEN 2330
2250 LET X(M(I,J))=5
2260 IF L(I)<5 THEN 2290
2270 PRINT "LET'S SEE YOU GET OUT OF THIS:  MACHINE MOVES TO";
2280 GOTO 2300
2290 PRINT "YOU FOX.  JUST IN THE NICK OF TIME, MACHINE MOVES TO";
2300 LET M=M(I,J)
2310 GOSUB 1570
2320 GOTO 500
2330 NEXT J
2340 PRINT "MACHINE CONCEDES THIS GAME."
2350 GOTO 1490
2360 LET S=1/8
2370 IF I-INT(I/4)*4>1 THEN 2400
2380 LET A=1
2390 GOTO 2410
2400 LET A=2
2410 FOR J=A TO 5-A STEP 5-2*A
2420 IF X(M(I,J))=S THEN 2450
2430 NEXT J
2440 GOTO 2000
2450 LET X(M(I,J))=5
2460 LET M=M(I,J)
2470 PRINT "MACHINE TAKES";
2480 GOSUB 1570
2490 GOTO 500
2500 FOR I=1 TO 64
2510 IF X(I)<>1/8 THEN 2530
2520 LET X(I)=0
2530 NEXT I
2540 RETURN
2550 END
3000 t=0
3010 gosub 3040
3020 print
3030 t=32
3040 for j=0 to 3
3050 t2 = j*4
3060 gosub 3100
3070 t2 = 16+j*4
3075 print "  ";
3080 gosub 3100
3085 print
3090 next j
3092 print
3095 return
3100 for k=1 to 4
3110 v=x(t+t2+k);
3111 if v=1 then print " X "; else if v=5 then print " O "; else print " . ";
3120 next k
3130 return
