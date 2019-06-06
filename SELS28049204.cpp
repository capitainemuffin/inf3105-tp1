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
    std::vector<Intervale> intervales_y;

    Intervale(double p1, double p2) : p1(p1), p2(p2) {
    }

    bool operator==(const Intervale &i2) {

        return this->p1 == i2.p1 && this->p2 == i2.p2;

    }

    friend bool operator<(const Intervale& i1, const Intervale &i2){

        return i1.p2 < i2.p2;
    }

    friend bool operator>(const Intervale& i1, const Intervale &i2){

        return i1.p2 > i2.p2;
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
    Point sup_gauche, sup_droit, inf_gauche, inf_droit;

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

    }

    /**
     * Teste si le rectangle est positif ou negatif
     *
     * @return true si positif, false sinon
     */
    bool estPositif() const {
        return this->type == Type::positif;
    }

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

        } else {

            this->rectangles_negatifs.push_back(rectangle);

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
        for (auto &rectangle : this->rectangles_positifs) {

            double temp = rectangle.inf_gauche.x;

            if (!(std::find(tous_les_x.begin(), tous_les_x.end(), temp) != tous_les_x.end())) {

                tous_les_x.push_back(temp);
            }

            temp = rectangle.inf_droit.x;

            if (!(std::find(tous_les_x.begin(), tous_les_x.end(), temp) != tous_les_x.end())) {

                tous_les_x.push_back(temp);
            }
        }

        std::sort(tous_les_x.begin(), tous_les_x.end());

        for (int i = 0; i < tous_les_x.size() - 1; i++) {

            intervales_x.__emplace_back(Intervale(tous_les_x[i], tous_les_x[i + 1]));
        }


        //calculer les intervales y
        for (auto &intervale_x : intervales_x) {

            for (auto &rectangle : this->rectangles_positifs) {

                if (rectangle.inf_gauche.x <= intervale_x.p1 && rectangle.inf_droit.x >= intervale_x.p2) {
                    //ce rectangle est dans cet intervale, mettre les intervales y1 et y2
                    const Intervale intervale_y = Intervale(rectangle.inf_gauche.y, rectangle.sup_gauche.y);

                    intervale_x.intervales_y.push_back(intervale_y);

                }
            }

            std::sort(intervale_x.intervales_y.begin(), intervale_x.intervales_y.end());
        }

        //fusionner les intervalles y qui se chevauchent ou qui sont identiques
        for (auto &intervale_x : intervales_x) {

            std::vector<Intervale> fusions_y;

            for (int i = intervale_x.intervales_y.size() - 1; i >= 0; i--) {

                double min = intervale_x.intervales_y[i].p1;
                double max = intervale_x.intervales_y[i].p2;

                int j = i - 1;

                while (j >= 0) {

                    if (min <= intervale_x.intervales_y[j].p2 && min >= intervale_x.intervales_y[j].p1) {

                        min = intervale_x.intervales_y[j].p1;
                        i--;

                    } else if (min <= intervale_x.intervales_y[j].p1 && max >= intervale_x.intervales_y[j].p2){
                        i --;
                    }

                    j--;
                }

                Intervale int_y = Intervale(min, max);

                //vérifier que l'intervale fusionnée ne se trouve pas déjà dans la liste
                if (!(std::find(fusions_y.begin(), fusions_y.end(), int_y) != fusions_y.end())) {

                    fusions_y.push_back(int_y);
                }

            }

                intervale_x.intervales_y = fusions_y;

        }


        //calculer l'aire
        long double aire = 0;

        for (const auto &intervale : intervales_x) {

            for (int i = 0; i < intervale.intervales_y.size(); i++) {

                double longueur = intervale.p2 - intervale.p1;
                double hauteur = intervale.intervales_y[i].p2 - intervale.intervales_y[i].p1;

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