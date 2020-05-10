#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
struct Pair {
    TipKljuca key;
    TipVrijednosti value;
};

template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
    public:
        Mapa() {};
        virtual ~Mapa(){};
        virtual int brojElemenata() const = 0;
        virtual TipVrijednosti &operator[] (TipKljuca key) = 0;
        virtual TipVrijednosti operator [] (TipKljuca key) const = 0;
        virtual void obrisi() = 0;
        virtual void obrisi(const TipKljuca &key) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca,TipVrijednosti> {
    
    private:
        Pair<TipKljuca,TipVrijednosti>* map;
        int size=0;
        int capacity=50;
        
        void adjust(){ 
            while(size >= capacity) 
                capacity*=2;
        }
        
    public:
        NizMapa() : size(0) { 
            map = new Pair<TipKljuca,TipVrijednosti>[capacity]; 
        }
        
        NizMapa(const NizMapa &nMap) {
            size = nMap.size;
            capacity = nMap.capacity;
            adjust();
            map = new Pair<TipKljuca,TipVrijednosti>[capacity];
            for(int i(0); i<size; i++)
                map[i]=nMap.map[i];
        }
        
        NizMapa(NizMapa &&nMap) {
            size=nMap.size;
            capacity=nMap.capacity;
            map = nMap.map;
            adjust();
            nMap.size=nMap.capacity=0;
            nMap.map=nullptr;
        }
        
        NizMapa &operator=(const NizMapa &nMap) {
            if(this!=&nMap){
                delete[] map;
                size=nMap.size;
                capacity=nMap.capacity;
                adjust();
                map=new Pair<TipKljuca,TipVrijednosti>[capacity];
                for(int i(0); i<size; i++)
                    map[i]=nMap.map[i];
            }
            return *this;
        }
        
        NizMapa &operator=(NizMapa &&nMap) {
            if(this!=&nMap){
                size=nMap.size;
                capacity=nMap.capacity;
                adjust();
                swap(map,nMap.map);
                nMap.size=nMap.capacity=0;
            }
            return *this;  
        }
        
        int brojElemenata() const { 
            return size;
        }
        
        TipVrijednosti &operator[] (TipKljuca key){
            for(int i(0);i<size;i++)
                if(key==map[i].key) 
                    return map[i].value;
            
            
            if(size>=capacity) {
                adjust();
                Pair<TipKljuca,TipVrijednosti>* temporary = new Pair<TipKljuca,TipVrijednosti>[capacity];
                for(int i(0); i<size; i++)
                    temporary[i]=map[i];
                delete[] map;
                map=temporary;
            }

            size++;
            Pair<TipKljuca,TipVrijednosti> temporary;
            temporary.key=key;
            temporary.value=TipVrijednosti();
            map[size-1]=temporary;
            return map[size-1].value;
        }
        
        TipVrijednosti operator [] (TipKljuca key) const {
            for(int i(0);i<size;i++)
                if(key==map[i].key) return map[i].value;
            return TipVrijednosti();      
        }
        
        void obrisi() {
            delete[] map; 
            size=0; 
            capacity=50; 
            map=new Pair<TipKljuca,TipVrijednosti>[capacity]; 
        }
        
        void obrisi(const TipKljuca &key) {
            for(int i(0);i<size;i++)
                if(map[i].key==key) {
                    for(int j(i);j<size-1;j++)
                        map[j]=map[j+1];
                    size--;
                    break;
                } 
        }
        
        ~NizMapa() override {
            delete[] map;
            map=nullptr;
        } 
        
        template<typename KeyType2, typename ValueType2>
        friend bool areSame(const NizMapa<KeyType2,ValueType2> &m1,const NizMapa<KeyType2,ValueType2> &m2);
};

template <typename TipKljuca, typename TipVrijednosti>
struct BSMCvor{
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    BSMCvor * lijevo,*desno,*roditelj;
    
    BSMCvor(){ 
        kljuc=TipKljuca();
        vrijednost=TipVrijednosti();
        lijevo=desno=roditelj=nullptr;
    }
    
    BSMCvor(const TipKljuca& kljuc, const TipVrijednosti& vrijednost, BSMCvor<TipKljuca,TipVrijednosti>* roditelj = nullptr, 
                                                                        BSMCvor<TipKljuca,TipVrijednosti>* lijevo = nullptr, BSMCvor<TipKljuca,TipVrijednosti>* desno = nullptr) : 
            kljuc(kljuc), vrijednost(vrijednost), roditelj(roditelj), lijevo(lijevo), desno(desno) {}
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa : public Mapa<TipKljuca,TipVrijednosti> {
        BSMCvor<TipKljuca,TipVrijednosti> * korijen;
        int broj;
    
        void umetniBSMCvor(BSMCvor<TipKljuca,TipVrijednosti> *novi){
            
            BSMCvor<TipKljuca,TipVrijednosti> *x=korijen, *y=nullptr;
            
            while(x!=nullptr) {
                y=x;
                if(novi->kljuc < x->kljuc) 
                    x=x->lijevo;
                else 
                    x=x->desno;
            }
            
            if(y==nullptr) 
                korijen=novi;
            
            else {
                if(novi->kljuc < y->kljuc) 
                    y->lijevo=novi;
                else 
                    y->desno=novi;
                
                novi->roditelj=y;
             }
        }
        
        void preorderCopy(BSMCvor<TipKljuca,TipVrijednosti> *stari, BSMCvor<TipKljuca,TipVrijednosti> *&novi, BSMCvor<TipKljuca,TipVrijednosti> *roditelj=nullptr){
            if(stari){
                novi = new BSMCvor<TipKljuca,TipVrijednosti>(stari->kljuc,stari->vrijednost,roditelj);
                preorderCopy(stari->lijevo,novi->lijevo, novi);
                preorderCopy(stari->desno,novi->desno, novi);
            }
        }
    
    public:
        BinStabloMapa() : korijen(nullptr), broj(0) {}
        ~BinStabloMapa() { 
            this->obrisi(); 
        }
        
        BinStabloMapa(const BinStabloMapa& bsm) : korijen(nullptr), broj(bsm.broj){
            preorderCopy(bsm.korijen,korijen);
        }

        BinStabloMapa(const BinStabloMapa&& bsm) : korijen(bsm.korijen), broj(bsm.broj){
            bsm.korijen = nullptr;
        }
        
        BinStabloMapa &operator=(const BinStabloMapa &bsm){
            if(this!=&bsm){
                this->obrisi();
                korijen=nullptr;
                broj=bsm.broj;
                preorderCopy(bsm.korijen,korijen);
            }
            return *this;
        }
        
        BinStabloMapa& operator=(BinStabloMapa &&bsm){
            if(this!=&bsm){
                this->obrisi();
                korijen=bsm.korijen;
                broj=bsm.broj;
                bsm.korijen=nullptr;
            }
            return *this;
        }
        
        
        int brojElemenata() const { 
            return broj; 
        }
        
        TipVrijednosti operator [] (TipKljuca kljuc) const {
            BSMCvor<TipKljuca,TipVrijednosti>* temp=korijen;
            
            while(temp && kljuc!=temp->kljuc) {
                if(kljuc<temp->kljuc) 
                    temp=temp->lijevo;
                else 
                    temp=temp->desno;
            }
            
            if(!temp) 
                return TipVrijednosti();
            
            return temp->vrijednost;
        }
        
        TipVrijednosti &operator[] (TipKljuca kljuc){
            
            BSMCvor<TipKljuca,TipVrijednosti>* temp=korijen;
            
            while(temp!=nullptr && kljuc!=temp->kljuc) {
                if(kljuc<temp->kljuc) 
                    temp=temp->lijevo;
                else 
                    temp=temp->desno;
            }
            
            if(temp==nullptr){
                temp = new BSMCvor<TipKljuca,TipVrijednosti>();
                broj++;
                temp->kljuc=kljuc;
                temp->vrijednost=TipVrijednosti();
                umetniBSMCvor(temp);
            }
            
            return temp->vrijednost;
        }
        
        void obrisi(const TipKljuca &kljuc){
            
            BSMCvor<TipKljuca,TipVrijednosti>* p=korijen, *roditelj=nullptr;
            
            while(p!=nullptr && kljuc!=p->kljuc) {
                roditelj=p;
                if(kljuc<p->kljuc) 
                    p=p->lijevo;
                else 
                    p=p->desno;
            }
            
            if(p==nullptr) 
                return;
                
            BSMCvor<TipKljuca,TipVrijednosti>* m;
            
            if(p->lijevo==nullptr) 
                m=p->desno;
                
            else {
                if(p->desno==nullptr) 
                    m=p->lijevo;
                else {
                    BSMCvor<TipKljuca,TipVrijednosti>* pm=p, *tmp;
                    m=p->lijevo;
                    tmp=m->desno;
                    while(tmp!=nullptr) {
                        pm=m;
                        m=tmp;
                        tmp=m->desno;
                    }
                    if(pm!=p) {
                        pm->desno=m->lijevo;
                        m->lijevo=p->lijevo;
                    }
                    m->desno=p->desno;
                }
            }    
            
            if(roditelj==nullptr) 
                korijen=m;
                
            else {
                if(p==roditelj->lijevo) 
                    roditelj->lijevo=m;
                else
                    roditelj->desno=m;
            }
            
            delete p;
            p=nullptr;
            broj--;
        }
        
        void obrisi(){
            while(korijen)
                obrisi(korijen->kljuc);
        }
};

double averageTime(clock_t t1, clock_t t2, int size) {
    return ((t2 - t1) / (CLOCKS_PER_SEC / 1000.)) / size;
}


int main(){
    NizMapa<int, int> arrayMap;
    BinStabloMapa<int, int> binarySearchTreeMap;
    
    srand(time(NULL));
    
    vector<int> randomKeys;
    int size = 10000;
    for(int i = 0; i < size; i++)
        randomKeys.push_back(rand());
        
    clock_t start = clock();
    for(auto rk : randomKeys)
        arrayMap[rk];
    clock_t finish = clock();
    cout << "Prosjecno vrijeme umetanja u mapu implementirane preko nizova: " << averageTime(start, finish, size) << "ms" << endl;
    
    start = clock();
    for(auto rk : randomKeys)
        binarySearchTreeMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme umetanja u mapu implementirane binarnim stablom pretrage: " << averageTime(start, finish, size) << "ms" << endl << endl;
    
    start = clock();
    for(auto rk : randomKeys)
        arrayMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme citanja iz mape implementirane preko nizova: " << averageTime(start, finish, size) << "ms" << endl;
    
    start = clock();
    for(auto rk : randomKeys)
        binarySearchTreeMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme citanja iz mape implementirane binarnim stablom pretrage: " << averageTime(start, finish, size) << "ms" << endl;
    
    
    cout << endl << "Vidimo da je implementacija mape preko binarnog stabla pretrage nezanemarljivo optimalnija.";
    return 0;
}
