#include <stdio.h>
#include "mensagem.h"
#include "memvirtu.h"
#include "lowparam.h"
#include "lowmacro.h"
#include "lowsolid.h"
#include "eulerops.h"
#include "vectorop.h"
#include "genfunc_.h"
#include "analise_.h"
#include "prop.h"
#include "mancommd.h"

real MSD_highNamePropriedadeArea(char *name);
real MSD_highPropriedadeArea(Id sn);
double MSD_lowPropriedadeArea(SPTYPE s);
double MSD_lowPropriedadeAreaFace(FPTYPE f);
double MSD_lowPropriedadeAreaLaco(LPTYPE l);

void MSD_execManipulatePropriedade(void)
{
    int ip ;
    char onam[30] ;
    real area, volume;
	float a, b, c, d, d1, d2, d3, d4, d5 ;      // defini��o das vari�veis locais

    for (ip = 0 ; ip == 0 ; ) {
        switch(optin()) {                 // recupera as o��es do comando

            // - Comando calcula a area - recupera o nome do solido
            case 'a':
                if (1 == sscanf(restbuf, "%s", onam)) {
                    area = MSD_highNamePropriedadeArea(onam) ;
                    ip = 1 ;
                    printf("area = %f\n", - area);
                }
                break ;

            // - Comando calcula o volume - recupera o nome do solido
            case 'v':
                if (1 == sscanf(restbuf, "%s", onam)) {
//                    volume = MSD_highNamePropriedadeVolume(onam) ;
                    ip = 1 ;
                    printf("volume = %f", volume);
                }
                break ;

            // - Comando que calcula o espelho em rela��o a um plano
            case 'e':
                if (5 == sscanf(restbuf, "%s %f %f %f %f", onam, &a, &b, &c, &d)) {
                    //MSD_highNamePropriedadeEspelha(onam, a, b, c, d) ;
                    ip = 1 ;
                }
                break ;

			// - Comando para criar o primitivo barra T
            case 't':
                if (6 == sscanf(restbuf, "%s %f %f %f %f %f", 
                                         onam, &d1, &d2, &d3, &d4, &d5)) {
                    //MSD_ highCreatePrimitivoT(onam, d1, d2, d3, d4, d5) ;
                    ip = 1 ;
                }
                break ;

        }
        if (ip == 0) {
            printf("-avet nome do solido\n") ;
            if (!lineins((char*)"? "))
                return ;
        }
    }
}

real MSD_highNamePropriedadeArea(char *name)
{
    int sn ;                       // identificador do s�lido

    if ((sn = MSD_getSolidIdFromName(name)) == -1) {
        fprintf(stderr, "area: nao encontrou o s�lido %s!\n", name) ;
        return(ERROR) ;
    }
    return(MSD_highArea(sn)) ;
}

real MSD_highPropriedadeArea(Id sn)
{
    SPTYPE s ;                     // ponteiro para o s�lido

    if ((s = MSD_getSolid(sn)) == SNIL) {
        fprintf(stderr, "area: nao encontrou o s�lido %d!\n", sn) ;
        return(0.0) ;
    }
    return(MSD_lowPropriedadeArea(s)) ;
}

double MSD_lowPropriedadeArea(SPTYPE s)
{
    DPTYPE d;
    FPTYPE f;
    double area = 0;

    for (d = SolSShells(s) ; d != DNIL ; d = SheNextD(d))
        for (f = SheSFaces(d) ; f != FNIL ; f = FacNextF(f))
            area += MSD_lowPropriedadeAreaFace(f);
    return(area);
}

double MSD_lowPropriedadeAreaFace(FPTYPE f)
{
    LPTYPE l;
    double area = 0;

    area = MSD_lowPropriedadeAreaLaco(FacFLOut(f));
    for (l = FacFLoops(f) ; l != LNIL ; l = LooNextL(l))
        if (l != FacFLOut(f))
            area -= MSD_lowPropriedadeAreaLaco(l);
    return(area);
}

/*** Calculo da area do laco ***/
double MSD_lowPropriedadeAreaLaco(LPTYPE l)
{
    HPTYPE he ;
    VPTYPE v1 ;
    vector aa, bb, cc, dd, vv1 ;

    veczer(dd) ;
    he = LooLEdg(l) ;
    v1 = HalVtx(he) ;
    he = HalNxt(he) ;
    do {
        veccopy(vv1, VerVCoord(v1)) ;
        vecminus(aa, VerVCoord(HalVtx(he)), vv1) ;
        vecminus(bb, VerVCoord(HalVtx(HalNxt(he))), vv1) ;
        cross(cc, aa, bb) ;
        vecplus(dd, dd, cc) ;
    } while ((he = HalNxt(he)) != LooLEdg(l)) ;
    return(-0.5 * dot(FacFeq(LooLFace(l)), dd)) ;
}
