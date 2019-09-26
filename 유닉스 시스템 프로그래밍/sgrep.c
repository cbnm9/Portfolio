#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

#define BUF_SIZE 5120       //Memory size for read file: 5KB

#define MAX_TARGET_NUM 50   //Maximum number of targets

#define FILE_SSS 1          //File, _SSS: For avoid name duplication
#define DIR_SSS 2           //Directory

#define FLAG_NUM 13  //Number of options

#define N_OPT 0 //Except this pattern
#define t_OPT 1 //Change word(pattern) and save copy
#define M_OPT 2 //File name for -t option
#define S_OPT 3 //Save file - Result of this program
#define c_OPT 4 //Print number of matches instead of rows
#define X_OPT 5 //Get Arguments Test
#define v_OPT 6 //Print not matched pattern
#define a_OPT 7 //Search all file in current directory
#define r_OPT 8 //Read directory recursively
#define A_OPT 9 //Print more rows after matched row
#define B_OPT 10//Print rows before matched row
#define C_OPT 11//-A option + -B option
#define h_OPT 12//Print how to use 'sgrep'

//Global variables define
char *pat;      //Pattern
char *comp_pat; //String to compare with pattern
char *comp_npat;//String to compare with exception pattern

char *target[MAX_TARGET_NUM];   //Target(File or Directory)
int target_index;               //Number of targets

char *args[FLAG_NUM];   //Arguments
int flags[FLAG_NUM];    //Options

char *copy_file_name;   //Copy file name(-t, -M)
char *save_file_name;   //Save file name(-S)

int copy_file_d;    //Copy file descriptor
int save_file_d;    //Save file descriptor

int numberOfRows;   //Number of rows(-c)

int multiple_target;    //Flag: If there is multiple target then 1

int argumentRequire;    //If there is option that require argument

int standard_input_flag;    //If there is no target then target is standard input

int A_flg, B_flg;   //A and B option flags: C option is A + B
char **B_ary;       //B array: A don't need array
int B_index;        //B index
int B_max;          //Size of B_ary
int A_max;          //Argument of A option
int A_count;        //-A option count
int B_full;         //If B is fill

int preTargetMatched;     //Last printed target

/* Functions define */
//Program initialize
void init_flags();

void prepareA(char *);
void prepareB(char *);
void prepareC(char *);

void makeCompareStringSpace();
void searchCurrentDir();
void preProcessing();
void setArgument(char *);
void setOptions(char *);
void getAllOptions(int, char *argv[]);

//Utility
void setArgumentRequire(int);
int getArgumentRequire();
void error_handling(char *);
int whatIsIt(char *);
int stringCompare(char *, char *);
void return_resource();
int checkAlreadyIn(char *);
void printHelp();

void initialize_save_file();
void setCopyFile(char *);

void insertQueue(char *);
void printQueue(char *,int);
void clearQueue();

//Program routine function
void printOptions();
void testGetOptions();

void c_optionAction(char *);
void printAction(char *, char *, int);
void grepAction(char *, char *, int);
int setPatternFlag(char *, char *, char *, char, int);
void findPatternInRow(char *, char *, char, char, int);

void search_file(int, char *);
void readFile(char *);
void readStdin();
void readDirectory(char *);
void readDirectoryRecursively(char *);

void sgrep_routine();

/* Function realization */
//=============================================================Program initialize
//Make all flags to 0
void init_flags() { 
    int i; 
    for(i=0; i<FLAG_NUM; i++) flags[i]=0; 

    argumentRequire=0;
    target_index=0;
    standard_input_flag=0;
    copy_file_d=0;
    save_file_d=0;
    numberOfRows=0;
    multiple_target=0;
    A_flg=0;
    B_flg=0;
    B_index=0;
    A_max=0;
    B_max=0;
    B_full=0;
    A_count=0;
    preTargetMatched=0;
}

//Prepare A, B and C option
void prepareA(char *num) {
    A_flg=1;

    //Change string to integer
    A_max=atoi(num);
    if(A_max<=0)
        A_flg=0;
}

void prepareB(char *num) {
    B_flg=1;

    //Change string to integer
    B_max=atoi(num);
    if(B_max<=0) {
        B_flg=0;
        return;
    }

    B_ary=(char **)malloc(sizeof(char*)*B_max);
    if(B_ary==NULL)
        error_handling("malloc() error - prepareB");
}

void prepareC(char *num) {
    prepareA(num);
    prepareB(num);
}

//Set comp_npat
void makeCompareStringSpace() {
    comp_npat=(char *)malloc(sizeof(char)*(strlen(args[N_OPT])+1));
    if(comp_npat==NULL)
        error_handling("malloc error: comp_npat");
}

//Search files in current directory
void searchCurrentDir() {
    DIR *dp;
    struct dirent *dent;
    char *file_name;
    int ignore_flag=0;

    //Open current directory
    dp=opendir(".");
    if(dp==NULL) {
        printf("-a option fail: Current directory open error\n");
        return ;
    }

    //Read directory
    while((dent=readdir(dp))) {
        file_name=(char *)malloc(sizeof(char)*(strlen(dent->d_name)+1));
        if(file_name==NULL)
            error_handling("malloc() error - searchCurrentDir");
        memset(file_name, 0, strlen(dent->d_name)+1);
        memcpy(file_name, dent->d_name, strlen(dent->d_name));

        //Ignore current directory and previous directory
        ignore_flag+=stringCompare(file_name, ".");
        ignore_flag+=stringCompare(file_name, "..");

        //Ignore if file is already in target array
        ignore_flag+=checkAlreadyIn(file_name);

        //Insert file name into target array
        if(!ignore_flag) {
            target[target_index++]=file_name;
        } else {
            free(file_name);
        }

        //Reset ignore_flag
        ignore_flag=0;
    }

    //Close directory
    closedir(dp);
}

//Preprocessing before sgrep_routine
void preProcessing() {

    //If -h option then print help and exit
    if(flags[h_OPT]) {
        printHelp();
        exit(1);
    }

    //Set copy file name
    if(flags[t_OPT]) {
        if(flags[M_OPT]) {
            copy_file_name=args[M_OPT];
        }

        //-t option ignore A, B, C, v, r option
        flags[A_OPT]=0;
        flags[B_OPT]=0;
        flags[C_OPT]=0;
        flags[v_OPT]=0;
        flags[r_OPT]=0;
    }

    //Set save file name
    if(flags[S_OPT]) {
        save_file_name=args[S_OPT];
    }

    //Include files what is in current directory into target array
    if(flags[a_OPT]) {
        searchCurrentDir();
    }

    //Set memory space for compare with exception pattern
    if(flags[N_OPT]) {
        makeCompareStringSpace();
    }

    //If targets is more than 1
    if(target_index>1) {
        multiple_target=1;
    }

    //If -r option then print file name
    if(flags[r_OPT])
        multiple_target=1;

    //-t option must use one target
    if(flags[t_OPT] && target_index > 1)
        error_handling("-t Option: Must use one target");

    //-c option ignore A, B, C option
    if(flags[c_OPT]) {
        flags[A_OPT]=0;
        flags[B_OPT]=0;
        flags[C_OPT]=0;
    }

    //If A option
    if(flags[A_OPT]) {
        prepareA(args[A_OPT]);
    }

    //If B option
    if(flags[B_OPT]) {
        prepareB(args[B_OPT]);
    }

    //If C option
    if(flags[C_OPT]) {
        prepareC(args[C_OPT]);
    }
}

//Set argument
void setArgument(char *arg) {
    args[getArgumentRequire()]=arg;
    argumentRequire=0;
}

//Set option flags
void setOptions(char *option) {
    int i;

    for(i=1; option[i]; i++) {
        switch(option[i]) {
            case 'N':
                flags[N_OPT]=1;
                setArgumentRequire(N_OPT);
                break;
            case 't':
                flags[t_OPT]=1;
                setArgumentRequire(t_OPT);
                break;
            case 'M':
                flags[M_OPT]=1;
                setArgumentRequire(M_OPT);
                break;
            case 'S':
                flags[S_OPT]=1;
                setArgumentRequire(S_OPT);
                break;
            case 'c':
                flags[c_OPT]=1;
                break;
            case 'X':
                flags[X_OPT]=1;
                break;
            case 'v':
                flags[v_OPT]=1;
                break;
            case 'a':
                flags[a_OPT]=1;
                break;
            case 'r':
                flags[r_OPT]=1;
                break;
            case 'A':
                flags[A_OPT]=1;
                setArgumentRequire(A_OPT);
                break;
            case 'B':
                flags[B_OPT]=1;
                setArgumentRequire(B_OPT);
                break;
            case 'C':
                flags[C_OPT]=1;
                setArgumentRequire(C_OPT);
                break;
            case 'h':
                flags[h_OPT]=1;
                break;
            default:
                error_handling("Invalid option");
                break;
        }
    }
}

//Get all options, pattern and targets
void getAllOptions(int argc, char *argv[]) {
    int i;
    int require_pattern=1;
    int target_recv_start=0;
    int target_recv_end=0;

    for(i=1; i<argc; i++) {
        
        //If there is option that require argument
        if(argumentRequire) {
            setArgument(argv[i]);
        }
        //If option
        else if(argv[i][0]=='-') {
            if(target_recv_start) {
                target_recv_start=0;
                target_recv_end=1;
            }
            setOptions(argv[i]);
        }
        //If pattern not received yet
        else if(require_pattern) {
            pat=argv[i];
            comp_pat=(char *)malloc(sizeof(char)*(strlen(pat)+1));
            if(comp_pat==NULL)
                error_handling("malloc error: comp_pat");
            require_pattern=0;
        }
        //If pattern received: next is targets
        else {
            if(target_recv_end)
                error_handling("Invalid usage");
            target_recv_start=1;
            target[target_index++]=argv[i];
        }

    }

    if(require_pattern && !flags[h_OPT])
        error_handling("There is no pattern");

    if(argumentRequire)
        error_handling("No argument in option");

    if((target_index==0) && (flags[a_OPT]==0))
        standard_input_flag=1;
}

//=============================================================Utility

//Set variable 'argumentRequire'
void setArgumentRequire(int req_opt) {
    if(!argumentRequire)
        argumentRequire=req_opt + 1;
    else
        error_handling("There is more than 2 option that require argument");
}

//Get variable 'argumentRequire'
int getArgumentRequire() {
    return argumentRequire - 1;
}

//Exception Handling
void error_handling(char *msg) {
    printf("Error: ");
    printf("%s", msg);
    printf("\n");
    exit(1);
}

//Distinguish file or directory
int whatIsIt(char *name) {
    struct stat buf;
    int kind;

    stat(name, &buf);
    kind=buf.st_mode & S_IFMT;

    switch(kind) {
        case S_IFDIR:
            return DIR_SSS;
        case S_IFREG:
            return FILE_SSS;
        default:
            return -1;
    }
}

//Compare pat and comp_pat
int stringCompare(char *str1, char *str2) {
    if(!strcmp(str1, str2))
        return 1;
    else
        return 0;
}

//Return resource : close all file
void return_resource() {
    if(save_file_d)
        close(save_file_d);

    if(copy_file_d)
        close(copy_file_d);
}

//Check 'name' file is in target array
int checkAlreadyIn(char *name) {
    int i;
    for(i=0; i<target_index; i++) {
        if(stringCompare(name, target[i]))
            return 1;
    }

    return 0;
}

//Print infomation of 'sgrep'
void printHelp() {
    printf("Author: 2013150016 Seo Min Seong, Computer Engineering, KPU\n");
    printf("\n");
    printf("Useage: sgrep [options]... Pattern [Targets]...\n");
    printf("Search for pattern in each file or standard input.\n");
    printf("pattern: just character or string(like as 'grep -F')\n");
    printf("Target: only general file or directory\n");
    printf("\n");
    printf("If target is directory and there is not -r option\n");
    printf("Then search file name in directory\n");
    printf("\n");
    printf("Example: sgrep -S apple dictionary.txt unix_dir\n");
    printf("\n");
    printf("Options(no argument): \n");
    printf("-h: Print how to use 'sgrep'\n");
    printf("-v: Search non-matching lines\n");
    printf("-c: Print only number of matching lines per file\n");
    printf("-r: Read all file in directory and search file\n");
    printf("    If there is directory in directory\n");
    printf("    then read all file in directory in directory - recursively\n");
    printf("-a: Target is files and directorys in current working directory\n");
    printf("-X: Print command line arguments(For test)\n");
    printf("\n");
    printf("Options(need argument):\n");
    printf("-A NUM : Print NUM lines of trailing context\n");
    printf("-B NUM : Print NUM lines of leading context\n");
    printf("-C NUM : Print NUM lines of trailing and leading context\n");
    printf("         = -A NUM -B NUM\n");
    printf("-t STR : Create copy file that all pattern replaced to STR\n");
    printf("-M NAME: Set copy file name is NAME(used with -t option)\n");
    printf("-S NAME: Save result of this command in file named NAME\n");
    printf("-N PAT : Remove specific pattern PAT from results\n");
    printf("\n");
    printf("## -t option ignore other options: A, B, C, N, c, v\n");
    printf("## -t option have to only one target file\n");
    printf("\n");
    printf("## -c option ignore other options: A, B, C\n");
    printf("\n");
}

//Initializing save file (S option)
void initialize_save_file() {
    time_t tt;
    struct tm *tm;
    char *output = "Date: %G-%m-%d %H:%M";
    char buf[256];
    int i;

    time(&tt);
    tm = localtime(&tt);
    strftime(buf, sizeof(buf), output, tm);
    
    //1. Print time
    printf("%s\n", buf);
    //2. Print pattern
    printf("Pattern: %s\n", pat);
    //3. Print targets
    printf("Targets: ");
    if(target_index) {
        for(i=0; i<target_index; i++) {
            printf("%s ",target[i]);
        }
        printf("\n");
    } else {
        printf("No Target\n");
    }
    //4. Print options
    printf("Options\n");
    printOptions();
    //5. Print \n\n
    printf("\n");
}

//Set up copy file name
void setCopyFile(char *name) {
    char temp[3]={'c','p','_'};

    //If not -M option then create copy file name
    if(!flags[M_OPT]) {
        copy_file_name=(char *) malloc(sizeof(char)*(strlen(name)+4));
        if(copy_file_name==NULL)
            error_handling("malloc Error - readFile");
            
        memset(copy_file_name, 0, sizeof(char)*(strlen(name)+4));
        memcpy(copy_file_name, temp, 3);
        memcpy(copy_file_name + 3, name, strlen(name));
            
    }

    //Create or open copy file
    copy_file_d=open(copy_file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(copy_file_d==-1)
        error_handling("Copy file open Error");
}

//Insert data in queue
void insertQueue(char *data) {
    char *temp;

    temp=(char *)malloc(sizeof(char)*(strlen(data)+1));
    if(temp==NULL)
        error_handling("malloc error - insertQueue");

    memset(temp, 0, strlen(data)+1);
    memcpy(temp, data, strlen(data));
    //If array was filled
    if(B_full) {
        free(B_ary[B_index]);
        B_ary[B_index]=temp;

        if(B_index==(B_max-1)) B_index=0;
        else B_index++;

    } else {
        B_ary[B_index++]=temp;
        if(B_index==B_max) {
            B_index=0;
            B_full=1;
        }
    }
}

//Print all data in queue
void printQueue(char *name, int fod) {
    int i;
    //Filled: index is first -> index ~ index-1 repeat
    //Not filled: 0 is first -> 0~index-1 repeat
    if(B_full) {
        printf("--\n");
        for(i=0; i<B_max; i++) {
            printAction(name, B_ary[B_index], fod);
            free(B_ary[B_index]);

            B_index++;
            if(B_index==B_max) B_index=0;
        }
    } else {
        for(i=0; i<B_index; i++) {
            printAction(name, B_ary[i], fod);
            free(B_ary[i]);
        }
    }

    //Reset variables
    B_index=0;
    B_full=0;
}

//Reset queue
void clearQueue() {
    int i;
    //Filled: index is first -> index ~ index-1 repeat
    //Not filled: 0 is first -> 0~index-1 repeat
    if(B_full) {
        for(i=0; i<B_max; i++) {
            free(B_ary[B_index]);
            B_index++;
            if(B_index==B_max) B_index=0;
        }
    } else {
        for(i=0; i<B_index; i++) {
            free(B_ary[i]);
        }
    }

    //Reset variables
    B_index=0;
    B_full=0;
}

//=============================================================Program routine function

//Print options: For -S option
void printOptions() {
    int i;

    for(i=0; i<FLAG_NUM; i++) {
        if(flags[i]) {
            switch(i) {
                case N_OPT:
                    printf("-N, %s\n", args[i]);
                    break;
                case t_OPT:
                    printf("-t, %s\n", args[i]);
                    break;
                case M_OPT:
                    printf("-M, %s\n", args[i]);
                    break;
                case S_OPT:
                    printf("-S, %s\n", args[i]);
                    break;
                case c_OPT:
                    printf("-c\n");
                    break;
                case X_OPT:
                    printf("-X\n");
                    break;
                case v_OPT:
                    printf("-v\n");
                    break;
                case a_OPT:
                    printf("-a\n");
                    break;
                case r_OPT:
                    printf("-r\n");
                    break;
                case A_OPT:
                    printf("-A, %s\n", args[i]);
                    break;
                case B_OPT:
                    printf("-B, %s\n", args[i]);
                    break;
                case C_OPT:
                    printf("-C, %s\n", args[i]);
                    break;
                case h_OPT:
                    printf("-h\n");
                    break;
            }
        }
    }
}

//Test that getAllOptions() works correctly
void testGetOptions() {
    int i;
    int kind;

    for(i=0; i<FLAG_NUM; i++) {
        if(flags[i]) {
            switch(i) {
                case N_OPT:
                    printf("-N Option: %s\n", args[i]);
                    break;
                case t_OPT:
                    printf("-t Option: %s\n", args[i]);
                    break;
                case M_OPT:
                    printf("-M Option: %s\n", args[i]);
                    break;
                case S_OPT:
                    printf("-S Option: %s\n", args[i]);
                    break;
                case c_OPT:
                    printf("-c Option\n");
                    break;
                case X_OPT:
                    printf("-X Option\n");
                    break;
                case v_OPT:
                    printf("-v Option\n");
                    break;
                case a_OPT:
                    printf("-a Option\n");
                    break;
                case r_OPT:
                    printf("-r Option\n");
                    break;
                case A_OPT:
                    printf("-A Option: %s\n", args[i]);
                    break;
                case B_OPT:
                    printf("-B Option: %s\n", args[i]);
                    break;
                case C_OPT:
                    printf("-C Option: %s\n", args[i]);
                    break;
                case h_OPT:
                    printf("-h Option\n");
                    break;
            }
        }
    }

    printf("Pattern: %s\n", pat);
    
    for(i=0; i<target_index; i++) {
        printf("Target name: %s, ", target[i]);

        kind=whatIsIt(target[i]);

        switch(kind) {
            case DIR_SSS:
                printf("Directory\n");
                break;
            case FILE_SSS:
                printf("File\n");
                break;
            default:
                printf("Not File or Directory\n");
                break;
        }
    }
}

//Do c option act: print the number of row that result of this program
void c_optionAction(char *name) {
    //If -c option then print number of rows that matched with pattern
    if(flags[c_OPT]) {
        if(multiple_target)
            printf("%s: ", name);
        printf("%d\n", numberOfRows);
        numberOfRows=0;
    }
}

//Print string
void printAction(char *name, char *buf, int fod) {
    if(multiple_target)
        printf("%s: ", name);
    printf("%s", buf);
    if(fod==DIR_SSS) {
        printf(" - content of directory\n");
    }
}

//Do grep act: print a row or increase count
void grepAction(char *name, char *buf, int fod) {
    //Print current row if not -c option
    if(flags[c_OPT])
        numberOfRows++;
    else {
        printAction(name, buf, fod);
    }

    preTargetMatched=1;
}

//Return pattern flag: pattern matched or not
int setPatternFlag(char *buf, char *comp, char *pat, char first_word, int i) {
    int j;
    int pat_flag=0;
    //If there is matched char
    //with first char of pattern
    if(buf[i]==first_word) {
        //comp_pat initialize
        memset(comp, 0, strlen(pat)+1);
        for(j=0; j<strlen(pat); j++) {
            //If string length of comp_pat is less than pat
            //then comp_pat is not pattern
            if(buf[i+j]=='\n') {
                pat_flag=-1;
                break;
            }

            comp[j]=buf[i+j];
        }

        //If string length of comp_pat equal with pat
        if(pat_flag!=-1) {
            //Check that comp_pat is pattern or not
            pat_flag=stringCompare(comp, pat);
        } else {
            //Reset pat_flag
            pat_flag=0;
        }
    }

    return pat_flag;
}

//Find pattern in row
void findPatternInRow(char *name, char *buf, char first_word, char npat_first_word, int fod) {
    int pat_flag=0;
    int npat_flag=0;
    int i;
    //Search row
    for(i=0; buf[i]; i++) {
            
        //Set pattern flag
        if(!pat_flag) {
            pat_flag=setPatternFlag(buf, comp_pat, pat, first_word, i);
        }

        //-N option check: if not found yet and -N option exist
        if((npat_flag==0) && (flags[N_OPT]==1)) {
            npat_flag=setPatternFlag(buf, comp_npat, args[N_OPT], npat_first_word, i);
        }
        
        //If -t option
        if(flags[t_OPT]) {
            if(pat_flag) {
                //Write args[t_OPT] instead of pat
                write(copy_file_d, args[t_OPT], strlen(args[t_OPT]));
                //Increase i: already read the pattern
                i=i + strlen(pat) - 1;
                //Reset pat_flag
                pat_flag=0;
            } else {
                //Write buf[i]
                write(copy_file_d, &(buf[i]), 1);
            }
        }
    }

    //If find pattern
    /*
    pat_flag=0, flags[v_OPT]=0 -> not
    pat_flag=0, flags[v_OPT]=1 -> do
    pat_flag=1, flags[v_OPT]=0 -> do
    pat_flag=1, flags[v_OPT]=1 -> not

    Therefore pat_flag != flags[v_OPT] -> do
    */
    if(pat_flag != flags[v_OPT]) {
        //If this row has not exception pattern
        if(!npat_flag) {
            //Find pattern in this row: print this row
            if(B_flg)
                printQueue(name, fod);
            grepAction(name, buf, fod);
            
            //If -A option then set count
            if(A_flg)
                A_count=A_max;
        }
    } else {
        if((B_flg==1) && (A_count==0))
            insertQueue(buf);

        if(A_flg) {
            if(A_count>0) {
                grepAction(name, buf, fod);
                A_count--;
                clearQueue();

                if(A_count==0)
                    printf("--\n");
            }
        }
    }
}

//Find row that it has matched string with pattern
void search_file(int fd, char *name) {
    char buf[BUF_SIZE];
    FILE *fp;
    char first_word=pat[0];
    char npat_first_word;

    if(flags[N_OPT])
        npat_first_word=args[N_OPT][0];

    //Get file pointer
    if(fd) {
        fp=fdopen(fd, "r");
        if(fp==NULL)
            error_handling("File pointer error");
    } else {
        fp=stdin;
    }

    //Search file - 1step: 1row or BUF_SIZE
    while(fgets(buf, BUF_SIZE, fp)) {
        findPatternInRow(name, buf, first_word, npat_first_word, FILE_SSS);
    }
    //Do -c option act
    c_optionAction(name);
}

//Read file and search pattern
void readFile(char *name) {
    int target_d;

    //If -t option then open copy file
    if(flags[t_OPT]) {
        setCopyFile(name);
    }

    //Open target file
    target_d=open(name, O_RDONLY);
    if(target_d==-1) {
        printf("File open error: %s\n", name);
        return;
    }

    //Read target file
    search_file(target_d, name);

    close(target_d);
    if(flags[t_OPT]) {
        close(copy_file_d);
    }
}

//Read Standard input
void readStdin() {

    //If -t option then open copy file
    if(flags[t_OPT]) {
        setCopyFile("stdin.txt");
    }

    //Search pattern in file
    search_file(0, "stdin");

    //Return resource - copy file
    if(flags[t_OPT]) {
        close(copy_file_d);
    }
}

//Read directory and search pattern
void readDirectory(char *name) {
    DIR *dp;
    struct dirent *dent;
    char first_word=pat[0];
    char npat_first_word;
    char *file_name;

    //If -t option then ignore directory
    if(flags[t_OPT]) {
        printf("Target is directory: ignore -t option");
        return;
    }

    //If -N option then initialize npat_first_word
    if(flags[N_OPT])
        npat_first_word=args[N_OPT][0];

    //Open directory
    dp=opendir(name);
    if(dp==NULL) {
        printf("Directory open error: %s\n", name);
        return ;
    }

    //Read directory
    while((dent=readdir(dp))) {
        file_name=dent->d_name;
        findPatternInRow(name, file_name, first_word, npat_first_word, DIR_SSS);
    }

    //Do c option act
    c_optionAction(name);

    //Close directory
    closedir(dp);
}

//Read directory recursively ( -r option )
void readDirectoryRecursively(char *name) {
    DIR *dp;
    struct dirent *dent;
    char *file_name;
    char temp='/';
    int whatis;
    int ignore_flag=0;

    //Open directory
    dp=opendir(name);
    if(dp==NULL) {
        printf("Directory open error: %s\n", name);
        return ;
    }

    //Read directory
    while((dent=readdir(dp))) {
        //Ignore current directory and previous directory -> infinity repeat problem
        ignore_flag+=stringCompare(dent->d_name, ".");
        ignore_flag+=stringCompare(dent->d_name, "..");

        if(!ignore_flag) {
            //Make file name
            file_name=(char *)malloc(sizeof(char)*(strlen(name)+2+strlen(dent->d_name)));
            if(file_name==NULL)
                error_handling("malloc() error - searchCurrentDir");
            memset(file_name, 0, strlen(name)+2+strlen(dent->d_name));
            memcpy(file_name, name, strlen(name));
            memcpy(file_name+strlen(name), &temp, 1);
            memcpy(file_name+strlen(name)+1, dent->d_name, strlen(dent->d_name));

            //Check this is file or directory or not
            whatis=whatIsIt(file_name);

            switch(whatis) {
                case FILE_SSS:
                    readFile(file_name);
                    break;
                case DIR_SSS:
                    readDirectoryRecursively(file_name);
                    break;
            }

            //Reset variables
            free(file_name);
        }
        ignore_flag=0;

        //If previous target matched
        //then print "---"(devide character)
        if(preTargetMatched && multiple_target && !flags[c_OPT]) {
            printf("-----\n");
            preTargetMatched=0;
        }
    }

    //Close directory
    closedir(dp);
}

//Search pattern in file or directory
void sgrep_routine() {

    int i, whatis;

    //If include test option
    if(flags[X_OPT]) {
        testGetOptions();
        return;
    }

    //If -S option
    if(flags[S_OPT]) {
        save_file_d=open(save_file_name, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if(save_file_d==-1)
            error_handling("Save file open Error");

        //Change standard output from 1(Monitor) to save file
        dup2(save_file_d, 1);

        initialize_save_file();
    }

    //If there is no target
    if(standard_input_flag) {
        readStdin();
        return;
    }

    //Search pattern in file or directory
    for(i=0; i<target_index; i++) {
        whatis=whatIsIt(target[i]);

        switch(whatis) {
            case FILE_SSS:
                readFile(target[i]);
                break;
            case DIR_SSS:
                if(flags[r_OPT]) readDirectoryRecursively(target[i]);
                else readDirectory(target[i]);
                break;
            default:
                printf("This is not file or directory: %s\n", target[i]);
                break;
        }

        //If -B option then clear queue
        if(B_flg)
            clearQueue();

        //If -A option then reset count value
        if(A_flg)
            A_count=0;

        //If previous target matched
        //then print "---"(devide character)
        if(preTargetMatched && multiple_target 
            && !flags[c_OPT] && !flags[r_OPT]) {
            printf("-----\n");
            preTargetMatched=0;
        }
    }
}

//Program main routine
int main(int argc, char *argv[]) {
    
    //1. Initializing variables
    init_flags();

    //2. Get all options, pattern and targets
    getAllOptions(argc, argv);

    //3. Set up variables depend on options
    preProcessing();

    //4. sgrep execution
    sgrep_routine();

    //5. Return resource
    return_resource();

    return 0;
}
