#include<bits/stdc++.h>
using namespace std;

int no_of_digits(int n){
    if(n==0)
        return 1;
    int count=0;
    while(n>0){
        count++;
        n/=10;
    }
    return count;
}

void print_top_part(int n){
    if(n==1){
        cout<<"\xda\xc4\xc4\xc4\xc4\xc4\xbf\n";
        return;
    }
    cout<<"\xda\xc4\xc4\xc4\xc4\xc4\xc2";
    for(int i=0;i<n-2;i++)
        cout<<"\xc4\xc4\xc4\xc4\xc4\xc2";
    cout<<"\xc4\xc4\xc4\xc4\xc4\xbf\n";
}

void print_middle_part(vector<int>& v)
{
    cout<<"\xb3";
    if(v.size()==1)
    {
        int digits=no_of_digits(v[0]);
        if(digits==1)
            cout<<"  "<<v[0]<<"  \xb3\n";
        else if(digits==2)
            cout<<" "<<v[0]<<"  \xb3\n";
        else
            cout<<" "<<v[0]<<" \xb3\n";
        return;
    }
    
    
    for(int i=0;i<v.size();i++)
    {
        int digits=no_of_digits(v[i]);
        if(digits==1)
            cout<<"  "<<v[i]<<"  \xb3";
        else if(digits==2)
            cout<<" "<<v[i]<<"  \xb3";
        else
            cout<<" "<<v[i]<<" \xb3";
    }
    cout<<"\n";
}

void print_last_part(int n)
{
    if(n==1)
    {
        cout<<"\xc0\xc4\xc4\xc4\xc4\xc4\xd9"; 
        return;
    }
    cout<<"\xc0\xc4\xc4\xc4\xc4\xc4"; 
    for(int i=0;i<n-2;i++)
        cout<<"\xc1\xc4\xc4\xc4\xc4\xc4"; 
    cout<<"\xc1\xc4\xc4\xc4\xc4\xc4\xd9"; 
}

void print_using_horizontal_boxes(vector<int>& v)
{
    print_top_part(v.size());
    print_middle_part(v);
    print_last_part(v.size());
}

void print_pages(vector<int>& f, int max_cap){
    if(!f.size()) return;
    cout<<endl;
    for(int i=0;i<max_cap*7;i++)
        cout<<"\xcd";
    cout<<endl;
    print_using_horizontal_boxes(f);
    cout<<endl;
    for(int i=0;i<max_cap*3;i++)
        cout<<"__";
    cout<<endl;
}

class node{
    public: 
        int page;
        node *next, *prev;
        node(int v){
            page=v;
        }
};

class LRUPrimaryCache {
public:
    
    node *head=new node(-1);
    node *tail=new node(-1);
    int cap;
    map<int,node*> m;
    
    LRUPrimaryCache(int capacity) {
        cap=capacity;
        head->next=tail;
        tail->prev=head;
        head->prev=tail->next=NULL;
    }
    
    void insert(int page){
        node *temp=new node(page);
        temp->next=head->next;
        temp->prev=head;
        head->next=temp;
        temp->next->prev=temp;
        cap--;
        m[page]=temp;
    }
    
    void Delete(node *t){
        t->prev->next=t->next;
        t->next->prev=t->prev;
        cap++;
        m.erase(t->page);
    }
    
    void put(int page) {
        if(m[page] != NULL) Delete(m[page]);
        else if(!cap) Delete(tail->prev);
        insert(page);
    }

    bool isPresent(int page){
        return m.find(page) != m.end();
    }
};

class LRUSecondaryCache{

    public: 

    map<int,node*> Ain, Aout;
    node* AinHead = new node(-1);
    node* AoutHead = new node(-1);
    node* AinTail = new node(-1);
    node* AoutTail = new node(-1);
    int capAin, capAout;

    LRUSecondaryCache(int capacity){
        capAin = capAout = capacity;
        AinHead->next = AinTail;
        AinTail->prev = AinHead;
        AinHead->prev = AinTail->next=NULL;
        AoutHead->next = AoutTail;
        AoutTail->prev = AoutHead;
        AoutHead->prev = AoutTail->next=NULL;
    }

    bool isPresentAin(int page){
        return Ain.find(page) != Ain.end();
    }

    bool isPresentAout(int page){
        return Aout.find(page) != Aout.end();
    }

    void insertAin(int page){
        if(Ain.find(page) != Ain.end()) return;
        if(!capAin) DeleteAin();
        node *temp=new node(page);
        temp->next = AinHead->next;
        AinHead->next = temp;
        temp->prev = AinHead;
        temp->next->prev = temp;
        Ain[page] = temp;
        capAin--;
    }
    
    void DeleteAin(){
        capAin++;
        int last = AinTail->prev->page;
        insertAout(last);
        Ain.erase(last);
        AinTail->prev->prev->next = AinTail;
        AinTail->prev = AinTail->prev->prev;
    }

    void insertAout(int page){
        if(Aout.find(page) != Aout.end()) return;
        if(!capAout) DeleteAout(-1);
        node *temp=new node(page);
        temp->next = AoutHead->next;
        temp->next->prev = temp;
        AoutHead->next = temp;
        temp->prev = AoutHead;
        Aout[page] = temp;
        capAout--;
    }
    
    void DeleteAout(int page){
        capAout++;
        if(page != -1){
            node* temp = Aout[page];
            Aout.erase(page);
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
        }
        else{
            Aout.erase(AoutTail->prev->page);
            AoutTail->prev->prev->next = AoutTail;
            AoutTail->prev = AoutTail->prev->prev;
        }
    }
};

void secondaryCache(LRUPrimaryCache &primary, int page, LRUSecondaryCache &secondary){
    if(primary.isPresent(page)) primary.put(page);
    else if(secondary.isPresentAout(page)){
        secondary.DeleteAout(page);
        primary.put(page);
    }
    else if(secondary.isPresentAin(page)) return;
    else{
        cout<<"Page fault occurred...\n";
        secondary.insertAin(page);
    }
}

int main(){
    int capacity, n;
    cout<<"Enter the maximum number of frames possible: ";
    cin>>capacity;
    LRUPrimaryCache primary(capacity);
    LRUSecondaryCache secondary(capacity >> 1);
    cout<<"Enter the number of pages invoked: ";
    cin>>n;
    vector<int> v(n);
    cout<<"Enter the sequence of pages invoked: ";
    for(int i=0; i<n; i++) cin>>v[i];
    cout<<"The sequence of pages stored in the frames of physical memory after each page is invoked is :\n";
    for(int i=0; i<n; i++){
        secondaryCache(primary, v[i], secondary);
        cout<<"Primary LRU buffer contents: ";
        vector<int> pri;
        node* temp = primary.head->next;
        while(temp->page != -1){
            pri.push_back(temp->page);
            temp = temp->next;
        }
        print_pages(pri, capacity);
        cout<<"\nSecondary Ain FIFO buffer contents: ";
        vector<int> secAin;
        temp = secondary.AinHead->next;
        while(temp->page != -1){
            secAin.push_back(temp->page);
            temp = temp->next;
        }
        print_pages(secAin, capacity >> 1);
        vector<int> secAout;
        cout<<"\nSecondary Aout FIFO buffer contents: ";
        temp = secondary.AoutHead->next;
        while(temp->page != -1){
            secAout.push_back(temp->page);
            temp = temp->next;
        }
        print_pages(secAout, capacity >> 1);
        cout<<"\n";
        for(int i=0;i<70;i++)
            cout<<"\xdf\xdf";
        cout<<endl;
    }
    return 0;
}

// 5
// 20
// 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1