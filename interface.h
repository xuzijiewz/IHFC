#define ro(x,n)  (((x>>n) | ( x<<(16-n)))&0xffff)
struct def_node{
    int word;     //the word that defference is
    int ip;       //where the defference is. (bit place)
};

struct param{ 
    int first_time;  
 int cur_hw;  //current hamming weight
 int max_hw;  //target hamming weight
 int hv_num;  //how many inside hash function will be used
 int pass_num;  //how many collection pass
 int min_fix_cell;
 int line_hw[16];   //the weight of line
 int line_curhw[16]; //the curten weight of a line
 unsigned int deff[16];
};
param * new_param(){
    param * p=new param;
    p->first_time=1;
    p->cur_hw=0;
    p->max_hw=0;  //target hamming weight
    p->hv_num=0;  //how many inside hash function will be used
    p->pass_num=0;
    p->min_fix_cell=256;
    for(int i=0;i<16;i++){
        p->line_hw[i]=0;
        p->line_curhw[i]=0;
        p->deff[i]=0x0;        
    }
    return p;
}
/*set a bit in a word '1' */
unsigned int  pad_one[16]={0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80,0x100,0x200,0x400,0x800,0x1000,0x2000,0x4000,0x8000};
/*set a bit in a word '0'*/
unsigned int  pad_zero[16]={
0xfffe,0xfffd,0xfffb,0xfff7,
0xffef,0xffdf,0xffbf,0xff7f,
0xfeff,0xfdff,0xfbff,0xf7ff,
0xefff,0xdfff,0xbfff,0x7fff};

//set the defference, p is the number that had been set
void set_def_ip(def_node def_ip[],int word, int ip, int  *p){
    def_ip[*p].word=word;
    def_ip[*p].ip=ip;
    *p=*p+1;
}
void collision_out(param *p){
    int wn=0;
    for(int i=0;i<16;i++) if(p->deff[i]!=0)wn++;
    int cn=0;
    for(int i=0;i<16;i++) if(p->line_hw[i]!=0) cn++;
    int fix_cell_num=16*(cn+wn)-cn*wn;
    if(fix_cell_num<p->min_fix_cell) p->min_fix_cell=fix_cell_num;
    for(int i=0;i<16;i++) printf("%x ",p->deff[i]);
    printf(" fixed cell num: %d\n",fix_cell_num); 
}
bool next_number(int m[],int num);
bool next_pl(int m[],int num);
bool next_zh(int m[],int n,int k);

void hv1_ini(param *p);
/*set defference in a line and send the deff to hv2*/
bool next_hv1(param *p);
/*hv1*/
unsigned int hv1(param *p);
/*hv1 check function*/
int hv1_fin_check(param *p);

/*set a defference and send the deff to hv3*/
bool next_hv2(param *p);
/*hv2*/
unsigned int hv2(param *p);
/*hv2 check function*/
int hv2_fin_check(param *p);

/*set a defference and send the deff to hv4*/
bool next_hv3(param *p);
/*hv3*/
unsigned int hv3(param *p);
/*hv3 check function*/
int hv3_fin_check(param *p);

/*set a defference and send the deff to hv1*/
bool next_hv4(param *p);
/*hv4*/
unsigned int hv4(param *p);
/*hv4 check function*/
int hv4_fin_check(param *p);