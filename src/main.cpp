#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <numeric>
#include "Choose_Ske.h"
#include <sstream>
#include <unordered_set>
#include <set>
#include <cstring>
#include <stdlib.h>
#include "random"
#include "MurmurHash.h"
#define x (1.05)
double F = 0.15, S = 4.4;
using namespace std;
vector<string> items;
unordered_map<string, int>freq;

int memaccess_i = 0, memaccess_q = 0, memaccess_d = 0;

int thres_mid = 16, thres_large = 1024;

double item_num = 0, flow_num = 0;
vector<double> experiment_ARE, experiment_AAE,experiment_CON, experiment_through_insert, experiment_through_query/*, experiment_sum, exper_max*/;

int D;


void readFile_CAIDA(const char* filename, int length, int MAX_ITEM = INT32_MAX){

    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open())
        cout << "File fail." << endl;
    int max_freq = 0;
    char key[length];
    for (int i = 0; i < MAX_ITEM; ++i)
    {
        inFile.read(key, length);
        if (inFile.gcount() < length)
            break;
        items.push_back(string(key, length));
        freq[string(key, length)]++;
    }

    inFile.close();
    for (auto pr : freq)
        max_freq = max(max_freq, pr.second);

    vector<int> fsd;
    for (auto pr : freq)
        fsd.push_back(pr.second);
//	nth_element(fsd.begin(), fsd.begin() + fsd.size() * .99, fsd.end());
    thres_large = fsd[fsd.size() * .99];

    item_num = items.size();
    flow_num = freq.size();
    cout << "dataset name: " << filename << endl;
    cout << freq.size() << "flows, " << items.size() << " items read" << endl;;
//    cout << "max freq = " << max_freq << endl;
}


void My_calcAcc(const vector<int>* ret_f,const int mem_in_byte,const int ID)
{
    string Mem = to_string(mem_in_byte);
    double _ARE = 0, _AAE = 0, _CON = 0;
    for (int i = 0; i < testcycles; ++i)
    {
        string filenametmp = string("Result") +  to_string(ID)+ Mem +to_string(i)+ string(".txt");
        const char*  Outfilename = filenametmp.c_str();
        ofstream Outfile (Outfilename,ios::out);
        int z = 0;
        for (auto pr : freq)
        {
            int est_val = ret_f[i][z++];
            int real_val = pr.second;
            int dist = abs(est_val - real_val);
            int conflict = est_val != real_val ? 1 : 0;
            _ARE += (double)dist / real_val, _AAE += dist, _CON += conflict;
//            cout<<pr.first.c_str()<<endl;
            Outfile<<est_val<<" "<<real_val<<" "<< pr.first.c_str()<<endl;
            //cout<<est_val<<','<<real_val<<endl;
        }
        Outfile.close();
    }
    _ARE /= freq.size(), _AAE /= freq.size(), _CON /= freq.size();
    _ARE /= testcycles, _AAE /= testcycles, _CON /= testcycles;
    experiment_ARE.push_back(_ARE);
    experiment_AAE.push_back(_AAE);
    experiment_CON.push_back(_CON);
    cout << "ARE = " << _ARE << ", AAE = " << _AAE<< ",CON = "<< _CON << endl;/*<< ", insert = " << throughput_i << ", query = " << throughput_o << endl;*/
}


void calcAcc(const vector<int>* ret_f)
{
    double _ARE = 0, _AAE = 0, _CON = 0;
    for (int i = 0; i < testcycles; ++i)
    {

        int z = 0;
        for (auto pr : freq)
        {
            int est_val = ret_f[i][z++];
            int real_val = pr.second;
            int dist = abs(est_val - real_val);
            int conflict = est_val != real_val ? 1 : 0;
            _ARE += (double)dist / real_val, _AAE += dist, _CON += conflict;

            //cout<<est_val<<','<<real_val<<endl;
        }
    }
    _ARE /= freq.size(), _AAE /= freq.size(), _CON /= freq.size();
    _ARE /= testcycles, _AAE /= testcycles, _CON /= testcycles;
    experiment_ARE.push_back(_ARE);
    experiment_AAE.push_back(_AAE);
    experiment_CON.push_back(_CON);
    cout << "ARE = " << _ARE << ", AAE = " << _AAE<< ",CON = "<< _CON << endl;/*<< ", insert = " << throughput_i << ", query = " << throughput_o << endl;*/
}


void test_BCM(int mem_in_byte, int _switch_num)
{
    int d = D;  //counts of hash function
    int w = mem_in_byte;  //   bits/counter_size/hash_counts
    int switch_num = _switch_num;
    vector<int> ret_f[testcycles];
    double throughput_i = 0, throughput_o = 0;
    long long estimate_sum = 0;
    for (int i = 0; i < testcycles; ++i)
    {
        LARGE_INTEGER frequency;        // ticks per second
        LARGE_INTEGER t1, t2, t3, t4;  // ticks
        long long resns;               // 纳秒计时结果
        QueryPerformanceFrequency(&frequency);

        Sketch *bcm;
        bcm=Choose_Sketch(w,d, i * 100,switch_num);
        QueryPerformanceCounter(&t1);
        for (auto key : items){
            bcm->Insert(key.c_str());
        }
        QueryPerformanceCounter(&t2);

        QueryPerformanceCounter(&t3);
        for (auto pr : freq)
            estimate_sum += bcm->Query(pr.first.c_str());
        QueryPerformanceCounter(&t4);

        // 转换为纳秒
        resns = (t2.QuadPart - t1.QuadPart) * 1000000000LL / frequency.QuadPart;
        throughput_i += (double)1000.0 * item_num / resns;
        resns = (t4.QuadPart - t3.QuadPart) * 1000000000LL / frequency.QuadPart;
        throughput_o += (double)1000.0 * flow_num / resns;
        for (auto pr : freq)
            ret_f[i].push_back(bcm->Query(pr.first.c_str()));
    }

    cout << endl;
    experiment_through_insert.push_back(throughput_i / testcycles);
    experiment_through_query.push_back(throughput_o / testcycles);
    My_calcAcc(ret_f,w,switch_num);
}


int main()
{
//    readFile_CAIDA("D:\\TailoredSketch_Code\\Data\\01_bin_str.dat",12);
    readFile_CAIDA("..\\Data\\zipf_example.dat",10);
    int mem_in_byte_start = 0.2 * (1<<20);
    int mem_in_byte_end = 1<<20 ;
    int mem_in_byte;
    D= 3;
    for (mem_in_byte = mem_in_byte_start; mem_in_byte <= mem_in_byte_end; mem_in_byte= mem_in_byte + int(0.05* (1 << 20)))
    {
        test_BCM(mem_in_byte,51);  //choose Sketch number
    }

//   case 0:  StingyCM;
//    case 1:  StingyCU;
//    case 2:  StingyCM_Base;
//    case 3:  StingyCU_Base;
//    case 4:  StingyCM_Sample;
//    case 5:  StingyC;
//
//    case 10:  CM_Sketch;
//
//    case 20:  CU_Sketch;
//
//    case 30:  C_Sketch;
//
//    case 40:  CountLess;
//
//    case 50:  O_Tailored;
//    case 51:  Tailored;*/


    //Per-Flow Szie measurement result writing in file, others tasks' result can gey by  Per-flow result

    ofstream oFile;
    oFile.open("sheet.csv", ios::app);
    if (!oFile) return 0;
    oFile << "AAE," << "ARE," << "insert," << "query," <<"CON,"<< endl;
    for (int o = 0, j = 0, kb = mem_in_byte; o < experiment_AAE.size(); o++){
        oFile << experiment_AAE.at(o) << "," << experiment_ARE.at(o) << "," << experiment_through_insert.at(o) << ","
              << experiment_through_query.at(o)<<","<< experiment_CON.at(o) /*<< "," << fixed << experiment_sum.at(o) << "," << fixed << exper_max.at(o)*/ << endl;
    }
    oFile.close();

    return 0;
}
