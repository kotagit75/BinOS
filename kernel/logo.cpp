#include "Logo.hpp"

#include "logger.hpp"

namespace Logo{

  void print_logo(){
    Log(kWarn,"                                               :----:\n\
                                            .=*+=::::=+*=.\n\
                                            =M-          -M=\n");
    Log(kWarn,"                                        *=              =*\n\
                                          *=     :=++=:     =*\n\
                                        :M     *M:..:M*     M:\n");
    Log(kWarn,"                                       =+    =M      M=    +=\n\
                                        +=    +*  ++  *+    =+\n\
                                        -*    .H-.HH.-H.    *-v\n");
    Log(kWarn,"                                        H:    .+HHHH+.    :H\n\
                                          :M.     H::H     .M:\n\
                                          .M=   -M  M-   =M.\n");
    Log(kWarn,"                                           -*+:M-  -M:+*-\n\
                                              .+H++++H+.\n\
                                                ++    ++\n");
    Log(kWarn,"                                              HM=..=MH\n\
                                              -M +@@+ M-\n\
                                              MMM=..=MMM\n");
    Log(kWarn,"                                            .HM+.  .+MH.\n\
                                              ++ :*MM*: ++\n\
                                              H.:+M==M+:.H\n");
    Log(kWarn,"                                           -HM+:    :+MH-\n\
                                            MH+.      .+HM\n\
                                            .H -**-  -**- H.\n");
    Log(kWarn,"                                          ++   .+HH+.   ++\n\
                                            H.  -**::**-  .H\n\
                                          -M:+M=.    .=M+:M-\n");
    Log(kWarn,"                                         MH+:          :+HM\n\
                                          .HM=            =MH.\n\
                                          ++ =M+:      :+M= ++\n");
    Log(kWarn,"                                        H.   :=**MM**=:   .H\n\
                                        -M     .=M++M=.     M-\n\
                                        M-   :**-    -**:   -M\n");
    Log(kWarn,"                                      .H .=M+.        .+M=. H.\n\
                                        +M**-              -**M+\n\
                                        HH-                  -HH\n");
    Log(kWarn,"                                     -M.+M+:.          .:+M+.M-\n\
                                      M-   :=+****++****+=:   -M\n\
                                      .H           ..           H.\n");
    Log(kWarn,"                                   ++                        ++\n\
                                      H.                        .H\n\
                                    -M                          M-\n");
    Log(kWarn,"                                   *-                          -*\n");
    Log(kWarn,"                                :*==-=+*=.    -**==+*+:    .++==+*+.\n\
                                  :#-    :#=   =#-     =#-   **     :\n\
                                  :#-    .#-  .#=       **   **:\n\
                                  :#+--=+*-   -#:       =#:  .+#*=:\n\
                                  :#+:::-=*=  -#:       -#:    .:=*#=.\n\
                                  :#-     -#- :#=       +#.        :#+\n\
                                  :#-     -#:  +#:     -#=   :      **\n\
                                  :#+----+#=    =#+-:-+#-   :**=-:-+*:\n\
                                  .------:       .-===-.      :-===:       \n");
    /*Log(kWarn,"                 :oMP*ooooooo**PMMPP*oo==-----------------==oo*PPMMP**ooooooo*PM*:\n\
                  :PM*ooooooo*PMP*o=-----==-----==-------=-----------=o*PMP*ooooooo*MP:\n\
                  oM*oooooo*PM*=-----------------------------=-====--=-----=*MP*oooooo*M*.\n\
                PPoooooooPMo-----=-===---==---==--------=------------ooooo=--oMPoooooooPP\n");
    Log(kWarn,"              =MoooooooPP=-=----=-===--------==-----==-------------*M===*M---=PPoooooooMo\n\
                *PoooooooMo--=----=-----=----------=-------------*PPPP@.   M=---=@oooooooPP\n\
                PMoooooooMP----------=--=----=-----------==------=====Po   oP---*MoooooooPM\n\
                P@PoooooooMP=--------------------------------=--------oMPPPP*-=PM*oooooo*@M\n");
    Log(kWarn,"              PoPP*oooooo*MPo=----------==-----=-----------------==------=oPMPoooooooPP=M\n\
                P= =MP*oooooo*PMP*o=-------------=-=---=---------------=o*PMP*oooooooPM= -M\n\
                P=   =PMPoooooooo*PMMP**o==---------------------==oo*PMMP**ooooooo*MP=.  -M\n\
                P=     .=PMP*ooooooooo**PPMMMPPPPPP*****PPPPPPMMMPP**ooooooooo*PMPo:     -M\n");
    Log(kWarn,"              P=        .-oPMMP**oooooooooooooo*********oooooooooooooo**PPMPo-.        -M\n\
                P=             .-=*PPMPPP****ooooooooooooooooo****PPPMPP*=-.             -M\n\
                P=                    .:-=oo**PPPPPPPPPPPPPPP**oo=-:.                    -M\n\
                P=                                                                       -M\n");
    Log(kWarn,"              P=    .--::               .--:          :..        :-:     :             -M\n\
                P=    :o=-o:--o:==--o=---::o.=*=-=-=-=o:o=:=-o=:==:-o.=o.*:o=====o       -M\n\
                P=     o=-oo---.o-===oo.. .=-:o=--=::=o-=o-=:o=-o*.-=-o*-==-o:o-:=       -M\n\
                P=                .=.:=      :.                 :-    =:                 -M\n");
    Log(kWarn,"              P=                   :====o=.    .-====oo:     -oooo-                    -M\n\
                P=                 :*=:oo: *P. .o*:-*o. PP:   *P*  -P-                   -M\n\
                P=                :P- oPP  *P..P* oP*   oPo  .PP*  o*:                   -M\n\
                *o                =P..PPo:oP: :P-=PP:   *P*   =PP*.                      =P\n");
    Log(kWarn,"              -M                 :.-PPo=*P*. .:PPo   .PP=    -PPP:                     P=\n\
                oP.                 oPP  .PP=  -PP-   =PP. .==:.PPP:                   **\n\
                  =M-               .PPo   PP=  =PP:  .PP= .PP-. :PPo                 -M=\n\
                  .oP=.            -PP-  =PP.  -PP- :PP=  :P*   :PP=              .=P*.\n\
                    .=P*=.         *PP:PPP*:    =PPPPo:    oPo=oPP-            .-*P=.\n");
    Log(kWarn,"                      .=*P*=:         .:.                  .:-:.          :-oP*=.\n\
                            :-oPP*=-:.                               .:-=*PPo=:\n\
                                .:=o*PPP*oo==--:::::::::::--==oo*PPP*o=-.\n\
                                        ..:--===ooooooooo===--:..\n");*/
  }
}