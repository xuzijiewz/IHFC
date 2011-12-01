#include "stdio.h"
#include <conio.h>
#include "interface.h"
#include "functions.h"
#include "hv1.h"
#include "hv2.h"
#include "hv3.h"
#include "hv4.h"
/*The program is wrote to find the low weight collision of inside hash function that has 4 function(hv1,hv2,hv3,hv4).
To reduce the calculated amount. We use branch and bound method.
The compiler is vs2008, there is a command _kbhit() is in conio.h

the format of output file:
had found collisions,  the minimum fixed cells
*/

/*
If it is continue old compute, there will has a file out.txt.
If it is a new compute, it need input how many function will be used and the weight of collision.
*/
param * read_p(){
    FILE *fn;
    fn=fopen("out.txt","r");
    param *p=new_param();
    if(fn==NULL){   //new compute        
        printf("input number of hv function: ");
        scanf("%d",&p->hv_num);
        printf("\ninput max defference weight: ");
        scanf("%d",&p->max_hw);
        return p;
    }
    fscanf(fn,"%d  %d  %d %d ",&p->hv_num,&p->max_hw,&p->pass_num,&p->min_fix_cell);
    for(int i=0;i<16;i++) fscanf(fn,"%d", &p->line_hw[i]);
    p->first_time=0;
    return p;    
}
void main(){      hv1_ini(read_p());   }