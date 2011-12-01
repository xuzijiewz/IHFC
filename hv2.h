//set a defference and send to next_hv3
bool next_hv2(param *p){
    //find the deffence that do not satisfy hv2
    unsigned int re=hv2(p);
    if(re==0){//all defference satisfy hv2
        if((p->hv_num)==2) return next_hv1(p);//if just test hv1 and hv2, move to hv1
        return next_hv3(p);   //if test more than 2 inside hash function, move hv3
    }
    //compute the defference that do not satisfy hv2. if the defferences is more than system can set, then return.
    int hw=0;
    for(int i=0;i<16;i++) hw+=(re>>i)%2;
    if(hw>((p->max_hw)-(p->cur_hw))) return false;
    //find the first defference
    int ip=0;
    while((re&pad_one[ip])==0) ip++;    
    //find the place that can set defference to make the defference that do not satisfy hv2 satisfy hv2
    def_node def_ip[16];
    int ip1=0;    
    for(int i=0;i<16;i++)
        if(p->line_curhw[i]<p->line_hw[i])            // the line has free bit         
            if((p->deff[ip]&pad_one[i])==0)            //the bit is 0            
                set_def_ip(def_ip,ip,i,&ip1);
   
    //set a defference and send deff to hv3 or hv1
    p->cur_hw++;    
    for(int i=0;i<ip1;i++){
        //set a defference        
        p->deff[def_ip[i].word]=p->deff[def_ip[i].word] | pad_one[def_ip[i].ip];
        p->line_curhw[def_ip[i].ip]++;
        //if the weught is enough, check deff
        if((p->cur_hw)==p->max_hw) p->pass_num+=hv1_fin_check(p);           
        else {
            if((p->hv_num)==2)  next_hv1(p);
            else next_hv3(p);
        }        
        //remove the defference that had been set in this step       
        p->deff[def_ip[i].word]=p->deff[def_ip[i].word] & pad_zero[def_ip[i].ip];
        p->line_curhw[def_ip[i].ip]--;
    }
    p->cur_hw--;
    return false;
}
/*hv2*/
unsigned int hv2(param *p){
    unsigned int re=0,t=0;
    int rn[4]={8,4,2,1};
    for(int i1=0;i1<16;i1++) {
        t=p->deff[i1];
        for(int i2=0;i2<4;i2++)t=(t>>rn[i2])^t;    
        re=re|((t&0x1)<<i1);
    }    
    return re;
}
/*hv2 check function*/
int hv2_fin_check(param *p){
    if(hv2(p)!=0) return 0;
    if(p->hv_num>2)  return hv3_fin_check(p);
    else collision_out(p);
    return 1;    
}
