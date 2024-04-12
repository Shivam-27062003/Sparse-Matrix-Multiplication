#include <iostream>
#include <cstring>
#include "library.hpp"
#include<omp.h>

unordered_map<unsigned int,vector<pair<unsigned int,int*>>>mat;
unordered_map<unsigned int,int*>res;
const int INF = (1<<16)-1;
const unsigned int num_255 = 255;
string str = "";


inline void input(unsigned int &x){
    x = 0;
    vector<unsigned char>a(4);
    for(int i=3;i>=0;i--){
        a[i]=getchar_unlocked();
    }
    x = (a[0] << 24) | (a[1] << 16) | (a[2] << 8) | a[3];
}

inline void input_block(int *a,unsigned int m){
    for(int i=0;i<m*m;i++)
        a[i]=getchar_unlocked();
}

inline void write(unsigned int x){
    unsigned char a[4];
    for (int i = 0; i < 4; i++){
        a[i] = x & num_255;
        x >>= 8;
    }
    for(auto i: a)str+=i;
}

inline void helper(int x){
    unsigned char c = x & 0xff;
    str+=c;
    c = (x >> 8) & num_255;
    str+=c;
}

inline void write_block(int *a, unsigned int m){
    for (unsigned int i = 0; i < m * m; i++)helper(a[i]);
}

inline void INNER_operation(int* ans,int *a,int *b,unsigned m){
    #pragma omp parallel for
    for (int i = 0; i < m; i++){
        for (int j = 0; j < m; j++) ans[i * m + j] = Inner(a[i * m],b[j]);
    }
    #pragma omp parallel for
    for (int i = 0; i < m; i++){
        for (int k = 1; k < m; k++){
            for (int j = 0; j < m; j++){
                ans[i * m + j] = Outer(ans[i * m + j], Inner(a[i * m + k], b[k * m + j]));
            }
        }
    }
    for (int i = 0; i < m * m; i++) ans[i] = min(ans[i], INF);
}

inline void OUTER_operation(int *a,int *b,unsigned m){
    for(int i=0;i<m*m;i++){
        a[i]=Outer(a[i],b[i]);
        a[i]=min(a[i],INF);
    }
}       

inline void transpose(int *a,int *b,int m){
    int u,v;
    for(int i=0;i<m*m;i++){
        int u = i/m;
        int v = i%m;
        b[v*m+u]=a[i];
    }
}

inline void multiply(unsigned int n,int m,int k){
    for(auto &x: mat){
        auto i = x.first;
        auto v = x.second;
        for(auto &y: v){
            auto k = y.first;
            auto v1 = y.second;
            for(auto &z: mat[k]){
                auto j = z.first;
                auto v2 = z.second;
                if (j < i) continue;
                int* temp = (int*) malloc(m * m * sizeof(int));
                #pragma omp parallel for
                for (int i = 0; i < m; i++){
                    for (int j = 0; j < m; j++) temp[i * m + j] = Inner(v1[i * m], v2[j]);
                }
                INNER_operation(temp,v1,v2,m);
                if (res.find(i * n + j) == res.end()) res[i * n + j] = temp;
                else OUTER_operation(res[i * n + j], temp, m);
            }
        }
    }
}

inline void multiply(int n,int m,int k){
    #pragma omp parallel
    {
        #pragma omp master
        {
            for(auto &x: mat){
                {
                    auto i = x.first;
                    auto v = x.second;
                    for(auto &y: v){
                        auto k = y.first;
                        for(auto &z: mat[k]){
                            auto j = z.first;
                            if (j < i) continue;
                                #pragma omp task depend(inout: res)
                                { 
                                    auto v1 = y.second;
                                    auto v2 = z.second;
                                    int* temp = (int*) malloc(m * m * sizeof(int));
                                    INNER_operation(temp,v1,v2,m);
                                    if (res.find(i * n + j) == res.end()) res[i * n + j] = temp;
                                    else OUTER_operation(res[i * n + j], temp, m);
                                }
                        }
                    }
                }
            }
        }
    }
}

inline void tranfer(int n,int m,int k){
    write(n*m);
    write(m);
    write((unsigned int)res.size());
    for (auto k: res){
        auto i=k.first;
        auto x=k.second;
        write(i/n);
        write(i%n);
        write_block(x, m);
    }
    cout<<str;
}

signed main(int argc,char *argv[]){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    freopen(argv[1],"r",stdin);
    freopen(argv[2],"w",stdout);
    unsigned int n,m,k;
    
    input(n);
    input(m);
    input(k);
    n = n/m;
    while(k--){
        int *a = (int*) malloc(m * m * sizeof(int));
        unsigned int i,j;
        input(i);
        input(j);
        input_block(a,m);
        mat[i].emplace_back(j,a);
        if(i!=j){
            int *b = (int*) malloc(m * m * sizeof(int)); 
            transpose(a,b,m);
            mat[j].emplace_back(i,b);
        }
    }
    if(m>=6)multiply((unsigned int)n,m,k);
    else{
        multiply((int)n,(int)m,(int)k);
    }
    tranfer(n,m,k);
    return 0;
}