#include <iostream>
#include <vector>
#include <string>
#include <queue>

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
class Triplet {
    private: 
        TipOznake oznaka;
        bool relacija;
        float tezina;
    
    public:
        Triplet(float tezina, TipOznake oznaka, bool r = true) : oznaka(oznaka), relacija(r), tezina(tezina) {} 
    
        Triplet() : relacija(false) {}
    
        bool operator==(const Triplet &p) {
            return (oznaka==p.getOznaka() && relacija==p.getRelacija() && tezina==p.getTezina());
        }
    
        float getTezina() const { return tezina; }
        float& getTezina() { return tezina; }
        void setTezina(float tezina) { this->tezina = tezina; }
    
        TipOznake getOznaka() const { return oznaka; }   
        TipOznake& getOznaka() { return oznaka; }
        void setOznaka(TipOznake oznaka) { this->oznaka = oznaka; }
    
        bool getRelacija() const { return relacija; }
        void setRelation(bool r) { 
            relacija = r;
            if(!relacija)
                tezina = 0;
        }
};

template <typename TipOznake>
class MatricaGraf : public UsmjereniGraf<TipOznake> {   
    private:
        vector<vector<Triplet<TipOznake>>> graf;
        vector<TipOznake> vertices;
    
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
        MatricaGraf(int n) {
            if (n < 0) 
                throw domain_error("Neispravan broj čvorova!");
            graf.resize(n);
        
            for(auto &v : graf)
                v.resize(n);
        
            vertices.resize(n);
        }
    
        int dajBrojCvorova() const override { return graf.size(); }
    
        void postaviBrojCvorova(int n) override {
            if(n == dajBrojCvorova())
                return;
            if (n < dajBrojCvorova()) 
                throw domain_error("Novi broj čvorova ne može biti manji od trenutnog!");
        
            for(auto &v : graf)
                v.resize(n);
                
            vertices.resize(n);
        }
    
        void dodajGranu(int cvor1, int cvor2, float tezina = 0) override {
            if (postojiGrana(cvor1, cvor2))
                throw logic_error("Grana vec postoji!");
            else {
                Triplet<TipOznake> tripletGrana(tezina, TipOznake(), true);
                graf[cvor1][cvor2] = tripletGrana;
            }
        }
    
        void obrisiGranu(int cvor1, int cvor2) override {
            if (postojiGrana(cvor1, cvor2)) 
                graf[cvor1][cvor2].setRelation(false);
        }
    
        void postaviTezinuGrane(int cvor1, int cvor2, float tezina = 0) override {
            if (!postojiGrana(cvor1, cvor2))
                throw logic_error("Grana ne postoji!");
            else 
                graf[cvor1][cvor2].setTezina(tezina);                
        }
    
        float dajTezinuGrane(int cvor1, int cvor2) const override {
            if (!postojiGrana(cvor1, cvor2)) 
                throw logic_error("Grana ne postoji!");                
            else
                return graf[cvor1][cvor2].getTezina();
        }
    
        bool postojiGrana(int cvor1, int cvor2) const override {
            validateVertex(cvor1);
            validateVertex(cvor2);
            return graf[cvor1][cvor2].getRelacija();
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
            if (!postojiGrana(cvor1, cvor2))
                throw logic_error("Grana ne postoji!");
            else
                graf[cvor1][cvor2].setOznaka(oznaka);      
        }
    
        TipOznake dajOznakuGrane(int cvor1, int cvor2) const override {
            if (!postojiGrana(cvor1, cvor2))
                throw logic_error("Grana ne postoji!");
            else 
                return graf[cvor1][cvor2].getOznaka();
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
    
        ~MatricaGraf() {}
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

int main() {
    return 0;
}
