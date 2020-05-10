#include <iostream>
#include <algorithm>
#include <utility>
#include <string>
#include <fstream>
#include <stdexcept>
#include <exception>

using std::string;
using std::cout;
using std::cin;
using std::endl;


template <typename Tip>
void bubble_sort(Tip* niz, int vel){
for(int i(vel - 1); i >= 1; i--)
      for(int j(1); j <= i; j++)
         if(niz[j - 1] > niz[j]) 
           std::swap(niz[j - 1], niz[j]);
}

template <typename Tip>
void selection_sort(Tip* niz, int vel){
    for(int i(0); i <= vel-2; i++) {
        Tip minElement = niz[i];
        int minElementIndex = i;
        for (int j(i+1); j <= vel-1; j++)
            if (niz[j] < minElement) {
                minElement = niz[j];
                minElementIndex = j;
            }
        niz[minElementIndex] = niz[i];
        niz[i] = minElement;
    }
}    

template <typename Tip>
void QuickSort(Tip *niz, int prvi, int zadnji){
    if(prvi<zadnji){
        Tip pivot = niz[zadnji];
        int i(prvi - 1); 
        for (int j(prvi); j <= zadnji-1; j++) 
            if (niz[j] <= pivot) { 
                i++;
                std::swap(niz[i], niz[j]); 
            } 
        std::swap(niz[i + 1],niz[zadnji]); 

        int j = i+1;
        QuickSort(niz, prvi, j-1);
        QuickSort(niz, j+1, zadnji);
    }
}

template <typename Tip>
void quick_sort(Tip* niz, int vel){
    QuickSort(niz, 0, vel-1);
}

template<typename Tip>
void Merge(Tip*niz,const int l, const int p, const int q, const int u){
    int i(0), j(q-l), k(l);
    Tip *novi = new Tip[u - l + 1];
    
    for(int m(0); m <= u - l; m++)
        novi[m] = niz[l + m];
    
    while(i <= p - l && j <= u - l) {
        if (novi[i] < novi[j])
            niz[k] = novi[i++];
        else
            niz[k] = novi[j++];
        k++;
    }
    
    while (i <= p-l) 
        niz[k++] = novi[i++];
    
    while (j <= u-l) 
        niz[k++] = novi[j++];
    
    delete [] novi;
}

template <typename Tip>
void MergeSort(Tip *niz, const int l, const int u){
    if(u > l){
        int p((l + u - 1)/2);
        int q(p + 1);
        MergeSort(niz,l,p);
        MergeSort(niz,q,u);
        Merge(niz,l,p,q,u);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel){
    MergeSort(niz, 0, vel - 1);
}
    
void ucitaj(string filename,int *&niz,int &vel){
    std::ifstream data(filename.c_str());    
    if(!data.good()) 
        throw std::runtime_error("Greska pri otvaranju datoteke!");
    
    vel=0; 
    char znak{};
    
    while(znak!=EOF){
        int x;
        data>>x;
        znak=data.get();
        if((data.fail() && !data.eof()) || !(znak==',' || znak==' ' ||znak==EOF || znak=='\n')) 
            throw std::logic_error("Problemi pri citanju datoteke");
        vel++;
    }
    
    data.clear();
    niz = new int[vel];
    int i(0);
    data.seekg(0,std::ios_base::beg);
    
    while(!data.eof()){
        data >> niz[i++];
        data.get(znak);
    }
    
    data.close();
}

void generisi(string filename,int vel){
    std::ofstream odata(filename.c_str());
    
    if(!odata.good()) 
        throw std::runtime_error("Greska pri stvaranju datoteke!");
    
    srand(time(NULL));
    
    for(int i(0); i<vel; i++){
        odata << rand()%1000;
        if(i < vel - 1) odata << "\n";
    }
    
    odata.close();
}

void upisi(string filename, int *niz, int vel){
    std::ofstream iDatoteka(filename.c_str());
    
    if(!iDatoteka.good()) 
        throw std::runtime_error("Greska pri stvaranju datoteke!");
    
    for(int i(0); i<vel; i++){
        iDatoteka << niz[i];
        if(i > 1  && !(niz[i] >= niz[i - 1])) 
            throw std::logic_error("Niz nije ispravno sortiran");
        if(i < vel - 1) 
            iDatoteka << "\n";
    }
    
    iDatoteka.close();
}

void menu(){
    clock_t time1, time2;
    int totalTime;
    time1 = clock();
    
    int izbor1;
    cout << "Unesite izbor (1 - Vec postojeca datoteka, 2 - Generisi novu datoteku sa random podacima): ";
    cin >> izbor1;
    
    string uDatoteka,iDatoteka;
    cout << "Unesite ime datoteke: ";
    cin >> uDatoteka;
    
    if(izbor1==2) { 
        cout << "Unesite broj koliko ce se generisati: ";
        int n; 
        cin >> n; 
        generisi(uDatoteka,n);
    }
    
    int izbor2;
    cout << "Izaberite algoritam soritranja : \n" 
         << "1. Bubble sort \n"
         << "2. Selection sort \n"
         << "3. Quick sort \n" 
         << "4. Merge sort \n" << endl;
    cin >>izbor2;
    cout << "Unesite ime izlazne datoteke: ";
    cin >> iDatoteka;
    
    int * niz;
    int vel;
    
    ucitaj(uDatoteka,niz,vel);
    
    switch(izbor2){
        case 1:
            bubble_sort(niz,vel);
            break;
        case 2:
            selection_sort(niz,vel);
            break;
        case 3:
            quick_sort(niz,vel);
            break;
        case 4:
            merge_sort(niz,vel);
            break;
        default:
            break;
    }
    
    upisi(iDatoteka,niz,vel);
    delete [] niz;
    
    time2 = clock(); 
    totalTime = (time2 - time1) / (CLOCKS_PER_SEC / 1000);
    std::cout << "\nVrijeme izvršenja: " << totalTime << " ms." << std::endl << std::endl;
}

int main() {
    menu();
    return 0;
}
  