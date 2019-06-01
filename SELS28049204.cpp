#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>

/**
 * Représentation d'un rectangle
 */
class Rectangle {

    /**
     * Représente d'un type de rectangle positif ou négatif
     */
    enum class Type {

        positif, negatif

    };

    /**
     * Représentation d'un coin de rectangle sur les axes x et y
     */
    class Coin {

        double x, y;

    public:

        /**
         * Constructeur par défaut d'un coin, coordonnées (0,0)
         */
        Coin() : x(0), y(0) {};


        /**
         * Constructeur qui initialize les coordonnées du coin
         *
         * @param x la coordonnée x du coin
         * @param y la coordonnée y du coin
         */
        Coin(double x, double y) {

            this->x = x;
            this->y = y;

        }

        friend std::ostream &operator<<(std::ostream &, Coin &);

    };

public:

    Type type;
    Coin sup_gauche, sup_droit, inf_gauche, inf_droit;

    /**
     * Constructeur par défaut d'un rectangle, tous les coins à (0,0)
     */
    Rectangle() :
            sup_gauche(Coin()),
            sup_droit(Coin()),
            inf_gauche(Coin()),
            inf_droit(Coin()) {};

    /**
     * Constructeur qui initialize les 4 coins du rectangle
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
        this->sup_gauche = Coin(x - longueur / 2, y + hauteur / 2);
        this->sup_droit = Coin(x + longueur / 2, y + hauteur / 2);
        this->inf_gauche = Coin(x - longueur / 2, y - hauteur / 2);
        this->inf_droit = Coin(x + longueur / 2, y - hauteur / 2);

    }

    /**
     * Teste si le rectangle est positif ou negatif
     *
     * @return true si positif, false sinon
     */
    bool estPositif() {
        return this->type == Type::positif;
    }

    friend std::istream &operator>>(std::ifstream &, Rectangle &);

    friend std::ostream &operator<<(std::ostream &, Coin &);

    friend std::ostream &operator<<(std::ostream &, Rectangle &);


};

/**
 * Représente une grille avec tous les rectangles à l'intérieur
 */
class Grille {

public:

    std::vector <Rectangle> rectangles;

    /**
     * Ajoute le rectangle à la grille
     *
     * @param rectangle
     */
    void ajouter(Rectangle &rectangle) {

        this->rectangles.push_back(rectangle);
    }

    /**
     * Calcule l'aire de tous les rectangles
     *
     * @return aire
     */
    long double aire() {

        long double aire = 0;
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

        std::cout << "Veuillez entrer le nom du fichier." << std::endl;
        std::cin >> repertoire;

        fichier.open(repertoire.c_str());

        if (repertoire.length() < 5 || !fichier) {

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

std::ostream &operator<<(std::ostream &os, Rectangle::Coin &coin) {

    os << "(" << coin.x << "," << coin.y << ")";

    return os;
}

std::ostream &operator<<(std::ostream &os, Rectangle &rectangle) {

    if (rectangle.estPositif()) {

        os << "Rectangle positif" << std::endl;

    } else {

        os << "Rectangle negatif" << std::endl;
    }

    os << "Supérieur gauche : " << rectangle.sup_gauche << std::endl;
    os << "Supérieur droite : " << rectangle.sup_droit << std::endl;
    os << "Inférieur gauche : " << rectangle.inf_gauche << std::endl;
    os << "Inférieur droit : " << rectangle.inf_droit << std::endl;

    return os;

}

/**
 * Insère les caractéristiques lues dans le fichier dans l'instance de Rectangle
 *
 * @param is la ligne lue
 * @param rectangle l'instance de Rectangle
 * @return l'instance stream
 */
std::istream &operator>>(std::ifstream &is, Rectangle &rectangle) {

    if (is) {

        char type;
        double x, y, longueur, hauteur;

        is >> type >> std::ws;
        is >> x >> std::ws;
        is >> y >> std::ws;
        is >> longueur >> std::ws;
        is >> hauteur >> std::ws;
        rectangle = Rectangle(type, x, y, longueur, hauteur);

        std::cout << "X : " << x << " Y : " << y << " L : " << longueur << " H : " << hauteur << std::endl;

    }

    return is;
}

/**
 * Insère les rectangles lus dans le fichier dans la grille
 *
 * @param is le fichier
 * @param grille l'instance de Grille
 * @return l'instance stream
 */
std::istream &operator>>(std::ifstream &is, Grille &grille) {

    while (!is.eof()) {

        Rectangle rectangle;
        is >> rectangle;
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

    for (int i = 0; i < grille.rectangles.size(); i++) {
        std::cout << grille.rectangles[i] << std::endl;
    }

    std::cout << "aire : " << grille.aire() << std::endl
              << "perimetre : " << grille.perimetre() << std::endl;

}