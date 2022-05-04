#include <iostream>
#include <cstring>
#include <ctime>
#include <random>
#include <algorithm>
#include "HashFunctionSHA512.h"
#include "AdaptiveBloomFilter.h"
#include "Dictionary.h"
#include <fstream>
#include <cassert>
#include "macros.h"
//#define TEST_FN 1
using namespace std;
using namespace mySHA512;

size_t seed;
int max_loop=10;
int factor=8;
int AS=32;
bool quiet=false;
unsigned int A;
int k=3;
int groups=2;
int npf=10;
int words;
int bits;
AdaptiveBloomFilter* abf;
//Dictionary* dc;
HashDictionary* hdc;
char** A_list;

string generateRandomElement(int a, int b,std::uniform_int_distribution<>& dis, std::mt19937& gen){
    string e;
    assert(a<b);
    for(int ii=0;ii<a+rand()%(b-a);ii++){
        e+=(char)dis(gen);
    }
    return e;
}

void simtime(time_t* starttime_ptr) {
    time_t endtime = time(NULL);
    struct tm * timeinfo=localtime(starttime_ptr);
    double second = difftime(endtime,*starttime_ptr);
    printf("simulation started @: %s \n", asctime(timeinfo));
    timeinfo=localtime(&endtime);
    printf("simulation ended   @: %s \n",asctime(timeinfo));
    printf("simulation time: %f sec \n",second);
}

void PrintUsage() {
    printf("usage:\n");
    printf(" ***\n");
    printf(" -w words: number of words\n");
    printf(" -b bits: number of bits per word\n");
    printf(" -n num_packets: number of packets for each flow \n");
    printf(" -a as_ratio: set the A/S ratio \n");
    printf(" -S seed: select random seed (for debug)\n");
    printf(" -f factor : set the factor of element to words \n");
    printf(" -k nhash : number of hash functions to select an element\n");
    printf(" -g groups : number of hash function groups \n");
    printf(" -h print usage\n");
}

void print_command_line(int argc, char* argv[]) {
    //print command line
    printf("command executed with command line: ");
    char **currentArgv = argv;
    for (int i = 0; i < argc; i ++) {
        printf("%s ", *currentArgv); /* %s instead of %c and drop [i]. */
        currentArgv++; /* Next arg. */
    }
    printf("\n");
}

void init(int argc, char **argv){
    printf("\n===========================================\n");
    printf("Simulator for the Adaptive Bloom Filter\n");
    printf("Run %s -h for usage\n",argv[0]);
    printf("===========================================\n\n");

    print_command_line(argc,argv); //print the command line with the option
    seed=time(nullptr);
    // Check for switches
    while (argc > 1 && argv[1][0] == '-'){
        argc--;
        argv++;
        int flag=0; //if flag 1 there is an argument after the switch
        char c = 0;
        while ((c = *++argv[0])){
            switch (c) {
                case 'q':
                    printf("\nQuiet enabled\n");
                    quiet=true;
                    break;
                case 'a':
                    flag=1;
                    AS=atoi(argv[1]);
                    argc--;
                    break;
                case 'w':
                    flag=1;
                    words=atoi(argv[1]);
                    argc--;
                    break;
                case 'S':
                    flag=1;
                    seed=atoi(argv[1]);
                    argc--;
                    break;
                case 'b':
                    flag=1;
                    bits=atoi(argv[1]);
                    argc--;
                    break;
                case 'f':
                    flag=1;
                    factor=atoi(argv[1]);
                    argc--;
                    break;
                case 'h':
                    PrintUsage();
                    exit(1);
                    break;
                case 'n':
                    flag=1;
                    npf=atoi(argv[1]);
                    argc--;
                    break;
                case 'k':
                    flag=1;
                    k=atoi(argv[1]);
                    argc--;
                    break;
                case 'g':
                    flag=1;
                    groups=atoi(argv[1]);
                    argc--;
                    break;
                default :
                    printf("Illegal option %c\n",c);
                    PrintUsage();
                    exit(1);
                    break;
            }
        }
        argv= argv + flag;
    }
    A=factor*words*AS;
    //Print general parameters
    printf("general parameters: \n");
    max_loop = 5;
    printf("seed: %d\n",seed);
    printf("number of words: %d\n",words);
    printf("number of bits per word: %d\n",bits);
    printf("number of hash function per group: %d\n",k);
    printf("number of groups of hash function: %d\n",groups);
    printf("A size: %d\n",A);
    printf("iterations: %d\n",max_loop);
    printf("AS ratio: %d\n",AS);
    printf("npf: %d\n",npf);
    printf("---------------------------\n");

    abf = new AdaptiveBloomFilter(words,bits,k,groups);
    hdc = new HashDictionary(ceil(words*factor*0.8));
}

void run(){
    time_t starttime = time(nullptr);

    srand(seed);
    std::random_device rd;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0,255);

    ofstream logfile(string(RESULT_PATH)+RESULT_FILE,ios::app);
    setbuf(stdout, nullptr);

    //int num_fails=0;
    int64_t tot_i=factor*words;
    int64_t num_swap=0;
    int64_t num_iter=0;
    int64_t max_FP=0;
    int64_t min_FP=INT_MAX;
    int64_t tot_FP=0;
    int64_t tot_count=0;
    A_list = new char*[A];
    for(int i=0;i<A;i++) A_list[i] = nullptr;

    char info[100];
    sprintf(info,"Initializing parameters words=%d, bits=%d, k=%d, groups=%d, factor=%d, AS=%d",words, bits, k, groups, factor,AS);
    cout<<info<<endl;
#ifndef TEST_FN
    sprintf(info,R"({"words" : %d, "bits" : %d, "k" : %d, "groups" : %d, "factor" : %d, "AS" : %d})",words, bits, k, groups, factor,AS);
    logfile<<info<<endl;
#endif

    for(int loop=0;loop<max_loop;loop++){
        int64_t sample_FP=0;
        abf->clear();
        hdc->clear();
        for(int i=0;i<A;i++){
            delete[] A_list[i];
            A_list[i] = nullptr;
        }
        bool fail_insert=false;

        for (int64_t i = 0;  i <tot_i;  i++){
            string element = generateRandomElement(5,55,dis,gen);
            if(hdc->query(element.c_str())){
                i--;
                continue;
            }
            hdc->add(element.c_str());
            abf->add(element.c_str());
        }
        printf("---------------------------\n");
        printf("End insertion\n");


        for(int64_t i = 0;i < A; i++){
            string element = generateRandomElement(5,55,dis,gen);
#ifndef TEST_FN
            if(hdc->query(element.c_str()))
            {
                i--;
                continue;
            }
#endif
            A_list[i]= strdup(element.c_str());
        }

        //int64_t count=0;
        int64_t ar_size=A;
        num_iter=npf*ar_size;

        for(int64_t iter=0; iter<num_iter; iter++){
            char* key = A_list[(unsigned int)rand() % ar_size];
            //count++;
            tot_count++;
            if(abf->query(key)){
#ifdef TEST_FN
                if(!hdc->query(key))
#endif
                {
                    tot_FP++;
                    sample_FP++;
                    abf->adapt(key);
                    num_swap++;
                }

            }
#ifdef TEST_FN
            else if(hdc->query(key)){
                cerr<<"false negative found"<<endl;
            }
#endif
        }

        printf("ACF FP: %lu : %.6f \n",sample_FP,sample_FP/(num_iter+0.0));
        printf("---------------------------\n");
        if (sample_FP<min_FP) min_FP=sample_FP;
        if (sample_FP>max_FP) max_FP=sample_FP;
    }
    for(int i=0;i<A;i++){
        delete[] A_list[i];
        A_list[i] = nullptr;
    }
    delete[] A_list;
    A_list = nullptr;
    printf("---------------------------\n");
    printf("---------------------------\n");
    printf("stat:ACF FP min/ave/max %lu %lu %lu \n",min_FP,tot_FP/max_loop, max_FP);
    printf("stat:ACF FPR(%lu) : %.6f \n",tot_FP,tot_FP/(tot_count+0.0));
    printf("stat:num SWAP : %ld \n",num_swap);
#ifndef TEST_FN
    sprintf(info, "%.6f",tot_FP/(tot_count+0.0));
    logfile<<info<<endl;
#endif
    logfile.close();
    simtime(&starttime);
}

int main(int argc, char **argv) {
    init(argc,argv);
    run();
    return 0;
}
