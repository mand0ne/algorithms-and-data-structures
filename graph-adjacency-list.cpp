#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <stdlib.h>
#include <queue>
#include <list>

using namespace std;

template <typename TipOznake>
class Grana;

template <typename TipOznake>
class Cvor;

template <typename TipOznake>
class GranaIterator;

template <typename TipOznake>
class UsmjereniGraf {
    protected: 
        UsmjereniGraf() {}
    
    public:
        UsmjereniGraf(int brojCvorova) {}
    
        virtual int dajBrojCvorova() const = 0;
        virtual void postaviBrojCvorova(int n) = 0;
    
        virtual Cvor<TipOznake> dajCvor(int redniBroj) = 0;
        virtual TipOznake dajOznakuCvora(int redniBroj) const = 0;
        virtual void postaviOznakuCvora(int redniBroj, TipOznake oznaka) = 0;
    
        virtual bool postojiGrana(int cvor1, int cvor2) const = 0;
        virtual void dodajGranu(int cvor1, int cvor2, float tezina = 0) = 0;
        virtual void obrisiGranu(int cvor1, int cvor2) = 0;
        virtual float dajTezinuGrane(int cvor1, int cvor2) const = 0;
        virtual void postaviTezinuGrane(int cvor1, int cvor2, float tezina = 0) = 0;
        
        virtual Grana<TipOznake> dajGranu(int cvor1, int cvor2) = 0;
        virtual TipOznake dajOznakuGrane(int cvor1, int cvor2) const = 0;
        virtual void postaviOznakuGrane(int cvor1, int cvor2, TipOznake oznaka) = 0;
        
    
        GranaIterator<TipOznake> dajGranePocetak() {
            return GranaIterator<TipOznake>(this);
        }
        GranaIterator<TipOznake> dajGraneKraj() {
            return GranaIterator<TipOznake>(this, true);
        }
    
        virtual void nadjiSljedecuGranu(int &p, int &k) const = 0;
    
        virtual ~UsmjereniGraf() {}
};

template <typename TipOznake>
class Cvor {
    private:
        UsmjereniGraf<TipOznake>* graf;
        int redniBroj;
    
    public:
        Cvor(UsmjereniGraf<TipOznake>* graf, int redniBroj) : graf(graf), redniBroj(redniBroj) {}
    
        TipOznake dajOznaku() { return graf->dajOznakuCvora(redniBroj); }
        void postaviOznaku(TipOznake oznaka) { graf->postaviOznakuCvora(redniBroj, oznaka); }
        int dajRedniBroj() { return redniBroj; }
};

template <typename TipOznake>
class Grana {
    private:
        UsmjereniGraf<TipOznake>* graf;
        int polazni, dolazni;
    
    public:
        Grana(UsmjereniGraf<TipOznake>* graf, int cvor1, int cvor2) : graf(graf), polazni(cvor1), dolazni(cvor2) {}
    
        float dajTezinu() { return graf->dajTezinuGrane(polazni, dolazni); }
        void postaviTezinu(float tezina) { graf->postaviTezinuGrane(polazni, dolazni, tezina); }
    
        TipOznake dajOznaku() { return graf->dajOznakuGrane(polazni, dolazni); }
        void postaviOznaku(TipOznake oznaka) { graf->postaviOznakuGrane(polazni, dolazni, oznaka); }
    
        Cvor<TipOznake> dajPolazniCvor() { return graf->dajCvor(polazni); }
        Cvor<TipOznake> dajDolazniCvor() { return graf->dajCvor(dolazni); }
};

template <typename TipOznake>
class GranaIterator {
    private:
        UsmjereniGraf<TipOznake>* graf;
        int polazni, dolazni;
    
    public:
        GranaIterator(UsmjereniGraf<TipOznake>*graf, bool kraj = false) : graf(graf), polazni(0), dolazni(-1) {
            if (kraj) {
                polazni = graf->dajBrojCvorova();
                dolazni = graf->dajBrojCvorova();
            }
            else 
                graf->nadjiSljedecuGranu(polazni, dolazni);
        }
    
        Grana<TipOznake> operator*() {
            return graf->dajGranu(polazni, dolazni);
        }
    
        bool operator==(const GranaIterator &gIterator) const {
            return (graf == gIterator.graf && polazni == gIterator.polazni && dolazni == gIterator.dolazni);
        }
    
        bool operator!=(const GranaIterator& iterator) const {
            return !(*this == iterator);
        }
    
        GranaIterator& operator++() {
            graf->nadjiSljedecuGranu(polazni, dolazni);
            return *this;
        }
    
        GranaIterator operator++(int) {
            GranaIterator<TipOznake> trenutni(*this);
            ++(*this);
            return trenutni;
        }
};

template<typename TipOznake>
class TripletLista {
    private: 
        TipOznake oznaka;
        int dolazni;
        float tezina;
    
    public:
        TripletLista(float tezina, TipOznake oznaka, int cvor) : oznaka(oznaka), dolazni(cvor), tezina(tezina) {} 
    
        TripletLista() {}
    
        bool operator==(const TripletLista &p) {
            return (oznaka==p.getOznaka() && dolazni==p.getDolazni() && tezina==p.getTezina());
        }
    
        float getTezina() const { return tezina; }
        float& getTezina() { return tezina; }
        void setTezina(float tezina) { this->tezina = tezina; }
    
        TipOznake getOznaka() const { return oznaka; }   
        TipOznake& getOznaka() { return oznaka; }
        void setOznaka(TipOznake oznaka) { this->oznaka = oznaka; }
    
        int getDolazni() const { return dolazni; }
        void setDolazni(int cvor) { dolazni = cvor; }
};

template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {   
    private:
        vector<vector<TripletLista<TipOznake>>> graf;
        vector<TipOznake> vertices;
    
        typename vector<TripletLista<TipOznake>>::const_iterator listFind(int cvor1, int cvor2) const {
            validateVertex(cvor1);
            validateVertex(cvor2);
            auto it(find_if(graf[cvor1].begin(), graf[cvor1].end(),[cvor2](const TripletLista<TipOznake> &triplet){
                                                                    return triplet.getDolazni()==cvor2;}));
            return it;
        }
        
        typename vector<TripletLista<TipOznake>>::iterator listFind(int cvor1, int cvor2) {
            validateVertex(cvor1);
            validateVertex(cvor2);
            auto it(find_if(graf[cvor1].begin(), graf[cvor1].end(),[cvor2](const TripletLista<TipOznake> &triplet){
                                                                    return triplet.getDolazni()==cvor2;}));
            return it;
        }
        
        void validateVertex(int redniBroj) const {
            if (redniBroj < 0 || redniBroj >= dajBrojCvorova()) 
                throw domain_error("Cvor nije validan!");
        }
        
        void nadjiSljedecuGranu(int &p, int &k) const override {
            int i(p), j(++k);
            while(i < dajBrojCvorova()) {
                while(j < dajBrojCvorova()) {
                    if (postojiGrana(i, j)) {
                        k = j;
                        p = i;
                        return;
                    }
                    j++;
                }
                j = 0;
                i++;
            }
            k = dajBrojCvorova();
            p = dajBrojCvorova();
        }
    
    public:
        ListaGraf(int n) {
            if (n < 0) 
                throw domain_error("Neispravan broj čvorova!");
                
            graf.resize(n);
            vertices.resize(n);
        }
    
        int dajBrojCvorova() const override { return graf.size(); }
    
        void postaviBrojCvorova(int n) override {
            if(n == dajBrojCvorova())
                return;
            if (n < dajBrojCvorova()) 
                throw domain_error("Novi broj čvorova ne može biti manji od trenutnog!");
            
            graf.resize(n);
            vertices.resize(n);
        }
    
        void dodajGranu(int cvor1, int cvor2, float tezina = 0) override {
            if (postojiGrana(cvor1, cvor2))
               throw logic_error("Grana vec postoji!");
            else {
                TripletLista<TipOznake> tripletGrana(tezina, TipOznake(), cvor2);
                graf[cvor1].push_back(tripletGrana);
            }
        }
    
        void obrisiGranu(int cvor1, int cvor2) override {
            remove_if(graf[cvor1].begin(), graf[cvor1].end(),[cvor2](const TripletLista<TipOznake> &triplet){ return triplet.getDolazni()==cvor2; });
        }
    
        void postaviTezinuGrane(int cvor1, int cvor2, float tezina = 0) override {
            auto it(listFind(cvor1,cvor2));
            if (graf[cvor1].empty() || (!graf[cvor1].empty() && it==graf[cvor1].end()))
                throw logic_error("Grana ne postoji!");   
            else
                it->setTezina(tezina);
        }
    
        float dajTezinuGrane(int cvor1, int cvor2) const override {
            auto it(listFind(cvor1,cvor2));
            if (graf[cvor1].empty() || (!graf[cvor1].empty() && it==graf[cvor1].end()))
                throw logic_error("Grana ne postoji!");               
            else
               return it->getTezina();
        }
        
        bool postojiGrana(int cvor1, int cvor2) const override {
            if(graf[cvor1].empty()) 
                return false;
            
            validateVertex(cvor1);
            validateVertex(cvor2);
            
            return find_if(graf[cvor1].begin(), graf[cvor1].end(),[cvor2](const TripletLista<TipOznake> &triplet){
                                                                    return triplet.getDolazni()==cvor2;}) != graf[cvor1].end();
        }
    
        void postaviOznakuCvora(int redniBroj, TipOznake oznaka) override {
            validateVertex(redniBroj);
            vertices[redniBroj] = oznaka;
        }
    
        TipOznake dajOznakuCvora(int redniBroj) const override {
            validateVertex(redniBroj);
            return vertices[redniBroj];
        }
    
        void postaviOznakuGrane(int cvor1, int cvor2, TipOznake oznaka) override {
            auto it(listFind(cvor1,cvor2));
            if (graf[cvor1].empty() || (!graf[cvor1].empty() && it==graf[cvor1].end()))
                throw logic_error("Grana ne postoji!");   
            else
                it->setOznaka(oznaka);      
        }
    
        TipOznake dajOznakuGrane(int cvor1, int cvor2) const override {
            auto it(listFind(cvor1,cvor2));
            if (graf[cvor1].empty() || (!graf[cvor1].empty() && it==graf[cvor1].end()))
                throw logic_error("Grana ne postoji!");   
            else
               return it->getOznaka();

        }
    
        Cvor<TipOznake> dajCvor(int redniBroj) override {
            validateVertex(redniBroj);
            return Cvor<TipOznake>(this, redniBroj);
        }
    
        Grana<TipOznake> dajGranu(int cvor1, int cvor2) override {
            if (!postojiGrana(cvor1, cvor2))
                throw logic_error("Grana ne postoji!");
            else
                return Grana<TipOznake>(this, cvor1, cvor2);
        }
    
        ~ListaGraf() {}
};

template <typename TipOznake>
void searchByBreadth(UsmjereniGraf<TipOznake>* graf, vector<bool> &vPosjecenosti, Cvor<TipOznake> cvor, vector<Cvor<TipOznake>> &vertices)
{
    vPosjecenosti[cvor.dajRedniBroj()] = true;
    queue<Cvor<TipOznake>> prioritetRed;
    prioritetRed.push(cvor);
    
    while (!prioritetRed.empty()) {
        Cvor<TipOznake> susjed = prioritetRed.front();
        prioritetRed.pop();
        vertices.push_back(susjed);
        for (int i(0); i < graf->dajBrojCvorova(); i++)
            if (!vPosjecenosti.at(i) && graf->postojiGrana(susjed.dajRedniBroj(), i)) {
                vPosjecenosti[i] = true;
                prioritetRed.push(graf->dajCvor(i));
            }
    }
}

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake>* graf, vector<Cvor<TipOznake>> &vertices, Cvor<TipOznake> cvor) {
    vector<bool> vPosjecenosti(graf->dajBrojCvorova(), false);
    searchByBreadth(graf, vPosjecenosti, cvor, vertices);
    
    for (int i(0); i<vPosjecenosti.size(); i++)
        if (!vPosjecenosti.at(i)) 
            searchByBreadth(graf, vPosjecenosti, graf->dajCvor(i), vertices);
}

template <typename TipOznake>
void searchByDepth(UsmjereniGraf<TipOznake>* graf, vector<bool> &vPosjecenosti, Cvor<TipOznake> cvor, vector<Cvor<TipOznake>> &vertices)
{
    vPosjecenosti.at(cvor.dajRedniBroj()) = true;
    vertices.push_back(cvor);
    
    for (int i(0); i < graf->dajBrojCvorova(); i++)
        if (!vPosjecenosti.at(i) && graf->postojiGrana(cvor.dajRedniBroj(), i)) 
            searchByDepth(graf, vPosjecenosti, graf->dajCvor(i), vertices);
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* graf, vector<Cvor<TipOznake>> &vertices, Cvor<TipOznake> cvor) {
    vector<bool> vPosjecenosti(graf->dajBrojCvorova(), false);
    searchByDepth(graf, vPosjecenosti, cvor, vertices);
    
    for (int i(0); i<vPosjecenosti.size(); i++)
        if (!vPosjecenosti.at(i)) 
            searchByDepth(graf, vPosjecenosti, graf->dajCvor(i), vertices);
}

unsigned int KnuthHash(int ulaz, unsigned int max) {
    return (ulaz*2654435761) % 4294967296;
}

template <typename TipKljuca, typename TipVrijednosti>
class Pair {
public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Pair(const TipKljuca& k, const TipVrijednosti& v) : kljuc(k), vrijednost(v) {}
    
};

template <typename TipKljuca, typename TipVrijednosti>
bool operator < (const Pair<TipKljuca, TipVrijednosti> &p1, const Pair<TipKljuca, TipVrijednosti> &p2){        
    return p1.kljuc<p2.kljuc;    
}
    
template <typename TipKljuca, typename TipVrijednosti>
bool operator > (const Pair<TipKljuca, TipVrijednosti> &p1, const Pair<TipKljuca, TipVrijednosti> &p2){
    return p1.kljuc>p2.kljuc;    
}
    
template <typename TipKljuca, typename TipVrijednosti>
class Mapa {
public:
    Mapa() {};
    virtual ~Mapa() {};
    virtual int brojElemenata() const = 0;
    virtual TipVrijednosti &operator[] (const TipKljuca &k) = 0;
    virtual TipVrijednosti operator [] (const TipKljuca &k) const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &k) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca,TipVrijednosti> {
    unsigned int totalSize;
    vector<vector<Pair<TipKljuca,TipVrijednosti>>> hashMap;
    function<unsigned int(const TipKljuca&, unsigned int)> hashFunction;
    
    typename vector<Pair<TipKljuca,TipVrijednosti>>::const_iterator findKey(const TipKljuca& k) const {
        if(!hashFunction)
            throw runtime_error("Hash funkcija nije definisana!");
        
        unsigned int index(hashFunction(k, hashMap.size()));
        Pair<TipKljuca,TipVrijednosti> temp(k, TipVrijednosti());
        auto it(lower_bound(hashMap[index].begin(), hashMap[index].end(), temp));
        
        if(hashMap[index].empty())
            it = hashMap[index].begin();
            
        return it;        
    }
    
    typename vector<Pair<TipKljuca,TipVrijednosti>>::iterator findKey(const TipKljuca& k) {
        if(!hashFunction)
            throw runtime_error("Hash funkcija nije definisana!");
        
        unsigned int index(hashFunction(k, hashMap.size()));
        Pair<TipKljuca,TipVrijednosti> temp(k, TipVrijednosti());
        auto it(lower_bound(hashMap[index].begin(), hashMap[index].end(), temp));
        
        if(hashMap[index].empty())
            it = hashMap[index].begin();
            
        return it;        
    }

public:
    HashMapaLan() : totalSize(0) {
        hashMap.resize(4096);
    }

    HashMapaLan(const HashMapaLan &hMap) : totalSize(hMap.totalSize), hashMap(hMap.hashMap), hashFunction(hMap.hashFunction) {}

    HashMapaLan(HashMapaLan &&hMap) noexcept : totalSize(hMap.totalSize), hashMap(hMap.hashMap), hashFunction(hMap.hashFunction){}

    HashMapaLan &operator = (const HashMapaLan &hMap){
        if(this!=&hMap) {
            totalSize = hMap.totalSize;
            hashMap = hMap.hashMap;
            hashFunction = hMap.hashFunction;
        }
        return *this;
    }

    HashMapaLan &operator = (HashMapaLan &&hMap) noexcept {
        if(this!=&hMap) {
            totalSize = hMap.totalSize;
            hashMap = hMap.hashMap;
            hashFunction = hMap.hashFunction;
        }
        return *this;
    }

    void definisiHashFunkciju(function<unsigned int(TipKljuca ulaz, unsigned int opsegIzlaza)> hf){
        hashFunction = hf;
    }

    int brojElemenata() const {
        return totalSize;
    }

    TipVrijednosti &operator[] (const TipKljuca &k) {
        auto it(findKey(k));
        int index(hashFunction(k, hashMap.size()));
	    if (it == hashMap[index].end() || it->kljuc != k) {
	        Pair<TipKljuca, TipVrijednosti> noviPar(k, TipVrijednosti());
		    auto insertIterator(lower_bound(hashMap[index].begin(), hashMap[index].end(), noviPar));
		    it = hashMap[index].insert(insertIterator, noviPar);
		    totalSize++;
	    }
	    return it->vrijednost;
    }

    TipVrijednosti operator[] (const TipKljuca &k) const {
        auto it(findKey(k));
	    int index(hashFunction(k, hashMap.size()));
	    if (it == hashMap[index].end() || it->kljuc != k)
		    return TipVrijednosti();
	    
	    return it->vrijednost;
    }

    void obrisi(){
        for(vector<Pair<TipKljuca,TipVrijednosti>> &v : hashMap)
                v.clear();
                
        totalSize = 0;
    }

    void obrisi(const TipKljuca &k) {
	    auto it(findKey(k));
	    int index(hashFunction(k, hashMap.size()));
	    if (it == hashMap[index].end() || it->kljuc != k)
		    throw logic_error("Kljuc nije pronadjen");
	    hashMap[index].erase(it);
	    totalSize--;
    }

    ~HashMapaLan() override {
        hashMap.clear();
    }
};

int main(){
  clock_t time1, time2;
  time1 = clock();
  UsmjereniGraf<bool> *g = new ListaGraf<bool>(100);

  for(int i=0; i<100;i++)
    for(int j=0; j<100; j++)
        if(i!=j){
            float tezina=i + (j+i)/1000.;
            g->dodajGranu(i, j, tezina);
        }
        
  g->postaviTezinuGrane(1, 2, 1.22);
  g->postaviTezinuGrane(3, 2, 3.22);
  g->postaviTezinuGrane(4, 2, 4.22);
  g->postaviTezinuGrane(2, 3, 2.33);
  
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 100; j++)
         if (g->postojiGrana(i,j))
            g->dajTezinuGrane(i, j);
    
    time2=clock(); 
    std::cout << endl <<endl <<  "Vrijeme izvrsavanja je " << (time2-time1)/(CLOCKS_PER_SEC / 1000) << "ms" << std::endl;
    
    delete g;
    return 0;
}