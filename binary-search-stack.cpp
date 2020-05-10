#include <iostream>
#include <cmath>
#include <vector>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::move;
using std::swap;

template <typename Tip>
class Stek{
private:
    struct Cvor {
        Tip element{};
        Cvor *sljedeci;
        Cvor (const Tip &element, Cvor *sljedeci):
            element(element)), sljedeci(sljedeci) {}
    };

    Cvor * pokazivacNaVrh;
    int velicina;

    void Kopiraj(const Stek &s){
        Cvor *q, *p(s.pokazivacNaVrh);
        pokazivacNaVrh = nullptr;
        velicina = s.velicina;
        
        while(p != nullptr) {
            Cvor *n(new Cvor(p->element,nullptr));
            
            if (pokazivacNaVrh == nullptr) 
                pokazivacNaVrh = n;
            else 
                q->sljedeci = n;

            p = p->sljedeci;
            q = n;
        }
    }

public:
    Stek() : pokazivacNaVrh(nullptr), velicina(0) {}

    Stek(const Stek &s){
        Kopiraj(s);
    }

    Stek(Stek &&s){
        swap(pokazivacNaVrh, s.pokazivacNaVrh);
        velicina = s.velicina;
    }

    ~Stek(){
        brisi();
    }

    Stek &operator = (const Stek &s){
        if (&s != this) {
            brisi();
            Kopiraj(s);
        }
        
        return *this;
    }

    Stek &operator = (Stek &&s){
        if (&s!=this) {
            swap(pokazivacNaVrh, s.pokazivacNaVrh);
            velicina = s.velicina;
        }
        
        return *this;
    }

    void stavi(const Tip &element){
        pokazivacNaVrh = new Cvor(element, pokazivacNaVrh);
        velicina++;
    }

    Tip skini(){
        Tip noviElement(pokazivacNaVrh->element);
        Cvor *p = pokazivacNaVrh->sljedeci;
        delete pokazivacNaVrh;
        pokazivacNaVrh = p;
        velicina--;
        return noviElement;
    }

    Tip &vrh(){
        return pokazivacNaVrh->element;
    }

    const Tip& vrh() const {
        return pokazivacNaVrh->element;
    }

    void brisi(){
        while(!jePrazan()) 
            skini();
    }

    bool jePrazan() const{
        return !pokazivacNaVrh;
    }

    int brojElemenata() const{
        return velicina;
    }
};

void pretraga (Stek<std::vector<int>> &s, int element)
{

    if (!s.brojElemenata()) {
        cout << "Nema elementa" << endl;
        return;
    }

    else if (!s.vrh().velicina()) {
        vector<int> v(move(s.skini()));
        pretraga(s, element);
        s.stavi(v);
    }

    else if (s.vrh()[0]>element) {
        vector<int> v(move(s.skini()));
        pretraga(s, element);
        s.stavi(v);
    }

    else if (element>=s.vrh()[0] && element<=s.vrh()[s.vrh().velicina()-1]) {
        int dno(0);
        int vrh(s.vrh().velicina()-1);

        while (vrh>=dno) {
            int sredina=(dno+vrh)/2;
            if (s.vrh()[sredina]<element) dno=sredina+1;
            else if (element<s.vrh()[sredina]) vrh=sredina-1;
            else {
                cout << sredina << " " << s.brojElemenata()-1;
                return;
            }
        }
        cout << "Nema elementa" << endl;
    }

    else cout << "Nema elementa" << endl;
}

void testFunction1()
{
    Stek<std::vector<int>> s;
    std::vector<int> v1({1, 2, 3, 4});
    std::vector<int> v2({5,6, 7,8,9,10});
    std::vector<int> v3({11, 12, 13});
    s.stavi(v1);
    s.stavi(v2);
    s.stavi(v3);
    pretraga(s, 7);
}
void testFunction2()
{
    Stek<std::vector<int>> s;
    pretraga(s, 0);
}
void testFunction3()
{
    Stek<std::vector<int>> s;
    std::vector<int> v1({1, 2, 3, 4});
    std::vector<int> v2({5,6, 7,8,9,10});
    s.stavi(v1);
    s.stavi(v2);
    pretraga(s, 11);
}
void testFunction4()
{
    Stek<std::vector<int>> s;
    std::vector<int> v1({1, 2, 3, 4});
    std::vector<int> v2({5,6, 7,8,9,10});
    std::vector<int> v3({11, 12, 13,15});
    s.stavi(v1);
    s.stavi(v2);
    s.stavi(v3);
    pretraga(s, 15);
}
void testFunction5()
{
    Stek<std::vector<int>> s;
    s.stavi({});
    s.stavi({});
    s.stavi({});
    pretraga(s, 0);
}

int main()
{
    testFunction1();
    cout <<endl;
    testFunction2();
    testFunction3();
    testFunction4();
    cout <<endl;
    testFunction5();
    return 0;
}
