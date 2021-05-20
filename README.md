# flappyBird
# repo
# Artur Czarnik & Tomasz Szczepanik

Niniejszy projekt dotyczy implementacji gry FlappyBird w standardowej wersji, tj. użytkownik sterujący ptakiem stara się uniknąć zderzenia z przesuwającymi się w jego kierunku podłużnymi przeszkodami.

-> ze względów implementacyjnych, ptak będzie faktycznie poruszać się jedynie wzdłuż prostej góra - dół, natomiast przeszkody będą przesuwać się w jego kierunku (z uwagi na względność ruchu i przyjętego układu odniesienia, jest to podejście równoważne)

-> areną działań będzie tablica 2D, z możliwie wysoką częstotliwością modyfikowana i wyświetlana w konsoli, w której każda komórka będzie odpowiadać najmniejszej niepodzielnej widzianej przez użytkownika cząstce grafiki

-> program będzie przyjmować interakcję użytkownika, modyfikując względem takowej trajektorię lotu ptaka, natomiast przeszkody będą "przesuwać się" w "lewo" tablicy, pojawiając się zgodnie z określonym algorytmem

-> jeżeli zestaw komórek symbolizujący ptaka przetnie się niepusto z którąś z przeszkód (bądź zbliży się do obramowania tablicy), wówczas gra się kończy

-> ptak może być np. obiektem, z polami definiującymi jego współrzędne w globalnej tablicy (tak samo jak i przeszkody)

Zdajemy sobie sprawę z trudności, które mogą się pojawić, przede wszystkim z dokładnym sposobem wyświetlania tablicy na konsoli ("czyszczeniu konsoli", bądź też ciągłym wypisywaniu nowych tablic), częstotliwością, która przy zachowaniu grywalności, byłaby akceptowalna przez parametry komputera oraz dopasowaniu rozmiaru tablicy do rozmiaru konsoli. Teraz chcemy poszukać rozwiązań tych problemów, nim przystąpimy do samej implementacji.


# Raport z dnia 13 maja 2021 r.

-> zastąpiliśmy w głównej pętli funkcję odpowiadającą za "czyszczeniu konsoli", która powodowała migotanie z taką, że niweluje ten problem praktycznie do zera

-> rozwiązaliśmy już większość problemów jakie na początku nam były postawione, przez co będziemy mogli przekształcać obecny algorytm do wersji ostatecznej

-> problemów, bądź większych błędów, które mogłyby powstrzymać nas od dalszej realizacji projektu nie stwierdziliśmy. 

# Raport z dnia 20 maja 2021 r.

-> przeszkody przyjęły ostateczną formę (tj. mają przerwę), są kreowane losowo

-> sterowalny obiekt (@), gra kończy się na razie wyłącznie wtedy, gdy obiekt poleci zbyt wysoko/nisko (do dopisania: przerwanie gry, gdy ptak trafi w przeszkodę, niewielka zmiana kodu)

-> wprowadziliśmy menu wejściowe - wyjściowe (niektóre funkcjonalności pozostaje dopisać - zapisanie gry, lista wyników; ew. szybsze wyświetlanie menu)

-> większych problemów nie było (zrezygnowaliśmy jedynie z szybszego czyszczenia ekranu, z powodu dziwnego działania, którego nie rozumiem (Tomasz Szczepanik)

-> nie korzystaliśmy z żadnego tutorialu, idea & mechanika gry jest naszą autorską
-> szukając konkretnych rzeczy, których potrzebowaliśmy do implementacji, użyliśmy przykładowych stron
https://cpp0x.pl/
https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
zaś istnienie takich funkcji/bibliotek sugerowali internauci na forach internetowych, na które się natknęliśmy np. wyszukując możliwości weryfikacji niekoniecznej interakcji użytkownika z klawiatury, bez konieczności użycia cin, który przerywałby wyświetlanie gry (i wymaga zawsze interakcji, a przecież gracz nie zawsze będzie naciskać spację)
