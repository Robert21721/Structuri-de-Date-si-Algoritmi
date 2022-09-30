#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"


/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    TrainStation *station = (TrainStation*) malloc(sizeof(TrainStation));

    station->platforms_no = platforms_no;
    station->platforms = (Train**) malloc(platforms_no * sizeof(Train*));

    for (int i = 0; i < platforms_no; i++) {
        station->platforms[i] = (Train*) malloc(sizeof(Train));
        station->platforms[i]->locomotive_power = -1;
        station->platforms[i]->train_cars = NULL;
    }

    return station;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    for (int i = 0; i < station->platforms_no; i++) {
        leave_train(station, i);
        free(station->platforms[i]);
    }

    free(station->platforms);
    free(station);
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if (station == NULL) {
        return;
    }
    for (int i = 0; i < station->platforms_no;i ++) {
        fprintf(f, "%d: ", i);

        if (station->platforms[i]->locomotive_power >= 0) {
            fprintf(f, "(%d)", station->platforms[i]->locomotive_power);

            TrainCar* idx = station->platforms[i]->train_cars;
            while (idx != NULL) {
                fprintf(f, "-|%d|", idx->weight);
                idx = idx->next;
            }
        }
        fprintf(f, "\n");
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    if (0 <= platform && platform < station->platforms_no) {
        if (station->platforms[platform]->locomotive_power == -1) {
            station->platforms[platform]->locomotive_power = locomotive_power;
            station->platforms[platform]->train_cars = NULL;
        }
    }
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void delete_list(TrainCar* car) {
    if (car == NULL) {
        return;
    }
    
    delete_list(car->next);
    free(car);
}

void leave_train(TrainStation *station, int platform) {
    if (0 <= platform && platform < station->platforms_no) {
        station->platforms[platform]->locomotive_power = -1;
        delete_list(station->platforms[platform]->train_cars);
        station->platforms[platform]->train_cars = NULL;
    }
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    if (0 <= platform && platform < station->platforms_no) {
        // daca exista un tren pe peronul respectiv
        if (station->platforms[platform]->locomotive_power != -1) {
            // aloc memorie pentru noul vagon
            TrainCar *new_car = (TrainCar*) malloc(sizeof(TrainCar));
            new_car->next = NULL;
            new_car->weight = weight;

            // daca rste primul vagon
            if (station->platforms[platform]->train_cars == NULL) {
                station->platforms[platform]->train_cars = new_car;
                return;
            }

            // daca nu este primul vagon il adaug la final
            TrainCar *idx = station->platforms[platform]->train_cars;
            TrainCar *prev = NULL;

            while (idx != NULL) {
                prev = idx;
                idx = idx->next;
            }

            prev->next = new_car;
        }
    }
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    if (0 <= platform && platform < station->platforms_no) {
        if (station->platforms[platform]->locomotive_power != -1) {
            // in cazul in care primele vagoane trebuiesc scoase
            while (station->platforms[platform]->train_cars != NULL &&
                    station->platforms[platform]->train_cars->weight == weight) {

                TrainCar *aux = station->platforms[platform]->train_cars;
                station->platforms[platform]->train_cars = station->platforms[platform]->train_cars->next;
                free(aux);
            }

            // dupa ce primul vagon nu mai are greutatea weight
            // le scot pe celalte
            if (station->platforms[platform]->train_cars != NULL) {
                TrainCar *idx = station->platforms[platform]->train_cars->next;
                TrainCar *prev = station->platforms[platform]->train_cars;
            
                while (idx != NULL) {
                    if (idx->weight == weight) {
                        TrainCar *aux = idx;
                        prev->next = idx->next;
                        idx = idx->next;
                        free(aux);
                    } else {
                        prev = idx;
                        idx = idx->next;
                    }
                }
            }
        }
    }

}


/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, int cars_no, int platform_b, int pos_b) {
    if (pos_a < 1 || pos_b < 1) // daca numarul vagonului este invalid
        return;

    // daca nu exista vagoane in b pozitia de atasare e dif de 1
    if (station->platforms[platform_b]->train_cars == NULL && pos_b != 1) 
        return;

    if (station->platforms[platform_a]->train_cars == NULL) // daca nu exista vagoane in a
        return;
        
    int i_a = 1, i_b = 1;
    TrainCar *idx_a = station->platforms[platform_a]->train_cars; // index prin vagoanele trenului a
    TrainCar *idx_b = station->platforms[platform_b]->train_cars; // index prin vagoanele trenului b
    TrainCar *start = NULL; // vagonul de dupa care se schimba legaturile
    TrainCar *fin = NULL;   // vagonul care se leaga la final


    while (i_a + 1 < pos_a) {   // avansez prin vagoanele trenului a

        idx_a = idx_a->next;
        i_a++;

        if (idx_a == NULL)
        return;  
    }

    while (i_b + 1 < pos_b) {   // avansez prin vagoanele trenului b

        idx_b = idx_b->next;
        i_b++;

        if (idx_b == NULL)
            return;
    }

    if (pos_a == 1) {   // vagoanele atasate incep cu primul

        int i = 1;  // avansez cu cars_no vagoane - 1
        while (i < cars_no) {
            idx_a = idx_a->next;
            i++;

            if (idx_a == NULL)
                return;
        }
        fin = idx_a->next;  // retin pozitia primului vagon ce nu va fi mutat

        if (pos_b == 1) {   // daca si pozitia de inserare in b este 1 atunci fac inserarea la inceput

            station->platforms[platform_b]->train_cars = station->platforms[platform_a]->train_cars;
            station->platforms[platform_a]->train_cars = idx_a->next;
            idx_a->next = idx_b;
            return;
        }
    // daca pozitia de inserare in b nu este 1 atunci fac inserarea la jumatate sau la final
        idx_a->next = idx_b->next;
        idx_b->next = station->platforms[platform_a]->train_cars;
        station->platforms[platform_a]->train_cars = fin;
        return;
    }


    if (i_a + 1 == pos_a) { // daca pozitia de unde se iau vagoanele din a nu este 1
        start = idx_a;  // vagonul de dupa care se schimba legaturile

        int i = 0;  // avansez cu exact cars_no vagoane
        while (i < cars_no) {
            idx_a = idx_a->next;
            i++;

            if (idx_a == NULL)
                return;
        }
        fin = idx_a->next;  // retin pozitia primului vagon ce nu va fi mutat

        if (pos_b == 1) {   // daca inserarea are loc la inceput
            idx_a->next = station->platforms[platform_b]->train_cars;
            station->platforms[platform_b]->train_cars = start->next;
            start->next = fin; 
            return;
        }
    // daca inserarea are loc la jumatate sau la final
        idx_a->next = idx_b->next;
        idx_b->next = start->next;
        start->next = fin;
        return;
    }
}

/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    int max = 0;
    int platform = 0;
    for (int i = 0; i < station->platforms_no; i++) {
        TrainCar *idx = station->platforms[i]->train_cars;
        int total_weight = 0;

        while (idx != NULL) {
            total_weight = total_weight + idx->weight;
            idx = idx->next;
        }

        if (station->platforms[i]->locomotive_power - total_weight > max) {
            max = station->platforms[i]->locomotive_power - total_weight;
            platform = i;
        }
    }

    return platform;
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {

    for (int i = 0; i < station->platforms_no; i++) {
        TrainCar *idx = station->platforms[i]->train_cars;
        int total_weight = 0;

        while (idx != NULL) {
            total_weight = total_weight + idx->weight;
            idx = idx->next;
        }

        if (station->platforms[i]->locomotive_power < total_weight) {
            return i;
        }
    }

    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    TrainCar *idx = station->platforms[0]->train_cars;
    int platform = -1;
    int min = station->platforms[0]->locomotive_power;

    for (int i = 0; i < station->platforms_no; i++) {
        TrainCar *idx = station->platforms[i]->train_cars;
        int total_weight = 0;

        if (station->platforms[i]->locomotive_power != -1) {
            while (idx != NULL) {
                total_weight = total_weight + idx->weight;
                idx = idx->next;
            }

            if (station->platforms[i]->locomotive_power - total_weight < min) {
                min = station->platforms[i]->locomotive_power - total_weight;
                platform = i;
            }
        }
    }

    return platform;
}


/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    int max = 0;
    int platform = -1;
    *start_car = NULL;

    for (int i = 0; i < station->platforms_no; i++) {
        TrainCar *idx = station->platforms[i]->train_cars;
        TrainCar *start = station->platforms[i]->train_cars;

        while (idx != NULL) {
            int total_weight = 0;
            idx = start;
            int j = 0;

            // calculez greutatea celor cars_no vagoane
            for (j = 0; idx != NULL && j < cars_no; j++) {
                total_weight = total_weight + idx->weight;
                idx = idx->next;
            }

            // daca numarul cautat de vagoane este mai mare decat cel existent
            if (idx == NULL && j < cars_no) {
                break;
            }

            if (total_weight > max) {
                platform = i;
                max = total_weight;
                *start_car = start;
            }

            start = start->next;
        }
    }

    return platform;
}


/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
// #######

// doar prin schimbarea greutatilor

// #######

// void order_train(TrainStation *station, int platform) {
//     int ok = 1;

//     while (ok == 1) {
//         ok = 0;

//         TrainCar *idx = station->platforms[platform]->train_cars;

//         while (idx->next != NULL) {
//             if (idx->next->weight > idx->weight) {
//                 int aux = idx->next->weight;
//                 idx->next->weight = idx->weight;
//                 idx->weight = aux;
//                 ok = 1;
//             } else {
//                 idx = idx->next;
//             }
//         }
//     }
// }

    // sortare prin insertie pe lista simplu inlantuita
void order_train(TrainStation *station, int platform) {
    TrainCar *prev_car = station->platforms[platform]->train_cars;
    TrainCar *car_to_add = station->platforms[platform]->train_cars->next;

    // incepand de la al 2lea vagon voi insera vagoanele car_to_add
    // intre primele vagoane care sunt deja sortate (de la primul la idx)
    while (car_to_add != NULL) {    // cat timp inca am vagoane de inserat
        TrainCar *next_car = car_to_add->next;

        // daca vagonul este mai greu decat primul vagon (caz separat)
        // pun car_to_add pe prima pozitie si refac legaturile
        if (car_to_add->weight > station->platforms[platform]->train_cars->weight) {
            // adaug vagonul
            TrainCar *aux = station->platforms[platform]->train_cars;
            station->platforms[platform]->train_cars = car_to_add;
            car_to_add->next = aux;

            // refac legatura in locul de unde am luat car_to_add
            prev_car->next = next_car;

            car_to_add = next_car;
            continue; // trec la urmatorul vagon
        }
        
        // daca vagonul trebuie inserat de la pozitia 2 in sus
        // ii caut locul de inserare
        TrainCar *idx = station->platforms[platform]->train_cars;
        for (idx = station->platforms[platform]->train_cars; idx != car_to_add; idx = idx->next) {
            if (car_to_add->weight > idx->next->weight) {
                // adaug vagonul
                TrainCar *aux = idx->next;
                idx->next = car_to_add;
                car_to_add->next = aux;

                // refac legatura in locul de unde am luat car_to_add
                prev_car->next = next_car;

                car_to_add = next_car;
                break;
            }
        }

        // daca nu a trebuit schimbata pozitia vagonului (ordinea este corecta)
        // avansez cu prev_to_add si car_to_add
        if (idx == car_to_add) {
            prev_car = car_to_add;
            car_to_add = next_car; 
        }
     }
}


/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    int platform = find_overload_train(station);

    // daca nu exista un tren supraincarcat
    if (platform < 0) {
            return;
        }

    int min = station->platforms[platform]->locomotive_power;
    int total_weight = 0;
    TrainCar *car = NULL;

    // calculez greutatea totala
    TrainCar *idx = station->platforms[platform]->train_cars;
    while (idx != NULL) {
        total_weight = total_weight + idx->weight;
        idx = idx->next;
    }

    // aflu ce vagon ar trebui scos astfel incat incarcatura sa fie optima
    idx = station->platforms[platform]->train_cars;
    while (idx != NULL) {
        if (total_weight - idx->weight <= station->platforms[platform]->locomotive_power && idx->weight < min) {
                min = idx->weight;
                car = idx;
            }
        idx = idx->next;
    }

    // in cazul in care vagolul care trebuie scos este primul
    if (car == station->platforms[platform]->train_cars) {
        station->platforms[platform]->train_cars = car->next;
        free(car);
        return;
    }

    // in cazul in care vagonul nu este primul
    idx = station->platforms[platform]->train_cars;
    while (idx->next != car) {
        idx = idx->next;
    }

    idx->next = idx->next->next;
    free(car);
}
