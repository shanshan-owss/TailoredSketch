#ifndef TAILOREDSKETCH_CODE_COUNTLESS_H
#define TAILOREDSKETCH_CODE_COUNTLESS_H

# include "params.h"
# include <iostream>
#include <string.h>
#include <iostream>
#include "MurmurHash.h"
#include "Sketch.h"
#include <random>
#include <mmintrin.h>
#include "time.h"
#include <stdio.h>
#include <stdlib.h>

#define Min(a,b) 	((a) < (b) ? (a) : (b))

using namespace std;
class CountLess:public Sketch{
public:
    int w0,w1,w2, d;
    unsigned int* counter[MAX_HASH_NUM];
    int COUNTER_SIZE_MAX_CNT = (1 << (COUNTER_SIZE - 1)) - 1;
    int hash_seed;
    int index[MAX_HASH_NUM];    //index of each d

public:
    CountLess(int _w, int _d, int _hash_seed = 1000){

        //the rate is from original paper , i.e., r = 2
        w0 = _w * 4/7;
        w1 = _w * 2/7 /2;
        w2 = _w * 1/7 /4;
        d = _d;
        srand(time(0));
        hash_seed = rand() ;
        counter[0] = new unsigned int[w0]();
        counter[1] = new unsigned int[w1]();
        counter[2] = new unsigned int[w2]();

    }
    void Insert(const char* str){
        int temp = 0;
        int min_value = COUNTER_SIZE_MAX_CNT;
        uint64_t loc_0 = MurmurHash32(str, KEY_LEN, hash_seed+0) % w0;
        if ( counter[0][loc_0] < 0xff & counter[0][loc_0] <= min_value ){
            min_value = counter[0][loc_0];
            counter[0][loc_0] ++;
        }

        uint64_t loc_1 = MurmurHash32(str, KEY_LEN, hash_seed+1) % w1;
        if ( counter[1][loc_1] < 0xffff & counter[1][loc_1] <= min_value ){
            min_value = counter[1][loc_1];
            counter[1][loc_1] ++;
        }

        uint64_t loc_2 = MurmurHash32(str, KEY_LEN, hash_seed+2) % w2;
        if ( counter[2][loc_2] < 0xffffffff & counter[2][loc_2] <= min_value ){
            min_value = counter[2][loc_2];
            counter[2][loc_2] ++;
        }

    }


    int Query(const char* str){
        uint32_t query = UINT32_MAX;

        uint64_t loc_0 = MurmurHash32(str, KEY_LEN, hash_seed+0) % w0;
        if (counter[0][loc_0] < 0xff){
            int v_0  = counter[0][loc_0];
            query = Min(query,v_0);
        }

        uint64_t loc_1 = MurmurHash32(str, KEY_LEN, hash_seed+1) % w1;
        if (counter[1][loc_1] < 0xffff){
            int v_1  = counter[1][loc_1];
            query = Min(query,v_1);
        }

        uint64_t loc_2 = MurmurHash32(str, KEY_LEN, hash_seed+2) % w2;
        if (counter[2][loc_2] < 0xffffffff){
            int v_2  = counter[2][loc_2];
            query = Min(query,v_2);
        }
        return  query;

    }


    ~CountLess()
    {
        for (int i = 0; i < d; i++){
            delete[]counter[i];
        }
    }
};


#endif //TAILOREDSKETCH_CODE_COUNTLESS_H
