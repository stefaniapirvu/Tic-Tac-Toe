#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INFINIT 10000

typedef struct node_t{
    char *nume;
    int val;
    int filme[200];
    struct node_t *next;
    struct node_t *last;
}node_t;

typedef struct graf_t{
    int nr_nod;
    struct node_t *adjList[7000];
}graf_t;

node_t * create_node (char *num, int value){  // se aloca spatiu pentru un nod in graf si atribuie nume si valoare
    node_t *new=(node_t*)calloc(1,sizeof(node_t));
    char *aux=(char*)calloc((strlen(num)+1),sizeof(char));
    strcpy(aux,num);
    new->val=value;
    new->nume=aux;
    new->next=NULL;
    new->last=new;

    return new;
}

int exista_legatura(node_t* nod1 ,node_t* nod2 , int nr_filme){ //verifica daca doi actori joaca impreuna intr-un film, returneaza 1 daca da si 0 daca nu
   int i,ok=0;
    for(i=0;i<nr_filme;i++)
        if(nod1->filme[i]==1 && nod2->filme[i]==1 )
           ok=1;
           
    return ok;
}

void parcurgere ( int nod,int *visited, graf_t * graf, int level, int nr_filme){ //parcurge componenta conexa pornind de la un nod dat
    int i;
    for (i=0;i<graf->nr_nod;i++){
      if(visited[i]==0 && exista_legatura(graf->adjList[nod],graf->adjList[i], nr_filme)==1){ // daca exista o legatura intre cele doua noduri si al 2-lea nu a fost vizitat
        visited[i]=level; //trecem nodul ca vizitat
        parcurgere(i,visited, graf, level,nr_filme);    // apelam parcurgerea pentru nodul deabia vizitat
      }}
}

int cmpfunc (const void *a , const void *b){ // functie de comparare a doua siruri de caractere folosita in qsort
    char **a1=(char**)a;
    char **b1=(char**)b;
    return strcmp(*a1,*b1);
}

void componente_conexe (graf_t *graf,int nr_filme,FILE*g){ // gaseste componenta conexa cu maximul de elemente si afiseaza alfabetic elememtele
    int i=0,k=1;
    int *visited=(int*)calloc(graf->nr_nod+1,sizeof(int));
    int max=0,p=0, componenta=0;
   
    for(i=0;i<graf->nr_nod;i++){// pentru fiecare nod nevizitat se percurge graful
        if(visited[i]==0) {
            visited[i]=k;  
            parcurgere(i,visited,graf,k,nr_filme );
            k++; // la fiecare apelare numarul componentei conexe creste
            }
      }
    int *v_conex=(int*)calloc(k+1,sizeof(int));// vector in care retin cate elememte are fiecare componenta conexa
    
    for(i=0;i<graf->nr_nod;i++){ 
        v_conex[visited[i]]++;
    }
    for(i=1;i<=k;i++)
        if(max < v_conex[i]){ 
            max=v_conex[i]; // gasesc numarul maxim de elemente dintr-o componenta conexa
            componenta=i;// gasesc componenta cu numarul maxim de componente
        }

    char **v=(char**)calloc((max+1),sizeof(char*)); 
    for(i=0;i<graf->nr_nod;i++)
        if (visited[i]==componenta){ 
            v[p]=(char*)calloc((strlen(graf->adjList[i]->nume)+1),sizeof(char));
            strcpy(v[p],graf->adjList[i]->nume); // retin in vector elementele din componenta conexa gasita anterior
            p++;
        }

    fprintf(g,"%d\n",max); // afisez numarul de elemente din componenta conexa
    qsort(v,max,sizeof(char*), cmpfunc); // sortez alfabetic vectorul si il afisez
    for(i=0;i<max;i++)
        fprintf(g,"%s",v[i]);

    for(i=0;i<p;i++)  // eliberez memoria alocata
        free(v[i]);
    
    free(v);
    free(visited);
    free(v_conex);

    }

int exista (graf_t* graf, char *a){ // verifica daca un actor este deja in graf
    int i;
    for(i=0;i<graf->nr_nod;i++){ // se parcurge graful, daca se gaseste actorul se returneaza valoarea sa (pozitia la care se afla)
        if (strcmp(graf->adjList[i]->nume, a)==0)
            return graf->adjList[i]->val;
        }
   
    return -1; // daca nu se gaseste se returneaza -1 (pentru ca 0 este o pozite in graf)
}


node_t* adaug_lista(node_t* last, node_t* urm){ // adauga un nod in lista
    node_t* newnod;
    newnod=create_node(urm->nume , urm->val); 
  
    last->next=newnod;// se adauga dupa ultimul element din lista
    return newnod;
}

void make_graf (FILE *f, FILE *g, graf_t* graf, int nr_filme){
    int i,j, nr_actori,nod_cr=0;
    char a[200];
      for(i=0;i<nr_filme;i++){ //pentru fiecare film se citeste numele si numarul de actori, urmat de actori
        fgets(a,200,f);  //nume film nu il retin pentru ca nu ma folosesc de el, in schimb retin idicele
        fscanf(f,"%d\n",&nr_actori);
        
        for(j=0;j<nr_actori;j++){
            fgets(a,100,f); // pentru fiecare actor, se verifica daca deja este in graf
            if ( exista(graf,a)==-1){ //daca nu este in graf , se adauga
                graf->adjList[nod_cr]=create_node(a, nod_cr); // se greeaza nodul , avand ca nume actorul citit 
                graf->adjList[nod_cr]->filme[i]=1; // in vectorul de filme pun 1 in pozitia filmului din care face parte
                nod_cr++; // se creste numarul de noduri din graf
                graf->nr_nod=nod_cr;  
            }
            else{
                graf->adjList[exista(graf,a)]->filme[i]=1; //daca deja exista, in vectorul de filme pun 1 in pozitia celuilalt film din care face parte
            }
           }
   }

   for (i=0;i<graf->nr_nod;i++){
    for(j=i+1;j<graf->nr_nod;j++) // se parcurg nodurile si se fac legaturile
        if (exista_legatura(graf->adjList[i],graf->adjList[j],nr_filme)){  // daca exista legatura(cel putin un film comun)
            graf->adjList[i]->last=adaug_lista(graf->adjList[i]->last,graf->adjList[j]); //nodul 1 este adaugat in lista vecinilor nodului 2 si invers
            graf->adjList[j]->last=adaug_lista(graf->adjList[j]->last,graf->adjList[i]);
         }
   }

}
void free_lista(node_t *head){  // functie care elibereaza memoria alocata pentru o lista
    node_t *aux=head;
    while(aux!=NULL){ // se parcurge lista
        node_t *urm=aux->next;
       free(aux->nume); // se elibereaza memoria alocata pentru numele nodului
        free (aux); //se elibereaza nodul
        aux=urm;
    }
}

void executa1 (FILE *f, FILE *g){ // functie pentru task1
    int i,nr_filme;
    graf_t* graf=(graf_t*)calloc(1,sizeof(graf_t));     // se aloca spatiu pentru graf
    fscanf(f,"%d\n",&nr_filme); //se citeste numarul de filme
    make_graf(f,g,graf, nr_filme); //se construieste graful
    componente_conexe(graf,nr_filme,g); // se gaseste componenta conexa maxima si se afiseaza alfabetic

    for(i=0;i<graf->nr_nod;i++)
        free_lista(graf->adjList[i]); //se elibereaza fiecare lista

    free(graf);
 }


void parcurgere2 ( int nod,int *visited, graf_t * graf, int level, int nr_filme){
    int i,vecini[graf->nr_nod],p=0; // folosesc un vector auxiliar in care retin indicii vecinilor nodului curent
    for (i=0;i<graf->nr_nod;i++){
      if((visited[i]==0  || visited[i]>level  ) && exista_legatura(graf->adjList[nod],graf->adjList[i], nr_filme)==1){// daca exista o legatura intre cele doua noduri si al 2-lea nu a fost vizitat, sau are nivelul mai mare decat cel curent
        visited[i]=level; 
        vecini[p]=i;
        p++ ;}  
      }
     for (i=0;i<p;i++)
        parcurgere2(vecini[i],visited, graf, level+1,nr_filme);  // se apeleaza parcurgerea pentru fiecare nod din vectorul vecini, cu nivelul urmator level+1
}

  void vizitare (graf_t *graf,int nr_filme,FILE*g, int nod1,int nod2){
    int *visited=(int*)calloc(graf->nr_nod+1,sizeof(int)); //folosesc un vector care imi spune daca un nod a fost vizitat sau nu
   
    visited[nod1]=1;  // setez nodul dat ca vizitat la nivelul 1
    parcurgere2(nod1,visited,graf,2,nr_filme ); //apelez parcurgerea din nodul dat , pornind de la nivelul 2

    if (visited[nod2]==0) // daca nod2 nu a fost vizitat, inseamna ca se afla in alta componenta conexa si se afiseaza -1
        fprintf(g,"-1\n");
    else 
        fprintf(g,"%d\n",visited[nod2]-visited[nod1] ); // altfel se afiseaza diferenta de nivel dintre cele douan noduri
  
    free(visited);
    }

 void executa2 (FILE *f, FILE *g){ // functie pentru task 2
    char *num=(char*)calloc(200,sizeof(char));
    int i,nr_filme;
    int nod1,nod2;
    graf_t* graf=(graf_t*)calloc(1,sizeof(graf_t));     // se aloca memorie pentru gref
    fscanf(f,"%d\n",&nr_filme);
    make_graf(f,g,graf, nr_filme); // se construieste graful

    fgets(num,200,f); // se citesc cele doua nume intre care trebuie facuta legatura si se gaseste valoarea nodului la care se afla in graf
    nod1=exista(graf ,num);
    fgets(num,200,f);
    nod2=exista(graf ,num);

    vizitare(graf,nr_filme,g,nod1,nod2); // se apeleaza functia vizitare
  
    free(num);   //se elibereaza memoria alocata
    for(i=0;i<graf->nr_nod;i++)
        free_lista(graf->adjList[i]);

    free(graf);
    
 }

 void dfsB(graf_t *graf,int v,int timp,int *indx,int *low,int *pi, int *n, char**vector){
     indx[v]=timp;
     low[v]=timp;
     timp=timp+1;
     node_t *list=graf->adjList[v]; //nod auxiliar
     while(list){ // se parcurge lista cu nodurile vecine ale nodului v
        if(list->val !=pi[v]){
            if(indx[list->val]==-1){
                pi[list->val]=v;
                dfsB(graf, list->val,timp,indx,low,pi,n,vector);
                if(low[v]<low[list->val])
                    low[v]=low[v];
                else
                    low[v]=low[list->val];
                
                if(low[list->val]>indx[v]){ // atunci se face punte intre cele doua noduri
              
                    if(strcmp(graf->adjList[list->val]->nume,graf->adjList[v]->nume)<0) //se verifica care nod este primul alfabetic 
                        {
                            strcpy(vector[(*n)],graf->adjList[list->val]->nume); //se copiaza in vector
                            vector[(*n)][strlen(vector[(*n)])-1]=' ';// se inlocuieste \n de la sfarsit cu spatiu 
                            strcat(vector[(*n)],graf->adjList[v]->nume); //se adauga si celalalt nume
                          
                            (*n)=(*n)+1; // numarul de punti creste
                            

                        }
                        else{
                          
                            strcpy(vector[(*n)],graf->adjList[v]->nume); // se copiaza intai celalalt nume si se procedeaza la fel
                            vector[(*n)][strlen(vector[(*n)])-1]=' ';
                            strcat(vector[(*n)],graf->adjList[list->val]->nume);
                            
                            (*n)=(*n)+1;

                        }
                }
             }
            else
                {if(low[v]<indx[list->val])
                    low[v]=low[v];
                else
                    low[v]=indx[list->val];}
        }

        list=list->next; // se trece la urmatorul element din lista
     }

 }


 void punti (graf_t *graf,int *n, char **vector){
     int i, timp=0;
    int *pi=(int*)malloc(graf->nr_nod*sizeof(int)); // am alocat memorie pentru cei 3 vectori
    int *indx=(int*)malloc(graf->nr_nod*sizeof(int));
    int *low=(int*)malloc(graf->nr_nod*sizeof(int));
    for(i=0;i<graf->nr_nod;i++){ // am initializat toate elementele din vector
        indx[i]=-1;
        low[i]=INFINIT;
        pi[i]=0;
    }
     for(i=0;i<graf->nr_nod;i++){  // se apeleaza dfsB pentru fiecare nod nevizitat
        if(indx[i]==-1)
           dfsB(graf,i,timp,indx,low,pi,n,vector);
       
    }

    free(pi); // eliberez memoria alocata pentru cei 3 vectori
    free(indx);
    free(low);
 }

void executa3(FILE *f, FILE *g){
    int i,nr_filme;
    graf_t* graf=(graf_t*)calloc(1,sizeof(graf_t));  // se aloca memorie pentru graf
    fscanf(f,"%d\n",&nr_filme);
    make_graf(f,g,graf, nr_filme);  // se construieste graful

    int n=0;
    char **vector=(char**)calloc(7000,sizeof(char*)); // aloc memorie pentru un vector de siruri de caractere auxiliar in care o sa retin lista cu numele perechilor de actori
    for(i=0;i<7000;i++)
        vector[i]=(char*)calloc(7000,sizeof(char));

    punti(graf,&n,vector);  // se apeleaza functia punti pentru n=0 primul nod , am urmat pseudocodul
    fprintf(g,"%d\n",n);
    qsort(vector,n,sizeof(char*), cmpfunc);// se sorteaza alfabetic vectorul si se afiseazaz
    for(i=0;i<n;i++)
        fprintf(g,"%s",vector[i]);

    for(i=0;i<7000;i++)
        free(vector[i]);
    
    free(vector);   // se elibereaza memoria

    for(i=0;i<graf->nr_nod;i++)
    free_lista(graf->adjList[i]);
    free(graf);

   

}

int main(int argc, char *argv[] ){

    FILE *f=fopen (argv[2],"r");
    FILE *g=fopen (argv[3],"w");
    if(strcmp(argv[1],"-c1")==0)
      executa1(f,g);

    if(strcmp(argv[1],"-c2")==0)
        executa2(f,g);

    if(strcmp(argv[1],"-c3")==0)
        executa3(f,g);
    

    fclose(f);
    fclose(g);

    return 0;
}