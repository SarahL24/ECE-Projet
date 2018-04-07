#include "grman/grman.h"
#include <iostream>

#include "graph.h"

int main()
{
    bool quit = false;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("ressources/images");
    /// Un exemple de graphe
    Graph g;
    //g.make_example();
    g.charge_file(true);
    //g.cherche_idx_a();
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] && quit == false)
    {

        g.suppr_sommet();
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();

        //si l'utilisateur veut charger un graphe
        g.is_charge();
        //on regarde si le bouton sauvegarde est appuy� et on sauvegarde ou non
        g.sauvegarde_graph();
        //si l'utilisateur veut quitter l'application
        quit = g.quitter();
        //si l'utilisateur veut supprimer un arc ou un sommet
        g.suppr_arc();


        //si l'utilisateur veut ajouter un sommet ou un arc
        g.add_interfaced_Sommet( 0, 0, 0, 0,"", 0 );
        g.add_interfaced_Arc(0,0,0,0.0);


        //g.affiche_sommets(g.m_Sommets);
        //si l'utilisateur veut voir les composantes fortement connexe du graphe
        g.cherche_connexe();
        //on remet les marquage a 0 quand le bouton est appuy�
        g.remise_zero();

        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();




    }



    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


