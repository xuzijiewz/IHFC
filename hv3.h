/*set a defference and send deff to next hv*/
bool next_hv3(param *p){
    //find the defference that do not satisfy hv3, if there is no defference do not satisfy hv3, goto next hv
    unsigned int re=hv3(p);
    if(re==0){
        if((p->hv_num)==3) return next_hv1(p);
        return next_hv4(p);
    }
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
    for(int i=0;i<16;i++) //p->lines[i] line -ip=word ip
        if(p->line_curhw[i]<p->line_hw[i])
            if((p->deff[(16+i-ip)%16]&pad_one[i])==0)            
                set_def_ip(def_ip,(16+i-ip)%16,i,&ip_num);
        
    //set a defference
    p->cur_hw++;
    for(int i=0;i<ip_num;i++){
        //set a new defference       
        p->deff[def_ip[i].word]=p->deff[def_ip[i].word] | pad_one[def_ip[i].ip];
        p->line_curhw[def_ip[i].ip]++;
        //if the weight enough, check deff
        if((p->cur_hw)==p->max_hw) p->pass_num+=hv1_fin_check(p);           
        else {
            if((p->hv_num)==3) next_hv1(p);
            else next_hv4(p);
        }//send to next hv      
        //remove the set defference
        p->deff[def_ip[i].word]=p->deff[def_ip[i].word] & pad_zero[def_ip[i].ip];
        p->line_curhw[def_ip[i].ip]--;
    }
    p->cur_hw--;
    return false;
}
/*hv3*/
unsigned int hv3(param *p){
    unsigned int re=0;
    for(int i=0;i<16;i++) re=re^ro(p->deff[i],i);    
    return re;
}
/*hv3 check function*/
int hv3_fin_check(param *p){
    if(hv3(p)!=0) return 0;
    if(p->hv_num>3)  return hv4_fin_check(p);
    else collision_out(p);
    return 1;
}