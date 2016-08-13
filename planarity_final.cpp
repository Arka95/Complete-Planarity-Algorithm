#include<iostream>
#include<stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include<list>
#include<set>
#include<stack>
#define white 0
#define grey 1
#define black 2
#define TE 10//Tree Edge
#define BE 11//Back Edge
#define FE 12//Forward Edge
#define X -65000//considering the phi val of considered edges to be X during segmentation
using namespace std;
//-------------------------------------------------DESCRIPTION----------------------------------------------------------
/*
A graph is planar if we are able to embed each segment without them overlapping each other. Here, a segment is a simple cycle.
Take the undirected graph input as adjacency list and classify all edges using DFS and edgeClass().
TOPSORT  the graph wrt the tree edges sequence and replace the old graph with the sorted one.
this new graphs will be treated as directed and will only contain tree edges and back edges. forward edges which are not tree edges are not considered in the first trial. instead they are considered as back edges in the next occurence
Generate the low points for the source vertices of every edge
Generate the phi values of every edge which are later used to sort the ordering of the edges
*/
//------------------------------------------------------STACK---------------------------------------------------
//---------------------------------------------------------GRAPH DATA STRUCTURE------------------------------------------------------------
class BST{
    int v1;
    int v2;
    BST()
    {

    }
};
class Time
{
    public:
    int c,d;
};

class Edge
{
     public:
        int source;
        int dest;
        int type;
        int phi;
        Edge *next;
        Edge()
        {}
        Edge(int s,int d,int t)
        {
            phi=-1;//initially all phi values of the edges are -1. they are assigned in the phi function
            source=s;
            dest=d;
            type=t;
            next=NULL;
        }
};

class Vertex
{
    public:
        Edge *head;
        Vertex()
        {
            head=new Edge();
            head=NULL;//initially head is null
        }
};

int* mini(int a,int b,int c,int d)
{
    int ar[4],min1,min2,arr[2],temp,i,j;

    ar[0] = a;
    ar[1] = b;
    ar[2] = c;
    ar[3] = d;

    for(i=0;i<4;i++)
    {

        for(j=0;j<4-i;j++)
        {
            if(ar[j] > ar[j+1])
            {
                temp = ar[j];
                ar[j] = ar[j+1];
                ar[j+1] = temp;

            }

        }
    }
    arr[0] = ar[0];
    arr[1] = ar[1];

    return arr;
}
/*-------------------------------------------------------UTILITIES----------------------------------------------
class Segment
{
    int index,last;
    std::vector<list<Edge> > verti ;
     void insertEdge(int s,int d,int t)
    {
        Edge *newEdge=new Edge(s,d,t);
        verti.push_back(*newEdge);
    }
};





*/
//-------------------------------------------------------Global Variables---------------------------------------
int current,g_time=0,d=0,f=0,*state,*succ,*pred,x;
int *num,*label,j=-1,*lowpt,*nextlowpt,flag=0;
Time *times;Edge *current_Node;
//int lowpt,nxtlwpt;
//----------------------------------------------------THE REAL DEAL-----------------------------------------------------------------
class Graph
{
    //-------------------------------------DS--------------------------------------------------
    public:
    int V,E,last;
    Vertex *edge;
     std::vector<list<Edge> > p ;
     std::vector<Graph> segs;

    //-------------------------------------DS--------------------------------------------------
    Graph(int v,int e)
    {
        E=e;
        V=v;
        edge=new Vertex[V];
    }
    void insertEdge(int s,int d,int t)
    {
        int i;
        Edge *ptr=edge[s].head;
        Edge *newEdge=new Edge(s,d,t);
        if(ptr==NULL)
        {
            edge[s].head=newEdge;
        }
        else
        {
            while(ptr->next!=NULL)
            ptr=ptr->next;
            ptr->next=newEdge;
        }
    }
    void printAdj()
    {
        int i;
        Edge *ptr;
        for(i=0;i<V;i++)
        {
            for(ptr=edge[i].head;ptr!=NULL;ptr=ptr->next)
            	{
					printf("\nsource:%d ---> dest:%d---type:%d---phi_value : %d ",i,ptr->dest,ptr->type,ptr->phi);
            		printf("\n\n");
            	}
        }
    }

    void DFS(int v)//TOPSORTS and assigns visitsequence
    {
        int i;

        if(state[v] == white)
        {
        	cout<<v<<"  ";
			j++;
        	label[v] =j;
        	times[v].d=++g_time;

        //}

        state[v] = grey;
        Edge *ptr;
        for(ptr=edge[v].head;ptr!=NULL;ptr=ptr->next)
        {
            if(state[ptr->dest] == white)
				{
                    succ[v]=ptr->dest;
                    pred[ptr->dest]=v;
                    DFS(ptr->dest);
                }
        }
    //}
        state[v] = black;
        times[v].c=++g_time;
    }
    }/*End of DFS()*/

    void DF_Traversal()
    {
        int v,i;
        label=new int[V];
		state=(int *)calloc(V,sizeof(int));
		pred=(int *)calloc(V,sizeof(int));
		succ=(int *)calloc(V,sizeof(int));
		times=new Time[V];
        for(v=0; v<this->V; v++)
        {
            if(state[v]!=grey)
            {
            	//state[v]=grey;
            	DFS(v);
			}

        }
        printf("\n");
    }/*End of DF_Traversal()*/

    void time_print()
    {
    	int i;

    	for(i=0;i<V;i++)
    	cout<<" Discovery time "<<times[i].d<<" completion time "<<times[i].c<<endl;

	}

    int edgeClass(int u,int v)
    {
            if(times[u].d<times[v].d && times[v].d<times[v].c&&times[v].c<times[u].c)
                {
                    if(u == pred[v] )
                        return 10;
                    else
                        return 12;
                }
             else if(times[v].d <=times[u].d&& times[u].d<times[u].c&&times[u].c<times[v].c)
                return 11;//for back edge
    }

    void edge_type()
    {
    	int idx;

    	for(idx = 0;idx < V;idx++)
		{
		  	Edge *ptr = edge[idx].head;
               while(ptr != NULL)
               {
					cout<<"\n Edge "<<idx<<" "<<ptr->dest<<" is a "<<ptr->type;
               		ptr = ptr->next;
               }
        }
       // cout<<"\n number of tree edge is "<<ct_t/2<<"\n number of back edge is "<<ct_b/2;

	}
//------------------------------------------------------------TOPSORT----------------------------------------------------------------------

    Graph Sort()
    {
        Edge *ptr;
        int i;
        num=new int[V];//cpp global pointer variables have to be initialized using new else not allocated
        //------------------------------------RESULTS----------------------------------------------------
        printf("\nLabels:\n");
        for(i=0;i<V;i++)
            printf("label[%d]=%d\n",i,label[i]);

        Graph gbd(V,E);//this is the topsorted graph

        for(i=0;i<V;i++)
            {
                for(ptr=edge[i].head;ptr!=NULL;ptr=ptr->next)
                {
                    int z=edgeClass(i,ptr->dest);

                    if(edgeClass(ptr->dest,i)!=10&&z!=12)
                        gbd.insertEdge(label[i],label[ptr->dest],z);

                }
            }
        return gbd;
    }

//-------------------------------------------------LoPoint CALCULATION----------------------------------------------------------------

void lowpoints(int a)
{
    Edge *ptr;
    int temp,temp1,temp2,*ar;
    lowpt[a] = nextlowpt[a] = a;

    for(ptr = edge[a].head;ptr != NULL;ptr = ptr->next)
    {
        if(ptr->type == 11)
        {
                if(lowpt[a] > ptr->dest)
                        {
							nextlowpt[a] = lowpt[a];
							lowpt[a] = ptr->dest;
						}
                else if(ptr->dest < nextlowpt[a])
						{
							nextlowpt[a] = ptr->dest;
						}
        }
        if(ptr->type == 10)
        {
            temp = ptr->dest;
            temp1 = lowpt[temp];
            temp2 = nextlowpt[temp];

            ar = mini(temp1,temp2,lowpt[a],nextlowpt[a]);

            lowpt[a] = ar[0];
            nextlowpt[a] = ar[1];
        }

    }

}
//-----------------------------------------------PHI Function=------------------------------------------------------------------------
    void phi()//assigns phi value of each edge
    {
        int v;
        Edge *ptr;
        for(v=0;v<V;v++)
        {
            for(ptr=edge[v].head;ptr!=NULL;ptr=ptr->next)
            {
                if(ptr->type == TE)  //TREE EDGE
                {
                    if(nextlowpt[ptr->dest] >= v)
                    {
                        ptr->phi=2*lowpt[ptr->dest];
                    }
                    else if(nextlowpt[ptr->dest]<v)
                    {
                        ptr->phi=2*lowpt[ptr->dest]+ 1;
                    }
                }
                else if(ptr->type== BE) //Back Edge
                {
                    ptr->phi=2*ptr->dest;
                }
            }
        }
    }
//-------------------------------------------------PHI SORT----------------------------------------------------------------------------
    void phiSort()//using bubblesort on a linked list data structure to sort the edges wrt phi values
    {
        int i,swapped=0;
        Edge *ptr1,*lptr=NULL;
        this->phi();
        this->printAdj();
         printf("\n\n\n Entering into Phisort\n");
        for(i=0;i<V;i++)
        {
            do
                {
                    swapped = 0;
                    ptr1 = edge[i].head;
                    while (ptr1->next != lptr)
                    {
                        if (ptr1->phi > ptr1->next->phi)
                        {
                            swap(ptr1->source,ptr1->next->source);
                            swap(ptr1->phi,ptr1->next->phi);
                            swap(ptr1->type,ptr1->next->type);
                            swap(ptr1->dest,ptr1->next->dest);
                            swapped = 1;
                        }
                        ptr1 = ptr1->next;
                    }
                }while (swapped);
        }
        this->printAdj();
    }
//-----------------------------------------------PATH FUNCTION------------------------------------------------------------------------
    std::stack<Edge> seq;
    void path(int v)
    {

        struct Edge *ptr;
        p[d].push_back(*edge[v].head);
           for(ptr=edge[v].head;ptr!=NULL;ptr=ptr->next)
            {
                seq.push(*ptr);
                    if(v<ptr->dest)
                     {
                         //tree edge
                         current_Node=this->edge[ptr->dest].head;
                         current=ptr->dest;
                         cout<<d<<": :"<<current<<endl;
                         path(ptr->dest);

                         //push(prevptr->dest);
                     }
                     else
                    {
                        //back edge
                        p[d].push_back(*edge[ptr->dest].head);
                        if(ptr->next==NULL)
                            seq.pop();
                        d++;//keep the traversal in a stack to check if a node has a predecessor. start the next path check from that predecessor
                        p[d].push_back(seq.top());


                  }
            }
    }
    void printPath()
    {
        int i;
        std::list<Edge>::iterator j;
        for(i=0;i<p.size();i++)
        {
            cout<<"path["<<i<<"]: ";
            for(j=p[i].begin();j!=p[i].end();j++)
                cout<<j->source<<" ";
            cout<<"size="<<p[i].size();
            cout<<endl;
        }

    }
  /*  void segmentation( )
    {
        Edge *ptr;
        std::set<Edge> edgeSet;
        std::list<Edge>::iterator it;
        for(v=0;v<V;v++)
            for(ptr=edge[v].head;ptr!=NULL;ptr=ptr->next)
            {
                edgeSet.insert(*ptr);
            }
        for(it=p[i].begin();it!=p[i].end();++it)
        {
            Graph *g=new Graph(p[i].l,qq                                                                                                );
//construct the graph with all the back edges , startiing from the last vertex traversed from the last segment
            edgeSet.erase(edgeSet.find(*it));
        }

    }


   /* void segmentation()
    {
        Graph c=this;
        list<Graph>Segments;
        d=0;
        std::list<int>::iterator j;
        // std::vector<list<int> > p ;
        //p0made segment
        for(i=1;i<V;i++)
            {
                for(c=p[i].begin();c!=p[i].end();c++)
                {
                    for(j=i+1;j<V;j++)
                    {
                        for(f=p[j].begin();f!=p[j].end();f++)
                            {
                                if(*c=*f)

                            }

                    }

                }


            }
    }

//----------------------------------------------------SEGMENT EMBEDDING--------------------------------------------------------------------------------
    /*
    stack[i] gives us an actual stack entry
    next[i] points to the next lower entry on that stack
    next[0] and next[-1] point to top of IST and OST respectively

    */
   /* void isPlanar(int i)
    {
        struct Edge *ptr;
        stack<int> ist=new stack(V);//IST
        stack<int> next=new stack(V);//OST
        stack<BST> bst=new stack(V);
        for(ptr=edge[i].head;ptr!=NULL;ptr=ptr->next)
        {
            if(s=0)
            {
                //new path starting at v
                s=v;
                p++;
                path(ptr->dest)=p;
                planar(ptr->dest);
            }
            if(v<ptr->dest)
            {
                while((stack[top]>=v||x==0)&&(stack[y]>=v||y==0))
                    x=y=BST;
                if(stack[x]>=v)
                    x=0;
                if(stack[y]>=v)
                    y=0;
                    while(next[-1]!=0&&stack[next[-1]]>=v)
                        next[-1]=next[next[-1]];
                while(next[0]!=0&&stack[next[0]]>=v)
                    next[0]=next[next[0]];
                if(path(ptr->dest)!=path(v))//all segments in (u,w) have been embeded
                {
                    //move bundles from  outside to inside and combine new bundles
                    in=0;
                    X=Y=BST;
                    while((stack[x]>f[path[ptr->dest]]||stack[y]>f[path[ptr-.dest]])&&stack[next[-1]]!=0)
                    {
                        if(stack[x]>f[path(ptr->dest)])
                        {
                            if(stack[y]>f[path(w)])
                            {
                                printf("NONPLANAR\n");
                                return ;
                            }
                            else
                                in=x;
                        }
                        else
                        {
                            temp=next[in];
                            next[in]=next[-1];
                            next[-1]=next[y];
                            next[y]=temp;
                            in=y;
                        }
                        x=BST.x;
                        y=BST.y;
                    }
                    if(x=0&&(in!=0||y!=0))
                        BST=(in,y);
                    else
                        BST=(x,y);//delete end of stack marker from OST
                    next[-1]=next[next[-1]];
                }
            }
            else
            {
                f[p]=ptr->dest;
                in=0;
                out=-1;
                while((next[in]!=0&&sack[next[in]]>ptr->dest)||(next[out]!=0&&stack[next[out]]>ptr->dest))
                {
                    //c//
                    x=BST.v1;
                    y=BST.v2;
                    if(x!=0&&y!=0)
                    {
                        if(stack[next[in]]>ptr->dest)
                        {
                            if(stack[next[out]]>ptr->dest)
                            {
                                cout<<"NONPLANAR";
                                break;
                            }
                            else
                            {
                                next[out]=next[in];
                                next[x]=next[y];
                                in=y;
                                out=x;
                            }
                        }
                        else
                        {
                            in=x;
                            out=y;
                        }

                    }
                    else if(x!=0)
                    {
                        temp=next[x];
                        next[x]=next[out];
                        next[out]=next[in];
                        next[in]=temp;
                        out=x;
                    }
                    else if(y!=0)
                        out=y;
                }
                //--------------------------------------------------------
                if(ptr->dest>f[path[s]])//8.16d
                {
                    //ptr->dest>finish vertex of earliest path containing s
                    stack[avail]=ptr->dest;
                    next[avail]=next[0];
                    next[0]=avail;
                    if(in==0)
                        in=avail;
                    avail++;
                }
                //----------------------------------------------------------
                if(out=-1)//8.16e
                    out=0;
                if(in!=0 || v!=s)
                    {
                        BST.v1=in;
                        BST.v2=out;
                    }
                //---------------------------------------------------------
                if(v!=s)//put an end of mark stack marker in OST//8.16f
                {
                    stack[avail]=0;
                    next[avail]=next[-1];
                    next[-1]=avail;
                    avail++;
                }
                //---------------------------------------------------------
                s=0;
            }
    }
*/

};
//--------------------------------------------MAIN-------------------------------------------------------------------------------------
int main()
{
    int V,E,i,s,d,w;
	FILE *fp=fopen("planar_graph.txt","r");
    fscanf(fp,"%d %d",&V,&E);
    lowpt=new int[V];
    nextlowpt=new int[V];
    int lp[V][2];
    Graph g(V,E);
    while(!feof(fp))
    {
        fscanf(fp,"%d %d ",&s,&d);
        g.insertEdge(s,d,0);
    }
    g.printAdj();
    printf("\tDFS:\n");
	g.DF_Traversal();
	//g.time_print();
    printf("\nAfter relabeling Output graph:\n");
	g=g.Sort();
	g.printAdj();
	for(i=V-1;i>=0;i--)
	{
	 	g.lowpoints(i);
	}
	cout<<"lowpoints:\n";
	for(i=0;i<V;i++)
    {
        cout<<lowpt[i]<<" ";
    }
    cout<<"\nnext_lowpoints:\n";
	for(i=0;i<V;i++)
    {
        cout<<nextlowpt[i]<<" ";
    }
    g.phiSort();
    g.p.resize(V);
    g.path(0);
    g.printPath();
    fclose(fp);
    return 0;
}
