#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct customer
{
    int accNo;
    char name[30];
    float balance;
};

struct transaction
{
    int accNo;
    char transType;
    float amount;
};


void insertCustomer(struct customer *[], int *);
void writeCustomerToFile(struct customer *[], int *, FILE *fs);
void insrtTransaction(struct customer *[], struct transaction *[], int *, int *);
void writeTransactionsToFile(struct transaction *[], int *, FILE *);
void loadCustomersFromFile(struct customer *[], int *, FILE *);
void displayAllCustomers(struct customer *[], int *);
void disPlayAllTransactions(struct transaction *[], int *);
int doTransaction(struct customer *[], struct transaction *, int );
void loadTransactionsFromFile(struct transaction *[], int *, FILE *);

int main()
{
    FILE *fCus, *fTran, *fCusWrite;
    fCus = fopen("customer.txt", "r+");
    fTran = fopen("transaction.txt", "r+");
    if (fCus == NULL || fTran == NULL)
    {
        puts("Cant't open file\n");
        fclose(fCus);
        fclose(fTran);
        exit(1);
    }
    struct customer *ptrCus[100];
    struct transaction *ptrTrans[100];
    int noOfCustomers = 0, noOfTransaction = 0;
    loadCustomersFromFile(ptrCus, &noOfCustomers, fCus);
    loadTransactionsFromFile(ptrTrans, &noOfTransaction, fTran);

    int choice;
    while (1)
    {
        printf("\n\n1.Add Customer\n2.Transaction\n3.Display All Customers\n4.Display All Transaction\n5.exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            insertCustomer(ptrCus, &noOfCustomers);
            break;

        case 2:
            insrtTransaction(ptrCus, ptrTrans, &noOfTransaction, &noOfCustomers);
            break;
        case 3:
            displayAllCustomers(ptrCus, &noOfCustomers);
            break;
        case 4:
            disPlayAllTransactions(ptrTrans, &noOfTransaction);
            break;
        case 5:
            writeCustomerToFile(ptrCus, &noOfCustomers, fCus);
            writeTransactionsToFile(ptrTrans, &noOfTransaction, fTran);
            fclose(fCus);
            fclose(fTran);
            exit(1);

        default:
            puts("Enter valid Choice:\n");
            break;
        }
    }

    return 0;
}
void insertCustomer(struct customer *customres[], int *noOfCustomer)
{
    struct customer *tempCustomer = (struct customer *)malloc(sizeof(struct customer));

    if (tempCustomer == NULL)
    {
        exit(1);
    }
    printf("\nEnter customer details:");
    puts("\nEnter account number:");
    scanf("%d", &tempCustomer->accNo);
    puts("\nEnter customer name:");
    scanf(" %s", tempCustomer->name);
    puts("\nEnter balance:");
    while (1)
    {
        puts("Minimum Balance should be 100\n");
        scanf("%f", &tempCustomer->balance);
        if (tempCustomer->balance >= 100)
            break;
    }

    for (int i = 0; i < *noOfCustomer; i++)
    {
        if (customres[i]->accNo == tempCustomer->accNo)
        {
            puts("Customer with given account number already exit\n");
            return;
        }
    }
    customres[*noOfCustomer] = tempCustomer;
    (*noOfCustomer)++;
}

void insrtTransaction(struct customer *customers[], struct transaction *trnsactions[], int *noOfTransactions, int *noOfCustomers)
{
    struct transaction *tempTransaction = (struct transaction *)malloc(sizeof(struct transaction));
    if (tempTransaction == NULL)
    {
        puts("memory overflow!");
        exit(1);
    }
    printf("Enter Transaction Details:\n");
    puts("\nEnter account number:\n");
    scanf("%d", &tempTransaction->accNo);
    puts("\nEnter Transaction type:\tD: Deposit\tW: Withdraw\n");
    while (1)
    {
        puts("Enter only D or W");
        scanf(" %c", &tempTransaction->transType);
        if (tempTransaction->transType == 'D' || tempTransaction->transType == 'W')
            break;
    }
    puts("\nEnter Amount:");
    scanf("%f", &tempTransaction->amount);

    int sucess = doTransaction(customers, tempTransaction, *noOfCustomers);
    if(sucess){
        trnsactions[*noOfTransactions] = tempTransaction;
        (*noOfTransactions)++;

    }
}

void writeCustomerToFile(struct customer *customers[], int *noOfCustomers, FILE *fs)
{
    fseek(fs, 0, SEEK_SET);
    for (int i = 0; i < *noOfCustomers; i++)
    {
        fprintf(fs, "%-6d\t%-12s\t%10.3f\n", customers[i]->accNo, customers[i]->name, customers[i]->balance);
    }
}
void displayAllCustomers(struct customer *customers[], int *noOfCustomers)
{
    printf("AC.no\tName\t\t  Balence\n");
    for (int i = 0; i < *noOfCustomers; i++)
    {
        printf("%-6d\t%-12s\t%10.3f\n", customers[i]->accNo, customers[i]->name, customers[i]->balance);
    }
}

void disPlayAllTransactions(struct transaction *transactions[], int *noOfTransactions)
{
    printf("\nAC.no\tType\t  Ammount\n");
    for (int i = 0; i < *noOfTransactions; i++)
    {
        printf("%-6d\t%-5c\t%10.3f\n", transactions[i]->accNo, transactions[i]->transType, transactions[i]->amount);
    }
}

void writeTransactionsToFile(struct transaction *transactions[], int *noOfTransactions, FILE *fs)
{
    fseek(fs, 0, SEEK_SET);
    for (int i = 0; i < *noOfTransactions; i++)
    {
        fprintf(fs, "%-6d\t%-5c\t%10.3f\n", transactions[i]->accNo, transactions[i]->transType, transactions[i]->amount);
    }
}

void loadCustomersFromFile(struct customer *customers[], int *noOfCustomers, FILE *fs)
{
    struct customer custTemp;
    while (fscanf(fs, "%d\t%s\t%f", &custTemp.accNo, custTemp.name, &custTemp.balance) != EOF)
    {
        struct customer *temp = (struct customer *)malloc(sizeof(struct customer));

        if (temp == NULL)
        {
            exit(1);
        }

        temp->accNo = custTemp.accNo;
        strcpy(temp->name, custTemp.name);
        temp->balance = custTemp.balance;

        customers[*noOfCustomers] = temp;
        (*noOfCustomers)++;
    }
}
void loadTransactionsFromFile(struct transaction *transactions[], int *noOfTransactions, FILE *fs)
{

    struct transaction tranTemp;
    while (fscanf(fs, "%d %c %f", &tranTemp.accNo, &tranTemp.transType, &tranTemp.amount) != EOF)
    {
        struct transaction *temp = (struct transaction *)malloc(sizeof(struct transaction));

        if (temp == NULL)
        {
            exit(1);
        }

        temp->accNo = tranTemp.accNo;
        temp->transType = tranTemp.transType;
        temp->amount = tranTemp.amount;

        transactions[*noOfTransactions] = temp;
        (*noOfTransactions)++;
    }
}

int doTransaction(struct customer *customers[], struct transaction *trans, int noOfCustomers)
{
    for (int i = 0; i < noOfCustomers; i++)
    {
        if (customers[i]->accNo == trans->accNo)
        {
            if (trans->transType == 'D')
            {
                customers[i]->balance += trans->amount;
                return 1;
            }
            if (trans->transType == 'W')
            {
                if (customers[i]->balance - trans->amount >= 100)
                {
                    customers[i]->balance -= trans->amount;
                    return 1;
                }
                else
                {
                    puts("Account Balance Should not go below 100\n");
                    return 0;
                }
            }
        }
    }
    puts("\nCustomer Account correspond to given account number does't exit\n");
    return 0;
}
