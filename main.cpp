/*
Studi kasus
Pengisian KRS oleh mahasiswa di awal semester

mahasiswa ambil mata kuliah, dimasukkan ke KRS
mahasiswa ada data mata kuliah yang sudah diambil
mahasiswa bisa cetak KRS yang berisi daftar mata kuliah
mahasiswa punya jatah SKS maksimal
mata kuliah ada dosen pengampu, punya jadwal, ada kuota, SKS
kuota penuh, mata kuliah tidak diambil
mata kuliah yang jadwalnya bersamaan dengan mata kuliah lain tidak bisa diambil 
mata kuliah ada prasyarat
mahasiswa hanya bisa ambil mata kuliah yang prasyaratnya sudah terpenuhi
mata kuliah ada jenisnya : wajib, pilihan

Buat program untuk mensimulasikan studi kasus tersebut
*/

// Asumsi:
// - Tiap matakuliah hanya memiliki 1 kelas dan 1 set jadwal dan dengan demikian 1 dosen pengampu
//
// Hak Cipta (C) 2021, Michael Raditya Krisnadhi

#include <iostream>
#include <list>
#include <map>
#include <string>


//==============================================================================
// Tipe Data
//==============================================================================

enum JenisMKWajib {
    MKWajibUniversitas,
    MKWajibProdi
};

enum HariID {
    HariSenin,
    HariSelasa,
    HariRabu,
    HariKamis,
    HariJumat
};

char g_hariStr[][10] = {"SENIN", "SELASA", "RABU", "KAMIS", "JUMAT"};


//==============================================================================
// Deklarasi dan Definisi Class
//==============================================================================

// Deklarasi class
class Mahasiswa;
class KRS;
class Matakuliah;
class MKWajib;
class MKPilihan;
class Jadwal;
class Dosen;

// Entity: Mahasiswa (strong)
// Relasi:
//  1-N dengan KRS
class Mahasiswa {
private:
    std::string m_nama; // nama mahasiswa

    std::map<int, KRS *> m_dataKRS; // mapping Semester -> KRS

public:
    Mahasiswa(void);
    Mahasiswa(const std::string &nama);
    ~Mahasiswa(void);

    // getters
    const std::string &getNama(void) const;

    void addKRS(int semester, KRS *krs); // tambah KRS pada semester tertentu
    KRS *getKRS(int semester); // akses KRS pada semester tertentu
    std::map<int, KRS *>::iterator beginKRS(void); // iterate KRS (begin)
    std::map<int, KRS *>::iterator endKRS(void); // iterate KRS (end)
    std::map<int, KRS *>::const_iterator beginKRS(void) const; // iterate KRS (begin const)
    std::map<int, KRS *>::const_iterator endKRS(void) const; // iterate KRS (end const)
    void deleteKRS(int semester); // hapus KRS pada semester tertentu
};

// Entity: KRS (weak oleh mahasiswa)
// Relasi:
//  N-1 dengan Mahasiswa
//  N-N dengan Matakuliah
class KRS {
private:
    const Mahasiswa *m_pemilik; // pemilik KRS
    std::list<const Matakuliah *> m_daftarMakul; // daftar matakuliah dalam linked list

public:
    KRS(void);
    KRS(const Mahasiswa *pemilik);

    const Mahasiswa *getPemilik(void) const; // dapatkan pemilik KRS

    void addMakul(const Matakuliah *makul); // tambah matakuliah di akhir list
    std::list<const Matakuliah *>::iterator beginMakul(void); // iterate daftar matakuliah (begin)
    std::list<const Matakuliah *>::iterator endMakul(void); // iterate daftar matakuliah (end)
    std::list<const Matakuliah *>::const_iterator beginMakul(void) const; // iterate daftar matakuliah (begin const)
    std::list<const Matakuliah *>::const_iterator endMakul(void) const; // iterate daftar matakuliah (end const)
    void deleteMakul(const std::string &kodeMK); // hapus matakuliah dari list berdasarkan kode matakuliah

    int getTotalMakul(void) const; // total matakuliah
    int getTotalSKS(void) const; // total bobot SKS
};

// Entity: Matakuliah (strong)
// Relasi:
//  N-N dengan KRS
//  N-N dengan Matakuliah
//  N-N dengan Jadwal
//  N-1 dengan Dosen
class Matakuliah {
private:
    std::string m_nama; // nama matakuliah
    int m_bobot; // bobot dalam SKS
    int m_kapasitas; // kapasitas matakuliah
    int m_peserta; // peserta matakuliah

    std::list<const Matakuliah *> m_prasyarat; // prasyarat matakuliah
    std::list<Jadwal *> m_daftarJadwal; // daftar sesi matakuliah
    const Dosen *m_pengampu; // dosen pengampu matakuliah

public:
    Matakuliah(void);
    Matakuliah(const std::string &nama, int bobot, int kapasitas);
    ~Matakuliah(void);

    // getters
    const std::string &getNama(void) const;
    int getBobot(void) const;
    int getKapasitas(void) const;
    int getPeserta(void) const;

    void tambahMahasiswa(void); // tambah mahasiswa ke matakuliah
    void kurangiMahasiswa(void); // kurangi mahasiswa dari matakuliah

    void addPrasyarat(const Matakuliah *prasyarat); // tambah matakuliah prasyarat
    std::list<const Matakuliah *>::const_iterator beginPrasyarat(void) const; // iterate matakuliah prasyarat (begin const)
    std::list<const Matakuliah *>::const_iterator endPrasyarat(void) const; // iterate matakuliah prasyarat (end const)

    void addJadwal(Jadwal *jadwal); // tambah jadwal
    void deleteJadwal(std::list<Jadwal *>::iterator jadwalIt); // hapus jadwal berdasarkan iterator
    std::list<Jadwal *>::iterator beginJadwal(void); // iterate daftar jadwal (begin)
    std::list<Jadwal *>::iterator endJadwal(void); // iterate daftar jadwal (end)
    std::list<Jadwal *>::const_iterator beginJadwal(void) const; // iterate daftar jadwal (begin const)
    std::list<Jadwal *>::const_iterator endJadwal(void) const; // iterate daftar jadwal (end const)

    virtual bool isPilihan(void) const = 0;

    void setPengampu(const Dosen *pengampu); // atur dosen pengampu
    const Dosen *getPengampu(void) const; // dapatkan dosen pengampu

    bool cekPrasyarat(const Mahasiswa *mhs, int semester) const; // cek prasyarat apakah terpenuhi
    bool cekTumbukan(const Matakuliah *lain) const; // cek tumbukan dengan matakuliah lain
};

// Entity: MKWajib (inherit Matakuliah)
class MKWajib : public Matakuliah {
private:
    JenisMKWajib m_jenis; // jenis matakuliah (wajib universitas/fakultas/prodi/peminatan)

public:
    MKWajib(void);
    MKWajib(const std::string &nama, int bobot, int kapasitas, JenisMKWajib jenis);

    // getters
    JenisMKWajib getJenis(void) const;

    bool isPilihan(void) const;
};

// Entity: MKPilihan (inherit Matakuliah)
class MKPilihan : public Matakuliah {
private:
    std::string m_labRiset; // lab riset penyedia matakuliah pilihan

public:
    MKPilihan(void);
    MKPilihan(const std::string &nama, int bobot, int kapasitas, const std::string &labRiset);

    // getters
    const std::string &getLabRiset(void) const;

    bool isPilihan(void) const;
};

// Entity: Jadwal (weak oleh Matakuliah)
// Relasi:
//  N-N dengan Matakuliah
class Jadwal {
private:
    HariID m_hari;
    int m_sesi;

public:
    Jadwal(void);
    Jadwal(HariID hari, int sesi);

    // getters
    HariID getHari(void) const;
    int getSesi(void) const;

    bool cekTumbukan(const Jadwal *lain) const; // cek bentrokan/tumbukan dengan jadwal lain
};

// Entity: Dosen (strong)
// Relasi:
//  1-N dengan Matakuliah
class Dosen {
private:
    std::string m_nama; // nama dosen

public:
    Dosen(void);
    Dosen(const std::string &nama);

    // getters
    const std::string &getNama(void) const;
};


//==============================================================================
// Database
//==============================================================================

std::map<std::string, Mahasiswa *> g_mahasiswaDB;
std::map<std::string, Matakuliah *> g_matakuliahDB;
std::map<std::string, Dosen *> g_dosenDB;

void generateMahasiswaDB(void) {
    g_mahasiswaDB["M0520001"] = new Mahasiswa("Adrian Smith");
    g_mahasiswaDB["M0520002"] = new Mahasiswa("Bruce Dickinson");
    g_mahasiswaDB["M0520003"] = new Mahasiswa("Dave Murray");
    g_mahasiswaDB["M0520004"] = new Mahasiswa("Janick Gers");
    g_mahasiswaDB["M0520005"] = new Mahasiswa("Nicko McBrain");
    g_mahasiswaDB["M0520006"] = new Mahasiswa("Steve Harris");
}

void generateMatakuliahDB(void) {
    g_matakuliahDB["IF3110"] = new MKWajib("Pengembangan Berbasis Web", 2, 5, MKWajibProdi);
    g_matakuliahDB["IF3111"] = new MKWajib("Pengembangan Berbasis Platform", 2, 5, MKWajibProdi);
    g_matakuliahDB["IF3130"] = new MKWajib("Jaringan Komputer", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3140"] = new MKWajib("Manajemen Basis Data", 2, 5, MKWajibProdi);
    g_matakuliahDB["IF3151"] = new MKWajib("Interaksi Manusia dan Komputer", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3170"] = new MKWajib("Kecerdasan Buatan", 4, 5, MKWajibProdi);
    g_matakuliahDB["IF3150"] = new MKWajib("Manajemen Proyek", 2, 5, MKWajibProdi);

    g_matakuliahDB["IF3230"] = new MKWajib("Sistem Paralel dan Terdistribusi", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3240"] = new MKWajib("Sistem Informasi", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3250"] = new MKWajib("Proyek Perangkat Lunak", 4, 5, MKWajibProdi);
    g_matakuliahDB["IF3260"] = new MKWajib("Grafika Komputer", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3280"] = new MKWajib("Sosio-informatika dan Profesionalisme", 3, 5, MKWajibProdi);
    g_matakuliahDB["KU206X"] = new MKWajib("Agama dan Budipekerti", 2, 5, MKWajibUniversitas);

    g_matakuliahDB["IF3230"]->addPrasyarat(g_matakuliahDB["IF3130"]);
    g_matakuliahDB["IF3240"]->addPrasyarat(g_matakuliahDB["IF3110"]);
    g_matakuliahDB["IF3240"]->addPrasyarat(g_matakuliahDB["IF3140"]);
    g_matakuliahDB["IF3250"]->addPrasyarat(g_matakuliahDB["IF3150"]);
}

void generateDosenDB(void) {
    g_dosenDB["WIN001"] = new Dosen("Winarno");
    g_dosenDB["ARD001"] = new Dosen("Ardhi Wijayanto");
    g_dosenDB["ABD001"] = new Dosen("Abdul Aziz");
    g_dosenDB["DEW001"] = new Dosen("Dewi Wisnu Wardani");
    g_dosenDB["SAR001"] = new Dosen("Sari Widya Sihwi");
    g_dosenDB["WIH001"] = new Dosen("Wiharto");
    g_dosenDB["WIS001"] = new Dosen("Wisnu Widiarto");
    g_dosenDB["HAR001"] = new Dosen("Haryono Setiadi");
    g_dosenDB["HER001"] = new Dosen("Heri Prasetyo");
}

void generateDosenPengampuDB(void) {
    g_matakuliahDB["IF3110"]->setPengampu(g_dosenDB["WIN001"]);
    g_matakuliahDB["IF3111"]->setPengampu(g_dosenDB["ARD001"]);
    g_matakuliahDB["IF3130"]->setPengampu(g_dosenDB["ABD001"]);
    g_matakuliahDB["IF3140"]->setPengampu(g_dosenDB["DEW001"]);
    g_matakuliahDB["IF3151"]->setPengampu(g_dosenDB["SAR001"]);
    g_matakuliahDB["IF3170"]->setPengampu(g_dosenDB["WIH001"]);
    g_matakuliahDB["IF3150"]->setPengampu(g_dosenDB["HAR001"]);

    g_matakuliahDB["IF3230"]->setPengampu(g_dosenDB["WIS001"]);
    g_matakuliahDB["IF3240"]->setPengampu(g_dosenDB["WIN001"]);
    g_matakuliahDB["IF3250"]->setPengampu(g_dosenDB["ARD001"]);
    g_matakuliahDB["IF3260"]->setPengampu(g_dosenDB["HER001"]);
    g_matakuliahDB["IF3280"]->setPengampu(g_dosenDB["HAR001"]);
    g_matakuliahDB["KU206X"]->setPengampu(NULL);
}

void generateJadwalMatakuliahDB(void) {
    g_matakuliahDB["IF3110"]->addJadwal(new Jadwal(HariSenin, 1));
    g_matakuliahDB["IF3110"]->addJadwal(new Jadwal(HariSenin, 2));
    g_matakuliahDB["IF3111"]->addJadwal(new Jadwal(HariSelasa, 3));
    g_matakuliahDB["IF3111"]->addJadwal(new Jadwal(HariSelasa, 4));
    g_matakuliahDB["IF3130"]->addJadwal(new Jadwal(HariSenin, 4));
    g_matakuliahDB["IF3130"]->addJadwal(new Jadwal(HariSenin, 5));
    g_matakuliahDB["IF3130"]->addJadwal(new Jadwal(HariSenin, 6));
    g_matakuliahDB["IF3140"]->addJadwal(new Jadwal(HariRabu, 2));
    g_matakuliahDB["IF3140"]->addJadwal(new Jadwal(HariRabu, 3));
    g_matakuliahDB["IF3151"]->addJadwal(new Jadwal(HariRabu, 4));
    g_matakuliahDB["IF3151"]->addJadwal(new Jadwal(HariRabu, 5));
    g_matakuliahDB["IF3151"]->addJadwal(new Jadwal(HariKamis, 1));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(HariKamis, 4));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(HariKamis, 5));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(HariKamis, 6));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(HariJumat, 2));
    g_matakuliahDB["IF3150"]->addJadwal(new Jadwal(HariJumat, 3));
    g_matakuliahDB["IF3150"]->addJadwal(new Jadwal(HariJumat, 4));

    g_matakuliahDB["IF3230"]->addJadwal(new Jadwal(HariSenin, 1));
    g_matakuliahDB["IF3230"]->addJadwal(new Jadwal(HariSenin, 2));
    g_matakuliahDB["IF3230"]->addJadwal(new Jadwal(HariSenin, 3));
    g_matakuliahDB["IF3240"]->addJadwal(new Jadwal(HariSenin, 4));
    g_matakuliahDB["IF3240"]->addJadwal(new Jadwal(HariSenin, 5));
    g_matakuliahDB["IF3240"]->addJadwal(new Jadwal(HariSenin, 6));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(HariRabu, 3));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(HariRabu, 4));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(HariRabu, 5));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(HariKamis, 2));
    g_matakuliahDB["IF3260"]->addJadwal(new Jadwal(HariKamis, 3));
    g_matakuliahDB["IF3260"]->addJadwal(new Jadwal(HariKamis, 4));
    g_matakuliahDB["IF3260"]->addJadwal(new Jadwal(HariKamis, 5));
    g_matakuliahDB["IF3280"]->addJadwal(new Jadwal(HariJumat, 1));
    g_matakuliahDB["IF3280"]->addJadwal(new Jadwal(HariJumat, 2));
    g_matakuliahDB["IF3280"]->addJadwal(new Jadwal(HariJumat, 3));
    g_matakuliahDB["KU206X"]->addJadwal(new Jadwal(HariJumat, 5));
    g_matakuliahDB["KU206X"]->addJadwal(new Jadwal(HariJumat, 6));
}

void generateDB(void) {
    generateMahasiswaDB();
    generateMatakuliahDB();
    generateDosenDB();
    generateDosenPengampuDB();
    generateJadwalMatakuliahDB();
}

void destroyDB(void) {
    for (
        std::map<std::string, Mahasiswa *>::iterator it = g_mahasiswaDB.begin();
        it != g_mahasiswaDB.end();
        ++it
    ) {
        delete it->second;
    }
    g_mahasiswaDB.clear();

    for (
        std::map<std::string, Matakuliah *>::iterator it = g_matakuliahDB.begin();
        it != g_matakuliahDB.end();
        ++it
    ) {
        delete it->second;
    }
    g_matakuliahDB.clear();

    for (
        std::map<std::string, Dosen *>::iterator it = g_dosenDB.begin();
        it != g_dosenDB.end();
        ++it
    ) {
        delete it->second;
    }
    g_dosenDB.clear();
}


//==============================================================================
// Entry point
//==============================================================================

struct Operasi {
    std::string deskripsi;
    void (*callback)(void);

    Operasi(const std::string deskripsi, void (*callback)(void)) {
        this->deskripsi = deskripsi;
        this->callback = callback;
    }
};

std::map<std::string, Operasi *> g_perintah;
bool g_running = true;
Mahasiswa *g_user = NULL;

void cbHelp(void) {
    std::cout << "LIST PERINTAH" << std::endl;
    for (
        std::map<std::string, Operasi *>::iterator it = g_perintah.begin();
        it != g_perintah.end();
        ++it
    ) {
        std::cout << " " << it->first << " - " << it->second->deskripsi << std::endl;
    }
}

void cbLogin(void) {
    std::string nim;
    std::cout << "Masukkan NIM anda: ";
    std::cin >> nim;
    if (g_mahasiswaDB.count(nim) == 1) {
        g_user = g_mahasiswaDB[nim];
        std::cout << "SUKSES! Berhasil login sebagai: " << g_user->getNama() << std::endl;
    } else {
        std::cout << "NIM anda tidak ditemukan :(" << std::endl;
    }
}

void cbDosenList(void) {
    std::cout << "LIST DOSEN" << std::endl;
    for (
        std::map<std::string, Dosen *>::iterator it = g_dosenDB.begin();
        it != g_dosenDB.end();
        ++it
    ) {
        std::cout << " " << it->first << " - " << it->second->getNama() << std::endl;
    }
}

void cbMakulList(void) {
    std::cout << "LIST MATAKULIAH" << std::endl;
    for (
        std::map<std::string, Matakuliah *>::iterator it = g_matakuliahDB.begin();
        it != g_matakuliahDB.end();
        ++it
    ) {
        std::string flag;
        if (it->second->isPilihan()) {
            flag = "PILIHAN";
        } else {
            flag = "WAJIB";
        }

        std::cout << " " << it->first << " - " << it->second->getNama() << " - " << it->second->getBobot() << " SKS [" << flag << "]" << std::endl;
    }
}

void cbMakulInfo(void) {
    std::cout << "INFO MATAKULIAH" << std::endl;

    std::string kode;
    std::cout << " Kode MK : ";
    std::cin >> kode;

    if (g_matakuliahDB.count(kode) == 1) {
        Matakuliah *makul = g_matakuliahDB[kode];

        std::cout << " Nama MK : " << makul->getNama() << std::endl;
        std::cout << " Jenis MK : " << (makul->isPilihan() ? "PILIHAN" : "WAJIB") << std::endl;
        std::cout << " Bobot : " << makul->getBobot() << " SKS" << std::endl;
        std::cout << " Dosen Pengampu : " << (makul->getPengampu() != NULL ? makul->getPengampu()->getNama() : "(dosen luar prodi)") << std::endl;
        std::cout << " Peserta : " << makul->getPeserta() << " / " << makul->getKapasitas() << std::endl;

        std::cout << " Prasyarat : ";
        if (makul->beginPrasyarat() != makul->endPrasyarat()) {
            for (
                std::list<const Matakuliah *>::const_iterator it = makul->beginPrasyarat();
                it != makul->endPrasyarat();
                ++it
            ) {
                std::cout << (*it)->getNama();
                ++it;
                if (it == makul->endPrasyarat()) {
                    std::cout << std::endl;
                } else {
                    std::cout << ", ";
                }
                --it;
            }
        } else {
            std::cout << "-" << std::endl;
        }

        std::cout << " Jadwal :" << std::endl;

        for (
            std::list<Jadwal *>::iterator it = makul->beginJadwal();
            it != makul->endJadwal();
            ++it
        ) {
            std::cout << "  " << g_hariStr[(*it)->getHari()] << " sesi " << (*it)->getSesi() << std::endl;
        }
    } else {
        std::cout << "Matakuliah tidak ditemukan :(" << std::endl;
    }
}

void cbKRSList(void) {
    if (g_user == NULL) {
        std::cout << "Login terlebih dahulu dengan mengetik `login`" << std::endl;
        return;
    }

    std::cout << "LIST KARTU RENCANA STUDI" << std::endl;
    for (
        std::map<int, KRS *>::iterator it = g_user->beginKRS();
        it != g_user->endKRS();
        ++it
    ) {
        std::cout << " - Semester " << it->first << " : " << it->second->getTotalMakul() << " matakuliah (total " << it->second->getTotalSKS() << " SKS)" << std::endl;
    }
}

void cbKRSNew(void) {
    if (g_user == NULL) {
        std::cout << "Login terlebih dahulu dengan mengetik `login`" << std::endl;
        return;
    }

    std::cout << "BUAT KRS BARU" << std::endl;

    int semester;
    std::cout << " Semester : ";
    std::cin >> semester;

    if (g_user->getKRS(semester) == NULL) {
        g_user->addKRS(semester, new KRS(g_user));
        std::cout << "SUKSES! Berhasil menambahkan KRS semester " << semester << std::endl;
    } else {
        std::cout << "Anda sudah memiliki KRS semester " << semester << std::endl;
    }
}

void cbKRSTambahmk(void) {
    if (g_user == NULL) {
        std::cout << "Login terlebih dahulu dengan mengetik `login`" << std::endl;
        return;
    }

    std::cout << "TAMBAHKAN MATAKULIAH KE KRS" << std::endl;

    int semester;
    std::cout << " Semester : ";
    std::cin >> semester;

    KRS *krs = g_user->getKRS(semester);
    if (krs != NULL) {
        std::string kodeMK;
        std::cout << " Kode MK : ";
        std::cin >> kodeMK;

        if (g_matakuliahDB.count(kodeMK) == 1) {
            Matakuliah *makul = g_matakuliahDB[kodeMK];

            if (makul->getPeserta() < makul->getKapasitas()) {
                if (makul->cekPrasyarat(g_user, semester)) {
                    bool tumbukan = false;
                    for (
                        std::list<const Matakuliah *>::iterator it = krs->beginMakul();
                        it != krs->endMakul();
                        ++it
                    ) {
                        if (makul->cekTumbukan(*it)) {
                            std::cout << "Terdapat tumbukan dengan matakuliah " << (*it)->getNama() << " :(" << std::endl;
                            tumbukan = true;
                            break;
                        }
                    }

                    if (!tumbukan) {
                        std::cout << " Nama : " << makul->getNama() << std::endl;
                        std::cout << " Bobot : " << makul->getBobot() << std::endl;

                        if (krs->getTotalSKS() + makul->getBobot() <= 24) {
                            krs->addMakul(makul);
                            makul->tambahMahasiswa();
                            std::cout << "SUKSES! Matakuliah berhasil ditambahkan" << std::endl;
                        } else {
                            std::cout << "Kapasitas SKS tidak cukup (maks hanya 24) :(" << std::endl;
                        }
                    }
                } else {
                    std::cout << "Maaf! Prasyarat matakuliah belum terpenuhi :(" << std::endl;
                }
            } else {
                std::cout << "Maaf! Kuota matakuliah sudah habis :(" << std::endl;
            }
        } else {
            std::cout << "Matakuliah tidak ditemukan :(" << std::endl;
        }
    } else {
        std::cout << "Anda belum memiliki KRS semester " << semester << " :(" << std::endl;
    }
}

void cbKRSHapusmk(void) {
    if (g_user == NULL) {
        std::cout << "Login terlebih dahulu dengan mengetik `login`" << std::endl;
        return;
    }

    std::cout << "HAPUS MATKULIAH DARI KRS" << std::endl;

    int semester;
    std::cout << " Semester : ";
    std::cin >> semester;

    KRS *krs = g_user->getKRS(semester);
    if (krs != NULL) {
        std::string kodeMK;
        std::cout << " Kode MK : ";
        std::cin >> kodeMK;

        if (g_matakuliahDB.count(kodeMK) == 1) {
            std::cout << "Menghapus matakuliah " << g_matakuliahDB[kodeMK]->getNama() << "..." << std::endl;
            krs->deleteMakul(kodeMK);
        } else {
            std::cout << "Matakuliah tidak ditemukan :(" << std::endl;
        }
    } else {
        std::cout << "Anda belum memiliki KRS semester " << semester << " :(" << std::endl;
    }
}

void cbKRSCetak(void) {
    if (g_user == NULL) {
        std::cout << "Login terlebih dahulu dengan mengetik `login`" << std::endl;
        return;
    }

    std::cout << "CETAK KRS" << std::endl;

    int semester;
    std::cout << " Semester : ";
    std::cin >> semester;

    KRS *krs = g_user->getKRS(semester);
    if (krs != NULL) {
        std::cout << " Total matakuliah : " << krs->getTotalMakul() << std::endl;
        std::cout << " Total SKS : " << krs->getTotalSKS() << " / 24" << std::endl;
        for (
            std::list<const Matakuliah *>::iterator it = krs->beginMakul();
            it != krs->endMakul();
            ++it
        ) {
            std::cout << " - " << (*it)->getNama() << " (" << (*it)->getBobot() << " SKS)" << std::endl;
        }
    } else {
        std::cout << "Anda belum memiliki KRS semester " << semester << " :(" << std::endl;
    }
}

void cbKRSDelete(void) {
    if (g_user == NULL) {
        std::cout << "Login terlebih dahulu dengan mengetik `login`" << std::endl;
        return;
    }

    std::cout << "HAPUS KRS" << std::endl;

    int semester;
    std::cout << " Semester : ";
    std::cin >> semester;

    KRS *krs = g_user->getKRS(semester);
    if (krs != NULL) {
        g_user->deleteKRS(semester);
        std::cout << "SUKSES! KRS berhasil dihapus" << std::endl;
    } else {
        std::cout << "Anda belum memiliki KRS semester " << semester << " :(" << std::endl;
    }
}

void cbLogout(void) {
    g_running = false;
}

int main(void) {
    std::cout << " === SIAKAD CLI v1.0 === " << std::endl;
    std::cout << "Generating database ...";
    generateDB();
    std::cout << " done" << std::endl;

    g_perintah["help"] = new Operasi("tampilkan daftar perintah", cbHelp);
    g_perintah["login"] = new Operasi("login sebagai mahasiswa", cbLogin);
    g_perintah["dosen_list"] = new Operasi("tampilkan daftar dosen", cbDosenList);
    g_perintah["makul_list"] = new Operasi("tampilkan daftar matakuliah", cbMakulList);
    g_perintah["makul_info"] = new Operasi("tampilkan informasi matakuliah", cbMakulInfo);
    g_perintah["krs_list"] = new Operasi("tampilkan daftar KRS", cbKRSList);
    g_perintah["krs_new"] = new Operasi("buat KRS baru", cbKRSNew);
    g_perintah["krs_tambahmk"] = new Operasi("tambah matakuliah ke KRS", cbKRSTambahmk);
    g_perintah["krs_hapusmk"] = new Operasi("hampus matakuliah dari KRS", cbKRSHapusmk);
    g_perintah["krs_cetak"] = new Operasi("cetak KRS", cbKRSCetak);
    g_perintah["krs_delete"] = new Operasi("hapus KRS yang ada", cbKRSDelete);
    g_perintah["logout"] = new Operasi("keluar dari program", cbLogout);

    std::cout << "Ketik `help` untuk melihat daftar perintah" << std::endl;

    while (g_running) {
        std::string cmd;

        if (g_user != NULL) {
            std::cout << "[" << g_user->getNama() << "] ";
        }
        std::cout << "$ ";
        std::cin >> cmd;

        if (g_perintah.count(cmd) == 1) {
            g_perintah[cmd]->callback();
        } else {
            std::cout << "Perintah tidak terdaftar. Ketik `help` untuk melihat daftar perintah" << std::endl;
        }
    }

    std::cout << "Goodbye :)" << std::endl;

    for (
        std::map<std::string, Operasi *>::iterator it = g_perintah.begin();
        it != g_perintah.end();
        ++it
    ) {
        delete it->second;
    }
    g_perintah.clear();

    std::cout << "Cleaning up ...";
    destroyDB();
    std::cout << " done" << std::endl;
}


//==============================================================================
// Implementasi class
//==============================================================================

// Mahasiswa

Mahasiswa::Mahasiswa(void) {
}

Mahasiswa::Mahasiswa(const std::string &nama) {
    m_nama = nama;
}

Mahasiswa::~Mahasiswa(void) {
    // hapus semua KRS karena KRS merupakan weak entity dari mahasiswa
    // sehingga tanpa mahasiswa, tidak ada KRS tiap mahasiswa
    for (
        std::map<int, KRS *>::iterator it = m_dataKRS.begin();
        it != m_dataKRS.end();
        ++it
    ) {
        delete it->second; // hapus instance KRS
    }
    m_dataKRS.clear();
}

const std::string &Mahasiswa::getNama(void) const {
    return m_nama;
}

void Mahasiswa::addKRS(int semester, KRS *krs) {
    if (m_dataKRS.count(semester) == 0) {
        m_dataKRS[semester] = krs; // transfer kepemilikan pointer
    } else {
        throw "KRS untuk semester tersebut masih ada";
    }
}

KRS *Mahasiswa::getKRS(int semester) {
    if (m_dataKRS.count(semester) == 1) {
        return m_dataKRS[semester];
    }
    return NULL;
}

std::map<int, KRS *>::iterator Mahasiswa::beginKRS(void) {
    return m_dataKRS.begin();
}

std::map<int, KRS *>::iterator Mahasiswa::endKRS(void) {
    return m_dataKRS.end();
}

std::map<int, KRS *>::const_iterator Mahasiswa::beginKRS(void) const {
    return m_dataKRS.begin();
}

std::map<int, KRS *>::const_iterator Mahasiswa::endKRS(void) const {
    return m_dataKRS.end();
}

void Mahasiswa::deleteKRS(int semester) {
    if (m_dataKRS.count(semester) == 1) {
        KRS *krs = m_dataKRS[semester];
        delete krs;
        m_dataKRS.erase(semester);
    }
}


// KRS

KRS::KRS(void) {
}

KRS::KRS(const Mahasiswa *pemilik) {
    m_pemilik = pemilik;
}

const Mahasiswa *KRS::getPemilik(void) const {
    return m_pemilik;
}

void KRS::addMakul(const Matakuliah *makul) {
    m_daftarMakul.push_back(makul);
}

std::list<const Matakuliah *>::iterator KRS::beginMakul(void) {
    return m_daftarMakul.begin();
}

std::list<const Matakuliah *>::iterator KRS::endMakul(void) {
    return m_daftarMakul.end();
}

std::list<const Matakuliah *>::const_iterator KRS::beginMakul(void) const {
    return m_daftarMakul.begin();
}

std::list<const Matakuliah *>::const_iterator KRS::endMakul(void) const {
    return m_daftarMakul.end();
}

void KRS::deleteMakul(const std::string &kodeMK) {
    if (g_matakuliahDB.count(kodeMK) == 1) {
        std::list<const Matakuliah *>::iterator it = m_daftarMakul.begin();
        while (it != m_daftarMakul.end()) {
            if ((*it) == g_matakuliahDB[kodeMK]) {
                break;
            }
            ++it;
        }
        if (it != m_daftarMakul.end()) {
            m_daftarMakul.erase(it);
        }
    }
}

int KRS::getTotalMakul(void) const {
    return m_daftarMakul.size();
}

int KRS::getTotalSKS(void) const {
    int total = 0;
    for (
        std::list<const Matakuliah *>::const_iterator it = m_daftarMakul.begin();
        it != m_daftarMakul.end();
        ++it
    ) {
        total = total + (*it)->getBobot();
    }
    return total;
}

// Matakuliah

Matakuliah::Matakuliah(void) {
}

Matakuliah::Matakuliah(const std::string &nama, int bobot, int kapasitas) {
    m_nama = nama;
    m_bobot = bobot;
    m_kapasitas = kapasitas;
    m_peserta = 0;
}

Matakuliah::~Matakuliah(void) {
    while (!m_daftarJadwal.empty()) {
        Jadwal *jadwal = m_daftarJadwal.front();
        delete jadwal;
        m_daftarJadwal.pop_front();
    }
}

const std::string &Matakuliah::getNama(void) const {
    return m_nama;
}

int Matakuliah::getBobot(void) const {
    return m_bobot;
}

int Matakuliah::getKapasitas(void) const {
    return m_kapasitas;
}

int Matakuliah::getPeserta(void) const {
    return m_peserta;
}

void Matakuliah::tambahMahasiswa(void) {
    if (m_peserta < m_kapasitas) {
        m_peserta++;
    }
}

void Matakuliah::kurangiMahasiswa(void) {
    if (m_peserta > 0) {
        m_peserta--;
    }
}



void Matakuliah::addPrasyarat(const Matakuliah *prasyarat) {
    m_prasyarat.push_back(prasyarat);
}

std::list<const Matakuliah *>::const_iterator Matakuliah::beginPrasyarat(void) const {
    return m_prasyarat.begin();
}

std::list<const Matakuliah *>::const_iterator Matakuliah::endPrasyarat(void) const {
    return m_prasyarat.end();
}

void Matakuliah::addJadwal(Jadwal *jadwal) {
    m_daftarJadwal.push_back(jadwal);
}

void Matakuliah::deleteJadwal(std::list<Jadwal *>::iterator jadwalIt) {
    m_daftarJadwal.erase(jadwalIt);
}

std::list<Jadwal *>::iterator Matakuliah::beginJadwal(void) {
    return m_daftarJadwal.begin();
}

std::list<Jadwal *>::iterator Matakuliah::endJadwal(void) {
    return m_daftarJadwal.end();
}

std::list<Jadwal *>::const_iterator Matakuliah::beginJadwal(void) const {
    return m_daftarJadwal.begin();
}

std::list<Jadwal *>::const_iterator Matakuliah::endJadwal(void) const {
    return m_daftarJadwal.end();
}

void Matakuliah::setPengampu(const Dosen *pengampu) {
    m_pengampu = pengampu;
}

const Dosen *Matakuliah::getPengampu(void) const {
    return m_pengampu;
}

bool Matakuliah::cekPrasyarat(const Mahasiswa *mhs, int semester) const {
    bool terpenuhi = true;
    for (
        std::list<const Matakuliah *>::const_iterator itMK = m_prasyarat.begin();
        itMK != m_prasyarat.end();
        ++itMK
    ) {
        bool sudahAmbil = false;
        for (
            std::map<int, KRS *>::const_iterator itKRS = mhs->beginKRS();
            (itKRS != mhs->endKRS()) && !sudahAmbil;
            ++itKRS
        ) {
            if (itKRS->first < semester) {
                for (
                    std::list<const Matakuliah *>::const_iterator itAmbil = itKRS->second->beginMakul();
                    (itAmbil != itKRS->second->endMakul()) && !sudahAmbil;
                    ++itAmbil
                ) {
                    if (*itMK == *itAmbil) {
                        sudahAmbil = true;
                        break;
                    }
                }
            }
        }
        terpenuhi = terpenuhi && sudahAmbil;
    }
    return terpenuhi;
}

bool Matakuliah::cekTumbukan(const Matakuliah *lain) const {
    bool tumbukan = false;
    for (
        std::list<Jadwal *>::const_iterator itA = m_daftarJadwal.begin();
        (itA != m_daftarJadwal.end()) && !tumbukan;
        ++itA
    ) {
        for (
            std::list<Jadwal *>::const_iterator itB = lain->beginJadwal();
            (itB != lain->endJadwal()) && !tumbukan;
            ++itB
        ) {
            if ((*itA)->cekTumbukan(*itB)) {
                tumbukan = true;
                break;
            }
        }
    }
    return tumbukan;
}

// MKWajib

MKWajib::MKWajib(void) {
}

MKWajib::MKWajib(const std::string &nama, int bobot, int kapasitas, JenisMKWajib jenis)
    : Matakuliah(nama, bobot, kapasitas)
{
    m_jenis = jenis;
}

JenisMKWajib MKWajib::getJenis(void) const {
    return m_jenis;
}

bool MKWajib::isPilihan(void) const {
    return false;
}

// MKPilihan

MKPilihan::MKPilihan(void) {
}

MKPilihan::MKPilihan(const std::string &nama, int bobot, int kapasitas, const std::string &labRiset)
    : Matakuliah(nama, bobot, kapasitas)
{
    m_labRiset = labRiset;
}

const std::string &MKPilihan::getLabRiset(void) const {
    return m_labRiset;
}

bool MKPilihan::isPilihan(void) const {
    return true;
}

// Jadwal

Jadwal::Jadwal(void) {
}

Jadwal::Jadwal(HariID hari, int sesi) {
    m_hari = hari;
    m_sesi = sesi;
}

HariID Jadwal::getHari(void) const {
    return m_hari;
}

int Jadwal::getSesi(void) const {
    return m_sesi;
}

bool Jadwal::cekTumbukan(const Jadwal *lain) const {
    return (m_hari == lain->getHari()) && (m_sesi == lain->getSesi());
}

// Dosen

Dosen::Dosen(void) {
}

Dosen::Dosen(const std::string &nama) {
    m_nama = nama;
}

const std::string &Dosen::getNama(void) const {
    return m_nama;
}

