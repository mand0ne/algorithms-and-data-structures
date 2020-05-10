#include <iostream>
#include <memory>
#include <stdexcept>
#include <exception>
#include <cmath>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::logic_error;
using std::range_error;

template <typename Tip>
class Lista {
public:
	Lista() {}
	virtual ~Lista() {};
	virtual int brojElemenata() const =0;
	virtual Tip& trenutni() =0;
	virtual Tip trenutni () const =0;
	virtual bool prethodni() =0;
	virtual bool sljedeci() =0;
	virtual void pocetak()  =0;
	virtual void kraj() =0;
	virtual void obrisi() =0;
	virtual void dodajIspred(const Tip&)=0;
	virtual void dodajIza(const Tip&)=0;
	virtual Tip& operator[](int)=0;
	virtual Tip operator [] (int) const =0;
	virtual void oslobodi(int n = -1) {}
	virtual void Ispis() {}

};

template <typename Tip>
class JednostrukaLista : public Lista<Tip> {

	struct Cvor {
		Tip element;
		Cvor* sljedeci=nullptr;
		Cvor (const Tip &element, Cvor *sljedeci=nullptr):
			element(element), sljedeci(sljedeci) {}
			
		Cvor(Cvor* sljedeci=nullptr):sljedeci(sljedeci) {}
	};
	
	Cvor* fpocetak; // Fiktivni pocetak
	Cvor* pKraj;     // Pokazivac na prethodnika zadnjeg
	Cvor* tekuci;   // Pokazivac na prethodnika tekuceg
	int velicinaLijevo;
	int velicinaDesno;
	int velicina;

	void podesiVelicinu()
	{
		velicina=velicinaLijevo+velicinaDesno;
	}
	void Inicijaliziraj()
	{
		tekuci= pKraj = fpocetak = new Cvor;
		velicinaLijevo=velicinaDesno=velicina=0;
	}
public:

	JednostrukaLista()
	{
		Inicijaliziraj();
	}

	JednostrukaLista(const JednostrukaLista<Tip> &l)
	{
		Inicijaliziraj();
		if (l.velicina) {
			int pozicija{0};
			bool found{false};
			Cvor* p(l.fpocetak);
			while (p->sljedeci!=nullptr) {
				this->dodajIza(p->sljedeci->element);
				this->sljedeci();

				if(p==l.tekuci) found=true;
				if(!found) pozicija++;
				
				p=p->sljedeci;
			}
			tekuci=fpocetak;
			while(pozicija--) tekuci=tekuci->sljedeci;
			Cvor *q=fpocetak;
			while(q->sljedeci->sljedeci!=nullptr) q=q->sljedeci;
			pKraj=q;
		}
	}

	JednostrukaLista(JednostrukaLista<Tip> &&l)
	{
		velicina=l.velicina;
		velicinaDesno=l.velicinaDesno;
		velicinaLijevo=l.velicinaLijevo;
		fpocetak=l.fpocetak;
		pKraj=l.pKraj;
		tekuci=l.tekuci;
		l.fpocetak=nullptr;
		l.tekuci=nullptr;
		l.pKraj=nullptr;
	}

	JednostrukaLista &operator =(JednostrukaLista<Tip> &&l)
	{
		if (l.fpocetak!=fpocetak) {
			swap(tekuci, l.tekuci);
			swap(fpocetak, l.fpocetak);
			swap(pKraj,l.pKraj);
			velicina=l.velicina;
			velicinaDesno=l.velicinaDesno;
			velicinaLijevo=l.velicinaLijevo;
		}
		return *this;
	}

	JednostrukaLista &operator =(const JednostrukaLista<Tip> &l)
	{
		if (&l!=this) {
			this->oslobodi();

			JednostrukaLista p(l);
			*this=std::move(p);
		}
		return *this;
	}

	int brojElemenata() const override
	{
		return velicina;
	}

	Tip &trenutni() override
	{
		if(!velicina) throw logic_error("Prazna lista");
		return tekuci->sljedeci->element;
	}

	Tip trenutni() const override
	{
		if(!velicina) throw logic_error("Prazna lista");
		return tekuci->sljedeci->element;
	}

	bool prethodni() override
	{
		if (tekuci==fpocetak) return false;

		Cvor* p(tekuci);
		tekuci=fpocetak;
		while (tekuci->sljedeci!=p) tekuci=tekuci->sljedeci;

		velicinaLijevo--;
		velicinaDesno++;
		return true;
	}

	bool sljedeci() override
	{
		if(velicinaDesno>1) tekuci=tekuci->sljedeci;
		else return false;

		velicinaLijevo++;
		velicinaDesno--;
		return true;
	}

	void pocetak() override
	{
		if (!velicina) throw logic_error("Prazna lista");
		tekuci=fpocetak;
		velicinaDesno+=velicinaLijevo;
		velicinaLijevo=0;
	}

	void kraj() override
	{
		if (!velicina ) throw logic_error("Prazna lista");
		tekuci=pKraj;
		velicinaLijevo+=velicinaDesno-1;
		velicinaDesno=1;
	}

	void dodajIspred(const Tip &element) override
	{
		tekuci->sljedeci = new Cvor(element, tekuci->sljedeci);
		if (pKraj == tekuci && tekuci->sljedeci->sljedeci!=nullptr)
			pKraj = tekuci->sljedeci;

		if(velicinaDesno) tekuci=tekuci->sljedeci;
		else {
			velicinaDesno++;
			podesiVelicinu();
			return;
		}
		velicinaLijevo++;
		podesiVelicinu();
	}

	void dodajIza(const Tip &element) override
	{
		if(!velicina){
			tekuci->sljedeci = new Cvor(element,tekuci->sljedeci);}
		else{ if(tekuci->sljedeci!=nullptr)
			tekuci->sljedeci->sljedeci = new Cvor(element, tekuci->sljedeci->sljedeci);}

		if(pKraj->sljedeci!=nullptr && pKraj->sljedeci->sljedeci!=nullptr) pKraj=pKraj->sljedeci;
		velicinaDesno++;
		podesiVelicinu();
	}

	Tip operator [] (int i) const override
	{
		if (i<0 || i>=velicina) throw std::domain_error("Neispravan indeks");
		Cvor *p(tekuci);
		int q(0);
		if(i>=velicinaLijevo) 
			q = velicinaLijevo;
		else 
			p=fpocetak;

		for(int j=q; j<i; j++)
			p=p->sljedeci;
		return p->sljedeci->element;
	}

	Tip &operator [] (int i) override
	{
		if (i<0 || i>=velicina) throw std::domain_error("Neispravan indeks");
		Cvor *p(tekuci);
		int q(0);
		if(i>=velicinaLijevo) q = velicinaLijevo;
		else p=fpocetak;

		for(int j=q; j<i; j++)
			p=p->sljedeci;
		return p->sljedeci->element;
	}

	void obrisi() override
	{
		if (!velicina) throw logic_error("Prazna lista");
		if(velicina>1){
		Cvor* p = tekuci->sljedeci;
		tekuci->sljedeci = p->sljedeci;
		if (velicinaDesno>1 && pKraj == p) pKraj = tekuci;
		delete p;
		if(velicinaDesno<=1){
			tekuci=fpocetak;
			while(tekuci->sljedeci!=nullptr && tekuci->sljedeci->sljedeci!=nullptr) tekuci=tekuci->sljedeci;
			pKraj=tekuci;
			velicinaDesno=2;
			velicinaLijevo--;
			}
		}
		else{
			delete tekuci->sljedeci;
			tekuci->sljedeci=nullptr;
		}
		velicinaDesno--;
		podesiVelicinu();
	}

	void oslobodi()
	{
		while (fpocetak) {
			tekuci=fpocetak;
			fpocetak=fpocetak->sljedeci;
			delete tekuci;
		}
		tekuci=pKraj=nullptr;
	}

	~JednostrukaLista()
	{
		oslobodi();
	}
	
	void umetanje(JednostrukaLista())

};

template <typename Tip>
class NizLista : public Lista<Tip>{
private:
	int kapacitet=50;
	int velicina;
	int tekuciIndex;
	Tip ** L=nullptr;
	void podesi()
	{
		while(velicina>kapacitet) kapacitet*=2;
	}

public:
	NizLista()
	{
		velicina=0;
		tekuciIndex=0;
		L=nullptr;
	}

	NizLista(const NizLista<Tip> &l)
	{
		int i{};
		if(l.velicina) {
			try {
				velicina=l.velicina;
				podesi();
				L=new Tip*[kapacitet] {};
				for (i=0; i<velicina; i++)
					L[i]= new Tip(*l.L[i]);
				tekuciIndex=l.tekuciIndex;
			} catch(...) {
				if (L!=nullptr)
					this->oslobodi();
			}
		}
	}

	NizLista(NizLista<Tip> &&l)
	{
		if(l.velicina) {
			tekuciIndex=l.tekuciIndex;
			velicina=l.velicina;
			kapacitet=l.kapacitet;
			podesi();
			L=l.L;
			l.L=nullptr;
		}
	}

	NizLista &operator =(const NizLista<Tip> &l)
	{
		if (&l!=this && l.velicina) {
			if (L) this->oslobodi();
			try {
				velicina=l.velicina;
				podesi();
				L=new Tip*[kapacitet] {};
				for (int i=0; i<velicina; i++)
					L[i]=new Tip(*l.L[i]);
				tekuciIndex=l.tekuciIndex;
			} catch(...) {
				if (L) this->oslobodi();
			}
		}
		return *this;
	}

	NizLista &operator =(NizLista<Tip> &&l)
	{
		if (&l.L!=this) {
			tekuciIndex=l.tekuciIndex;
			velicina=l.velicina;
			kapacitet=l.kapacitet;
			podesi();
			swap(L, l.L);
		}
		return *this;
	}

	int brojElemenata() const override
	{
		return velicina;
	}

	Tip& trenutni() override
	{
		if(!velicina) throw logic_error("Prazna lista");
		return *L[tekuciIndex];
	}

	Tip trenutni() const override
	{
		if(!velicina) throw logic_error("Prazna lista");
		return *L[tekuciIndex];
	}

	bool prethodni() override
	{
		if(!velicina) throw logic_error("Prazna lista");
		if(tekuciIndex==0) return false;
		tekuciIndex--;
		return true;
	}

	bool sljedeci() override
	{
		if(!velicina) throw logic_error("Prazna lista");
		if(tekuciIndex==velicina-1) return false;
		tekuciIndex++;
		return true;
	}

	void pocetak() override
	{
		if(!velicina) throw logic_error("Prazna lista");
		tekuciIndex=0;
	}

	void kraj() override
	{
		if(!velicina) throw logic_error("Prazna lista");
		tekuciIndex=velicina-1;
	}

	void obrisi() override
	{
		if(!velicina) throw logic_error("Prazna lista");
		if(tekuciIndex==velicina-1) {
			tekuciIndex=velicina-2;
			delete L[velicina-1];
		}

		else {
			delete L[tekuciIndex];
			L[tekuciIndex]=nullptr;
			for(int i=tekuciIndex; i+1<velicina; i++)
				L[i]=L[i+1];
		}

		L[velicina-1]=nullptr;
		velicina--;
		if(!velicina) this->oslobodi();
	}

	void dodajIspred(const Tip& el) override
	{
		if(!velicina) {
			L = new Tip*[kapacitet] {};
			L[0]= new Tip(el);
			tekuciIndex=0;
			velicina++;
			return;
		}

		velicina++;

		if(velicina>kapacitet) {
			podesi();
			Tip **novi=nullptr;

			try {
				novi=new Tip*[kapacitet] {};
				for(int i=0; i<tekuciIndex && i<velicina-1; i++)
					novi[i]=new Tip(*L[i]);

				novi[tekuciIndex]= new Tip(el);

				for(int i=tekuciIndex+1; i<velicina; i++)
					novi[i]=new Tip(*L[i-1]);

				this->oslobodi(velicina-1);
				L=novi;
				novi=nullptr;
			}

			catch(std::exception &ex) {
				for(int j=0; j<kapacitet; j++) delete novi[j];
				delete[] novi;
				throw "Greska pri alokaciji";
			}
		} else {
			for(int i=velicina-1; i>tekuciIndex && i>0; i--)
				L[i]=L[i-1];


			L[tekuciIndex]= new Tip(el);
		}

		tekuciIndex++;

	}

	void dodajIza(const Tip& el) override
	{
		if(!velicina) {
			L = new Tip*[kapacitet];
			L[0]= new Tip(el);
			tekuciIndex=0;
			velicina++;
			return;
		}

		tekuciIndex++;
		this->dodajIspred(el);
		tekuciIndex-=2;
	}

	Tip& operator[](int index) override
	{
		if (index<0 || index>=velicina) throw std::domain_error("Neispravan indeks");
		return *L[index];
	}

	Tip operator[](int index) const override
	{
		if (index<0 || index>=velicina) throw std::domain_error("Neispravan indeks");
		return *L[index];
	}

	void oslobodi(int n=-1) override
	{
		if(n==-1) n=velicina;
		for(int j=0; j<n; j++) {
			delete L[j];
			L[j]=nullptr;
		}
		delete[] L;
		L=nullptr;

	}

	~NizLista()
	{
		this->oslobodi();
		velicina=0;
		tekuciIndex=0;
	}

	void Ispis()
	{
		for(int i=0; i<velicina; i++)
			cout << *L[i] << " ";
	}
};



template <typename Tip>
bool Isti(const Lista<Tip> &l1, const Lista<Tip> &l2){
	if(l1.brojElemenata()!=l2.brojElemenata()) return false;
	for(int i=0;i<l1.brojElemenata();i++)
	if(l1[i]!=l2[i]) return false;
	return true;
}

void TestKopirajucegKonstruktora() {
	srand(time(NULL));
	JednostrukaLista<int> l1;
    for (int i=0; i<55; i++) l1.dodajIspred(log(i)+(rand() % 100 + 1));
    try{
    JednostrukaLista<int> l2(l1);
    if (!Isti(l1,l2)) std::cout << "Test kopirajućeg konstruktora za JednostrukuListu: Greska" << std::endl;
    else std::cout << "Test kopirajućeg konstruktora za JednostrukuListu: OK" << std::endl;
    }
    catch(std::exception &ex){ std::cout << "Test kopirajućeg konstruktora za JednostrukuListu: Greska" << std::endl << ex.what() << endl;}
    
    
    NizLista<int> n1;
    for (int i=0; i<55; i++) n1.dodajIspred(log(i)+(rand() % 100 + 1));
    try{
    NizLista<int> n2(n1);
    if (!Isti(n1,n2)) std::cout << "Test kopirajućeg konstruktora za NizListu: Greska" << std::endl;
    else std::cout << "Test kopirajućeg konstruktora za NizListu: OK" << std::endl;
    }
    catch(std::exception &ex){ std::cout << "Test kopirajućeg konstruktora za NizListu: Greska" << std::endl << ex.what() << endl;}
}
void TestOperatoraDodjele() {
    NizLista<int> n1;
    for (int i=0; i<5; i++) n1.dodajIspred(i+(rand() % 1020 + 255));
    NizLista<int> n2;
    try{
    n2=n1;
    	if (!Isti(n1,n2)) std::cout << "Test operatora dodjele za NizListu: Greska" << std::endl;
    else std::cout << "Test operatora dodjele za NizListu: OK" << std::endl;
    }
    catch(std::exception &ex){ std::cout << "Test kopirajućeg konstruktora za NizListu: Greska" << std::endl << ex.what() << endl;}
    
    
    JednostrukaLista<int> l1;
    for (int i=0; i<5; i++) l1.dodajIspred(i+(rand() % 1020 + 255));
    JednostrukaLista<int> l2;
    try{
    l2=l1;
    	if (!Isti(l1,l2)) std::cout << "Test operatora dodjele za JednostrukuListu: Greska" << std::endl;
    else std::cout << "Test operatora dodjele za JednostrukuListu: OK" << std::endl;
    }
    catch(std::exception &ex){ std::cout << "Test kopirajućeg konstruktora za NizListu: Greska" << std::endl << ex.what() << endl;}
    
}
void TestBrojaElemenata(const Lista<int> &l, int n) {
    if (l.brojElemenata()!=n) std::cout << "Test broja elemenata: Greska" << std::endl;
    else std::cout << "Test broja elemenata: OK" << std::endl;
}
void TestTrenutnog(Lista<int> &l) {
    l.dodajIspred(1337);
    l.prethodni();
    if (l.trenutni()!=1337) std::cout << "Test trenutnog elementa: Greska" << std::endl;
    else std::cout << "Test trenutnog elemenata: OK" << std::endl;
}
void TestTrenutnogIzuzetak(Lista<int> &l) {
    while (l.brojElemenata()) l.obrisi();
    try {
        l.trenutni();
        std::cout << "Test trenutnog elemenata sa izuzetkom: Greska" << std::endl;
    }
    catch(std::logic_error &ex) {
        std::cout << "Test trenutnog elemenata sa izuzetkom: OK" << std::endl  << ex.what() << endl;
        return;
    }
}
void TestPrethodnog(Lista<int> &l) {
    l.dodajIza(213); l.dodajIza(0); l.sljedeci(); l.dodajIspred(4);
    l.prethodni();
    if (l.trenutni()!=4) std::cout << "Test prethodnog elementa: Greska" << std::endl;
    else std::cout << "Test prethodnog elemenata: OK" << std::endl;
}
void TestPrethodnogSaIzuzetkom(Lista<int> &l) {
    l.pocetak();
    if (l.prethodni()) std::cout << "Test prethodnog elementa sa izuzetkom: Greska" << std::endl;
    else std::cout << "Test prethodnog elemenata sa izuzetkom: OK" << std::endl;
}
void TestSljedeceg(Lista<int> &l) {
    l.dodajIspred(-32); l.dodajIza(-16);
    l.sljedeci();
    if (l.trenutni()!=-16) std::cout << "Test sljedeceg elementa: Greska" << std::endl;
    else std::cout << "Test sljedeceg elemenata: OK" << std::endl;
}
void TestSljedecegSaIzuzetkom(Lista<int> &l) {
    l.dodajIspred(-323); l.dodajIza(47);
    l.kraj();
    if (l.sljedeci()) std::cout << "Test sljedeceg elementa sa izuzetkom: Greska" << std::endl;
    else std::cout << "Test sljedeceg elemenata sa izuzetkom: OK" << std::endl;
}
void TestPocetka(Lista<int> &l) {
    while (l.prethodni());
    l.dodajIspred(16);
    l.pocetak();
    if (l.trenutni()!=16) std::cout << "Test pocetnog elementa: Greska" << std::endl;
    else std::cout << "Test pocetnog elementa: OK" << std::endl;
}
void TestKraja(Lista<int> &l) {
    while (l.sljedeci());
    l.dodajIza(-2321);
    l.kraj();
    if (l.trenutni()!=-2321) std::cout << "Test krajnjeg elementa: Greska" << std::endl;
    else std::cout << "Test krajnjeg elementa: OK" << std::endl;
}
void TestBrisanja(Lista<int> &l) {
	int x = l.trenutni()-343;
	l.obrisi();
    l.dodajIspred(7);
    l.dodajIza(8);
    if (l.trenutni()==x+343) std::cout << "Test brisanja elementa: Greska" << std::endl;
    else std::cout << "Test brisanja elementa: OK" << std::endl;
}
void TestBrisanjaZadnjeg(Lista<int> &l) {
	l.kraj();
	l.obrisi();
    l.dodajIza(-90);
    l.kraj();
    if (l.trenutni()!=-90) std::cout << "Test brisanja zadnjeg elementa: Greska" << std::endl;
    else std::cout << "Test brisanja zadnjeg elementa: OK" << std::endl;
}
void TestDodajIspred(Lista<int> &l) {
    l.dodajIspred(1); l.dodajIspred(-3);
    l.prethodni();
    if (l.trenutni()!=-3) std::cout << "Test dodavanja elementa ispred: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa ispred: OK" << std::endl;
}
void TestDodajIspredPrazna(Lista<int> &l) {
    while (l.brojElemenata()) l.obrisi();
    l.dodajIspred(-75);
    l.kraj();
    if (l.trenutni()!=-75) std::cout << "Test dodavanja elementa ispred na praznu listu: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa ispred na praznu listu: OK" << std::endl;
}
void TestDodajIza(Lista<int> &l) {
    l.dodajIza(900); l.dodajIza(314);
    l.sljedeci();
    if (l.trenutni()!=314) std::cout << "Test dodavanja elementa iza: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa iza: OK" << std::endl;
}
void TestDodajIzaPrazna(Lista<int> &l) {
    while (l.brojElemenata()) l.obrisi();
    l.dodajIza(57);
    if (l.trenutni()!=57) std::cout << "Test dodavanja elementa iza na praznu listu: Greska" << std::endl;
    else std::cout << "Test dodavanja elementa iza na praznu listu: OK" << std::endl;
}
void TestOperatora(Lista<int> &l) {
    l.dodajIza(17);
    l.pocetak();
    l.dodajIza(1);
    l.dodajIza(-43);
    if (l[1]!=-43) std::cout << "Test operatora []: Greska" << std::endl;
    else std::cout << "Test operatora []: OK" << std::endl;
}
void TestOperatoraVanOpsega(Lista<int> &l) {
    while (l.brojElemenata()!=0) l.obrisi();
    l.dodajIza(66); l.dodajIza(33);
    try {
        l[3];
        std::cout << "Test operatora [] van opsega: Greska" << std::endl;
    }
    catch(std::exception &ex) {
        std::cout << "Test operatora [] van opsega: OK : " << ex.what() << std::endl;
        return;
    }
}


void permutation(string prefix, string str) {
    int n = str.size();
    if (n == 0) 
    	cout << prefix << endl;
    else {
        for (int i = 0; i < n; i++){
        	cout << str.substr(0,i)  << endl;
        string s2 = str.substr(0, i) + str.substr(i+1, n);
        	cout << "Substting: " << s2 << endl;
        	cout << "Prefix: " << prefix + str.at(i) << endl;
        	
            permutation(prefix + str.at(i), s2 );
    }}
}

int main() {
	permutation("", "ABC");
	
	string s = "TEST";
	string s2 = s.substr(0,3);
	cout << endl << s2;
    return 0;
}
