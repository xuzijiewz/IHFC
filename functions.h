bool next_number(int m[],int num){
    //next weught rank
    //n,0,0,0         2,2,2,2 
    //find the place where start change
    if(m[num-1]<(m[0]+4)) return false;
    int ip=0;
    while(m[ip+1]<(m[0]+4) && ip<(num-1)) ip++;    
    //start change
    int sum=0;
    for(int j=0;j<ip+1;j++) sum=sum+m[j];
    m[ip+1]-=2;
    sum=sum+2;
    int avg_num=sum/m[ip+1];
    int num1=sum%m[ip+1];
    int j;
    for(j=0;j<avg_num;j++) m[ip-j]=m[ip+1];
    m[ip-j]=num1;
    for(j=ip-avg_num-1;j>=0;j--)m[j]=0;    
    return true;
}

bool next_pl(int m[],int num){
    //next rank
    //find the place where start change.
    int ip=0;
    while((m[ip]>=m[ip+1]) && (ip<(num-1))) ip++;
    if(ip==num-1) return false;
    //find the number that will take place m[ip+1]
    int ip1=0;
    while(m[ip1]>=m[ip+1])ip1++;  
    unsigned int wap=m[ip+1];    
    m[ip+1]=m[ip1];
    //find the place where place wap
    int ip2=ip1+1;
    while(m[ip2]>wap) ip2++;
    while(ip1<ip2-1) {
        m[ip1]=m[ip1+1];
        ip1++;
    }
    m[ip2-1]=wap;
    //begin change.
    int i=0;
    while(i<(ip-i)){
        wap=m[i];
        m[i]=m[ip-i];
        m[ip-i]=wap;
        i++;
    }    
    return true;
}

bool next_zh(int m[],int n,int k){
    //next combination, pick n from k
    //0,1,...,n-1      k-n,...,k-2,k-1
    int ip=0;
    while(m[ip]==(k-ip-1)) ip++;
    if(ip==n) return false;
    m[ip]++;
    for(int i=ip;i>0;i--)m[i-1]=m[i]+1;
    return true;
}