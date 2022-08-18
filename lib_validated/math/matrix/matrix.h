/*
créer une class matrice
utilisation de memoire dynamique
possibilité de redimensionner la matrice
opération standard comme la transposition, la trace, le determinant, 
l'inverse, multiplication par un scalaire, addition, soustraction, multiplication par une matrice ...
*/

/*
créé par : robinAZERTY
version du : 18/08/2022
*/

#pragma once

#include <string>
#include <math.h>

using namespace std;

class Matrix
{
public:
    // constructeurs et destructeur
    Matrix();                                                         // constructeur par defaut
    Matrix(const unsigned int &nb_rows, const unsigned int &nb_cols); // constructeur avec parametres de dimentions
    Matrix(const Matrix &m);                                          // constructeur par copie
    ~Matrix();                                                        // destructeur

    // méthodes de redimensionnements
    virtual void resize(const int &nb_new_rows, const int &nb_new_cols); // redimensionne la matrice
    // void push_cols(const double **new_cols, const int &at_col_n);//ajoute des colonnes à la matrice
    // void push_rows(const double **new_rows, const int &at_row_n);//ajoute des lignes à la matrice

    // méthodes d'accès
    int get_nb_rows() const; // retourne le nombre de lignes de la matrice
    int get_nb_cols() const; // retourne le nombre de colonnes de la matrice
    double **data() const;   // retourne le tableau de données de la matrice
    /*
   //double data(const int& row, const int& col) const;//retourne la valeur de la matrice à la ligne row et à la colonne col
    /*
    //méthodes d'affectation
    */

    virtual void set(const unsigned int &row, const unsigned int &col, const double &data); // affecte une valeur à une case de la matrice
    void fill(const double &data);                                                  // remplit la matrice avec la valeur data
    // méthodes de calcul
    Matrix transpose() const;   //
    Matrix inverse() const;     //
    double determinant() const; //
    double trace() const;       //

    // operateurs
    Matrix operator+(const Matrix &m) const; //
    Matrix operator-(const Matrix &m) const; //
    Matrix operator*(const Matrix &m) const; //
    Matrix operator*(const double &d) const; //
    Matrix operator/(const double &d) const; //
    // access operator
    double &operator()(const unsigned int &row, const unsigned int &col) const; //

    Matrix &operator=(const Matrix &m);     //
    Matrix operator+=(const Matrix &m);     //
    Matrix operator-=(const Matrix &m);     //
    Matrix operator*=(const Matrix &m);     //
    Matrix operator*=(const double &d);     //
    Matrix operator/=(const double &d);     //
    bool operator==(const Matrix &m) const; //
    bool operator!=(const Matrix &m) const; //

    // méthode de construction de message pour affichage
    void set_description(const string description); // affecte la description de la matrice
    string to_str() const;

protected:
    string description = "Matrice";
    unsigned int rows; //de 0 à 4 294 967 295
    unsigned int cols; //de 0 à 4 294 967 295
    double **Data;// donc 4294967295² = 18 446 744 065 119 617 025 au max
};

Matrix operator*(const double &d, Matrix const &m) { return m * d; }; // la multiplication par un scalaire est symétrique