#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <iomanip>
#include <algorithm>

/**
 * Représentation d'un type positif ou négatif
 */
enum class Type {

    positif, negatif

};

class IntervaleY {
public:
    double p1, p2;
    Type type;

    IntervaleY(Type type, double p1, double p2) : type(type), p1(p1), p2(p2) {
    }

    bool operator==(const IntervaleY &i2) {

        return this->p1 == i2.p1 && this->p2 == i2.p2;

    }

    friend bool operator<(const IntervaleY &i1, const IntervaleY &i2) {

        return i1.p2 < i2.p2;
    }

    friend bool operator>(const IntervaleY &i1, const IntervaleY &i2) {

        return i1.p2 > i2.p2;
    }

};

class IntervaleX {
public :
    double p1, p2;
    std::vector<IntervaleY> intervales_y;

    IntervaleX(double p1, double p2) : p1(p1), p2(p2) {
    }

    bool operator==(const IntervaleX &i2) {

        return this->p1 == i2.p1 && this->p2 == i2.p2;

    }

    friend bool operator<(const IntervaleX &i1, const IntervaleX &i2) {

        return i1.p2 < i2.p2;
    }

    friend bool operator>(const IntervaleX &i1, const IntervaleX &i2) {

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
    Rectangle(char type, double x, double y, double longueur, double hauteur) {

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
        return Rectangle('p', x_gauche, x_droit, x_droit - x_gauche, y_haut - y_bas);
    }

    friend std::ostream &operator<<(std::ostream &, Rectangle &);

};

/**
 * Représente une grille avec tous les rectangles à l'intérieur
 */
class Grille {

private:

    std::vector<IntervaleX> get_intervales_x() {

        std::vector<double> tous_les_x;

        for (auto &rectangle : this->rectangles) {

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

        // Sortir tous les X

        //Créer les intervales X
        std::vector<IntervaleX> intervales_x;
        for (int i = 0; i < tous_les_x.size() - 1; i++) {

            intervales_x.__emplace_back(IntervaleX(tous_les_x[i], tous_les_x[i + 1]));
        }

        return intervales_x;

    }

    std::vector<IntervaleY> get_intervales_y(IntervaleX intervale_x) {


        //Créer les intervales Y
        for (auto &rectangle : this->rectangles) {

            if (rectangle.inf_gauche.x <= intervale_x.p1 && rectangle.inf_droit.x >= intervale_x.p2) {
                //ce rectangle est dans cet intervale, mettre les intervales y1 et y2

                if (rectangle.type == Type::positif) {

                    const IntervaleY intervale_y = IntervaleY(Type::positif, rectangle.inf_gauche.y,
                                                              rectangle.sup_gauche.y);
                    intervale_x.intervales_y.push_back(intervale_y);

                } else {

                    const IntervaleY intervale_y = IntervaleY(Type::negatif, rectangle.inf_gauche.y,
                                                              rectangle.sup_gauche.y);
                    intervale_x.intervales_y.push_back(intervale_y);

                }


            }
        }

        std::sort(intervale_x.intervales_y.begin(), intervale_x.intervales_y.end());

        //fusionner les intervalles y qui se chevauchent ou qui sont identiques
        std::vector<IntervaleY> fusions_y;

        for (int i = intervale_x.intervales_y.size() - 1; i >= 0; i--) {

            double min = intervale_x.intervales_y[i].p1;
            double max = intervale_x.intervales_y[i].p2;

            int j = i - 1;

            while (j >= 0) {

                if (min <= intervale_x.intervales_y[j].p2 && min >= intervale_x.intervales_y[j].p1) {

                    min = intervale_x.intervales_y[j].p1;
                    i--;

                } else if (min <= intervale_x.intervales_y[j].p1 && max >= intervale_x.intervales_y[j].p2) {
                    i--;
                }

                j--;
            }

            IntervaleY int_y = IntervaleY(Type::positif, min, max);

            //vérifier que l'intervale fusionnée ne se trouve pas déjà dans la liste
            if (!(std::find(fusions_y.begin(), fusions_y.end(), int_y) != fusions_y.end())) {

                fusions_y.push_back(int_y);
            }

        }

        return fusions_y;

    }

public:

    std::vector<Rectangle> rectangles;

    void fractionner_rectangles() {

        std::vector<IntervaleX> intervales_x = get_intervales_x();

        for (auto &intervale_x : intervales_x) {
            intervale_x.intervales_y = get_intervales_y(intervale_x);
        }

        std::vector<Rectangle> rectangles_fractionnes;
        for (auto &intervale_x : intervales_x) {
            for (auto &intervale_y : intervale_x.intervales_y) {

                double longueur = intervale_x.p2 - intervale_x.p1;
                double hauteur = intervale_y.p2 - intervale_y.p1;
                double x = intervale_x.p1 + longueur / 2;
                double y = intervale_y.p1 + hauteur / 2;
                char type = intervale_y.type == Type::positif ? 'p' : 'n';
                rectangles_fractionnes.__emplace_back(Rectangle(type, x, y, longueur, hauteur));
            }
        }

        this->rectangles = rectangles_fractionnes;

    }

    /**
     * Calcule l'aire de tous les rectangles
     *
     * @return aire
     */
    long double aire() {

        fractionner_rectangles();
        //calculer l'aire
        long double aire = 0;

        for (const auto &rectangle : this->rectangles) {

            aire += (rectangle.sup_droit.y - rectangle.inf_droit.y) * (rectangle.inf_droit.x - rectangle.inf_gauche.x);
        }


        return aire;

    }

    /**
     * Calcule le perimètre de tous les rectangles
     *
     * @return perimetre
     */
    double perimetre() {

        fractionner_rectangles();


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
        const Rectangle rectangle = Rectangle(type, x, y, longueur, hauteur);

        std::cout << "X : " << x << " Y : " << y << " L : " << std::setprecision(16) << longueur << " H : "
                  << std::setprecision(16) << hauteur << std::endl;

        grille.rectangles.push_back(rectangle);
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

    for (int i = 0; i < grille.rectangles.size(); i++) {

        std::cout << grille.rectangles[i] << std::endl;
    }

    std::cout << "aire : " << std::setprecision(16) << grille.aire() << std::endl
              << "perimetre : " << std::setprecision(16) << grille.perimetre() << std::endl;

    return 0;

}