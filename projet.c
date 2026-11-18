/*Ce programme est un problème de répartition de cadeaux entre plusieurs enfants. Chaque enfant possède une satisfaction différente pour chaque cadeau.L’objectif est de distribuer les cadeaux de manière à maximiser la satisfaction minimale parmi tous les enfants.Un cadeau ne peut être donné qu'une fois 
l'objectif : trouver une affectation de chaque cadeau telle que la
satisfaction de l’enfant le moins satisafait est maximale à l'aide de recuit */

#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/resource.h>

#define nb_enfant 100 /* capacité max des enfants pareil pour cadeau */

#define nb_cadeau 200


typedef struct { /*libre et id pour ne pas donner le même cadeau */
    int id;
    int libre;   
} cadeau;


typedef struct { /* on veut la liste de cadeaux pris pour la satisfaction,la satisfaction de chaque enfant pour chaque cadeau,le nb cadeaux pris et la sat finale */
    int id;
    int satisfaction_cadeaux[nb_enfant];
    int cadeaux_pris[nb_cadeau];
    int nb_cadeaux;
} enfant;

int charger_donnees(char *nomFichier, enfant enfants[], int *n, int *m) {
    int i ;
    int j ;
    FILE *f = fopen(nomFichier, "r"); /* ouvre le fichier en mode lecture  */
    if (f == NULL) { /* si on arrive pas à ouvrir le fichier */
        printf("Impossible d'ouvrir %s\n", nomFichier);
        return 0;
    }

    
    if (fscanf(f, "%d %d", n, m) != 2) return 0; /* on teste si on a bien nb enfants et nb cadeaux */

   
    for (i = 0; i < *n; i=i+1) { /*on initialise les données en même temps que la lecture du fichier  */
        enfants[i].id = i; /* on assigne id et 0 pour nombre de cadeaux pris */
        enfants[i].nb_cadeaux = 0;
        for (j = 0; j < *m; j=j+1) { /*on lit le fichier et on assigne la sat pour chaque enfant et chaque cadeau */
            fscanf(f, "%d", &enfants[i].satisfaction_cadeaux[j]);
        }
    }

    fclose(f);/*on ferme le fichier  */
    return 1;
}


int calcul_satisfaction(enfant *eleve){ /*avant d'utiliser la satisfaction normalisée nous avions utilisé cette fonction */
    int i;
    int total = 0;
    for(i = 0; i < eleve->nb_cadeaux; i=i+1){
        total = total + eleve->satisfaction_cadeaux[eleve->cadeaux_pris[i]];
    }
    return total;
}


void satisfaction_aleatoire(enfant enfants[],int n,int m){ /*avant de tout mettre dans information.txt ,les satisfactions étaient crées au hasard */
      int i ;
      int j;
      for(i=0;i<=n-1 ; i = i+1){
        for (j=0; j<=m-1 ; j = j +1){
            enfants[i].satisfaction_cadeaux[j] = 1 + rand()%10;}}
            }
            
            
float satisfaction_normalisee(enfant *e, int nb_cadeaux_total) { /*calcul la sat normalisée d'un enfant */
    int i;
    int somme_recue = 0; /*on utilise pour calculer la sat */
    int somme_totale = 0; /* utilisée pour la normalisation */
    int id_cadeau ;

    for (i = 0; i < nb_cadeaux_total; i=i+1) { /*on additione tout les resutlats possibles  */
        somme_totale = somme_totale + e->satisfaction_cadeaux[i];
    }

    if (somme_totale == 0) /* nous avons utilisé cela car nous avons eu un problème de divison par zero */
        return 0.0f;

    for (i = 0; i < e->nb_cadeaux; i=i+1) { /* calcul la sat en fonction des cadeaux détenu par l'enfant*/
        id_cadeau = e->cadeaux_pris[i];
        somme_recue = somme_recue +  e->satisfaction_cadeaux[id_cadeau];
    }

    return (float)somme_recue / (float)somme_totale; /* sat normalisee */
}
        
        
float energie_normalisee(enfant enfants[], int n, int nb_cadeaux_total) { /* cette fonction nous donne la sat la plus petite */
    int i ;
    float min_sat ;
    float s;
    min_sat = satisfaction_normalisee(&enfants[0], nb_cadeaux_total); /*on init avec l'enfant le plus petit */
    for (i = 1; i < n; i=i+1) { /*on parcours pour chaque enfant et on compare */
        s = satisfaction_normalisee(&enfants[i], nb_cadeaux_total);
        if (s < min_sat) min_sat = s;
    }
    return min_sat;
}
            

void solutiondepart(enfant enfants[], cadeau cadeaux[], int n, int m) { /*on distribue les cadeaux */
    int i;
    int j ;
    for(j = 0; j < m; j=j+1){ /*initialiser cadeaux */
        cadeaux[j].id = j;
        cadeaux[j].libre = 1;
    }
    
    for (i = 0; i < n; i=i+1) { /*on en profite pour tout initialiser ici */
        enfants[i].nb_cadeaux = 0;
    }

    for (j = 0; j < m; j=j+1) {/*on parcours les cadeaux */
        int destinataire = j % n; /*on prends un enfant  */
        
        enfants[destinataire].cadeaux_pris[enfants[destinataire].nb_cadeaux] = cadeaux[j].id; /*on lui donne le cadeau j */
        enfants[destinataire].nb_cadeaux++; /*on augmente sa taille */
        cadeaux[j].libre = 0; /*le cadeau ne peut plus être donné */
    }
}


void transition_une(enfant *e1, enfant *e2) { /*donne un cadeau de e1 pour e2 */
    int i ;
    int ind_cadeau_a_retirer ;
    int cadeau_id ;
    if (e1->nb_cadeaux <= 0) { /*e1 ne peut rien donner si il a rien */
        return; 
    }

    
    ind_cadeau_a_retirer = rand() % e1->nb_cadeaux; /*on prends un indice au hasard parmis ceux qu'il a  */
    cadeau_id = e1->cadeaux_pris[ind_cadeau_a_retirer]; /*on prends le bon cadeau */

    
    for (i = ind_cadeau_a_retirer; i < e1->nb_cadeaux - 1; i=i+1) { /*on décale le tab */
        e1->cadeaux_pris[i] = e1->cadeaux_pris[i + 1];
    }
    e1->nb_cadeaux--; /* il a un cadeau en moins */

    
    e2->cadeaux_pris[e2->nb_cadeaux] = cadeau_id; /*on donne a e2 le cadeau */
    e2->nb_cadeaux++; /*e2 a un cadeau en plus  */
}

void transition_deux(enfant *e1, enfant *e2) { /*echange de cadeau entre e1 et e2 */
    int temp;
    int ind1;
    int ind2;
    if (e1->nb_cadeaux <= 0 || e2->nb_cadeaux <= 0) { /*si les deux n'ont pas de cadeau alors on arrête */
        return;
    }

    ind1 = rand() % e1->nb_cadeaux; /*on prend 2 cadeaux 1 à e1 et 1 à e2 de manière aléatoire*/
    
    ind2 = rand() % e2->nb_cadeaux;

    
    temp = e1->cadeaux_pris[ind1]; /*on échange */
    e1->cadeaux_pris[ind1] = e2->cadeaux_pris[ind2];
    e2->cadeaux_pris[ind2] = temp;

    
}


void echange(enfant avant[],enfant apres[], int n ){ /*sauvegarde de la solution pour l'enfant */
    int i;
    for(i=0; i < n ; i = i + 1){
        apres[i] = avant [i] ;}}

/*int min(enfant enfants[],int n){
    int i ;
    int s ;
    int min= calcul_satisfaction(&enfants[0]) ;
    for(i=0;i<=n-1;i=i+1){
      s = calcul_satisfaction(&enfants[i]);
      if(s <= min ){
      min =  s ; }
    }
    return min ;
}

int energie(enfant enfants[],int n){
    int resultat ;
    resultat = min(enfants,n);
    return resultat ;
    }
*/


void recuit(enfant enfants[], int n) { /*recuit simulé */
    float Ey, E, diff, d;
    int iter_max = 5000; 
    int iter_en_cours = 0;
    int i ;
    int j;
    int t ;
    float temp = 1.0;         
    float temp_final = 0.0001; 
    float refroidissement = 0.99;
    enfant enfants_un[nb_enfant];  
    enfant enfants_deux[nb_enfant];
    echange(enfants, enfants_un, n);
    E = energie_normalisee(enfants_un, n, nb_cadeau); /*on calcule l'énergie de départ */
    while (temp > temp_final && iter_en_cours < iter_max) { /*TQ système n'est pas gelé */
        echange(enfants_un, enfants_deux, n);
        i = rand() % n; /*choisir l'enfant */
        do {
            j = rand() % n;
        } while (i == j);
        t = (rand() % 2)+1; /*choisir une transition */
        if (t ==1){ /*choisit la trans 1 */
        if (enfants_deux[i].nb_cadeaux > 0) {
            transition_une(&enfants_deux[i], &enfants_deux[j]);
        }}
        else if (t==2){ /*choisir la trans 2 */
          if (enfants_deux[i].nb_cadeaux > 0 && enfants_deux[j].nb_cadeaux > 0) {
                transition_deux(&enfants_deux[i], &enfants_deux[j]);
            }}

            Ey = energie_normalisee(enfants_deux, n, nb_cadeau); /*on calcule nouvelle énergie */
            diff = Ey - E; 
            d = (float)rand() / RAND_MAX; /*on génère un d aléatoire */
            if (d < exp(diff / temp)) { /*on garde que certaines transitions */
                echange(enfants_deux, enfants_un, n); /* on garde la solution */
                E = Ey;
            }
      
        temp *= refroidissement; /*on refroidit le système */
        iter_en_cours = iter_en_cours +1; 
    }
    
    
    echange(enfants_un, enfants, n);
}


double temps_CPU()
{
	struct rusage usage;

	getrusage(RUSAGE_SELF, &usage);
	return usage.ru_utime.tv_sec + (usage.ru_utime.tv_usec / 1000000.0);
}






int main() {
    int n, m;
    int i ;
    double START, END;
    enfant enfants[nb_enfant];
    cadeau cadeaux[nb_cadeau];
    
   /* srand(8); */
    srand(time(NULL));

    START = temps_CPU();
    if (!charger_donnees("information.txt", enfants, &n, &m)) {
        return 1; 
    }

    solutiondepart(enfants, cadeaux, n,m); 

    printf("--- AVANT RECUIT ---\n");
    for(i = 0; i < n; i=i+1) {
        printf("Enfant %d : %d cadeaux | Sat normalisee : %f\n", 
                i, enfants[i].nb_cadeaux, satisfaction_normalisee(&enfants[i], m));
    }
    printf("Energie (min sat) : %f\n", energie_normalisee(enfants, n, m));

    
    recuit(enfants, n); 

    printf("\n--- APRES RECUIT ---\n");
    for(i = 0; i < n; i=i+1) {
        printf("Enfant %d : %d cadeaux | Sat normalisee : %f\n", 
                i, enfants[i].nb_cadeaux, satisfaction_normalisee(&enfants[i], m));
    }
    printf("Energie finale (min sat) : %f\n", energie_normalisee(enfants, n, m));
    END = temps_CPU();
        printf("\nCPU time : %.4f s\n", END-START);
    return 0;

}
