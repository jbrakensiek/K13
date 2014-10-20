//VERTEX, EDGE, dgraph, Drawing-----------------------------------------------
//I AM GOING TO MAKE VERY VECTOR ONLY CONTAINING POINTERS.

//---------------------------------------------------------------------------

#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <functional>
#include <fstream>

#define MAXN 300
#include "nauty.h"

//#define MAX_DEGREE  30
#define MAX_GRAPH_SIZE 300
#define MAX_NONCROSSING_SIZE 15

using namespace std;
using namespace std::rel_ops;

// Type Definitions
class vertex;
class dgraph;
class chain;
class drawing;

typedef chain fpath;
typedef vector<vertex *> vlist;
typedef pair<vertex*, vertex*> edge;
typedef vector<edge> elist;
typedef vertex face;
typedef  vlist flist;
typedef list<fpath*> plist;
typedef vector<plist*> pathset;
typedef list<vertex*> vchain;
typedef list<vchain* > vllist;

//FUNCTION DECLARATION
ostream& operator<<(ostream&, const edge&);
ostream& operator<<(ostream& , const vertex&);
ostream& operator<<(ostream& , fpath&);

bool operator==(const vertex&, const vertex&);
bool operator==(fpath&, fpath&);
int str2num(string&);
void num2str(int, char*);
pair<int,int> str2pair(string&);
int readnextgraph(istream&, stringstream&, int);
void movetochar(istream&,char);
void moveb4char(istream&,char);
int tlength(plist&);
int isomorph(FILE *, graph*, int, int);
void showgraph(graph* g, int m, int n);

// VERTEX DECLARATION --------------------------------------------------------
// A vertex has a label and a list of pointers to vertices, known as neighbors.

class vertex {

public:

//Constructors
        vertex();
        vertex(int);
        vertex(int, const vlist&);

//Destructor
        ~vertex();

//Element Access
        int getlabel() const;
        int getweight() const;
        vlist& getneighbors();
        edge getdualedge(int pos) const;
        int degree() const;
        bool isneighbor(vertex*) const;
	ostream &print(ostream &) const;
        ostream &showall(ostream&) const;
        void getfaces(vlist&) const;

        int whereis(vertex*) const;
        int whereis(vertex*,edge) const;
        int where1face(vertex*,face*) const;
        int where2face(vertex*,face*) const;

        const vertex* operator[](int)const;

//Modifiers
        vertex*& operator[](int);

        void setlabel(int);
        void setweight(int);
        void setneighbors(const vlist&);
        void setdualedge(edge e, int pos);
        void setdualedge(edge e);

        void setfaces(const flist&);
        void delneighbor(int);
        void addneighbor(vertex*, int);
        void addneighbor(vertex*);
        void addneighbor(vertex*, int,  edge);
        void addneighbor(vertex*, edge);
        void setneighbor(vertex*,int);
        void setneighbor(vertex*,edge,int);

 protected:


        //Copying vertex not allowed becuase of problem of linking.
        vertex(const vertex& vertex){}
        void operator=(const vertex&){}

        int label;
        int weight;
        vlist neighbors;
        //The reason to choose an array of pointers to vertex is:
        //1. vector instead of list, for random access.
        //2. If we chose vector<vertex>, there would be trouble deriving
        //   a class dvertex s.t. dvertex contains a member vector<pvertex>.
        //3. Using vector<vertex&> causes an error, which is not clear to me
        //   at this time.

        elist dualedges;

};

// END VERTEX DECLARATION --------------------------------------------------


// GRAPH DECLARATION --------------------------------------------------------
// A graph is an ordered list of vertices.
// There is no edge defined in a graph, since the information of edges is
// implied in the vertices.

class dgraph{
public:

// Constructors
        dgraph();
        dgraph(istream&);
        dgraph(const dgraph&); // Copy Constructor

//Destructor
        ~dgraph();

// Element Access
        const vertex* operator[](int)const;
        const vertex* operator()(int) const;

        int nsize() const;
        int csize() const;
        int tsize() const;
        ostream& print(ostream&,bool) const;
        ostream& print_bylabel(ostream&,bool) const;
        ostream& showall(ostream&,bool) const;
        ostream& showall_bylabel(ostream&,bool) const;
        void save2file(const char*) const;
        int whereis(int) const;
        int nonwhereis(vertex*) const;
        int nonwhereis(int) const;
        int maxlabel() const;

//SPECIAL Memebers
        int distance(vertex*, vertex*);
        int distance(int,int);

        void distances(int, vlist&, vector<int>&);
        void distances(vertex*, vlist&, vector<int>&);
//        void distances(int, vector<int>&, vector<int>&);

        edge belong2(const edge&);

//Modifiers
        vertex*& operator[](int);
        vertex*& operator()(int);
        void addnewvertex(int*, int*, bool);
        void addnewvertex(vertex*, bool);
//        void delvertex(int, bool);
        vlist* getvertices();
        vlist* getnoncross();
        void relabel();
        void toggle(int);

        void setweights(int);

        void operator=(const dgraph&); // Copy by assignment
        void setconnection(istream&);
        void setconnection(istream&,vlist&);
        void nautilize(graph*);
        void getorbits(int*);

private:
        void init(istream&);
        vlist vertices;
        vlist noncrossings;
};

//END dgraph DECLARATION ----------------------------------------------------


// DRAWING DECLARATION
class drawing{
public:

//Constructors
        drawing(){};
        drawing(const drawing&);
        drawing(istream&);
        ~drawing();


//Element Access
        ostream& showall(ostream&) const;
        ostream& showall_bylabel(ostream&) const;
        dgraph& getprimal();
        dgraph& getdual();

        int num_noncr() const;
        int num_cross() const;
        int num_face() const;

        const dgraph& getprimal() const;
        const dgraph& getdual() const;

        void save2file(const char* FileName, bool) const;

//Modifiers
        void operator=(const drawing&);

        vertex* splitedge(vertex*,vertex*,edge,bool);
        vertex* splitedge(int,int,int,int,bool);

        vertex* splitedge(vertex*,vertex*,bool);
        vertex* splitedge(int,int,bool);
        
        void connect2v(vertex*, vertex*, face*);
        void connect2v(vertex*, vertex*);
        void newvinface(vertex*, face*, vertex*);
//        void newvinface(int,int,int);
        void connect2v(int, int, int);
        void connect2v(int, int);
        void relabel();

//SPECIAL MEMBERS
        void addnewvertex(bool crossing);
        int relposition(fpath&,fpath&, face*);
        int relposition(fpath&,fpath&, int);

        int outatright(plist&,fpath&, face*);
        int outatright(plist&,fpath&, int);

        void cutbypaths(plist&,vllist&);
        void attachpaths(plist&);

        void translate(fpath&,drawing&);
        void translate(plist&,drawing&);

        int findpaths(face*, vertex*, plist&, int);

        int generatepaths(face*, pathset&, int);
        int generatepaths(int, pathset&, int);

        int generatedrawings(pathset&, ofstream*,FILE**,unsigned long*, int, int);

        bool valid(fpath&);
//        int shrink(list<fpath> &);
        bool forced2cross(fpath&,fpath&);
        bool forced2cross(plist&);

        int forcedcrossings();
        int forcedcrossings(list<int>&);
        int forcedcrossings(list<int>&, list<int>&);

        int mincrnum();
        void getuniquefaces(list<int>&);
        
private:
        void init(istream&);
        void init2(istream&);
        void generatedual();
        dgraph primal;
        dgraph dual;
};

// END DRAWING DECLARATION

class chain{
public:
//Constructors
        chain();
//Destructor
        ~chain();

//Access
        unsigned size() const;
        unsigned length() const;
        const vertex* operator[](int) const;
        list<vertex*>::iterator whereis(vertex*);
        list<vertex*>::iterator head();
        list<vertex*>::iterator tail();
        vertex* back();

//Modifers
        vertex* &operator[](int);
        void extend2(vertex*);
        void popback();
        void setendpoint(vertex*);

#ifdef INC_GRAPH
   //For face path only
        void extend2(drawing&,int);
        void setendpoint(drawing&,int);
#endif

//Data Members

        list<vertex*> beads;
        vertex* endpoint;
        int delta; //Extra Infomation
        bool destroy;
};

// END CHAIN DECLARATION


//CHAIN IMPLEMENTATION

//Constructors
chain::chain(): destroy(false){}


chain::~chain()
{       if(destroy)
        { for(list<vertex*>::iterator itr=beads.begin();itr!=beads.end();itr++)
              delete *itr;
          delete endpoint;
        }
}


//Access

unsigned chain::size() const{ return beads.size();}


unsigned chain::length() const
{ if(beads.size()==0) return 0;
  else return beads.size()-1;
}


const vertex* chain::operator[](int i) const
{       list<vertex*>::const_iterator itr=beads.begin();
        for(unsigned j=0;j<beads.size();j++,itr++)
           if(itr==beads.end()) break;
        return *itr;
}


list<vertex*>::iterator chain::whereis(vertex* f)
{       list<vertex*>::iterator itr=beads.begin();
        for(unsigned i=0;i<beads.size();i++, itr++)
             if(*itr==f) break;
        return itr;
}


list<vertex*>::iterator chain::head()
{       return beads.begin();}



list<vertex*>::iterator chain::tail()
{
        return beads.end(); }



vertex* chain::back()
{       return beads.back();
}

//Modifers


vertex* &chain::operator[](int i)
{       list<vertex*>::iterator itr=beads.begin();
        for(int j=0;j<i;j++)
        {    itr++;
             if(itr==beads.end()) break;
        }
        return *itr;
}


void chain::extend2(vertex* nextf)
{
beads.push_back(nextf);
}

#ifdef INC_GRAPH


void chain::extend2(drawing& D,int n)
{
     extend2(D.getdual()[n-1]);
}


void chain::setendpoint(drawing& D,int v)
{       endpoint=D.getprimal()[v-1];
}
#endif



void chain::popback()
{       beads.pop_back();
}
//END CHAIN IMPLEMENTATION

//VERTEX IMPLEMENTATION  ---------------------------------------------------

//Constructors

vertex::vertex():weight(0)
{
#ifdef MAX_DEGREE
        neighbors.reserve(MAX_DEGREE);
        dualedges.reserve(MAX_DEGREE);
#endif
}

vertex::vertex(int vlabel):label(vlabel),weight(0)
{
#ifdef MAX_DEGREE
       neighbors.reserve(MAX_DEGREE);
       dualedges.reserve(MAX_DEGREE);
#endif
}

vertex::vertex(int vlabel, const vlist& vL)
       :label(vlabel),weight(0),neighbors(vL)
{
#ifdef MAX_DEGREE
       neighbors.reserve(MAX_DEGREE);
       dualedges.reserve(MAX_DEGREE);
#endif
}

//Destructor
vertex::~vertex(){}

//Element Access

int vertex::getlabel() const { return label;}

int vertex::getweight() const{return weight;}

vlist& vertex::getneighbors(){return neighbors;}

edge vertex::getdualedge(int pos) const
{       pos=pos % degree();
        if((static_cast<unsigned>(pos)<dualedges.size()))
                return dualedges[pos];
        else return edge(NULL,NULL);
}


int vertex::degree() const {return neighbors.size();}


// Determine if a vertex is logically  a neighbor.
bool vertex::isneighbor(vertex* pv) const
{
        return whereis(pv)!=-1;
}

ostream &vertex::print(ostream & os=cout) const
{       os<<label<<": ";
        vlist::const_iterator itr;
        for(itr=neighbors.begin();itr!=neighbors.end();itr++)
                os<<" "<<(*itr)->getlabel();
        os<<";";
        return os;
}

ostream &vertex::showall(ostream& os=cout) const
{       os<<label<<": ";
        for(unsigned pos=0;pos!=neighbors.size();pos++)
        {       os<<" "<<*neighbors[pos];
                if(pos<dualedges.size())
                   os<<" "<<dualedges[pos];
        }
        os<<";";
        return os;
}

void vertex::getfaces(vlist& vL) const
{       vL.clear();

        for(int i=0;i<degree();i++)
        {    if ((dualedges[i].first==NULL)||(dualedges[i].second==NULL))
                   break;
             vL.push_back(dualedges[i].first);
        }
}



// Return the (first) position of a neighbor.
// Return -1 if not found.
int vertex::whereis(vertex *pv) const
{       vlist::const_iterator itr;
        int pos;
        for(itr=neighbors.begin(),pos=0;itr!=neighbors.end();itr++,pos++)
                if(*itr==pv) return pos;
        return -1;
}

int vertex::whereis(vertex* neighbor,edge dual) const
{
        int pos;
        for(pos=0;pos<degree();pos++)
        {       if(neighbors[pos]==neighbor && dualedges[pos]==dual)
                        return pos;
        }
        return -1;
}


int vertex::where1face(vertex* neighbor,face* firstf) const
{       int pos;
        for(pos=0;pos<degree();pos++)
              if(neighbors[pos]==neighbor && dualedges[pos].first==firstf)
                     return pos;
        return -1;
}


int vertex::where2face(vertex* neighbor,face* secondf) const
{       int pos;
        for(pos=0;pos<degree();pos++)
              if(neighbors[pos]==neighbor && dualedges[pos].second==secondf)
                     return pos;
        return -1;
}

const vertex* vertex::operator[](int index)const
{       return neighbors[index];
}


//Modifiers

vertex*& vertex::operator[](int index)
{       return neighbors[index];
}

void vertex::setlabel(int newlabel){label=newlabel;}

void vertex::setweight(int newweight){weight=newweight;}

void vertex::setneighbors(const vlist& newneighbors)
{    neighbors=newneighbors;
}


void vertex::setdualedge(edge e, int pos)
{       pos=pos % degree();
        dualedges[pos]=e;

}


void vertex::setdualedge(edge e)
{
       setdualedge(e, degree()-1);
}

void vertex::setfaces(const flist& fL)
{       if((fL.size()>=neighbors.size())&&(fL.size()!=0))
        {       dualedges.clear();
                dualedges.push_back(edge(fL[neighbors.size()-1],fL[0]));
                for(unsigned pos=1;pos<neighbors.size();pos++)
                        dualedges.push_back(edge(fL[pos-1],fL[pos]));
        }
}

// Delete a neighbor. If the vertex to be deleted is not a neighbor,
// nothing is done.
// This operation is designed for graphs, not drawings. Deleting a vertex
// in a drawing is more complicated.

void vertex::delneighbor(int pos)
{       pos=pos % degree();
        neighbors.erase(neighbors.begin()+pos);
        if(static_cast<unsigned>(pos)<dualedges.size())
                dualedges.erase(dualedges.begin()+pos);
}

// Add a neighbor AFTER a given position
//NOTE: This is deigned for only graphs, not drawings.
// This method used in a drawing will mess the correspondence of dual edges
void vertex::addneighbor(vertex* pv, int pos)
{

        addneighbor(pv,pos,edge(NULL,NULL));
}

void vertex::addneighbor(vertex* pv, int pos, edge e)
{
        int forinsert= pos % degree();

        neighbors.insert(neighbors.begin()+forinsert+1,pv);
        dualedges.insert(dualedges.begin()+forinsert+1,e);
}

// only in graphs, recommended for graphs
void vertex::addneighbor(vertex* pv)
{
        neighbors.push_back(pv);
        dualedges.push_back(edge(NULL,NULL));
}

void vertex::addneighbor(vertex*pv,edge e)
{
        addneighbor(pv); //This already adds a default edge, so can't use
                         // insertdualedge next.
        setdualedge(e);

}

void vertex::setneighbor(vertex* newneighbor,int pos)
{       pos=pos% degree();

        neighbors[pos]=newneighbor;
}
void vertex::setneighbor(vertex* newneighbor,edge newdualedge,int pos)
{       pos=pos% degree();
        neighbors[pos]=newneighbor;
        setdualedge(newdualedge,pos);
}

//END VERTEX IMPLEMENTATION-------------------------------------------------


//GRAPH IMPLEMENTATION -----------------------------------------------------

//Constructors
dgraph::dgraph(){
vertices.reserve(MAX_GRAPH_SIZE);
noncrossings.reserve(MAX_NONCROSSING_SIZE);
}

//Construct a graph from an input stream. The graph is stored in the following
// format:
//n=3,c=0
//3: 1 2
//1: 2 3
//2: 3 1
// where each vertex is represented by an integer. The number followed
// immediately by ':' is the vertex, and all the numbers between this vertex
// and next vertex are neighbors of this vertex.
dgraph::dgraph(istream& ingraph)
{
        vertices.reserve(MAX_GRAPH_SIZE);
        noncrossings.reserve(MAX_NONCROSSING_SIZE);
        init(ingraph);
}

void dgraph::init(istream& ingraph)
{

        vertices.clear();
        noncrossings.clear();

    // create vertices and insert it into the vertex list.
        int n,c;

        ingraph.seekg(ios_base::beg); // Set pointer to front.

        movetochar(ingraph,'='); ingraph>>n;
        movetochar(ingraph,'='); ingraph>>c;

        for(int i=0;i<n+c;i++)
        {      vertex* pv=new vertex(i+1);

               if(pv==NULL)
                {     cout<<"Failed to generate vertex "<<i+1;
                      exit(EXIT_FAILURE);
                }


               vertices.push_back(pv);
        }

        noncrossings.resize(n); // Then n is available from the graph
        setconnection(ingraph);

}
dgraph::dgraph(const dgraph& primal)
{
        vertices.reserve(MAX_GRAPH_SIZE);
        noncrossings.reserve(MAX_NONCROSSING_SIZE);
        operator=(primal);
}

//Destructor

// Note that pointers to vertices are not available outside class graph.
dgraph::~dgraph()

{       for(unsigned pos=0;pos<vertices.size();pos++)
            if(vertices[pos]!=NULL)     delete vertices[pos];

}

//Element Access
const vertex* dgraph::operator[](int index) const
{ return vertices[index];}

const vertex* dgraph::operator()(int index) const
{ return noncrossings[index];}

//Return number of non-crossings
int dgraph::nsize() const {return noncrossings.size();}

//Return number of crossings
int dgraph::csize() const {return vertices.size()-noncrossings.size();}

//Return total size
int dgraph::tsize() const { return vertices.size();}

// Print the graph,
// (Need to use relabel() before saving it to a file.
ostream& dgraph::print(ostream& os=cout,bool title=1) const
{
        if(title==1)
                os<<"n="<<nsize()<<",c="<<csize()<<endl;

        for(unsigned pos=0;pos<noncrossings.size();pos++)
            noncrossings[pos]->print(os)<<endl;

        for(unsigned pos=0;pos<vertices.size();pos++)
        {   int i=nonwhereis(vertices[pos]);
            if(i==-1)
                vertices[pos]->print(os)<<endl;
        }
        return os;

}

ostream& dgraph::print_bylabel(ostream& os=cout,bool title=1) const
{
        if(title==1)
                os<<"n="<<nsize()<<",c="<<csize()<<endl;

        for(unsigned pos=0;pos<vertices.size();pos++)
                vertices[pos]->print(os)<<endl;

        return os;

}


// Show all the details of the graph
ostream& dgraph::showall(ostream& os=cout,bool title=1) const
{
        if(title)
                os<<"n="<<nsize()<<",c="<<csize()<<endl;
        for(unsigned pos=0;pos<noncrossings.size();pos++)
            noncrossings[pos]->showall(os)<<endl;

        for(unsigned pos=0;pos<vertices.size();pos++)
        {   int i=nonwhereis(vertices[pos]);
            if(i==-1)
                vertices[pos]->showall(os)<<endl;
        }
        return os;
}

ostream& dgraph::showall_bylabel(ostream& os=cout,bool title=1) const
{
        if(title)
                os<<"n="<<nsize()<<",c="<<csize()<<endl;

        for(unsigned pos=0;pos<vertices.size();pos++)
                vertices[pos]->showall(os)<<endl;
        return os;
}

void dgraph::save2file(const char* FileName) const
{
        ofstream fstr(FileName, ios_base::app);
        fstr<<endl;
        print(fstr);
        fstr.close();
}
// Return the position of a vertex, given the label.
// Return -1 if not found.
int dgraph::whereis(int v) const
{       unsigned pos;
        for(pos=0;pos<vertices.size();pos++)
                if(vertices[pos]->getlabel()==v) return pos;
        return -1;
}

// Return the position of a noncrossing vertex, given the label.
// Return -1 if not a noncrossing.
int dgraph::nonwhereis(vertex* v) const
{       unsigned pos;
        for(pos=0;pos<noncrossings.size();pos++)
                if(noncrossings[pos]==v) return pos;
        return -1;

}


int dgraph::nonwhereis(int v) const
{
        return nonwhereis(vertices[v-1]);
}


int dgraph::maxlabel() const
{
        int max=0;
        if(vertices.size()>0)
        {       vlist::const_iterator itr=vertices.begin();
                for(;itr!=vertices.end();itr++)
                    if ((*itr)->getlabel()>max)
                        max=(*itr)->getlabel();
        }
        return max;
}


int dgraph::distance(vertex* v1, vertex* v2)
{
        if(v1==v2) return 0;
        setweights(-1);
        v1->setweight(0);

        list<vertex*> weighted;
        weighted.push_back(v1);

        while(weighted.size()!=0)
        {       list<vertex*>::iterator itr=weighted.begin();
                for(;itr!=weighted.end();itr++)
                {   vertex* vcheck=*itr;
                    vlist* curneighs=&(vcheck->getneighbors());
                    vlist::iterator vitr=curneighs->begin();
                    for(;vitr!=curneighs->end();vitr++)
                    {   vertex* curv=*vitr;
                        if(curv==v2) return vcheck->getweight()+1;
                        else if(curv->getweight()==-1)
                        {       curv->setweight(vcheck->getweight()+1);
                                weighted.push_back(curv);
                        }
                    }
                    weighted.erase(itr);
                    itr--;
                }
        }
        return -1;
}


int dgraph::distance(int s,int t)
{        return distance(vertices[s-1],vertices[t-1]);
}


// We could use the following codes:
// for(unsinged i=0;i<vL.size();i++)
//     dists.push_back(distances(v1,vL[i]));
// However, this will possibly cost more time.
void dgraph::distances(int v1, vlist& vL, vector<int>& dists)
{     distances(vertices[v1-1],vL,dists);
}

void dgraph::distances(vertex* v1, vlist& vL, vector<int>& dists)
{

        int vnum=vL.size();
        dists.resize(vnum,-1);
        setweights(-1);
        v1->setweight(0);
        int counted=0;
        int pos=0;
        for(vlist::iterator itr=vL.begin();itr!=vL.end();itr++,pos++)
            if(v1==*itr)
            {   dists[pos]=0;
                counted++;
                if(counted==vnum) return;
                break;
            }

        list<vertex*> weighted;
        weighted.push_back(v1);

        while(weighted.size()!=0)
        {       list<vertex*>::iterator itr=weighted.begin();
                for(;itr!=weighted.end();itr++)
                {   vertex* vcheck=*itr;
                    vlist* curneighs=&(vcheck->getneighbors());
                    vlist::iterator vitr=curneighs->begin();
                    for(;vitr!=curneighs->end();vitr++)
                    {   vertex* curv=*vitr;
                        if(curv->getweight()==-1)
                        {  curv->setweight(vcheck->getweight()+1);
                           vlist::iterator itr_ends=vL.begin();
                           int pos=0;
                           for(;itr_ends!=vL.end();itr_ends++,pos++)
                              if(curv==*itr_ends)
                              {  dists[pos]=curv->getweight();
                                 counted++;
                                 if(counted==vnum) return;
                                 break;
                              }
                           weighted.push_back(curv);
                        }
                    }
                    weighted.erase(itr);
                    itr--;
                }
        }
}

/*
void dgraph::distances(int v1, vector<int>& intL, vector<int>& dists)
{       vlist vL;
        vL.reserve(MAX_GRAPH_SIZE);

        for(unsigned i=0;i<intL.size();i++)
            vL.push_back(vertices[intL[i]-1]);
        distances(vertices[v1-1],vL,dists);
}
*/


edge dgraph::belong2(const edge& segment)
{       vertex * first=segment.first;
        vertex * second=segment.second;

        vertex* last=second;
        while(nonwhereis(first)==-1)
        {       int d= first->degree();
                if(d!=4) return edge();

                int pos=first->whereis(last);
                pos=(pos+2)% d;
                last =first;
                first=(*last)[pos];
        }

        last=segment.first;
        while(nonwhereis(second)==-1)
        {       int d= second->degree();
                if(d!=4) return edge();

                int pos=second->whereis(last);
                pos=(pos+2)% d;
                last =second;
                second=(*last)[pos];
        }

        return edge(first,second);
}


//Modifiers
vertex*& dgraph::operator[](int index){return vertices[index];}

vertex*& dgraph::operator()(int index)
{ return noncrossings[index];}

//Add a new vertex with label = maxlabel+1.
void dgraph::addnewvertex(int* begin, int* end, bool crossing=false)
{
       vertex* newv=new vertex(maxlabel()+1);

               if(newv==NULL)
                {     cout<<"Failed to generate vertex "<<maxlabel()+1;
                      exit(EXIT_FAILURE);
                }


       for(int* i=begin; i!=end; i++)
       {    int pos=whereis(*i);
            if(pos!=-1)
            {   newv->addneighbor(vertices[pos]);
                vertices[pos]->addneighbor(newv);
            }
       }
       vertices.push_back(newv);
       if(!crossing) noncrossings.push_back(newv);
}

void dgraph::addnewvertex(vertex* pv, bool crossing=false)
{       vertices.push_back(pv);
        if(!crossing) noncrossings.push_back(pv);
}
/*
//Delete the vertex with a given label.
void dgraph::delvertex(int v, bool modifyothers=true)
{       int pos=whereis(v);
        if(pos==-1) return;
        vertex* temp=vertices[pos];
        vertices.erase(vertices.begin()+pos);

        vlist::iterator itr=noncrossings.begin();
        for(;itr!=noncrossings.end();itr++)
             if((*itr)->getlabel()==v)
             {   noncrossings.erase(itr);
                 break;
             }

     // Update neighbor information of v's neighbors
     if(modifyothers)
     {
        vlist vL=temp->getneighbors();
        for(unsigned npos=0;npos<vL.size();npos++)
        {       vlist nneighbor=vL[npos]->getneighbors();
                for(unsigned nnpos=0;nnpos<nneighbor.size();nnpos++)
                   if(nneighbor[nnpos]==temp)
                        vL[npos]->delneighbor(nnpos);
        }
      }
        delete temp;
}
*/

vlist* dgraph::getvertices()
{      return &vertices;
}

vlist* dgraph::getnoncross()
{      return &noncrossings;
}


void dgraph::relabel()
{       unsigned pos;
        for(pos=0;pos<vertices.size();pos++)
                vertices[pos]->setlabel(pos+1);
}


void dgraph::toggle(int v)
{
        vertex* pv=vertices[whereis(v)];
        vlist::iterator itr=noncrossings.begin();
        for(;itr!=noncrossings.end();itr++)
           if(*itr==pv)
           {    noncrossings.erase(itr);
                return;
           }
        noncrossings.push_back(pv);

}

void dgraph::setweights(int w)
{       for(int i=0;i<tsize();i++)
              vertices[i]->setweight(w);
}

// Copy by assignment
void dgraph::operator=(const dgraph& D)
{

        this->~dgraph();
        stringstream temp;
        D.showall(temp);

        init(temp);
}

void dgraph::setconnection(istream& ingraph)
{
    // set neighbors of each vertex
        int lastvertex;
        int nextneighbor;
        string nextdata;

        int count=-1;
        int n=nsize();
        while(!ingraph.eof())
        {      nextdata=""; //for skipping white spaces at the end of file
               ingraph>>nextdata;
               char tail=nextdata[nextdata.size()-1];
               if(nextdata==""||tail=='-') break;
               nextneighbor=str2num(nextdata);
               switch(tail)
               {    case ':': { count++;
                                lastvertex=nextneighbor;
                                if(count<n)
                                     noncrossings[count]=vertices[lastvertex-1];
                                break;
                              }
//                    case ')': break;
                     default:
                              vertices[lastvertex-1]
                                 ->addneighbor(vertices[nextneighbor-1]);
               }
        }

}


void dgraph::setconnection(istream& ingraph,vlist& dualvertices)
{
    // set neighbors of each vertex
        int lastvertex;
        int nextneighbor;
        string nextdata;

        int count=-1;
        int n=nsize();
        while(!ingraph.eof())
        {
               nextdata=""; //Skip white spaces at the end of file
               ingraph>>nextdata;

               if(nextdata==""||nextdata[nextdata.size()-1]=='-') break;
               nextneighbor=str2num(nextdata);
               char tail=nextdata[nextdata.size()-1];
               if(tail==EOF) break;
               switch(tail)
               {    case ':': { lastvertex=nextneighbor;
                                count++;
                                if(count<n)
                                     noncrossings[count]=vertices[lastvertex-1];
                                break;
                              }
                     case ')':
                    case ';' : {
                                pair<int,int> duale=str2pair(nextdata);
                                int vpos1=duale.first;
                                int vpos2=duale.second;
                                if((vpos1<1)||(vpos2<1)) {cout<<"reading <1 labels"; cin.get();}
                                edge e=make_pair(dualvertices[vpos1-1],dualvertices[vpos2-1]);
                                if(lastvertex<1) {cout<<"reading lastvertex<1"; cin.get();}
                                if(vertices[lastvertex-1]->degree()==0) {cout<<"reading error!";cin.get();}
                                vertices[lastvertex-1]->setdualedge(e);
                                break;}
                     default:
                              vertices[lastvertex-1]
                                 ->addneighbor(vertices[nextneighbor-1]);
               }
       }

}

void dgraph::nautilize(graph* g)
{
        int n=tsize();
        int m=(n + WORDSIZE - 1) / WORDSIZE;
        nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

        set* gv;
        for (int v = 0; v < n; v++)
        {
                face* curv=vertices[v];
                int curlabel=curv->getlabel();
                gv= GRAPHROW(g,curlabel-1,m);
                EMPTYSET(gv,m);
                for(int neigh=0;neigh< curv->degree();neigh++)
                {   int neighlab=(*curv)[neigh]->getlabel();
                    ADDELEMENT(gv,neighlab-1);
                }
        }

}

void dgraph::getorbits(int* orbits)
{
        graph g[MAXN*MAXM];
        int lab[MAXN],ptn[MAXN];
        static DEFAULTOPTIONS_GRAPH(options);
        statsblk(stats);
        setword workspace[50*MAXM];

        int n=tsize();
        int m=(n + WORDSIZE - 1) / WORDSIZE;
        nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

        nautilize(g);

        nauty(g,lab,ptn,NULL,orbits,&options,&stats,
                             workspace,50*MAXM,m,n,NULL);
}

//END GRAPH IMPLEMENTATION--------------------------------------------------

// DRAWING IMPLEMENTATION

//drawing::drawing(){}

drawing::drawing(const drawing& D) {

operator=(D);

}

void drawing::init2(istream& indrawing)
{

    // create vertices and insert it into the vertex list.
        int n,c;
        indrawing.seekg(ios_base::beg); // Set pointer to front.

        movetochar(indrawing,'='); if(indrawing.eof()) return;
        indrawing>>n;
        movetochar(indrawing,'='); if(indrawing.eof()) return;
        indrawing>>c;
////

        vlist *vertices=primal.getvertices();
        vlist *noncrossings=primal.getnoncross();
        vlist *dvertices=dual.getvertices();

        vertices->clear();
        noncrossings->clear();
        dvertices->clear();

        for(int i=0;i<n+c;i++)
        {       vertex* pv=new vertex(i+1);

                if(pv==NULL)
                {     cout<<"Failed to generate vertex "<<i+1;
                      exit(EXIT_FAILURE);
                }

                vertices->push_back(pv);
        }
        noncrossings->resize(n);
        primal.setconnection(indrawing);
        generatedual();

}

void drawing::generatedual()
{       for(int i=0;i<primal.tsize();i++)
        {    vertex* curv=primal[i];
             for(int neigh=0;neigh<curv->degree(); neigh++)
             {   vertex* v1= curv;
                 vertex* v2=(*curv)[neigh];
                 int pos=v1->whereis(v2);
                 edge cure=v1->getdualedge(pos);
                 if(cure.second!=NULL) continue;
                 else
                 {   face* newf=new vertex(dual.maxlabel()+1);
                     dual.addnewvertex(newf);

                     do
                     {
                        cure.second=newf;
                        newf->addneighbor(cure.first,edge(v1,v2));
                        if(cure.first!=NULL)
                        {
                                int fpos=cure.first->whereis(NULL,edge(v2,v1));
                                cure.first->setneighbor(newf,fpos);
                        }
                        v1->setdualedge(cure,pos);
                        pos=v2->whereis(v1);
                        v2->setdualedge(edge(cure.second,cure.first),pos);
                        pos--;
                        if(pos<0) pos+=v2->degree();
                        v1=v2;
                        v2=(*v1)[pos];
                        cure=v1->getdualedge(pos);
                     }   while(v1!=curv);
                 }
             }
        }


}


void drawing::init(istream& indrawing)
{

    // create vertices and insert it into the vertex list.
        int n,c,f;
        indrawing.seekg(ios_base::beg); // Set pointer to front.

        movetochar(indrawing,'='); if(indrawing.eof()) return;
        indrawing>>n;
        movetochar(indrawing,'='); if(indrawing.eof()) return;
        indrawing>>c;
        movetochar(indrawing,'='); if(indrawing.eof()) return;
        indrawing>>f;
////

        vlist *vertices=primal.getvertices();
        vlist *noncrossings=primal.getnoncross();
        vlist *dvertices=dual.getvertices();

        vertices->clear();
        noncrossings->clear();
        dvertices->clear();

        for(int i=0;i<n+c;i++)
        {       vertex* pv=new vertex(i+1);

                if(pv==NULL)
                {     cout<<"Failed to generate vertex "<<i+1;
                      exit(EXIT_FAILURE);
                }

                vertices->push_back(pv);
        }
        noncrossings->resize(n);
//        primal.setvertices(vertices,noncrossings);

        for(int i=0;i<f;i++)
        {       vertex* pv=new vertex(i+1);

                if(pv==NULL)
                {     cout<<"Failed to generate vertex "<<i+1;
                      exit(EXIT_FAILURE);
                }

                dvertices->push_back(pv);
        }
//        dual.setvertices(dvertices);

        primal.setconnection(indrawing,*dvertices);

        dual.setconnection(indrawing,*vertices);


}
drawing::drawing(istream& indrawing)
{       init2(indrawing);
}


drawing::~drawing()
{}

ostream& drawing::showall(ostream& os=cout) const
{       os<<"n="<<primal.nsize();
        os<<",c="<<primal.csize();
        os<<",f="<<dual.tsize()<<endl;
        primal.showall(os,0)<<"---"<<endl;
        dual.showall_bylabel(os,0);
        return os;
}

ostream& drawing::showall_bylabel(ostream& os=cout) const
{       os<<"n="<<primal.nsize();
        os<<",c="<<primal.csize();
        os<<",f="<<dual.tsize()<<endl;
        primal.showall_bylabel(os,0)<<"---"<<endl;
        dual.showall_bylabel(os,0);
        return os;
}


dgraph& drawing::getprimal() { return primal;}
dgraph& drawing::getdual() {return dual;}


int drawing::num_noncr() const {return primal.nsize();}
int drawing::num_cross() const {return primal.csize();}
int drawing::num_face() const {return dual.tsize();}


const dgraph& drawing::getprimal() const { return primal;}
const dgraph& drawing::getdual() const {return dual;}

void drawing::save2file(const char* FileName, bool appen=false) const
{
        ofstream fstr;
        if(appen) fstr.open(FileName, ios_base::app);
        else fstr.open(FileName, ios_base::trunc);
        fstr<<endl;
        showall(fstr);
        fstr<<endl<<"--- ";
        fstr.close();
}

//Modifiers

void drawing::operator=(const drawing& D)
{

        stringstream temp;
        D.showall(temp);
        init(temp);
}


// Split the edge v1v2 in primal graph, where the dual edge is duale.
vertex* drawing::splitedge(vertex* v1,vertex* v2,edge duale,bool crossing=true)
{
       edge dual2=edge(duale.second,duale.first);

    // Create a new vertex and add it to the primal graph.
       vertex* newv=new vertex(primal.maxlabel()+1);

       if(newv==NULL)
        {     cout<<"Failed to generate vertex "<<primal.maxlabel()+1;
              exit(EXIT_FAILURE);
        }

       newv->addneighbor(v1,dual2);
       newv->addneighbor(v2,duale);
       primal.addnewvertex(newv,crossing);
//       if(!crossing) primal.toggle(newv->getlabel());

    // Reset neighbors of v1
       int oldpos=v1->whereis(v2,duale);
       if(oldpos==-1) return NULL;
       v1->setneighbor(newv,oldpos);


   // Reset neighbors of v2
       oldpos=v2->whereis(v1,dual2);
       if(oldpos==-1) return NULL;
       v2->setneighbor(newv,oldpos);

   // Reset neighbors of the two faces which share edge v1v2.
       face* f1=duale.first;
       face* f2=duale.second;

       oldpos= f1->whereis(f2,edge(v2,v1));
       if(oldpos<0) { cout<<"oldpos="<<oldpos<<"in splitedge" ; cin.get();}
       f1->setdualedge(edge(v2,newv),oldpos);

       f1->addneighbor(f2,oldpos,edge(newv,v1));

       oldpos= f2->whereis(f1,edge(v1,v2));
       if(oldpos<0) {cout<<"oldpos="<<oldpos<<"in splitedge"; cin.get();}
       f2->setdualedge(edge(newv,v2),oldpos);

       int pre_oldpos=oldpos-1;
       if(pre_oldpos<0) pre_oldpos+=f2->degree();
       f2->addneighbor(f1,pre_oldpos,edge(v1,newv));

       return newv;


}

vertex* drawing::splitedge(vertex* v1,vertex* v2,bool crossing=true)
{       int pos=v1->whereis(v2);
        edge duale=v1->getdualedge(pos);
        return splitedge(v1,v2,duale,crossing);
}

vertex* drawing::splitedge(int v1,int v2,bool crossing=true)
{
        return splitedge(primal[v1-1],primal[v2-1],crossing);
}

vertex* drawing::splitedge(int v1,int v2,int dv1,int dv2,bool crossing=true)
{
        return splitedge(primal[v1-1],primal[v2-1],edge(dual[dv1-1],dual[dv2-1]),crossing);
}


// Connect two vertices through a given face
// Assume that the vertices from v1 to v2 are both unique on the boundary.
void drawing::connect2v(vertex* v1, vertex* v2, face* f)
{

        vlist polygon;
  //      polygon.reserve(MAX_DEGREE);
        f->getfaces(polygon);
        int facesize=polygon.size();

        // Found positions of v1,v2 on the face boundary.
        int pos1=-1,pos2=-1;
        for(int i=0; i<facesize;i++)
        {       if((pos1!=-1)&&(pos2!=-1)) break;
                if(polygon[i]==v1) pos1=i;
                if(polygon[i]==v2) pos2=i;
        }

        //If v1 is isolated, ...
        if((pos1==-1)&&(v1->degree()==0))
        {       newvinface(v2,f,v1);
                return;
        }

        //If v2 is isolated, ...
        if((pos2==-1)&&(v2->degree()==0))
        {       newvinface(v1,f,v2);
                return;
        }

        int next1= (pos1+1) % facesize;
        int ipos1= v1->where2face(polygon[next1],f);

        int next2=(pos2+1) % facesize;
        int ipos2=v2->where2face(polygon[next2],f);

   // Create a new face
        face* newf=new face(dual.maxlabel()+1);
        if(newf==NULL)
        {     cout<<"Failed to generate face "<<dual.maxlabel()+1;
              exit(EXIT_FAILURE);
        }
        dual.addnewvertex(newf);

   // Add face f to neighbors of newf
        newf->addneighbor(f,edge(v2,v1));

   //connect v1 & v2.
        v1->addneighbor(v2,ipos1,edge(newf,f));
        v2->addneighbor(v1,ipos2,edge(f,newf));

    // scanning vertices from v1 to the one before v2.
        int totaldist=pos2-pos1;
        if(totaldist<0) totaldist+=facesize;

        for(int dist=0;dist<totaldist;dist++)
        {     int curpos=(pos1+dist)% facesize;
              int nextpos=(curpos+1)% facesize;
              int cur_ipos=polygon[curpos]->where2face(polygon[nextpos],f);
        //Find the outerface next to current vertex.
              face* rface=(polygon[curpos]->getdualedge(cur_ipos)).first;

       //Reset dual edge of each edge next to current vertex
              polygon[curpos]->setdualedge(edge(rface,newf),cur_ipos);

              int next_ipos=polygon[nextpos]->where1face(polygon[curpos],f);
              polygon[nextpos]->setdualedge(edge(newf,rface),next_ipos);

      // Add each face outside to neighbors of the new face
              newf->addneighbor(rface,edge(polygon[curpos],polygon[nextpos]));

     // replace f with newf in neighbors of each outer face
              int fpos=rface->where2face(f,polygon[curpos]);
              (*rface)[fpos]=newf;

     // Remove each outer face from neighbors of the old face f
              int outerpos=f->where1face(rface,polygon[curpos]);
              if(dist==totaldist-1)  f->addneighbor(newf,outerpos,edge(v1,v2));
              f->delneighbor(outerpos);
        }

}

void drawing::connect2v(vertex* v1, vertex* v2)
{       flist faces1, faces2;
     //   faces1.reserve(MAX_DEGREE);
     //   faces2.reserve(MAX_DEGREE);
        v1->getfaces(faces1);
        v2->getfaces(faces2);

//        itr=find_first_of(faces1.begin(),faces1.end(),faces2.begin(),faces2.end());
        face* commonface=NULL;
        bool found = false;
        for(flist::iterator itr1=faces1.begin();itr1!=faces1.end();itr1++)
        {   for(flist::iterator itr2=faces2.begin();itr2!=faces2.end();itr2++)
                if(*itr1==*itr2)
                {   commonface=*itr1;
                    found=true;
                    break;
                }
            if(found) break;
        }

        if(found)
                connect2v(v1,v2,commonface);
}


void drawing::newvinface(vertex* v1, face* f, vertex* alreadyinface=NULL)
{
        vlist polygon;
   //     polygon.reserve(MAX_DEGREE);
        f->getfaces(polygon);
        int facesize=polygon.size();

        // Found position of v1 on the face boundary.
        int pos1=-1;
        for(int i=0; i<facesize;i++)
        {   if(polygon[i]==v1)
             { pos1=i; break;}
        }
        if(pos1==-1) return;

        int nexti= (pos1+1) % facesize;
        int ipos= v1->where2face(polygon[nexti],f);

        // Create a new vertex in the middle of face f
        vertex* newv;
        if(alreadyinface==NULL)
        {        newv=new vertex(primal.maxlabel()+1);

                if(newv==NULL)
                {     cout<<"Failed to generate vertex "<<primal.maxlabel()+1;
                      exit(EXIT_FAILURE);
                }
         }
        else
                newv=alreadyinface;

        newv->addneighbor(v1,edge(f,f));

        if(alreadyinface==NULL) primal.addnewvertex(newv);

        int nextipos=(ipos+1)% v1->degree();
        face* lface=(v1->getdualedge(nextipos)).second;
        int fpos=f->where2face(lface,v1);

        //Update neighbor faces of f

        f->addneighbor(f,fpos,edge(newv,v1));
        f->addneighbor(f,fpos,edge(v1,newv));

        //Add the new vertex to v1
        v1->addneighbor(newv,ipos,edge(f,f));

}
/*
void drawing::newvinface(int v1, int f, int inner=-1)
{
        if(inner==-1)
                newvinface(primal[v1-1],dual[f-1]);
        else newvinface(primal[v1-1],dual[f-1],primal[inner-1]);
}
*/
void drawing::connect2v(int v1, int v2, int f)
{       connect2v(primal[v1-1],primal[v2-1],dual[f-1]);
}

void drawing::relabel()
{       primal.relabel();
        dual.relabel();
}


void drawing::connect2v(int v1, int v2)
{       connect2v(primal[v1-1],primal[v2-1]);
}


//Add an isolated vertex
void drawing::addnewvertex(bool crossing=false)
{
        vertex* newv=new vertex(primal.maxlabel()+1);

        if(newv==NULL)
        {     cout<<"Failed to generate vertex "<<primal.maxlabel()+1;
              exit(EXIT_FAILURE);
        }

        primal.addnewvertex(newv,crossing);

}



// 0 --- There is a forced path crossing.
// 1 --- P2 is codirectional on the right side of P1
// 2 --- P2 is reverse-directional on the right side of P1
// -1 --- P2 is codirectional on the left side of P1
// -2 --- P2 is reverse-codirectional on the left side of P1
// 111 ---- P1 or P2 does not pass face f, or unable to determine
// 222 ---- P1==P2

// This member function works even if some edges are splitted.
int drawing::relposition(fpath& P1,fpath& P2, face* f)
{

        if(P1==P2) return 222;

// Find the previous and next faces on P1
        face* preface1=NULL;
        face* nextface1=NULL;
        vchain::iterator pos1=P1.whereis(f);

// If f is not a face in P1, return error.
        if(pos1==P1.tail())   return 111;

        vchain::iterator nextpos1=pos1, lastpos1=pos1;

        if(lastpos1!=P1.head()) { lastpos1--; preface1=*lastpos1;  }
        nextpos1++;
        if(nextpos1!=P1.tail())    nextface1=*nextpos1;

// Find the previous and next faces on P2
        face* preface2=NULL;
        face* nextface2=NULL;

        vchain::iterator pos2=P2.whereis(f);

// If f is not a face in P2, return error.
        if(pos2==P2.tail())   return 111;

        vchain::iterator nextpos2=pos2, lastpos2=pos2;

        if(lastpos2!=P2.head()) { lastpos2--; preface2=*lastpos2; }
        nextpos2++;
        if(nextpos2!=P2.tail())    nextface2=*nextpos2;

// Find where the two paths enter and go out of face f.
        float enter1=-1, enter2=-1, out1=-1, out2=-1;
        flist* neighborfaces=&(f->getneighbors());
        int fsize=neighborfaces->size();
        flist::iterator itr=neighborfaces->begin();
        int i=0;
        for(;itr!=neighborfaces->end();itr++,i++)
        {       if(*itr==preface1) enter1=i;
                //The last position (counter clockwise) is assigned,
                //if some edges are splitted (i.e. successive equal neighbor faces)

                if(*itr==preface2) enter2=i;
                if(*itr==nextface1) out1=i;
                if(*itr==nextface2) out2=i;
        }


// Dealing with the case when f is a starting face
        if( ((enter1==-1)||(enter2==-1)))
                if(out1==out2)  return relposition(P1,P2,nextface1);
                else  return 111; // don't use relative positions in starting
                //face to determine whether P1 and P2 cross or not.

// Dealing with the case when f is the last face of P1 or P2
// e.g. if the endpoint of P1 is between the ith and (i+1)th neighbor faces,
// set out1=i+0.5;
// fsize+0.5 is allowed.
// Splitted edges don't affect the endpoints.
        vlist polygon;
    //    polygon.reserve(MAX_DEGREE);
        f->getfaces(polygon);
        for(unsigned i=0;i<polygon.size();i++)
        {
             //If already found positions, quit the loop
                if((out1!=-1)&&(out2!=-1)) break;

             // Here (out1==-1) or (out2==-1) is necessary, since P1 or P2
             // could enter some face with the endpoint on the boundary, before
             // it reaches its end.
                if((out1==-1)&&(polygon[i]==P1.endpoint)) out1=i-0.5;
                if((out2==-1)&&(polygon[i]==P2.endpoint)) out2=i-0.5;
        }

//Adjust enter1 to position 0
        enter2=(enter2<enter1)?(enter2-enter1+fsize):(enter2-enter1);
        if(out1!=-1) out1=(out1<enter1)?(out1-enter1+fsize):(out1-enter1);
        if(out2!=-1) out2=(out2<enter1)?(out2-enter1+fsize):(out2-enter1);
        enter1=0;

 // Return result
/*
        if( ((enter1==enter2)&&(out1==out2))||((enter1==out1)&&(enter2==out1)) )
             if(nextface1!=NULL)
                return relposition(P1,P2,nextface1);
             else return 111;
        if( (enter2<out2)&&(out2<=out1) )
                return 1;
        if( (out2<enter2)&&(enter2<=out1) )
                return 2;
        if( (out1<=enter2)&&((enter2<out2)||(out2==0)) )
                return -2;

        // ||(enter2==0) is necessary here.
        if( (out1<=out2)&&((out2<enter2)||(enter2==0)) )
                return -1;
        else return 0;
*/
        if((enter2<out2)&&(out2<out1)) return 1;

        if((out2<enter2)&&(enter2<out1)) return 2;

        if((out1<out2)&&((out2<enter2)||(enter2==0))) return -1;

        if((out1<enter2)&&((enter2<out2)||(out2==0))) return -2;

        if(out2==out1)
        {
                int nextrel=relposition(P1,P2,nextface1);
                if(enter2==enter1) return nextrel;

                int onlypossible;
                if(enter2<out1) onlypossible=1;
                if(out1<enter2) onlypossible=-1;

                if(nextrel==onlypossible) return onlypossible;
                else return 0;
        }

        if(enter2==out1)
        {
                int nextrel=relposition(P1,P2,nextface1);
                if(out2==enter1) return nextrel;

                int onlypossible;
                if(out2<out1) onlypossible=2;
                if(out1<out2) onlypossible=-2;

                if(nextrel==onlypossible) return onlypossible;
                else return 0;
        }

        return 0;

}



int drawing::relposition(fpath& P1,fpath& P2, int n)
{       return relposition(P1,P2,dual[n-1]);
}

// Return the number of face paths which are:
//1. before curP in plist pL,
//2. on the right hand side of curP in face f,
//3. and cross the same edge to exit face as curP.
// return -1 if unable to determine.
int drawing::outatright(plist& pL,fpath& curP, face* f)
{
        int count=0;
        vchain::iterator curfpos=curP.whereis(f);

     //If f is not on curP, or f is the last face on curP, return error.
        vchain::iterator endf=curP.tail();
        endf--;
        if((curfpos==curP.tail())||(curfpos==endf)) return -1;

        vchain::iterator nextf_itr=curfpos;
        nextf_itr++;
        face* nextf_curP=*nextf_itr;

        for(plist::iterator itr=pL.begin();itr!=pL.end(); itr++)
        {
//            if(**itr==curP) break;
            if(*itr==&curP) break; // faster

            //If the ith path does not pass face f, skip to next.
            vchain::iterator fpos=(*itr)->whereis(f);
            if(fpos==(*itr)->tail()) continue;

            face* nextf=NULL;
            face* lastf=NULL;
            if(fpos!=(*itr)->head())
            {
                vchain::iterator lastf_itr=fpos;
                lastf_itr--;
                lastf=*lastf_itr;
            }
            if(fpos!=(--(*itr)->tail()))
            {   vchain::iterator nextf_itr=fpos;
                nextf_itr++;
                nextf=*nextf_itr;
            }

            //If the ith path does not cross the same edge as curP does,
            //skip to next.
            if((nextf!=nextf_curP)&&(lastf!=nextf_curP)) continue;

            int porder=relposition(curP,**itr,f);

            //If the two paths have to cross, return -1.
            if(porder==0) return -1;

            //If the ith path or curP doesn't pass face f, or
            //they equal, check next.
            if(porder>100) continue; // not neccessary

            if(porder>0) count++;
       }

       return count;

}

/*
int drawing::outatright(plist& pL,fpath& curP, face* f)
{       int pos=-1;
        for(unsigned i=0;i<pL.size();i++)
                if(pL[i]==curP) {pos=i; break;}
        if(pos!=-1) return outatright(pL,pos,curP,f);
        else return -1;
}
*/

int drawing::outatright(plist& pL,fpath& curP, int f)
{       return outatright(pL,curP,dual[f-1]);
}

void drawing::cutbypaths(plist& pL, vllist& vLL)
{
      //Nothing is done if there is no path.
        if(pL.size()==0) return;


        vertex* newv_start=new vertex(primal.maxlabel()+1);

        if(newv_start==NULL)
        {     cout<<"Failed to generate vertex "<<primal.maxlabel()+1;
              exit(EXIT_FAILURE);
        }

//        vLL.resize(pL.size());

        primal.addnewvertex(newv_start,false);


//        for(unsigned i=0;i<vLL.size();i++,itr++)
//                (*itr).push_back(newv);
        for(unsigned i=0; i<pL.size();i++)
        {       vchain* newvch=new vchain;
                newvch->push_back(newv_start);
                vLL.push_back(newvch);
        }

        vllist::iterator vlitr=vLL.begin();
        plist::iterator plitr=pL.begin();
        for(;plitr!=pL.end();plitr++,vlitr++)
        {

        // Split edges following the path

//            for(int fnum=0;fnum<pL[i].size()-1;fnum++)
//                //here -1 means we dont process the last face at this time

             list<vertex*>::iterator vitr=(**plitr).head(),vitr_end=(**plitr).tail();
             vitr_end--;
             for(;vitr!=vitr_end;vitr++)
             {  face* curface=*vitr;
                list<vertex*>::iterator nextvitr=vitr;
                nextvitr++;
                face* nextface=*nextvitr;

                 //Find the position of nextface in the neighbors of curface
                int fpos=curface->whereis(nextface);


                int firstpos=fpos;
                flist* fneigh=&(curface->getneighbors());

                //Find the first position where nextface is a neighbor of curface
                // curface might have several copies of nextface as neighbors

                for(int deltapos=1;deltapos<=curface->degree();deltapos++)
                {   int lastfpos=fpos-deltapos;
                    if(lastfpos<0) lastfpos+=curface->degree();
                    if((*fneigh)[lastfpos]==nextface)
                         firstpos--;
                    else break;
                }
                if(firstpos<0) firstpos+=curface->degree();

             //Find the number of paths alread added, which cross the same edge
             // with current path to exit current face.
//                int rightpathnum=outatright(pL,pL[i],pL[i][fnum]);
                int rightpathnum=outatright(pL,**plitr,curface);

                int insertpos=(firstpos+rightpathnum)% curface->degree();

                edge crossedge=curface->getdualedge(insertpos);
                vertex* newv = splitedge(crossedge.second,crossedge.first,edge(curface,nextface));
                if(newv==NULL)
                {     cout<<"Failed to split edge "<<crossedge;
                      exit(EXIT_FAILURE);
                }
                //vLL[i].push_back(newv);
//               (*itr).push_back(newv);
                (**vlitr).push_back(newv);

             }
               // (*itr).push_back(pL[i].endpoint);
             (**vlitr).push_back((**plitr).endpoint);
            }

}

void drawing::attachpaths(plist& pL)
{
        vllist vLL;
//        vLL.reserve(MAX_NONCROSSING_SIZE);
 //       vLL.resize(pL.size());
//        for(unsigned i=0;i<vLL.size();i++)
//            vLL[i].reserve(MAX_PATH_LENGTH);

        cutbypaths(pL,vLL);

        //It is necessary to process starting vertices in each path at first,
        //otherwise we couldn't be able to determine which face the starting
        //vertex is in later.
//-------
        vllist::iterator vlitr;


        vlitr=vLL.begin();

        for(;vlitr!=vLL.end();vlitr++)
        {     list<vertex*>::iterator vitr=(**vlitr).begin();
              list<vertex*>::iterator next_vitr=vitr;
              next_vitr++;

              //if((**vitr).degree()==0)
              if(vlitr==vLL.begin())
                        connect2v(*vitr, *next_vitr,*(pL.front()->head()));
              else connect2v(*vitr,*next_vitr);
        }

//-------

        vlitr=vLL.begin();

        for(;vlitr!=vLL.end();vlitr++)
        {     list<vertex*>::iterator vitr=(**vlitr).begin();
              vitr++;
              list<vertex*>::iterator almostend=(**vlitr).end();
              almostend--;

              for(;vitr!=almostend;vitr++)
              { list<vertex*>::iterator next_vitr=vitr;
                next_vitr++;
                connect2v(*vitr,*next_vitr);
              }
        }

//---DESTROY THE VERTEX LISTS----
        vlitr=vLL.begin();
        for(;vlitr!=vLL.end();vlitr++)
             delete *vlitr;
//---DESTROY THE VERTEX LISTS-----
}

//Convert a face path p in current drawing to a path in a copied drawing D.
void drawing::translate(fpath& p,drawing& D)
{
           int eplabel=p.endpoint->getlabel();

           vertex* ep=D.getprimal()[eplabel-1];
           p.endpoint=ep;

           list<vertex*>::iterator itr=p.head();
           for(;itr!=p.tail();itr++)
           {    int label=(**itr).getlabel();
                *itr=D.getdual()[label-1];
           }
}

void drawing::translate(plist& pL,drawing& D)
{       for(plist::iterator plitr=pL.begin();plitr!=pL.end();plitr++)
           translate(**plitr,D);
}

int drawing::findpaths(face* f, vertex* v, plist& pL, int this_delta)
{


        flist fL;
#ifdef MAX_DEGREE
        fL.reserve(MAX_DEGREE);
#endif
        v->getfaces(fL);
        vector<int> dists;
        dual.distances(f,fL,dists);
        int this_forced=*min_element(dists.begin(),dists.end());
        int count=0;
        vertex* lastv=f;

        fpath lastp;

        lastp.extend2(lastv);
        lastp.endpoint=v;

        if(this_forced==0)
        {       lastp.delta=0;
//NEW USED
                fpath * toadd=new fpath(lastp);
                pL.push_back(toadd);
                count++;
        }

        int nextvpos=0;
        do
        {
          //If all reachable neighbor faces of endpoint have all been searched
          //or the path is too long, back one step.
           if((nextvpos==lastv->degree())||(static_cast<int>(lastp.length())>=this_forced+this_delta))
           {   lastp.popback();
               if(lastp.size()==0) {return count;}
               nextvpos=lastp.back()->whereis(lastv)+1;
               lastv=lastp.back();
               continue;
           }

          vertex* nextv=(*lastv)[nextvpos];
          if(static_cast<int>(lastp.length())>=this_forced-1)
          {  // Check if the next face already exists in the path

             bool found=false;
             for(unsigned i=0;i<lastp.size();i++)
                 if(lastp[i]==nextv)
                 {      found=true;
                        break;
                 }
             if(found) {nextvpos++;continue;}

             for(unsigned i=0;i<fL.size();i++)
                 if(nextv==fL[i])
                 {
                    lastp.extend2(nextv);
                    bool good=valid(lastp);
                    if(good)
                    {  count++;
                       lastp.delta=lastp.length()-this_forced;
//NEW USED
                       fpath *toadd=new fpath(lastp);

                       pL.push_back(toadd);
                    }
                    lastp.popback();
                    if(good) break;
                 }
           }

           if(static_cast<int>(lastp.length())<this_forced+this_delta-1)
             {   lastp.extend2(nextv);
                 lastv=nextv;
                 nextvpos=0;
                 continue;
             }
           else nextvpos++;


        } while(1);
}


int drawing::generatepaths(face* f, pathset& paths, int this_delta)
{
        int totalpnum=0;
        for(int n=0;n<primal.nsize();n++)
        {
              plist* pL=new plist;
              int pnum=findpaths(f,primal(n),*pL, this_delta);


              paths[n]=pL;

              totalpnum+=pnum;
        }

        return totalpnum;
}


int drawing::generatepaths(int f, pathset& paths, int this_delta)
{       return generatepaths(dual[f-1],paths,this_delta);
}

int drawing::generatedrawings(pathset& paths, ofstream* saveFile,FILE** canongs, unsigned long* totalnum, int mincrnum, int maxcrnum)
{

        int CRNUM=6000;

        int n=primal.nsize();

        vector<plist::iterator> indexlist;
        indexlist.reserve(MAX_NONCROSSING_SIZE);


        for(int i=0;i<n;i++)
        {   if(paths[i]->size()==0) return CRNUM;
            indexlist.push_back(paths[i]->begin());
        }

    //Generate all possible combinations
        while(1)
        {     if(indexlist[n-1]==paths[n-1]->end())
              {
                 int notfull;
                 for(notfull=n-2; notfull>=0; notfull--)
                 {   plist::iterator plitr=paths[notfull]->end();
                     plitr--;
                     if(indexlist[notfull]!=plitr) break;
                 }
                 if(notfull==-1)    return CRNUM;


                 indexlist[notfull]++;
                 for(int i=notfull+1;i<n;i++)
                     indexlist[i]=paths[i]->begin();
              }

        //Get the corresponding list of paths
              plist pL4add;
              for(int i=0;i<n;i++)
              {
                  fpath* curp=*indexlist[i];
                  fpath* copied=new fpath(*curp);

                  pL4add.push_back(copied);
              }
///////////////////////////////////////////////////


              int this_crnum=num_cross()+tlength(pL4add);
              bool forced=forced2cross(pL4add);
              if((this_crnum>=mincrnum)&&(this_crnum<=maxcrnum)&&!forced)
              {
                  int this_delta=this_crnum-mincrnum;

                  drawing* newD=new drawing(*this);

                  translate(pL4add,*newD);

                  newD->attachpaths(pL4add);

#ifdef SKIP_SAVING
                  totalnum[this_delta]++;
                  int this_nextmin=newD->mincrnum();
                  if(this_nextmin<CRNUM) CRNUM=this_nextmin;
#else

        //--Get the canonical labelling of the primal graph ----
                  graph g[MAXN*MAXM],canong[MAXN*MAXM];
                  int lab[MAXN],ptn[MAXN],orbits[MAXN];
                  static DEFAULTOPTIONS_GRAPH(options);
                  statsblk(stats);
                  setword workspace[50*MAXM];

                  options.getcanon=TRUE;
                  int nn =newD->getprimal().tsize();
                  int mm=(nn + WORDSIZE - 1) / WORDSIZE;
                  int gsize=nn*mm*sizeof(graph);

                  nauty_check(WORDSIZE,mm,nn,NAUTYVERSIONID);

                  newD->getprimal().nautilize(g);

                  for(int i=0;i<primal.tsize();i++)
                  {   lab[i]=i; ptn[i]=1; }
                  ptn[num_noncr()-1]=0; ptn[primal.tsize()-1]=0;

                  nauty(g,lab,ptn,NULL,orbits,&options,&stats,
                             workspace,50*MAXM,mm,nn,canong);

        //---------------------------
//                  stringstream fstr;
//                  fstr<<"#"<<this_delta<<"_"<<nautyFilename;
//                  FILE* canongfp;
//                  canongfp=fopen(fstr.str().c_str(),"a+b");

                  fflush(canongs[this_delta]);
                  fseek(canongs[this_delta],0,SEEK_SET);
                  int status=isomorph(canongs[this_delta],canong,mm,nn);

                  if(status==0)
                  {   totalnum[this_delta]++;
                      fprintf(canongs[this_delta],"<m=%d,n=%d>",mm,nn);
                      fwrite(canong,1,gsize,canongs[this_delta]);

                      newD->getprimal().print(saveFile[this_delta]);
                      saveFile[this_delta]<<"---"<<endl;
                  }
#endif
       //----------------------------------------------------------

                  delete newD;

              }
              indexlist[n-1]++;
//----DESTROY THE COPIED PATH LIST----
 // pointers are unchanged when calling 'translate'.
              for(plist::iterator pitr=pL4add.begin();pitr!=pL4add.end();pitr++)
                   delete *pitr;

//----END DESTROY THE COPIED PATH LIST----

////////////////////////////////////////////////////

        }


}


bool drawing::valid(fpath& P)
{
        vertex* endpoint=P.endpoint;

        //Find the edges in primal crossed by pL[i].
        list<edge> eL;
//        eL.reserve(MAX_PATH_LENGTH);

        for(unsigned j=0;j<P.length();j++)
        {   int pos=P[j]->whereis(P[j+1]);
            edge e=P[j]->getdualedge(pos);
            eL.push_back(primal.belong2(e));
        }


        for(list<edge>::iterator itr=eL.begin();itr!=eL.end();itr++)
        {
            if((itr->first==endpoint)||(itr->second==endpoint))
                    return false;

            edge re_edge=edge(itr->second,itr->first);
            for(list<edge>::iterator itr2=eL.begin();itr2!=itr;itr2++)
                if((*itr2==*itr)||(*itr2==re_edge))
                    return false;
        }

        return true;

}


bool drawing::forced2cross(fpath& P1,fpath& P2)
{
/*        if(P1==P2) return true;

        int last_status, status;
        last_status=relposition(P1,P2,P1[1]);
        if(last_status==0) return true;

        for(int i=2;i<P1.size();i++)
        {       int status=relposition(P1,P2,P1[i]);
                if((status==0)||(last_status+status==0)) return true;
                last_status=status;
        }
*/
        list<vertex*>::iterator vitr=P1.tail();

        for(vitr--;vitr!=P1.head();vitr--)
            if(relposition(P1,P2,*vitr)==0) return true;

        return false;

}

bool drawing::forced2cross(plist& pL)
{
/*        for(unsigned i=0;i<pL.size();i++)
            for(unsigned j=0;j<i;j++)
               if(forced2cross(pL[i],pL[j]))
                  return true;
*/
        plist::iterator pitr=pL.begin(),pre_pitr;
        for(;pitr!=pL.end();pitr++)
             for(pre_pitr=pL.begin();pre_pitr!=pitr; pre_pitr++)
             if(forced2cross(**pitr,**pre_pitr))
                return true;
        return false;
}

int drawing::mincrnum()
{
if(num_noncr()!=5)
{
        list<int> uniquefaces;
        getuniquefaces(uniquefaces);
}
//        list<int> all_forced;
//        return forcedcrossings(uniquefaces,all_forced)+num_cross();
        return forcedcrossings()+num_cross();
}

void drawing::getuniquefaces(list<int>& uniquefaces)
{
        int orbits[MAXN];
        dual.getorbits(orbits);

        for(int i=0;i<num_face();i++)
           if(orbits[i]==i) uniquefaces.push_back(i+1);
}

int drawing::forcedcrossings()
{
        list<int> faces;
        list<int> all_forced;
        for(int i=1;i<=num_face();i++)
                faces.push_back(i);
        return forcedcrossings(faces, all_forced);
}


int drawing::forcedcrossings(list<int>& all_forced)
{
        list<int> faces;
        for(int i=1;i<=num_face();i++)
                faces.push_back(i);
        return forcedcrossings(faces, all_forced);
}

int drawing::forcedcrossings(list<int>& uniquefaces, list<int>& all_forced)
{
        list<int>::iterator itr;
        for(itr=uniquefaces.begin();itr!=uniquefaces.end();itr++)
        {
                int v_min_forced=0;
                for(int i=0;i<num_noncr();i++)
                {    flist fL;
                     primal(i)->getfaces(fL);
                     vector<int> dists;
                     dual.distances(*itr,fL,dists);
                     int min_dist=*min_element(dists.begin(),dists.end());
                     v_min_forced+=min_dist;
                }
                all_forced.push_back(v_min_forced);
        }
        return *min_element(all_forced.begin(),all_forced.end());
}

// END DRAWING IMPLEMENTATION


// FUNCTION IMPLEMENTATION ------------------------------------------------


// output an edge as "(v1,v2)".
ostream& operator<<(ostream &os, const edge &e)
{
        os<<"(";
        if(e.first==NULL) os<<-1;
        else os<<*e.first;

        os<<",";

        if(e.second==NULL) os<<-1;
        else os<<*e.second;
        os<<")";
        return os;
}

// ouput a vertex as its label.
ostream& operator<<(ostream& os, const vertex& v)
{       os << v.getlabel();
        return os;
}

ostream& operator<<(ostream& os,fpath& P)
{
        list<vertex*>::iterator vitr=P.head();
       for(;vitr!=P.tail();vitr++)
                os<<(*vitr)->getlabel()<<" ";
        if(P.endpoint!=NULL)
                os<<"-> v"<<P.endpoint->getlabel();
        return os;
}


//Logical equality, determined by their labels.
bool operator==(const vertex& v1, const vertex &v2)
{       return v1.getlabel()==v2.getlabel();
}

int str2num(string& s)
{       stringstream sstr;
        sstr.str(s);
        int result;
        sstr>>result;
        return result;
}

void num2str(int i, char* str)
{     stringstream sstr;
      sstr<<i;
      strcpy(str,sstr.str().c_str());
}

bool operator==(fpath& P1, fpath& P2)
{       if (P1.size()!=P2.size()) return false;
        if (P1.endpoint!=P2.endpoint) return false;

        list<vertex*>::iterator vitr1=P1.head(), vitr2=P2.head();
        for(;vitr1!=P2.tail();vitr1++,vitr2++)
               if((*vitr1)!=(*vitr2)) return false;
        return true;
}

pair<int,int> str2pair(string& s)
{
        stringstream sstr;
        sstr.str(s);
        int x,y;
        sstr.ignore(1);
        sstr>>x;
        sstr.ignore(1);
        sstr>>y;
        return make_pair(x,y);
}

// Graphs are separated by "----------"
int readnextgraph(istream& graphs, stringstream& nextgraph, int numblocks=1)
{
        nextgraph.str("");
        string nextdata;
        nextgraph.clear();
        if(graphs.eof()) return 1;
        for(int i=0;i<numblocks;i++)
          while(!graphs.eof())
          {       nextdata=""; //Skip white spaces at the end of file
                  graphs>>nextdata;
                  if(nextdata=="") break;
                  nextgraph<<" "<<nextdata;
                  if(nextdata[0]=='-') break;
          }
        if(nextgraph.str().length()==0) return 1;
        return 0;
}




void movetochar(istream& istr,char ch)
{
        while(!istr.eof())
        { char c=istr.get();
          if(c==ch) break;
        }
}

void moveb4char(istream& istr,char ch)
{
        while(!istr.eof())
        { char c=istr.get();
          if(c==ch) { istr.unget(); break; }
        }
}

int tlength(plist& pL)
{    int total=0;
     plist::iterator pitr=pL.begin();
     for(;pitr!=pL.end();pitr++)
     {    total+=(*pitr)->length(); }
     return total;
}


int isomorph(FILE *canongfp, graph* canong, int m, int n)
{
        int nn,mm;
        int ggsize;
        graph nextcanong[MAXN*MAXM];
        int status=0;
        while(!feof(canongfp))
        {
                int numread;
                nn=-1;mm=-1;
                numread=fscanf(canongfp,"<m=%d,n=%d>",&mm,&nn);
                if(numread==EOF) break;
                if((n!=n)||(m!=m)) continue;
                ggsize=mm*nn*sizeof(graph);

                fread(nextcanong,1,ggsize,canongfp);

                if(memcmp(canong,nextcanong,ggsize)==0)  { status=1; break;}
        }

        return status;
}

void showgraph(graph* g, int m, int n)
{       int v1,v2;
        set* gv;
        printf("\n");
        for(v1=0;v1<n;v1++)
        {   printf("%d: ",v1+1);
            gv=GRAPHROW(g,v1,m);
            for(v2=0;v2<n;v2++)
                if(ISELEMENT(gv,v2)) printf(" %d",v2+1);
            printf(";\n");
        }
}
// END FUNCTION IMPLEMENTATION ---------------------------------------------

