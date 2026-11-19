# Opis projektu z przedmiotu Systemy Operacyjne (2026/2027)

Autor: PB bo RODO
Nr albumu: 1....9 bo RODO
Wybrany temat: Temat 1 – Stacja paliw
Link do repozytorium GitHub: https://github.com/Voxner-bah/so-stacja-paliw.git


## 1. Opis zadania projektowego i zasad symulacji

Celem projektu jest wieloprocesowa symulacja działania stacji paliw napisana w języku C w środowisku Linux (torus) z wykorzystaniem mechanizmów komunikacji i synchronizacji międzyprocesowej (IPC). Symulacja wykorzystuje współczynnik skali czasu, który pozwala przyspieszać lub spowalniać upływ czasu symulacyjnego względem czasu rzeczywistego.

### Infrastruktura stacji paliw

* **Stanowiska tankowania (8 dystrybutorów):**
  * **Stanowiska 1-6 (uniwersalne):** obsługują benzynę E5, E10 oraz olej napędowy B7, B10.
  * **Stanowisko 7 (uniwersalne + LPG):** obsługuje E5, E10, B7, B10 oraz autogaz LPG.
  * **Stanowisko 8 (dedykowane):** obsługuje wyłącznie pojazdy ciężarowe i autobusy tankujące olej napędowy B7 lub B10.
* **Stanowiska szybkiego ładowania EV (2 stanowiska):**
  * Ładowarki DC 50 kW / 100 kW dla samochodów elektrycznych. Czas ładowania odpowiada przedziałowi 40-60 minut czasu symulacyjnego.
* **Parking (10 miejsc parkingowych):**
  * Służy kierowcom robiącym wyłącznie zakupy w bistro oraz tym, którzy po zatankowaniu zjeżdżają ze stanowiska na parking przed zapłatą (ok. 2% tankujących).
* **Pawilon stacyjny (3 kasy, w tym strefa bistro):**
  * W godzinach 9:00-18:00 działają zawsze co najmniej 2 kasy (w tym jedna w strefie bistro).
  * Poza godzinami 9:00-18:00 działa zawsze co najmniej 1 kasa (w strefie bistro).

### Zasady obsługi pojazdów i klientów

* **Nadjeżdżające pojazdy i limit na stacji:** W losowych momentach na stację podjeżdżają pojazdy na paliwa E5, E10, B7, B10, LPG, CNG/LNG, H2, samochody elektryczne (EV) oraz klienci nietankujący (ok. 2% wszystkich klientów robi tylko zakupy w bistro). Łączna liczba pojazdów w symulacji jest losowana z większego zakresu, ale jednocześnie na terenie stacji może przebywać maksymalnie 100 kierowców (lub mniej, zgodnie z ustawieniem). Gdy któryś pojazd odjedzie, kierownik wpuszcza kolejne czekające samochody.
* **Niedostępne paliwa:** Pojazdy na paliwa niedostępne na stacji (CNG/LNG, H2) odjeżdżają bez zajmowania kolejek do dystrybutorów.
* **Wybór stanowiska:** Kierowcy ustawiają się do pasujących im dystrybutorów lub ładowarek według zasady najkrótszej kolejki.
* **Tankowanie i zerowanie licznika:**
  * Tankowanie można rozpocząć dopiero wtedy, gdy miejsce przy dystrybutorze jest wolne oraz licznik wskazuje 0.00.
  * Licznik zeruje (zwalnia dystrybutor) kasjer po opłaceniu rachunku przez poprzedniego kierowcę.
* **Odjazd na parking przed zapłatą i pierwszeństwo:**
  * Około 2% kierowców zwalnia dystrybutor przed zapłatą i jedzie na parking (zakładamy, że nie kradną i idą zapłacić do kasy).
  * Jeśli wszystkie 10 miejsc na parkingu jest zajętych, kierowca czeka na wolne miejsce. Kierowcy po tankowaniu mają w kolejce na parking **wyższy priorytet** niż klienci przyjeżdżający tylko na zakupy.
  * Nawet jeśli poprzedni kierowca odjechał ze stanowiska na parking, kolejny kierowca nie może zacząć tankowania, dopóki rachunek nie zostanie opłacony w kasie i licznik nie wskaże 0.00.
* **Zakupy w bistro i zasady działania kas:**
  * Około 20% tankujących kierowców robi dodatkowo zakupy w strefie bistro.
  * Klienci w pawilonie wybierają kasę według zasady najkrótszej kolejki.
  * **Otwieranie i zamykanie kas:**
    * Jeśli w kolejce do kasy stoi więcej niż 5 osób, otwiera się kolejna kasa (maksymalnie 3 czynne kasy).
    * Jeśli spełniony jest warunek **K / (N - 1) < 5**, gdzie K to liczba klientów we wszystkich kolejkach, a N to liczba czynnych kas, jedna z kas zostaje zamknięta (po obsłużeniu obecnej kolejki i z zachowaniem minimum dla danej godziny).
  * **Klienci VIP:** Stanowią mniej niż 0.3% kierowców i płacą w kasie bez kolejki.

## 2. Architektura systemu i procesy

Symulacja działa w modelu rozproszonym na osobnych procesach uruchamianych przez funkcje fork() i exec():

* **Proces główny (main):**
  * Tworzy i konfiguruje zasoby IPC (pamięć dzieloną, semafory, kolejki komunikatów, plik logów) z minimalnymi prawami dostępu (0600).
  * Sprawdza poprawność danych wejściowych, ustawia współczynnik skali czasu oraz pilnuje limitów procesów na serwerze torus.
  * Uruchamia proces kierownika, 3 osobne procesy kasjerów oraz generuje procesy kierowców.
  * Zarządza zegarem symulacji (godziny 9:00-18:00 oraz noc), odbiera zakończone procesy przez wait() / waitpid() i usuwa zasoby IPC na końcu działania.
* **Kierownik stacji (kierownik):**
  * Nadzoruje ruch na stacji, pilnuje limitu jednocześnie obecnych kierowców (wpuszcza kolejnych po zwolnieniu miejsca) oraz wysyła sygnały sterujące.
* **Kasjer (kasjer – 3 osobne procesy, bez wątków):**
  * Obsługuje kolejki klientów (z pierwszeństwem dla VIP), pobiera opłaty za paliwo i bistro, zeruje liczniki dystrybutorów do 0.00 oraz otwiera lub zamyka kasy według warunku K / (N - 1) < 5.
* **Kierowca (kierowca):**
  * Osobny proces pojazdu z wylosowanymi cechami: typ pojazdu (osobowy, ciężarowy, autobus), rodzaj paliwa, status VIP, zakupy w bistro oraz ewentualny zjazd na parking przed zapłatą.

### Wykorzystane mechanizmy systemowe IPC

* **Pamięć dzielona (shmget, shmat, shmdt, shmctl):** Przechowuje aktualny stan stacji: stan otwarcia i liczniki 8 dystrybutorów oraz 2 ładowarek, długości kolejek, zajętość 10 miejsc parkingowych, liczbę klientów K, liczbę czynnych kas N, liczbę kierowców na stacji, zegar symulacji i współczynnik skali czasu.
* **Semafory (semget, semop, semctl):** Chronią pamięć dzieloną i plik logów przed jednoczesnym zapisem oraz synchronizują dostęp do dystrybutorów, ładowarek, parkingu (z priorytetem dla tankujących) i limitu aut na stacji bez aktywnego czekania.
* **Kolejki komunikatów (msgget, msgsnd, msgrcv, msgctl):** Obsługują wymianę informacji między kierowcami a kasjerami (rozliczenie paliwa, bistro, potwierdzenie zapłaty). Pole typu komunikatu pozwala obsługiwać klientów **VIP** przed zwykłymi klientami.
* **Sygnały (sigaction, kill):**
  * **Sygnał 1 (SIGUSR1):** Zamknięcie wybranego stanowiska tankowania lub ładowania. Pojazd w trakcie tankowania kończy operację, a czekające w kolejce przechodzą na inne stanowiska.
  * **Sygnał 2 (SIGUSR2):** Ponowne otwarcie wcześniej zamkniętego stanowiska.
  * **Sygnał 3 (SIGTERM):** Zamknięcie stacji dla nowych pojazdów. Obecni kierowcy kończą tankowanie i płacą, a po wyjeździe ostatniego auta symulacja kończy się.
  * **Sygnał 4 (SIGINT / SIGQUIT – Pożar):** Natychmiastowa ewakuacja stacji. Kasy przerywają pracę, wszyscy opuszczają teren, a zasoby IPC są usuwane.

## 3. Opis testów

### Test 1: Blokada licznika dystrybutora (0.00) i priorytet parkingowy

* **Cel testu:** Sprawdzenie, czy odjazd kierowcy na parking przed zapłatą blokuje tankowanie dla kolejnego auta do momentu opłacenia rachunku oraz czy tankujący mają pierwszeństwo wjazdu na parking.
* **Warunki początkowe:** Wszystkie 10 miejsc na parkingu są zajęte, szansa zjazdu przed zapłatą zostaje tymczasowo podniesiona do 50%, a przed parkingiem czekają klienci po tankowaniu i klienci idący tylko do bistro.
* **Oczekiwany rezultat:** Kierowca po tankowaniu wjeżdża na zwolnione miejsce przed klientem nietankującym. Kolejny pojazd przy dystrybutorze czeka z rozpoczęciem tankowania, aż kasjer wyzeruje licznik do 0.00. Nie dochodzi do zakleszczenia.

### Test 2: Dynamiczne otwieranie i zamykanie kas oraz obsługa VIP

* **Cel testu:** Sprawdzenie działania otwierania kasy przy kolejce większej niż 5 osób, zamykania przy warunku K / (N - 1) < 5, limitów godzinowych (9:00-18:00 i noc) oraz obsługi VIP bez kolejki.
* **Warunki początkowe:** Szybkie wpuszczenie grupy kierowców do kas, aby kolejka przekroczyła 5, a potem 10 osób, dodanie klienta VIP, a następnie zatrzymanie napływu nowych klientów.
* **Oczekiwany rezultat:** Otwierają się kolejne kasy aż do 3. Klient VIP zostaje obsłużony jako pierwszy mimo pełnej kolejki. Po spadku liczby klientów kasy zamykają się do minimum dla bieżącej godziny.

### Test 3: Podział dystrybutorów (stanowiska 7 i 8) oraz odrzucanie złych paliw

* **Cel testu:** Sprawdzenie kierowania aut do właściwych dystrybutorów i odrzucania niedostępnych paliw.
* **Warunki początkowe:** Wygenerowanie aut na LPG, ciężarówek i autobusów (B7/B10), aut osobowych (B7) oraz pojazdów na H2 i CNG.
* **Oczekiwany rezultat:** Pojazdy H2 i CNG od razu opuszczają stację. Ciężarówki i autobusy jadą wyłącznie na stanowisko 8, a auta osobowe nie zajmują stanowiska 8. Pojazdy LPG ustawiają się wyłącznie na stanowisku 7.

### Test 4: Sygnały kierownika (zamykanie stanowisk, koniec pracy i pożar)

* **Cel testu:** Sprawdzenie reakcji procesów na wszystkie 4 sygnały sterujące.
* **Warunki początkowe:** Wypełnione kolejki przy dystrybutorach. Wysłanie Sygnału 1 dla stanowiska 1, potem Sygnału 2, a w kolejnych uruchomieniach Sygnału 3 oraz Sygnału 4 (pożar).
* **Oczekiwany rezultat:** Po Sygnale 1 auto kończy tankowanie, a oczekujące zmieniają kolejkę. Po Sygnale 3 nie wjeżdżają nowe auta i po wyjeździe obecnych program kończy się czysto. Po Sygnale 4 wszyscy natychmiast opuszczają stację, a struktury IPC znikają z systemu.

### Test 5: Test przeciążeniowy, limit 100 kierowców i skala czasu (lub po prostu test przeciążeniowy, lub do zmiany)

* **Cel testu:** Sprawdzenie stabilności przy losowej, dużej liczbie pojazdów, poprawności limitu maksymalnie 100 (lub mniej) kierowców jednocześnie na stacji oraz działania współczynnika skali czasu.
* **Warunki początkowe:** Ustawienie szybkiego współczynnika skali czasu, wylosowanie dużej łącznej liczby pojazdów (np. 300-500) przy limicie jednoczesnych kierowców na stacji ustawionym na 100 (lub mniej).
* **Oczekiwany rezultat:** Kierownik pilnuje, aby liczba aktywnych kierowców na stacji nigdy nie przekroczyła ustalonego limitu (maksymalnie 100), wpuszczając kolejne pojazdy dopiero po wyjeździe poprzednich. System nie przekracza limitu procesów użytkownika na serwerze torus, nie powstają procesy zombie, a po teście wszystkie zasoby IPC są usunięte.