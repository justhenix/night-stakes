# Night Stakes 🎲💀

> *"One night. Win it all, or lose it all."*

**Night Stakes** adalah permainan simulasi judi berbasis *Command Line Interface* (CLI) dengan nuansa *thriller* psikologis. Bermain sebagai seseorang yang terlilit hutang dan terdesak, kamu memasuki sebuah ruangan remang-remang untuk mempertaruhkan satu-satunya hal yang tersisa: nyawamu.

![Language](https://img.shields.io/badge/Language-C-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey)

## 🕯️ Sinopsis

Kamu berada di sebuah ruangan gelap. Di seberang meja hijau yang lusuh, seorang Dealer menunggu. Hutangmu menumpuk, surat pengusiran sudah datang, dan asuransi menolak klaimmu. Ini adalah jalan buntu atau jalan keluar.

Tugasmu sederhana: Menangkan serangkaian permainan kecil, kumpulkan uang, dan (semoga) keluar hidup-hidup. Tapi ingat, Dealer punya aturan sendiri, dan "The House" tidak suka kalah.

## 🃏 Fitur Permainan

* **Atmospheric Storytelling:** Narasi deskriptif dengan efek ketik (*typewriter effect*) untuk membangun ketegangan.
* **3 Minigames Mematikan:**
    1.  **eX Ou (Tic-Tac-Toe):** Melawan AI Dealer.
    2.  **Smart Test (Math & Riddles):** Uji logika dan matematika dasar.
    3.  **Russian Roulette:** Ujian keberuntungan terakhir.
* **Multiple Endings:** Terdapat **5 Akhir Cerita (Endings)** yang berbeda tergantung pada jumlah uang yang kamu kumpulkan, keputusanmu untuk berhenti atau lanjut, dan nasibmu di meja judi:
    * Ending 1: *Hollow*
    * Ending 2: *Enough*
    * Ending 3: *More Debt*
    * Ending 4: *Peace*
    * Ending 5: *Next Time*
* **Sistem Ekonomi & Nyawa:** Kelola uangmu dengan bijak. Jika bangkrut, permainan berakhir.

## ⚠️ Peringatan (Disclaimer)

Game ini mengandung tema perjudian, *Russian Roulette*, dan deskripsi kematian (teks). Ditujukan untuk tujuan hiburan dan narasi fiksi semata.

## 🛠️ Cara Menjalankan

Game ini ditulis dalam bahasa C standar dan kompatibel dengan Windows (menggunakan `windows.h`) maupun Linux/macOS (menggunakan `unistd.h`).

### Persiapan
Disarankan untuk memainkannya dalam mode **Fullscreen** atau memaksimalkan jendela terminal agar tampilan ASCII Art dan teks tidak berantakan.

### Kompilasi & Run

1.  **Clone repositori:**
    ```bash
    git clone [https://github.com/justhenix/night-stakes.git](https://github.com/justhenix/night-stakes.git)
    cd night-stakes
    ```

2.  **Compile kode:**
    * **Windows (MinGW/GCC):**
        ```bash
        gcc main.c -o nightstakes.exe
        ```
    * **Linux/macOS:**
        ```bash
        gcc main.c -o nightstakes
        ```

3.  **Mainkan:**
    * **Windows:** Klik ganda `nightstakes.exe` atau ketik `.\nightstakes` di terminal.
    * **Linux:** Ketik `./nightstakes` di terminal.

## 🎮 Kontrol

* Ketik jawaban (Angka/Huruf) lalu tekan **Enter**.
* Tekan sembarang tombol saat diminta untuk melanjutkan cerita.

## 👨‍💻 Kredit

* **Creator:** Gamma Rasyad
* **License:** MIT License

---
*"The house always wins... isn't it?"*
