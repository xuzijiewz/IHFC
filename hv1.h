void write_p(param *p){
    FILE *fn;
    fn=fopen("out.txt","w");
    fprintf(fn,"%d  %d  %d %d \n", p->hv_num,p->max_hw,p->pass_num,p->min_fix_cell);
    for(int i=0;i<16;i++) fprintf(fn,"%d ", p->line_hw[i]);
    fclose(fn);
}
//start find collection, 
//set the weight of defference.
//which line has defference
void hv1_ini(param *p){ 
    int line_hw_com[16],t;
    for(int i=0;i<16;i++)line_hw_com[i]=0;
    line_hw_com[15]=p->max_hw;
    do{        
        if(p->first_time==0){
            for(int i=0;i<16;i++)line_hw_com[i]=p->line_hw[i];
            for(int i=15;i>0;i--)for(int j=0;j<i;j++){
                if(line_hw_com[j]>line_hw_com[j+1]){
                    t=line_hw_com[j];
                    line_hw_com[j]=line_hw_com[j+1];
                    line_hw_com[j+1]=t;
                }
            }
            p->first_time=1;
        }
        else for(int i=0;i<16;i++)p->line_hw[i]=line_hw_com[i];
        do{
            for(int i=0;i<16;i++){
                p->deff[i]=0x0;
                p->line_curhw[i]=0;
            }
            p->cur_hw=0;            
            write_p(p);
            for(int i=0;i<16;i++) printf("%d ", p->line_hw[i]);
            printf("\n");
            if(_kbhit()) return;
            //begin set defference
            next_hv1(p); 
        }while(next_pl(p->line_hw,16));//next line combination, pick (p->max_hw/2-ip) lines from 16 lines
    }while(next_number(line_hw_com,16)); //next weight combination
    printf("\nthere are %d collisions.\nfix at lease %d cells.\n",p->pass_num,p->min_fix_cell);
    FILE *fn;
    fn=fopen("out.txt","w");
    fprintf(fn,"\nthere are %d collisions.\nfix at lease %d cells.\n",p->pass_num,p->min_fix_cell);
    fclose(fn);
    getchar();
    getchar();
}

/*set defferences in a line and send deff to hv2*/
bool next_hv1(param *p){
    //find the first line that weight less than p->line_hw, if no weighter 0, find the first line    
    int def_line=0;       //the index to the line that lighter than p->line_hw[ip]
    while(def_line<16 && (p->line_curhw[def_line]==p->line_hw[def_line] || p->line_curhw[def_line]==0)) def_line++;
    if(def_line==16){
        def_line=0;
        while(p->line_curhw[def_line]==p->line_hw[def_line])def_line++;
    }

    int line_hw=p->line_curhw[def_line];   //temp
    //find the word can been used, the p->lines[ip]-th bit is 0
    def_node defs[16];
    int ip1=0;      //ip1 is the number of bits that can be set deffernce.
    for(int i=0;i<16;i++) if((p->deff[i]&pad_one[def_line])==0)    set_def_ip(defs,i,def_line,&ip1); 

    //set the set defferences weight    
    p->cur_hw+=((p->line_hw[def_line])-line_hw);    
    p->line_curhw[def_line]=p->line_hw[def_line];

    int pad_def_point[16];    
    for(int i=0;i<(p->line_hw[def_line]-line_hw);i++) pad_def_point[i]=p->line_hw[def_line]-line_hw-1-i;    

    do{

        //set defferences to p->lines[ip] line. the word number is 
        for(int i=0;i<(p->line_hw[def_line]-line_hw);i++)
            p->deff[defs[pad_def_point[i]].word]=p->deff[defs[pad_def_point[i]].word] | pad_one[def_line];

        //if the weight of set defferences is p->max_hw, check. we do not just test hv1.

        if((p->cur_hw)==p->max_hw) p->pass_num+=hv1_fin_check(p);           
        else next_hv2(p);//else send to next_hv2          

        //remove the defferences that this step pad        
        for(int i=0;i<(p->line_hw[def_line]-line_hw);i++)
            p->deff[defs[pad_def_point[i]].word]=p->deff[defs[pad_def_point[i]].word] & pad_zero[def_line];
    }while(next_zh(pad_def_point,(p->line_hw[def_line]-line_hw),16-line_hw));//next defference combination in this line
    p->cur_hw-=((p->line_hw[def_line])-line_hw);
    p->line_curhw[def_line]=line_hw;
    return false;//all defferences combination in this line has been test
}

/*hv1*/
unsigned int hv1(param *p){
    unsigned int  re=0;
    for(int i=0;i<16;i++) re=re^p->deff[i]; 
    return re;
}
/*hv1 check function*/
int hv1_fin_check(param *p){
    if (hv1(p)>0) return 0;
    if(p->hv_num>1)  return hv2_fin_check(p);
    else collision_out(p);
    return 1;
}
