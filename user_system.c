#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


struct account 
{
    char name[100];
    char passw[20];
    int account_num;
};

void file_handling(int* account_number, struct account* p)
{
    int acc_num = *account_number;
    char file_name[256];
    sprintf(file_name, "%d", acc_num); //Converts int to str

    char filepath[100] = "users/"; //filepath
    strcat(filepath, file_name);

    const char ender[] = ".txt"; //const
    strcat(filepath, ender); //concatenate


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

    printf("1. Create Account.\n");
    printf("2. Log Into Account.\n");
    printf("3. Exit program.\n");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            create_acc();
            return;
        case 2:
            //log_in();
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