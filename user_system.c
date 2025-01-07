#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>


struct account 
{
    char name[100];
    char passw[20];
    int account_num;
};

int file_exists(char* filename)
{
    char dir_name[] = "users/";

    DIR* dp;
    struct dirent* entry;

    dp=opendir(dir_name);

    if(dp==NULL)
    {
        printf("\nDirectory doesn't exist or cannot be opened. Returning to homescreen\n\n");
        return 1;
    }

    entry = readdir(dp);
    while (entry != NULL)
    {
        if (strcmp((entry->d_name), ".")!= 0 || strcmp((entry->d_name), "..")!=0)
        {
            if(strcmp((entry->d_name), filename))
            {
                //printf("\nFile exists.\n\n");
                return 0;
            }
            else
            {
                printf("\nFile does not exist.\n\n");
                return -1;
            }
        }
    }
    closedir(dp);
    printf("\nFile does not exist.\n\n");
    return -1;
}

void file_handling(int* account_number, struct account* p)
{
    int acc_num = *account_number;
    char file_name[256];
    sprintf(file_name, "%d", acc_num); //Converts int to str

    char filepath[100] = "users/"; //filepath
    strcat(filepath, file_name);

    const char ender[] = ".txt"; //const
    strcat(filepath, ender); //concatenate


    int exists = file_exists(file_name);

    switch (exists)
    {
        case 1:
            //Error opening directory
            return;
        case 0:
            printf("\nAn account with this account number exists. Please log in.\n\n");
            return;
        case -1:
            printf("\nAccount is being created now.\n\n");
            break;
    }

    FILE* fp = fopen(filepath, "w");

    if(fp==NULL)
    {
        printf("\nFile failed to open. Nothing was saved.\n");
        printf("\nReturning to homescreen.\n\n");
        return;
    }

    fprintf(fp, "Name: %s\nPassword: %s\nAccount Number: %d\n", p->name, p->passw, p->account_num);   
    fclose(fp); 
}

void log_in()
{   
    int account_number;

    printf("\nPlease enter your account number here: \n");
    scanf("%d", &account_number);

    char file_name[256];
    sprintf(file_name, "%d", account_number);
    
    char filepath[100] = "users/"; //filepath
    strcat(filepath, file_name); //concatenate

    const char ender[] = ".txt"; //const
    strcat(filepath, ender); //concatenate

    int exists = file_exists(filepath);

    switch (exists)
    {
        case 1:
            //Error opening directory
            return;
        case 0:
            //printf("\nAccount found.\n\n");
            break;
        case -1:
            printf("\nAccount does not exist. Returning to starting screen where you may create an account or exit the program.\n\n");
            return;
    }

    FILE* fp = fopen(filepath, "r");

    if (fp==NULL)
    {
        printf("\nError opening file. Returning to homescreen.\n\n");
        return;
    }

    char file_data[50];

    int count = 0;
    char password_data[50];

    while (fgets(file_data, 50, fp)!=NULL) //50 is the approx. length of each line
    {
        if (count==1) //Getting the data from the password line
        {
            strcpy(password_data, file_data);
            break;
        }
        else
        {
            count++;
        }
    }

    if (strcmp(password_data, "")==0) //Making sure the line in the file wasn't empty
    {
        printf("\nPassword data cannot be found. Returning to home screen.\n\n");
        return;
    }

    char password [20];
    int passw = 10; //Length of the prefix password in the file
    int pass_len = 20; //Maximum length of the user's pasword

    strncpy(password, password_data + passw, pass_len); //Getting the user's password
    int user_pass_length = strlen(password);
    password[user_pass_length-1]='\0';

    char user_password[20];

    int tries=0;

    while (tries < 3)
    {
        printf("\nTries left: %d\nPlease enter your password: \n", (3-tries));
        scanf("%s", user_password);
        int result = strcmp(password, user_password);

        if (result!=0)
        {
            tries++;
        }
        else if (result==0)
        {
            printf("\nLog in success!\n\n");
            rewind(fp);
            while ((fgets(file_data, 256, fp))!=NULL)
            {
                printf("%s", file_data);
            }
            return;
        }
    }

    fclose(fp);

    printf("\nNo more tries. Wait 10 seconds before you can try again.\n\n");

    int seconds = 10;

    while (seconds > 0)
    {
        printf("%d...\n", seconds);

        clock_t timer = clock() + CLOCKS_PER_SEC;
        while (clock() < timer) { }

        seconds--;
    }
    
    printf("\nReturning to starting screen now.\n\n");

    return;
}

bool password_checker(char* password)
{
    int num_count=0;
    int cap_count=0;
    int sym_count=0;

    for (int i=0; i < strlen(password); i++)
    {
        if(isdigit(password[i]))
        {
            num_count++;
        }
        if(isupper(password[i]))
        {
            cap_count++;
        }
        if(password[i]=='$' || password[i]=='!' || password[i]=='@' || password[i]=='#')
        {
            sym_count++;
        }
    }

    if (num_count < 3)
    {
        printf("At least 3 numbers are needed in the password. Returning to starting screen.\n\n");
        return false;
    }
    if (cap_count < 1)
    {
        printf("At least 1 captial letter is needed. Returning to starting screen.\n\n");
        return false;
    }
    if (sym_count < 1)
    {
        printf("At least 1 symbol is needed ($, !, @, #). Returning to starting screen.\n\n");
        return false;
    }

    return true;
}

void create_acc()
{

    char temp_name[100];
    char temp_pass[20];
    char check_temp_pass[20];
    int temp_accNum;  

    printf("Enter your name:\n");
    scanf("%s", temp_name);

    if (strlen(temp_name) <= 100)
    {
        for(int i=0; i < strlen(temp_name); i++)
        {
            if (!isalpha(temp_name[i]))
            {
                printf("Not valid name. Please enter characters only. Returning to starting screen.\n\n");
                return;
            }
        }
    }else
    {
        printf("Name cannot be more than 100 characters. Returning to home screen\n\n");
        return;
    }

    printf("Enter your password:\n");
    scanf("%s", temp_pass);

    if (strlen(temp_pass) < 8 || strlen(temp_pass) > 20)
    {
        printf("Password should not be less than 8 characters or more than 20 characters. Returning to starting screen.\n\n");
        return;
    }

    bool pass_ok = password_checker(temp_pass);
    
    if (pass_ok == false)
    {
        return;
    }

    printf("Re-enter your password:\n");
    scanf("%s", check_temp_pass);

    if(strcmp(temp_pass, check_temp_pass) != 0)
    {
        printf("Passwords do not match. Returning to starting screen.\n\n");
        return;
    }

    printf("Enter bank account number:\n");
    scanf("%d", &temp_accNum);

    if (temp_accNum < 8)
    {
        printf("Invalid account number. Returning to starting screen.\n\n");
        return;
    }
    if (isalpha(temp_accNum))
    {
        printf("Invalid account number. Returning to starting screen.\n\n");
        return;
    }

    struct account p;
    strncpy(p.name, temp_name, sizeof(p.name)-1);
    strncpy(p.passw, temp_pass, sizeof(p.passw)-1);
    p.account_num = temp_accNum;

    printf("\nName: %s\nPassword: %s\nAccount Number: %d\n\n", p.name, p.passw, p.account_num);
    printf("Please remember your password.\n\n");

    file_handling(&p.account_num, &p);

    return;
}

void starting_screen(bool* exit_program)
{
    int choice;

    printf("---------------------------\n\n     Starting Screen\n\n");
    printf("1. Create Account.\n");
    printf("2. Log Into Account.\n");
    printf("3. Exit program.\n");
    printf("---------------------------\n");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            create_acc();
            return;
        case 2:
            log_in();
            return;
        case 3:
            *exit_program = true;
            return;
        default:
            printf("Invalid choice. Must be 1 or 2.\nReturning to starting screen.\n\n");
            return;
    }

}

int main()
{
    bool exit_p = false;

    while (exit_p == false)
    {
        starting_screen(&exit_p);
    }

    printf("Exiting program...\n");
    printf("Program exited.\n");
}