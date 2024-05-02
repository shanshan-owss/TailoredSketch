#ifndef TAILOREDSKETCH_CODE_CHOOSE_SKE_H
#define TAILOREDSKETCH_CODE_CHOOSE_SKE_H

#include "Sketch.h"
#include "CM_Sketch.h"
#include "CU_Sketch.h"
#include "C_Sketch.h"
#include "StingyCM.h"
#include "StingyCU.h"
#include "StingyCM_Base.h"
#include "StingyCU_Base.h"
#include "StingyC.h"
#include "Tailored.h"
#include "O_Tailored.h"
#include "CountLess.h"
#include "StingyCM_Sample.h"

Sketch* Choose_Sketch(uint32_t w, uint32_t d, uint32_t hash_seed = 1000,int id=10){
    switch (id){
        case 0:return new StingyCM(w,d,hash_seed);
        case 1:return new StingyCU(w,d,hash_seed);
        case 2:return new StingyCM_Base(w,d,hash_seed);
        case 3:return new StingyCU_Base(w,d,hash_seed);
        case 4:return  new StingyCM_Sample(w,d,hash_seed);
        case 5: return  new StingyC(w,d,hash_seed);


        case 10:return new CM_Sketch(w,d,hash_seed);
//        case 11:return new CM_Sketch_HS(w,d,hash_seed);
//        case 12:return new CM_Sketch_PQ(w,d,hash_seed);
        case 20:return new CU_Sketch(w,d,hash_seed);
//        case 21:return new CU_Sketch_HS(w,d,hash_seed);
//        case 22:return new CU_Sketch_PQ(w,d,hash_seed);
        case 30:return new C_Sketch(w,d,hash_seed);
//        case 31:return new C_Sketch_HS(w,d,hash_seed);
//        case 32:return new C_Sketch_PQ(w,d,hash_seed);

        case 40:return new CountLess(w,d,hash_seed);

        case 50:return  new O_Tailored(w,d,hash_seed);
        case 51:return  new Tailored(w,d,hash_seed);


    }
    return NULL;
}




#endif //TAILOREDSKETCH_CODE_CHOOSE_SKE_H
