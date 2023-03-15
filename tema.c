#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node{  // lista dublu inlantuita care retine textul citit / editat
    char val;
    struct node *next;
    struct node *prev;
}node_t;


typedef struct stiva_t{  // stiva in care sunt stocate comenzile
    char *comm;
    struct stiva_t *next;
    
}stiva_t;


void save( node_t *head, FILE **g );
node_t* backspace( node_t *cursor, node_t *head);
node_t* applycommand( char *comanda, node_t *head,node_t *cursor, stiva_t* first,FILE **g,FILE *f, char* sirsters);
stiva_t* adaugcomanda(stiva_t *first, char *comanda);
node_t* adaug(node_t *head, char x, node_t *cursor) ;
node_t* gotoline(node_t *head, int nrlinie);
node_t* goto_character (node_t *head, node_t *cursor, int nrcaracter, int nrlinie);
 node_t* delete( node_t *head, node_t*cursor,int nrcaractere);
node_t* deleteline(node_t *head, node_t* cursor, int nrlinie );
node_t* undo (node_t * head, node_t*cursor, stiva_t* first, char *sirsters);
node_t* redo (node_t * head, node_t*cursor, stiva_t* first, char*sirsters);
node_t* delete_word (node_t *head, node_t*cursor, char *parametru1);
node_t* find_word( node_t*cursor, char *parametru1 );
node_t *insert( node_t *cursor, char x);
char * copiere(node_t*cursor, char*sirsters);
node_t * replace_word(node_t *head, node_t*cursor,char *parametru1, char *parametru2);
node_t *replace_all_word (node_t* head, node_t*cursor,char*parametru1, char*parametru2);
node_t* delete_all_word(node_t *head, node_t*cursor, char *parametru1);

stiva_t* adaugcomanda(stiva_t *first, char *comanda){  // adauga o comanda citita in stiva
    stiva_t *temp;
    char *nume;
    nume=(char*)malloc((strlen(comanda)+1)*sizeof(char)); // se aloca spatiu pentru sirul de caractere care va retine comanda
    strcpy(nume,comanda);
    temp = (stiva_t*)malloc(sizeof(stiva_t));
    temp->comm =nume;
    if (first == NULL)
    {
    temp->next = NULL;
    first = temp;
    }
    else 
    {
    temp->next=first;
    first = temp;
    }

    return first;
}
node_t* adaug(node_t *head, char x, node_t *cursor) { // adauga un nod in lista , fiecare nod retine un caracter al textului
    node_t *temp, *aux;
    temp = (node_t*)malloc(sizeof(node_t));
    temp->val = x;
    if (head == NULL) 
    {
    temp->next = NULL;
    temp->prev= NULL;
    head = temp;
    return head;
    }   
    else 
    {
    aux = head;
    while (aux->next != NULL)
    aux = aux->next;
    
    aux->next = temp;
    temp->prev=aux; 
    temp->next = NULL;
   

}   cursor=temp;
    return cursor;
}

void save( node_t *head, FILE **g ){ //printeaza noul text editat si salvat in fisierul deiesire
    fclose(*g);
   *g=fopen ("editor.out","w");
    node_t *aux=head;
      while(aux){
        fprintf(*g,"%c",aux->val);
        aux=aux->next;
    }
   
}

node_t* gotoline(node_t *head, int nrlinie){ //intoarce nodul la care se afla primul caracter dintr-o anumita linie
    int k=0;
    node_t *aux =head;
    while(k < nrlinie-1 && aux->next!=NULL)
    { if (aux->val=='\n')
        k++;

      aux=aux->next; 
    }
    return aux;
}
node_t* goto_character (node_t *head, node_t *cursor, int nrcaracter, int nrlinie){ // intoarce nodul la care se afla primul caracter dintr-o anumita linie, la o anumita pozitie
    int p=1;
    node_t *aux3=cursor;
    if (nrlinie !=0){
        aux3=gotoline(head,nrlinie); // apeleaza functia goline dupa care se muta pana la caracterul dorit
        while (p < nrcaracter){
            aux3=aux3->next; 
            p++;
        }
            
        }
    else {
        while ((aux3->prev!= NULL)&&(aux3->prev->val!='\n')) // daca nrlinie =0 se muta pana la caracterul dorit din linia curenta 
            aux3=aux3->prev;
        
        while (p < nrcaracter){
            aux3=aux3->next; 
            p++;    
          }    
         }
        
    return aux3;
}

node_t* deleteline(node_t *head, node_t* cursor, int nrlinie ){  // sterge o linie intreaga (sterge elementele intre doua "\n")
    node_t*aux=cursor;
    
    if (nrlinie==1){  // daca nr liniei este 1 se muta si head(inceputul de lista )
        aux=goto_character(head,cursor,1,2);
        aux=aux->prev;
        while(aux!=head){
            aux=backspace(aux,head);
        }
        aux=head;
        head=head->next;
      
        return head;
    }
    else
    {   aux=goto_character(head,cursor,1,nrlinie); // se parcurge linia si se sterge fiecare element pana la "\n"
        while(aux->val!='\n'){
            aux=backspace(aux,head);
            aux=aux->next;
        }
        aux=backspace(aux,head);
        if (nrlinie==0)
            return aux->next;
        else 
            return cursor;
        }
    
    }

node_t* backspace( node_t *cursor, node_t *head){ //sterge elementul dinaintea cursorului
     node_t *aux=cursor;
    if (cursor->prev!=NULL){
        
        node_t *prec ,*urm;
        prec=aux->prev;
        if(aux->next!= NULL) urm=aux->next;
        else urm=NULL;
        if(aux!=prec){
            if(urm!=NULL) prec->next=urm;
            else prec->next=NULL;
            if(urm!=NULL) urm->prev=prec;
            cursor=prec;
             }
    }
    else {                          //daca  cursorul este dupa primul element se muta si head
       
        cursor=aux->next;
        head=aux->next;
        

    }
    free(aux);
    return cursor;
}
 node_t* delete( node_t *head, node_t*cursor,int nrcaractere){ // sterge nodul de dupa cursor si returneaza pozitia a cursorului 
     int k=0;
     while( k<nrcaractere)
     {cursor=backspace(cursor->next, head);   
        k++;       
     }
    return cursor;
 }

char * copiere(node_t*cursor, char*sirsters){ //copiaza intr-un sir de caractere caracterele de pe o linie 
    int i=0;
    node_t*aux=cursor;
        while ((aux->prev!= NULL)&&(aux->prev->val!='\n')){
            sirsters[i]=aux->val;
            aux=aux->prev;
            i++;
        }
         sirsters[i]=aux->val;
 
    return sirsters;
}
node_t* undo (node_t * head, node_t*cursor, stiva_t* first, char *sirsters){

    if (strcmp(first->next->comm, "::i\n")==0) // pentru scriere undo sterge ultima linie scrisa
        {   sirsters=copiere(cursor,sirsters); // se salveaza in stiva intr-un sir de caractere linia care urmeaza a si stearsa 
            cursor=deleteline(head,cursor,0);
        }
    return cursor;    
}

node_t* redo (node_t * head, node_t*cursor, stiva_t* first, char*sirsters){
    
    int i;
    if (strcmp(first->next->comm, "u\n")==0)  // redo pentru scriere scrie ultima linie stearsa de undo 
        {   for(i=strlen(sirsters)-1; i>=0; i--) //copiaza din sirulsters salvat in stiva inapoi in lista
            cursor=adaug(head,sirsters[i],cursor);
        }
    return cursor;    
}

node_t* find_word( node_t*cursor, char *parametru1 ){  // cauta dupa cursor cuvantul "parametru1" care urmeaza sa fie sters/inlocuit si intoarce pozitia lui
      node_t*aux =cursor;
      node_t*aux2;
      int i=0,k=0,p=0;
      while (aux->next!=NULL && p==0){ // parcurge lista pana la capat sau pana gaseste cuvantul 
        aux2=aux;
        k=0;
        for (i=0;i<strlen(parametru1);i++){
           if (aux2->next!=NULL)
            {if (parametru1[i]!=aux2->val)
                k=1;
            aux2=aux2->next;}
            }
            if (k==0)
                p=1;
            aux=aux->next;    
      }
    return aux->prev;
  }
 node_t* delete_word (node_t *head, node_t*cursor, char *parametru1){  // sterge cuvantul dat ca parametru
     node_t* aux=cursor;
     int i; 
     
     aux=find_word(aux,parametru1); //cauta pozitia cuvantului
     for(i=0;i<strlen(parametru1);i++)
       {
        aux=backspace(aux,head);  //sterge atatea noduri cate litere are cuvantul
        aux=aux->next;}
    
    return aux;
 }
 node_t *insert( node_t *cursor, char x){ //insereaza un nod la o anumita pozitie
    node_t *temp, *aux=cursor;
    temp = (node_t*)malloc(sizeof(node_t));
    temp->val = x;
    temp->next=aux->next;
    aux->next->prev=temp;
    aux->next=temp;
    temp->prev=aux;

    return temp;

 }
 node_t * replace_word(node_t *head, node_t*cursor,char *parametru1, char *parametru2){ // inlocuieste cuvantul dat cu altul
     node_t* aux=cursor;
     int i;
    aux=delete_word(head,cursor,parametru1); //sterge cuvantul dat
        if (aux!=head)
         aux=aux->prev;
   
    for(i=0;i<strlen(parametru2);i++)  //insereaza pe rand in pozitia de unde a fost sters cuvantul initial noduri care contin literele noului cuvant
    aux=insert(aux,parametru2[i]);

    return aux;


 }

 
 node_t *replace_all_word (node_t* head, node_t*cursor,char*parametru1, char*parametru2){
    node_t* aux=cursor;
    node_t* aux2=cursor;
    int k=0, n=0, p=0, i=0;
 
            while (aux2->next!=NULL)
            {   if (find_word(aux2,parametru1))
                    n++;
                aux2=find_word(aux2,parametru1);
                    if (aux2==head )
                        p++;
                aux2=aux2->next;
            }
         if (p){ 
             if(strlen(parametru1)==strlen(parametru2))
                 for(i=0;i<strlen(parametru1);i++)
                { aux->val=parametru2[i];  
                  aux=aux->next;}
             else 
                 aux->val=parametru2[0]; 
                  for (i=0;i<strlen(parametru1)-1;i++)   
                   backspace(aux->next,head) ;
                 for (i=1;i<strlen(parametru2);i++)
                    aux=insert(aux,parametru2[i]);
                }

        for (k=0;k<n-1-p;k++)
          {
            aux=replace_word(head,aux,parametru1,parametru2);
          }
    return cursor;    
}

 node_t* delete_all_word(node_t *head, node_t*cursor, char *parametru1){
    node_t* aux=cursor;
    node_t* aux2=cursor;
    int k=0, n=0;
            while (aux2->next!=NULL)
            {   if (find_word(aux2,parametru1))
                    n++;
                aux2=find_word(aux2,parametru1);
                aux2=aux2->next;
            }
        for (k=0;k<n-1;k++)
          {
            aux=delete_word(head,aux,parametru1);
          }
          aux2=head;
          while (aux2->next!=NULL) // daca se sterge o intreaga linie si ramane doar '\n' acesta se sterge
            {   if (( aux2->val=='\n') && (aux2->next->val=='\n'))
                    aux2=backspace(aux2,head);
                aux2=aux2->next;
            }

    return aux;    
}


node_t* applycommand( char *comanda, node_t *head,node_t *cursor, stiva_t* first,FILE **g,FILE *f, char* sirsters){  // functie care apeleaza comanda citita 
                                                                                                // comenzile se despart in functie de nume si cati paramentrii au
    char *name;
    char *parametru1;
    int param1;
    char *parametru2;
    int param2;
    char *token;
    
     if ( comanda[0]=='d' && comanda[1]=='w')
        {   
            token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
            
         cursor=delete_word (head,cursor,parametru1 );
         
         }

      else if ( comanda[0]=='d' && comanda[1]=='a')
        {  
             token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
         cursor=delete_all_word (head,cursor,parametru1);
         
         }          
    else if ( strcmp(comanda,"q\n")==0);
         
    else if ( strcmp(comanda,"s\n")==0)
         save (head,g);
    else if ( strcmp(comanda,"b\n")==0 ) 
        cursor= backspace(cursor, head);

     else if ( comanda[0]=='g' && comanda[1]=='l')
        {   
            token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
            param1=atoi(parametru1);
         cursor=gotoline(head,param1);
       
        }
   
   else if ( strcmp(comanda,"dl\n")==0 ) 
        { cursor=deleteline(head,cursor,  0); // daca linia care trebuie stearsa nu este specificata , 0 inseamna linia curenta
            }

    else if ( comanda[0]=='d' && comanda[1]=='l')
        {   token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
            param1=atoi(parametru1);
         cursor=deleteline  (head,cursor,param1);
         if(param1==1)
         head=cursor;
         }

    else if (strcmp(comanda,"d\n")==0 )        
        delete(head,cursor,1);
    else if ( comanda[0]=='d')
        {   token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
            param1=atoi(parametru1);
        cursor= delete (head,cursor,param1);}     

   else if ( comanda[0]=='g' && comanda[1]=='c')
        {   token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
            param1=atoi(parametru1);
            token=strtok(NULL," \n");
            if(token!=NULL){
                parametru2=token; 
                param2=atoi(parametru2);
                cursor=goto_character(head,cursor,param1,param2);
              }
            else 
                cursor=goto_character(head,cursor,param1,0);}
              
    else if ( strcmp(comanda,"u\n")==0)
         cursor=undo(head,cursor,first,sirsters);
              
    else if (strcmp(comanda,"r\n")==0)
            cursor=redo(head,cursor,first,sirsters);

    else if ( comanda[0]=='r' && comanda[1]=='e')
        {  
            
            token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
            token=strtok(NULL," \n");
            parametru2=token; 
            cursor=replace_word(head,cursor,parametru1,parametru2);
              }
    else if ( comanda[0]=='r' && comanda[1]=='a')
        {   
            token =strtok(comanda," \n");
            name=token;
            token=strtok(NULL," \n");
            parametru1=token; 
            token=strtok(NULL," \n");
            parametru2=token; 
            cursor=replace_all_word(head,cursor,parametru1,parametru2);
            
              }
             
          
    
    return cursor;
}

void free_stiva(stiva_t* first){ // elibereaza medoria alocata stivei
  stiva_t *aux=first;
    while (first!=NULL)
    {
        aux=first;
        first=first->next;
        free(aux->comm);
        free(aux);
    }

}

void free_lista(node_t* head){ // elibereaza memoria alocata listei
    node_t *aux=head;
    while (aux!=NULL)
    {   node_t *urm=aux->next;
        free (aux);
        aux=urm;
    }

}

int main(int arg, char *argv[]){
     FILE *f=fopen ("editor.in","r");
    FILE *g=fopen ("editor.out","w");
    node_t *head=NULL;  //heaad-primul nod din lista
    node_t *cursor=NULL; // cursor pntru lista 
    stiva_t*first = NULL;  // pointer catre primul element din stiav de comenzi
    char x, comanda[50],schimb=0;
     char sirsters[100]="";

    while((x=fgetc(f))!=EOF){  //se citeste caracter cu  caracter din fisier
        if (x==':'){ // daac se intalneste ::i se trece la citirea comenilor
            schimb++;
         while(x!='\n')
            x=fgetc(f);
        if(schimb%2==0) 
         x=fgetc(f);
        }
       if (schimb%2==0)  // daca se citesc caractere , se adauga in lista care retine textul si cursorul se modifica  pe masura ce se adauga caractere
            if(head==NULL){ head=adaug(head,x,cursor);cursor=head; 
            }
            else 
            {   if (cursor->next==NULL)  // daca cursorul nu se afla la capat de text , se insereaza in interiorul textului 
                cursor=adaug(head,x,cursor);
                else if (x!='\n') cursor=insert(cursor,x);}
        else{first=adaugcomanda(first,"::i\n");  //se trece la modul de citire comenzi,  se adauga in stiva comanda de citire ::i
            while ((fgets(comanda,50,f)!=NULL)&&(strcmp(comanda,"::i\n")!=0 )&&(strcmp(comanda,"::i \n")!=0 )) 
            {                                                   // se citesc comenzile se adauga in stiva si se apeleaza
                first=adaugcomanda(first,comanda);  
                cursor= applycommand( comanda ,head,cursor, first,&g,f,sirsters);
            } 
            schimb++;     // daca se intalneste ::i se trece iar la modul de citire text
        }
    }
   cursor=adaug(head,x,cursor);
  
    free_stiva(first); // se elibereaza stiva
    free_lista(head); // se elibereaza lista
    fclose(g);
    fclose(f);
    return 0;
}