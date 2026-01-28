Opis Projektu
Aplikacja konsolowa C++ do bezstratnej kompresji i dekompresji plików tekstowych algorytmem Huffmana. Optymalizuje rozmiar danych poprzez przypisywanie najkrótszych kodów binarnych najczęściej występującym znakom.
Funkcje
Kompresja: Analiza pliku, budowa drzewa Huffmana i zapis danych ze słownikiem.
Dekompresja: Odtwarzanie tekstu na podstawie słownika i ciągu bitów.
Kolejka Priorytetowa: implementacja kopca binarnego (bubbleUp, bubbleDown).
Testowanie: Moduł weryfikujący poprawność działania struktur danych.
Struktura Programu
Node: Struktura węzła (znak, waga, wskaźniki left/right).
PriorityQueue: Minimalny kopiec binarny do selekcji najmniejszych wag.
process(): Zliczanie znaków, budowa drzewa i eksport do pliku.
decompress(): Translacja kodów binarnych na tekst pierwotny.
Instrukcja Obsługi
Kompilacja: 
g++ -o huffman main.cpp
Uruchomienie: 
Wybierz opcję z menu:
1: Kompresja (wymaga pliku .txt).
2: Dekompresja pliku wynikowego.
3: Test poprawności kolejki priorytetowej.
