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
