//---------------------------------------------------------------------------

#pragma hdrstop

#define N 4 
//#define SKIP_SAVING

//#define KNOWN_CRNUM 60

//#define INIT 2722
#define COUNT1 56974
#define COUNT2 770562
#define COUNT3 4840481
#define CURCr 100

#define SHOW_DETAILS

#define WIDTH 4
//#define DELTA 0

#include <fstream>
#include <ctime>
#include <iomanip>
#include "dgraph.h"
using namespace std;


// FUNCTION DECLARATION

void showtime(ostream&, time_t);
void showfaces(ostream&, list<int>&, int num_each_line=16);
int getmincrnum(const char* FileName, ofstream& LogFile, int step=100);
int getmincrnum(const char* FileName);
void regroup(string, int*, int delta=2,int mincrnum=-1);
int digits(int a);

// MAIN -----------------------------------------------------------
#pragma argsused
int main(void)
{
// INITIALIZATION -------------------------------------------------
        time_t t_begin=time(NULL);
        int n = N;
        int delta=0;
        int PARTS = 1;

        if(n>=10) return 1; //we don't generate drawings of K11 from K10.

        if (n==7) delta = 2; // three output files for K7->K8.
        if (n==8) PARTS = 3; // three input files for K8->K9.

// When SKIP_SAVING is turned off, and n=9, we only search for
// optimal drawings of K10, i.e., delta=0.
#ifdef SKIP_SAVING
        if(n==9) delta = 2;
#endif

// creating file names-------------
//They look like:
//sourceFilename:   #0_K4.txt,
//logFilename:      mylog_K4to5.txt,
//saveFilename      #0_K4to5.txt,
//nautyFilename     #0_K4to5.bin.

        string sourceFilename, logFilename;

        string saveFilename, nautyFilename;

        stringstream fstr;

        fstr.str("");
        fstr<<"K"<<n;
        fstr<<".txt";
        sourceFilename=fstr.str();

        fstr.str("");
        fstr<<"mylog_K"<<n<<"to"<<n+1;
        fstr<<".txt";

        logFilename=fstr.str();

        fstr.str("");
        fstr<<"K"<<n<<"to"<<n+1;
        fstr<<".txt";

        saveFilename=fstr.str();

        fstr.str("");
        fstr<<"K"<<n;
        fstr<<"_nauty.bin";

        nautyFilename=fstr.str();

//-------------------------------------

        ofstream LogFile;
        LogFile.open(logFilename.c_str());


        unsigned long totalnum[3]={0};
        unsigned long pre_totalnum[3]={0};

#ifdef INIT
        totalnum[0]=COUNT1;
        totalnum[1]=COUNT2;
        totalnum[2]=COUNT3;
        for(int i=0;i<3;i++)
                pre_totalnum[i]=totalnum[i];
#endif


        ofstream saveFile[3];

        for(int i=0;i<=delta;i++)
        {
                stringstream fstr;
                fstr<<"#"<<i<<"_"<<saveFilename;
                saveFile[i].open(fstr.str().c_str());
                saveFile[i]<<string(35,' ')<<endl;

          //Create files for saving nauty-formatted.
          //If already exist, empty them.
                fstr.str("");
                fstr<<"#"<<i<<"_"<<nautyFilename;
                FILE *canongfp;
                canongfp=fopen(fstr.str().c_str(),"w");
                fclose(canongfp);
        }

        FILE* canongs[3];
        for(int i=0;i<=delta;i++)
        {
                fstr.str("");
                fstr<<"#"<<i<<"_"<<nautyFilename;
                canongs[i]=fopen(fstr.str().c_str(),"a+b");
        }


// END INITIALIZATION ----------------------------------------------------

#ifndef KNOWN_CRNUM
        cout<<"Computing crossing number of K_"<<n+1<<":"<<endl;
        LogFile<<"Computing crossing number of K_"<<n+1<<":"<<endl;

        int mincrnum=5000;
        for(int i=0;i<PARTS;i++)
        {   fstr.str(""); fstr<<"#"<<i<<"_"<<sourceFilename;

            int next_mincrnum=getmincrnum(fstr.str().c_str(),LogFile);

            LogFile<<fstr.str()<<" searched!"<<endl;
            cout<<fstr.str()<<" searched!"<<endl;

            if(next_mincrnum<mincrnum) mincrnum=next_mincrnum;
        }
        LogFile<<endl;
        cout<<endl;
#else
        int mincrnum= KNOWN_CRNUM;
#endif
        int maxcrnum=mincrnum+delta;
        LogFile<<"*** cr(K_"<<n+1<<")="<<mincrnum<<" ***"<<endl;
        cout<<"*** cr(K_"<<n+1<<")="<<mincrnum<<" ***"<<endl;

#ifdef SKIP_SAVING
        LogFile<<"No drawings will be saved."<<endl;
        LogFile<<"Crossing Number of K_"<<n+2<<" will be computed directly."<<endl;
        cout<<"No drawings will be saved."<<endl;
        cout<<"Crossing Number of K_"<<n+2<<" will be computed directly."<<endl;

#endif

        int NEXT_CRNUM=5000;

#ifdef INIT
        NEXT_CRNUM=CURCr;
#endif

for(int i=0;i<PARTS;i++)
{
        int draw_num=0;
        fstr.str(""); fstr<<"#"<<i<<"_"<<sourceFilename;
        LogFile<<endl<<"Processing File: "<<fstr.str()<<" ..."<<endl;
        cout<<endl<<"Processing File: "<<fstr.str()<<" ..."<<endl;

        ifstream GraphFile;
        GraphFile.open(fstr.str().c_str());
        if(GraphFile.fail()) { cout<<"Can't find file "<<sourceFilename; cin.get();return 1;}
        stringstream nextgraph;

        movetochar(GraphFile,'=');
        int total;
        GraphFile>>total;
        LogFile<<endl<<total<<" drawings to be processed:"<<endl;
        cout<<endl<<total<<" drawings to be processed:"<<endl;

        movetochar(GraphFile,'n');
while(!GraphFile.eof())
{

        nextgraph.str("");
        readnextgraph(GraphFile,nextgraph);


//Initialize Graph
        draw_num++;

#ifdef INIT
        if(draw_num<INIT) continue;
#endif

        drawing D(nextgraph);



//--------
        LogFile<<"-----------"<<endl<<"Drawing "<<draw_num<<", ";
        LogFile<<D.num_face()<<" faces:"<<endl;
        cout<<"-----------"<<endl<<"Drawing "<<draw_num<<", ";
        cout<<D.num_face()<<" faces:"<<endl;

        list<int> uniquefaces;
if(n!=5){

        D.getuniquefaces(uniquefaces);

        showfaces(LogFile,uniquefaces);
        showfaces(cout,uniquefaces);
}
else
        for(int i=1;i<=D.num_face();i++)
                uniquefaces.push_back(i);


        LogFile<<endl<<"Face Number"<<"  "<<"Number of Drawings"<<"   "<<"Local crossing number"<<endl;
        LogFile<<"---------------------------------------------------"<<endl;

#ifdef SHOW_DETAILS
        cout<<endl<<"Face Number"<<"  "<<"Number of Drawings"<<"   "<<"Local crossing number"<<endl;
        cout<<"---------------------------------------------------"<<endl;
#endif

        list<int> all_forced;
        D.forcedcrossings(uniquefaces,all_forced);
//        D.forcedcrossings(all_forced);

        list<int>::iterator itr_forced=all_forced.begin();

        list<int>::iterator itr=uniquefaces.begin();
        for(;itr!=uniquefaces.end();itr++,itr_forced++)
        {
               int fnum=*itr;

//           for(int fnum=1;fnum<=D.num_face();fnum++, itr_forced++)
//           {
               pathset paths;

               paths.reserve(MAX_NONCROSSING_SIZE);
               paths.resize(n);

               int face_mincrnum=*itr_forced+D.num_cross();
               int face_delta=maxcrnum-face_mincrnum;
               if(face_delta<0)  continue;

               D.generatepaths(fnum,paths,face_delta);

               unsigned long face_total[3]={0};
               int THIS_NEXT_CRNUM=D.generatedrawings(paths,saveFile,canongs,face_total, mincrnum, maxcrnum);

               if(THIS_NEXT_CRNUM< NEXT_CRNUM)
                        NEXT_CRNUM=THIS_NEXT_CRNUM;

               for(int i=0;i<=delta;i++)
                        totalnum[i]+=face_total[i];

//-------DESTROY THE PATHS--------------
                for(int i=0;i<n;i++)
                {    plist::iterator pitr=paths[i]->begin();
                     for(;pitr!=paths[i]->end();pitr++)
                         delete *pitr;
                     delete paths[i];
                }
//-------DESTROY THE PATHS---------------

                LogFile<<setw(WIDTH)<<fnum<<":";
                LogFile<<setw(10)<<"("<<setw(WIDTH)<<face_total[0];
                for(int i=1;i<=delta;i++)
                    LogFile<<","<<setw(WIDTH)<<face_total[i];
                LogFile<<")";
                LogFile<<setw(20)<<face_mincrnum<<endl;
                LogFile<<"---------------------------------------------------"<<endl;
#ifdef SHOW_DETAILS
                cout<<setw(WIDTH)<<fnum<<":";
                cout<<setw(10)<<"("<<setw(WIDTH)<<face_total[0];
                for(int i=1;i<=delta;i++)
                    cout<<","<<setw(WIDTH)<<face_total[i];
                cout<<")";

                cout<<setw(20)<<face_mincrnum<<endl;
                cout<<"---------------------------------------------------"<<endl;
#endif
        }
        movetochar(GraphFile,'n');

        LogFile<<"Number of Drawings: ("<<totalnum[0]-pre_totalnum[0];
        cout<<"Number of Drawings: ("<<totalnum[0]-pre_totalnum[0];
        pre_totalnum[0]=totalnum[0];
        for(int i=1;i<=delta;i++)
        {       LogFile<<","<<totalnum[i]-pre_totalnum[i];
                cout<<","<<totalnum[i]-pre_totalnum[i];

                pre_totalnum[i]=totalnum[i];
        }
        LogFile<<")\n";
        cout<<")\n";

        LogFile<<"Total so far: ("<<totalnum[0];
        cout<<"Total so far: ("<<totalnum[0];
        for(int i=1;i<=delta;i++)
        {       LogFile<<","<<totalnum[i];
                cout<<","<<totalnum[i];
        }
        LogFile<<")\n";
        cout<<")\n";

#ifdef SKIP_SAVING
        LogFile<<"min cr(D_"<<n+2<<")="<<NEXT_CRNUM<<endl;
        cout<<"min cr(D_"<<n+2<<")="<<NEXT_CRNUM<<endl;
#endif

}
        GraphFile.close();

}

//---Record the total number of drawings -----

        for(int i=0;i<=delta;i++)
        {       saveFile[i].seekp(0,ios_base::beg);
                saveFile[i]<<"Total="<<totalnum[i]<<endl;
                saveFile[i].close();

                fclose(canongs[i]);
        }


        LogFile<<"\nTotal: ("<<totalnum[0];
        cout<<"\nTotal: ("<<totalnum[0];
        for(int i=1;i<=delta;i++)
        {       LogFile<<","<<totalnum[i];
                cout<<","<<totalnum[i];
        }
        LogFile<<"), ";
        cout<<"), ";
#ifdef SKIP_SAVING
        LogFile<<"*** cr(K_"<<n+2<<")="<<NEXT_CRNUM<<" ***"<<endl;
        cout<<"*** cr(K_"<<n+2<<")="<<NEXT_CRNUM<<" ***"<<endl;
#else
        LogFile<<"saved in file #?_"<<saveFilename<<endl;
        cout<<"saved in file #?_"<<saveFilename<<endl;
#endif

//----------------------------------------------


// EXIT
  // Calculate time used.

        showtime(LogFile,t_begin);
        showtime(cout,t_begin);
        LogFile.close();


  // Press <Enter> to exit.

        cout<<"\nComplete! See "<<logFilename.c_str()<<" for details.";

        cin.sync();
        cin.get();
        return 0;
//END EXIT
}
//---------------------------------------------------------------------------

//FUNCTION IMPLEMENTATION

void showtime(ostream& ostr, time_t t_begin)
{
        time_t t_used=time(NULL)-t_begin;
        clock_t t_used2=clock();

        ostr<<endl<<"Time used: ";
        if (t_used<1) ostr<< "<1 second";
        else
        {       ostr<<t_used<<" second";
                if(t_used!=1) ostr<<"s";
        }
        ostr<<"."<<endl;

        double secs=t_used2/CLOCKS_PER_SEC;
        ostr<<"CPU time: "<<secs<<" second";
        if(secs!=1.0) ostr<<"s";
        ostr<<"."<<endl;

}

void showfaces(ostream& LogFile, list<int>& uniquefaces, int num_each_line)
{

        LogFile<<endl<<uniquefaces.size()<<" distinct faces:"<<endl;
        list<int>::iterator itr=uniquefaces.begin();
        int maxlabel=*max_element(uniquefaces.begin(),uniquefaces.end());
        int maxw=digits(maxlabel);
        LogFile<<"{"<<setw(maxw)<<*itr;
        itr++;
        for(int i=1;itr!=uniquefaces.end();itr++,i++)
        {
               LogFile<<",";
               if(i%num_each_line==0) LogFile<<endl<<" ";
               LogFile<<setw(maxw)<<*itr;
        }
        LogFile<<"}"<<endl;

}

int getmincrnum(const char* FileName, ofstream& LogFile, int step)
{
        int min_crnum=5000;
        stringstream nextgraph;
        ifstream GraphFile;
        GraphFile.open(FileName);
        if(GraphFile.fail()) { cout<<"Can't open file!"; exit(1);}
        movetochar(GraphFile,'=');
        int total;
        GraphFile>>total;
        LogFile<<endl<<total<<" drawings to be processed:"<<endl;
        cout<<endl<<total<<" drawings to be processed:"<<endl;

        movetochar(GraphFile,'n');
        int count=0;
        while(!GraphFile.eof())
        {
                nextgraph.str("");
                int status=readnextgraph(GraphFile,nextgraph);
                if(status==1) break;
                drawing D(nextgraph);
                count++;
                if(count % step ==0)
                {       LogFile<<"Processed: "<<count<<"/"<<total<<endl;
                        cout<<"Processed: "<<count<<"/"<<total<<endl;
                }

                int cur_mincrnum = D.mincrnum();

                if(cur_mincrnum<min_crnum) min_crnum=cur_mincrnum;
        }
        GraphFile.close();
        return min_crnum;
}

int getmincrnum(const char* FileName)
{
        int min_crnum=5000;
        stringstream nextgraph;
        ifstream GraphFile;
        GraphFile.open(FileName);
        if(GraphFile.fail()) { cout<<"Can't open file!"; exit(1);}

        movetochar(GraphFile,'n');
        while(!GraphFile.eof())
        {
                nextgraph.str("");
                int status=readnextgraph(GraphFile,nextgraph);
                if(status==1) break;
                drawing D(nextgraph);

                int cur_mincrnum = D.mincrnum();

                if(cur_mincrnum<min_crnum) min_crnum=cur_mincrnum;
        }
        GraphFile.close();
        return min_crnum;
}


void regroup(string sourceFilename,int* totalnum, int delta,int mincrnum)
{

        if(mincrnum<0) mincrnum=getmincrnum(sourceFilename.c_str());

        ofstream FinalDrawings[3]; // Here we assume that delta<3.
        stringstream fstr;
        for(int i=0;i<=delta;i++)
        {       fstr.str("");
                fstr<<"#"<<i<<"_";
                fstr<<sourceFilename;
                FinalDrawings[i].open(fstr.str().c_str());
                FinalDrawings[i]<<string(30,' ')<<endl;

                totalnum[i]=0;
        }


        ifstream Drawings;
        Drawings.open(sourceFilename.c_str());

        movetochar(Drawings,'n');

        while(!Drawings.eof())
        {
                int n,c,f;

                movetochar(Drawings,'=');

                if(Drawings.eof()) { Drawings.close(); break;}

                Drawings>>n;
                movetochar(Drawings,'=');

                if(Drawings.eof()) { Drawings.close(); break;}

                Drawings>>c;
                int real_delta=c-mincrnum;
                if(real_delta<=delta)
                {
                        totalnum[real_delta]++;

                        FinalDrawings[real_delta]<<"n="<<n<<",c="<<c;
                        while((Drawings.peek()!='n')&&!Drawings.eof())
                        {       char c=Drawings.get();
                                FinalDrawings[real_delta].put(c);
                        }
                }
                else
                        movetochar(Drawings,'n');
        }
        Drawings.close();

}

int digits(int a)
{
        int temp;
        if(a<0) temp=-a;
        else temp=a;
        int d=1;
        while(temp>=10)
        {     temp/=10;
              d++;
        }
        return d;
}
//--

//END FUNCTION IMPLEMENTATION



