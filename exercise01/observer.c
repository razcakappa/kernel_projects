#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

void getprocline(FILE *thisProcFile, const char *needle, const char *needle2);

char *gettimestring (int n);

int main(int argc, char *argv[]){

    char repTypeName[16], c1, c2;
    int interval, duration;
    struct timeval now;

    strcpy(repTypeName, "Standard");

    if(argc > 1){
        sscanf(argv[1], "%c%c", &c1, &c2);

        if(c1 != '-'){
            fprintf(stderr, "usage: observer [-s][-l int dur]\n");
            exit(1);
        }
        if(c2 == 's'){
            strcpy(repTypeName, "Short");
        }
        if(c2 == 'l'){
            strcpy(repTypeName, "Long");
            interval = atoi (argv[2]);
            duration = atoi (argv[3]);
        }
    }

    // Printing Program Introduction
    gettimeofday(&now, NULL);
    printf("Status report type %s at %s\n", 
            repTypeName, ctime(&(now.tv_sec)));

    //thisProcFile = fopen("/proc/sys/kernel/hostname", "r");

    // cpu type and model
    FILE *proc_cpu_info = fopen("/proc/cpuinfo", "r");
    if(proc_cpu_info == NULL){
        fprintf(stderr, "file can't be opened\n");
        exit(1);
    }
    const char *cpu_name = "model name";
    const char *cpu_id = "processor";
    printf("Proc Info:\n"); 
    getprocline(proc_cpu_info, cpu_name, cpu_id);
    fclose(proc_cpu_info);


    // Kernel version
    FILE *proc_version = fopen("/proc/version", "r");
    if(proc_cpu_info == NULL){
        fprintf(stderr, "file can't be opened\n");
        exit(1);
    }
    const char version[10] = "Linux";
    printf("\nKernel Version: "); 
    getprocline(proc_version, version, "NIL");
    fclose(proc_version);

    // Uptime
    FILE *proc_uptime = fopen("/proc/uptime", "r");
    char *value;
    int count = 0;
    char c;
    do{
        char *ptr = fgetc(proc_uptime);
        value[count] = ptr;
        count++;
        strcpy(c,ptr);
    } while(strcmp(c," ") != 0);

    char *uptime;// = (char*)malloc(255*sizeof(char)); 
    uptime = gettimestring(111458);
    printf("Uptime: %s\n", uptime);
}


void getprocline(FILE *thisProcFile, const char *needle, const char *needle2){
    size_t sz = 0;
    char *lin = 0;
    do{

        ssize_t lsz = getline (&lin, &sz, thisProcFile);
        if(lsz<0) break;

        if(strstr(lin, needle)){
            printf("%s",lin); 
        }
        else if(needle2 != "NIL" && strstr(lin, needle2)){
            printf("%s",lin); 
            //printf(")))) %s ((( ",needle2); 
        }

    } while (!feof (thisProcFile));
}

char *gettimestring (int n){
    int day = n / (24*3600);
    char days[4];
    sprintf(days, "%d", day);


    n = n % (24*3600);
    int hour = n/3600;
    char hours[2];
    sprintf(hours, "%d", hour);


    n %= 3600;
    int minutes = n/3600;
    char mnts[3];
    sprintf(mnts, "%d", minutes);


    n %= 60;
    int seconds = n;
    char scnds[2];
    sprintf(scnds, "%d", seconds);


    char *str = (char*)malloc(255*sizeof(char)); 

    strcpy(str, days);
    strcat(str, ":");
    strcat(str, hours);
    strcat(str, ":");
    strcat(str, mnts);
    strcat(str, ":");
    strcat(str, scnds);

    //printf("DIE: %s\n",str);exit(1);
    return str;
}
