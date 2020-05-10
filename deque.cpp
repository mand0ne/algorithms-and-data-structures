#include <iostream>
#include <stdexcept>
#include <limits>
#include <exception>

using namespace std;

#include <iostream>
#include <stdexcept>

using namespace std;

template <typename Tip>
class DvostraniRed
{
private:
    struct Node {
        Tip element{};
        Node *prethodni=nullptr; 
        Node *sljedeci=nullptr; 


        Node (const Tip &element, Node* prevp, Node *nextp=nullptr):
            element(element), prethodni(prevp),sljedeci(nextp)
        {
            if (prethodni) prethodni->sljedeci=this;
            if (sljedeci) sljedeci->prethodni=this;
        }

        Node(Node* prevp=nullptr, Node* nextp=nullptr):prethodni(prevp),sljedeci(nextp)
        {
            if (prethodni) prethodni->sljedeci=this;
            if (sljedeci) sljedeci->prethodni=this;
        }
    };

    Node* fictionalFirst; // Fiktivni pocetak
    Node* fictionalLast;   //  Fiktivni kraj
    Node* current; // Prethodni od tekuceg
    int velicina=0;

    void Copy (const DvostraniRed &r)
    {
        Node *p(r.fictionalFirst->sljedeci);
        while(p!=r.fictionalLast) {
            staviNaVrh(p->element);
            p=p->sljedeci;
        }
    }

    void Initialize()
    {
        current=fictionalFirst=new Node(nullptr,nullptr);
        fictionalFirst->sljedeci = fictionalLast = new Node(fictionalFirst,nullptr);
    }
    
    Tip &trenutni()
    {
        if(!velicina) throw logic_error("Prazan red");
        return current->sljedeci->element;
    }

    Tip trenutni() const
    {
        if(!velicina) throw logic_error("Prazan red");
        return current->sljedeci->element;
    }

    bool prethodni()
    {
        if (current != fictionalFirst) {
            current = current->prethodni;
            return true;
        }
        return false;
    }

    bool sljedeci()
    {
        if(current!=fictionalLast->prethodni->prethodni) current=current->sljedeci;
        else return false;
        return true;
    }

    void pocetak()
    {
        if(!velicina) throw logic_error("Prazan red!");
        current=fictionalFirst;
    }

    void kraj()
    {
        if(!velicina) throw logic_error("Prazan red!");
        current=fictionalLast->prethodni->prethodni;
    }

    void obrisi()
    {
        if(!velicina) throw logic_error("Prazan red");
        Node* temp = current->sljedeci;
        temp->sljedeci->prethodni = current;
        current->sljedeci = temp->sljedeci;
        delete temp;
        velicina--;
    }

public:

    DvostraniRed()
    {
        Initialize();
    }
    
    ~DvostraniRed()
    {
        brisi();
        delete fictionalFirst; delete fictionalLast;
        fictionalFirst=fictionalLast=current=nullptr;
    }

    DvostraniRed (const DvostraniRed &r)
    {
        Initialize();
        Copy(r);
    }

    DvostraniRed (DvostraniRed &&r)
    {
        velicina=r.velicina;
        fictionalFirst=r.fictionalFirst;
        fictionalLast=r.fictionalLast;
        current=r.current;
        r.fictionalFirst=r.fictionalLast=r.current=nullptr;
        r.velicina=0;
    }

    DvostraniRed &operator =(const DvostraniRed &r)
    {
        if (&r!=this) {
            brisi();
            Copy(r);
        }
        return *this;
    }

    DvostraniRed &operator =(DvostraniRed &&r)
    {
        if (&r != this) {
            std::swap(fictionalFirst,r.fictionalFirst);
            std::swap(fictionalLast,r.fictionalLast);
            std::swap(current,r.current);
            velicina=r.velicina;
            r.velicina=0;
        }
        return *this;
    }
    
    void staviNaVrh (const Tip &element)
    {
        if(velicina) {
            this->kraj();
            new Node(element,current->sljedeci,fictionalLast); // dodaj iza zadnjeg
        } else new Node(element, fictionalFirst, fictionalFirst->sljedeci); // dodaje se prvi element

        velicina++;
    }

    void staviNaCelo (const Tip &element)
    {
        new Node(element, fictionalFirst, fictionalFirst->sljedeci);
        velicina++;
    }

    Tip skiniSaVrha()
    {
        this->kraj();
        Tip x = trenutni();
        this->obrisi();
        return x;
    }

    Tip skiniSaCela()
    {
        this->pocetak();
        Tip x = trenutni();
        this->obrisi();
        return x;
    }

    Tip& celo() const
    {
        if (!velicina) throw std::logic_error("Prazan red!");
        return fictionalFirst->sljedeci->element;
    }

    Tip& vrh() const
    {
        if (!velicina) throw std::logic_error("Prazan red!");
        return fictionalLast->prethodni->element;
    }

    int brojElemenata() const
    {
        return velicina;
    }

    void brisi()
    {
        while(brojElemenata()!=0) this->skiniSaCela();
    }
    
    template<typename Tip2>
    friend bool areSame(DvostraniRed<Tip2> dp1, DvostraniRed<Tip2> dp2);
};


template<typename Tip>
bool areSame(DvostraniRed<Tip> dp1, DvostraniRed<Tip> dp2){
	if(dp1.brojElemenata()!=dp2.brojElemenata()) return false;
	dp1.pocetak(); dp2.pocetak();
	while(dp1.brojElemenata()!=0){
		if(std::abs(dp1.trenutni()-dp2.trenutni())<std::numeric_limits<Tip>::epsilon()) return false;
		dp1.obrisi(); dp2.obrisi();
	}
	return true;
}
void constructorTest(){
	srand(time(NULL));
	DvostraniRed<int> l1;
    for (int i=0; i<3; i++) l1.staviNaCelo(i+(rand()%100+1));
    DvostraniRed<int> l2(l1);
    if (areSame(l1,l2)) cout << "Copy constructor: Good" << endl;
    else cout << "Copy constructor: Error" << endl;
    DvostraniRed<int> l3(l2);
    if (areSame(l1,l3)) cout << "Move constructor: Good" << endl;
    else cout << "Move constructor: Error" << endl;
    
}
void assignmentOperatorsTest(){
	DvostraniRed<int> l1;
    for (int i=0; i<10; i++) l1.staviNaCelo(i+i*i*i);
    DvostraniRed<int> l2;
    for (int i=0; i<20; i++) l2.staviNaCelo(i*(rand()%200+100));
    DvostraniRed<int> l3;
    l3=l1;
    if(areSame(l1,l3)) cout << "Assignment operator: Good" << endl;
    else cout << "Assignment operator: Error" << endl;
    
	l1=l2;
	l3=std::move(l2);
	if(areSame(l1,l3)) cout << "Assignment operator (move): Good" << endl;
    else cout << "Assignment operator (move): Error" << endl;
}
void testiranjeCelo(){
     DvostraniRed<int> r;
    for (int i=0; i<10; i++) r.staviNaVrh(i+(rand()%100+1));
    r.staviNaCelo(1337);
    if(r.celo()==1337) cout << "Testiranje cela 1: Good" << endl;
    else cout << "Testiranje cela 1: Error" << endl;
    
    r.staviNaCelo(98);
    if(r.skiniSaCela()==98) cout << "Testiranje cela 2: Good" << endl;
    else cout << "Testiranje cela 2: Error" << endl;
    
}
void testiranjeVrha(){
    DvostraniRed<int> r;
    for (int i=0; i<10; i++) r.staviNaVrh(i+(rand()%100+1));
    r.staviNaVrh(1337);
    if(r.vrh()==1337) cout << "Testiranje vrha 1: Good" << endl;
    else cout << "Testiranje vrha 1: Error" << endl;
    
    r.staviNaVrh(98);
    if(r.skiniSaVrha()==98) cout << "Testiranje vrha 2: Good" << endl;
    else cout << "Testiranje vrha 2: Error" << endl;
    
}

int main() {
    constructorTest();
    assignmentOperatorsTest();
    testiranjeVrha();
    testiranjeCelo();
    return 0;
}