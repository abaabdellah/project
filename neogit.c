#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
char addressinit[50];
char configname[20];
char configemail[30];

int run_init(FILE *p, int argc, char *const argv[])

{

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        return 1;
    char tmp_cwd[1024];
    bool exists = false;
    struct dirent *entry;
    do
    {
        // find .neogit
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            perror("Error opening current directory\n");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".neogit") == 0)
                exists = true;
        }
        closedir(dir);

        // update current working directory
        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
            return 1;

        // change cwd to parent
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
                return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    // return to the initial cwd
    if (chdir(cwd) != 0)
        return 1;

    if (!exists)
    {
        {
            if (mkdir(".neogit", 0755) != 0)
                return 1;
            printf("repository initialized\n");
            return 1;
        }
    }
    else
    {
        printf("neogit repository has already initialized\n");
    }

    return 0;
}

int checkParentFolders(const char *filePath, const char *parentFolderName)
{
    char pathCopy[30];
    char *token;

    // Create a copy of the file path
    strncpy(pathCopy, filePath, 30);

    // Get the first folder in the path (ignoring the file name)
    token = strtok(pathCopy, "\\");
    if (token == NULL)
    {
        printf("Invalid file path.\n");
        return 0;
    }

    // Traverse the remaining folders in the path
    while (token != NULL)
    {
        char folderName[15];
        strncpy(folderName, token, 15);

        // Check if the folder name matches the specified name
        if (strcmp(folderName, parentFolderName) == 0)
        {
            return 1;
        }

        token = strtok(NULL, "\\");
    }

    printf("there isn't any repository!\n");
    return 0;
}

int configglobal(FILE *p, char arg3[20], char arg4[20])
{
    char str[50];
    if (strcmp(arg3, "user.name") == 0)
    {

        p = fopen("config.txt", "w"); /////should handle
        fprintf(p, "%s ", arg4);
        strcpy(configname, arg4);
        fclose(p);

        return 1;
    }
    p = fopen("config.txt", "a");
    fprintf(p, "%s", arg4);
    strcpy(configemail, arg4);
    fclose(p);
     FILE *a = fopen("config.txt", "r");
     char st[50];
     fscanf(a , "%s", st);
    char *token = strtok(st, " ");
    strcpy(configname, token);
     printf("your username is %s\nyour email is %s\n", configname, configemail);
     fclose(a);

    return 1;
}

int configlocal(FILE *p, char arg3[20], char arg4[20]) //////
{
    if (strcmp(arg3, "user.name") == 0)
    {

        p = fopen("/mnt/c/projectf/.neogit/config.txt", "w"); /////should handle
        fprintf(p, "%s ", arg4);
        strcpy(configname, arg4);
        fclose(p);

        return 1;
    }
    p = fopen("config.txt", "a");
    fprintf(p, "%s ", arg4);
    strcpy(configname, arg4);
    fclose(p);
    FILE *a = fopen("/mnt/c/projectf/.neogit/config.txt", "r");
    char str[50];
    fgets(str, 50, a);
    fclose(a);
    char *token = strtok(str, " ");
    strcpy(configname, token);
    printf("your username is %s\nyour email is %s\n", configname, configemail);

    return 1;
}

int aliasglobal(FILE *p, char arg3[20], char arg4[20])
{

    p = fopen("aliasglobal.txt", "a"); /////should handle
    fprintf(p, "%s", arg3 + 6);
    fprintf(p, "%s#", arg4);
    fclose(p);
    return 1;
}

int aliaslocal(FILE *p, char arg3[20], char arg4[20])
{

    p = fopen("/mnt/c/projectf/.neogit/aliaslocal.txt", "w"); /////should handle
    fprintf(p, "%s%s#", arg3 + 6, arg4);
    fclose(p);

    return 1;
}

int add(char arg2[30])
{
    if (strstr(arg2, ".") != NULL)
    {
        char cwd[1000];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            struct stat st = {0};
            if (stat("/mnt/c/projectf/.neogit/add", &st) == -1)
            {
                // Create the folder
                int status = mkdir("/mnt/c/projectf/.neogit/add", 0777);
            }
            // else
            //     printf("add made before!\n");
        }
        FILE *sourceFile, *destinationFile;
        char sourceAddress[100];
        strcpy(sourceAddress, getcwd(NULL, 0));
        strcat(sourceAddress, "/");
        strcat(sourceAddress, arg2);
        // printf("%s", sourceAddress);
        char buffer[4096];

        sourceFile = fopen(sourceAddress, "r");
        if (sourceFile == NULL)
        {
            printf("Failed to open source file.\n");
            return 1;
        }
        // Open the destination file
        char destinationAddress[] = "/mnt/c/projectf/.neogit/add/";
        strcat(destinationAddress, arg2);
        destinationFile = fopen(destinationAddress, "w");
        ////writing name of files in a file
        FILE *p = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "a");
        fprintf(p, "%s,", arg2);
        fclose(p);

        while (fgets(buffer, 100, sourceFile) != NULL)
        {
            fputs(buffer, destinationFile);
        }

        fclose(sourceFile);
        fclose(destinationFile);
        printf("staged successfully!");
        return 1;
    }

    /// copying folder...
    struct dirent *entry;
    char addr[50] = "/";
    strcpy(addr, arg2);

    DIR *dir = opendir(addr);
    while ((entry = readdir(dir)) != 0)
    { // printf("%s" ,entry->d_name);
        if (entry->d_type == DT_REG)
        {

            // printf("%s", entry->d_name);
            FILE *sourceFile, *destinationFile;
            char sourceAddress[100];
            strcpy(sourceAddress, getcwd(NULL, 0));
            strcat(sourceAddress, "/");
            strcat(sourceAddress, arg2);
            strcat(sourceAddress, "/");
            strcat(sourceAddress, entry->d_name);
            // printf("%s", sourceAddress);
            char buffer[4096];

            sourceFile = fopen(sourceAddress, "r");
            if (sourceFile == NULL)
            {
                printf("Failed to open source file.\n");
                return 1;
            }
            // Open the destination file
            char destinationAddress[] = "/mnt/c/projectf/.neogit/add/";
            strcat(destinationAddress, entry->d_name);
            destinationFile = fopen(destinationAddress, "w");
            ////writing name of files in a file
            FILE *p = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "a");
            fprintf(p, "%s,", entry->d_name);
            fclose(p);

            while (fgets(buffer, 100, sourceFile) != NULL)
            {
                fputs(buffer, destinationFile);
            }

            fclose(sourceFile);
            fclose(destinationFile);
        }
    }
    printf("staged successfully\n");

    return 1;
}

int add_n(int ar)
{
    struct dirent *entry;
    DIR *dir = opendir(".");
    // printf("%s",getcwd(NULL ,0));

    if (dir == NULL)
    {
        perror("Error opening current directory");
        return 1;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (entry->d_type == DT_REG)

        {
            FILE *p = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "r");
            char string[1000];
            fgets(string, 1000, p);
            printf("%s  ", entry->d_name);
            if (strstr(string, entry->d_name) != NULL)
            {
                printf("+\n");
            }
            else
                printf("-\n");
            fclose(p);
        }

        else
        {

            char str[50];
            strcpy(str, getcwd(NULL, 0));
            strcat(str, "/");
            strcat(str, entry->d_name);
            // printf("((%s))", str);
            DIR *dir2 = opendir(str);
            struct dirent *entry2;
            if (dir2 == NULL)
            {
                perror("Error opening current directory");
                return 1;
            }
            int flag = 0;
            while ((entry2 = readdir(dir2)) != NULL)
            {
                if (strcmp(entry2->d_name, ".") == 0 || strcmp(entry2->d_name, "..") == 0)
                    continue;

                FILE *p2 = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "r");
                char string[1000];
                fgets(string, 1000, p2);
                if (strstr(string, entry2->d_name) == NULL)
                {
                    printf("%s   -\n", entry->d_name);
                    flag = 1;
                    break;
                }
            }
            if (!flag)
            {
                printf("%s   +\n", entry->d_name);
            }
        }
    }
    closedir(dir);
    return 1;
}

int reset(char arg2[20])
{
    struct stat st = {0};
    if (stat("/mnt/c/projectf/.neogit/reset", &st) == -1)
    {
        // Create the folder
        int status = mkdir("/mnt/c/projectf/.neogit/reset", 0777);
    }
    // else
    //     printf("reset made before!\n");

    if (strstr(arg2, ".") != 0)
    {
        FILE *sourceFile, *destinationFile;
        char sourceAddress[100];
        strcpy(sourceAddress, "/mnt/c/projectf/.neogit/add/");
        strcat(sourceAddress, arg2);
        // printf("%s", sourceAddress);
        char buffer[4096];

        sourceFile = fopen(sourceAddress, "r");
        if (sourceFile == NULL)
        {
            printf("Failed to open source file.\n");
            return 1;
        }
     // Open the destination file
        // char destinationAddress[] = "/mnt/c/projectf/.neogit/reset/";
        // strcat(destinationAddress, arg2);
        // destinationFile = fopen(destinationAddress, "w");
        ////writing names!
        FILE *p = fopen("/mnt/c/projectf/.neogit/reset/namefilesreset.txt", "a");
        fprintf(p, "%s,", arg2);
        fclose(p);
        // ////delete name of file
        FILE *a = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "r");
        char str[410];
        char tokha[10][50];
        fgets(str, 400, a);
        char *token = strtok(str, ",");
        int m = 0;
        while (token != NULL)
        {
            if (strcmp(token, arg2) != 0)
            {continue;
            }
            
            strcpy(tokha[m], token);
            m++;
            token = strtok(NULL,",");
        }
        fclose(a);
        FILE *p2 = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "w");
        for (int i = 0; i < m; i++)
        {
            fprintf(p2, "%s,", tokha[i]);
        }
        fclose(p2);
        fclose(sourceFile);
        fclose(destinationFile);
    }


    /// folder


    else
    {

        struct dirent *entry;
        char addr[50] = "/";
        DIR *dir = opendir(addr);
        strcpy(addr, arg2);
        while ((entry = readdir(dir)) != 0)
        {
            FILE *sourceFile, *destinationFile;
            char sourceAddress[100];
            strcpy(sourceAddress, "/mnt/c/projectf/.neogit/add/");
            strcat(sourceAddress, entry->d_name);
            // printf("%s", sourceAddress);
            char buffer[4096];

            sourceFile = fopen(sourceAddress, "r");
            if (sourceFile == NULL)
            {
                printf("Failed to open source file.\n");
                return 1;
            }
            // Open the destination file
            // char destinationAddress[] = "/mnt/c/projectf/.neogit/reset/";
            // strcat(destinationAddress, arg2);
            // destinationFile = fopen(destinationAddress, "w");
            ////writing names!
            FILE *p = fopen("/mnt/c/projectf/.neogit/reset/namefilesreset.txt", "a");
            fprintf(p, "%s,", arg2);
            fclose(p);
            ////delete name of file
            // FILE *a = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "r");
            // char str[410];
            // char tokha[10][50];
            // fgets(str, 400, a);
            // char *token = strtok(str, ",");
            // int m = 0;
            // while (token != NULL && strcmp(token, arg2) != 0)
            // {
            //     strcpy(tokha[m], token);
            //     m++;
            // }
            //fclose(a);
            // FILE *p2 = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "w");
            // for (int i = 0; i < m; i++)
            // {
            //     fprintf(p2, "%s,", tokha[i]);
            // }
            // fclose(p2);

            // while (fgets(buffer, 100, sourceFile) != NULL)
            // {
            //     fputs(buffer, destinationFile);
            // }
            // fclose(sourceFile);
            // fclose(destinationFile);
        }
    }
}

int resetundo()
{
    FILE *a;
    a = fopen("/mnt/c/projectf/.neogit/reset/namefilesreset.txt", "r");
    char str[100];
    fgets(str, 100, a);
    char *token = strtok(str, ",");
    while (token != NULL)
    {
        strcpy(str, token);
    }
    if (strstr(str, " ") == NULL)
    {

        fclose(a);
        FILE *b = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "a");
        fprintf(b, "%s,", str);
        fclose(b);
        printf("staged!");
        return 1;
    }
    char tokha[10][30];
    char *token2 = strtok(str, " ");
    int g = 0;
    while (token2 != NULL)
    {
        strcpy(tokha[g], token2);
        g++;
    }
    FILE *c = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "a");
    for (int i = 0; i < g; i++)
    {
        fprintf(c, "%s,", tokha[i]);
    }
    fclose(c);
    printf("staged!");
    return 1;
}

int main(int argc, char *argv[])
{
    FILE *p;
    if (argc < 2)
    {
        printf("Invalid command!\n");
        return 1;
    }

    ////check alias
    // FILE *a;
    // char lines[100];
    // a = fopen("/mnt/c/projectf/.neogit/aliaslocal.txt", "r");
    // while (fgets(lines, 100, a) != NULL)
    // {
    //     for (int i = 0; i < argc; i++)
    //     {
    //         /* code */
    //     }

    //     // if (strstr())
    //     // {
    //     //     /* code */
    //     // }
    // }

    ////alias global
    if (strcmp(argv[1], "config") == 0 && strcmp(argv[2], "-global") == 0 && strncmp(argv[3], "alias", 5) == 0)
        return aliasglobal(p, argv[3], argv[4]);

    ////alias local
    if (strcmp(argv[1], "config") == 0 && strncmp(argv[2], "alias", 5) == 0)
        return aliaslocal(p, argv[2], argv[3]);

    ////init
    if (strcmp(argv[1], "init") == 0)
        return run_init(p, argc, argv);

    ////config global
    if (strcmp(argv[1], "config") == 0 && strcmp(argv[2], "-global") == 0)
        return configglobal(p, argv[3], argv[4]);

    ////config local
    if (strcmp(argv[1], "config") == 0)
        return configlocal(p, argv[2], argv[3]);

    ////add
    if (strcmp(argv[1], "add") == 0)
    {
        for (int i = 0; i < argc - 2; i++)
        {
            return add(argv[2]);
        }
        return 1;
    }

    ////add-f
    if (strcmp(argv[1], "add-f") == 0) ////without function!!
    {
        int a = argc - 2;
        for (int i = 0; i < a; i++)
        {
            struct stat st = {0};

            if (stat("/mnt/c/projectf/.neogit/add", &st) == -1)
            {
                int status = mkdir("add", 0777);
            }

            FILE *sourceFile, *destinationFile;
            char sourceAddress[50];
            strcpy(sourceAddress, getcwd(NULL, 0));
            strcat(sourceAddress, "/");
            strcat(sourceAddress, argv[i + 2]);
            char buffer[4096];
            // Open the source file
            sourceFile = fopen(sourceAddress, "r");
            if (sourceFile == NULL)
            {
                printf("Failed to open source file.\n");
                return 1;
            }

            char destinationAddress[] = "/mnt/c/projectf/.neogit/add/";
            strcat(destinationAddress, argv[i + 2]);
            destinationFile = fopen(destinationAddress, "w");
            FILE *o = fopen("/mnt/c/projectf/.neogit/add/namefiles.txt", "a");
            fprintf(o, "%s,", argv[i + 2]);
            fclose(o);
            // Copy data from source file to destination file
            while (fgets(buffer, 100, sourceFile) != NULL)
            {
                fputs(buffer, destinationFile);
            }

            fclose(sourceFile);
            fclose(destinationFile);
            printf("staged successfully!");
        }
    }

    ////add-n
    if (strcmp(argv[1], "add-n") == 0)
        return add_n(argc);

    ////reset
    if (strcmp(argv[1], "reset") == 0 && argc == 3)
        return reset(argv[2]);

    ////resetundo
    if (strcmp(argv[1], "reset") == 0)
        return resetundo(argv[3]);
}