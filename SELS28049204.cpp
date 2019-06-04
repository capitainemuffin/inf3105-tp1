#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <algorithm>

class Intervale {
public :
    double p1, p2;
    std::vector<Intervale> ptr_intervales_y;

    Intervale(double p1, double p2):p1(p1), p2(p2){
    }

    Intervale(Intervale& i1, Intervale& i2){

        this->p1 = i1.p1;
        this->p2 = i2.p2;
    }
};


/**
* Représentation d'un point sur les axes x et y
*/
class Point {

public:

    double x, y;

    /**
     * Constructeur par défaut d'un Point, coordonnées (0,0)
     */
    Point() : x(0), y(0) {};

    /**
     * Constructeur qui initialize les coordonnées du Point
     *
     * @param x la coordonnée x du Point
     * @param y la coordonnée y du Point
     */
    Point(double _x, double _y) : x(_x), y(_y) {};

    friend std::ostream &operator<<(std::ostream &, Point &);

};

/**
 * Représentation d'un rectangle
 */
class Rectangle {

private:

    /**
     * Représentation d'un type positif ou négatif
     */
    enum class Type {

        positif, negatif

    };

public:

    Type type;
    double longueur, hauteur;
    Point sup_gauche, sup_droit, inf_gauche, inf_droit;
    int niveau;
    /**
     * Constructeur qui initialize les 4 Coins du rectangle
     *
     * @param type positif ou negatif
     * @param x point milieu sur l'axe des x
     * @param y point milieu sur l'axe des y
     * @param longueur longueur du rectangle
     * @param hauteur hauteur du rectangle
     */
    Rectangle(char type, double x, double y, double longueur, double hauteur, int niveau) {

        assert(type == 'p' || type == 'n');
        assert(longueur > 0 && hauteur > 0);

        this->type = type == 'p' ? Type::positif : Type::negatif;
        this->sup_gauche = Point(x - longueur / 2, y + hauteur / 2);
        this->sup_droit = Point(x + longueur / 2, y + hauteur / 2);
        this->inf_gauche = Point(x - longueur / 2, y - hauteur / 2);
        this->inf_droit = Point(x + longueur / 2, y - hauteur / 2);

        this->longueur = longueur;
        this->hauteur = hauteur;
        this->niveau = niveau;

    }

    /**
     * Teste si le rectangle est positif ou negatif
     *
     * @return true si positif, false sinon
     */
    bool estPositif() const {
        return this->type == Type::positif;
    }

    /**
     * Teste si le premier Rectangle est plus petit que le deuxième.
     *
     * @param rec1 instance de Rectangle
     * @param rec2 instance de Rectangle
     * @return true si plus petit, faux sinon
     */
    friend bool operator<(const Rectangle &rec1, const Rectangle &rec2) {

        bool plusPetit = false;

        if (rec1.inf_gauche.x < rec2.inf_gauche.x) {

            plusPetit = true;

        } else if (rec1.inf_gauche.x == rec2.inf_gauche.x && rec1.inf_gauche.y < rec2.inf_gauche.y) {

            plusPetit = true;

        }

        return plusPetit;

    }

    bool croise(const Rectangle &rec2) {

        return
                this->sup_gauche.x < rec2.inf_droit.x &&
                this->inf_droit.x > rec2.sup_gauche.x &&
                this->sup_gauche.y > rec2.inf_droit.y &&
                this->inf_droit.y < rec2.sup_gauche.y;

    }

    Rectangle croisement(const Rectangle &rec2) {

        double x_gauche = std::max(this->inf_gauche.x, rec2.inf_gauche.x);
        double x_droit = std::min(this->sup_droit.x, rec2.sup_droit.x);
        double y_haut = std::min(this->sup_gauche.y, rec2.sup_gauche.y);
        double y_bas = std::max(this->inf_droit.y, rec2.inf_droit.y);

        return Rectangle('p', x_gauche, x_droit, x_droit - x_gauche, y_haut - y_bas, rec2.niveau + 1);
    }

    friend std::istream &operator>>(std::ifstream &, Rectangle &);

    friend std::ostream &operator<<(std::ostream &, Rectangle &);

};

/**
 * Représente une grille avec tous les rectangles à l'intérieur
 */
class Grille {

public:

    std::vector<Rectangle> rectangles_positifs;
    std::vector<Rectangle> rectangles_negatifs;

    /**
     * Ajoute le rectangle à la grille
     *
     * @param rectangle
     */
    void ajouter(const Rectangle &rectangle) {

        if (rectangle.estPositif()) {
            this->rectangles_positifs.push_back(rectangle);
            std::sort(rectangles_positifs.begin(), rectangles_positifs.end());

        } else {

            this->rectangles_negatifs.push_back(rectangle);
            std::sort(rectangles_negatifs.begin(), rectangles_negatifs.end());

        }

    }

    /**
     * Calcule l'aire de tous les rectangles
     *
     * @return aire
     */
    long double aire() {

        std::vector<Intervale> intervales_x;
        std::vector<double> tous_les_x;

        // calculer les intervales x
        for (auto& rectangle : this->rectangles_positifs){

            double temp = rectangle.inf_gauche.x;

            if(!std::find(tous_les_x.begin(), tous_les_x.end(), temp)){

                tous_les_x.push_back(temp);
            }

            temp = rectangle.inf_droit.x;

            if(!std::find(tous_les_x.begin(), tous_les_x.end(), temp)){

                tous_les_x.push_back(temp);
            }
        }

        std::sort(rectangles_positifs.begin(), rectangles_positifs.end());



        //calculer les intervales y
        for(auto& intervale_x : intervales_x){

            for(auto& rectangle : this->rectangles_positifs){

                if (rectangle.inf_gauche.x <= intervale_x.p1 && rectangle.inf_droit.x >= intervale_x.p2){
                    //ce rectangle est dans cet intervale, mettre les intervales y1 et y2
                    const Intervale intervale_y = Intervale(rectangle.inf_gauche.y, rectangle.sup_gauche.y);

                    intervale_x.ptr_intervales_y.push_back(intervale_y);

                }
            }
        }

        //fusionner les intervalles y qui se chevauchent
        for(auto& intervale_x : intervales_x){

            std::vector<Intervale> intervales_fusionnees;

            for(int i = 0 ; i < intervale_x.ptr_intervales_y.size() -1 ; i++){


                Intervale intervale_1 = intervale_x.ptr_intervales_y[i];
                Intervale intervale_2 = intervale_x.ptr_intervales_y[i+1];

                if(intervale_1.p2 < intervale_2.p1){

                    Intervale inter = Intervale(intervale_1, intervale_2);
                    intervales_fusionnees.push_back(inter);

                } else {

                    intervales_fusionnees.push_back(intervale_1);
                }

            }

            intervale_x.ptr_intervales_y = intervales_fusionnees;
        }
        //calculer l'aire
        long double aire = 0;

        for(const auto& intervale : intervales_x){

            for(int i = 0 ; i < intervale.ptr_intervales_y.size(); i++){

                double longueur = intervale.p2 - intervale.p1;
                double hauteur = intervale.ptr_intervales_y[i].p2 - intervale.ptr_intervales_y[i].p1;

                aire += (longueur * hauteur);
            }
        }


        return aire;

    }

    /**
     * Calcule le perimètre de tous les rectangles
     *
     * @return perimetre
     */
    double perimetre() {

        double perimetre = 0;

        return perimetre;
    }

    friend std::istream &operator>>(std::istream &, Grille &);

};

/**
 * Demande d'entrer un nom de fichier puis essaye de l'ouvrir.
 * Valide l'existence du fichier et l'extension ".txt".

 * @param fichier l'instance stream du fichier
 * @return fichier l'instance stream du fichier
 */
std::ifstream &validation_fichier(std::ifstream &fichier) {

    bool valide = false;

    while (!valide) {

        std::string repertoire;

        std::cout << "Entrez le nom du fichier : ";
        std::cin >> repertoire;

        fichier.open(repertoire.c_str());

        if (repertoire.length() < 5 || fichier.fail()) {

            std::cout << "Fichier spécifié invalide." << std::endl;
            fichier.close();

        } else if (repertoire.substr(repertoire.length() - 4, 4) != ".txt") {

            std::cout << "Le fichier doit terminer par \".txt\"" << std::endl;
            fichier.close();

        } else {

            valide = true;

        }

    }

    return fichier;

}

std::ostream &operator<<(std::ostream &os, Point &point) {

    os << "(" << std::setprecision(16) << point.x << "," << point.y << ")";

    return os;
}

std::ostream &operator<<(std::ostream &os, Rectangle &rectangle) {

    if (rectangle.estPositif()) {

        os << "Rectangle positif" << std::endl;

    } else {

        os << "Rectangle negatif" << std::endl;
    }

    os << "Supérieur gauche : " << std::setprecision(16) << rectangle.sup_gauche << std::endl;
    os << "Supérieur droite : " << std::setprecision(16) << rectangle.sup_droit << std::endl;
    os << "Inférieur gauche : " << std::setprecision(16) << rectangle.inf_gauche << std::endl;
    os << "Inférieur droit : " << std::setprecision(16) << rectangle.inf_droit << std::endl;

    return os;

}

/**
 * Insère les rectangles du fichier dans la grille
 *
 * @param is le fichier
 * @param grille l'instance de Grille
 * @return l'instance stream
 */
std::istream &operator>>(std::ifstream &is, Grille &grille) {

    while (!is.eof()) {

        char type;
        double x, y, longueur, hauteur;

        is >> type >> std::ws;
        is >> std::setprecision(16) >> x >> std::ws;
        is >> std::setprecision(16) >> y >> std::ws;
        is >> std::setprecision(16) >> longueur >> std::ws;
        is >> std::setprecision(16) >> hauteur >> std::ws;
        const Rectangle rectangle = Rectangle(type, x, y, longueur, hauteur, 1);

        std::cout << "X : " << x << " Y : " << y << " L : " << std::setprecision(16) << longueur << " H : "
                  << std::setprecision(16) << hauteur << std::endl;

        grille.ajouter(rectangle);
    }

    return is;
}

/**
* Exécution
**/
int main() {

    std::ifstream fichier;
    validation_fichier(fichier);

    Grille grille = Grille();
    fichier >> grille;
    fichier.close();

    for (int i = 0; i < grille.rectangles_positifs.size(); i++) {

        std::cout << grille.rectangles_positifs[i] << std::endl;
    }

    std::cout << "aire : " << std::setprecision(16) << grille.aire() << std::endl
              << "perimetre : " << std::setprecision(16) << grille.perimetre() << std::endl;

    return 0;

}