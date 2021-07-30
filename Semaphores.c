//Implements Synchronization for a shared bank account using Semaphores
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//function declarations
void *handleHusbandT(void *id);
void *handleWifeT(void *id);
void deposit(float amount);
void withdraw(float amount);

//global variables
volatile float balance = 0.0;
char husbandT[256] = "";
char wifeT[256] = "";
volatile int locked = 0;

int main(int argc, char *argv[])
{
    if(argc<4)
    {
        printf("\nLess number of inputs passed. %d inputs passed instead of 3", argc - 1);
        return 0;
    }
    sscanf(argv[1], "%f",&balance);
    strcpy(husbandT,argv[2]);
    strcpy(wifeT,argv[3]);
    
    //creates two threads
    pthread_t threads[2];
    
    printf("Opening balance: GHS %.2f", balance);
    
    //thread to handle husband's transactions
    if(pthread_create(&threads[0], NULL, handleHusbandT, (void *)0))
    {
         printf("\nError: unable to create thread to handle husband transaction");
         exit(-1);
    }
    
    //thread to handle wife's transactions
    if(pthread_create(&threads[1], NULL, handleWifeT, (void *)1))
    {
         printf("\nError: unable to create thread to handle wife transaction");
         exit(-1);
    }
    
    //wait for the two threads to finish executing before printing closing balance
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);
    printf("\nClosing balance: GHS %.2f", balance);
    return 0;
}

/*Handles Husband transaction*/
void *handleHusbandT(void *id)
{
    /*Opening husband transaction */
    FILE *husb_file;
    husb_file = fopen(husbandT,"r");
    if (husb_file == NULL)
    {
        printf("\nCould not open file \"%s\" for hasband transaction, Please specify an available file.",husbandT);
        exit(0);
    }
    char transaction[256];
    
    /*Obtains transactions from input text file*/
    char transactionType[256] = "";
    float transactionAmount=0.0;
    while (fgets(transaction, sizeof(transaction), husb_file)) 
    {
        /*take off \n character and splits transaction by space */
        char *curTransaction = strtok (transaction, "\n");
        curTransaction = strtok (transaction, " ");
        strcpy(transactionType,curTransaction);
        curTransaction = strtok (NULL, " ");
        transactionAmount = atof(curTransaction);
        curTransaction = strtok (NULL, " ");
        
        while(locked){} //while balance is locked, keep waiting
        locked = 1; //lock balance before transaction so other threads cannot use/modify it
        if(strcmp(transactionType,"deposit")==0)
        {
            deposit(transactionAmount);
        }
        else if(strcmp(transactionType,"withdraw")==0)
        {
            withdraw(transactionAmount);
        }
        else
        {
            printf("\nInvalid transaction type \"%s\"", transactionType);
        }
        locked = 0; //unlock balance after transaction so other thread can use it
    }
    fclose(husb_file);
}

/*Handles WIfe's transactions*/
void *handleWifeT(void *id)
{
    /*Opening wife's transaction */
    FILE *wife_file;
    wife_file = fopen(wifeT,"r");
    if (wife_file == NULL)
    {
        printf("\nCould not open file \"%s\" for wife's transactions, Please specify an available file.",wifeT);
        exit(0);
    }
    char transaction[256];
    
    /*Obtains transactions from input text file*/
    char transactionType[256] = "";
    float transactionAmount=0.0;
    while (fgets(transaction, sizeof(transaction), wife_file)) 
    {
        /*takes off \n character and splits transaction by space */
        char *curTransaction = strtok (transaction, "\n");
        curTransaction = strtok (transaction, " ");
        strcpy(transactionType,curTransaction);
        curTransaction = strtok (NULL, " ");
        transactionAmount = atof(curTransaction);
        curTransaction = strtok (NULL, " ");
        
        while(locked){} //while balance is locked, keep waiting
        locked = 1; //lock balance before transaction so other threads cannot use/modify it
        if(strcmp(transactionType,"deposit")==0)
        {
            deposit(transactionAmount);
        }
        else if(strcmp(transactionType,"withdraw")==0)
        {
            withdraw(transactionAmount);
        }
        else
        {
            printf("\nInvalid transaction type \"%s\". Valid transactions are \"deposit\" or \"withdraw\" in lower case", transactionType);
        }
        locked = 0; //unlock balance after transaction so other thread can use it
    }
    fclose(wife_file);
}

/*Deposits into account*/
void deposit(float amount)
{
    if(amount>0)
    {
        balance += amount;
        printf("\nDeposit:  GHS %.2f\t\tAccount balance: GHS %.2f",amount,balance);
    }
    else
    {
        printf("\nDeposit:  GHS %.2f\t\tTransaction declined",amount);
    }
}

/*Withdraws from account*/
void withdraw(float amount)
{
    if(amount>0 && amount <= balance)
    {
        balance -= amount;
        printf("\nWithdraw: GHS %.2f\t\tAccount balance: GHS %.2f",amount,balance);
    }
    else
    {
        printf("\nWithdraw: GHS %.2f\t\tTransaction declined",amount);
    }
}

/*I compiled and tested my code using onlinegdb.com. To do this, copy and paste the code there, select
C as the language and upload the input text files. Pass your input parameters via terminal in the section
named command line arguments and click on the run button at the top*/