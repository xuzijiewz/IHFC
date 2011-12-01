/*set a defference and send deff to next hv*/
bool next_hv4(param *p){
    //find the defference that do not satisfy hv4
    unsigned int re=hv4(p);
    if(re==0) return  next_hv1(p);
    //compute the defference that do not satisfy hv2. if the defferences is more than system can set, then return.
    int hw=0;
    for(int i=0;i<16;i++) hw+=(re>>i)%2;
    if(hw>((p->max_hw)-(p->cur_hw))) return false;
    //set a new defference and send to next hv
    int ip=0;
    while((re&pad_one[ip])==0) ip++;
    //find the place that can used to set defference
    def_node def_ip[16];
    int ip_num=0;
    
    for(int i=0;i<16;i++)
        {
            if((p->deff[(16+ip-i)%16]&pad_one[i])==0) 
                if(p->line_curhw[i]<p->line_hw[i]&&(16+ip-i)%16<8)
                    set_def_ip(def_ip,(16+ip-i)%16,i,&ip_num); 
        
            if((p->deff[(15+ip-i)%16]&pad_one[i])==0) 
                if(p->line_curhw[i]<p->line_hw[i]&&(15+ip-i)%16>7)
                    set_def_ip(def_ip,(15+ip-i)%16,i,&ip_num); 
                    }
    //set a new defference
    p->cur_hw++;
    for(int i=0;i<ip_num;i++){
        //set a new defference        
        p->deff[def_ip[i].word]=p->deff[def_ip[i].word] | pad_one[def_ip[i].ip];
        p->line_curhw[def_ip[i].ip]++;
        //if weight is enough,check deff
        if((p->cur_hw)==p->max_hw) p->pass_num+=hv1_fin_check(p);           
        else next_hv1(p);//send to hv1      
        //remove the set defference     
        p->deff[def_ip[i].word]=p->deff[def_ip[i].word] & pad_zero[def_ip[i].ip];
        p->line_curhw[def_ip[i].ip]--;
    }
    p->cur_hw--;
    return false;
}
/*hv4*/
unsigned int hv4(param *p){
     unsigned int re=0;
    for(int i=0;i<8;i++) 
        re=re^ro(p->deff[i],(16-i));
    for(int i=8;i<16;i++) 
        re=re^ro(p->deff[i],(15-i));
    return re;
}
/*hv4 check function*/
int hv4_fin_check(param *p){
    if(hv4(p)!=0) return 0; 
    collision_out(p);   
    return 1;
}