#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;

template <typename Tip>
class Stek {
private:
    struct Cvor {
        Tip element{};
        Cvor *sljedeci;
        Cvor (const Tip &element, Cvor *sljedeci):
            element(element), sljedeci(sljedeci) {}
    };

    Cvor * pVrh; // Pokazivac na vrh steka
    int velicina;
    void Kopiraj(const Stek &s){
        Cvor *q, *p(s.pVrh);
        pVrh = nullptr;
        velicina = s.velicina;
        while(p != nullptr) {
            Cvor *n(new Cvor(p->element,nullptr));
            if (pVrh == nullptr) pVrh = n;
            else q->sljedeci = n;
            
            p = p->sljedeci;
            q = n;
        }
    }
    
public:
    Stek() : pVrh(nullptr),velicina(0) {}

    Stek(const Stek &s){
        Kopiraj(s);
    }
    
    Stek(Stek &&s){
        std::swap(pVrh,s.pVrh);
        velicina=s.velicina;
    }

    ~Stek(){
        brisi();
    }
    
    Stek &operator =(const Stek &s){
    if (&s!=this){
    brisi();
    Kopiraj(s);}
    return *this;
    }
    
    Stek &operator = (Stek &&s){
    if (&s!=this){
    std::swap(pVrh,s.pVrh);  
    velicina=s.velicina;}
    return *this;
    }
    
    void stavi(const Tip &element){
        pVrh = new Cvor(element, pVrh);
        velicina++;
    }
    
    Tip skini(){
    if (!pVrh) throw std::logic_error("Prazan stek");
    Tip element(pVrh->element);
    Cvor *p = pVrh->sljedeci;
    delete pVrh;
    pVrh = p;
    velicina--;
    return element;       
    }
    
    Tip &vrh(){
    if (!pVrh) throw std::logic_error("Prazan stek");
    return pVrh->element;
    }
    
    const Tip& vrh() const {
    if (!pVrh) throw std::logic_error("Prazan stek");
    return pVrh->element;
    }
    
    void brisi(){
    while(!JeLiPrazan()) skini();
    }
    
    bool JeLiPrazan() const{
        return !pVrh;
    }
    
    int brojElemenata() const {
        return velicina;
    }
};

void TestKonstruktora() {
    Stek<int> s;
    if (s.JeLiPrazan()) std::cout << "Test konstruktora: OK" << std::endl;
    else std::cout << "Test konstruktora: Greska" << std::endl;
}

void TestKopirajucegKonstruktora() {
    Stek<int> s1;
    for (int i=0; i<54; i++) s1.stavi(i+log(i)*100);
    Stek<int> s2(s1);
    s1.skini();
    if (s2.brojElemenata()==54) std::cout << "Test kopirajuceg konstruktora: OK" << std::endl;
    else std::cout << "Test kopirajuceg konstruktora: Greska" << std::endl;
}

void TestOperatoraDodjele() {
    Stek<int> s1;
    Stek<int> s2;
    for (int i=0; i<100; i++) s2.stavi(i+rand());
    s1=s2;
    s2.skini();
    if (s1.brojElemenata()==100 && s2.brojElemenata()==99) std::cout << "Test operatora dodjele: OK" << std::endl;
    else std::cout << "Test operatora dodjele: Greska" << std::endl;
}

void TestBrojaElemenata(Stek<int> &s, int n) {
    if (s.brojElemenata()==n) std::cout << "Test broja elementa: OK" << std::endl;
    else std::cout << "Test broja elementa: Greska" << std::endl;
}

void TestStavljanja(Stek<int> &s) {
    int n(s.brojElemenata());
    s.stavi(8); s.skini(); s.stavi(99);
    int x(s.vrh());
    if (x==99 && s.brojElemenata()==n+1) std::cout << "Test stavljanja elementa: OK" << std::endl;
    else std::cout << "Test stavljanja elementa: Greska" << std::endl;
} 

void TestSkidanja(Stek<int> &s) {
    int x(s.vrh());
    s.stavi(15); s.skini(); s.stavi(134); s.skini();
    int y(s.skini());
    if (x==y) std::cout << "Test skidanja elementa: OK" << std::endl;
    else std::cout << "Test skidanja elementa: Greska" << std::endl;
}

void TestSkidanjaSaIzuzetkom(Stek<int> &s) {
    s.brisi();
    try {
        s.skini();
    }
    catch(std::logic_error &x) {
        std::cout << "Test skidanja elementa sa izuzetkom: OK" << std::endl;
        return;
    }
    catch(...) {}
    std::cout << "Test skidanja elementa sa izuzetkom: Greska" << std::endl;
}

void TestVrha(Stek<int> &s) {
    s.stavi(8); s.skini(); s.stavi(72);
    if (s.vrh()==72) std::cout << "Test vrha: OK" << std::endl;
    else std::cout << "Test vrha: Greska" << std::endl;
}

void TestVrhaSaIzuzetkom(Stek<int> &s) {
    s.stavi(72); s.stavi(11); s.stavi(4);
    s.brisi();
    try {
        s.vrh();
    }
    catch(std::logic_error &x) {
        std::cout << "Test vrha sa izuzetkom: OK" << std::endl;
        return;
    }
    catch(...) {}
    std::cout << "Test vrha sa izuzetkom: Greska" << std::endl;
}

void TestBrisanja(Stek<int> &s) {
    s.brisi();
    if (s.brojElemenata()==0) std::cout << "Test brisanja steka: OK" << std::endl;
    else std::cout << "Test brisanja steka: Greska" << std::endl;
}

void TestPraznog(Stek<int> &s) {
    s.brisi();
    if (s.JeLiPrazan()) std::cout << "Test provjere praznog steka 1: OK" << std::endl;
    else std::cout << "Test provjere praznog steka 1: Greska" << std::endl;
}


int main() {
    TestKonstruktora();
    TestKopirajucegKonstruktora();
    TestOperatoraDodjele();
    Stek<int> s;
    for (int i=0; i<44; i++) s.stavi(i+rand()%100+10);
    TestBrojaElemenata(s, 44);
    TestStavljanja(s);
    TestSkidanja(s);
    TestSkidanjaSaIzuzetkom(s);
    TestVrha(s);
    TestVrhaSaIzuzetkom(s);
    TestBrisanja(s);
    TestPraznog(s);
    return 0;
}