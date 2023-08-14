#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;
void Bitmap(vector<vector<int>> &Bitmap_vc ,vector<int> &Bitmap_CID_len,vector<int> &Bitmap_CID_real_len,int &Customer_Transaction_MaximumIdCount){

    ifstream in;
    ofstream out;
    in.open("data.txt");
    if(in.fail()){
        cout << "input file opening failed";
        exit(1);
    }
    vector<int> Itemset_Temporary{0,0,0,0},push_Temporary{0,0,0,0};
    int CID_TIDCount =0;


    int CID=0, TID=0 ,Item;

    int CID_flag=CID,TID_flag=TID,first_flag =0;

    int k,zero_count;
    while(!in.eof()){
        in >> CID>>TID>>Item;
        if(TID_flag!=TID && first_flag!=0){//TID change

            CID_TIDCount++;
            Bitmap_vc.push_back(Itemset_Temporary);
            Itemset_Temporary={0,0,0,0};

        }

        if(CID_flag!=CID && first_flag!=0){//CID change

            k=ceil(log2(CID_TIDCount));
            //zero_count = pow(2,k);

            if(pow(2,k)>4){
                zero_count=pow(2,k)-CID_TIDCount;
            }else{
                zero_count=4-CID_TIDCount;
            }

            Itemset_Temporary={0,0,0,0};
            for(int i=0;i<zero_count;i++){
                Bitmap_vc.push_back(push_Temporary);
            }
            Bitmap_CID_len.push_back(CID_TIDCount);
            Customer_Transaction_MaximumIdCount=max(CID_TIDCount, Customer_Transaction_MaximumIdCount);
            CID_TIDCount=0;

            if(pow(2,k)<4){//any sequence of size smaller than or equal to 4 is considered as a 4-bit sequence
                Bitmap_CID_real_len.push_back(4);
            }else{
                Bitmap_CID_real_len.push_back(pow(2,k));
            }

        }

        first_flag++;
        Itemset_Temporary[Item-1]++;
        CID_flag=CID;
        TID_flag=TID;

    }
    in.close();

    CID_TIDCount++;
    Bitmap_vc.push_back(Itemset_Temporary);


    k=ceil(log2(CID_TIDCount));
    zero_count = pow(2,k);
    if(pow(2,k)>4){
        zero_count=pow(2,k)-CID_TIDCount;
    }else{
        zero_count=4-CID_TIDCount;
    }

    Itemset_Temporary={0,0,0,0};
    for(int i=0;i<zero_count;i++){
        Bitmap_vc.push_back(push_Temporary);
    }
    Bitmap_CID_len.push_back(CID_TIDCount);
    CID_TIDCount=0;
    if(pow(2,k)<4){//any sequence of size smaller than or equal to 4 is considered as a 4-bit sequence
        Bitmap_CID_real_len.push_back(4);
    }else{
        Bitmap_CID_real_len.push_back(pow(2,k));
    }
}

vector<int> DFS_bitmap_initial(vector<vector<int>> &Bitmap_vc,vector<int> DFS_bitmap,int i_item){
    for(int i=0;i<Bitmap_vc.size();i++){
        DFS_bitmap[i]=Bitmap_vc[i][i_item];
    }

    return DFS_bitmap;
}

vector<int> DFS_bitmap_S_step(vector<vector<int>> &Bitmap_vc,vector<int> &Bitmap_CID_real_len,vector<int> DFS_bitmap,int i_item){
    bool flag_k;
    flag_k = false;
    vector<int> DFS_bitmap_s(DFS_bitmap.size());
    int B_i=0;

    for(int i=0;i<Bitmap_CID_real_len.size();i++){
        flag_k = false;
        for(int j=0;j<Bitmap_CID_real_len[i];j++){
            if(flag_k){
                DFS_bitmap_s[B_i]=1;
            }else{
                if(DFS_bitmap[B_i] == 1){
                    flag_k = true;
                }
            }

            B_i++;
        }
    }

    for(int i=0;i<Bitmap_vc.size();i++){
        DFS_bitmap_s[i] = (int)(DFS_bitmap_s[i] == 1 ? true : false) && (Bitmap_vc[i][i_item] == 1 ? true : false);
    }

    return DFS_bitmap_s;

}

bool frequentJudge(vector<vector<int>> &Bitmap_vc,vector<int> &Bitmap_CID_real_len,vector<int> DFS_bitmap,int &support){

    int B_i=0;
    int DFS_bitmap_support=0;
    bool flag_sup;
    for(int i=0;i<Bitmap_CID_real_len.size();i++){
        flag_sup=false;
        for(int j=0;j<Bitmap_CID_real_len[i];j++){
            if(DFS_bitmap[B_i]>0){
                flag_sup=true;
            }
            B_i++;
        }
        if(flag_sup) DFS_bitmap_support++;
    }

    return DFS_bitmap_support>=support;

}

vector<int> DFS_bitmap_I_step(vector<vector<int>> &Bitmap_vc,vector<int> &Bitmap_CID_real_len,vector<int> DFS_bitmap,int i_item){

    for(int i=0;i<Bitmap_vc.size();i++){
        DFS_bitmap[i] = (int)(DFS_bitmap[i] == 1 ? true : false) && (Bitmap_vc[i][i_item] == 1 ? true : false);
    }
    return DFS_bitmap;

}

void DFS(vector<vector<int>> &Bitmap_vc,vector<int> &Bitmap_CID_len,vector<int> &Bitmap_CID_real_len,vector<int> DFS_bitmap,
        vector<vector<int>> LetterSet,vector<int> Stemp_item,vector<int> Itemp_item,
        int item_quantity ,int &support,bool once,int &Recursion_quantity,int &Customer_Transaction_MaximumIdCount,int Now_item)
{
    if(item_quantity>Customer_Transaction_MaximumIdCount){
        return;
    }

    cout<<"Item:";
    for(int i=0;i<LetterSet.size();i++){
        for(int j=0;j<LetterSet[i].size();j++){
            cout<<LetterSet[i][j]+1<<" ";
        }
        if(i!=LetterSet.size()-1){
            cout<<",";
        }

    }
    cout<<endl;
    cout<<"DFS_bitmap:";
    for(int j=0;j<DFS_bitmap.size();j++){
        cout<<DFS_bitmap[j]<<" ";
    }
    cout<<endl;
    cout<<"Stemp_item:";
    for(int j=0;j<Stemp_item.size();j++){
        cout<<Stemp_item[j]<<" ";
    }
    cout<<endl;
    cout<<"Itemp_item:";
    for(int j=0;j<Itemp_item.size();j++){
        cout<<Itemp_item[j]<<" ";
    }
    cout<<endl;
    cout<<endl;


    vector<int> DFS_bitmap_temp;

    vector<int> Stemp_item_temp;
    vector<int> Itemp_item_temp;

    vector<int> Itemp_sud;


    for(int i=0;i<Stemp_item.size();i++){// item = {A,B,C,D} => 0,1,2,3  For each (i ∈ Sn)
        if(once){
            DFS_bitmap_temp=DFS_bitmap_initial(Bitmap_vc,DFS_bitmap,Stemp_item[i]);
        }else{
            DFS_bitmap_temp=DFS_bitmap_S_step(Bitmap_vc,Bitmap_CID_real_len,DFS_bitmap,Stemp_item[i]);
        }

        if(frequentJudge(Bitmap_vc,Bitmap_CID_real_len,DFS_bitmap_temp,support)){
            Stemp_item_temp.push_back(Stemp_item[i]);
        }
    }

    for(int i=0;i<Stemp_item_temp.size();i++){// item = {A,B,C,D} => 0,1,2,3  For each (i ∈ Stemp)
        if(once){
            DFS_bitmap_temp=DFS_bitmap_initial(Bitmap_vc,DFS_bitmap,Stemp_item_temp[i]);
        }else{
            DFS_bitmap_temp=DFS_bitmap_S_step(Bitmap_vc,Bitmap_CID_real_len,DFS_bitmap,Stemp_item_temp[i]);
        }

        Itemp_sud.clear();
        for(int j=0;j<Stemp_item_temp.size();j++){// Itemp
            if(Stemp_item_temp[j]<=Stemp_item_temp[i]){
                continue;
            }
            Itemp_sud.push_back(Stemp_item_temp[j]);
        }


        LetterSet.push_back({Stemp_item_temp[i]});
        Recursion_quantity++;
        DFS(Bitmap_vc,Bitmap_CID_len,Bitmap_CID_real_len,DFS_bitmap_temp,LetterSet ,Stemp_item_temp,Itemp_sud, item_quantity+1 ,support,false,Recursion_quantity,Customer_Transaction_MaximumIdCount,Stemp_item[i]);
        LetterSet.pop_back();
    }
    Itemp_item_temp.clear();
    for(int i=0;i<Itemp_item.size();i++){// item = {A,B,C,D} => 0,1,2,3 For each (i ∈ In)
        DFS_bitmap_temp=DFS_bitmap_I_step(Bitmap_vc,Bitmap_CID_real_len,DFS_bitmap,Itemp_item[i]);
        if(frequentJudge(Bitmap_vc,Bitmap_CID_real_len,DFS_bitmap_temp,support)){
            Itemp_item_temp.push_back(Itemp_item[i]);
        }
    }



    int LetterSet_size_i=(int)LetterSet.size()-1;


    for(int i=0;i<Itemp_item_temp.size();i++){// item = {A,B,C,D} => 0,1,2,3 For each (i ∈ Itemp)
        DFS_bitmap_temp=DFS_bitmap_I_step(Bitmap_vc,Bitmap_CID_real_len,DFS_bitmap,Itemp_item_temp[i]);

        Itemp_sud.clear();
        for(int j=0;j<Itemp_item_temp.size();j++){// Itemp
            if(Itemp_item_temp[j]<=Itemp_item_temp[i]){
                continue;
            }
            Itemp_sud.push_back(Itemp_item_temp[j]);
        }

        LetterSet[LetterSet_size_i].push_back(Itemp_item_temp[i]);
        Recursion_quantity++;
        DFS(Bitmap_vc,Bitmap_CID_len,Bitmap_CID_real_len,DFS_bitmap_temp,LetterSet ,Stemp_item_temp,Itemp_sud, item_quantity ,support,false,Recursion_quantity,Customer_Transaction_MaximumIdCount,Itemp_item[i]);
        LetterSet[LetterSet_size_i].pop_back();
    }


}

int main()
{
    vector<vector<int>> Bitmap_vc;
    vector<int> Bitmap_CID_len,Bitmap_CID_real_len;
    int Customer_Transaction_MaximumIdCount=0;
    Bitmap(Bitmap_vc,Bitmap_CID_len,Bitmap_CID_real_len,Customer_Transaction_MaximumIdCount);

    vector<vector<int>> LetterSet;
    vector<int> DFS_bitmap(Bitmap_vc.size()),Stemp_item={0,1,2,3},Itemp_item={0,1,2,3};
    int support=1,Recursion_quantity=0;




    DFS(Bitmap_vc,Bitmap_CID_len,Bitmap_CID_real_len,DFS_bitmap,LetterSet,Stemp_item,Itemp_item,0,support ,true,Recursion_quantity,Customer_Transaction_MaximumIdCount,-1);
    cout<<"\n\nRecursion_quantity:"<<Recursion_quantity;
    return 0;
}
