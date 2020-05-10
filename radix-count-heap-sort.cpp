#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std; 
  
void countSort(vector<int> &v, int stepen) {
    int n=v.size();
    int *sortiran = new int[n]; 
    int histogram[10]{0}; 
  
    for (int i = 0; i < n; i++) 
        histogram[ (v[i] / stepen) % 10]++; 
  
    for (int i = 1; i < 10; i++) 
        histogram[i] = histogram[i] + histogram[i - 1]; 

    for (int i = n - 1; i >= 0; i--) { 
        histogram[ (v[i] / stepen) % 10 ]--;
        sortiran[histogram[ (v[i ] / stepen) % 10 ]] = v[i]; 
    } 
  
    for (int i = 0; i < n; i++) 
        v[i] = sortiran[i]; 
        
    delete[] sortiran;
} 

void radixSort(vector<int> &v) {
    int maxElement = *(max_element(v.begin(),v.end()));
    
    for (int stepen = 1; maxElement/stepen > 0; stepen *= 10) 
        countSort(v, stepen); 
} 

void popraviDolje(vector<int> &v, int i, int &velicina);
void stvoriGomilu(vector<int> &v) {
    int velicina(v.size());
    for(int i(velicina/2); i>=0; i--)
        popraviDolje(v,i,velicina);
}
void umetniUGomilu(vector<int> &v, int umetnuti, int &velicina) {
    v.push_back(umetnuti);
    velicina++;
    
    function<void(int)> popraviGore = [&v](int i){
        while(i != 0  &&  v[i] > v[(i - 1) / 2]){
            swap(v[i],v[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    };
    
    popraviGore(velicina - 1);
}
int izbaciPrvi(vector<int> &v, int &velicina) {
    if(!velicina) 
        throw std::runtime_error("The heap is empty");
    velicina--;
    if(velicina){
        swap(v[0],v[velicina]);
        popraviDolje(v,0,velicina);
    }
    return v[velicina];
}
void gomilaSort(vector<int> &v) {
    stvoriGomilu(v);
    int velicina(v.size());
    for(int i(0); i<=v.size() - 2; i++)
        izbaciPrvi(v,velicina);
}

int main() { 
    return 0; 
} 

void popraviDolje(vector<int> &v, int i, int &velicina) {
    function<bool(int)> JeLiList = [velicina](int i) ->bool {return (i >= velicina/2) && (i < velicina);};
    function<int(int)> LijevoDijete = [](int i) -> int {return 2 * i + 1;};
    function<int(int)> DesnoDijete = [](int i) -> int {return 2 * i + 2;};

    while(!JeLiList(i)){ 
        int veci(LijevoDijete(i));
        int dd(DesnoDijete(i));
        if(dd < velicina  &&  v[dd] > v[veci]) 
            veci=dd;
        if(v[i] > v[veci]) 
            return;
        swap(v[i],v[veci]);
        i=veci;
    }
}