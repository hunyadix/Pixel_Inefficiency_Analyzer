#ifndef SCAN_POINTS_H
#define SCAN_POINTS_H 

#define NOVAL_I -9999
#define NOVAL_F -9999.0

namespace scans {
  
  //________________________________________________________________________________________
  //                                 Time Delay Scans
  
  int delay_scan_no(int run) {
    switch (run) {
      // Delay Scan - 2011
    case 160413:
      return 0;
    case 160497:
      return 0; 
    case 160577:
      return 0;
    case 160578:
      return 0;
      // 2012 - April
    case 190411 :
      return 1;
    case 190456:
      return 1;
      // 2012 - July
    case 198022: // New Vana Settings
      return 2;
    case 198023:
      return 2;
    case 198048: // Old Vana Settings
      return 3;
      
      // 2015 - 03 June - 0T
    case 246908:
      return 4;
    case 246919:
      return 4;
    case 246920:
      return 4;
    case 246923:
      return 4;
    case 246926:
      return 4;
    case 246930:
      return 4;
    case 246936:
      return 4;
      
      // 2015 - July - 3.8T
    case 251027:
      return 5;
    case 251028:
      return 5;
    case 251131:
      return 5;
    case 251134:
      return 5;
      
    default:
      return -1;
    }
  }

  int delay(int run, int ls, int orbit) {
    switch (run) {
      // Delay Scan - 2011
    case 160413:
      return 
        orbit   <  11531874 ?  25
        : orbit <  22836667 ?   0
        : orbit <  36525473 ? NOVAL_I // BAD
        : orbit <  47699601 ?   7 
        : orbit <  60471838 ?   8
        : orbit <  71445138 ?  10
        : orbit <  84930036 ? -25
        : orbit < 100147197 ? NOVAL_I // BAD
        : orbit < 114600000 ?  14
        : orbit < 129039651 ?  16
        : orbit < 145100000 ? -12
        : orbit < 160586081 ?  10
        : orbit < 174462084 ?  -6
        : orbit < 191550000 ?  -8
        : 12;
    case 160497:
      return
        orbit   < 11923615 ? NOVAL_I
        : orbit < 23009846 ? -4
        : orbit < 34154357 ? -2
        : NOVAL_I; // BAD 
    case 160577:
      return
        orbit   < 10359519 ? NOVAL_I
        : orbit < 26677701 ? 4
        : orbit < 40220000 ? 18
        : orbit < 53440000 ? 20
        : orbit < 66319978 ? 22
        : 14;
    case 160578:
      return
        orbit   <   1245694 ? NOVAL_I
        : orbit <  14309529 ? 14
        : orbit <  43223561 ? 16
        : orbit <  71862094 ? 15
        : orbit < 108617422 ? 13
        : NOVAL_I;
      // 2012 - April
    case 190411 :
      return
        ls   <  26 ?  13
        : ls <  44 ?  -6
        : ls <  63 ? -12
        : ls <  80 ? -18
        : ls <  94 ?   6
        : ls < 108 ?  12
        : ls < 127 ?  NOVAL_I
        : ls < 150 ?  13
        : ls < 175 ?  14
        : ls < 196 ?  15
        : ls < 214 ?  16
        : ls < 230 ?  12
        : ls < 246 ?  17
        : ls < 261 ?  18
        : ls < 277 ?  20
        : ls < 292 ?   0
        : ls < 309 ?  -2
        : -4;
    case 190456:
      return
        ls   <  70  ?  13
        : ls <  88  ?   2
        : ls < 108  ?   6
        : ls < 132  ?  10
        : ls < 149  ?  11
        : ls < 164  ?  38
        : ls < 174  ? -12
        : NOVAL_I;
      // 2012 - July
    case 198022: // New Vana Settings
      return 
        orbit   < 47500000 ? NOVAL_I
        : orbit < 49500000 ? 13
        : orbit < 53000000 ?  1
        : orbit < 56500000 ? 24
        : orbit < 60000000 ? 22
        : 20  ;
    case 198023:
      return
        orbit   <  4906628 ? 18
        : orbit <  9006831 ? 16
        : orbit < 12767345 ? 14
        : orbit < 16996865 ? 12
        : orbit < 21167493 ? 10
        : orbit < 25000000 ?  8
        : NOVAL_I;
    case 198048: // Old Vana Settings
      return
        orbit   <  2600000 ?  1
        : orbit <  5600000 ? 24
        : orbit <  8600000 ? 22
        : orbit < 12000000 ? 20
        : orbit < 15200000 ? 18
        : 16;
      // 2015 - 03 June - 0T
    case 246908:
      return ls<70 ? NOVAL_I : 11;
    case 246919:
      return
        ls   < 10 ? -10 // 11ns, but somehow l1 efficiencies are different
        : ls < 24 ? NOVAL_I // somehow l1 efficiencies are different - l1 occupancy ~0
        : ls < 38 ? 47
        : -3;
    case 246920:
      return NOVAL_I; // Bad run
    case 246923:
      return
        ls   <   9 ? NOVAL_I // Supposed to be 34ns
        : ls <  22 ? 10
        : 28;
    case 246926:
      return
	ls   <   7 ? NOVAL_I // Supposed to be 28ns, but the point is well off
        : ls <  20 ? 16
        : ls <  32 ? 26
        : ls <  43 ? 24
        : ls <  56 ? 20
        : ls <  67 ? 18
        : ls <  79 ? 14
        : ls <  91 ? 12
        : ls < 102 ? 30
        : ls < 113 ?  8
        : ls < 124 ?  6
        : ls < 135 ?  2
        : ls < 147 ?  0
        : ls < 160 ? -2
        : ls < 172 ? -5
        : ls < 183 ? 36
        : ls < 196 ? 32
        : ls < 210 ? 19
        : ls < 242 ? 16
        : 18;
    case 246930:
      return 22;
    case 246936:
      return NOVAL_I; // Supposed to be 14ns, but it likely remained at 11ns
      
      // 2015 - July - 3.8T
    case 251027:
      return 22;
    case 251028:
      return ls<20? 22 : 20;
    case 251131:
      return
        ls   < 25 ? 22
        : ls < 40 ? 21
        : 23;
    case 251134:
      return
        ls   < 21 ? 24
        : ls < 35 ? 25
        : ls < 48 ? 26
        : ls < 63 ? 27
        : 20;
      
    default:
      return NOVAL_I;
    }
  }
  
  //________________________________________________________________________________________
  //                                  HV Bias Scans
  
  int hv_scan_no(int run) {
    switch (run) {
    case 132599: return 0; // 2010 April 05 - L3,D1 (1 hv group)
    case 132601: return 0;
    case 132602: return 0;
    case 149182: return 1; // 2010 Oct 28 - L1,L3,D1 (1 hv group)
    case 160431: return 2; // 2011 March 14,16,18 - L1,L3,D1 (1 hv group)
    case 160578: return 2;
    case 160579: return 2;
    case 160819: return 2;
    case 160827: return 2;
    case 160835: return 2;
    case 165098: return 3; // 2011 May 15 - L1 (1 hv group)
    case 165099: return 3;
    case 170000: return 4; // 2011 July 14 - L1 (Full)
    case 171897: return 5; // 2011 July 28 - L3 (Full)
    case 172488: return 6; // 2011 August 03 - L2 (Full)
    case 175834: return 7; // 2011 September 07 - L1,D1 (1 hv group)
    case 178367: return 8; // 2011 October 12 - L1 (1 hv group)
    case 180076: return 9; // 2011 October 27 - L1 (1 hv group)
    case 180093: return 9;
    case 180241: return 10; // 2011 October 30 - D1 (1 hv group)
    case 180250: return 10;
    case 190538: return 11; // 2012 April 06 - L1-3, D1-2 (Full)
    case 190539: return 11;
    case 190591: return 11;
    case 190592: return 11;
    case 190593: return 11;
    case 190595: return 11;
    case 198023: return 12; // 2012 July 02 - L1-3, D1-2 (Full)
    case 198041: return 12;
    case 198044: return 12;
    case 198045: return 12;
    case 200786: return 13; // 2012 August 13 - L1 (Full)
    case 203739: return 14; // 2012 September 27 - L1 (Full)
    case 208392: return 15; // 2012 Dec 02 - L1-3,D1-2 (Full)
    case 208393: return 15;
    case 208394: return 15;
    case 208395: return 15;
    case 208397: return 15;
    case 210534: return 16; // 2013 Jan 21 - L1-3,D1 (1 hv group)
    case 247073: return 17; // 2015 Jun 08 - L1-3, D1-2 (Full)
    case 247078: return 17;
    case 247079: return 17;
    case 247081: return 17;
    case 247266: return 17;
    case 247267: return 17;
    case 247398: return 17;
    default: return -1;
    }
  }
  
  // Define Layer 1 Scans below
  int hv_l1(int run, int ls, int orbit) {
    switch (run) {
      // hv scan 2010 Oct 28 - L1,L3,FPix modules
    case 149182:
      return
        ls  < 16 ? 130
        : ls< 30 ? 110
        : ls< 47 ? 100
        : ls< 64 ?  90
        : ls< 81 ?  80
        : ls< 98 ?  70
        : ls<115 ?  60
        : ls<134 ?  50
        : ls<150 ?  40
        : ls<169 ?  30
        : 150;
      // hv scan 2011 March 14,16,18 - L1,L3,FPix modules
    case 160431:
      return 150;
    case 160578:
      return orbit<108617422 ? NOVAL_I : 30; // Exclude Delay scan
    case 160579:
      return
        orbit  < 10060929 ? 40
        : orbit< 20603100 ? 50
        : orbit< 30368455 ? 60
        : orbit< 36726096 ? 70
        : orbit< 41127055 ? 80
        : 90;
    case 160819:
      return
        orbit  <   734481 ?  NOVAL_I
        : orbit<  4945473 ? 150
        : orbit<  9184218 ?  70
        : orbit< 13531251 ?  80
        : orbit< 21375662 ?  90
        : orbit< 27850887 ? 100
        : NOVAL_I;
    case 160827:
      return 130;
    case 160835:
      return 150;
      // hv scan 2011 may 15 - Bpix_BmO_SEC6_LYR1-2_HV1
    case 165098:
      return
        ls  <169 ? 150
        : ls<235 ?  60
        : ls<302 ?  50
        : ls<379 ?  70
        : 40;
    case 165099:
      return ls< 33 ? 40 : 80;
      // voltage scan 2011 July 14 - L1
    case 170000:
      return
        orbit  <   75305 ? NOVAL_I
        : orbit< 6600594 ? 150
        : orbit<13798045 ?  20
        : orbit<20548946 ?  30
        : orbit<27295775 ?  40
        : orbit<34043021 ?  50
        : orbit<40787479 ?  60
        : orbit<47534688 ?  70
        : orbit<54281248 ?  80
        : orbit<61029618 ?  90
        : orbit<67777302 ? 100
        : 110;
      // voltage scan 2011 Sep 07 - L1, FPix module
    case 175834:
      return
        // L1 scan
        orbit  < 4600000 ? 150
        : orbit< 8000000 ? 100
        : orbit<11400000 ?  80
        : orbit<14800000 ?  70
        : orbit<18200000 ?  60
        : orbit<21600000 ?  50
        : orbit<25000000 ?  40
        : orbit<28300000 ?  30
        : orbit<31600000 ?  20
        : orbit<35000000 ?  10
        : orbit<38300000 ?   0
        : 150;
      // voltage scan 2011 Oct 12 - L1 module
    case 178367:
      return
        orbit  <22121144 ? 150
        : orbit<28980545 ? 130
        : orbit<35632848 ? 110
        : orbit<42301525 ?  90
        : orbit<49144222 ?  80
        : orbit<55705743 ?  70
        : orbit<62453171 ?  60
        : orbit<69382373 ?  50
        : orbit<75990540 ?  40
        : orbit<82987680 ?  30
        : orbit<89595172 ?  20
        : orbit<96264991 ?  10
        : orbit<102930200 ?  0
        : NOVAL_I;
      // voltage scan 2011 Oct 27 - L1 module
    case 180076:
      return
        orbit  <65271702 ? 150
        : orbit<69554218 ? 100
        : orbit<75434312 ?  70
        : orbit<79819278 ?  50
        : orbit<85070175 ?  40
        : orbit<89517576 ?  30
        : 20;
    case 180093:
      return
        orbit  < 3257552 ?  20
        : orbit< 7805011 ?  15
        : orbit<11807013 ?  10
        : orbit<16433020 ?   5
        : orbit<21057541 ?   0
        : orbit<22826117 ? NOVAL_I
        : 150;
      // voltage scan 2012 Apr 06 - L1, L2, L3, D1, D2
    case 190538:
      return
        ls< 50 ? 150 // L1
        : ls< 66 ?   0
        : ls< 78 ?   5
        : ls< 91 ?  10
        : ls<104 ?  15
        : ls<117 ?  20
        : 30;
    case 190539:
      return
        ls  <  3 ? NOVAL_I
        : ls< 17 ?  30
        : ls< 30 ?  40
        : ls< 47 ?  50
        : ls< 62 ?  70
        : ls< 75 ? 100
        : 150;
      // voltage scan 2012 Jul 02 - L1, L2, L3, D1, D2
    case 198023:
      return
        orbit  <25000000 ? NOVAL_I // L1
        : orbit<28800000 ? 150
        : orbit<30600000 ? 100
        : orbit<32800000 ?  80
        : orbit<35600000 ?  70
        : orbit<38200000 ?  60
        : orbit<39700000 ?  50
        : orbit<41100000 ?  40
        : orbit<42350000 ?  30
        : orbit<43900000 ?  20
        : 10;
    case 198041:
      return orbit  < 2600000 ?   5 : 150; // L2
      // voltage scan 2012 Aug 13 - Full Layer 1
    case 200786:
      return
        orbit  < 4000000 ? 150
        : orbit< 7400000 ? 100
        : orbit<11300000 ?  80
        : orbit<14200000 ?  70
        : orbit<17500000 ?  60
        : orbit<21500000 ?  50
        : orbit<24800000 ?  40
        : orbit<28300000 ?  30
        : orbit<31600000 ?  20
        : orbit<35100000 ?  10
        : orbit<38400000 ?   5
        : NOVAL_I;
      // voltage scan 2012 Sep 27 - Full Layer 1
    case 203739:
      return
        orbit  < 7500000 ? NOVAL_I
        : orbit<11400000 ? 100
        : orbit<14600000 ?  80
        : orbit<18000000 ?  60
        : orbit<21800000 ?  40
        : orbit<25400000 ?  30
        : orbit<29500000 ?  20
        : orbit<32200000 ?  10
        : orbit<36500000 ?   5
        : orbit<40494167 ? 150
        : NOVAL_I;
      // voltage scan 2012 Dec 02 - Full Layer 1-3, Disk 1-2
    case 208392:
      return
        orbit  < 5600000 ? 300
        : orbit< 7500000 ? 150
        : orbit<10300000 ? 100
        : 80;
    case 208393:
      return
        orbit  < 4300000 ?  70
        : orbit< 6300000 ?  60
        : orbit< 8600000 ?  50
        : orbit<11300000 ?  40
        : orbit<14300000 ?  30
        : orbit<16500000 ?  20
        : orbit<18800000 ?  10
        : orbit<20800000 ?   5
        : 150;
      // voltage scan 2013 Jan 21 - Layer 1-3, Disk 1 (1 HV Group)
    case 210534:
      return
        orbit  <15500000 ? 150
        : orbit<19150000 ? 100
        : orbit<23150000 ?  90
        : orbit<26650000 ?  80
        : orbit<30600000 ?  70
        : orbit<34550000 ?  60
        : orbit<37850000 ?  50
        : orbit<41700000 ?  40
        : orbit<45900000 ?  30
        : orbit<49400000 ?  20
        : orbit<53450000 ?  10
        : 150;
      // 2015 Jun 08 - L1-3, D1-2 (Full)
    case 247078:
      return 
	orbit<  10000000 ? 200  
	: orbit<55700000 ? NOVAL_I
	: 10;
    case 247079:
      return
	orbit<   1200000 ?  NOVAL_I
        : orbit< 5300000 ?  20
        : orbit< 9200000 ?  30
        : orbit<12300000 ?  40
        : orbit<15800000 ?  50
        : orbit<19200000 ?  60
	: 70;
    case 247081:
      return
        orbit<   4800000 ?  70
        : orbit< 8400000 ?  80
        : orbit<11900000 ?  90
        : 100;
    case 247266:
      return
        orbit<   6500000 ? NOVAL_I
        : orbit<10500000 ? 125
	: 150;
    default:
      return NOVAL_I;
    }
  } // end hv_l1
  
  
  // Define Layer 2 Scans below
  int hv_l2(int run, int ls, int orbit) {
    switch (run) {
      // voltage scan 2011 Aug 03 - L2
    case 172488:
      return
        orbit  <14400000 ?  NOVAL_I
        : orbit<17500000 ?  80
        : orbit<20900000 ?  50
        : orbit<24200000 ?  20
        : orbit<27600000 ?  70
        : orbit<31100000 ?  40
        : orbit<34500000 ?  60
        : orbit<39600000 ?  30
        : 150;
      // voltage scan 2012 Apr 06 - L1, L2, L3, D1, D2
    case 190539:
      return
        ls  < 91 ? 150
        : ls<105 ?   0 // L2
        : ls<120 ?   5
        : ls<135 ?  10
        : ls<150 ?  15
        : ls<165 ?  20
        : ls<180 ?  30
        : ls<196 ?  40
        : ls<211 ?  50
        : ls<225 ?  70
        : ls<240 ? 100
        : 150;
      // voltage scan 2012 Jul 02 - L1, L2, L3, D1, D2
    case 198041:
      return
        orbit< 4700000 ? 150 // L2
        : orbit< 6700000 ? 100
        : orbit< 8700000 ?  80
        : orbit<12100000 ?  70
        : orbit<14100000 ?  60
        : 50;
    case 198044:
      return
        orbit  < 1300000 ?  50
        : orbit< 4000000 ?  40
        : orbit< 6000000 ?  30
        : orbit< 8000000 ?  20
        : orbit<10700000 ?  10
        : orbit<12800000 ?   5
        : 150;
      // voltage scan 2012 Dec 02 - Full Layer 1-3, Disk 1-2
    case 208393:
      return
        orbit  <21100000 ? 150
        : orbit<25900000 ? 300
        : orbit<28200000 ? 100
        : orbit<30700000 ?  80
        : orbit<32700000 ?  70
        : orbit<35500000 ?  60
        : orbit<37700000 ?  50
        : orbit<40300000 ?  40
        : orbit<42300000 ?  30
        : orbit<44500000 ?  20
        : orbit<48300000 ?  10
        : orbit<50600000 ?   5
        : 150;
      // voltage scan 2013 Jan 21 - Layer 1-3, Disk 1 (1 HV Group)
    case 210534:
      return hv_l1(run, ls, orbit);
      // 2015 Jun 08 - L1-3, D1-2 (Full)
    case 247266:
      return
        orbit<  15300000 ? 200
        : orbit<18900000 ?  10
        : orbit<21500000 ?  20
        : orbit<25000000 ?  30
        : orbit<28400000 ?  40
        : orbit<31900000 ?  50
	: 60;
    case 247267:
      return orbit<19200000 ?  60 : 70;
    case 247398:
      return
        orbit<   5500000 ? NOVAL_I
        : orbit< 9300000 ? NOVAL_I
        : orbit<12800000 ?  90
        : orbit<16000000 ? 100
        : orbit<19600000 ? 125
	: orbit<23800000 ? 150
	: NOVAL_I;
    default:
      return NOVAL_I;
    }
  } // end hv_l2
  
  
  // Define Layer 3 Scans below
  int hv_l3(int run, int ls, int orbit) {
    switch (run) {
      // hv scan 2010 April 05 - L3,FPix modules
    case 132599:
      return 
        ls  < 75 ? 150
        : ls<126 ? 140
        : ls<178 ? 130
        : ls<233 ? 120
        : ls<283 ? 110
        : ls<332 ? 100
        : ls<385 ?  90
        : 80;
    case 132601:
      return
        ls  < 50 ? 70
        : ls<105 ? 60
        : ls<155 ? 50
        : ls<207 ? 40
        : ls<258 ? 30 
        : 150;
      // hv scan 2010 Oct 28 - L1,L3,FPix modules
    case 149182:
      return hv_l1(run, ls, orbit);
      // hv scan 2011 March 14,16,18 - L1,L3,FPix modules
    case 160431:
      return hv_l1(run, ls, orbit);
    case 160578:
      return hv_l1(run, ls, orbit);
    case 160579:
      return hv_l1(run, ls, orbit);
    case 160819:
      return hv_l1(run, ls, orbit);
    case 160827:
      return hv_l1(run, ls, orbit);
    case 160835:
      return hv_l1(run, ls, orbit);
    // voltage scan 2011 July 28 - L3
    case 171897:
      return
        orbit  < 6600000 ?  NOVAL_I
        : orbit<13600000 ?  70
        : orbit<20400000 ?  60
        : orbit<27200000 ?  50
        : orbit<33900000 ?  40
        : orbit<40800000 ?  30
        : orbit<47500000 ?  20
        : orbit<54100000 ?  80
        : 150;
      // voltage scan 2012 Apr 06 - L1, L2, L3, D1, D2
    case 190539:
      return
        ls<263 ? 150
        : ls<280 ?   0 // L3
        : ls<295 ?   5
        : ls<315 ?  10
        : 15;
    case 190591:
      return
        ls  < 87 ?  15
        : ls<104 ?  20
        : ls<118 ?  30
        : ls<134 ?  40
        : ls<150 ?  50
        : ls<163 ?  70
        : ls<178 ? 100
        : 150;
      // voltage scan 2012 Jul 02 - L1, L2, L3, D1, D2
    case 198044:
      return
        orbit<14800000 ? 150 // L3
        : 100;
    case 198045:
      return
        orbit  < 1200000 ? 100
        : orbit< 3200000 ?  80
        : orbit< 5800000 ?  70
        : orbit< 7900000 ?  60
        : orbit<10600000 ?  50
        : orbit<12600000 ?  40
        : orbit<14600000 ?  30
        : orbit<16700000 ?  20
        : orbit<18661675 ?  10
        : orbit<20700000 ?   5
        : 150;
      // voltage scan 2012 Dec 02 - Full Layer 1-3, Disk 1-2
    case 208392:
      return
        orbit  < 5600000 ? 300
        : orbit< 7500000 ? 150
        : orbit<10300000 ? 100
        : 80;
    case 208393:
      return
        orbit  <50600000 ? 150
        : orbit<55200000 ? 300
        : 100;
    case 208394:
      return orbit< 2200000 ? 100 : 80;
    case 208395:
      return
        orbit  < 2100000 ?  80
        : orbit< 4700000 ?  70
        : orbit< 7000000 ?  60
        : orbit< 9400000 ?  50
        : orbit<11500000 ?  40
        : orbit<13700000 ?  30
        : orbit<15500000 ?  20
        : orbit<18100000 ?  10
        : orbit<19900000 ?   5
        : 150 ;
      // voltage scan 2013 Jan 21 - Layer 1-3, Disk 1 (1 HV Group)
    case 210534:
      return hv_l1(run, ls, orbit);
      // 2015 Jun 08 - L1-3, D1-2 (Full)
    case 247398:
      return
        orbit<  23800000 ? 200
        : orbit<28800000 ?  10
        : orbit<31300000 ?  20
        : orbit<35000000 ?  30
        : orbit<39500000 ?  40
        : orbit<43000000 ?  50
        : orbit<46300000 ?  60
        : orbit<50300000 ?  70
        : orbit<54200000 ?  80
        : orbit<57700000 ?  90
        : orbit<61300000 ? 100
        : orbit<64800000 ? 125
	: 150;
    default:
      return NOVAL_I;
    }
  } // end hv_l3
  
  
  // Define Disk 1 Scans below
  int hv_d1(int run, int ls, int orbit) {
    switch (run) {
      // hv scan 2010 April 05 - L3,FPix modules
    case 132599:
      return 
        ls  < 75 ? 300
        : ls<126 ? 250
        : ls<178 ? 200
        : ls<233 ? 150
        : ls<283 ? 110
        : ls<332 ? 100
        : ls<385 ?  90
        : 80;
    case 132601:
      return
        ls  < 50 ? 70
        : ls<105 ? 60
        : ls<155 ? 50
        : ls<207 ? 40
        : ls<258 ? 30
        : 300;
      // hv scan 2010 Oct 28 - L1,L2,FPix modules
    case 149182:
      return
        ls  < 16 ? 200
        : ls< 30 ? 130
        : ls< 47 ? 100
        : ls< 64 ?  90
        : ls< 81 ?  80
        : ls< 98 ?  70
        : ls<115 ?  60
        : ls<134 ?  50
        : ls<150 ?  40
        : ls<169 ?  30
        : 300;
      // hv scan 2011 March 14,16,18 - L1,L2,FPix modules
    case 160431:
      return 150;
    case 160578:
      return orbit<108617422 ? NOVAL_I : 20; // Exclude Delay scan
    case 160579:
      return
        orbit  < 10060929 ? 30
        : orbit< 20603100 ? 35
        : orbit< 30368455 ? 40
        : orbit< 36726096 ? 50
        : orbit< 41127055 ? 60
        : 70;
    case 160819:
      return
        orbit  <  4945473 ? 300
        : orbit<  9184218 ?  50
        : orbit< 13531251 ?  60
        : orbit< 21375662 ?  70
        : orbit< 27850887 ?  80
        : NOVAL_I;
    case 160827:
      return 120;
    case 160835:
      return orbit< 22377196 ? 120 : 300;
      // voltage scan 2011 Sep 07 - L1, FPix module
    case 175834:
      return
        // FPix scan
        orbit  < 70200000 ? 300
        : orbit< 75800000 ? 150
        : orbit< 79200000 ?  80
        : orbit< 82500000 ?  70
        : orbit< 86000000 ?  60
        : orbit< 89300000 ?  50
        : orbit< 92500000 ?  40
        : orbit< 98500000 ?  30
        : orbit<101900000 ?  20
        : orbit<105300000 ?  10
        : orbit<108800000 ?   0
        : NOVAL_I;
    // voltage scan 2011 Oct 3 - FPix module
    case 180241:
      return
        orbit  < 99822153 ? 300
        : orbit<103299446 ?  15
        : orbit<106987682 ?  10
        : 70;
    case 180250:
      return
        orbit  <15726839 ? NOVAL_I
        : orbit<19447884 ?  70
        : orbit<23554126 ?  50
        : orbit<27501036 ?  40
        : orbit<31867338 ?  30
        : orbit<34939336 ?  20
        : orbit<38504259 ?  15
        : orbit<42213474 ?  10
        : orbit<45793856 ?   5
        : orbit<51126926 ?   0
        : 150;
      // voltage scan 2012 Apr 06 - L1, L2, L3, D1, D2
    case 190591:
      return
        ls  <191 ? 300
        : ls<203 ?   0 // D1
        : 5;
    case 190592:
      return
        ls  < 14 ?  10
        : ls< 29 ?  15
        : 20;
    case 190593:
      return 20;
    case 190595:
      return
        ls  < 16 ?  20
        : ls< 30 ?  30
        : ls< 43 ?  40
        : ls< 56 ?  50
        : ls< 69 ?  70
        : ls< 82 ? 100
        : ls< 95 ? 150
        : 300;
      // voltage scan 2012 Jul 02 - L1, L2, L3, D1, D2
    case 198045:
      // D1
      return
        orbit  <20700000 ? 300
        : orbit<23200000 ? 150
        : orbit<25200000 ? 100
        : orbit<27200000 ?  80
        : orbit<29281687 ?  70
        : orbit<31300000 ?  60
        : orbit<33261006 ?  50
        : orbit<35400000 ?  40
        : orbit<37338482 ?  30
        : orbit<39418496 ?  20
        : orbit<41343074 ?  10
        : orbit<43500000 ?   5
        : 300;
      // voltage scan 2012 Dec 02 - Full Layer 1-3, Disk 1-2
    case 208397:
      return
        orbit  < 1400000 ? NOVAL_I
        : orbit< 4400000 ? 150
        : orbit< 6200000 ? 100
        : orbit< 8400000 ?  80
        : orbit<10200000 ?  70
        : orbit<12300000 ?  60
        : orbit<14500000 ?  50
        : orbit<16500000 ?  40
        : orbit<18600000 ?  30
        : orbit<20950000 ?  20
        : orbit<23300000 ?  10
        : orbit<25950000 ?   5
        : 300;
      // voltage scan 2013 Jan 21 - Layer 1-3, Disk 1 (1 HV Group)
    case 210534:
      return
        orbit  <15500000 ? 300
        : orbit<19150000 ? 100
        : orbit<23150000 ?  90
        : orbit<26650000 ?  80
        : orbit<30600000 ?  70
        : orbit<34550000 ?  60
        : orbit<37850000 ?  50
        : orbit<41700000 ?  40
        : orbit<45900000 ?  30
        : orbit<49400000 ?  20
        : orbit<53450000 ?  10
        : 300;
      // 2015 Jun 08 - L1-3, D1-2 (Full)
    case 247073:
      return
        orbit<  41800000 ? 300
        : orbit<42700000 ? NOVAL_I
        : orbit<46600000 ?  10
        : orbit<51000000 ?  20
        : orbit<54800000 ?  30
        : orbit<58500000 ?  40
        : orbit<63300000 ?  50
        : orbit<66800000 ?  60
        : orbit<70700000 ?  70
        : orbit<74200000 ?  80
        : orbit<78000000 ?  90
	: 100;
    case 247078:
      return
        orbit<   1900000 ? NOVAL_I
        : orbit< 5700000 ? 125
	: 150;
    default:
      return NOVAL_I;
    }
  } // end hv_d1
  
  
  // Define Disk 2 Scans below
  int hv_d2(int run, int ls, int orbit) {
    switch (run) {
      // voltage scan 2012 Apr 06 - L1, L2, L3, D1, D2
    case 190595:
      return
        ls  <107 ? 300
        : ls<119 ?   0 // D2
        : ls<132 ?   5
        : ls<147 ?  10
        : ls<161 ?  15
        : ls<175 ?  20
        : ls<187 ?  30
        : ls<199 ?  40
        : ls<210 ?  50
        : ls<223 ?  70
        : ls<237 ? 100
        : ls<250 ? 150
        : 300;
      // voltage scan 2012 Jul 02 - L1, L2, L3, D1, D2
    case 198045:
      // D2
      return
        orbit<43500000 ? 300
        : orbit<45500000 ? 150
        : orbit<47600000 ? 100
        : orbit<49650000 ?  80
        : orbit<51450000 ?  70
        : orbit<53550000 ?  60
        : orbit<55400000 ?  50
        : orbit<57450000 ?  40
        : orbit<59450000 ?  30
        : orbit<61500000 ?  20
        : orbit<63500000 ?  10
        : orbit<65600000 ?   5
        : NOVAL_I;
      // voltage scan 2012 Dec 02 - Full Layer 1-3, Disk 1-2
    case 208397:
      return
        orbit<25950000 ? 300
        : orbit<29750000 ? 150
        : orbit<32000000 ? 100
        : orbit<34500000 ?  80
        : orbit<36800000 ?  70
        : orbit<38600000 ?  60
        : orbit<41000000 ?  50
        : orbit<42700000 ?  40
        : orbit<44700000 ?  30
        : orbit<46700000 ?  20
        : orbit<49500000 ?  10
        : orbit<52650000 ?   5
        : 300;
      // 2015 Jun 08 - L1-3, D1-2 (Full)
    case 247078:
      return
        orbit<  10100000 ? 300
        : orbit<13500000 ?  10
        : orbit<17300000 ?  20
        : orbit<20900000 ?  30
        : orbit<24600000 ?  40
        : orbit<28200000 ?  50
        : orbit<31500000 ?  60
        : orbit<34900000 ?  70
        : orbit<38500000 ?  80
        : orbit<42200000 ?  90
        : orbit<45400000 ? 100
        : orbit<49400000 ? 125
	: 150;
    default:
      return NOVAL_I;
    }
  } // end hv_d2

  int bias_voltage(int hv_scan_no, int run, int ls, int orbit, int det, int lay, int disk) {
    if (hv_scan_no==-1) return NOVAL_I;
    return (det==1) ? ((disk==1) ? hv_d1(run, ls, orbit) : (disk==2) ? hv_d2(run, ls, orbit) : NOVAL_I) :
      ((lay==1) ? hv_l1(run, ls, orbit) : (lay==2) ? hv_l2(run, ls, orbit) : (lay==3) ? hv_l3(run, ls, orbit) : NOVAL_I);
  }
  
  bool is_full_hv_scan(int hv_scan_no) {
    return hv_scan_no==4||hv_scan_no==5||hv_scan_no==6||hv_scan_no==11||hv_scan_no==12||hv_scan_no==13||hv_scan_no==14||hv_scan_no==15||hv_scan_no==17;
  }
  
  bool is_one_hv_group_scan(int hv_scan_no, int hv, bool det, int layer, int ladder, int module, int disk, int blade, int panel) {
    if (hv_scan_no==-1) return 0;
    if (hv==NOVAL_I) return 0;
    if (det) {
      // FPix_BmI_D1_ROG1_HV1
      if (disk==-1) return blade>0 && blade<4 && (panel+module)<4; // ROG1 HV1
      else return 0;
    } else {
      //BPIX_BmO_SEC2/6_LYR1-2_HV1
      if (layer==1) return module<0  && ((hv_scan_no==1 && ladder==-3)||ladder==-7); // BmO SEC2/6
      //BPix_BpO_SEC2_LYR3_HV1
      else if (layer==3) return module>0 && (ladder==-4||ladder==-5); // BpO SEC2 HV1
      else return 0;
    }
  }
  
}// namespace scans

#endif
