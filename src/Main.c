//#include "C:/Wichtig/System/Static/Container/Hashmap.h"
#include "C:/Wichtig/System/Static/Container/Array.h"
#include "C:/Wichtig/System/Static/Container/List.h"
#include "C:/Wichtig/System/Static/Container/Pair.h"
#include "C:/Wichtig/System/Static/Library/String.h"

typedef unsigned long long Hash;

Hash Hash_Simple(int Max,void* Hash){
    return *((int*)Hash) % Max;
}

char Hash_CmpKeys(void* Key1,void* Key2){
    int* AcK1 = (int*)Key1;
    int* AcK2 = (int*)Key2;
    return *AcK1==*AcK2;
}

void Hash_Print(Pair* p){
    printf("Pair: %d : %d, ",*((int*)Pair_First(p)),*((int*)Pair_Second(p)));
}

typedef struct Hashmap {
    Hash (*HowHash)(int Max,void* Hash);
    char (*CmpKeys)(void* Key1,void* Key2);
    void (*Print)(Pair* p);
    Array Data;
} Hashmap;

Hashmap Hashmap_New_Ex(unsigned int SIZE,Hash (*HowHash)(int Max,void* Hash),char (*CmpKeys)(void* Key1,void* Key2),void (*Print)(Pair* p)) {
    Hashmap hm;
    hm.HowHash = HowHash;
    hm.CmpKeys = CmpKeys;
    hm.Print = Print;
    hm.Data = Array_New(sizeof(List),SIZE);
    return hm;
}

Hashmap Hashmap_New(unsigned int SIZE) {
    Hashmap hm;
    hm.HowHash = Hash_Simple;
    hm.CmpKeys = Hash_CmpKeys;
    hm.Print = Hash_Print;
    hm.Data = Array_New(sizeof(List),SIZE);
    return hm;
}

void Hashmap_Add(Hashmap* v,unsigned int KEY_SIZE,void* KeyIn,unsigned int ELEMENT_SIZE,void* Item) {
    List* list = (List*)Array_Get(&v->Data,v->HowHash(v->Data.SIZE,KeyIn));
    if(list->size<=0){
        *list = List_New(sizeof(Pair));
    }
    Pair p = Pair_New(KEY_SIZE,KeyIn,ELEMENT_SIZE,Item);
    List_Push(list,sizeof(Pair),&p);
}

void Hashmap_Remove(Hashmap* v, void* KeyIn) {
    List* list = (List*)Array_Get(&v->Data,v->HowHash(v->Data.SIZE,KeyIn));
    if(list->size>0){
        for(int i = 0;i<list->size;i++){
            Pair* p = (Pair*)List_Get(list,i);
            if(v->CmpKeys(*(char**)Pair_First(p),*(char**)KeyIn)){
                List_Remove(list,i);
                break;
            }
        }
    }
}

void* Hashmap_Find(Hashmap* v, void* KeyIn) {
    List* list = (List*)Array_Get(&v->Data,v->HowHash(v->Data.SIZE,KeyIn));
    for(int i = 0;i<list->size;i++){
        Pair* p = (Pair*)List_Get(list,i);
        if(v->CmpKeys(*(char**)Pair_First(p),*(char**)KeyIn)) return Pair_Second(p);
    }
    return NULL;
}

void Hashmap_ForEach(Hashmap* v,void (*Func)(Pair* p)) {
    for(int i = 0;i<v->Data.SIZE;i++){
        List* list = (List*)Array_Get(&v->Data,i);
        if(list){
            for(int j = 0;j<list->size;j++){
                Pair* p = (Pair*)List_Get(list,j);
                if(p->Data) Func(p);
            }
        }
    }
}

void Hashmap_Free(Hashmap* v) {
    for(int i = 0;i<v->Data.SIZE;i++){
        List* list = (List*)Array_Get(&v->Data,i);
        if(list){
            for(int j = 0;j<list->size;j++){
                Pair* p = (Pair*)List_Get(list,j);
                if(p->Data) Pair_Free(p);
            }
            List_Free(list);
        }
    }
    Array_Free(&v->Data);
}


void Hashmap_Print(Hashmap* v) {
    printf("-- Hashmap --\n");
    for(int i = 0;i<v->Data.SIZE;i++){
        printf("| List: ");
        
        List* list = (List*)Array_Get(&v->Data,i);
        if(list){
            for(int j = 0;j<list->size;j++){
                Pair* p = (Pair*)List_Get(list,j);
                if(p && p->Data) v->Print(p);
            }
        }
        printf("\n");
    }
    printf("-------------\n");
}



Hash CStr_Hash(int Max,void* Key){
    char* cstr = *(char**)Key;
    //int len = CStr_Size(cstr);
    Hash h = CStr_SimpleHash(cstr);
    return h % Max;
}
void CStr_Print(Pair* p){
    if(p)   printf("Pair: %s : %d, ",*((char**)Pair_First(p)),*((int*)Pair_Second(p)));
}
void Pair_FreeKey(Pair* p){
    free(*(char**)(Pair_First(p)));
}

int main(){
    /*
    Hashmap hmap = Hashmap_New(10);

    Hashmap_Add(&hmap,4,(int[]){ 0 },4,(int[]){ 3 });
    Hashmap_Add(&hmap,4,(int[]){ 1 },4,(int[]){ 4 });
    Hashmap_Add(&hmap,4,(int[]){ 2 },4,(int[]){ 5 });
    Hashmap_Add(&hmap,4,(int[]){ 3 },4,(int[]){ 6 });
    Hashmap_Add(&hmap,4,(int[]){ 4 },4,(int[]){ 7 });

    Hashmap_Print(&hmap);
    
    Hashmap_Remove(&hmap,(int[]){ 1 });
    Hashmap_Remove(&hmap,(int[]){ 2 });
    
    Hashmap_Add(&hmap,4,(int[]){ 4 },4,(int[]){ 8 });
    Hashmap_Add(&hmap,4,(int[]){ 5 },4,(int[]){ 9 });

    Hashmap_Print(&hmap);

    Hashmap_Free(&hmap);
    */

    Hashmap hmap = Hashmap_New_Ex(10,CStr_Hash,(char (*)(void*,void*))CStr_Cmp,CStr_Print);

    Hashmap_Add(&hmap,8,(char*[]){ CStr_Cpy("First")    },4,(int[]){ 1 });
    Hashmap_Add(&hmap,8,(char*[]){ CStr_Cpy("Second")   },4,(int[]){ 2 });
    Hashmap_Add(&hmap,8,(char*[]){ CStr_Cpy("Third")    },4,(int[]){ 3 });
    Hashmap_Add(&hmap,8,(char*[]){ CStr_Cpy("Fourth")   },4,(int[]){ 4 });
    Hashmap_Add(&hmap,8,(char*[]){ CStr_Cpy("Fivth")    },4,(int[]){ 5 });

    Hashmap_Print(&hmap);
    
    Hashmap_Remove(&hmap,(char*[]){ "Fourth" });
    Hashmap_Remove(&hmap,(char*[]){ "Fivth" });

    void* Found = Hashmap_Find(&hmap,(char*[]){ "Third" });
    if(Found)   printf("Found: %d\n",*(int*)Found);
    else        printf("Not Found!\n");
    
    Hashmap_Add(&hmap,8,(char*[]){ CStr_Cpy("Sixth")    },4,(int[]){ 6 });
    Hashmap_Add(&hmap,8,(char*[]){ CStr_Cpy("Seventh")  },4,(int[]){ 7 });

    Hashmap_Print(&hmap);

    Hashmap_ForEach(&hmap,Pair_FreeKey);
    Hashmap_ForEach(&hmap,Pair_Free);

    Hashmap_Free(&hmap);

    return 0;
}