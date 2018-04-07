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
    g.charge_file(true);
    //g.cherche_idx_a();
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] && quit == false)
    {

        /*if(affiche == true)
        {
            for(auto &elem : g.m_Sommets)
            {
                std::cout<<std::endl;
                std::cout<<"index : "<<elem.first<<std::endl;
            }
        }*/
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();

        //si l'utilisateur veut charger un graphe
        g.is_charge();
        //on regarde si le bouton sauvegarde est appuyé et on sauvegarde ou non
        g.sauvegarde_graph();
        //si l'utilisateur veut quitter l'application
        quit = g.quitter();
        //si l'utilisateur veut supprimer un arc ou un sommet
        g.suppr_arc();
        g.suppr_sommet();

        //si l'utilisateur veut ajouter un sommet ou un arc
        g.add_interfaced_Sommet( 0, 0, 0, 0,"", 0 );

        g.add_interfaced_Arc(0,0,0,0.0);


        //si l'utilisateur veut voir les composantes fortement connexe du graphe
        g.cherche_connexe();

        //on remet les marquage a 0 quand le bouton est appuyé
        g.remise_zero();



        //etude temporelle
        tours_de_boucle=g.simu(tours_de_boucle);


        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();





    }



    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


