# itsr_tugas_magang

Magang Modul 2

*Explaination*: The Golden Spiral Turtle
_side note: mayoritas code dibuat menggunakan AI, namun saya akan menjelaskan logika yang telah tertulis_

Golden Ratio:
Dalam matematika, dua nilai dianggap berada dalam hubungan Golden Ratio (φ) jika rasio antara jumlah kedua nilai itu terhadap nilai yang besar sama dengan rasio antara nilai besar terhadap nilai kecil. Nilai yang lebih besar dilambangkan dengan huruf a, sedangkan nilai yang lebih kecil dilambangkan dengan huruf b. (source: WIkipedia)

Golden Spiral:
Dalam geometri , Golden Spiral adalah spiral logaritmik yang faktor pertumbuhannya adalah φ. Artinya, Golden Spiral menjadi lebih lebar (atau semakin jauh dari titik asalnya) dengan faktor φ untuk setiap seperempat putaran yang dilakukannya. (source: Wikipedia)

Sebenarnya Golden Spiral bisa berlanjut sampai tidak hingga, namun karena keterbatasan tempat dan memori dari turtlesim, kita akan hanya membuat contoh kecil (serta kita mulai dari "luar", bukan dari titik origin/tengah)

Nilai & Rumus yang akan kita gunakan:
-. Nilai φ ≈ 1.618033988749.... (nanti akan dibulatkan sampai 1.618)
-. Nilai π ≈ 3.141592653589.... (nanti akan dibulatkan sampai 3.14159)
-. b = ln(φ) / (π/2)
-. kecepatan angular = 1.5 (standard)
-. kecepatan linear = 8 * e^(-b*t)
(8 adalah kecepatan maximal, kita dapat menggati value secara manual jika ingin spiral lebih besar atau kecil;
t akan selalu terhitung setiap 0.02s / perhitungan interval 50Hz sampai mencapai 15s lalu akan berhenti)

kecepatan linear akan lama kelamaan melambat (decay) dimana kelambatan linear tersebut menghasilkan Golden Spiral

Urutan code yang dijalankan:
1. "Teleport" turtle di titik x = 4.0 & y = 2.0 (bisa diubah manual)
2. Menghilangkan jejak "teleport" & gambar line sebelumnya
2a. Mengubah warna line jadi "gold"
3. Menjalankan kode menggunakan kecepatan linear & angular




