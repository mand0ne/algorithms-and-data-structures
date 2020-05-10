#include <iostream>
#include <memory>
#include <stdexcept>
#include <exception>
#include <cmath>
#include <limits>
using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::logic_error;
using std::range_error;

template <typename Tip>
class Lista{
public:
	Lista() {}
	virtual ~Lista() {};
	virtual int brojElemenata() const =0;
	virtual Tip& trenutni() =0;
	virtual Tip trenutni() const =0;
	virtual bool prethodni() =0;
	virtual bool sljedeci() =0;
	virtual void pocetak()  =0;
	virtual void kraj() =0;
	virtual void obrisi() =0;
	virtual void dodajIspred(const Tip&)=0;
	virtual void dodajIza(const Tip&)=0;
	virtual Tip& operator[](int)=0;
	virtual Tip operator [] (int) const =0;
	virtual void oslobodi(){}
	
};



template <typename Tip>
class DvostrukaLista : public Lista<Tip>{
private:
	struct Node{
		Tip data{};
		Node *previous=nullptr;
		Node *next=nullptr;
		
		Node (const Tip &data, Node* cPrevious, Node * cNext=nullptr):
			data(data), previous(cPrevious),next(cNext) {
			if (previous) previous->next=this;
			if (next) next->previous=this;
		}

		Node(Node* cPrevious=nullptr, Node* cNext=nullptr):previous(cPrevious),next(cNext) {
			if (previous) previous->next=this;
			if (next) next->previous=this;
		}
	};
	
	Node* fictionalFirst; // Fiktivni pocetak
	Node* fictionalLast;   //  Fiktivni kraj
	Node* current; // Ovo je zapravo previous od tekuceg, laksa implementacija
	int size;
	int sizeToLeft;  // Zamisljamo kursor da se nalazi prije tekuceg
	int sizeToRight;  // sizeToLeft - broj elemena lijevo od kursora, sizeToRight-desno

	void modifySize(){
		size=sizeToLeft+sizeToRight;
	}
	void Initialize(){
		current=fictionalFirst=new Node(nullptr,nullptr);
		fictionalFirst->next = fictionalLast = new Node(fictionalFirst,nullptr);
		size=sizeToLeft=sizeToRight=0;
	}
public:
	DvostrukaLista(){
		Initialize();
	}
	DvostrukaLista(const DvostrukaLista<Tip> &l){
		Initialize();
		if (l.size) {
			int index{0};
			bool found{false};
			Node* p(l.fictionalFirst);
			while (p->next!=l.fictionalLast) {
				this->dodajIza(p->next->data);
				this->sljedeci();

				if(p==l.current) found=true;
				if(!found) index++;
				
				p=p->next;
			}
			current=fictionalFirst;
			while(index--) current=current->next; 
			if(current->next==fictionalLast && current!=fictionalFirst) current=current->previous;
		}
	}
	DvostrukaLista(DvostrukaLista<Tip> &&l){
		size=l.size;
		sizeToRight=l.sizeToRight;
		sizeToLeft=l.sizeToLeft;
		fictionalFirst=l.fictionalFirst;
		fictionalLast=l.fictionalLast;
		current=l.current;
		if(current->next==fictionalLast && current!=fictionalFirst) current=current->previous;
		l.fictionalFirst=l.fictionalLast=l.current=nullptr;
	}
	DvostrukaLista &operator =(DvostrukaLista<Tip> &&l){
		if (l.fictionalFirst!=fictionalFirst) {
			std::swap(fictionalFirst,l.fictionalFirst);
			std::swap(fictionalLast,l.fictionalLast);
			std::swap(current,l.current);
			if(current->next==fictionalLast && current!=fictionalFirst) current=current->previous;
			size=l.size;
			sizeToRight=l.sizeToRight;
			sizeToLeft=l.sizeToLeft;
		}
		return *this;
	}
	DvostrukaLista &operator =(const DvostrukaLista<Tip> &l){
		if (l.fictionalFirst!=fictionalFirst) {
			DvostrukaLista p(l);
			this->oslobodi();
			*this=std::move(p);
		}
		return *this;
	}
	int brojElemenata() const override{
		return size;
	}
	Tip &trenutni() override{
		if(!size) throw logic_error("Prazna lista");
		return current->next->data;
	}
	Tip trenutni() const override{
		if(!size) throw logic_error("Prazna lista");
		return current->next->data;
	}
	bool prethodni() override{
		if (current != fictionalFirst) {
			current = current->previous;
			sizeToLeft--;
			sizeToRight++;
			return true;
		}
		return false;
	}
	bool sljedeci() override{
		if(sizeToRight>1) current=current->next;
		else return false;
		sizeToRight--;
		sizeToLeft++;
		return true;
	}
	void pocetak() override{
		if(!size) throw logic_error("Prazna lista!");
		current=fictionalFirst;
		sizeToRight+=sizeToLeft;
		sizeToLeft=0;
	}
	void kraj() override{
		if(!size) throw logic_error("Prazna lista!");
		current=fictionalLast->previous->previous;
		sizeToLeft+=sizeToRight-1;
		sizeToRight=1;
	}
	void dodajIspred(const Tip &data) override{
		new Node(data, current, current->next);
		if(sizeToRight) current=current->next;
		else {
			sizeToRight++;
			modifySize();
			return;
		}
		sizeToLeft++;
		modifySize();
	}
	void dodajIza(const Tip &data) override{
		if(!size)
			new Node(data, current, current->next);
		else
			new Node(data,current->next,current->next->next);
			
		sizeToRight++;
		modifySize();
	}
	Tip &operator [] (int i) override{
		Node *q=current;
		if (i<0 || i>=size)
			throw std::domain_error("Ilegalan index");

		int k{};
		if (i < abs(i-sizeToLeft) && i<size/2) {
			k=i;
			q = fictionalFirst;
		} else if (abs(i-sizeToLeft) < size-i)
			k=i-sizeToLeft;
		else {
			k = i-size-1;
			q = fictionalLast;
		}
		if (k<0) {
			for (int i = 0; i < abs(k); i++)
				q = q->previous;
		} else {
			for (int i=0; i < k; i++)
				q = q->next;
		}

		return q->next->data;
	}
	
	Tip operator [] (int i) const override{
		Node *q=current;
		if (i<0 || i>=size)
			throw std::domain_error("Ilegalan index");

		int k{};
		if (i < abs(i-sizeToLeft) && i<size/2) {
			k=i;
			q = fictionalFirst;
		} else if (abs(i-sizeToLeft) < size-i)
			k=i-sizeToLeft;
		else {
			k = i-size-1;
			q = fictionalLast;
		}
		if (k<0) {
			for (int i = 0; i < abs(k); i++)
				q = q->previous;
		} else {
			for (int i=0; i < k; i++)
				q = q->next;
		}

		return q->next->data;
	}
	
	void obrisi() override{
		if(!size) throw logic_error("Prazna lista");
		Node* temp = current->next;
		temp->next->previous = current;
		current->next = temp->next;
		delete temp;
		sizeToRight--;
		if(current->next==fictionalLast && current!=fictionalFirst) {
			current=current->previous;
			sizeToRight++;
			sizeToLeft--;
		}
		modifySize();
	}
	void oslobodi() override{
		while(fictionalFirst) {
			current=fictionalFirst;
			fictionalFirst=fictionalFirst->next;
			delete current;
		}
		current=fictionalLast=nullptr;
		size=0;
		sizeToLeft=0;
		sizeToRight=0;
		modifySize();
	}
	~DvostrukaLista(){
		oslobodi();
	}
};

template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n){
	DvostrukaLista<Tip> dp(static_cast<DvostrukaLista<Tip>&>(const_cast<Lista<Tip>&>(n)));
	dp.pocetak();
	Tip x{dp.trenutni()};
	dp.sljedeci();
	for(int i=0; i<dp.brojElemenata(); i++) {
		if(x<dp.trenutni()) 
			x=dp.trenutni();
		dp.sljedeci();
	}
	return x;
}

template<typename Tip>
bool areSame(DvostrukaLista<Tip> dp1, DvostrukaLista<Tip> dp2){
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
	DvostrukaLista<int> l;
    if (l.brojElemenata()!=0) cout << "Default no argument constructor: Error" << endl;
    else cout << "Default no argument constructor: Good" << endl;

	DvostrukaLista<int> l1;
    for (int i=0; i<10; i++) l1.dodajIspred(i+(rand()%100+1));
    DvostrukaLista<int> l2(l1);
    if (areSame(l1,l2)) cout << "Copy constructor: Good" << endl;
    else cout << "Copy constructor: Error" << endl;
    
    DvostrukaLista<int> l3(std::move(l2));
    if (areSame(l1,l3)) cout << "Move constructor: Good" << endl;
    else cout << "Move constructor: Error" << endl;
}
void assignmentOperatorsTest(){
	DvostrukaLista<int> l1;
    for (int i=0; i<10; i++) l1.dodajIspred(i+i*i*i);
    DvostrukaLista<int> l2;
    for (int i=0; i<20; i++) l2.dodajIspred(i*(rand()%200+100));
    DvostrukaLista<int> l3;
    l3=l1;
    if(areSame(l1,l3)) cout << "Assignment operator: Good" << endl;
    else cout << "Assignment operator: Error" << endl;
    
	l1=l2;
	l3=std::move(l2);
	if(areSame(l1,l3)) cout << "Assignment operator (move): Good" << endl;
    else cout << "Assignment operator (move): Error" << endl;
}
void currentElementTest(Lista<int> &l) {
    int x=l.trenutni();
    l.dodajIspred(26);  l.prethodni(); l.obrisi();
    if (l.trenutni()==x) cout << "Current element: Good" << endl;
    else cout << "Current element: Error" << endl;

	while (l.brojElemenata()) l.obrisi();
    try {
        l.trenutni();
    }
    catch(std::logic_error &c) {
        cout << "Current element (empty set) : Good" << endl;
        return;
    }
    catch(...) {}
    cout << "Current element (empty set) : Error" << endl;
}
void previousElementTest(Lista<int> &l) {
    l.dodajIza(11); l.dodajIspred(4); l.dodajIspred(300);
    l.prethodni(); l.prethodni();
    if (l.trenutni()!=4) cout << "Previous element test: Error" << endl;
    else cout << "Previous element test: Good" << endl;
    
    l.pocetak();
    if(l.prethodni()) cout << "Previous element test (exception) : Error" << endl;
    else cout << "Previous element test: Good" << endl;
}
void nextElementTest(Lista<int> &l) {
    l.dodajIza(9); l.dodajIspred(7); l.dodajIza(11);
    l.sljedeci();
    if (l.trenutni()!=11) cout << "Next element test: Error" << endl;
    else cout << "Next element test: Good" << endl;
    
    l.kraj();
    if(l.sljedeci()) cout << "Next element test (exception) : Error" << endl;
    else cout << "Next element test: Good" << endl;
}
void firstElementTest(Lista<int> &l) {
    while (l.prethodni());
    l.dodajIspred(10); l.dodajIspred(1231); l.dodajIza(14);
    l.pocetak();
    if (l.trenutni()!=10) cout << "First element test: Error" << endl;
    else cout << "First element test: Good" << endl;
}
void lastElementTest(Lista<int> &l) {
	l.dodajIspred(120); l.dodajIspred(33); l.dodajIza(78);
    l.kraj();
    l.dodajIza(99);
    l.kraj();
    if (l.trenutni()!=99) cout << "Last element test: Error" << endl;
    else cout << "Last element test: Good" << endl;
}
void deleteTest(Lista<int> &l) {
    l.dodajIspred(1); l.dodajIspred(2312); l.dodajIza(1337);
    l.obrisi();
    if (l.trenutni()!=1337) cout << "Delete test: Error" << endl;
    else cout << "Delete test: Good" << endl;
}
void addBeforeTest(Lista<int> &l) {
    l.dodajIspred(37); l.dodajIspred(23);
    l.prethodni(); l.prethodni();
    if (l.trenutni()!=37) cout << "Add before test: Error" << endl;
    else cout << "Add before test: Good" << endl;
}
void addAfterTest(Lista<int> &l) {
	l.pocetak();
    l.dodajIza(23); l.dodajIza(37); l.dodajIza(4); l.dodajIza(9);
    l.sljedeci(); l.sljedeci();
    if (l.trenutni()!=4) cout << "Add after test: Error" << endl;
    else cout << "Add after test: Good" << endl;
}
void indexOperatorTest(Lista<int> &l) {
    l.kraj();
    l.dodajIza(5435);
    if (l[l.brojElemenata()-1]!=5435) cout << "[] Operator test: Error" << endl;
    else cout << "[] Operator test: Good" << endl;
    
     while (l.brojElemenata()!=0)
    	l.obrisi();
    
    try {
        l[2];
    }
    catch(std::domain_error) {
        cout << "[] Operator test (out of range): Good" << endl;
        return;
    }
    cout << "[] Operator test (out_of_range): Error" << endl;
}
void dajMaksimumTest(Lista<int> &l) {
    while (l.brojElemenata()!=0) l.obrisi();
    
    for(int i=0;i<50;i++) l.dodajIspred(i+10000);
    if (dajMaksimum(l)!=10049) cout << "Test dajMaksimum: Error" << endl;
    else cout << "Test dajMaksimum: Good" << endl;
    
}

int main(){
	constructorTest();
	assignmentOperatorsTest();
	srand(time(NULL));
	DvostrukaLista<int> dp;
	for(int i=0;i<50;i++) dp.dodajIspred(i*(rand()%100+1));
	currentElementTest(dp);
	previousElementTest(dp);
	nextElementTest(dp);
	firstElementTest(dp);
	lastElementTest(dp);
	deleteTest(dp);
	addBeforeTest(dp);
	addAfterTest(dp);
	indexOperatorTest(dp);
	dajMaksimumTest(dp);
	return 0;
}