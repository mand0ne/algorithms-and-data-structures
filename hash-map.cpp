#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>

using namespace std;

unsigned int KnuthHash(int ulaz, unsigned int max) {
    return (ulaz*2654435761) % 4294967296;
}

template <typename TipKljuca, typename TipVrijednosti>
class Pair{
public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    bool DEL;
    Pair() : DEL(false) {}
    Pair(const TipKljuca& k, const TipVrijednosti& v, bool D = false) : kljuc(k), vrijednost(v), DEL(D) {}
};

template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
public:
    Mapa() {};
    virtual ~Mapa() {};
    virtual int brojElemenata() const = 0;
    virtual TipVrijednosti &operator[] (TipKljuca k) = 0;
    virtual TipVrijednosti operator [] (TipKljuca k) const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &k) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca,TipVrijednosti> {

    private:
        Pair<TipKljuca,TipVrijednosti>* map;
        int size=0;
        int capacity=50;

        void adjust(){
            while(size>=capacity) capacity*=2;
        }

    public:
        NizMapa() : size(0) {
            map=new Pair<TipKljuca,TipVrijednosti>[capacity];
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

        TipVrijednosti &operator[] (TipKljuca kljuc){
            for(int i(0);i<size;i++)
                if(kljuc==map[i].kljuc)
                    return map[i].vrijednost;


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
            temporary.kljuc=kljuc;
            temporary.vrijednost=TipVrijednosti();
            map[size-1]=temporary;
            return map[size-1].vrijednost;
        }

        TipVrijednosti operator [] (TipKljuca kljuc) const {
            for(int i(0);i<size;i++)
                if(kljuc==map[i].kljuc) return map[i].vrijednost;
            return TipVrijednosti();
        }

        void obrisi() {
            delete[] map;
            size=0;
            capacity=50;
            map=new Pair<TipKljuca,TipVrijednosti>[capacity];
        }

        void obrisi(const TipKljuca &kljuc) {
            for(int i(0);i<size;i++)
                if(map[i].kljuc==kljuc) {
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

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa<TipKljuca,TipVrijednosti>{
    static const unsigned int initial_capacity = 128;
    unsigned int size;
    unsigned int max;
    Pair<TipKljuca,TipVrijednosti>** hashMap;
    function<unsigned int(const TipKljuca&, unsigned int)> hashFunction;

    void podesi()
    {
        while(size >= max)
            max*=2;
    }

    void oslobodiSve(int n)
    {
        for(int i=0; i<n; i++) {
            delete hashMap[i];
            hashMap[i] = nullptr;
        }

        delete[] hashMap;
        hashMap = nullptr;
        size=0;
    }

    void prosiri()
    {
        int i=0;
        try {
            int oldMax = max, oldSize = size;
            podesi();
            Pair<TipKljuca, TipVrijednosti> **temp = new Pair<TipKljuca, TipVrijednosti>* [max] {};
            for (; i < oldSize; i++)
                temp[i] = new Pair<TipKljuca, TipVrijednosti>(*hashMap[i]);

            oslobodiSve(oldMax);
            size = oldSize;
            hashMap = temp;
        } catch(exception &e) {
            oslobodiSve(i);
            cout << e.what() << endl << "Prosirivanje nije uspjelo..." << endl;
        }
    }

    unsigned int LinearniHash(const TipKljuca& kljuc, unsigned int i) const
    {
        return (hashFunction(kljuc, max) + i) % max;
    }

    int nadjiIndeksKljuca(const TipKljuca& k) const
    {
        unsigned int index, i(0);
        do {
            index = LinearniHash(k, i);
            if (hashMap[index] && !hashMap[index]->DEL && hashMap[index]->kljuc == k)
                return index;
            else
                i++;
        } while(hashMap[index]!=nullptr && i != max);

        return -1;
    }

    int umetniPar(const TipKljuca& k)
    {
        int index, i(0);
        do {
            index = LinearniHash(k, i);
            if (hashMap[index]==nullptr || hashMap[index]->DEL) {

                if(hashMap[index]!=nullptr)
                    delete hashMap[index];

                hashMap[index] = new Pair<TipKljuca, TipVrijednosti>(k,TipVrijednosti());
                size++;
                return index;
            } else
                i++;
        } while (i < max);

        return -1;
    }


public:
    HashMapa() : size(0), max(initial_capacity), hashMap(new Pair<TipKljuca,TipVrijednosti>*[max] {}) {}

    HashMapa(const HashMapa &hMap) : size(hMap.size), max(hMap.max), hashMap(new Pair<TipKljuca,TipVrijednosti>*[max] {}), hashFunction(hMap.hashFunction)
    {
        for(int i(0); i<max; i++)
            if(hMap.hashMap[i])
                hashMap[i] = new Pair<TipKljuca,TipVrijednosti>(*hMap.hashMap[i]);
    }

    HashMapa(HashMapa &&hMap) noexcept : size(hMap.size), max(hMap.max), hashMap(hMap.hashMap), hashFunction(hMap.hashFunction)
    {
        hMap.size=hMap.max=0;
        hMap.hashMap=nullptr;
    }

    HashMapa &operator = (const HashMapa &hMap)
    {
        if(this!=&hMap) {
            oslobodiSve(max);
            size=hMap.size;
            max=hMap.max;
            hashMap = new Pair<TipKljuca,TipVrijednosti>*[max] {};
            hashFunction = hMap.hashFunction;
            for(int i(0); i<max; i++)
                if(hMap.hashMap[i])
                    hashMap[i] = new Pair<TipKljuca,TipVrijednosti>(*hMap.hashMap[i]);
        }
        return *this;
    }

    HashMapa &operator = (HashMapa &&hMap) noexcept
    {
        if(this!=&hMap) {
            oslobodiSve(max);
            size=hMap.size;
            max=hMap.max;
            hashMap=hMap.hashMap;
            hashFunction = hMap.hashFunction;
            hMap.hashMap=nullptr;
        }
        return *this;
    }

    void definisiHashFunkciju(function<unsigned int(TipKljuca ulaz, unsigned int opsegIzlaza)> hf){
        hashFunction = hf;
    }

    int brojElemenata() const
    {
        return size;
    }

    TipVrijednosti &operator[] (TipKljuca k)
    {
        if(!hashFunction)
            throw runtime_error("Hash funkcija nije definisana!");

        unsigned int index(nadjiIndeksKljuca(k));
        if(index != -1)
            return hashMap[index]->vrijednost;
        else {
            if(size >= max)
                prosiri();

            return hashMap[umetniPar(k)]->vrijednost;
        }
    }

    TipVrijednosti operator [] (TipKljuca k) const
    {
        if(!hashFunction)
            throw runtime_error("Hash funkcija nije definisana!");

        unsigned int index(nadjiIndeksKljuca(k));
        if(index != -1)
            return hashMap[index]->vrijednost;
        else
            return TipVrijednosti();
    }

    void obrisi()
    {
        for(int i(0); i<max; i++) {
            delete hashMap[i];
            hashMap[i]=nullptr;
        }

        size=0;
    }

    void obrisi(const TipKljuca &k)
    {
        unsigned int index(nadjiIndeksKljuca(k));

        if(index != -1) {
            size--;
            hashMap[index]->DEL=true;
        } else
            throw logic_error("Kljuc nije pronaden!");
    }

    ~HashMapa() override
    {
        podesi();
        oslobodiSve(max);
    }
};

double averageTime(clock_t t1, clock_t t2, int size) {
    return ((t2 - t1) / (CLOCKS_PER_SEC / 1000.)) / size;
}

int main(){
    NizMapa<int, int> arrayMap;
    BinStabloMapa<int, int> binarySearchTreeMap;
    HashMapa<int, int> hashMap;
    hashMap.definisiHashFunkciju(KnuthHash);

    srand(time(NULL));

    vector<int> randomKeys;
    int size = 1250;
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
    cout << "Prosjecno vrijeme umetanja u mapu implementirane binarnim stablom pretrage: " << averageTime(start, finish, size) << "ms" << endl;

    start = clock();
    for(auto rk : randomKeys)
        hashMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme umetanja u hash mapu: " << averageTime(start, finish, size) << "ms" << endl<< endl;


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

    start = clock();
    for(auto rk : randomKeys)
        hashMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme citanja iz hash mape: " << averageTime(start, finish, size) << "ms" << endl;

    cout << endl;
    start = clock();
    arrayMap.obrisi();
    finish = clock();
    cout << "Prosjecno vrijeme brisanja svega iz niz mape: " << averageTime(start, finish, size) << "ms" << endl;

    start = clock();
    binarySearchTreeMap.obrisi();
    finish = clock();
    cout << "Prosjecno vrijeme brisanja svega iz mape implementirane binarnim stablom pretrage: " << averageTime(start, finish, size) << "ms" << endl;

    start = clock();
    hashMap.obrisi();
    finish = clock();
    cout << "Prosjecno vrijeme brisanja svega iz hash mape: " << averageTime(start, finish, size) << "ms" << endl;

    /* Preko C9 nije moguce efikasno izvrsiti vece testiranje */
    /* Pokretanjem istog programa samo za 50000 elemenata na CLion-u daje sljedece rezultate:

       Prosjecno vrijeme umetanja u mapu implementirane preko nizova: 0.03912ms
       Prosjecno vrijeme umetanja u mapu implementirane binarnim stablom pretrage: 0.00044ms
       Prosjecno vrijeme umetanja u hash mapu: 0.62452ms

       Prosjecno vrijeme citanja iz mape implementirane preko nizova: 0.03874ms
       Prosjecno vrijeme citanja iz mape implementirane binarnim stablom pretrage: 0.00028ms
       Prosjecno vrijeme citanja iz hash mape: 0.26162ms

       Prosjecno vrijeme brisanja svega iz niz mape: 0ms
       Prosjecno vrijeme brisanja svega iz mape implementirane binarnim stablom pretrage: 0.00032ms
       Prosjecno vrijeme brisanja svega iz hash mape: 0.00048ms
    */
    
    /* Razlog nezadovoljavajucih rezultata za hash mapu je vjerovatno ne bas najbolja hash funkcija a i to sto se problem kolizije rijesava linearnim adresiranjem. */

    return 0;
}
