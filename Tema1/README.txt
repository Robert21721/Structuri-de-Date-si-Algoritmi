##########################

    ROLUL FUNCTIILOR

##########################

open_train_station
    - aceasta functie aloca memorie pentru fiecare gara si pentru peroane
    - initializeaza puterea locomotivei cu -1 si primul vagon cu NULL

close_train_station
    - aceasta functie dezaloca memoria pentru fiecare tren, peron si apoi
        pentru gara

show_existing_trains
    - aceasta functie scrie in fisier trenurile existente la acel moment
        respectand structura din enunt

arrive_train
    - aceasta functie adauga o locomotiva pe un peron daca acesta este liber

leave_train
    - aceasta functie reseteaza puterea locomotivei la -1, primul vagon la NULL
        si sterge vagoanele existente de pe o anumita linie

add_train_car
    - aceasta functie adauga un vagon la un tren

remove_train_cars
    - aceasta functie scoate toate vagoanele de o anumita greutate dintr-un tren

move_train_cars
    - aceasta functie muta un numar de vagoane incepand cu pozitia a dintr-un tren
        la pozitia b din alt tren

find_express_train
    - aceasta functie gaseste trenul unde diferenta dintre puterea locomotivei si
        greutatea vagoanelor este maxima

find_overload_train
    - aceasta functie gaseste trenul care are greutatea totala a vagoanelor mai mare
        decat putera locomotivei

find_optimal_train
    - aceasta functie gaseste trenul care are greutatea vagoanelor cat mai mare, fara a 
        depasi insa puterea locomotivei


find_heaviest_sequence_train
    - aceasta functie gaseste cea mai grea secventa de cars_no vagoane

order_train
    - aceasta functie are rolul de a ordona vagoanele in ordine descrescatoare
    - este implementata in 2 moduri: prima (cea comentata) modifica doar greutatile
        vagoanelor in timp ce a doua muta structurile cu totul folosind legaturile
        de next

fix_overload_train
    - aceasta functie are rolul de a scoate un vagon astfel incat greutatea totala a
        vagoanelor ramase sa fie maxima fara a depasi insa puterea locomotivei