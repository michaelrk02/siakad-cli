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

#include <fstream>
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

// ISerializable interface
class ISerializable {
public:
    virtual std::string getClassID(void) const = 0;
    virtual void serialize(std::ostream &output) const = 0;
    virtual void deserialize(std::istream &input) = 0;
};


//==============================================================================
// Deklarasi dan Definisi Class
//==============================================================================

#define ClassID_Mahasiswa   "SiakadCLI.Mahasiswa"
#define ClassID_KRS         "SiakadCLI.KRS"
#define ClassID_Matakuliah  "SiakadCLI.Matakuliah"
#define ClassID_MKWajib     "SiakadCLI.MKWajib"
#define ClassID_MKPilihan   "SiakadCLI.MKPilihan"
#define ClassID_Jadwal      "SiakadCLI.Jadwal"
#define ClassID_Dosen       "SiakadCLI.Dosen"

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
class Mahasiswa : public ISerializable {
private:
    std::string m_id; // ID mahasiswa
    std::string m_nama; // nama mahasiswa

    std::map<int, KRS *> m_dataKRS; // mapping Semester -> KRS

public:
    Mahasiswa(void);
    Mahasiswa(const std::string &id, const std::string &nama);
    ~Mahasiswa(void);

    // getters
    const std::string &getID(void) const;
    const std::string &getNama(void) const;

    void addKRS(int semester, KRS *krs); // tambah KRS pada semester tertentu
    KRS *getKRS(int semester); // akses KRS pada semester tertentu
    std::map<int, KRS *>::iterator beginKRS(void); // iterate KRS (begin)
    std::map<int, KRS *>::iterator endKRS(void); // iterate KRS (end)
    std::map<int, KRS *>::const_iterator beginKRS(void) const; // iterate KRS (begin const)
    std::map<int, KRS *>::const_iterator endKRS(void) const; // iterate KRS (end const)
    void deleteKRS(int semester); // hapus KRS pada semester tertentu

    // ISerializable
    virtual std::string getClassID(void) const;
    virtual void serialize(std::ostream &output) const;
    virtual void deserialize(std::istream &input);
};

// Entity: KRS (weak oleh mahasiswa)
// Relasi:
//  N-1 dengan Mahasiswa
//  N-N dengan Matakuliah
class KRS : public ISerializable {
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

    // ISerializable
    virtual std::string getClassID(void) const;
    virtual void serialize(std::ostream &output) const;
    virtual void deserialize(std::istream &input);
};

// Entity: Matakuliah (strong)
// Relasi:
//  N-N dengan KRS
//  N-N dengan Matakuliah
//  N-N dengan Jadwal
//  N-1 dengan Dosen
class Matakuliah : public ISerializable {
protected:
    std::string m_id; // ID matakuliah
    std::string m_nama; // nama matakuliah
    int m_bobot; // bobot dalam SKS
    int m_kapasitas; // kapasitas matakuliah
    int m_peserta; // peserta matakuliah

    std::list<const Matakuliah *> m_prasyarat; // prasyarat matakuliah
    std::list<Jadwal *> m_daftarJadwal; // daftar sesi matakuliah
    const Dosen *m_pengampu; // dosen pengampu matakuliah

public:
    Matakuliah(void);
    Matakuliah(const std::string &id, const std::string &nama, int bobot, int kapasitas);
    ~Matakuliah(void);

    // getters
    const std::string &getID(void) const;
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

    // ISerializable
    virtual std::string getClassID(void) const;
};

// Entity: MKWajib (inherit Matakuliah)
class MKWajib : public Matakuliah {
private:
    JenisMKWajib m_jenis; // jenis matakuliah (wajib universitas/fakultas/prodi/peminatan)

public:
    MKWajib(void);
    MKWajib(const std::string &id, const std::string &nama, int bobot, int kapasitas, JenisMKWajib jenis);

    // getters
    JenisMKWajib getJenis(void) const;

    bool isPilihan(void) const;

    // ISerializable
    virtual std::string getClassID(void) const;
    virtual void serialize(std::ostream &output) const;
    virtual void deserialize(std::istream &input);
};

// Entity: MKPilihan (inherit Matakuliah)
class MKPilihan : public Matakuliah {
private:
    std::string m_labRiset; // lab riset penyedia matakuliah pilihan

public:
    MKPilihan(void);
    MKPilihan(const std::string &id, const std::string &nama, int bobot, int kapasitas, const std::string &labRiset);

    // getters
    const std::string &getLabRiset(void) const;

    bool isPilihan(void) const;

    // ISerializable
    virtual std::string getClassID(void) const;
    virtual void serialize(std::ostream &output) const;
    virtual void deserialize(std::istream &input);
};

// Entity: Jadwal (weak oleh Matakuliah)
// Relasi:
//  N-N dengan Matakuliah
class Jadwal : public ISerializable {
private:
    const Matakuliah *m_makul;
    HariID m_hari;
    int m_sesi;

public:
    Jadwal(void);
    Jadwal(const Matakuliah *makul, HariID hari, int sesi);

    // getters
    const Matakuliah *getMakul(void) const;
    HariID getHari(void) const;
    int getSesi(void) const;

    bool cekTumbukan(const Jadwal *lain) const; // cek bentrokan/tumbukan dengan jadwal lain

    // ISerializable
    virtual std::string getClassID(void) const;
    virtual void serialize(std::ostream &output) const;
    virtual void deserialize(std::istream &input);
};

// Entity: Dosen (strong)
// Relasi:
//  1-N dengan Matakuliah
class Dosen : public ISerializable {
private:
    std::string m_id; // ID dosen
    std::string m_nama; // nama dosen

public:
    Dosen(void);
    Dosen(const std::string &id, const std::string &nama);

    // getters
    const std::string &getID(void) const;
    const std::string &getNama(void) const;

    // ISerializable
    virtual std::string getClassID(void) const;
    virtual void serialize(std::ostream &output) const;
    virtual void deserialize(std::istream &input);
};


//==============================================================================
// Helpers
//==============================================================================

inline void strSerialize(std::ostream &output, const std::string &str) {
    unsigned int size = str.size();
    output.write((const char *)&size, sizeof(unsigned int));
    if (size != 0) {
        output.write((const char *)str.data(), size * sizeof(char));
    }
}

inline void strDeserialize(std::istream &input, std::string &str) {
    unsigned int size;
    input.read((char *)&size, sizeof(unsigned int));

    char *buffer = new char[size + 1];
    if (size != 0) {
        input.read((char *)buffer, size * sizeof(char));
    }
    buffer[size] = '\0';

    str.assign(buffer);

    delete buffer;
}


//==============================================================================
// Database
//==============================================================================

std::map<std::string, Mahasiswa *> g_mahasiswaDB;
std::map<std::string, Matakuliah *> g_matakuliahDB;
std::map<std::string, Dosen *> g_dosenDB;

void generateMahasiswaDB(void) {
    g_mahasiswaDB["M0520001"] = new Mahasiswa("M0520001", "Adrian Smith");
    g_mahasiswaDB["M0520002"] = new Mahasiswa("M0520002", "Bruce Dickinson");
    g_mahasiswaDB["M0520003"] = new Mahasiswa("M0520003", "Dave Murray");
    g_mahasiswaDB["M0520004"] = new Mahasiswa("M0520004", "Janick Gers");
    g_mahasiswaDB["M0520005"] = new Mahasiswa("M0520005", "Nicko McBrain");
    g_mahasiswaDB["M0520006"] = new Mahasiswa("M0520006", "Steve Harris");
}

void generateMatakuliahDB(void) {
    g_matakuliahDB["IF3110"] = new MKWajib("IF3110", "Pengembangan Berbasis Web", 2, 5, MKWajibProdi);
    g_matakuliahDB["IF3111"] = new MKWajib("IF3111", "Pengembangan Berbasis Platform", 2, 5, MKWajibProdi);
    g_matakuliahDB["IF3130"] = new MKWajib("IF3130", "Jaringan Komputer", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3140"] = new MKWajib("IF3140", "Manajemen Basis Data", 2, 5, MKWajibProdi);
    g_matakuliahDB["IF3151"] = new MKWajib("IF3151", "Interaksi Manusia dan Komputer", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3170"] = new MKWajib("IF3170", "Kecerdasan Buatan", 4, 5, MKWajibProdi);
    g_matakuliahDB["IF3150"] = new MKWajib("IF3150", "Manajemen Proyek", 2, 5, MKWajibProdi);

    g_matakuliahDB["IF3230"] = new MKWajib("IF3230", "Sistem Paralel dan Terdistribusi", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3240"] = new MKWajib("IF3240", "Sistem Informasi", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3250"] = new MKWajib("IF3250", "Proyek Perangkat Lunak", 4, 5, MKWajibProdi);
    g_matakuliahDB["IF3260"] = new MKWajib("IF3260", "Grafika Komputer", 3, 5, MKWajibProdi);
    g_matakuliahDB["IF3280"] = new MKWajib("IF3280", "Sosio-informatika dan Profesionalisme", 3, 5, MKWajibProdi);
    g_matakuliahDB["KU206X"] = new MKWajib("KU206X", "Agama dan Budipekerti", 2, 5, MKWajibUniversitas);

    g_matakuliahDB["IF3230"]->addPrasyarat(g_matakuliahDB["IF3130"]);
    g_matakuliahDB["IF3240"]->addPrasyarat(g_matakuliahDB["IF3110"]);
    g_matakuliahDB["IF3240"]->addPrasyarat(g_matakuliahDB["IF3140"]);
    g_matakuliahDB["IF3250"]->addPrasyarat(g_matakuliahDB["IF3150"]);
}

void generateDosenDB(void) {
    g_dosenDB["WIN001"] = new Dosen("WIN001", "Winarno");
    g_dosenDB["ARD001"] = new Dosen("ARD001", "Ardhi Wijayanto");
    g_dosenDB["ABD001"] = new Dosen("ABD001", "Abdul Aziz");
    g_dosenDB["DEW001"] = new Dosen("DEW001", "Dewi Wisnu Wardani");
    g_dosenDB["SAR001"] = new Dosen("SAR001", "Sari Widya Sihwi");
    g_dosenDB["WIH001"] = new Dosen("WIH001", "Wiharto");
    g_dosenDB["WIS001"] = new Dosen("WIS001", "Wisnu Widiarto");
    g_dosenDB["HAR001"] = new Dosen("HAR001", "Haryono Setiadi");
    g_dosenDB["HER001"] = new Dosen("HER001", "Heri Prasetyo");
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
    g_matakuliahDB["IF3110"]->addJadwal(new Jadwal(g_matakuliahDB["IF3110"], HariSenin, 1));
    g_matakuliahDB["IF3110"]->addJadwal(new Jadwal(g_matakuliahDB["IF3110"], HariSenin, 2));
    g_matakuliahDB["IF3111"]->addJadwal(new Jadwal(g_matakuliahDB["IF3111"], HariSelasa, 3));
    g_matakuliahDB["IF3111"]->addJadwal(new Jadwal(g_matakuliahDB["IF3111"], HariSelasa, 4));
    g_matakuliahDB["IF3130"]->addJadwal(new Jadwal(g_matakuliahDB["IF3130"], HariSenin, 4));
    g_matakuliahDB["IF3130"]->addJadwal(new Jadwal(g_matakuliahDB["IF3130"], HariSenin, 5));
    g_matakuliahDB["IF3130"]->addJadwal(new Jadwal(g_matakuliahDB["IF3130"], HariSenin, 6));
    g_matakuliahDB["IF3140"]->addJadwal(new Jadwal(g_matakuliahDB["IF3140"], HariRabu, 2));
    g_matakuliahDB["IF3140"]->addJadwal(new Jadwal(g_matakuliahDB["IF3140"], HariRabu, 3));
    g_matakuliahDB["IF3151"]->addJadwal(new Jadwal(g_matakuliahDB["IF3151"], HariRabu, 4));
    g_matakuliahDB["IF3151"]->addJadwal(new Jadwal(g_matakuliahDB["IF3151"], HariRabu, 5));
    g_matakuliahDB["IF3151"]->addJadwal(new Jadwal(g_matakuliahDB["IF3151"], HariKamis, 1));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(g_matakuliahDB["IF3170"], HariKamis, 4));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(g_matakuliahDB["IF3170"], HariKamis, 5));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(g_matakuliahDB["IF3170"], HariKamis, 6));
    g_matakuliahDB["IF3170"]->addJadwal(new Jadwal(g_matakuliahDB["IF3170"], HariJumat, 2));
    g_matakuliahDB["IF3150"]->addJadwal(new Jadwal(g_matakuliahDB["IF3150"], HariJumat, 3));
    g_matakuliahDB["IF3150"]->addJadwal(new Jadwal(g_matakuliahDB["IF3150"], HariJumat, 4));

    g_matakuliahDB["IF3230"]->addJadwal(new Jadwal(g_matakuliahDB["IF3230"], HariSenin, 1));
    g_matakuliahDB["IF3230"]->addJadwal(new Jadwal(g_matakuliahDB["IF3230"], HariSenin, 2));
    g_matakuliahDB["IF3230"]->addJadwal(new Jadwal(g_matakuliahDB["IF3230"], HariSenin, 3));
    g_matakuliahDB["IF3240"]->addJadwal(new Jadwal(g_matakuliahDB["IF3240"], HariSenin, 4));
    g_matakuliahDB["IF3240"]->addJadwal(new Jadwal(g_matakuliahDB["IF3240"], HariSenin, 5));
    g_matakuliahDB["IF3240"]->addJadwal(new Jadwal(g_matakuliahDB["IF3240"], HariSenin, 6));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(g_matakuliahDB["IF3250"], HariRabu, 3));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(g_matakuliahDB["IF3250"], HariRabu, 4));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(g_matakuliahDB["IF3250"], HariRabu, 5));
    g_matakuliahDB["IF3250"]->addJadwal(new Jadwal(g_matakuliahDB["IF3250"], HariKamis, 2));
    g_matakuliahDB["IF3260"]->addJadwal(new Jadwal(g_matakuliahDB["IF3260"], HariKamis, 3));
    g_matakuliahDB["IF3260"]->addJadwal(new Jadwal(g_matakuliahDB["IF3260"], HariKamis, 4));
    g_matakuliahDB["IF3260"]->addJadwal(new Jadwal(g_matakuliahDB["IF3260"], HariKamis, 5));
    g_matakuliahDB["IF3280"]->addJadwal(new Jadwal(g_matakuliahDB["IF3280"], HariJumat, 1));
    g_matakuliahDB["IF3280"]->addJadwal(new Jadwal(g_matakuliahDB["IF3280"], HariJumat, 2));
    g_matakuliahDB["IF3280"]->addJadwal(new Jadwal(g_matakuliahDB["IF3280"], HariJumat, 3));
    g_matakuliahDB["KU206X"]->addJadwal(new Jadwal(g_matakuliahDB["KU206X"], HariJumat, 5));
    g_matakuliahDB["KU206X"]->addJadwal(new Jadwal(g_matakuliahDB["KU206X"], HariJumat, 6));
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

        std::cout << " Jenis MK : ";
        if (makul->getClassID() == ClassID_MKWajib) {
            std::cout << "WAJIB";
            switch (((MKWajib *)makul)->getJenis()) {
            case MKWajibUniversitas:
                std::cout << " UNIVERSITAS";
                break;
            case MKWajibProdi:
                std::cout << " PRODI";
                break;
            }
            std::cout << std::endl;
        } else if (makul->getClassID() == ClassID_MKPilihan) {
            std::cout << " PILIHAN (Lab Riset: " << ((MKPilihan *)makul)->getLabRiset() << ")";
        }

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

void cbDBReset(void) {
    g_user = NULL;
    destroyDB();
    std::cout << "SUKSES! Database ter-reset. Anda akan ter-logout secara otomatis" << std::endl;
}

void cbDBGenerate(void) {
    g_user = NULL;
    destroyDB();
    generateDB();
    std::cout << "SUKSES! Berhasil generate database" << std::endl;
}

void cbDBSave(void) {
    std::string namaDB;
    std::cout << " Nama DB (tanpa spasi) : ";
    std::cin >> namaDB;

    std::ofstream file(namaDB.c_str());
    if (file.is_open()) {
        unsigned int nMahasiswa = g_mahasiswaDB.size();
        file.write((const char *)&nMahasiswa, sizeof(unsigned int));
        for (
            std::map<std::string, Mahasiswa *>::iterator it = g_mahasiswaDB.begin();
            it != g_mahasiswaDB.end();
            ++it
        ) {
            strSerialize(file, it->first);
            it->second->serialize(file);
        }

        unsigned int nMatakuliah = g_matakuliahDB.size();
        file.write((const char *)&nMatakuliah, sizeof(unsigned int));
        for (
            std::map<std::string, Matakuliah *>::iterator it = g_matakuliahDB.begin();
            it != g_matakuliahDB.end();
            ++it
        ) {
            strSerialize(file, it->first);
            strSerialize(file, it->second->getClassID());
            it->second->serialize(file);
        }

        unsigned int nDosen = g_dosenDB.size();
        file.write((const char *)&nDosen, sizeof(unsigned int));
        for (
            std::map<std::string, Dosen *>::iterator it = g_dosenDB.begin();
            it != g_dosenDB.end();
            ++it
        ) {
            strSerialize(file, it->first);
            it->second->serialize(file);
        }

        file.close();

        std::cout << "SUKSES! Berhasil save ke DB" << std::endl;
    } else {
        std::cout << "Gagal save ke DB :(" << std::endl;
    }
}

void cbDBLoad(void) {
    std::string namaDB;
    std::cout << " Nama DB (tanpa spasi) : ";
    std::cin >> namaDB;

    std::ifstream file(namaDB.c_str());
    if (file.is_open()) {
        destroyDB();

        unsigned int nMahasiswa, nMatakuliah, nDosen;

        file.read((char *)&nMahasiswa, sizeof(unsigned int));
        for (unsigned int i = 0; i < nMahasiswa; i++) {
            std::string id;
            strDeserialize(file, id);
            g_mahasiswaDB[id] = new Mahasiswa;

            Mahasiswa *tmp = new Mahasiswa;
            tmp->deserialize(file);
            delete tmp;
        }

        file.read((char *)&nMatakuliah, sizeof(unsigned int));
        for (unsigned int i = 0; i < nMatakuliah; i++) {
            std::string id, classID;
            strDeserialize(file, id);
            strDeserialize(file, classID);

            if (classID == ClassID_MKWajib) {
                g_matakuliahDB[id] = new MKWajib;

                MKWajib *tmp = new MKWajib;
                tmp->deserialize(file);
                delete tmp;
            } else if (classID == ClassID_MKPilihan) {
                g_matakuliahDB[id] = new MKPilihan;

                MKPilihan *tmp = new MKPilihan;
                tmp->deserialize(file);
                delete tmp;
            }
        }

        file.read((char *)&nDosen, sizeof(unsigned int));
        for (unsigned int i = 0; i < nDosen; i++) {
            std::string id;
            strDeserialize(file, id);
            g_dosenDB[id] = new Dosen;

            Dosen *tmp = new Dosen;
            tmp->deserialize(file);
            delete tmp;
        }

        file.clear();
        file.seekg(0);

        file.read((char *)&nMahasiswa, sizeof(unsigned int));
        for (unsigned int i = 0; i < nMahasiswa; i++) {
            std::string id;
            strDeserialize(file, id);
            g_mahasiswaDB[id]->deserialize(file);
        }

        file.read((char *)&nMatakuliah, sizeof(unsigned int));
        for (unsigned int i = 0; i < nMatakuliah; i++) {
            std::string id, classID;
            strDeserialize(file, id);
            strDeserialize(file, classID);
            g_matakuliahDB[id]->deserialize(file);
        }

        file.read((char *)&nDosen, sizeof(unsigned int));
        for (unsigned int i = 0; i < nDosen; i++) {
            std::string id;
            strDeserialize(file, id);
            g_dosenDB[id]->deserialize(file);
        }

        file.close();

        std::cout << "SUKSES! Berhasil load DB" << std::endl;
    } else {
        std::cout << "Tidak dapat menemukan DB :(" << std::endl;
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
    g_perintah["db_reset"] = new Operasi("reset database", cbDBReset);
    g_perintah["db_generate"] = new Operasi("generate database bawaan", cbDBGenerate);
    g_perintah["db_save"] = new Operasi("simpan ke database", cbDBSave);
    g_perintah["db_load"] = new Operasi("muat dari database", cbDBLoad);
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

Mahasiswa::Mahasiswa(const std::string &id, const std::string &nama) {
    m_id = id;
    m_nama = nama;
}

Mahasiswa::~Mahasiswa(void) {
    for (
        std::map<int, KRS *>::iterator it = m_dataKRS.begin();
        it != m_dataKRS.end();
        ++it
    ) {
        delete it->second;
    }
    m_dataKRS.clear();
}

const std::string &Mahasiswa::getID(void) const {
    return m_id;
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

std::string Mahasiswa::getClassID(void) const {
    return ClassID_Mahasiswa;
}

void Mahasiswa::serialize(std::ostream &output) const {
    std::string classID = getClassID();
    strSerialize(output, classID);

    strSerialize(output, m_id);
    strSerialize(output, m_nama);

    unsigned int nKRS = m_dataKRS.size();
    output.write((const char *)&nKRS, sizeof(unsigned int));
    for (
        std::map<int, KRS *>::const_iterator it = m_dataKRS.begin();
        it != m_dataKRS.end();
        ++it
    ) {
        int semester = it->first;
        output.write((const char *)&semester, sizeof(int));

        it->second->serialize(output);
    }
}

void Mahasiswa::deserialize(std::istream &input) {
    std::string classID;
    strDeserialize(input, classID);

    strDeserialize(input, m_id);
    strDeserialize(input, m_nama);

    for (
        std::map<int, KRS *>::iterator it = m_dataKRS.begin();
        it != m_dataKRS.end();
        ++it
    ) {
        delete it->second;
    }
    m_dataKRS.clear();

    unsigned int nKRS;
    input.read((char *)&nKRS, sizeof(unsigned int));
    for (unsigned int i = 0; i < nKRS; i++) {
        int semester;
        input.read((char *)&semester, sizeof(int));

        KRS *krs = new KRS(this);
        krs->deserialize(input);
        m_dataKRS[semester] = krs;
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

std::string KRS::getClassID(void) const {
    return ClassID_KRS;
}

void KRS::serialize(std::ostream &output) const {
    std::string classID = getClassID();
    strSerialize(output, classID);

    unsigned int nMakul = m_daftarMakul.size();
    output.write((const char *)&nMakul, sizeof(unsigned int));
    for (
        std::list<const Matakuliah *>::const_iterator it = m_daftarMakul.begin();
        it != m_daftarMakul.end();
        ++it
    ) {
        strSerialize(output, (*it)->getID());
    }
}

void KRS::deserialize(std::istream &input) {
    std::string classID;
    strDeserialize(input, classID);

    unsigned int nMakul;
    input.read((char *)&nMakul, sizeof(unsigned int));
    for (unsigned int i = 0; i < nMakul; i++) {
        std::string idMakul;
        strDeserialize(input, idMakul);
        if (g_matakuliahDB.count(idMakul) == 1) {
            m_daftarMakul.push_back(g_matakuliahDB[idMakul]);
        }
    }
}


// Matakuliah

Matakuliah::Matakuliah(void) {
}

Matakuliah::Matakuliah(const std::string &id, const std::string &nama, int bobot, int kapasitas) {
    m_id = id;
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

const std::string &Matakuliah::getID(void) const {
    return m_id;
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

std::string Matakuliah::getClassID(void) const {
    return ClassID_Matakuliah;
}


// MKWajib

MKWajib::MKWajib(void) {
}

MKWajib::MKWajib(const std::string &id, const std::string &nama, int bobot, int kapasitas, JenisMKWajib jenis)
    : Matakuliah(id, nama, bobot, kapasitas)
{
    m_jenis = jenis;
}

JenisMKWajib MKWajib::getJenis(void) const {
    return m_jenis;
}

bool MKWajib::isPilihan(void) const {
    return false;
}

std::string MKWajib::getClassID(void) const {
    return ClassID_MKWajib;
}

void MKWajib::serialize(std::ostream &output) const {
    strSerialize(output, m_id);
    strSerialize(output, m_nama);
    output.write((const char *)&m_bobot, sizeof(int));
    output.write((const char *)&m_kapasitas, sizeof(int));
    output.write((const char *)&m_peserta, sizeof(int));

    unsigned int nPrasyarat = m_prasyarat.size();
    output.write((const char *)&nPrasyarat, sizeof(unsigned int));
    for (
        std::list<const Matakuliah *>::const_iterator it = m_prasyarat.begin();
        it != m_prasyarat.end();
        ++it
    ) {
        strSerialize(output, (*it)->getID());
    }

    unsigned int nJadwal = m_daftarJadwal.size();
    output.write((const char *)&nJadwal, sizeof(unsigned int));
    for (
        std::list<Jadwal *>::const_iterator it = m_daftarJadwal.begin();
        it != m_daftarJadwal.end();
        ++it
    ) {
        (*it)->serialize(output);
    }

    std::string idDosen = "";
    if (m_pengampu != NULL) {
        idDosen = m_pengampu->getID();
    }
    strSerialize(output, idDosen);

    output.write((const char *)&m_jenis, sizeof(JenisMKWajib));
}

void MKWajib::deserialize(std::istream &input) {
    strDeserialize(input, m_id);
    strDeserialize(input, m_nama);
    input.read((char *)&m_bobot, sizeof(int));
    input.read((char *)&m_kapasitas, sizeof(int));
    input.read((char *)&m_peserta, sizeof(int));

    m_prasyarat.clear();

    unsigned int nPrasyarat;
    input.read((char *)&nPrasyarat, sizeof(unsigned int));
    for (unsigned int i = 0; i < nPrasyarat; i++) {
        std::string idMakul;
        strDeserialize(input, idMakul);
        if (g_matakuliahDB.count(idMakul) == 1) {
            m_prasyarat.push_back(g_matakuliahDB[idMakul]);
        }
    }

    for (
        std::list<Jadwal *>::iterator it = m_daftarJadwal.begin();
        it != m_daftarJadwal.end();
        ++it
    ) {
        delete (*it);
    }
    m_daftarJadwal.clear();

    unsigned int nJadwal;
    input.read((char *)&nJadwal, sizeof(unsigned int));
    for (unsigned int i = 0; i < nJadwal; i++) {
        Jadwal *jdw = new Jadwal(this, HariSenin, 0);
        jdw->deserialize(input);
        m_daftarJadwal.push_back(jdw);
    }

    std::string idDosen;
    strDeserialize(input, idDosen);
    if (!idDosen.empty()) {
        if (g_dosenDB.count(idDosen) == 1) {
            m_pengampu = g_dosenDB[idDosen];
        }
    } else {
        m_pengampu = NULL;
    }

    input.read((char *)&m_jenis, sizeof(JenisMKWajib));
}

// MKPilihan

MKPilihan::MKPilihan(void) {
}

MKPilihan::MKPilihan(const std::string &id, const std::string &nama, int bobot, int kapasitas, const std::string &labRiset)
    : Matakuliah(id, nama, bobot, kapasitas)
{
    m_labRiset = labRiset;
}

const std::string &MKPilihan::getLabRiset(void) const {
    return m_labRiset;
}

bool MKPilihan::isPilihan(void) const {
    return true;
}

std::string MKPilihan::getClassID(void) const {
    return ClassID_MKPilihan;
}

void MKPilihan::serialize(std::ostream &output) const {
    strSerialize(output, m_id);
    strSerialize(output, m_nama);
    output.write((const char *)&m_bobot, sizeof(int));
    output.write((const char *)&m_kapasitas, sizeof(int));
    output.write((const char *)&m_peserta, sizeof(int));

    unsigned int nPrasyarat = m_prasyarat.size();
    output.write((const char *)&nPrasyarat, sizeof(unsigned int));
    for (
        std::list<const Matakuliah *>::const_iterator it = m_prasyarat.begin();
        it != m_prasyarat.end();
        ++it
    ) {
        strSerialize(output, (*it)->getID());
    }

    unsigned int nJadwal = m_daftarJadwal.size();
    output.write((const char *)&nJadwal, sizeof(unsigned int));
    for (
        std::list<Jadwal *>::const_iterator it = m_daftarJadwal.begin();
        it != m_daftarJadwal.end();
        ++it
    ) {
        (*it)->serialize(output);
    }

    std::string idDosen = "";
    if (m_pengampu != NULL) {
        idDosen = m_pengampu->getID();
    }
    strSerialize(output, idDosen);

    strSerialize(output, m_labRiset);
}

void MKPilihan::deserialize(std::istream &input) {
    strDeserialize(input, m_id);
    strDeserialize(input, m_nama);
    input.read((char *)&m_bobot, sizeof(int));
    input.read((char *)&m_kapasitas, sizeof(int));
    input.read((char *)&m_peserta, sizeof(int));

    m_prasyarat.clear();

    unsigned int nPrasyarat;
    input.read((char *)&nPrasyarat, sizeof(unsigned int));
    for (unsigned int i = 0; i < nPrasyarat; i++) {
        std::string idMakul;
        strDeserialize(input, idMakul);
        if (g_matakuliahDB.count(idMakul) == 1) {
            m_prasyarat.push_back(g_matakuliahDB[idMakul]);
        }
    }

    for (
        std::list<Jadwal *>::iterator it = m_daftarJadwal.begin();
        it != m_daftarJadwal.end();
        ++it
    ) {
        delete (*it);
    }
    m_daftarJadwal.clear();

    unsigned int nJadwal;
    input.read((char *)&nJadwal, sizeof(unsigned int));
    for (unsigned int i = 0; i < nJadwal; i++) {
        Jadwal *jdw = new Jadwal(this, HariSenin, 0);
        jdw->deserialize(input);
        m_daftarJadwal.push_back(jdw);
    }

    std::string idDosen;
    strDeserialize(input, idDosen);
    if (!idDosen.empty()) {
        if (g_dosenDB.count(idDosen) == 1) {
            m_pengampu = g_dosenDB[idDosen];
        }
    } else {
        m_pengampu = NULL;
    }

    strDeserialize(input, m_labRiset);
}

// Jadwal

Jadwal::Jadwal(void) {
}

Jadwal::Jadwal(const Matakuliah *makul, HariID hari, int sesi) {
    m_makul = makul;
    m_hari = hari;
    m_sesi = sesi;
}

const Matakuliah *Jadwal::getMakul(void) const {
    return m_makul;
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

std::string Jadwal::getClassID(void) const {
    return ClassID_Jadwal;
}

void Jadwal::serialize(std::ostream &output) const {
    std::string classID = getClassID();
    strSerialize(output, classID);

    output.write((const char *)&m_hari, sizeof(HariID));
    output.write((const char *)&m_sesi, sizeof(int));
}

void Jadwal::deserialize(std::istream &input) {
    std::string classID;
    strDeserialize(input, classID);

    input.read((char *)&m_hari, sizeof(HariID));
    input.read((char *)&m_sesi, sizeof(int));
}


// Dosen

Dosen::Dosen(void) {
}

Dosen::Dosen(const std::string &id, const std::string &nama) {
    m_id = id;
    m_nama = nama;
}

const std::string &Dosen::getID(void) const {
    return m_id;
}

const std::string &Dosen::getNama(void) const {
    return m_nama;
}

std::string Dosen::getClassID(void) const {
    return ClassID_Dosen;
}

void Dosen::serialize(std::ostream &output) const {
    std::string classID = getClassID();
    strSerialize(output, classID);

    strSerialize(output, m_id);
    strSerialize(output, m_nama);
}

void Dosen::deserialize(std::istream &input) {
    std::string classID;
    strDeserialize(input, classID);

    strDeserialize(input, m_id);
    strDeserialize(input, m_nama);
}

