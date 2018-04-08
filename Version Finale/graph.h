#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont propos�es 3 classes fondamentales
            Sommet (=Sommet)
            Arc (=Ar�te ou Arc)
            Graph (=Graphe)

    Les ar�tes et les sommets et le graphe qu'ils constituent
    "travaillent" �troitement ensemble : pour cette raison les
    Sommet et Arc se d�clarent amis (friend) de Graph pour que
    ce dernier puisse librement acc�der aux membres (y compris
    protected ou private) de Sommet et Arc.

    Ces Classes peuvent �tres compl�t�es. Il est �galement possible
    de les d�river mais il est malheureusement assez difficile
    de d�river le "triplet" des 3 classes en maintenant des relations
    coh�rentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute pr�f�rable, si possible,
    de "customiser" ces classes de base directement, sans h�ritage.

    Le mod�le propos� permet de repr�senter un graphe orient� �ventuellement
    pond�r�, les arcs portent une ou des informations suppl�mentaire(s).
    Les relations/navigations Arcs -> m_Sommets et m_Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse �tre assez facilement
    lue et �crite en fichier, et b�n�ficie d'une bonne lisibilit� en cas de bugs...

    Chaque arc poss�de 2 attributs principaux (en plus de son �ventuelle pond�ration)
        -> m_from (indice du sommet de d�part de l'arc )
        -> m_to (indice du sommet d'arriv�e de l'arc )

    Chaque sommet poss�de 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs)
        -> m_out (liste des indices des arcs partant du sommet : acc�s aux successeurs)

    Cependant le probl�me des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entit� (un arc et/ou un sommet sont enlev�s du graphe) alors :

    - Soit il faut reprendre toute la num�rotation pour "boucher le trou"
      (par exemple on a supprim� le sommet n�4, le sommet n�5 devient le 4, 6 devient 5 etc...)
      ce qui pose des probl�mes de stabilit� et de coh�rence, et une difficult� � r�-introduire
      le(s) m�me(s) �l�ment supprim� (au m�me indice)

    - Soit on admet que la num�rotation des sommets et arcs n'est pas contigue, c�d qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La num�rotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet � des indices arbitraires (pas forc�ment contigus)

    C'est cette 2�me approche qui est propos�e ici : dans la classe graphe vous trouverez
        -> map<int, Arc>   m_arcs
        -> map<int, Sommet> m_Sommets    (le pluriel de vertex est vertices)

    Il faudra �tre attentif au fait que par rapport � un simple vecteur, le parcours des �l�ments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_arcs.size(); ++i) ...m_arcs[i]...
    et que les parcours � it�rateur ne donneront pas directement des Arc ou des Sommet
    mais des pairs, l'objet d'int�r�t se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_arcs.begin(); it!=m_arcs.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_arcs) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associ�e � chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non repr�sent�s � l'�cran
    Imaginons par exemple qu'on doive g�n�rer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas repr�senter graphiquement ces 1000 graphes, et les
    interfaces p�sent lourd en ressource, avec cette organisation on est libre de r�server ou
    pas une interface de pr�sentation associ�e aux datas (d�couplage donn�es/interface)

***********************************************************************************************/

/*!
 * \file graphe.h
 * \brief  classes : graphe, arc et sommet
 * \version 400
 * code ecrit a partir du code squelette de M.Fercoq et agrementer de nos fonctions
 *
 */
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <string>
#include <stdlib.h>
#include<sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

#include "grman/grman.h"

/***************************************************
                    SOMMET
****************************************************/
/*! \class SommetInterface
 * \brief classe representant les sommets graphe
 *
 *  La classe gere la visulalisation des sommets du graphe
 */
class SommetInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Sommet;
    friend class ArcInterface;
    friend class Graph;

    private :

        /*! Les widgets de l'interface.*/

        grman::WidgetBox m_top_box; /*!< La boite qui contient toute l'interface d'un sommet*/

        grman::WidgetVSlider m_slider_value; /*!< Un slider de visualisation/modification de la valeur du sommet*/

        grman::WidgetText m_label_value; /*!< Un label de visualisation de la valeur du sommet*/

        grman::WidgetImage m_img; /*!< Une image de "remplissage"*/

        grman::WidgetText m_label_idx; /*!<  Un label indiquant l'index du sommet*/

        grman::WidgetText m_box_label_idx; /*!<  Une boite pour le label pr�c�dent*/

    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        SommetInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
};

/*! \class Sommet
 * \brief classe gerant les sommets du graphe
 *
 *  La classe gere l'ensemble des sommets du graphe
 */
class Sommet
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class SommetInterface;
    friend class Arc;
    friend class ArcInterface;

    private :
        ///
        std::vector<int> m_in; /*!< liste des indices des arcs arrivant au sommet : acc�s aux pr�d�cesseurs*/

        ///
        std::vector<int> m_out; /*!< liste des indices des arcs partant du sommet : acc�s aux successeurs*/

        /// un exemple de donn�e associ�e � l'arc, on peut en ajouter d'autres...
        double m_value; /*!< valeur du sommet*/

        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<SommetInterface> m_interface = nullptr;

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne pr�c�dente est en gros �quivalent � la ligne suivante :
        // SommetInterface * m_interface = nullptr;

        std::string m_nom;  /*!< nom du sommet*/
        int m_coordx;  /*!< coordonnees en x*/
        int m_coordy;  /*!< coordonnees en y*/
        bool is_marq = false;  /*!< marquage*/
        int m_color = 0; /*!< couleur de la boite*/

                ///Ressources

        double m_coeff_reproduction=0.1;
        double m_croissance_basic=1;


        /// Forte connexite
        bool m_actif; /*!< Si le sommet est actif*/
        int m_decouverte; /*!< entier de comparaison*/
        int m_idx_T; /*!< entier de comparaison*/
        int m_index; /*!< index du sommet*/
        int m_etat; /*!< Etat du sommet :0 si jamais visite, 1 si en traitetement et 2 si fini*/
        bool m_premier = false; /*!< Si le sommet est le premier de la compo connexe*/







    public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Sommet (double value=0, SommetInterface *interface=nullptr) :
            m_value(value), m_interface(interface)  {  }
        Sommet(double value, std::string nom) :
            m_value(value), m_nom(nom) {  }


        /// Sommet �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
        /// le pre_update et post_update de Sommet (pas directement la boucle de jeu)
        /// Voir l'impl�mentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};



/***************************************************
                    ARC
****************************************************/
/*! \class ArcInterface
 * \brief classe representant les arcs graphe
 *
 *  La classe gere la visulalisation des arcs du graphe
 */
class ArcInterface
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Arc;
    friend class Graph;

    private :

        /*! Les widgets de l'interface.*/


        grman::WidgetEdge m_top_edge; /*!< Le WidgetEdge qui "contient" toute l'interface d'un arc*/


        grman::WidgetBox m_box_edge; /*!< Une boite pour englober les widgets de r�glage associ�s*/

        grman::WidgetVSlider m_slider_weight; /*!< Un slider de visualisation/modification du poids valeur de l'arc*/


        grman::WidgetText m_label_weight; /*!<  Un label de visualisation du poids de l'arc*/

        grman::WidgetBox m_box_index; /*!< une boite pour afficher l'index de l'arete*/
        //label de visualisation de l'index
        grman::WidgetText m_label_index;


    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
        ArcInterface(Sommet& from, Sommet& to);
};

/*! \class Arc
 * \brief classe gerant les arcs du graphe
 *
 *  La classe gere l'ensemble des arcs du graphe
 */
class Arc
{
    // Les (methodes des) classes amies pourront acc�der
    // directement aux attributs (y compris priv�s)
    friend class Graph;
    friend class ArcInterface;

    private :
        int m_from; /*!< indice du sommet de depart de l'arc*/

        int m_to; /*!< indice du sommet d'arrivee de l'arc*/

        double m_weight; /*!< poids de l'arc*/

        int m_index; /*!< Index de l'arc*/

        bool m_marq = false; /*!< Marquage de l'arc*/

        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<ArcInterface> m_interface = nullptr; /*!POINTEUR sur l'interface associ�e*/


    public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Arc (double weight=0, ArcInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }



        /// Arc �tant g�r� par Graph ce sera la m�thode update de graph qui appellera
        /// le pre_update et post_update de Arc (pas directement la boucle de jeu)
        /// Voir l'impl�mentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};




/***************************************************
                    GRAPH
****************************************************/
/*! \class GrapheInterface
 * \brief classe representant le graphe
 *
 *  La classe gere la visulalisation du graphe
 */
class GraphInterface
{
    friend class Graph;

    private :

        /*! Les widgets de l'interface.*/


        grman::WidgetBox m_top_box; /*!< La boite qui contient toute l'interface d'un graphe*/

        grman::WidgetBox m_main_box; /*!< Dans cette boite seront ajout�s les (interfaces des) sommets et des arcs...*/

        grman::WidgetBox m_tool_box; /*!< Dans cette boite seront ajout�s des boutons de contr�le etc....*/

        grman::WidgetButton m_bouton_sauv;      /*!< Bouton pour sauvegarer le graphe*/
        grman::WidgetText m_bouton_label_sauv;

        grman::WidgetButton m_bouton_charge;     /*!< Bouton pour charger un graphe*/
        grman::WidgetText m_bouton_label_charge;

        grman::WidgetButton m_bouton_ajout_sommet;       /*!< Bouton pour ajouter un sommet*/
        grman::WidgetText m_bouton_label_ajout_sommet;

        grman::WidgetButton m_bouton_ajout_arc;       /*!< Bouton pour ajouter un arc*/
        grman::WidgetText m_bouton_label_ajout_arc;

        grman::WidgetButton m_bouton_suppr_s;       /*!< Bouton pour supprimer un sommet*/
        grman::WidgetText m_bouton_label_suppr_s;

        grman::WidgetButton m_bouton_suppr_a;       /*!< Bouton pour supprimer un arc*/
        grman::WidgetText m_bouton_label_suppr_a;

        grman::WidgetButton m_bouton_quit;       /*!< Bouton pour quitter l'application*/
        grman::WidgetText m_bouton_label_quit;

        grman::WidgetButton m_bouton_connexe; /*!< Bouton pour chercher les composantes fortement connexe*/
        grman::WidgetText m_bouton_label_connexe;

        grman::WidgetButton m_bouton_retirer; /*!< Bouton pour retirer les marquage de l'interface*/
        grman::WidgetText m_bouton_label_retirer;

        grman::WidgetButton m_bouton_etude; /*!< Bouton pour lancer l'�tude temporelle1*/
        grman::WidgetText m_bouton_label_etude;
        grman::WidgetButton m_bouton_etude2; /*!< Bouton pour lancer l'�tude temporelle2*/
        grman::WidgetText m_bouton_label_etude2;

        grman::WidgetButton m_bouton_arret; /*!< Bouton pour arreter l'etude temporelle*/
        grman::WidgetText m_bouton_label_arret;

        grman::WidgetButton m_bouton_graphe_red; /*!< Bouton pour afficher le graphe reduit*/
        grman::WidgetText m_bouton_label_graphe_red;

        grman::WidgetCheckBox m_checkbox_debeug; /*!< Bouton pour debug l'etude temporelle : pas a pas*/
        grman::WidgetText m_checkbox_label_debeug;




    public :

        // Le constructeur met en place les �l�ments de l'interface
        // voir l'impl�mentation dans le .cpp
    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe grapheinterface
     *
     *  \param positions
     */
    GraphInterface(int x, int y, int w, int h);
};


/*! \class Graphe
 * \brief classe gerant le graphe
 *
 *  La classe gere le graphe et toutes es fonctiosn associ�es
 */
class Graph
{

public :

        std::map<int, Arc> m_arcs; /*!< Liste des aretes du graphe*/

        std::map<int, Sommet> m_Sommets; /*!< Liste des sommets du graphe*/

        /// le POINTEUR sur l'interface associ�e, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr; /*!POINTEUR sur l'interface associ�e*/

        int m_ordre; /*!< ordre du graphe*/
        int m_nb_arete; /*!< nombre d'aret du graphe*/

        /*!Attribut forte connexite*/
        std::stack<Sommet> m_parcours ;/*!< pile de parcours du graphe -- forte connexite*/
        int m_i = 0; /*!< compteur -- forte connexite*/
        std::stack<Sommet> m_pile_tarjan; /*!< pile de successeurs -- forte connexite*/
        int m_nb_forte_connexe = 0; /*!< compteur du nombre de forte connexite -- forte connexite*/




    //public:

        /// Les constructeurs sont � compl�ter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        /*!
        *  \brief ajout d'un sommet
        *
        *  Ajoute un sommet dans l'interface graphique et dans notre map de sommets
        */
        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0, double coef_repro = 0, double coef_basic=0 );
        /*!
        *  \brief ajout d'un sommet manuellement
        *
        *  Ajoute un sommet dans l'interface graphique et dans notre map de sommets a partir de l'interface
        */
        void add_interfaced_Sommet(int idx, double value, int x, int y, std::string path, int pic_idx, double coeef_repro=0, double coef_basic=0 );

        /*!
        *  \brief ajout d'un arc
        *
        *  Ajoute un sommet dans l'interface graphique et dans notre map d'arcs
        */
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);
        /*!
        *  \brief ajout d'un arc manuellement
        *
        *  Ajoute un arc dans l'interface graphique et dans notre map d'arc
        */
        void add_interfaced_Arc(int idx, int id_vert1, int id_vert2, double weight);

        /// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
        /// Voir impl�mentation dans le .cpp
        /// Cette m�thode est � enlever et remplacer par un syst�me
        /// de chargement de fichiers par exemple.
        void make_example();
        /*!
        *  \brief  si l'utilisateur veut charger un graphe
        *
        *  sert a savoir si l'utilisateur veut charger un graphe
        */
        void is_charge();

        /*!
        *  \brief charge un graphe
        *
        *  charge un graphe a partir d'un fichier texte
        */
        void charge_file(bool charge);
        /*!
        *  \brief sauvegarde un graphe
        *
        *  sauvegarde un graphe dans un fichier texte
        */
        void sauvegarde_graph();
        /*!
        *  \brief quitte l'application
        *
        *  quitte l'application si l'utilisateur le demande
        */
        bool quitter();
        /*!
        *  \brief supprime un arc
        *
        *  supprime un arc
        */
        void suppr_arc(int idx);
        /*!
        *  \brief supprime un sommet
        *
        *  supprime un sommet et les arcs qui lui sont rattaches
        */
        void suppr_sommet();
        //bool suppr_sommet();
        /*!
        *  \brief cherche les composantes fortement connexe du graphe
        */
        void cherche_connexe();

        /*!
        *  \brief cherche l'indice  le plus grand parmi tous les sommets
        */
        int cherche_idx_s();
        /*!
        *  \brief cherche l'indice  le plus grand parmi tous les arcs
        */
        int cherche_idx_a();
        /*!
        *  \brief sremet tous les marquage de l'interface a zero
        */
        void remise_zero();
        /*!
        *  \brief Etude temporelle
        *
        *  Etude temporelle de l'ecosysteme
        */
        int simu(int tours_de_boucle);
        /*!
        *  \brief Etude temporelle
        *
        *  Etude temporelle de l'ecosysteme
        */
        int simu2( int tours_de_boucle);
        /*!
        *  \brief Arret de l'etude temporelle
        */
        void arret_etude();

        /*!
        *  \brief cherche les composantes fortement connexe d'un graphe
        */
        void cherche_forte_connexe();
        /*!
        *  \brief fonction de parcours d'un sommet pour la forte connexite
        */
        void fonction4(int idx_u);
        /*!
        *  \brief affiche les attributs des sommets et l'etat des piles
        */
        void affiche_tab(); //on affiche les tableaux
        /*!
        *  \brief Initialise les attributs avant de passer dans la recherche de forte connexite
        */
        void Init_forte_connexe();
        /*!
        *  \brief Affiche le graphe reduit
        */
        void affiche_graphe_reduit();
        /*!
        *  \brief Permet de sauvegarder le graphe en cours
        */
        void sauvegarde_graph_2();





        /*!
        *  \brief MAJ de l'interface
        *
        *  La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
        */
        void update();
};


#endif // GRAPH_H_INCLUDED
