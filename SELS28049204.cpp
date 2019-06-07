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

/**
 * Représentation d'une intervale Y
 */
class IntervaleY {
public:
    double p1, p2;
    Type type;

    /**
     * Constructeur d'une intervale Y.
     *
     * @param type le type positif ou negatif
     * @param p1 la borne inférieure
     * @param p2 la borne supérieure
     */
    IntervaleY(Type type, double p1, double p2) : type(type), p1(p1), p2(p2) {
    }

    /**
     * Teste si les bornes de l'instance sont identiques à celles de de l'intervale i2.
     *
     * @param i2 instance de Intervale
     * @return True si égal, False sinon
     */
    bool operator==(const IntervaleY &i2) {

        return this->p1 == i2.p1 && this->p2 == i2.p2;

    }

    /**
     * Teste si la borne supérieure de l'intervale Y gauche est inférieure à celle de l'intervale Y droite.
     *
     * @param i1
     * @param i2
     * @return True si inférieure, False sinon
     */
    friend bool operator<(const IntervaleY &i1, const IntervaleY &i2) {

        return i1.p2 < i2.p2;
    }

    /**
     * Teste si la borne supérieure de l'intervale Y gauche est supérieure à celle de l'intervale Y droite
     *
     * @param i1
     * @param i2
     * @return True si supérieure, False sinon
     */
    friend bool operator>(const IntervaleY &i1, const IntervaleY &i2) {

        return i1.p2 > i2.p2;
    }

};

/**
 * Représente une intervale X
 */
class IntervaleX {
public :
    double p1, p2;
    std::vector<IntervaleY> intervales_y;

    /**
     * Constructeur d'une intervale X
     *
     * @param p1 la borne à gauche
     * @param p2 la borne à droite
     */
    IntervaleX(double p1, double p2) : p1(p1), p2(p2) {
    }

    /**
     * Teste si les bornes de l'intervale sont identiques à celles de l'intervale i2.
     *
     * @param i2 instance d'IntervaleX
     * @return Vrai si identique, False sinon
     */
    bool operator==(const IntervaleX &i2) {

        return this->p1 == i2.p1 && this->p2 == i2.p2;

    }

    /**
     * Teste si la borne droite de l'intervaleX i1 est inférieure à l'intervale droite de l'intervaleX i2
     *
     * @param i1 intervaleX
     * @param i2 intervaleX
     * @return Vrai si inférieure, Faux sinon
     */
    friend bool operator<(const IntervaleX &i1, const IntervaleX &i2) {

        return i1.p2 < i2.p2;
    }

    /**
     * Teste si la borne droite de l'intervaleX i1 est supérieure à l'intervale droite de l'intervaleX i2
     *
     * @param i1 intervaleX
     * @param i2 intervaleX
     * @return Vrai si supérieure, Faux sinon
     */
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
     * Teste si le Rectangle rec2 croise l'instance
     *
     * @param rec2 Rectangle
     *
     * @return Vrai si croise, Faux sinon
     */
    bool croise(const Rectangle &rec2) {

        return
                this->sup_gauche.x < rec2.inf_droit.x &&
                this->inf_droit.x > rec2.sup_gauche.x &&
                this->sup_gauche.y > rec2.inf_droit.y &&
                this->inf_droit.y < rec2.sup_gauche.y;
    }

    /**
     * Retourne un rectangle qui représente le croisement du rectangle actuel et rec2
     *
     * @param rec2 Rectangle
     * @return Rectangle qui représente le croisement
     */
    Rectangle croisement(const Rectangle &rec2) {
        double x_gauche = std::max(this->inf_gauche.x, rec2.inf_gauche.x);
        double x_droit = std::min(this->sup_droit.x, rec2.sup_droit.x);
        double y_haut = std::min(this->sup_gauche.y, rec2.sup_gauche.y);
        double y_bas = std::max(this->inf_droit.y, rec2.inf_droit.y);
        return Rectangle('p', x_gauche, x_droit, x_droit - x_gauche, y_haut - y_bas);
    }

};

/**
 * Représente une grille avec tous les rectangles à l'intérieur
 */
class Grille {

private:

    /**
     * Génère des intervales avec tous les points X d'une liste de rectangles.
     *
     * @param rectangles liste de rectangles
     * @return liste d'intervaleX
     */
    static std::vector<IntervaleX> get_intervales_x(std::vector<Rectangle> &rectangles) {

        std::vector<double> tous_les_x;

        for (auto &rectangle : rectangles) {

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

    /**
     * Génére des intervales avec tous les points Y d'une liste de rectangles, à l'intérieurs des intervaleX données.
     *
     * @param intervale_x liste d'intervaleX
     * @param rectangles liste de Rectangles
     * @return liste d'intervaleY
     */
    static std::vector<IntervaleY> get_intervales_y(IntervaleX intervale_x, std::vector<Rectangle> &rectangles) {


        //Créer les intervales Y
        for (auto &rectangle : rectangles) {

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

            Type type_i = intervale_x.intervales_y[i].type;
            double min_i = intervale_x.intervales_y[i].p1;
            double max_i = intervale_x.intervales_y[i].p2;

            int j = i - 1;

            while (j >= 0) {

                Type type_j = intervale_x.intervales_y[j].type;
                double min_j = intervale_x.intervales_y[j].p1;
                double max_j = intervale_x.intervales_y[j].p2;


                if (min_i <= max_j && min_i >= min_j) {

                    min_i = min_j;
                    i--;

                } else if (min_i <= min_j && max_i >= max_j) {
                    i--;
                }

                j--;
            }

            IntervaleY int_y = IntervaleY(intervale_x.intervales_y[i].type, min_i, max_i);

            //vérifier que l'intervale fusionnée ne se trouve pas déjà dans la liste
            if (!(std::find(fusions_y.begin(), fusions_y.end(), int_y) != fusions_y.end())) {

                fusions_y.push_back(int_y);
            }

        }

        return fusions_y;

    }

public:

    std::vector<Rectangle> rectangles_positifs;
    std::vector<Rectangle> rectangles_negatifs;

    /**
     * Fractionne les rectangles de la grille selon des intervales X et Y.
     */
    void fractionner_rectangles() {

        //fractionner les rectangles positifs
        if (!rectangles_positifs.empty()) {

            std::vector<IntervaleX> intervales_x_positifs = get_intervales_x(this->rectangles_positifs);
            for (auto &intervale_x : intervales_x_positifs) {
                intervale_x.intervales_y = get_intervales_y(intervale_x, this->rectangles_positifs);
            }

            std::vector<Rectangle> rectangles_fractionnes_positifs;
            for (auto &intervale_x : intervales_x_positifs) {
                for (auto &intervale_y : intervale_x.intervales_y) {

                    double longueur = intervale_x.p2 - intervale_x.p1;
                    double hauteur = intervale_y.p2 - intervale_y.p1;
                    double x = intervale_x.p1 + longueur / 2;
                    double y = intervale_y.p1 + hauteur / 2;
                    char type = intervale_y.type == Type::positif ? 'p' : 'n';
                    rectangles_fractionnes_positifs.__emplace_back(Rectangle(type, x, y, longueur, hauteur));
                }
            }

            this->rectangles_positifs = rectangles_fractionnes_positifs;

        }

        //fractionner les rectangles negatifs
        if (!rectangles_negatifs.empty()) {

            std::vector<IntervaleX> intervales_x_negatifs = get_intervales_x(this->rectangles_negatifs);
            for (auto &intervale_x : intervales_x_negatifs) {
                intervale_x.intervales_y = get_intervales_y(intervale_x, this->rectangles_negatifs);
            }

            std::vector<Rectangle> rectangles_fractionnes_negatifs;
            for (auto &intervale_x : intervales_x_negatifs) {
                for (auto &intervale_y : intervale_x.intervales_y) {

                    double longueur = intervale_x.p2 - intervale_x.p1;
                    double hauteur = intervale_y.p2 - intervale_y.p1;
                    double x = intervale_x.p1 + longueur / 2;
                    double y = intervale_y.p1 + hauteur / 2;
                    char type = intervale_y.type == Type::positif ? 'p' : 'n';
                    rectangles_fractionnes_negatifs.__emplace_back(Rectangle(type, x, y, longueur, hauteur));
                }
            }

            this->rectangles_negatifs = rectangles_fractionnes_negatifs;

        }

    }

    /**
     * Retourne l'aire de la forme sur la grille
     *
     * @return aire
     */
    long double aire() {

        fractionner_rectangles();
        long double aire = 0;

        for (auto &rectangle_p : this->rectangles_positifs) {

            aire += (rectangle_p.sup_droit.y - rectangle_p.inf_droit.y) *
                    (rectangle_p.inf_droit.x - rectangle_p.inf_gauche.x);

            for (auto &rectangles_n : this->rectangles_negatifs) {

                if (rectangle_p.croise(rectangles_n)) {

                    Rectangle intersection = rectangle_p.croisement(rectangles_n);
                    aire -= (intersection.sup_droit.y - intersection.inf_droit.y) *
                            (intersection.inf_droit.x - intersection.inf_gauche.x);
                }
            }
        }

        return aire;

    }

    /**
     * Retourne le perimètre de la forme sur la grille
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

        if (rectangle.type == Type::positif) {

            grille.rectangles_positifs.push_back(rectangle);

        } else {

            grille.rectangles_negatifs.push_back(rectangle);
        }
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

    std::cout << "aire : " << std::setprecision(16) << grille.aire() << std::endl
              << "perimetre : " << std::setprecision(16) << grille.perimetre() << std::endl;

    return 0;

}