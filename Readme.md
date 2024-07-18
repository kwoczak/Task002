# Message Queue Priority 

## Opis 

Projekt demonstruje zarządzanie priorytetami wiadomości w systemie kolejek wiadomości POSIX. Nadawca (`sender`) wysyła wiadomości z różnymi priorytetami, a odbiorca (`receiver`) odbiera je w odpowiedniej kolejności priorytetów. Odbiorca odpowiada na każdą wiadomość, podając długość otrzymanej wiadomości.


## Wymagania

- System operacyjny Linux
- Kompilator g++ z obsługą standardu C++17
- POSIX message queues

Skompiluj projekt za pomocą Makefile

## Uruchomienie programu
Uruchom odbiorcę:

./receiver 

##Uruchom nadawcę:

./sender

Nadawca umoliwia wprowadzenie wiadomości i ich priorytety. Aby zakończyć program, wpisz exit jako wiadomość.
