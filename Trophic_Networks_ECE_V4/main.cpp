/*!
 * \file main.cpp
 * \brief main du programme
 */
#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    int tours_de_boucle=0;
    bool quit = false;
    //bool affiche = false;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("ressources/images");
    /// Un exemple de graphe
    Graph g;
    //g.make_example();
    /*!
     * Charge le premier graphe
     */
    g.charge_file(true);
    //g.cherche_idx_a();
    /// Vous gardez la main sur la "boucle de jeu"
       /*!
        *Garde la main sur la boucle de jeu : tant que l'utilisateur ne quitte pas l'application
        */
    while ( !key[KEY_ESC] && quit == false)
    {

        /*if(affiche == true)
        {
            for(auto &elem : g.m_Sommets)
            {
                std::cout<<std::endl;
                std::cout<<"index sommets main: "<<elem.first<<std::endl;
            }
        }*/
        g.update(); /*!appelle les méthodes d'update des objets qui comportent des widgets*/

        //si l'utilisateur veut charger un graphe
        g.is_charge(); /*!Charge un graphe*/
        //on regarde si le bouton sauvegarde est appuyé et on sauvegarde ou non
        g.sauvegarde_graph(); /*!Sauvegarde un graphe*/
        //si l'utilisateur veut quitter l'application
        quit = g.quitter(); /*!quitte l'application*/
        //si l'utilisateur veut supprimer un arc ou un sommet
        g.suppr_arc(-1); /*!Supprime un arc*/
        //affiche = g.suppr_sommet();
        g.suppr_sommet(); /*!Supprime un sommet*/

        //si l'utilisateur veut ajouter un sommet ou un arc
        g.add_interfaced_Sommet( 0, 0, 0, 0,"", 0,0.0,0.0); /*!Ajoute un sommet*/

        g.add_interfaced_Arc(0,0,0,0.0); /*!Ajoute un arc*/


        //si l'utilisateur veut voir les composantes fortement connexe du graphe
        g.cherche_connexe(); /*!Cherche les composantes fortement connexes*/

        //on remet les marquage a 0 quand le bouton est appuyé
        g.remise_zero(); /*!Remise a zero des marquages*/



        //etude temporelle
        tours_de_boucle=g.simu(tours_de_boucle); /*!Etude temporelle*/
        g.arret_etude(); /*!Fin etude temporelle*/

        grman::mettre_a_jour(); /*!Mise à jour générale (clavier/souris/buffer etc...)*/


    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


