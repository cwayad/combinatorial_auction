#include <stdlib.h>
#include<stdio.h>
#include <time.h>
/*Structure de Données*/
typedef struct suite suite;
struct suite
{
    int clause;
    suite *svt;
} ;

typedef struct Liste Liste;
struct Liste
{
    suite *clau;
    float prix;
    float eval;
    int redendance;
    float critere;
};
/*Initialisation d'une nouvelle liste chainée*/
Liste *initialisation()
{
    Liste *liste = malloc(sizeof(*liste));
    suite *element = malloc(sizeof(*element));
    if (liste == NULL || element == NULL)
    {
        exit(EXIT_FAILURE);
    }
    element=NULL;
    liste->clau = element;
    return liste;
}

int card(Liste *liste)
{
    if (liste == NULL)   return 0;
    suite *actuel = liste->clau;
    int nb=0;
    while (actuel != NULL)
    {
        nb++;
        actuel = actuel->svt;
    }
    return nb;
}

int exist(Liste *liste,int a)
{
    if (liste==NULL) return 0;
    suite *actuel = liste->clau;
    while (actuel != NULL)
    {
        if (a==actuel->clause) return 1;
        actuel = actuel->svt;
    }
    return 0;
}

void inserer(Liste *liste, int nvNombre)
{
    suite *nouveau = malloc(sizeof(*nouveau));
    nouveau->clause = nvNombre; /* Insertion de l'élément au début de la liste */
    nouveau->svt=liste->clau;
    liste->clau=nouveau;
}

int depiler(Liste *liste)
{
    if (liste== NULL)
    {
    }
    if (liste->clau != NULL)
    {
        suite *aSupprimer = liste->clau;
        int a=aSupprimer->clause;
        liste->clau=aSupprimer->svt;
        free(aSupprimer);
        return a;
    }
}

int tete(Liste *liste)
{
    if (liste == NULL) exit(EXIT_FAILURE);
    if (liste->clau != NULL)
    {
        return liste->clau->clause;
    }
    return -1;
}

////////////////////////////////////////////////////////
int main()
{
    clock_t t1, t2;
    int i,j,k,res,offr=1000,obj=500,f=0,in,nb=1,pil,sec=0,nbr=0,maxred=0;
    float temps=0,maxeval=0;
    char s[10]="in200.txt";
    suite *a=NULL;
    suite *b=NULL;
    Liste *prems=initialisation();
    Liste *ppp=initialisation();
    Liste *meilleur=NULL;
    Liste *second=initialisation();
    Liste *objet[obj];
    Liste *offre[offr];
    Liste *index[offr];
    i=0;

    int redand[obj];
    while(i<offr)
    {
        index[i]=initialisation();
        offre[i]=initialisation();
        i++;
    }
    i=0;
    while(i<obj)
    {
        redand[i]=0;
        objet[i]=initialisation();
        i++;
    }
///////////////////////////////////
    FILE* fichier = NULL;
    fichier = fopen(s, "r+");
    char car[1000];
    i=0;
    fgets(car,1000,fichier);
    /*Lecture Du Benchmark*/
    while(f!=-1)
    {
        fscanf(fichier,"%d",&f);
        in=fgetc(fichier);

        if (in=='.') //On est sur une nouvelle offre
        {            fscanf(fichier,"%d",&j);
            in=fgetc(fichier);

            if (i>0)
            {
                offre[i-1]->eval=(float)(offre[i-1]->prix/nbr);
                if (offre[i-1]->eval<maxeval) maxeval=offre[i-1]->eval;
            }

            offre[i]->prix=f+((float)j/1000);
            i++;
            fscanf(fichier,"%d",&f);
            in=fgetc(fichier);
            nbr=0;
        }
        if (f==-1) //On lit la derniére offre
        {
            offre[i-1]->eval=(float)(offre[i-1]->prix/nbr);
            if ((float)(offre[i-1]->prix/nbr)>maxeval)
            {
                maxeval=offre[i-1]->eval;
            }
            break;
        }
        inserer(offre[i-1],f);
        inserer(objet[f-1],i-1);
        redand[f]++;
        nbr++;
    }

    fclose(fichier);
    int min=0,kl=0;
    int intermediaire[offr],intermediaire2[offr],fff=0;
    i=0;
    /*Remplire les critéres de chaque offre*/
    while (i<offr)
    {
        if (redand[i]>maxred) maxred=redand[i];
        i++;
    }
    i=0;
    /*Remplire les critéres de chaque offre*/
    while (i<offr)
    {
        intermediaire[i]=1;
        a=offre[i]->clau;
        in=0;
        kl=0;
        while(a!=NULL)
        {
            k=a->clause;
            b=objet[k-1]->clau;
            while(b!=NULL)
            {
                if(b->clause!=i)
                {
                    if (exist(index[i],b->clause)==0)
                        inserer(index[i],b->clause);
                }
                b=b->svt;
            }
            in+=redand[a->clause-1];
            kl++;
            a=a->svt;
        }
        offre[i]->redendance=in;
        offre[i]->critere=(float)((1-in/maxred)+ 3*(offre[i]->eval/maxeval));
        i++;
    }
    nb=0;
    t1=clock();
    /***************************Traitement***************************/
    int choix=0,pr=offre[0]->prix,nn=0,eval=0,boucler=0;//offre[0]->critere;
    meilleur=initialisation();
    meilleur->prix=0;
    while(boucler<offr)
    {
        i=0;
//Initialiser le vecteur intermediaire à 1
        while(i<offr)
        {
            intermediaire[i]=1;
            i++;
        }
//choisir l'offre avec laquelle initialiser
        a=index[boucler]->clau;
        while(a!=NULL)
        {
            intermediaire[a->clause]=0;
            a=a->svt;
        }

        i=0;
//Selectionner les prochaines offres
        free(ppp);
        ppp=initialisation();
        while(i<offr)
        {
            if (intermediaire[i]==1 & i!=boucler)
            {
                inserer(ppp,i);
            }
            i++;
        }
//Si la combinaison actuelle est meilleure, on remplace
        a=ppp->clau;
        while(a!=NULL)
        {
            b=index[a->clause]->clau;
            while(b!=NULL)
            {
                intermediaire[b->clause]=0;
                b=b->svt;
            }
            i=0;
            fff=0;
            while(i<offr)
            {
                if (intermediaire[i]==1)
                {
                    fff+=(int)offre[i]->prix;
                    if(i!=boucler & i!=a->clause)
                    {
                        b=index[i]->clau;
                        while(b!=NULL)
                        {
                            intermediaire[b->clause]=0;
                            b=b->svt;
                        }
                    }
                }
                i++;
            }


        if (fff>meilleur->prix)
        {
            free(meilleur);
            meilleur=initialisation();
            i=0;
            meilleur->prix=fff;
            while(i<offr)
            {
                if (intermediaire[i]==1)
                    inserer(meilleur,i);
                i++;
            }
        }

            a=a->svt;
        }

        boucler++;
    }

    /***************************Fin Traitement***************************/
    //suppression(meilleur,3);
    a=meilleur->clau;

    while (a!=NULL)
    {
        printf("%d  ",a->clause);
        a=a->svt;
    }
    printf("\n%d",card(meilleur));

    printf("\n  %f\n",meilleur->prix);
    t2=clock();
    temps=(float)(t2-t1)/CLOCKS_PER_SEC;
    printf(" \n*****END***** %s   %f\n",s,temps);
}
