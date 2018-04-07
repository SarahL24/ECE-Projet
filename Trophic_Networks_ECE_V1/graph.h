#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

/**************************************************************
    Ici sont proposées 3 classes fondamentales
            Sommet (=Sommet)
            Arc (=Arête ou Arc)
            Graph (=Graphe)

    Les arêtes et les sommets et le graphe qu'ils constituent
    "travaillent" étroitement ensemble : pour cette raison les
    Sommet et Arc se déclarent amis (friend) de Graph pour que
    ce dernier puisse librement accéder aux membres (y compris
    protected ou private) de Sommet et Arc.

    Ces Classes peuvent êtres complétées. Il est également possible
    de les dériver mais il est malheureusement assez difficile
    de dériver le "triplet" des 3 classes en maintenant des relations
    cohérentes ( rechercher "c++ class covariance" et "c++ parallel inheritance"
    pour commencer .. ). Il est donc sans doute préférable, si possible,
    de "customiser" ces classes de base directement, sans héritage.

    Le modèle proposé permet de représenter un graphe orienté éventuellement
    pondéré, les arcs portent une ou des informations supplémentaire(s).
    Les relations/navigations Arcs -> m_Sommets et m_Sommets -> Arcs se font
    dans les 2 sens et utilisent des INDICES et NON PAS DES ADRESSES (pointeurs)
    de telle sorte que la topologie du graphe puisse être assez facilement
    lue et écrite en fichier, et bénéficie d'une bonne lisibilité en cas de bugs...

    Chaque arc possède 2 attributs principaux (en plus de son éventuelle pondération)
        -> m_from (indice du sommet de départ de l'arc )
        -> m_to (indice du sommet d'arrivée de l'arc )

    Chaque sommet possède 2 liste d'arcs (en plus de ses attributs "internes", marquages...)
        -> m_in (liste des indices des arcs arrivant au sommet : accès aux prédécesseurs)
        -> m_out (liste des indices des arcs partant du sommet : accès aux successeurs)

    Cependant le problème des indices (par rapport aux pointeurs) et qu'en cas
    de destruction d'une entité (un arc et/ou un sommet sont enlevés du graphe) alors :

    - Soit il faut reprendre toute la numérotation pour "boucher le trou"
      (par exemple on a supprimé le sommet n°4, le sommet n°5 devient le 4, 6 devient 5 etc...)
      ce qui pose des problèmes de stabilité et de cohérence, et une difficulté à ré-introduire
      le(s) même(s) élément supprimé (au même indice)

    - Soit on admet que la numérotation des sommets et arcs n'est pas contigue, càd qu'il
      peut y avoir des "trous" : sommets 0 1 5 7 8, pas de sommets 2 ni 3 ni 4 ni 6. La numérotation
      est stable mais on ne peut plus utiliser un simple vecteur pour ranger la liste de tous
      les arcs et tous les sommets aux indices correspondants, on peut utiliser une map
      qui associe un objet arc ou sommet à des indices arbitraires (pas forcément contigus)

    C'est cette 2ème approche qui est proposée ici : dans la classe graphe vous trouverez
        -> map<int, Arc>   m_arcs
        -> map<int, Sommet> m_Sommets    (le pluriel de vertex est vertices)

    Il faudra être attentif au fait que par rapport à un simple vecteur, le parcours des éléments
    ne pourra PAS se faire avec un simple for (int i=0; i<m_arcs.size(); ++i) ...m_arcs[i]...
    et que les parcours à itérateur ne donneront pas directement des Arc ou des Sommet
    mais des pairs, l'objet d'intérêt se trouvant dans "second" ("first" contenant l'indice)
                for (auto &it = m_arcs.begin(); it!=m_arcs.end(); ++it) ...it->second...
    ou bien     for (auto &e : m_arcs) ...e.second...

    Il n'est pas obligatoire d'utiliser ces classes pour le projet, vous pouvez faire les votres

    Au niveau de l'interface, on dissocie une classe interface associée à chaque classe fondamentale
    de telle sorte qu'il soit possible de travailler avec des graphes non représentés à l'écran
    Imaginons par exemple qu'on doive générer 1000 permutations de graphes pour tester des
    combinaisons, on ne souhaite pas représenter graphiquement ces 1000 graphes, et les
    interfaces pèsent lourd en ressource, avec cette organisation on est libre de réserver ou
    pas une interface de présentation associée aux datas (découplage données/interface)

***********************************************************************************************/

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

#include "grman/grman.h"

/***************************************************
                    VERTEX
****************************************************/

class SommetInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Sommet;
    friend class ArcInterface;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // La boite qui contient toute l'interface d'un sommet
        grman::WidgetBox m_top_box;

        // Un slider de visualisation/modification de la valeur du sommet
        grman::WidgetVSlider m_slider_value;

        // Un label de visualisation de la valeur du sommet
        grman::WidgetText m_label_value;

        // Une image de "remplissage"
        grman::WidgetImage m_img;

        // Un label indiquant l'index du sommet
        grman::WidgetText m_label_idx;

        // Une boite pour le label précédent
        grman::WidgetText m_box_label_idx;

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        SommetInterface(int idx, int x, int y, std::string pic_name="", int pic_idx=0);
        //bool Suppr();
};


class Sommet
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class SommetInterface;
    friend class Arc;
    friend class ArcInterface;

    private :
        /// liste des indices des arcs arrivant au sommet : accès aux prédécesseurs
        std::vector<int> m_in;

        /// liste des indices des arcs partant du sommet : accès aux successeurs
        std::vector<int> m_out;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_value;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<SommetInterface> m_interface = nullptr;

        // Docu shared_ptr : https://msdn.microsoft.com/fr-fr/library/hh279669.aspx
        // La ligne précédente est en gros équivalent à la ligne suivante :
        // SommetInterface * m_interface = nullptr;

        std::string m_nom;
        int m_coordx;
        int m_coordy;

    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Sommet (double value=0, SommetInterface *interface=nullptr) :
            m_value(value), m_interface(interface)  {  }
        Sommet(double value, std::string nom) :
            m_value(value), m_nom(nom) {  }


        /// Sommet étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Sommet (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};



/***************************************************
                    EDGE
****************************************************/

class ArcInterface
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Arc;
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        // Le WidgetEdge qui "contient" toute l'interface d'un arc
        grman::WidgetEdge m_top_edge;

        // Une boite pour englober les widgets de réglage associés
        grman::WidgetBox m_box_edge;

        // Un slider de visualisation/modification du poids valeur de l'arc
        grman::WidgetVSlider m_slider_weight;

        // Un label de visualisation du poids de l'arc
        grman::WidgetText m_label_weight;

    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        ArcInterface(Sommet& from, Sommet& to);
};


class Arc
{
    // Les (methodes des) classes amies pourront accéder
    // directement aux attributs (y compris privés)
    friend class Graph;
    friend class ArcInterface;

    private :
        /// indice du sommet de départ de l'arc
        int m_from;

        /// indice du sommet d'arrivée de l'arc
        int m_to;

        /// un exemple de donnée associée à l'arc, on peut en ajouter d'autres...
        double m_weight;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<ArcInterface> m_interface = nullptr;


    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Arc (double weight=0, ArcInterface *interface=nullptr) :
            m_weight(weight), m_interface(interface)  {  }



        /// Arc étant géré par Graph ce sera la méthode update de graph qui appellera
        /// le pre_update et post_update de Arc (pas directement la boucle de jeu)
        /// Voir l'implémentation Graph::update dans le .cpp
        void pre_update();
        void post_update();
};




/***************************************************
                    GRAPH
****************************************************/

class GraphInterface
{
    friend class Graph;

    private :

        /// Les widgets de l'interface. N'oubliez pas qu'il ne suffit pas de déclarer
        /// ici un widget pour qu'il apparaisse, il faut aussi le mettre en place et
        /// le paramétrer ( voir l'implémentation du constructeur dans le .cpp )

        /// La boite qui contient toute l'interface d'un graphe
        grman::WidgetBox m_top_box;

        /// Dans cette boite seront ajoutés les (interfaces des) sommets et des arcs...
        grman::WidgetBox m_main_box;

        /// Dans cette boite seront ajoutés des boutons de contrôle etc...
        grman::WidgetBox m_tool_box;

        grman::WidgetButton m_bouton_sauv;       // Sera le bouton avec le texte sauvegarde
        grman::WidgetText m_bouton_label_sauv;
        grman::WidgetCheckBox m_pause;       // Sera la checkbox avec le texte pause
        grman::WidgetText m_checkbox_label_paus;
        grman::WidgetButton m_bouton_charge;       // Sera le bouton avec le texte chargement
        grman::WidgetText m_bouton_label_charge;
        grman::WidgetButton m_bouton_ajout;       // Sera le bouton avec le texte ajouter
        grman::WidgetText m_bouton_label_ajout;
        grman::WidgetButton m_bouton_suppr_s;       // Sera le bouton avec le texte supprimer les sommet
        grman::WidgetText m_bouton_label_suppr_s;
        grman::WidgetButton m_bouton_suppr_a;       // Sera le bouton avec le texte supprimer les arc
        grman::WidgetText m_bouton_label_suppr_a;
        grman::WidgetButton m_bouton_quit;       // Sera le bouton avec le texte supprimer
        grman::WidgetText m_bouton_label_quit;     //bouton pour quitter l'application

        // A compléter éventuellement par des widgets de décoration ou
        // d'édition (boutons ajouter/enlever ...)



    public :

        // Le constructeur met en place les éléments de l'interface
        // voir l'implémentation dans le .cpp
        GraphInterface(int x, int y, int w, int h);
};


class Graph
{
    private :

        /// La "liste" des arêtes
        std::map<int, Arc> m_arcs;

        /// La liste des sommets
        std::map<int, Sommet> m_Sommets;

        /// le POINTEUR sur l'interface associée, nullptr -> pas d'interface
        std::shared_ptr<GraphInterface> m_interface = nullptr;

        ///ordre, nb arete
        int m_ordre;
        int m_nb_arete;





    public:

        /// Les constructeurs sont à compléter selon vos besoin...
        /// Ici on ne donne qu'un seul constructeur qui peut utiliser une interface
        Graph (GraphInterface *interface=nullptr) :
            m_interface(interface)  {  }

        void add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name="", int pic_idx=0 );
        void add_interfaced_edge(int idx, int vert1, int vert2, double weight=0);

        /// Méthode spéciale qui construit un graphe arbitraire (démo)
        /// Voir implémentation dans le .cpp
        /// Cette méthode est à enlever et remplacer par un système
        /// de chargement de fichiers par exemple.
        void make_example();
        void is_charge(); //sert a savoir si le bouron charge est appuyé, si oui, on carge un fichier
        void charge_file(bool charge); //charge un graphe a partir d'un fichier
        void sauvegarde_graph(); //enregistre tout un graphe, positions, ect...
        bool quitter();
        void suppr_arc();
        void suppr_sommet();


        /// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
        void update();
};


#endif // GRAPH_H_INCLUDED
