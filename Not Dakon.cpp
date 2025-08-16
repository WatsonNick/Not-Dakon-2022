#include <iostream>     // Input Output
#include <iomanip>      // Adjust 
#include <chrono>       // Time
#include <thread>       // Thread
#include <windows.h>    // Adjust Position
#include <conio.h>      // Keyboard Interrupt
using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

// Ada 16 lubang pada permainan congklak, dengan susunan sbb:
// 0..6  lubang kecil pemain 1, [7]  lubang besar pemain 1
// 8..14 lubang kecil pemain 2, [15] lubang besar pemain 2

  //                 1    2    3    4    5    6    7
  //          ___________________________________________
  //Pemain 2       (14) (13) (12) (11) (10) ( 9) ( 8)    
  //          (15)                                    ( 7)
  //Pemain 1       ( 0) ( 1) ( 2) ( 3) ( 4) ( 5) ( 6)
  //          -------------------------------------------
  // Isi 7 biji pada lubang kecil
int lubang[16];

//Bersihkan layar dan cetak papan dakon.
void cetak_papan() 
{
  system("cls");
  const char sep = ' ';
  const int W = 3;
  cout << endl <<"         ";
  for(int o = 1; o<=7; o++){
    cout<<left<<setw(W+2)<<setfill(' ')<<o;
  }
  cout << endl;
  for(int o = 1; o<=48; o++){
    cout <<"-";
  }
  cout << endl <<"      ";
  // Lubang kecil di atas
  for (int i = 14; i >= 8; i--) {
    cout<<" ("<<right<<setw(W-1)<<setfill(sep)<<lubang[i]<<")";
  }
  cout << endl;
  
  // Lumbung
  cout<<setw(3)<<"("<<right<<setw(W-1)<<setfill(sep)<<lubang[15] << ")";
  cout<<setw(37)<<"("<<setw(W-1)<<right<<setfill(sep)<<lubang[7]<< ")";
  
  // Lubang kecil di bawah
  cout <<endl<<"      ";
  for (int i = 0; i <= 6; i++) {
    cout<<" ("<<right<<setw(W-1)<<setfill(sep)<<lubang[i]<<")";
  }
  cout <<endl;
  for(int o = 1; o<=48; o++){
    cout <<"-";
  }
  cout << endl;
}

/* Cetak jumlah biji dalam genggaman di dekat lubang
 * 
 * index = Index lubang
 * jumlah = Banyaknya biji dalam genggaman
 *
 * Mengambil isi lubang dan mendistribusikan biji ke lubang di 
 * sebelahnya berlawanan jarum jam hingga habis.
 * 
 * @return Indeks lubang di mana biji terakhir ditaruh.
 */
int gerakkan(int index, int pemain) {
  // Pindahkan biji ke genggaman
  int genggaman = lubang[index];
  lubang[index] = 0;

  if(pemain == 1){
    char color = 4;
    string c = "color " + to_string(color);
    system(c.c_str());
  }
  else{
    char color = 1;
    string c = "color " + to_string(color);
    system(c.c_str());
  }

  // Perbaharui papan dan beri jeda waktu sebelum melanjutkan
  cetak_papan();
  cout << "Ambil lubang "; 
  if(index >=0 and index <= 7) cout << index+1 << endl;
  else{
    if(index == 8) cout << "7" << endl;
    if(index == 9) cout << "6" << endl;
    if(index == 10) cout << "5" << endl;
    if(index == 11) cout << "4" << endl;
    if(index == 12) cout << "3" << endl;
    if(index == 13) cout << "2" << endl;
    if(index == 14) cout << "1" << endl;
  }
  this_thread::sleep_for(chrono::milliseconds(750));
  int belum = 0;
  // Distribusikan biji berlawanan arah jarum jam
  while (genggaman > 0) {
    // Geser index ke lubang di sampingnya
    index = (index + 1) % 16;
    // Lumbung lawan agar tidak terisi
    while (((pemain == 1) && (index == 15)) || ((pemain == 2) && (index == 7))) {
      index = (index + 1) % 16;
    }
    
    // Pindahkan satu biji dari genggaman ke lubang
    lubang[index]++;
    genggaman--;

    char pencet;
    if(belum == 0){
      cout << "Tekan F untuk percepat" << endl;
      cout << "Tekan tombol lain untuk perlambat" << endl;
      pencet = _getch();
      belum = -1;
    }
    // Perbaharui papan dan penunjuk 
    cetak_papan();
    if(index >= 0 and index <= 7){
      gotoxy(9+5*index,6); cout << ">" << endl;      
    }
    else{
      gotoxy(79-index*5,2); cout << "<" << endl;      
      gotoxy(0,7);
    }
    //Menentukan jeda waktu sebelum melanjutkan
    cout << "Biji di Tangan Pemain " << pemain <<": " << genggaman << endl;
    if(pencet == 'F' or pencet == 'f') this_thread::sleep_for(chrono::milliseconds(1));
    else this_thread::sleep_for(chrono::milliseconds(750));
    
  }
  return index;
}

void menembak_biji(int index, int pemain) {
  // Pindahkan biji ke genggaman
    int genggaman = lubang[index];
    lubang[index] = 0;
    
    // Perbaharui papan dan beri jeda waktu sebelum melanjutkan
    // cetak_papan();
    // - pemain 1 = lubang[7], - pemain 2 = lubang[15]
    if (pemain == 1) index = 7;
    else index = 15;

    lubang[index] += genggaman;
    genggaman = 0;
  
    // Perbaharui papan dan beri jeda waktu sebelum melanjutkan
    cetak_papan();
}

/* Meminta pemain memilih lubang 1..7 dan mengembalikan indeks lubang 
 * yang dipilih.
 */
int pilih_lubang(int pemain) {
  int input, index;
  
  char color = 7;
  string c = "color " + to_string(color);
  system(c.c_str());
  // Minta input dari pemain
  cout << "Pilih lubang 1 -- 7  | ";
  cin>>input;

  if (pemain == 1) index = input - 1;
  else index = 14 - (input - 1);
  
  // Jika lubang yang dipilih kosong dan lebih dari 7 atau kurang dari 1, ulangi permintaan input
  if (lubang[index] <= 0 || input < 1 || input > 7) return pilih_lubang(pemain);
  else return index;
}

//lubang di salah satu sisi pemain sudah kosong permainan selesai.
bool permainan_selesai() {
  // Pengecekan sisi pemain 1
  bool selesai = true;
  for (int i = 0; i <= 6; i++) {
    if (lubang[i] > 0) {
      selesai = false;
      break;
    }
  }
  
  // Jika sisi pemain 1 masih terisi, coba cek sisi pemain 2
  if (selesai == false) {
    selesai = true;
    for (int i = 8; i <= 14; i++) {
      if (lubang[i] > 0) {
        selesai = false;
        break;
      }
    }
  }
  
  return selesai;
}

void jalan_permainan(int pemain) {
  int index = -1;
  
  // Selama permainan belum selesai akan lanjut
  while (permainan_selesai() == false) {
    // Cetak kembali papan
    cetak_papan();
    
    // Minta input dari pemain
    // index -1 berarti lubang belum dipilih.
    if (index == -1) {
      cout << "GILIRAN PEMAIN " << pemain << endl;
      index = pilih_lubang(pemain);
    }
  
    // Lakukan distribusi biji
    index = gerakkan(index, pemain);
    
    // Jika berakhir di rumah pemain, pemain kembali mendapat 
    // giliran memilih lubang dan mendistribusikan.

    if (index == 7 || index == 15) index = -1;
    // Reset index agar pemain bisa memilih kembali
    
    // Jika berakhir di lubang kecil dan tidak kosong, pemain kembali 
    // mendapat giliran mendistribusikan dari lubang terakhir
    // Lubang kosong akan berisi 1 biji karena ditaruh biji terakhir.
    else if (lubang[index] > 1) {
      // Tidak perlu mereset index
    }
    
    // Jika berakhir di lubang kosong
    else {
      // Jika berakhir di sisi milik pemain, pindahkan biji 
      // di lubang terakhir, juga seluruh biji di lubang seberangnya.
      
      // Pengecekan untuk pemain 1
      if ((pemain == 1) && (index >= 0) && (index <= 6) && (lubang[14 - index] >= 1)) {
          menembak_biji(14 - index, pemain);
          menembak_biji(index, pemain);
        }
      // Pengecekan untuk pemain 2
      else if ((pemain == 2) && (index >= 8) && (index <= 14) && (lubang[14 - index] >= 1)) {
          menembak_biji(14 - index, pemain);
          menembak_biji(index, pemain);
        }
      
      // Ganti giliran pemain
      index = -1;
      pemain = (pemain % 2) + 1;
    }
  }
}

void alur_babak() {
  //int babak = 0;
  int pemain = 1;
  
  // Pada awal permainan, beri 49 biji di rumah masing-masing
  // Isi 7 biji pada lubang kecil pemain 1
  for (int i = 0; i <= 6; i++) {
    lubang[i] = 7;
  }
  
  // Kosongkan rumah pemain 1 dan pemain 2
  lubang[7] = 0;
  lubang[15] = 0;
  
  // Isi 7 biji pada lubang kecil pemain 2
  for (int i = 8; i <= 14; i++) {
    lubang[i] = 7;
  }

  while (permainan_selesai() == false){
    // Kontrol alur permainan
    jalan_permainan(pemain);
  }
  cetak_papan();
}

//Instruction
void instruction(){
  system("cls");
  cout <<"                 1    2    3    4    5    6    7 "        << endl;
  cout <<"          <-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<-<"    << endl;
  cout <<"Pemain 2       ( 7) ( 7) ( 7) ( 7) ( 7) ( 7) ( 7) "       << endl;    
  cout <<"          ( 0)                                    ( 0) "  << endl;
  cout <<"Pemain 1       ( 7) ( 7) ( 7) ( 7) ( 7) ( 7) ( 7) "       << endl;
  cout <<"          ->->->->->->->->->->->->->->->->->->->->->->-"    << endl;
  cout <<"\n Aturan Permainan: " << endl;
  cout << "1. Permainan dilakukan oleh dua orang." << endl;
  cout << "\n2. Masing-masing pemain dibagi tujuh lubang dan satu lumbung besar." << endl;
  cout << "\n3. pemain secara bergantian memilih satu lubang kecil miliknya untuk memindahkan biji-bijinya" << endl;
  cout << "   1 per 1 ke lubang lainnya berlawanan arah jarum jam, hingga biji yang digenggamannya habis." << endl;
  cout << "\n4. Apabila biji di genggaman habis, tetapi berhenti di lubang yang berisi biji di wilayah pemain," << endl;
  cout << "   pemain akan mengambil seluruh biji pada lubang tersebut dan melanjutkan permainan." << endl;
  cout << "\n5. Saat biji pemain 1 berhenti di lubang milik pemain 1 yang kosong, pemain 1 akan menembak biji" << endl; 
  cout << "   milik pemain 2 di seberangnya juga biji terakhir yang pemain 1, begitu pula sebaliknya." << endl;
  cout << "\n6. Apabila berhenti di lubang milik lawan, giliran pemain tersebut selesai." << endl;
  cout << "\n7. Permainan selesai saat salah satu pemain sudah tidak memiliki biji di lubang miliknya." << endl; 
  cout << "\n8. Pemenangnya adalah pemain yang di lumbung memiliki biji baling sedikit." << endl;

  _getch();
}

//Main menu
bool play = true;
void menu() {
  while(play){
    system("cls");
    gotoxy(10,3); cout << "_______________________________" << endl;
    gotoxy(10,4); cout << "  SELAMAT DATANG DI NOT DAKON  " << endl;
    gotoxy(10,5); cout << "===============================" << endl;
    gotoxy(10,6); cout << "     MENU               " << endl;
    gotoxy(10,7); cout << "-------------------------------" << endl;
    gotoxy(10,8); cout << "  1. MAIN               " << endl;
    gotoxy(10,9); cout << "  2. CARA BERMAIN        " << endl;
    gotoxy(10,10); cout << "  3. KELUAR               " << endl;
    gotoxy(10,11); cout << "_______________________________" << endl;
    gotoxy(10,12); cout << "  PILIH MENU: ";
    char choice = _getch();
    if(choice == '1') {
      alur_babak();
      play = false;
    }
    if(choice == '2'){ 
      instruction();
    }
    if(choice == '3') exit(0);
  }
}
void tentukan_pemenang()
{
    //Menghitung jumlah biji untuk pemain 1 & 2
    int jumlahbiji1 = 0, jumlahbiji2 = 0;
    for (int i = 0; i<7; i++){
      jumlahbiji1 += lubang[i];
    }
    jumlahbiji1 = jumlahbiji1 + lubang[7]; 
    cout<<"PEMAIN 1 "<<jumlahbiji1<<endl;

    for (int i = 8; i<15; i++){
      jumlahbiji2 += lubang[i];
    }
    jumlahbiji2 = jumlahbiji2 + lubang[15]; 
    cout<<"PEMAIN 2 "<<jumlahbiji2<<endl;

    for(int i=1;i<47;i++){
       cout << "=";
    }
    cout << endl;
    
    // Tentukan pemenangnya
    if (jumlahbiji1 < jumlahbiji2)  cout << "PEMAIN 1 MENANG" << endl;
    else if (jumlahbiji1 > jumlahbiji2) cout << "PEMAIN 2 MENANG" << endl;
    else cout << "SERI" << endl;
}


int main(){
    menu();

    char color = 7;
    string c = "color " + to_string(color);
    system(c.c_str());
    cout << "PERMAINAN SELESAI" << endl;
    tentukan_pemenang();
    cout << "\nTEKAN TOMBOL APAPUN UNTUK KELUAR" << endl;
    _getch();
    exit(0);
    return 0;
}
