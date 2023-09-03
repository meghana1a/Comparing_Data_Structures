#include<iostream>
#include<conio>
#include <climits>
#include<vector>
using namespace std;
class bank
{
    public:
        string name;
        int netamount;
        set<string> types;
}; 

int getminindex(bank netamounts[],int numbanks)
{
    int min=INT_MAX, minindex=-1;
    for(int i=0;i<numbanks;i++)
    {
        if(netamounts[i].netamount == 0) 
            continue;
        if(netamounts[i].netamount < min)
        {
            minindex = i;
            min = netamounts[i].netamount;
        }
    }
    return minindex;
}

int getmaxindex(bank netamounts[],int numbanks)
{
    int max=INT_MIN, maxindex=-1;
    for(int i=0;i<numbanks;i++)
    {
        if(netamounts[i].netamount == 0)
            continue;
        if(netamounts[i].netamount > max)
        {
            maxindex = i;
            max = netamounts[i].netamount;
        }
    }
    return maxindex;
}

pair<int,string> getMaxindex(bank netamounts[],int numbanks,int minindex,bank input[],int max_num_types)
{
    int max=INT_MIN;
    int maxindex=-1;
    string matchingType;
    for(int i=0;i<numbanks;i++)
    {
        if(netamounts[i].netamount == 0)
            continue;
        if(netamounts[i].netamount < 0)
            continue;
        vector<string> v(max_num_types);
        vector<string>::iterator ls=set_intersection(netamounts[minindex].types.begin(),netamounts[minindex].types.end(), netamounts[i].types.begin(),netamounts[i].types.end(), v.begin());

        if((ls-v.begin())!=0 && max<netamounts[i].netamount)
        {
            max=netamounts[i].netamount;
            maxindex=i;
            matchingType = *(v.begin());
        } 
    }

    //if there is NO such max which has a common type with any remaining banks then maxindex has -1
    // also return the common payment type
    return make_pair(maxindex,matchingType);
}

void printanswer(vector<vector<pair<int,string>>> finalgraph, int numbanks,bank input[])
{
    cout<<"\nThe transactions for minimum cash flow are as follows : \n\n";
    for(int i=0;i<numbanks;i++)
    {
        for(int j=0;j<numbanks;j++)
        {
            if(i==j) 
                continue;
            if(finalgraph[i][j].first != 0 && finalgraph[j][i].first != 0)
            {
                if(finalgraph[i][j].first == finalgraph[j][i].first)
                {
                    finalgraph[i][j].first=0;
                    finalgraph[j][i].first=0;
                }
                else if(finalgraph[i][j].first > finalgraph[j][i].first)
                {
                    finalgraph[i][j].first -= finalgraph[j][i].first; 
                    finalgraph[j][i].first =0;
                    cout<<input[i].name<<" pays Rs" << finalgraph[i][j].first<< "to "<<input[j].name<<" via "<<finalgraph[i][j].second<<endl;
                }
                else
                {
                    finalgraph[j][i].first -= finalgraph[i][j].first;
                    finalgraph[i][j].first = 0;
                    cout<<input[j].name<<" pays Rs "<< finalgraph[j][i].first<<" to "<<input[i].name<<" via "<<finalgraph[j][i].second<<endl;
                }
            }
            else if(finalgraph[i][j].first != 0)
            {
                cout<<input[i].name<<" pays Rs "<<finalgraph[i][j].first<<" to "<<input[j].name<<" via "<<finalgraph[i][j].second<<endl;
                
            }
            else if(finalgraph[j][i].first != 0)
            {
                cout<<input[j].name<<" pays Rs "<<finalgraph[j][i].first<<" to "<<input[i].name<<" via "<<finalgraph[j][i].second<<endl;
                
            }
            finalgraph[i][j].first = 0;
            finalgraph[j][i].first = 0;
        }
    }
    cout<<"\n";
}
void online_transactions(int numbanks,bank input[],unordered_map<string,int>& index_of,int num_transactions,vector<vector<int>>& graph,int max_num_types)
{
    //Find net amount of each bank has
    bank netamounts[numbanks];
    for(int b=0;b<numbanks;b++)
    {
        netamounts[b].name = input[b].name;
        netamounts[b].types = input[b].types;
        int amount = 0;
        //incoming edges
        //column travers
        for(int i=0;i<numbanks;i++)
        {
            amount += (graph[i][b]);
        }
        //outgoing edges
        //row traverse
        for(int j=0;j<numbanks;j++)
        {
            amount += ((-1) * graph[b][j]);
        }
        netamounts[b].netamount = amount;
    }
    vector<vector<pair<int,string>>> finalgraph(numbanks,vector<pair<int,string>>(numbanks,{0,""}));//adjacency matrix

    //find min and max net amount
    int z=0;
    for(int i=0;i<numbanks;i++)
    {
        if(netamounts[i].netamount == 0) 
            z++;
    }
    while(z!=numbanks)
    {
        int minindex=getminindex(netamounts, numbanks);
        pair<int,string> maxanswer = getMaxindex(netamounts, numbanks, minindex,input,max_num_types);
        int maxindex = maxanswer.first;
        if(maxindex == -1)
        {
            (finalgraph[minindex][0].first) += abs(netamounts[minindex].netamount);
            (finalgraph[minindex][0].second) = *(input[minindex].types.begin());
            int simple_maxindex = getmaxindex(netamounts, numbanks);
            (finalgraph[0][simple_maxindex].first) += abs(netamounts[minindex].netamount);
            (finalgraph[0][simple_maxindex].second) = *(input[simple_maxindex].types.begin());
            netamounts[simple_maxindex].netamount += netamounts[minindex].netamount;
            netamounts[minindex].netamount = 0;
            if(netamounts[minindex].netamount == 0) z++;
            if(netamounts[simple_maxindex].netamount == 0) z++;
        }
        else
        {
            int transaction_amount = min(abs(netamounts[minindex].netamount), netamounts[maxindex].netamount);
            (finalgraph[minindex][maxindex].first) += (transaction_amount);
            (finalgraph[minindex][maxindex].second) = maxanswer.second;
            netamounts[minindex].netamount += transaction_amount;
            netamounts[maxindex].netamount -= transaction_amount;
            if(netamounts[minindex].netamount == 0)
                z++;
            if(netamounts[maxindex].netamount == 0) 
                z++;
        }

    }
    printanswer(finalgraph,numbanks,input);
    
}
int main() 
{ 
    cout<<"\n\t\t\t\t********************* Welcome to CASH FLOW MINIMIZER SYSTEM ***********************\n\n\n";
    cout<<"This system minimizes the number of transactions among multiple banks in the different corners of the world that use different modes of payment.There is one world bank (with all payment modes) to act as an intermediary between banks that have no common mode of payment. \n\n";
    cout<<"Enter the number of banks participating in the transactions.\n";
    int numbanks;cin>>numbanks;
    bank input[numbanks];
    unordered_map<string,int> index_of;//stores index of a bank
    cout<<"Enter the details of the banks and transactions as stated:\n";
    cout<<"Bank name ,number of payment modes it has and the payment modes.\n";
    cout<<"Bank name and payment modes should not contain spaces\n";
    int max_num_types;
    for(int i=0; i<numbanks;i++)
    {
        if(i==0)
        {
            cout<<"World Bank : ";
        }
        else
        {
            cout<<"Bank "<<i<<" : ";
        }
        cin>>input[i].name;
        index_of[input[i].name] = i;
        int num_types;
        cin>>num_types;
        if(i==0) 
            max_num_types = num_types;
        string type;
        while(num_types--)
        {
            cin>>type;
            input[i].types.insert(type);
        }   
    }
    cout<<"Enter number of transactions.\n";
    int num_transactions;
    cin>>num_transactions;
    vector<vector<int>> graph(numbanks,vector<int>(numbanks,0));//adjacency matrix
    cout<<"Enter the details of each transaction as stated:";
    cout<<"Debtor Bank , creditor Bank and amount\n";
    cout<<"The transactions can be in any order\n";
    for(int i=0;i<num_transactions;i++)
   {
        cout<<(i)<<" th transaction : ";
        string s1,s2;
        int amount;
        cin >> s1>>s2>>amount;
        graph[index_of[s1]][index_of[s2]] = amount;
    }
    online_transactions(numbanks,input,index_of,num_transactions,graph,max_num_types);
    return 0; 
}
