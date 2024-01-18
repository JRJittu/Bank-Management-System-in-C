#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct detail
{
    char User_name[50];
    char password[20];
    int Balance;
    char F_name[20];
    char L_name[20];
    char Father_name[20];
    char Mother_name[20];
    int date, month, year;
    char Address[100];
    char Contact[20];
    char e_mail[50];
    char PAN_num[20];
    char Adhar_num[20];
} Detail;

char array[15][50] = {"User_name", "password", "Balance", "F_name", "L_name", "Father_name", "Mother_name", "DOB_date", "DOB_month", "DOB_year",
"Address", "contact", "e_mail", "PAN_num", "Adhar_num"};

void Create_account(void);
void login(void);
void Deposit_money(Detail *, int);
void withdraw_money(Detail *, int);
void transfermoney(Detail *, int);
void upadate_acc(Detail *, int);

void Append_To_File(FILE *, const Detail *);
char *create_passwd();
void Operations(Detail *);
Detail *Read_detail(char *, char *);
int find_row(Detail *);
int change(int, int, char *);
void after_change(void);
char* fetch_passwd(FILE*,char*);
int valid_user(char*);

void gotoxy(int x, int y) // Function for setting cursor position
{
    COORD c;
    c.X = x;
    c.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int main()
{
    int choice;
    system("cls");
    gotoxy(20, 2);

    printf("WELCOME TO BANK ACCOUNT SYSTEM\n\n");
    gotoxy(18, 4);
    printf("**********************************");

    gotoxy(22, 6);
    printf("1.... CREATE A BANK ACCOUNT");

    gotoxy(22, 8);
    printf("2.... ALREADY A USER? SIGN IN");
    gotoxy(22, 10);
    printf("3.... EXIT\n\n");

    printf("\n\nENTER YOUR CHOICE :--> ");

    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        system("cls");
        printf("\n\n USERNAME 50 CHARACTERS MAX!!");
        printf("\n\n PASSWORD 50 CHARACTERS MAX!!");
        Create_account();
        break;

    case 2:
        login();
        break;

    case 3:
        exit(0);
        break;

        getch();
    }
}

void display_passbook(Detail *user)
{
    system("cls");
    gotoxy(20, 1);
    printf("\n\nBANK PASS BOOK \n");
    printf("--------------------------------------------");
    printf("\nUser Name     :  %s", user->User_name);
    printf("\nName          :  %s  %s", user->F_name, user->L_name);
    printf("\nDOB           :  %d %d %d", user->date, user->month, user->year);
    printf("\nFather        :  %s", user->Father_name);
    printf("\nMother        :  %s", user->Mother_name);
    printf("\nAddress       :  %s ", user->Address);
    printf("\nPhone Number  :  %s", user->Contact);
    printf("\nEmail         :  %s", user->e_mail);
    printf("\nPAN Number    :  %s", user->PAN_num);
    printf("\nAadhar Num    :  %s", user->Adhar_num);
}

int valid_user(char* user_id)
{
    FILE* fp = fopen("Bank_Detail.csv","r");
    char line[500];
    while(fgets(line,500,fp))
    {
        char* token=strtok(line,",");
        if(strcmp(token,user_id)==0)
        {
            return 0;
        }
    }
    fclose(fp);
    return 1;
}

void Create_account(void) // Function to create user account
{
    FILE *fp;
    Detail u1;

    system("cls");
    printf("\n\n!!!!!CREATE ACCOUNT!!!!!");

    printf("\n\nFIRST NAME.. ");
    scanf("%s", &u1.F_name);

    printf("\n\nLAST NAME.. ");
    scanf("%s", &u1.L_name);

    printf("\n\nFATHER's NAME.. ");
    scanf("%s", &u1.Father_name);

    printf("\n\nMOTHER's NAME.. ");
    scanf("%s", &u1.Mother_name);

    printf("\n\nADDRESS.. ");
    fflush(stdin);
    fgets(u1.Address, sizeof(u1.Address), stdin);
    u1.Address[strcspn(u1.Address, "\n")] = '\0';

    printf("\n\nDATE OF BIRTH  Date Month Year.. ");
    scanf("%d%d%d", &u1.date, &u1.month, &u1.year);

    printf("\n\nPHONE NUMBER.. ");
    scanf("%s", &u1.Contact);
    while (strlen(u1.Contact) != 10)
    {
        printf("\nInvalid Phone Number");
        printf("\n\nPHONE NUMBER.. ");
        scanf("%s", &u1.Contact);
    }

    printf("\n\nAADHAR NUMBER.. ");
    scanf("%s", &u1.Adhar_num);
    while (strlen(u1.Adhar_num) != 12)
    {
        printf("\nInvalid Aadhar Number");
        printf("\n\nAADHAR NUMBER.. ");
        scanf("%s", &u1.Adhar_num);
    }

    printf("\n\nPAN NUMBER.. ");
    scanf("%s", &u1.PAN_num);
    while (strlen(u1.PAN_num) != 10)
    {
        printf("\nInvalid PAN Number");
        printf("\n\nPAN NUMBER.. ");
        scanf("%s", &u1.PAN_num);
    }

    printf("\n\nE-MAIL.. ");
    scanf("%s", &u1.e_mail);

    u1.Balance = 0;
    printf("\n\nUSERNAME.. ");
    scanf("%s", &u1.User_name);
    while(!valid_user(u1.User_name)){
        printf("\nUser Name is already exist...");
        printf("\nEnter valid User Name : ");
        scanf("%s",&u1.User_name);
    }

    printf("\n\nPASSWORD.. ");
    strcpy(u1.password, create_passwd());

    u1.Balance = 0;
    fp = fopen("Bank_Detail.csv", "ab");
    if (fp == NULL)
    {
        printf("\nFailed to create Bank account..");
        fclose(fp);
        return;
    }

    Append_To_File(fp, &u1);

    fclose(fp);
    system("cls");
    gotoxy(30, 10);
    printf("ACCOUNT CREATED SUCCESSFULLY....");
    gotoxy(0, 20);
    printf("Press enter to login... ");
    getch();
    login();
}

char *create_passwd() // Function to create Password
{
    char *password = (char *)malloc(20); // Allocate memory for the password
    if (password == NULL)
    {
        fprintf(stderr, "\nError: Password cannot generate right now...");
        exit(1);
    }
    int ch;
    int i = 0;

    while (1)
    {
        ch = _getch();
        if (ch == 13)
            break;

        else if (ch == 8)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            password[i] = ch;
            ch = '*';
            printf("%c", ch);
            i++;
        }
    }
    password[i] = '\0';
    return password;
}

void Append_To_File(FILE *file, const Detail *detail) // To write into csv file
{

    if (file != NULL)
    {
        fprintf(file, "%s,%s,%d,%s,%s,%s,%s,%d,%d,%d,%s,%s,%s,%s,%s\n",
                detail->User_name, detail->password, detail->Balance, detail->F_name,
                detail->L_name, detail->Father_name, detail->Mother_name,
                detail->date, detail->month, detail->year, detail->Address,
                detail->Contact, detail->e_mail, detail->PAN_num, detail->Adhar_num);

        fclose(file);
    }
    else
    {
        fprintf(stderr, "Error: Unable to open user_data.csv for appending.\n");
    }
}

void login()
{
    Detail *user = (Detail *)malloc(sizeof(Detail));
    if (user == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return;
    }

    char user_id[50];
    char passwd[20];
    printf("\n\nEnter the user id  --> ");
    scanf("%s", user_id);
    printf("\nEnter the Password --> ");
    scanf("%s", passwd);

    user = Read_detail(user_id, passwd);

    if (!user)
    {
        char ch;
        printf("\nUser didn't Exist...");
        printf("\nDo you want to create account..?  Y / N  : ");
        scanf(" %c", &ch);
        if (ch == 'Y' || ch == 'y')
        {
            Create_account();
        }
        else
        {
            main();
        }
    }
    else
    {
        display_passbook(user);
        Operations(user);
    }
}

Detail *Read_detail(char *user_id, char *passwd)
{
    FILE *fp = fopen("Bank_Detail.csv", "r");
    if (fp == NULL)
    {
        printf("\nFailed to fetch the details...");
        fclose(fp);
        return NULL;
    }

    Detail *user = (Detail *)malloc(sizeof(Detail));
    if (user == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    char content[500];
    int userFound = 0;

    while (fgets(content, sizeof(content), fp))
    {
        strcpy(user->User_name, strtok(content, ","));
        strcpy(user->password, strtok(NULL, ","));
        if (strcmp(user_id, user->User_name) == 0 && strcmp(passwd, user->password) == 0)
        {
            userFound = 1;
            user->Balance = atoi(strtok(NULL, ","));
            strcpy(user->F_name, strtok(NULL, ","));
            strcpy(user->L_name, strtok(NULL, ","));
            strcpy(user->Father_name, strtok(NULL, ","));
            strcpy(user->Mother_name, strtok(NULL, ","));
            user->date = atoi(strtok(NULL, ","));
            user->month = atoi(strtok(NULL, ","));
            user->year = atoi(strtok(NULL, ","));
            strcpy(user->Address, strtok(NULL, ","));
            strcpy(user->Contact, strtok(NULL, ","));
            strcpy(user->e_mail, strtok(NULL, ","));
            strcpy(user->PAN_num, strtok(NULL, ","));
            strcpy(user->Adhar_num, strtok(NULL, ","));
            break;
        }
    }

    fclose(fp);
    if (!userFound)
        return NULL;

    return user;
}

int find_row(Detail *user)
{
    int target = 1;
    char line[500];

    FILE *fp = fopen("Bank_Detail.csv", "r");
    if (fp == NULL)
    {
        printf("\nError 404 not Found!.....");
        return 0;
    }

    while (fgets(line, sizeof(line), fp))
    {
        char *token = strtok(line, ",");
        if (strcmp(token, user->User_name) == 0)
            break;
        target++;
    }
    fclose(fp);
    return target;
}

void Operations(Detail *user)
{
    int target_row = find_row(user);
    int choice = 0;
    while (choice < 7)
    {
        user=Read_detail(user->User_name, user->password);
        printf("\n\n0 : To exit");
        printf("\n1 : Check Balance");
        printf("\n2 : Deposite");
        printf("\n3 : Withdraw");
        printf("\n4 : Transfer Money");
        printf("\n5 : To upadate account");
        printf("\n6 : Display Passbook");
        printf("\n\nEnter your choice --> ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 0:
                exit(0);
                break;
            case 1:
                printf("\nYour account Balance is : %d", user->Balance);
                break;
            case 2:
                Deposit_money(user, target_row);
                break;
            case 3:
                withdraw_money(user, target_row);
                break;
            case 4:
                transfermoney(user, target_row);
                break;
            case 5:
                upadate_acc(user, target_row);
                break;
            case 6:
                display_passbook(user);
                break;
        }
    }
}

void Deposit_money(Detail *user, int target)
{
    int Deposit_amount;
    printf("\nEnter the deposit amount : ");
    scanf("%d", &Deposit_amount);
    user->Balance += Deposit_amount;
    char amount[20];
    sprintf(amount, "%d", user->Balance);
    change(target, 3, amount);
    printf("\nDeposite Successfull.....");
}

void withdraw_money(Detail *user, int target)
{
    int withdraw_amount;
    printf("\nEnter the money you wanted to withdraw : ");
    scanf("%d", &withdraw_amount);
    while (user->Balance < withdraw_amount)
    {
        printf("\nYour Balence is insufficient. Please enter new amount: ");
        scanf("%d", &withdraw_amount);
    }
    user->Balance -= withdraw_amount;
    printf("\nProcessing...");
    char amount[20];
    sprintf(amount, "%d", user->Balance);
    change(target, 3, amount);
    printf("\nYour amount is Withdrawn successfully...");
}

char* fetch_passwd(FILE* fp,char* user_id)
{
    char line[500];
    while(fgets(line,500,fp))
    {
        char* token=strtok(line,",");
        if(strcmp(token,user_id)==0)
        {
            token=strtok(NULL,",");
            return token;
        }
    }
    return NULL;
}

void transfermoney(Detail *user, int target1)
{
    FILE *fp = fopen("Bank_Detail.csv", "r");
    if (fp == NULL)
    {
        printf("\nError Occuured 404 Not Found!");
        fclose(fp);
        return;
    }
    Detail *user1 = (Detail *)malloc(sizeof(Detail));

    char user_id[50];
    char passwd[20];
    printf("\nEnter the user name of Receivers account : ");
    scanf("%s", user_id);
    strcpy(passwd,fetch_passwd(fp,user_id));

    user1=Read_detail(user_id,passwd);
    while (user1 == NULL || strcmp(user->User_name , user1->User_name)==0)
    {
        char ch;
        printf("\nEnter the login details correctly...");
        printf("\nDo You want to enter again (Y/N):");
        scanf(" %c", &ch);
        if (ch == 'Y' || ch == 'y')
        {
            printf("\nEnter the user name of Receivers account : ");
            scanf("%s", user_id);
            printf("\nEnter the Password : ");
            scanf("%s", passwd);
                user1=Read_detail(user_id,passwd);
        }
        else
            Operations(user);
    }
    if (user1 != NULL)
    {
        int amount;
        printf("\nEnter the money you want to transfer: ");
        scanf("%d", &amount);
        while(amount > user->Balance){
            printf("\nInsufficiant Balance.....");
            printf("\nEnter the amount again : ");
            scanf("%d", &amount);
        }
        user->Balance -= amount;
        user1->Balance += amount;
    }
    fclose(fp);
    printf("\nProcessing...");
    int target2 = find_row(user1);
    printf("\nTargeted reciever row : %d",target2);
    char amount1[15];
    char amount2[15];
    sprintf(amount1, "%d", user->Balance);
    sprintf(amount2, "%d", user1->Balance);
    change(target1, 3, amount1);
    change(target2, 3, amount2);
    printf("\nThe Amount is transfered successfully....");
}

void upadate_acc(Detail *user, int target_row)
{
    FILE *fp = fopen("Bank_Detail.csv", "r");
    if (fp == NULL)
    {
        printf("\nError Occuured 404 Not Found!");
        fclose(fp);
        return;
    }
    system("cls");
    char attribute[50];
    for (int i = 0; i < 15; i++)
        printf("\n%s", array[i]);

    int target_column = 1;
    int found = 0;

    printf("\n\nWhat do you want to upadate ..? ");
    scanf("%s", attribute);
    for (int i = 0; i < 15; i++)
    {
        if (strcmp(array[i], attribute) == 0)
        {
            found++;
            break;
        }
        else
            target_column++;
    }

    while (!found)
    {
        printf("\nPlease enter the correct details of update....");
        printf("\n\nWhat do you want to upadate ..? ");
        scanf("%s", attribute);
        for (int i = 0; i < 15; i++)
        {
            if (strcmp(array[i], attribute) == 0)
            {
                found++;
                break;
            }
            else
                target_column++;
        }
    }
    fclose(fp);
    if (found)
    {
        printf("\nEnter the new %s ", array[target_column - 1]);
        char new[50];
        fflush(stdin);
        fgets(new,50, stdin);
        new[strcspn(new, "\n")] = '\0';
        change(target_row, target_column, new);
    }
    printf("\nYour %s upadated successfully...", attribute);
    if(strcmp(attribute,array[1])==0){
        printf("\nYou Changed the password, please login again.. ");
        login();
    }
}

int change(int target_row, int target_column, char *new_item)
{
    FILE *inputFile = fopen("Bank_Detail.csv", "r");
    FILE *temp = fopen("temp.csv", "w");
    if (temp == NULL || inputFile == NULL)
    {
        printf("\nFailed to Upadate... ");
        return 0;
    }

    int curr_row = 1;
    char line[500];
    while (fgets(line, sizeof(line), inputFile))
    {
        if (curr_row == target_row)
        {
            int curr_column = 1;
            char *token = strtok(line, ",");
            while (token != NULL)
            {
                if (curr_column == target_column)
                    fprintf(temp,"%s",new_item);
                else
                    fprintf(temp, "%s", token);
                if(curr_column<15)
                    fprintf(temp,",");

                token = strtok(NULL, ",");
                curr_column++;
            }
            if(target_column==15)
                fprintf(temp,"\n");
        }
        else
            fprintf(temp,"%s",line);

        curr_row++;
    }
    fclose(inputFile);
    fclose(temp);
    after_change();
    return 1;
}

void after_change()
{
    FILE *source = fopen("temp.csv", "r");
    FILE *dest = fopen("Bank_Detail.csv", "w");

    int ch;
    while ((ch = fgetc(source)) != EOF)
        fputc(ch, dest);
    fclose(source);
    fclose(dest);
}