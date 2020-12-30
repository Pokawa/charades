# charades
[![Build Status](https://travis-ci.org/Pokawa/charades.svg?branch=main)](https://travis-ci.org/Pokawa/charades)


```commandline
mkdir build
cd build
cmake ..
make
```
executables in `build/apps/server` and `build/apps/client`

# Opis
## Kalambury a’la kurnik

* **Jan Todek 141326**
* **Hubert Cwajda 141203**

Gracz łączy się do serwera i wysyła swój nick (jeśli nick jest już zajęty,
serwer prosi o podanie innego nicku).

Gracz po wybraniu nicku trafia do lobby, w którym widzi bieżącą listę pokoi
i ilość graczy która jest w każdym pokoju oraz ich nicki.

Z lobby gracz może wejść do istniejącego pokoju lub założyć nowy. Gracz może
w każdej chwili wrócić z pokoju do lobby.

Jeśli w pokoju trwa gra, gracz dołącza jako zgadujący. Jeśli nie, gracz widzi
listę graczy którzy są w pokoju i czeka na rozpoczęcie gry.

Grę może rozpocząć gracz który najdłużej jest w pokoju, pod warunkiem że w
pokoju jest przynajmniej dwóch graczy.

Gracze, żeby rysować muszą zaznaczyć opcję zgody na rysowanie. Gracze ustawiani są w kolejkę do rysowania w kolejności zgłoszenia zgody, a po zakończeniu rysowania dodawani są z powrotem na koniec kolejki. (jeśli mają zaznaczoną dalej zgodę)

Gdy przyjdzie kolej rysującego, otrzymuje on hasło wylosowane przez serwer. Zgadujący widzą ile słów jest w haśle oraz udzielają odpowiedzi na publicznym chatcie. Czas rundy jest ograniczony.

Zgadujący mają za zadanie zgadnąć całe hasło. Jeśli wpisze on co najmniej n-znakowy ciąg zawierający się w haśle to serwer powiadomi chat że wpisana fraza jest blisko hasła. Kiedy upłynie połowa czasu na zgadywanie serwer podaje pierwszą literę hasła, w ostatniej ćwiartce czasu podaje drugą literę hasła.

Kiedy całe hasło zostało odgadnięte, lub kiedy upłynął określony czas, lub
kiedy rozłączył się pokazujący, kończy się runda, następuje przyznanie punktów oraz pokazującym zostaje następna osoba z kolejki. Przyznane punkty to ilość znaków hasła dla zgadującego oraz podłoga połowy tej liczby dla rysującego, w przypadku nie odgadnięcia hasła przez żadnego ze zgadujących, rysujący traci  podłogę połowy tej liczby.

Kolejnością sprawdzania poprawności haseł, jest kolejność przychodzenia na serwer( zwycięzca jest jeden)

Gra trwa do momentu aż w pokoju jest przynajmniej dwóch graczy lub któryś z graczy osiągnie próg punktowy. Gracze na bieżąco widzą ranking graczy z punktami, włączając w to też graczy którzy rozłączyli się z gry.
