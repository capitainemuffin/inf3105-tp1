#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <iomanip>

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
    static long double aire(std::vector<Rectangle> rectangles) {

        long double aire = 0;
        std::cout << rectangles.size() << std::endl;
        if (rectangles.size() > 1) {

            std::vector<Rectangle> sous_rectangles;

            for (int i = 0; i < rectangles.size(); i++) {

                Rectangle rec1 = rectangles[i];

                for (int j = i + 1; j < rectangles.size(); j++) {

                    Rectangle rec2 = rectangles[j];

                    if (rec1.croise(rec2)) {
                        std::cout << "croisement" << std::endl;
                        const Rectangle croisement = rec1.croisement(rec2);
                        sous_rectangles.push_back(croisement);

                    }
                }

            }

            if(sous_rectangles[0].niveau %2 != 0){

                aire += Grille::aire(sous_rectangles);

            } else {

                aire -= Grille::aire(sous_rectangles);

            }

        } else {

            if(rectangles[0].niveau %2 != 0){

                aire += Grille::aire(rectangles);

            } else {

                aire -= Grille::aire(rectangles);

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

        for (int i = 0; i < rectangles_positifs.size(); i++) {

            long double perimetre_a_soustraire = 0;
            Rectangle rec1 = rectangles_positifs[i];

            for (int j = i + 1; j < rectangles_positifs.size(); j++) {

                Rectangle rec2 = rectangles_positifs[j];

                if (rec1.croise(rec2)) {

                    const Rectangle croisement = rec1.croisement(rec2);
                    perimetre_a_soustraire += (croisement.longueur * 2) + (croisement.hauteur * 2);

                }
            }

            perimetre += (rec1.longueur * 2) + (rec1.hauteur * 2) - perimetre_a_soustraire;
        }

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

    std::cout << "aire : " << std::setprecision(16) << grille.aire(grille.rectangles_positifs) << std::endl
              << "perimetre : " << std::setprecision(16) << grille.perimetre() << std::endl;

    return 0;

}