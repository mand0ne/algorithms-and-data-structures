#include <iostream>

template <typename Tip>
class Red {
    struct Node {
        Tip data{};
        Node *next;
        Node (const Tip &data, Node *next): data(data), next(next) {}
    };
    Node *first; 
    Node *last;
    int size;
    void Copy(const Red &r) {
        Node *p(r.first);
        while(p!=nullptr) {
            stavi(p->data);
            p=p->next;
        }
    }
    bool isEmpty() const { return first==nullptr; }
    public:
    Red() : first(nullptr), last(nullptr), size(0) {}
    ~Red() {brisi();}
    Red (const Red &r) { 
        first=nullptr; last=nullptr; size=0;
        Copy(r);
    }
    Red (Red &&r) {
        size=r.size;
        first=r.first; 
        last=r.last;
        r.first=r.last=nullptr;
    }
    Red &operator =(const Red &r) {
        if (&r!=this){
        brisi();
        Copy(r);}
        return *this;
    }
    Red &operator =(Red &&r) {
        if (&r != this){
        std::swap(first,r.first);
        std::swap(last,r.last);
        size=r.size;}
        return *this;
    }
    void stavi (const Tip &data) {
        Node *n(new Node(data, nullptr));
        if (first==nullptr)  first=last=n;
        else {
            last->next=n;
            last = last->next;
        }
        size++;
    }
    Tip skini() {
        if (first==nullptr) throw std::logic_error("Prazan red!");
        Tip data(first->data);
        Node *p=first;
        if (first == last) first=last=nullptr;
        else first=first->next;
        delete p;
        size--;
        return data;
    }
    Tip& celo() const {
        if (first==nullptr) throw std::logic_error("Prazan red!");
        return first->data;
    }
    int brojElemenata() const { return size;}
    void brisi(){
    while(!isEmpty()) skini();
    }
};

template <typename Tip>
bool areSame(Red<Tip> r1, Red<Tip> r2){
    if(r1.brojElemenata()!=r2.brojElemenata()) return false;
    while(r1.brojElemenata()!=0){
        if(r1.celo()!=r2.celo()) return false;
        r1.skini(); r2.skini();
    }
    return true;
}
void testingConstructors() {
    srand(time(NULL));
    Red<int> r1;
    if (r1.brojElemenata()==0) std::cout << "Default no argument constructor: Good" << std::endl;
    else std::cout << "Default no argument constructor: Error" << std::endl;
    
    Red<int> r2;
    for (int i=0; i<10; i++) r1.stavi(i+(rand()%100+1));
    Red<int> r3(r2);
    if (areSame(r2,r3)) std::cout << "Copy constructor: Good" << std::endl;
    else std::cout << "Copyconstructor: Error" << std::endl;
    
    Red<int> r4(std::move(r3));
    if(areSame(r4,r2)) std::cout << "Move constructor: Good" << std::endl;
    else std::cout << "Move constructor: Error" << std::endl;
}
void testingAssignmentOperators() {
    srand(time(NULL));
    Red<int> r1;
    for (int i=0; i<7; i++) r1.stavi(i+(rand()%100+1));
    Red<int> r2;
    for (int i=0; i<12; i++) r2.stavi(i+(rand()%1000+100));
    r1=r2;
    if (areSame(r1,r2)) std::cout << "Assignment operator: Good" << std::endl;
    else std::cout << "Assignment operator: Error" << std::endl;
    
    Red<int> r3;
    r3=std::move(r1);
    if (areSame(r2,r3)) std::cout << "Assignment operator (move): Good" << std::endl;
    else std::cout << "Assignment operator (move): Error" << std::endl;
}
void pushTest(Red<int> &r) {
    r.stavi(1337);
    while(r.brojElemenata()!=1) r.skini();
    
    if (r.brojElemenata()==1 && r.celo()==1337) std::cout << "Push test: Good" << std::endl;
    else std::cout << "Push test: Error" << std::endl;
} 
void popTest(Red<int> &r){
    Red<int> r1(r); 
    int n1(r1.brojElemenata());
    int y(r1.celo());
    int x(r1.skini());
    if (x==y && r1.brojElemenata()==n1-1) std::cout << "Pop test: Good" << std::endl;
    else std::cout << "Pop test: Error" << std::endl;
}
void headTest(Red<int> &r) {
    r.stavi(8); r.skini(); r.stavi(12312); r.stavi(0);
    while(r.brojElemenata()!=1) r.skini();
    if (r.celo()==0) std::cout << "Head test: Good" << std::endl;
    else std::cout << "Head test: Error" << std::endl;
}
void deleteTest(Red<int> &r) {
    r.stavi(26); r.stavi(98);
    r.brisi();
    if (r.brojElemenata()==0) std::cout << "Delete test: Good" << std::endl;
    else std::cout << "Delete test: Error" << std::endl;
}

int main() {
    srand(time(NULL));
    testingConstructors();
    testingAssignmentOperators();
    
    Red<int> r;
    for (int i=0; i<100; i++) r.stavi(i+(rand()%100+10));
    
    Red<int> r1(r);
    Red<int> r2;
    r2=r1;
    pushTest(r);
    popTest(r);
    headTest(r1);
    deleteTest(r2);
    
    return 0;
}