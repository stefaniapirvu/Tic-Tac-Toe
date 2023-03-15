#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct tree_t {  // structura arbore care retine fiecare tabla de x si 0
    char tabla[4][4];
    struct tree_t *next[9]; // vector de fii
    struct tree_t *prev; //parintele
    int nodes; // numarul de fii
    int is_true; // valoare de adevar a nodului

}tree_t;


 int level (tree_t*joc){ // nivelul la care se afla un nod in arbore
    int k=0;
    tree_t *aux=joc;
    while (aux->prev!=NULL){ // se parcurge de la nod pana la nodul parinte 
        k++;   // se incrementeaza nivelul
        aux=aux->prev;
    }
    
    return k;    
 }

 tree_t * newnod( tree_t *joc ){ // creaza un nou nod in arbore 
    tree_t *new;
    new=(tree_t*)calloc(1,sizeof(tree_t)); //se aloca memorie

    new->prev=joc;
    return new;
}

 void print_table (tree_t *joc , FILE *g){ // functie care printeaza matricea de x si 0
    int i,j;

    for(i=0; i<3; i++){
        for(j=0;j< level(joc); j++) // in functie de nivelul nodului la care este matricea se printeaza tab
             fprintf(g ,"\t");
        fprintf(g ,"%c %c %c",joc->tabla[i][0],joc->tabla[i][1],joc->tabla[i][2]);
        fprintf(g,"\n");
    }
    fprintf(g,"\n");
 }

   
  void print_is_true(tree_t *joc , FILE *g){
    int j;
    
        for(j=0;j< level(joc); j++)// in functie de nivelul nodului la care este matricea se printeaza tab
            fprintf(g ,"\t");
            
        if(joc->is_true == 1)  // daca valoarea de adevar a nodului este 1 se printeaza T, altfel F
            fprintf(g ,"T");
        else 
            fprintf(g ,"F");
        fprintf(g,"\n");
    
 }
 
void print_tree(tree_t*joc , FILE *g ){ // se printeaza arborele cu matrici pentru task1 , arborele se parcurge recursiv
    int i;
    if(joc==NULL) 
        return;
    int nr=joc->nodes;
        print_table(joc,g);
        for(i=0;i<=nr;i++)
        print_tree(joc->next[i],g);
       
}
void print_tree_is_true(tree_t*joc , FILE *g ){ // se printeaza arborele cu T si F pentru task2 , arborele se parcurge recursiv
    int i;
    if(joc==NULL)
        return;
    int nr=joc->nodes;
        print_is_true(joc,g);
        for(i=0;i<=nr;i++)
        print_tree_is_true(joc->next[i],g);
       
}





int final (tree_t *joc , char jucator){ // returneaza 1 daca nodul este frunza
    int i,j ,k=0;

    for (i=0; i<3; i++)
        if (joc->tabla[i][0]== jucator && joc->tabla[i][1]== jucator && joc->tabla[i][2]== jucator ) // daca acelasi jucator este pe o linie
            return 1;
                
    for (j=0; j<3; j++)
        if (joc->tabla[0][j]== jucator && joc->tabla[1][j]== jucator && joc->tabla[2][j]== jucator )//daca acelasi jucator este pe o coloana
            return 1;

    if (joc->tabla[0][0]== jucator && joc->tabla[1][1]== jucator && joc->tabla[2][2]== jucator ) //daca acelasi jucator este pe diagonala
            return 1;
    if (joc->tabla[0][2]== jucator && joc->tabla[1][1]== jucator && joc->tabla[2][0]== jucator ) //daca acelasi jucator este pe diagonala secundara
            return 1;
    for (i=0; i<3; i++)
        for (j=0; j<3; j++)
        if (joc->tabla[i][j]== '-')  // daca nu mai sunt locuri libere pe tabla , nodul este frunza
            k++;
     
    if (k==0)
        return 1;

    return 0;
    
}

int castig (tree_t *joc , char jucator){ // returneaza 1 daca jucatorul a castigat 
     int i,j;

    for (i=0; i<3; i++)
        if (joc->tabla[i][0]== jucator && joc->tabla[i][1]== jucator && joc->tabla[i][2]== jucator ) // a castigat daca acelasi jucator este pe o linie
            return 1;
                
    for (j=0; j<3; j++)
        if (joc->tabla[0][j]== jucator && joc->tabla[1][j]== jucator && joc->tabla[2][j]== jucator )// a castigat daca acelasi jucator este pe o coloana
            return 1;

    if (joc->tabla[0][0]== jucator && joc->tabla[1][1]== jucator && joc->tabla[2][2]== jucator ) // a castigat daca acelasi jucator este pe diagonala
            return 1;
    if (joc->tabla[0][2]== jucator && joc->tabla[1][1]== jucator && joc->tabla[2][0]== jucator )// a castigat daca acelasi jucator este pe diagonala secundara
            return 1;

    return 0;
}

 char switch_jucator(char jucator){ // schimba randul jucatorilor 
    char jucator2;
    if (jucator=='X') // daca jucatorul curent este X, este randul lui 0 si noul jucator curent este 0
        jucator2='O';
    else 
        jucator2='X';

    return jucator2;
 }

 void copy_tabla(tree_t* joc, int k){ // se copiaza tabla dintr-un nod in fiul k al acestuia
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
             joc->next[k]->tabla[i][j]=joc->tabla[i][j];
 }

 void next_move(tree_t* joc , FILE *g, char jucator){ // urmatoarea mutare pentru task 1
     int i,j,k;
    if (final(joc , switch_jucator(jucator))==0){ // daca jocul nu se afla la final, nodul nu este frunza
        k=-1;
        for(i=0;i<3;i++)
        for(j=0;j<3;j++){
            if (joc->tabla[i][j]=='-'){ // fiecare spatiu liber este inlocuit cu jucatorul curent 
                 k++;
                joc->next[k]=newnod(joc);
                joc->nodes=k;
                copy_tabla(joc,k);
                joc->next[k]->tabla[i][j]=jucator;
                next_move(joc->next[k],g  , switch_jucator(jucator)); // functia se apeleaza recursiv pentru urmatorul jucator
            }
        }       
    }
 
 }

 void free_mem(tree_t*joc){ // functie de eliberare  memoriei alocate
    int i;
    int nr=joc->nodes; 
    for(i=0;i<=nr;i++)
        if(joc->next[i]){ 
            free_mem(joc->next[i]); // se parcurge recursiv arborele 
            free(joc->next[i]); // se elibereaza memoria pentru fiecare nod
        }

 }

void executa1( FILE *f, FILE *g){  // functie pentru taskul 1
    char jucator,a;  //  jucatorul poate fi x sau 0
    int i,j;
    tree_t *joc=(tree_t*)calloc(1,sizeof(tree_t)); // se aloca memorie pentru primul nod al arborelui
    jucator = fgetc(f); // se citeste jucatorul curent
    a=fgetc(f);

    for(i=0;i<3;i++){ // se citeste tabla in primul nod
        for(j=0;j<3;j++){
            a=fgetc(f);
            joc->tabla[i][j]=a;
            a=fgetc(f);
        }  
    }

    next_move(joc,g,jucator); //se apeleaza functia recursiva de next_move
    print_tree(joc ,g); // afiseaza arborele
    free_mem(joc);// se elibereaza memoria arborelui
    free(joc); // se elibereaza memoria alocata pentru primul nod
   
}

int si_sau(tree_t* joc, char jucator){ // functia care determina recursiv valoarea de adevar a fiecarui nod
    int i,sum=0;
    int nr=joc->nodes+1;
    if(joc->next[0]==NULL && castig(joc,jucator)==1) // daca nodul este frunza se  verifica daca jucatorul este castigator
        joc->is_true=1;
    if(joc->next[0]==NULL && castig(joc,jucator)==0)  // daca nodul este frunza se  verifica daca jucatorul nu este castigator
        joc->is_true=0;
    
    if(joc->next[0]!=NULL){    
        for(i=0;i<nr;i++)
            sum=sum+ si_sau(joc->next[i], jucator); //se calculeaza suma valorilor de adevar a nodurilor fii

        if (level(joc)%2==0 && sum >= 1) //daca se afla pe nivel par(primul nivel este 0) se face sau . este suficient ca suma sa fie 1
                joc->is_true=1;
        if (level(joc)%2==0 && sum==0)   
                joc->is_true=0;
        
        if (level(joc)%2 !=0 && sum ==nr)//daca se afla pe nivel impar(primul nivel este 0) se face si . este necesar ca suma sa fie egala cu nr de fii
                joc->is_true=1;
        if (level(joc)%2 !=0 && sum!=nr)   
                joc->is_true=0;  
        }  
        return joc->is_true;

}

void executa2 (FILE *f, FILE *g){// functie pentru taskul 1
        char jucator,a; //  jucatorul poate fi x sau 0
    int i,j;
     tree_t *joc=(tree_t*)calloc(1,sizeof(tree_t));// se aloca memorie pentru primul nod al arborelui
    jucator = fgetc(f);// se citeste jucatorul curent
    a=fgetc(f);

    for(i=0;i<3;i++){// se citeste tabla in primul nod
        for(j=0;j<3;j++){
            a=fgetc(f);
            joc->tabla[i][j]=a;
            a=fgetc(f);
        }  
    }

   next_move(joc,g,jucator);//se apeleaza functia recursiva de next_move
   si_sau(joc,jucator); // se apeleaza functia si/sau pentru a determina valoarea de adevar a nodurilor

   print_tree_is_true(joc,g);// afiseaza arborele
   free_mem(joc);// se elibereaza memoria arborelui
    free(joc);// se elibereaza memoria alocata pentru primul nod
}


int main(int argc, char *argv[] ){

    FILE *f=fopen (argv[2],"r");
    FILE *g=fopen (argv[3],"w");
  
    if(strcmp(argv[1],"-c1")==0)
        executa1(f,g);

    if(strcmp(argv[1],"-c2")==0)
        executa2(f,g);

    fclose(f);
    fclose(g);

    return 0;
}