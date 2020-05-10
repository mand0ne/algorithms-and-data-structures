#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <iomanip>
#include <functional>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa{
public:
    Mapa() = default;
    virtual ~Mapa() = default;
    virtual int brojElemenata() const = 0;
    virtual TipVrijednosti &operator[] (TipKljuca k) = 0;
    virtual TipVrijednosti operator [] (TipKljuca k) const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &k) = 0;
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
struct AVLCvor{
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    AVLCvor* lijevo,* desno,* roditelj;
    int balans;

    AVLCvor() : kljuc(TipKljuca()), vrijednost(TipVrijednosti()), lijevo(nullptr), desno(nullptr), roditelj(nullptr), balans(0){}

    AVLCvor(const TipKljuca& kljuc, const TipVrijednosti& vrijednost, AVLCvor<TipKljuca,TipVrijednosti>* roditelj = nullptr,
               AVLCvor<TipKljuca,TipVrijednosti>* lijevo = nullptr, AVLCvor<TipKljuca,TipVrijednosti>* desno = nullptr, int balans=0) 
                     : kljuc(kljuc), vrijednost(vrijednost), lijevo(lijevo), desno(desno), roditelj(roditelj), balans(balans) {}
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa : public Mapa<TipKljuca,TipVrijednosti> {
    AVLCvor<TipKljuca,TipVrijednosti>* korijen;
    int velicina;

    void umetniCvor(AVLCvor<TipKljuca,TipVrijednosti>* &novi){
        AVLCvor<TipKljuca,TipVrijednosti>* x = korijen, * y = nullptr;

        while(x) {
            y = x;
            if(novi->kljuc < x->kljuc)
                x = x->lijevo;
            else
                x = x->desno;
        }

        if(!y)
            korijen = novi;

        else {
            if(novi->kljuc < y->kljuc)
                y->lijevo = novi;
            else
                y->desno = novi;

            novi->roditelj=y;
        }

        if (y && y->desno && y->lijevo)
            y->balans = 0;
        else if(y && (y->desno || y->lijevo))
            azurirajBalans(y);
    }

    void lijevaRotacija(AVLCvor<TipKljuca,TipVrijednosti>* c)  {
        AVLCvor<TipKljuca,TipVrijednosti>* desnoDijete = c->desno;
        AVLCvor<TipKljuca,TipVrijednosti>* ddLijevoDijete = desnoDijete->lijevo;

        desnoDijete->lijevo = c;
        desnoDijete->roditelj = c->roditelj;
        if (desnoDijete->roditelj == nullptr)
            korijen = desnoDijete;
        else if (c == c->roditelj->desno)
            desnoDijete->roditelj->desno = desnoDijete;
        else
            desnoDijete->roditelj->lijevo = desnoDijete;

        c->roditelj = desnoDijete;
        c->desno = ddLijevoDijete;

        if (ddLijevoDijete != nullptr)
            ddLijevoDijete->roditelj = c;

        c->balans = 0;
        desnoDijete->balans = 0;
    }

    void desnaRotacija(AVLCvor<TipKljuca,TipVrijednosti>* c) {
        AVLCvor<TipKljuca,TipVrijednosti>* lijevoDijete = c->lijevo;
        AVLCvor<TipKljuca,TipVrijednosti>* ldDesnoDijete = lijevoDijete->desno;

        lijevoDijete->desno = c;
        lijevoDijete->roditelj = c->roditelj;
        if (lijevoDijete->roditelj == nullptr)
            korijen = lijevoDijete;
        else if (c == c->roditelj->desno)
            lijevoDijete->roditelj->desno = lijevoDijete;
        else
            lijevoDijete->roditelj->lijevo = lijevoDijete;

        c->roditelj =  lijevoDijete;
        c->lijevo = ldDesnoDijete;

        if (ldDesnoDijete != nullptr)
            ldDesnoDijete->roditelj = c;

        c->balans = 0;
        lijevoDijete->balans = 0;
    }

    void dvostrukaRotacija(AVLCvor<TipKljuca,TipVrijednosti>* c, bool prvoDesna){
        if(prvoDesna){
            desnaRotacija(c->desno);
            lijevaRotacija(c);
        }
        else{
            lijevaRotacija(c->lijevo);
            desnaRotacija(c);
        }
    }

    void azurirajBalans(AVLCvor<TipKljuca,TipVrijednosti> *novi){
        if(!novi->roditelj)
            return;

        AVLCvor<TipKljuca,TipVrijednosti>* roditeljNovog = novi->roditelj;

        if(novi == roditeljNovog->lijevo)
            roditeljNovog->balans++;
        else
            roditeljNovog->balans--;


        if(roditeljNovog->balans < -1) {
            if(roditeljNovog->desno->balans <= -1)
                lijevaRotacija(roditeljNovog);
            else
                dvostrukaRotacija(roditeljNovog, true);
        }

        else if(roditeljNovog->balans > 1) {
            if(roditeljNovog->lijevo->balans >= 1)
                desnaRotacija(roditeljNovog);
            else
                dvostrukaRotacija(roditeljNovog, false);
        }

        else if (roditeljNovog->balans==0)
            return;
        else
            azurirajBalans(roditeljNovog);
    }

    void kopirajPreorderObilaskom(AVLCvor<TipKljuca,TipVrijednosti> *stari, AVLCvor<TipKljuca,TipVrijednosti> *&novi, AVLCvor<TipKljuca,TipVrijednosti> *roditelj=nullptr){
        if(stari){
            novi = new AVLCvor<TipKljuca,TipVrijednosti>(stari->kljuc, stari->vrijednost, roditelj, nullptr, nullptr, stari->balans);
            kopirajPreorderObilaskom(stari->lijevo, novi->lijevo, novi);
            kopirajPreorderObilaskom(stari->desno, novi->desno, novi);
        }
    }

public:
    AVLStabloMapa() : korijen(nullptr), velicina(0) {}

    ~AVLStabloMapa() override {
        this->obrisi();
    }

    AVLStabloMapa(const AVLStabloMapa& avlsm) : korijen(nullptr), velicina(avlsm.velicina){
        kopirajPreorderObilaskom(avlsm.korijen,korijen);
    }

    AVLStabloMapa(const AVLStabloMapa&& avlsm) noexcept : korijen(avlsm.korijen), velicina(avlsm.velicina){
        avlsm.korijen = nullptr;
    }

    AVLStabloMapa &operator=(const AVLStabloMapa& avlsm){
        if(this!=&avlsm){
            this->obrisi();
            velicina = avlsm.velicina;
            kopirajPreorderObilaskom(avlsm.korijen, korijen);
        }
        return *this;
    }

    AVLStabloMapa& operator=(AVLStabloMapa&& avlsm) noexcept {
        if(this != &avlsm){
            this->obrisi();
            korijen = avlsm.korijen;
            velicina = avlsm.velicina;
            avlsm.korijen = nullptr;
        }
        return *this;
    }

    int brojElemenata() const override {
        return velicina;
    }

    TipVrijednosti operator [] (TipKljuca kljuc) const override {
        AVLCvor<TipKljuca,TipVrijednosti>* temp=korijen;

        while(temp && kljuc!=temp->kljuc) {
            if(kljuc<temp->kljuc)
                temp = temp->lijevo;
            else
                temp = temp->desno;
        }

        if(!temp)
            return TipVrijednosti();

        return temp->vrijednost;
    }

    TipVrijednosti &operator[] (TipKljuca kljuc) override {

        AVLCvor<TipKljuca,TipVrijednosti>* temp = korijen;

        while(temp && kljuc != temp->kljuc) {
            if(kljuc<temp->kljuc)
                temp = temp->lijevo;
            else
                temp = temp->desno;
        }

        if(!temp){
            try{
                temp = new AVLCvor<TipKljuca,TipVrijednosti>(kljuc,TipVrijednosti());
                velicina++;
                umetniCvor(temp);
            }
            catch(exception &e){
                if(!temp)
                    delete temp;
                temp = nullptr;
                cout << "Exception: " << e.what() << endl;
                throw e;
            }
        }

        return temp->vrijednost;
    }

    void obrisi(const TipKljuca& k) override {
        AVLCvor<TipKljuca,TipVrijednosti>* p = korijen;
        AVLCvor<TipKljuca,TipVrijednosti>* q = nullptr, * tmp = nullptr, * pp=nullptr, *rp=nullptr;

        while (p != nullptr) {
            if (k == p->kljuc) 
                break;
            
            q = p;
            
            if (k < p->kljuc) 
                p = p->lijevo;
            else 
                p = p->desno;
        }

        if (p == nullptr) 
            return;

        if (p->lijevo == nullptr) { 
            rp = p->desno; 
            pp = q; 
        }
        
        else if (p->desno == nullptr) { 
            rp = p->lijevo; 
            pp = q; 
        }
        
        else {
            pp = p;
            rp = p->lijevo;
            tmp = rp->desno;
            
            while (tmp != nullptr) {
                pp = rp;
                rp = tmp;
                tmp = rp->desno;
            }
            
            if (pp != p) {
                pp->desno = rp->lijevo;
                
                if (rp->lijevo) 
                    rp->lijevo->roditelj = pp;
                
                rp->lijevo = p->lijevo;
                
                if (p->lijevo) 
                    p->lijevo->roditelj = rp;
            }
            
            rp->desno = p->desno;
            
            if (p->desno) 
                p->desno->roditelj = rp;
        }

        if (q == nullptr) 
            korijen = rp;
        else if (p == q->lijevo) { 
            q->lijevo = rp; 
            q->balans--; 
        }
        else {
            q->desno=rp; 
            q->balans++; 
        }


        if (rp != nullptr) 
            rp->roditelj = q;
        p->desno = p->lijevo = p->roditelj = nullptr;

        delete p;
        p = nullptr;
        velicina--;
    }

    void oslobodiSve(AVLCvor<TipKljuca,TipVrijednosti>* &c){
        if(!c){
            velicina = 0;
            return;
        }
        
        oslobodiSve(c->lijevo);
        oslobodiSve(c->desno);
        c->roditelj = c->lijevo = c->desno = nullptr;
        delete c;
        c = nullptr;
    }

    void obrisi() override {
        oslobodiSve(korijen);
    }
};

double averageTime(clock_t t1, clock_t t2, int size) {
    return ((t2 - t1) / (CLOCKS_PER_SEC / 1000.)) / size;
}



int main(){
    AVLStabloMapa<int, int> avlTreeMap;
    BinStabloMapa<int, int> binarySearchTreeMap;
    
    srand(time(NULL));
    
    vector<int> randomKeys;
    int size = 10000;
    for(int i = 0; i < size; i++)
        randomKeys.push_back(rand()+i/2);
        
    clock_t start = clock();
    for(auto rk : randomKeys)
        avlTreeMap[rk];
    clock_t finish = clock();
    cout << "Prosjecno vrijeme umetanja u mapu implementirane preko AVL stabla: " << averageTime(start, finish, size) << "ms" << endl;
    
    start = clock();
    for(auto rk : randomKeys)
        binarySearchTreeMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme umetanja u mapu implementirane binarnim stablom pretrage: " << averageTime(start, finish, size) << "ms" << endl << endl;
    
    start = clock();
    for(auto rk : randomKeys)
        avlTreeMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme citanja iz mape implementirane preko AVL stabla: " << averageTime(start, finish, size) << "ms" << endl;
    
    start = clock();
    for(auto rk : randomKeys)
        binarySearchTreeMap[rk];
    finish = clock();
    cout << "Prosjecno vrijeme citanja iz mape implementirane binarnim stablom pretrage: " << averageTime(start, finish, size) << "ms" << endl;
    
    
    return 0;
}