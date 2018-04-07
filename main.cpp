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

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] && quit == false)
    {

        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();


        //si l'utilisateur veut charger un graphe
        g.is_charge();
        //on regarde si le bouton sauvegarde est appuy� et on sauvegarde ou non
        g.sauvegarde_graph();
        //si l'utilisateur veut quitter l'application
        quit = g.quitter();
        g.suppr_arc();
        g.suppr_sommet();

        g.add_interfaced_Sommet( 0, 0, 0, 0,"ressources/graphe3/", 0 );
        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();


    }


    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


