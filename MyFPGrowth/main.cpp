#ifndef FP_TREE_DS
#define FP_TREE_DS
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <stack>
#include <queue>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;
class TreeNode
{
    public:int itemNum; //item set
    public:int count; // item set count
    public:vector<TreeNode *> child; // its children node
    public:TreeNode *parent; // ptr to parent
    public:TreeNode()
    {
        parent=NULL;
    }
};

vector<pair<vector<int>,int> > freqItem;
int trans = 0;
float minSupport;

#endif
bool cmp_by_value(const pair<int,int>& lhs, const pair<int,int>& rhs) {
    return lhs.second > rhs.second;
}

void constructFList(vector<pair<int,bool> > &FList, char* fileName){
    //redirect stdin to 1.in file

    ifstream settingsFile(fileName);
    vector<vector<int>> transactions;
    string line;
    map<int,int> itemCount;
    map<int,int> frequentItemTemp;
    
    transactions.clear();
    //read until EOF
    while(!getline(settingsFile, line).eof()){
        vector<int> arr;
        istringstream ssline(line);
        string number;
        while(getline(ssline, number, ','))
            arr.push_back(atoi(number.c_str()));
        transactions.push_back(arr);
        trans += 1;
    }
    settingsFile.close();
    itemCount.clear();
    for (int i = 0; i < transactions.size(); i++)
    {
        for (int j = 0; j < transactions[i].size(); j++)
        {
            if(itemCount.count(transactions[i][j]) > 0)
                itemCount[transactions[i][j]] += 1;
            else
                itemCount[transactions[i][j]] = 1;
        }
    }
    minSupport = ceil(minSupport*(float)trans);
    for (map<int,int>::iterator it=itemCount.begin(); it!=itemCount.end(); ++it){
        if((float)it->second > minSupport)
            frequentItemTemp[it->first] = it->second;
    }
    
    vector<pair<int, int>> itemVector(frequentItemTemp.begin(), frequentItemTemp.end());
    sort(itemVector.begin(), itemVector.end(), cmp_by_value);
    
    for (int i = 0; i != itemVector.size(); i++) {
        //cout << itemVector[i].first << ":" << itemVector[i].second << endl;
        FList.push_back(make_pair(itemVector[i].first, false));
    }
}

void initializeHeaderTable(vector<pair<int,bool> > &FList, vector<pair<int,vector<TreeNode* > > > &headerTable){
    vector<TreeNode *> initial;
    initial.clear();
    
    for(int i = 0; i < FList.size(); i++){
        headerTable.push_back(make_pair(FList[i].first, initial));
    }
}

void insert(vector<pair<int,int>> insertList,int currentIndex,TreeNode *prt,vector<TreeNode *>& tree, vector<pair<int,vector<TreeNode* > > > &headerTable){
    bool isFound = false;
    if(tree.size() > 0){
        for(int i = 0; i < tree.size(); i ++){
            if(insertList.size() > 0){
                if(tree[i]->itemNum == insertList[currentIndex].first){
                    isFound = true;
                    tree[i]->count += insertList[currentIndex].second;
                    if(currentIndex + 1 < insertList.size())
                        insert(insertList, currentIndex+1, tree[i], tree[i]->child, headerTable);
                }
            }
        }
    }
    if(isFound == false){
        if(insertList.size() != 0){
            TreeNode *newNode;
            newNode = new TreeNode;
            newNode->itemNum = insertList[currentIndex].first;
            newNode->count = insertList[currentIndex].second;
            newNode->child.clear();
            newNode->parent = prt;
            if(prt != NULL){
                //cout << "newNode: itemNum=" << newNode->itemNum << " Count= " << newNode->count << " parent= " << newNode->parent->itemNum << endl;
            }
            else{
                //cout << "newNode: itemNum=" << newNode->itemNum << " parent= NULL" << endl;
            }

            for(int i = 0; i < headerTable.size(); i++){
                if(headerTable[i].first == insertList[currentIndex].first){
                    headerTable[i].second.push_back(newNode);
                    //cout << "header " << i << " link to " << newNode->itemNum << endl;
                }
            }
            
            if(currentIndex +1 < insertList.size()){
                insert(insertList,currentIndex+1,newNode,newNode->child, headerTable);
            }
            tree.push_back(newNode); //push_back to be child
        }
    }
}

void createTree(vector<vector<pair<int,int> > > transactions, vector<pair<int,bool> > &FList, vector<TreeNode *> &treeRoot, vector<pair<int,vector<TreeNode* > > > &headerTable){
    
    vector<pair<int,int>> insertList;
    insertList.clear();
    for (int i = 0; i < transactions.size(); i++)
    {
        for(int j = 0; j< FList.size(); j++){
            for (int k = 0; k < transactions[i].size(); k++){
                if (FList[j].first == transactions[i][k].first){
                    FList[j].second = true;
                    insertList.push_back(make_pair(FList[j].first, transactions[i][k].second) );
                }
            }
        }
        /*
        for(int i=0;i<insertList.size();i++)
            cout << insertList[i] << endl;
        */
        insert(insertList,0,NULL,treeRoot, headerTable);
        
        for (int i = 0; i < FList.size(); i ++){
            FList[i].second = false;
        }
        insertList.clear();
    }
    
}

void ScanDataset(vector<pair<int,bool> > &FList, vector<TreeNode *> &treeRoot, vector<pair<int,vector<TreeNode* > > > &headerTable, char* fileName){
    //redirect stdin to 1.in file
    ifstream settingsFile(fileName);
    vector<vector<pair<int,int> > > transactions;
    string line;
    
    //read until EOF
    while(!getline(settingsFile, line).eof()){
        vector<pair<int,int> > arr;
        istringstream ssline(line);
        string number;
        while(getline(ssline, number, ','))
            arr.push_back(make_pair(atoi(number.c_str()), 1) );
        transactions.push_back(arr);
    }
    settingsFile.close();
    
    createTree(transactions, FList, treeRoot, headerTable);
}

void verticalMining(TreeNode *node, int count, int CPBIndex, vector<vector<pair<int,int> > > &CPB){
    CPB[CPBIndex].push_back(make_pair(node->itemNum, count));
    //cout << CPB[CPBIndex][0].first << " "<< CPB[CPBIndex][0].second << endl;
    if(node->parent != NULL){
        verticalMining(node->parent,count,CPBIndex, CPB);
    }
}

void mineTree(vector<pair<int,vector<TreeNode* > > > &headerTable, vector<vector<pair<int,int> > > &CPB, vector<pair<int,bool> > FList, vector<int> curSet){
    for (vector<pair<int,vector<TreeNode* > > >::reverse_iterator i = headerTable.rbegin(); i != headerTable.rend(); ++i )
    {
        vector<vector<pair<int,int> > > transactions;
        map<int,int> frequentItemTemp;
        map<int, int> metaTemp;
        vector<pair<int,bool> > metaFList;
        vector<TreeNode *> metatreeRoot;
        vector<pair<int,vector<TreeNode* > > > metaheaderTable;
        vector<vector<pair<int,int> > > metaCPB(500000);
        vector<int> metaCurSet;
        int frequentCount = 0;
        
        metaTemp.clear();
        CPB.clear();
        transactions.clear();
        frequentItemTemp.clear();
        metaFList.clear();
        metatreeRoot.clear();
        metaheaderTable.clear();
        metaCurSet.clear();
        
        for(int k = 0; k < i->second.size(); k++){
            //CPB[k].push_back(make_pair(i->second[k]->itemNum, i->second[k]->count));
            frequentCount += i->second[k]->count;
            if(i->second[k]->parent != NULL){
                //cout << i->second[k]->parent->itemNum << " "<< i->second[k]->count << endl;
                verticalMining(i->second[k]->parent, i->second[k]->count, k, CPB);
            }
        }
        
        /* handle metaCurSet */
        for(int k = 0; k < curSet.size(); k++){
            metaCurSet.push_back(curSet[k]);
        }
        metaCurSet.push_back(i->first);
        freqItem.push_back(make_pair(metaCurSet, frequentCount));
        
        /*
        for(int j = 0; j < metaCurSet.size(); j++){
            cout << metaCurSet[j] << ",";
        }
        cout << "count: " << frequentCount;
        */
        int j = 0;
        while(j < CPB.capacity()){
            if(CPB[j].size() > 0){
                for(int k = 0; k < CPB[j].size(); k++){
                    //cout << "CBP of item " << CPB[j][k].first << " count = " << CPB[j][k].second << endl;
                    if(metaTemp.count(CPB[j][k].first))
                        metaTemp[CPB[j][k].first] += CPB[j][k].second;
                    else
                        metaTemp[CPB[j][k].first] = CPB[j][k].second;
                }
                //cout << endl;
                transactions.push_back(CPB[j]);
            }
            CPB[j].clear();
            j++;
        }
        
        if(metaTemp.size() > 0){
            for (map<int,int>::iterator it=metaTemp.begin(); it!=metaTemp.end(); ++it){
                //cout << "metaTemp " << it->first << " count " << it->second << endl;
                if((float)it->second >= minSupport)
                    frequentItemTemp[it->first] = it->second;
            }
            
            vector<pair<int, int>> itemVector(frequentItemTemp.begin(), frequentItemTemp.end());
            sort(itemVector.begin(), itemVector.end(), cmp_by_value);
            
            for (int k = 0; k != itemVector.size(); k++) {
                //cout << itemVector[i].first << ":" << itemVector[i].second << endl;
                metaFList.push_back(make_pair(itemVector[k].first, false));
            }
            
            /*
             for (int i = 0; i != metaFList.size(); i++) {
             cout << i << ":" << metaFList[i].first << endl;
             }
             */
            /*
             for(int k=0; k < transactions.size(); k++){
             for(int l = 0; l < transactions[k].size(); l++){
             
             cout << "transaction " << transactions[k][l].first << "count " << transactions[k][l].second << endl;
             }
             cout << endl;
             }
             */
            
            //cout << endl;
            initializeHeaderTable(metaFList, metaheaderTable);
            createTree(transactions, metaFList, metatreeRoot, metaheaderTable);
            mineTree(metaheaderTable, metaCPB, metaFList, metaCurSet);
        }
        /*
        for(int k = 0; k < metaheaderTable.size(); k++){
            cout<< "metaHeader for item number "<< k << ": " << metaheaderTable[k].first << endl;
            for(int l = 0; l < metaheaderTable[k].second.size(); l++){
                cout << metaheaderTable[k].second[l]->count << endl;
            }
        }
         */
    }
}
/*
bool compareVector(const pair<vector<int>,int>& lhs, const pair<vector<int>,int>& rhs) {
    return lhs.first.size() < rhs.first.size();
}
*/
int main(int argc, char* argv[])
{
    if( argc == 4 ){
        //cout << argv[1] << " " << argv[2] << " " << argv[3] << endl;
        minSupport = atof(argv[1]);
        vector<pair<int,bool> > FList;
        vector<TreeNode *> treeRoot;
        vector<pair<int,vector<TreeNode* > > > headerTable;
        vector<vector<pair<int,int> > > CPB(500000);
        vector<int> curSet;
        vector<pair<vector<int>,int> > finalFreqItem;
        
        constructFList(FList,argv[2]);
        /*
        for (int i = 0; i != FList.size(); i++) {
            cout << i << ":" << FList[i].first << endl;
        }
        */
        initializeHeaderTable(FList, headerTable);
        ScanDataset(FList, treeRoot, headerTable, argv[2]);
        /*
         for(int i = 0; i < headerTable.size(); i++){
         cout<< "Header for item number "<< i << ": " << headerTable[i].first << endl;
         for(int j = 0; j < headerTable[i].second.size(); j++){
         cout << headerTable[i].second[j]->count << endl;
         }
         }
         */
        curSet.clear();
        mineTree(headerTable, CPB, FList, curSet);
        
        for(int i = 0; i < freqItem.size(); i++){
            sort(freqItem[i].first.begin(), freqItem[i].first.end());
        }
        for(int i = 0; i < freqItem.size(); i++){
            sort(freqItem.begin(), freqItem.end());
        }
        for(int i = 1; i < 1000; i++){
            for(int j = 0; j < freqItem.size(); j++){
                if(freqItem[j].first.size() == i){
                    finalFreqItem.push_back(make_pair(freqItem[j].first, freqItem[j].second));
                }
            }
        }
        
        fstream outputFile;
        outputFile.open(argv[3], ios::out);
        
        for(int i = 0; i < finalFreqItem.size(); i++){
            //cout << "frequent item found: ";
            //cout << finalFreqItem[i].first[0];
            outputFile << finalFreqItem[i].first[0];
            for(int j = 1; j < finalFreqItem[i].first.size(); j++){
                //cout << "," << finalFreqItem[i].first[j];
                outputFile << "," << finalFreqItem[i].first[j];
            }
            //cout << ":" << fixed << setprecision( 4 ) << setfill( '0' ) << (float)finalFreqItem[i].second/(float)trans << endl;;
            outputFile << ":" << fixed << setprecision( 4 ) << setfill( '0' ) << (float)finalFreqItem[i].second/(float)trans << endl;;
        }
        outputFile.close();
    }
    
    return 0;
}
