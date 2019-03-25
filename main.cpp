#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;


//================================================================//
//       klasa NEH
//----------------------------------------------------------------//
//  Klasa zawieraj¹ca pola i metody s³u¿¹ca do rozwi¹zania
//  problemu FSP za pomoc¹ algorytmu NEH
//----------------------------------------------------------------//
/*
   Pola:
       + int lzad     - liczba zadan
       + int lmasz    - liczba maszyn
       + int ile      - przechowije info, ile zadan jest na permutacji
       + int Cmax     - chwilowy maksymalny czas zakoñczenia pracy
       + int *perm    - tablica z permutacją zadañ (numerowane od 1)
       + int *dane    - wskaźnik na tablice danych [lzad][lmasz+1]
                        ostatnia kolumna to suma czasow wykonywania zadania
       + int *kolej   - tablica z kolejności zadañ do wrzucenia (nr od 1)
       + int *tab     - tablica pomocnicza [lmasz+1][lzad+1]
                        pierwszy wiersz i kolumna są zawsze zerowe

   Metody:

   Konstruktor inicjalizuje tablice. Wczytuje dane z pliku *nazwa
   do tablicy dane.
      NEH(char *nazwa);

   Destruktor .
      ~NEH();

   Uzupełnia *kolej danymi z *dane. Zdania ustawiane są ze względu
   na czas wykonywania w kolejności rosnącej.
      void Sort();

   Liczy maksymalną ścieżke dochodzącą w grafie przedstawionym
   za pomocą tablicy *tab. Obliczone Cmax ustwaiane jest w polu Cmax
   Zwracana wartoœæ: obliczone Cmax
      int LiczCmax();

   Analizuje kolejne wstawienia zadań. Wywołuje LiczCmax() i na podstawie
   wyników wyznacza najlepszą permutacjê zadan.
      int LiczPerm();

   Wypisuje permutacje z tablicy perm.
      void Wypisz();

*/
class NEH
{
  public:
  int lzad;
  int lmasz;
  int ile;
  int Cmax;
  int *perm;
  int **dane;
  int *kolej;
  int **tab;
//konstr
  NEH(char *nazwa);
  //destruk
  ~NEH();
  //sortuje rosnaco z pliku dane
  void Sort();
//
  int LiczCmax();
  //najlepsza permutacja zadan
  int LiczPerm(int zad);
  //wypisuje permutacje z tablicy perm
  void Wypisz();

};



  NEH::NEH(char *nazwa)
  {
  ile = 0; //ile zadan na permutacji
  fstream plik;
  char temp;
  plik.open( nazwa, ios::in );
  if( plik.good() )
    {
    //plik >> temp;  //jesli jest dodatkowy tekst w pliku
    plik >> lzad;
    plik >> lmasz;

    dane = new int *[lzad];
    for ( int i = 0; i < lzad; i++ )
        dane[i] = new int [lmasz+1];
    for(int i=0 ; i<lzad ; i++)
        dane[i][lmasz] = 0;
    for( int i = 0 ; i < lzad ; i++)
       for( int j = 0 ; j<lmasz ; j++)
      {
          plik >> dane[i][j]; //pominiêcie nr maszyny
          plik >> dane[i][j];
          dane[i][lmasz] += dane[i][j]; //suma czasów
      }
    perm = new int[lzad];
    for(int i=0 ; i<lzad ; i++)
      perm[i]=0; //pusta lista zadan na maszynie
    kolej = new int[lzad];
    for(int i=0 ; i<lzad ; i++)
      kolej[i]=i+1;

    tab = new int *[lmasz+1];
    for ( int i = 0; i < lmasz+1; i++ )
        tab[i] = new int [lzad+1];

    for( int i = 0 ; i < lmasz+1 ; i++)
       for( int j = 0 ; j<lzad+1 ; j++)
     tab[i][j]=0;
    plik.close();
    }

  }

  NEH::~NEH()
  {
  delete perm;
  delete dane;
  delete kolej;
  delete tab;
  }

  void NEH::Sort()
  { // wersja bąbelkowa
    int temp;
    for(int i=0 ; i<lzad ; i++)
      for(int j=0 ; j<lzad-1 ; j++)
        if(dane[kolej[j]-1][lmasz]<dane[kolej[j+1]-1][lmasz])
        {
        swap (dane[kolej[j]-1][lmasz], dane[kolej[j+1]-1][lmasz]);
       }

  }


  int NEH::LiczCmax()
  {
    for(int i=1; i<lmasz+1 ; i++)
      for(int j=1; j<ile+1 ; j++)
      //max(element poprzedni, element z góry)+wartoœæ w tym
      //punkcie, (tablica danych indeksowana odwrotnie oraz
      //bez zerowego wiersza i kolumny
         tab[i][j]=max(tab[i-1][j],tab[i][j-1])+dane[perm[j-1]-1][i-1];
    Cmax = tab[lmasz][ile];
    return Cmax;
  }

  int NEH::LiczPerm(int zad)
  {
    int cmax;
    int best = 0;
    int i;
    int temp;
    ile++; // zwiekszamy liczbe zadan na maszynie
    for(i=lzad-1 ; i>0 ; i--)
      perm[i]=perm[i-1]; //robimy miejsce
    perm[0]=zad;         //wstawiamy zadanie na początek
    cmax = LiczCmax();
    i=0;
    while(perm[i+1]!=0 && i<lzad-1 ) //liczymy kolejne mozliwosci
      {
      temp = perm[i+1];
      perm[i+1]=perm[i];
      perm[i]=temp;
      LiczCmax();
      if(Cmax<cmax)
        {
        cmax = Cmax;
        best=i+1;
        }
      i++;
      }
    // poprawiamy kolejnosc
    temp = perm[i];
    for(i ; i>best ; i--)
      perm[i]=perm[i-1];
    perm[best]=temp;
  }

  void NEH::Wypisz()
  {
    for(int i=0 ; i<lzad ; i++)
      if( perm[i] != 0)
         cout << perm[i] << " ";
    cout << endl;
  }


int main()
{
  char nazwa[20] = "data.txt";
  float czas;
  clock_t start, koniec;
  start = clock();
  NEH N(nazwa);
  N.Sort();
  for(int i=0 ; i<N.lzad ; i++)
  {
   N.LiczPerm(N.kolej[i]);
   N.Wypisz();
  }
  koniec = clock();
  czas = (float)(koniec - start) / CLOCKS_PER_SEC;
  cout <<  "\n czas dzialania: " << czas << endl;

  getchar();
  return 0;
}
