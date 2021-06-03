#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_YR 9999
#define MIN_YR 1900
#define MAX_SIZE_USER_NAME 30
#define MAX_SIZE_PASSWORD 20
#define FILE_NAME "books.bin" //Macro related to the books info
#define MAX_BOOK_NAME 50
#define MAX_AUTHOR_NAME 50
#define MAX_STUDENT_NAME 50
#define MAX_STUDENT_ADDRESS 300
#define FILE_HEADER_SIZE sizeof(sFileHeader) //structure to store data

typedef struct
{
    int yyyy;
    int mm;
    int dd;
}Date;

typedef struct
{
    char userName[MAX_SIZE_USER_NAME];
    char password[MAX_SIZE_PASSWORD];
}sFileHeader;

typedef struct
{
    unsigned int bookd_id;
    char bookName[MAX_BOOK_NAME];
    char authorName[MAX_AUTHOR_NAME];
    char studentName[MAX_STUDENT_NAME];
    char studentAddress[MAX_STUDENT_ADDRESS];
    Date bookIssueDate;
}s_BooksInfo;

void printMessageCenter(const char *message)
{
    int len = 0;
    int pos = 0;

    //calculate how many space need to print
    len = (78 - strlen(message))/2;
    printf("\t\t\t");

    for(pos = 0; pos < len; pos++)
    {
        //print space
        printf(" ");
    }
    //print message
    printf("%s", message);
}

void headMessage(const char *message)
{
    system("cls");
    printf("\t\t\t############################################################\n");
    printf("\t\t\t############                                    ############\n");
    printf("\t\t\t############      LIBRARY MANAGEMENT SYSTEM     ############\n");
    printf("\t\t\t############                                    ############\n");
    printf("\t\t\t############################################################\n");
    printf("\t\t\t------------------------------------------------------------\n");
    printMessageCenter(message);
    printf("\t\t\t------------------------------------------------------------\n");
}

void welcomeMessage()
{
    headMessage("Powered by ++DevX solutions++\n");
    printf("\n\n\n\n\n");
    printf("\n\t\t\t **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n\t\t\t       =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n\t\t\t       =-=-=-=       WELCOME       =-=-=-=");
    printf("\n\t\t\t       =-=-=-=          TO         =-=-=-=");
    printf("\n\t\t\t       =-=-=-=        L.M.S.       =-=-=-=");
    printf("\n\t\t\t       =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=");
    printf("\n\t\t\t **-**-**-**-**-**-**-**-**-**-**-**-**-**-**-**\n");
    printf("\n\n\n\t\t\tEnter any key to continue...");
    getchar();
}

int isNameValid(const char *name)
{
    int validName = 1;
    int len = 0;
    int index = 0;

    len = strlen(name);

    for(index = 0; index < len; ++index)
    {
        if(!(isalpha(name[index])) && (name[index] != '\n') && (name[index] != ' '))
        {
            validName = 0;
            break;
        }
    }
    return validName;
}

//Function to check leap year
//Function returns 1 if leap year

int isLeapYear(int year)
{
    return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

//returns 1 if given date is valid

int isValidDate(Date *validDate)
{
    //check range of year, month and day
    if(validDate->mm < 1 || validDate->mm > 12)
        return 0;

    if(validDate->dd < 1 || validDate->dd > 31)
        return 0;

    //handle february days in leap year
    if(validDate->mm == 2)
    {
        if(isLeapYear(validDate->yyyy))
            return (validDate->dd <= 29);
        else
            return (validDate->dd <= 28);
    }

    //handle months which has only 30 days
    if(validDate->mm == 4 || validDate->mm == 6 || validDate->mm == 9 || validDate->mm == 11)
        return (validDate->dd <= 30);
    return 1;
}

//All books in list
void addBookInDatabase()
{
    int days;
    s_BooksInfo addBookInfoInDatabase = {0};
    FILE *fp = NULL;
    int status = 0;
    fp = fopen(FILE_NAME, "ab+");

    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    headMessage("ADD NEW BOOKS\n");
    printf("\n\n\t\tENTER YOUR DETAILS BELOW: ");
    printf("\n\t\t\t-------------------------------------------------------------------------\n");
    printf("\n\t\t\tBook ID NO = ");
    fflush(stdin);
    scanf("%u", &addBookInfoInDatabase.bookd_id);

    do
    {
        printf("\n\t\tBook Name = ");
        fflush(stdin);
        fgets(addBookInfoInDatabase.bookName, MAX_BOOK_NAME, stdin);
        status = isNameValid(addBookInfoInDatabase.bookName);

        if(!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.\n");
        }
    }while(!status);

    do
    {
        printf("\n\t\t\tStudent Name = ");
        fflush(stdin);
        fgets(addBookInfoInDatabase.studentName, MAX_STUDENT_NAME, stdin);
        status = isNameValid(addBookInfoInDatabase.studentName);

        if(!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.\n");
        }
    }while(!status);

    do
    {
        printf("\n\t\t\tAuthor Name = ");
        fflush(stdin);
        fgets(addBookInfoInDatabase.authorName, MAX_AUTHOR_NAME, stdin);
        status = isNameValid(addBookInfoInDatabase.authorName);

        if(!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.\n");
        }
    }while(!status);

    do
    {
        //get date year, month and day from user
        printf("\n\t\t\tEnter date in format(day/month/year): ");
        scanf("%d%d%d", &addBookInfoInDatabase.bookIssueDate.dd, &addBookInfoInDatabase.bookIssueDate.mm, &addBookInfoInDatabase.bookIssueDate.yyyy);

        //check date validity
        status = isValidDate(&addBookInfoInDatabase.bookIssueDate);

        if(!status)
        {
            printf("\n\t\t\tPlease enter a valid date.\n");
        }
    }while(!status);

    fwrite(&addBookInfoInDatabase, sizeof(addBookInfoInDatabase), 1, fp);
    fclose(fp);

}

//search books
void searchBooks()
{
    int found = 0;
    char bookName[MAX_BOOK_NAME] = {0};
    s_BooksInfo addBookInfoInDatabase = {0};
    FILE *fp = NULL;
    int status = 0;

    fp = fopen(FILE_NAME,"rb");

    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }

    headMessage("SEARCH BOOKS\n");
    //put the control on book detail

    if(fseek(fp, FILE_HEADER_SIZE, SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issue while reading file\n");
        exit(1);
    }

    printf("\n\n\t\t\tEnter Book Name to search:");
    fflush(stdin);
    fgets(bookName, MAX_BOOK_NAME, stdin);

    while(fread(&addBookInfoInDatabase, sizeof(addBookInfoInDatabase), 1, fp))
    {
        if(!strcmp(addBookInfoInDatabase.bookName, bookName))
        {
            found = 1;
            break;
        }
    }

    if(found)
    {
        printf("\n\t\t\tBook id = %u\n", addBookInfoInDatabase.bookd_id);
        printf("\t\t\tBook name = %s\n", addBookInfoInDatabase.bookName);
        printf("\t\t\tBook authorName = %s\n", addBookInfoInDatabase.authorName);
        printf("\t\t\tBook issue date(day/month/year) = (%d/%d/%d)",addBookInfoInDatabase.bookIssueDate.dd,addBookInfoInDatabase.bookIssueDate.mm,addBookInfoInDatabase.bookIssueDate.yyyy);

    }
    else
    {
        printf("\n\t\t\tNo Record");
    }

    fclose(fp);
    printf("\n\n\n\t\t\tPress any key to go to main menu....");
    getchar();
}

//view books function
void viewBooks()
{
    int found = 0;
    char bookName[MAX_BOOK_NAME] = {0};
    s_BooksInfo addBookInfoInDatabase = {0};
    FILE *fp = NULL;
    int status = 0;
    unsigned int countBook = 1;

    headMessage("VIEW BOOKS DETAILS\n");

    fp = fopen(FILE_NAME,"rb");

    if(fp == NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }

    if(fseek(fp, FILE_HEADER_SIZE, SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issue while reading file\n");
        exit(1);
    }

    while(fread(&addBookInfoInDatabase, sizeof(addBookInfoInDatabase), 1, fp))
    {
        printf("\n\t\t\tBook count = %d\n\n", countBook);
        printf("\n\t\t\tBook id = %u\n", addBookInfoInDatabase.bookd_id);
        printf("\t\t\tBook name = %s\n", addBookInfoInDatabase.bookName);
        printf("\t\t\tBook authorName = %s\n", addBookInfoInDatabase.authorName);
        printf("\t\t\tBook issue date(day/month/year) = (%d/%d/%d)",addBookInfoInDatabase.bookIssueDate.dd,addBookInfoInDatabase.bookIssueDate.mm,addBookInfoInDatabase.bookIssueDate.yyyy);
        found = 1;
        ++countBook;

    }

    fclose(fp);
    if(!found)
    {
        printf("\n\t\t\tNo Record");
    }
    printf("\n\n\n\t\t\tPress any key to go to main menu....");
    fflush(stdin);
    getchar();
}

//delete function
void deleteBooks()
{
    int found = 0;
    int bookDelete = 0;
    sFileHeader fileHeaderInfo = {0};
    char bookName[MAX_BOOK_NAME] = {0};
    s_BooksInfo addBookInfoInDatabase = {0};
    FILE *fp = NULL;
    FILE *tmpfp = NULL;
    int status = 0;

    headMessage("DELETE BOOK DETAILS\n");
    fp = fopen(FILE_NAME, "rb");

    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    tmpfp = fopen("tmp.bin", "wb");

    if(tmpfp = NULL)
    {
        fclose(fp);
        printf("File is not opened\n");
        exit(1);
    }

    fread(&fileHeaderInfo, FILE_HEADER_SIZE, 1, fp);
    fwrite(&fileHeaderInfo, FILE_HEADER_SIZE, 1, tmpfp);

    printf("\n\t\t\tEnter Book ID No. for delete: ");
    scanf("%d", &bookDelete);

    while(fread(&addBookInfoInDatabase, sizeof(addBookInfoInDatabase), 1, fp))
    {
        if(addBookInfoInDatabase.bookd_id != bookDelete)
        {
            fwrite(&addBookInfoInDatabase, sizeof(addBookInfoInDatabase), 1, tmpfp);
        }
        else
        {
            found = 1;
        }
    }

    (found)? printf("\n\t\t\tRecord deleted successfully..."):printf("\n\t\t\tRecord not found");
    fclose(fp);
    fclose(tmpfp);
    remove(FILE_NAME);
    rename("tmp.bin", FILE_NAME);
}

void updateCredential(void)
{
    sFileHeader fileHeaderInfo = {0};
    FILE *fp = NULL;

    unsigned char userName[MAX_SIZE_USER_NAME] = {0};
    unsigned char password[MAX_SIZE_PASSWORD] = {0};
    headMessage("UPDATE CREDENTIAL\n");
    fp = fopen(FILE_NAME, "rb+");

    if(fp = NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    fread(&fileHeaderInfo, FILE_HEADER_SIZE, 1, fp);

    if(fseek(fp, 0, SEEK_SET) != 0)
    {
        fclose(fp);
        printf("\n\t\t\tFacing issue while updating password\n");
        exit(1);
    }

    printf("\n\t\t\tNew Username : ");
    fflush(stdin);
    fgets(userName, MAX_SIZE_USER_NAME, stdin);

    printf("\n\t\t\tNeew Password : ");
    fflush(stdin);
    fgets(password, MAX_SIZE_PASSWORD, stdin);

    strncpy(fileHeaderInfo.userName, userName, sizeof(userName));
    strncpy(fileHeaderInfo.password, password, sizeof(password));

    fwrite(&fileHeaderInfo, FILE_HEADER_SIZE, 1, fp);
    fclose(fp);

    printf("\n\t\t\tYour Password has been changed successfully");
    fflush(stdin);
    getchar();
    exit(1);
}

void menu()
{
    int choice = 0;
    do
    {
        headMessage("MAIN MENU\n");
        printf("\n\t\t\t1. Add Books");
        printf("\n\t\t\t2. Search Books");
        printf("\n\t\t\t3. View Books");
        printf("\n\t\t\t4. Delete Books");
        printf("\n\t\t\t5. Update Books");
        printf("\n\t\t\t0. Exit");

        printf("\n\n\n\t\t\tEnter choice => ");
        scanf("%d", &choice);

        switch(choice)
        {

        case 1:
            addBookInDatabase();
            break;

        case 2:
            searchBooks();
            break;

        case 3:
            viewBooks();
            break;

        case 4:
            deleteBooks();
            break;

        case 5:
            updateCredential();
            break;

        case 0:
            printf("\n\n\n\t\t\tThank you!!!\n\n\n\n\n");
            exit(1);
            break;

        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
        }
    }while(choice);
}

//login password
void login()
{
    unsigned char userName[MAX_SIZE_USER_NAME] = {0};
    unsigned char password[MAX_SIZE_PASSWORD] = {0};
    int L = 0;
    sFileHeader fileHeaderInfo = {0};

    FILE *fp = NULL;
    headMessage("Login\n");

    fp = fopen(FILE_NAME, "rb");

    if(fp == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }

    fread(&fileHeaderInfo, FILE_HEADER_SIZE, 1, fp);
    fclose(fp);

    do
    {

        printf("\n\n\n\t\t\t\tUsername: ");
        fgets(userName, MAX_SIZE_USER_NAME, stdin);

        printf("\n\t\t\t\tPassword: ");
        fgets(password, MAX_SIZE_PASSWORD, stdin);

        if((!strcmp(userName, fileHeaderInfo.userName)) && (!strcmp(password, fileHeaderInfo.password)))
        {
            menu();
        }
        else
        {
            printf("\t\t\t\tLogin failed enter again username & password\n\n");
            L++;
        }
    }while(L <= 3);

    if(L > 3)
    {
        headMessage("Login failed\n");
        printf("\t\t\t\tSorry, Unknown User.");
        getchar();
        system("cls");
    }
}

int isFileExists(const char *path)
{
    //Try to open file
    FILE *fp = fopen(path, "rb");
    int status = 0;

    //If file does not exists
    if(fp != NULL)
    {
        status = 1;
        fclose(fp);
    }
    return status;
}

void init()
{
    FILE *fp = NULL;
    int status = 0;

    const char defaultUsername[] = "deelaka\n";
    const char defaultPassword[] = "deelaka\n";

    sFileHeader fileHeaderInfo = {0};
    status = isFileExists(FILE_NAME);

    if(!status)
    {
        //create the binary file
        fp = fopen(FILE_NAME, "wb");

        if(fp != NULL)
        {
            //copy default password
            strncpy(fileHeaderInfo.password, defaultPassword, sizeof(defaultPassword));
            strncpy(fileHeaderInfo.userName, defaultUsername, sizeof(defaultUsername));

            fwrite(&fileHeaderInfo, FILE_HEADER_SIZE, 1, fp);
            fclose(fp);
        }
    }
}

int main()
{
    init();
    welcomeMessage();
    login();

    return 0;
}
