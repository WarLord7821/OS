#include <stdio.h>

struct Process{
    int pid;
    int AT;
    int BT;
    int WT;
    int TAT;
    int ST;
    int CT;
};
int total_wt = 0;
int total_tat = 0;

void main(){
    //Take input of number of processes from user
    printf("Enter Number of Process\n");
    int n;
    scanf("%d", &n);

    struct Process p[n];
    //Take input for AT and BT for Each Process
    for(int i = 0; i<n; i++){
        p[i].pid = i;
        printf("Enter AT for Process with Pid %d\n", i);
        scanf("%d", &p[i].AT);
        printf("Enter BT for Process with Pid %d\n", i);
        scanf("%d", &p[i].BT);
    }

    //Sort the Processes as per AT using Bubble Sort
    for(int j = 0; j<n;j++){
        for(int k = j+1; k<n;k++){
            if (p[j].AT > p[k].AT){
                struct Process temp;
                temp = p[j];
                p[j] = p[k];
                p[k] = temp;
            }
        }
    }

    //Calculate WT and TAT
//for p[0]th process
p[0].ST = p[0].AT;
p[0].CT = p[0].ST + p[0].BT;
p[0].WT = 0;
p[0].TAT = p[0].WT + p[0].BT;

total_wt += p[0].WT;
total_tat += p[0].TAT;

//for prcesses other than P
for (int i =1; i<n ; i++){
    if(p[i].AT > p[i-1].CT){
        p[i].ST = p[i].AT;
    }
    else{
        p[i].ST = p[i-1].CT;
    }
    p[i].CT = p[i].ST + p[i].BT;
    p[i].WT = p[i].ST - p[i].AT;
    p[i].TAT = p[i].WT + p[i].BT;

    total_wt += p[i].WT;
    total_tat += p[i].TAT;
}

int avg_wt = total_wt/n;
int avg_tat = total_tat/n;

for(int i =0; i<n ;i++){
    printf("------------For Process Pid = %d----------\n", i);
    printf("W.T. = %d \n",p[i].WT);
    printf("T.A.T = %d \n",p[i].TAT);
}
printf("Avg W.T. = %d and Avg T.A.T = %d",avg_wt,avg_tat);
}