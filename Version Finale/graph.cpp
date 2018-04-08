/*!
 * \file graphe.cpp
 * \brief implementation des classes : graphe, arc et sommet
 */
#include "graph.h"

/***************************************************
                    SOMMET
****************************************************/
/*!
 *  \brief Constructeur
 *
 *  Le constructeur met en place les éléments de l'interface
 */
SommetInterface::SommetInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();


    // Le slider de réglage de valeur de la population
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0, 100.0);  // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,100);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}


/*!
 *  Gestion du Sommet avant l'appel à l'interface
 */
void Sommet::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );

    if(m_color == 1) //on regarde si le sommet a une mise en valeur ou non
    {
        m_interface->m_top_box.set_bg_color(MARRON);
    }
    else if (m_color == 2)
    {
        m_interface->m_top_box.set_bg_color(ROSE);
    }
    else
    {
        m_interface->m_top_box.set_bg_color(BLANC);
    }

}


/*!
 *  Gestion du Sommet après l'appel à l'interface
 */
void Sommet::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale et les coordonnees du sommet
    m_value = m_interface->m_slider_value.get_value();
    m_coordx = m_interface->m_top_box.get_posx();
    m_coordy = m_interface->m_top_box.get_posy();
}



/***************************************************
                    ARC
****************************************************/

/*!
 *  \brief Constructeur
 *
 *  Le constructeur met en place les éléments de l'interface
 */
ArcInterface::ArcInterface(Sommet& from, Sommet& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating ArcInterface between vertices having no interface" << std::endl;
        throw "Bad ArcInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    //Une boite pour afficher l'index de l'arete
    m_top_edge.add_child(m_box_index);
    m_box_index.set_dim(10,10);
    m_box_index.set_bg_color(MARRONCLAIR);
    m_box_index.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);

    //Une visualisation de l'index
    m_box_index.add_child(m_label_index);
    m_label_index.set_gravity_xy(grman::GravityX::Center,grman::GravityY::Center);


    // Le slider de réglage de valeur de nourriture
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0, 10.0);  // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/*!
 *  Gestion de l'arc avant l'appel à l'interface
 */
void Arc::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (double)m_weight ) );

    ///Copier l'index dans le label d'index
    m_interface->m_label_index.set_message(std::to_string( (int)m_index ));
}

/*!
 *  Gestion de l'arc après l'appel à l'interface
 */
void Arc::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/


/*!
 * \brief Constructeur
 *  Ici le constructeur se contente de préparer un cadre d'accueil des
 *  éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
 */
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    //premier bouton de sauvegarde
    m_tool_box.add_child(m_bouton_sauv);
    m_bouton_sauv.set_frame(3,8,80,20);
    m_bouton_sauv.set_bg_color(VERTCLAIR);
    m_bouton_sauv.add_child(m_bouton_label_sauv);
    m_bouton_label_sauv.set_message("Sauvegarde");

    //deuxieme bouton de chargement
    m_tool_box.add_child(m_bouton_charge);
    m_bouton_charge.set_frame(3,33,80,20);
    m_bouton_charge.set_bg_color(VERTCLAIR);
    m_bouton_charge.add_child(m_bouton_label_charge);
    m_bouton_label_charge.set_message("Chargement");

    //troisieme bouton pour ajouter un sommet
    m_tool_box.add_child(m_bouton_ajout_sommet);
    m_bouton_ajout_sommet.set_frame(3,58,80,20);
    m_bouton_ajout_sommet.set_bg_color(VERTCLAIR);
    m_bouton_ajout_sommet.add_child(m_bouton_label_ajout_sommet);
    m_bouton_label_ajout_sommet.set_message("Ajouter sommet");

    //troisieme bouton pour ajouter un arc
    m_tool_box.add_child(m_bouton_ajout_arc);
    m_bouton_ajout_arc.set_frame(3,108,80,20);
    m_bouton_ajout_arc.set_bg_color(VERTCLAIR);
    m_bouton_ajout_arc.add_child(m_bouton_label_ajout_arc);
    m_bouton_label_ajout_arc.set_message("Ajouter arc");

    //quatrieme bouton pour supprimer sommet
    m_tool_box.add_child(m_bouton_suppr_s);
    m_bouton_suppr_s.set_frame(3,83,80,20);
    m_bouton_suppr_s.set_bg_color(VERTCLAIR);
    m_bouton_suppr_s.add_child(m_bouton_label_suppr_s);
    m_bouton_label_suppr_s.set_message("Supprimer sommet");

    //bouton pour supprimer arc
    m_tool_box.add_child(m_bouton_suppr_a);
    m_bouton_suppr_a.set_frame(3,133,80,20);
    m_bouton_suppr_a.set_bg_color(VERTCLAIR);
    m_bouton_suppr_a.add_child(m_bouton_label_suppr_a);
    m_bouton_label_suppr_a.set_message("Supprimer arc");

    //bouton pour l'etude temporelle
    m_tool_box.add_child(m_bouton_etude);
    m_bouton_etude.set_frame(3,158,80,20);
    m_bouton_etude.set_bg_color(VERTCLAIR);
    m_bouton_etude.add_child(m_bouton_label_etude);
    m_bouton_label_etude.set_message("etude");

    //bouton pour l'etude temporelle2
    m_tool_box.add_child(m_bouton_etude2);
    m_bouton_etude2.set_frame(3,183,80,20);
    m_bouton_etude2.set_bg_color(VERTCLAIR);
    m_bouton_etude2.add_child(m_bouton_label_etude2);
    m_bouton_label_etude2.set_message("etude2");

    //bontou pour arreter l'etude
    m_tool_box.add_child(m_bouton_arret);
    m_bouton_arret.set_frame(3,208,80,20);
    m_bouton_arret.set_bg_color(VERTCLAIR);
    m_bouton_arret.add_child(m_bouton_label_arret);
    m_bouton_label_arret.set_message("Arret");

    //debeug
    m_tool_box.add_child(m_checkbox_debeug);
    m_checkbox_debeug.set_frame(3,233,80,20);
    m_checkbox_debeug.set_bg_color(VERTCLAIR);
    m_checkbox_debeug.add_child(m_checkbox_label_debeug);
    m_checkbox_label_debeug.set_message("debeug");

    //bouton pour chercher connexite du graphe
    m_tool_box.add_child(m_bouton_connexe);
    m_bouton_connexe.set_frame(3,258,80,20);
    m_bouton_connexe.set_bg_color(VERTCLAIR);
    m_bouton_connexe.add_child(m_bouton_label_connexe);
    m_bouton_label_connexe.set_message("compo forte connexe");

    m_tool_box.add_child(m_bouton_graphe_red);
    m_bouton_graphe_red.set_frame(3,283,80,20);
    m_bouton_graphe_red.set_bg_color(VERTCLAIR);
    m_bouton_graphe_red.add_child(m_bouton_label_graphe_red);
    m_bouton_label_graphe_red.set_message("Grapphe reduit");


    //bouton pour retirer les marquages du graphe
    m_tool_box.add_child(m_bouton_retirer);
    m_bouton_retirer.set_frame(3,675,80,20);
    m_bouton_retirer.set_bg_color(VERTCLAIR);
    m_bouton_retirer.add_child(m_bouton_label_retirer);
    m_bouton_label_retirer.set_message("retirer marq");

    //bouton pour quitter
    m_tool_box.add_child(m_bouton_quit);
    m_bouton_quit.set_frame(3,700,80,20);
    m_bouton_quit.set_bg_color(VERTCLAIR);
    m_bouton_quit.add_child(m_bouton_label_quit);
    m_bouton_label_quit.set_message("Quitter");



    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);
}


/// Méthode spéciale qui construit un graphe arbitraire (démo)
/// Cette méthode est à enlever et remplacer par un système
/// de chargement de fichiers par exemple.
/// Bien sûr on ne veut pas que vos graphes soient construits
/// "à la main" dans le code comme ça.
void Graph::make_example()
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent être définis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
    add_interfaced_vertex(0, 30.0, 200, 100, "Herbe.bmp");
    add_interfaced_vertex(1, 60.0, 400, 100, "Homme.bmp");
    //add_interfaced_vertex(2,  50.0, 200, 300, "Gigantoraptor.bmp");
    //add_interfaced_vertex(3,  0.0, 400, 300, "Iguanodon.bmp");
    //add_interfaced_vertex(4,  100.0, 600, 300, "Oviraptor.bmp");
    //add_interfaced_vertex(5,  0.0, 100, 500, "Spinosaure.bmp");
    //add_interfaced_vertex(6,  0.0, 300, 500, "Ver_de_terre.bmp");
    //add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg",3);

    /// Les arcs doivent être définis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
    //add_interfaced_edge(0, 1, 3, 50.0);
    //add_interfaced_edge(1, 3, 4, 50.0);
    //add_interfaced_edge(2, 4, 2, 75.0);
    //add_interfaced_edge(3, 2, 0, 25.0);
    //add_interfaced_edge(4, 0, 5, 25.0);
    //add_interfaced_edge(5, 6, 1, 25.0);
    //add_interfaced_edge(6, 5, 6, 0.0);
}

/*!
 *  La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
 */
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_Sommets)
        elt.second.pre_update();

    for (auto &elt : m_arcs)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_Sommets)
        elt.second.post_update();

    for (auto &elt : m_arcs)
        elt.second.post_update();

}


/*!
 *  \brief cherche l'indice  le plus grand parmi tous les sommets
 * renvoie l'index le plus grand parmi les sommets
 */
int Graph::cherche_idx_s()
{
    int tmp = m_Sommets.begin()->first;
    for(auto &elem : m_Sommets)
    {
        if(tmp < elem.first)
        {
            tmp = elem.first;
        }
    }
    std::cout<<"index : "<< tmp<<std::endl;
    return tmp;
}

/*!
 *  \brief cherche l'indice  le plus grand parmi tous les arcs
 * renvoie l'index le plus grand parmi les arcs
 */
int Graph::cherche_idx_a()
{
    int tmp = m_arcs.begin()->first;

    for(auto &elem : m_arcs)
    {
        if(tmp < elem.first)
        {
            tmp = elem.first;
        }
    }
    std::cout<<"index : "<< tmp<<std::endl;
    return tmp;
}


/*!
 *  \brief ajout d'un sommet
 * Aide à l'ajout de sommets interfacés
 *  Ajoute un sommet dans l'interface graphique et dans notre map de sommets
 */
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx, double coef_repro, double coef_basic )
{
    if ( m_Sommets.find( idx)!= m_Sommets.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    SommetInterface *vi = new SommetInterface( idx, x, y, pic_name+".jpg", pic_idx);
    // Ajout de la top box de l'interface de somet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_Sommets[idx] = Sommet(value, vi);
    m_Sommets[idx].m_coordx = x; //coordonnees
    m_Sommets[idx].m_coordy = y;
    m_Sommets[idx].m_nom = pic_name; //nom
    m_Sommets[idx].m_coeff_reproduction = coef_repro; //coefficients ressources
    m_Sommets[idx].m_croissance_basic = coef_basic;
    m_Sommets[idx].m_color = 0;
    //m_Sommets[idx].m_index = idx;


}


/*!
 *  \brief ajout d'un sommet manuellement
 *
 *  Ajoute un sommet dans l'interface graphique et dans notre map de sommets a partir de l'interface
 */
void Graph::add_interfaced_Sommet(int idx, double value, int x, int y, std::string path, int pic_idx, double coeef_repro, double coef_basic )
{

    //initialisation
    std::string path_txt;
    std::string name;
    std::string temp;
    int choix; //choix du sommet
    int idx_g;
    bool ok = false;
    double coef_repro = 0;
    double coef_bas = 0;

    std::vector<std::string> noms_animaux;

    if ( m_interface->m_bouton_ajout_sommet.get_value()== true)//si le bouton pour ajouter un sommet a ete appuye
    {
        while(choix!=1 && choix!=2 && choix !=3) //blindage choix du graphe
        {
            std::cout<<"entrer le graphe que vous voulez (1,2 ou 3) pour l'ajout de sommet : ";
            std::cin>>choix;
            if(choix==1)
            {
                path_txt = "ressources/graphe1/Noms_Dino.txt";
            }
            if(choix==2)
            {
                path_txt = "ressources/graphe2/Noms_Sib.txt";
            }
            if(choix ==3)
            {
                path_txt = "ressources/graphe3/Noms_Prehi.txt";
            }
        }
        std::ifstream fichier (path_txt,std::ios::in);
        if(fichier) //on affiche les sommets de cet ecosysteme
        {
            std::string line;
            std::cout<<"Voici la liste de sommets disponible a ajouter dans ce graph:"<<std::endl;
            while(getline(fichier, line))
            {
                std::cout<<line<<std::endl;
                noms_animaux.push_back(line);
            }

        }
        else  // sinon
        {
            std::cerr << "Impossible d'ouvrir le fichier ici !" << std::endl;
        }

        while(ok == false)//blindage du choix
        {
            std::cout<<"Choisir l'animal a ajouter s'il vous plait"<<std::endl;
            std::cin>> name;
            for(unsigned int i=0; i<noms_animaux.size(); i++)
            {
                if(noms_animaux[i] == name)
                {
                    ok = true;
                }
            }
        }

        std::cout<<"Choisir le coefficient de reproduction de l'animal a ajouter s'il vous plait"<<std::endl;
        std::cin>> coef_repro;
        std::cout<<"Choisir le coefficient de croissance basique de l'animal a ajouter s'il vous plait"<<std::endl;
        std::cin>> coef_bas;

        idx_g = cherche_idx_s();
        add_interfaced_vertex(idx_g+1,value,x,y,name,pic_idx, coef_repro, coef_bas);//creation du sommet
    }//fin if
    m_interface->m_bouton_ajout_sommet.set_value(false); //on remet la valeur du bouton a false
}


/*!
 *  \brief ajout d'un arc
 *  Aide à l'ajout d'arcs interfacés
 *  Ajoute un sommet dans l'interface graphique et dans notre map d'arcs
 */
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_arcs.find(idx)!=m_arcs.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_Sommets.find(id_vert1)==m_Sommets.end() || m_Sommets.find(id_vert2)==m_Sommets.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_Sommets" << std::endl;
        throw "Error adding edge";
    }

    //on creer l'arc dans la map du graphe
    ArcInterface *ei = new ArcInterface(m_Sommets[id_vert1], m_Sommets[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_arcs[idx] = Arc(weight, ei);
    m_arcs[idx].m_from = id_vert1;
    m_arcs[idx].m_to = id_vert2;
    m_arcs[idx].m_index = idx;
    m_Sommets[id_vert2].m_in.push_back(id_vert1);
    m_Sommets[id_vert1].m_out.push_back(id_vert2);

}

/*!
*  \brief ajout d'un arc manuellement
*
*  Ajoute un arc dans l'interface graphique et dans notre map d'arc
*/
void Graph::add_interfaced_Arc(int idx, int id_vert1, int id_vert2, double weight)
{
    int depart,arriver;
    int idx_g;
    bool ok = false;

    if( m_interface->m_bouton_ajout_arc.get_value()== true)//si le bouton pour ajouter un arc est appuye
    {
        std::cout<<"Creer un arc"<<std::endl;

        while(ok == false) //blindage du choix du sommet de depart
        {
            std::cout<<"depart: ";
            std::cin>>depart;
            for(auto &elem : m_Sommets)
            {
                if(elem.first == depart)
                {
                    ok = true;
                }
            }
        }
        ok = false;
        while(ok == false) //blindage du choix du sommet d'arriver
        {
            std::cout<<"arriver: ";
            std::cin>>arriver;
            for(auto &elem : m_Sommets)
            {
                if(elem.first == arriver)
                {
                    ok = true;
                }
            }
        }

        idx_g = cherche_idx_a();
        add_interfaced_edge(idx_g+1,depart,arriver,0);//on creer l'arc

    }

    m_interface->m_bouton_ajout_arc.set_value(false);//on remet la valeur du bouton a false
}

/*!
 *  \brief charge un graphe si l'utilisateur le demande
 *
 *  sert a savoir si le bouton charge est appuyé, si oui, on lance le chargement, en effacant toutes les données deja existantes
 */
void Graph::is_charge()
{

    if(m_interface->m_bouton_charge.get_value()==true)
    {

        //on efface les donnes puis on charge un graphe
        m_arcs.erase(m_arcs.begin(),m_arcs.end());
        m_Sommets.erase(m_Sommets.begin(), m_Sommets.end());
        charge_file(true);
    }
}

/*!
 *  \brief charge un graphe
 *
 *  charge un graphe a partir d'un fichier text
 */
void Graph::charge_file(bool charge)
{
    //initialisation
    std::vector<std::string> aretes_tmp;
    std::string sousChaine;
    std::string str_tmp;
    double poids;
    int m1;
    int m2;
    int value;
    int coordsx;
    int coordsy;
    int choix = 0;
    std::string path;
    double coef_repro, coef_basic;



    while(choix!=1 && choix!=2 && choix !=3 && choix!=4)//choix du graphe, blinde
    {
        std::cout<<"entrer le graphe que vous voulez (1,2 ou 3) pour le chargement du graphe : ";
        std::cin>>choix;
        if(choix == 1)
        {
            path = "ressources/graphe1/Graphe_Dino.txt";
        }
        if(choix == 2)
        {
            path = "ressources/graphe2/Graphe_Sib.txt";
        }
        if(choix == 3)
        {
            path = "ressources/graphe3/Graphe_Prehi.txt";
        }
        if(choix == 4)
        {
            path = "ressources/graphe4/graphe_forte_connexite.txt";
        }
    }


    if(charge == true)
    {
        m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);


        ///On charge le fichier
        std::ifstream fichier(path, std::ios::in);  // on ouvre le fichier en lecture
        if(fichier)  // si l'ouverture a réussi
        {

            // instructions dans le fichier ouvert
            std::string line; // ligne temporaire
            std::string nom_sommet_temp;

            getline(fichier, line); // On lit la première ligne qui est un entier représentant l'ordre
            if(atoi(line.c_str())) // Si la premiere ligne coreespond à un nombre reel
            {
                m_ordre = atoi(line.c_str()); //converti la chaine de caractere recu en integer
            }
            else
            {
                std::cerr << "Votre fichier ne contient pas un ordre correct !" << std::endl;
            }

            /// Remplissage du vecteur de sommet
            // Les sommets doivent être définis avant les arcs
            // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
            for(int i=0; i<m_ordre; i++) // pour tous les sommets dans le fichier, on les creer dans le graphe
            {
                getline(fichier,line);
                //std::cout<<line<<std::endl;
                std::stringstream ss(line);
                while(getline(ss,sousChaine,'/'))
                {
                    //std::cout<<sousChaine<<std::endl;
                    aretes_tmp.push_back(sousChaine);
                }

                str_tmp = aretes_tmp.back();
                //std::cout<<"coef av "<<str_tmp<<std::endl;
                coef_repro = atof(str_tmp.c_str());
                //std::cout<<"coef ap "<<coef_repro<<std::endl;
                aretes_tmp.pop_back();
                str_tmp = aretes_tmp.back();
                coef_basic = atof(str_tmp.c_str());
                //std::cout<<coef_basic<<std::endl;
                aretes_tmp.pop_back();
                str_tmp = aretes_tmp.back();
                coordsy = atoi(str_tmp.c_str());
                //std::cout<<coordsy<<std::endl;
                aretes_tmp.pop_back();
                str_tmp = aretes_tmp.back();
                coordsx = atoi(str_tmp.c_str());
                //std::cout<<coordsy<<std::endl;
                aretes_tmp.pop_back();
                str_tmp = aretes_tmp.back();
                value = atoi(str_tmp.c_str());
                //std::cout<<value<<std::endl;
                aretes_tmp.pop_back();
                nom_sommet_temp = aretes_tmp.back();
                //std::cout<<nom_sommet_temp<<std::endl;
                add_interfaced_vertex(i, value, coordsx, coordsy, nom_sommet_temp,0, coef_repro,coef_basic); //on creer le sommet avec toutes les valeurs recuperees
            }//fin for

            //affichage de donnees
            std::cout<<aretes_tmp.size()<<std::endl;
            std::cout<<m_Sommets.size()<<std::endl;
            for (int i =0; i<m_ordre; i++)
            {
                std::cout<<m_Sommets.find(i)->first<<" "<<m_Sommets.find(i)->second.m_nom<<std::endl;
            }

            //on charge le nombre d'arete
            getline(fichier, line); // On lit la première ligne qui est un entier représentant l'ordre
            if(atoi(line.c_str())) // Si la premiere ligne coreespond à un nombre reel
            {
                m_nb_arete = atoi(line.c_str()); //converti la chaine de caractere recu en integer
            }
            else
            {
                std::cerr << "Votre fichier ne contient pas un ordre correct !" << std::endl;
            }

            ///on charge les aretes et on remplit le vecteur d'arete
            /// Les arcs doivent être définis entre des sommets qui existent !
            // Ajouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
            for(int i=0; i<m_nb_arete; i++) //pour toutes les autres lignes. On lit le contenu, on le divise en plusieurs parties
            {
                //puis on met les informations recoltees dans leurs variables. extremite1, extremite2, poids
                getline(fichier,line);
                std::stringstream ss(line);
                while(getline(ss,sousChaine,'/'))
                {
                    //std::cout<<sousChaine<<std::endl;
                    aretes_tmp.push_back(sousChaine);
                }

                str_tmp = aretes_tmp.back();
                //std::cout<<str_tmp<<std::endl;
                poids = atof(str_tmp.c_str());
                //std::cout<<poids<<std::endl;
                aretes_tmp.pop_back();
                str_tmp = aretes_tmp.back();
                m2 = atoi(str_tmp.c_str());
                aretes_tmp.pop_back();
                str_tmp = aretes_tmp.back();
                m1 = atoi(str_tmp.c_str());
                add_interfaced_edge(i, m1, m2, poids);//on crer l'arc
            } //fin for
        } //fin if fichier
        else  // sinon
        {
            std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
        }
    }//fin if bool
    m_interface->m_bouton_charge.set_value(false);//on remet la valeur du bouton charge a false
}

/*!
 *  \brief sauvegarde un graphe
 *
 *  sauvegarde un graphe dans un fichier texte
 */
void Graph::sauvegarde_graph()
{
    if(m_interface->m_bouton_sauv.get_value() == true)//si le bouton sauvegarde est appuye
    {
        std::string path;
        std::cout<<"entrer le chemin pour enregistrer votre graphe : ";
        std::cin>>path;
        std::ofstream fichier(path, std::ios::out | std::ios::trunc);  //déclaration du flux et ouverture du fichier
        if(fichier)
        {
            fichier <<m_ordre<<std::endl;
            for(auto &elem : m_Sommets)//enregistrements des sommets
            {
                fichier<<elem.second.m_nom<<"/"<<elem.second.m_value<<"/"<<elem.second.m_coordx<<"/"<<elem.second.m_coordy<<"/"<<elem.second.m_coeff_reproduction<<"/"<<elem.second.m_croissance_basic<<std::endl;
                std::cout<<elem.second.m_nom<<"/"<<elem.second.m_value<<"/"<<elem.second.m_coordx<<"/"<<elem.second.m_coordy<<"/"<<elem.second.m_coeff_reproduction<<"/"<<elem.second.m_croissance_basic<<std::endl;
            }
            fichier<<m_nb_arete<<std::endl;
            for(auto &elem : m_arcs)//enregistrements des arcs
            {
                fichier<<elem.second.m_from<<"/"<<elem.second.m_to<<"/"<<elem.second.m_weight<<std::endl;
                std::cout<<elem.second.m_from<<"/"<<elem.second.m_to<<"/"<<elem.second.m_weight<<std::endl;
            }
            fichier.close();
        }
        else
        {
            std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
        }
    }


    m_interface->m_bouton_sauv.set_value(false);//on remet la valeur du bouton a false
}

///sauvegarde du graphe pour afficher le graphe reduit
void Graph::sauvegarde_graph_2()
{

        std::string path;
        std::cout<<"entrer le chemin pour enregistrer votre graphe : ";
        std::cin>>path;
        std::ofstream fichier(path, std::ios::out | std::ios::trunc);  //déclaration du flux et ouverture du fichier
        if(fichier)
        {
            fichier <<m_ordre<<std::endl;
            for(auto &elem : m_Sommets)//enregistrements des sommets
            {
                fichier<<elem.second.m_nom<<"/"<<elem.second.m_value<<"/"<<elem.second.m_coordx<<"/"<<elem.second.m_coordy<<"/"<<elem.second.m_coeff_reproduction<<"/"<<elem.second.m_croissance_basic<<std::endl;
                std::cout<<elem.second.m_nom<<"/"<<elem.second.m_value<<"/"<<elem.second.m_coordx<<"/"<<elem.second.m_coordy<<"/"<<elem.second.m_coeff_reproduction<<"/"<<elem.second.m_croissance_basic<<std::endl;
            }
            fichier<<m_nb_arete<<std::endl;
            for(auto &elem : m_arcs)//enregistrements des arcs
            {
                fichier<<elem.second.m_from<<"/"<<elem.second.m_to<<"/"<<elem.second.m_weight<<std::endl;
                std::cout<<elem.second.m_from<<"/"<<elem.second.m_to<<"/"<<elem.second.m_weight<<std::endl;
            }
            fichier.close();
        }
        else
        {
            std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;
        }
}


/*!
 *  \brief quitte l'application
 *  sert a savoir si l'utilisateur veut quitter
 *  quitte l'application si l'utilisateur le demande
 */
bool Graph::quitter()
{
    if(m_interface->m_bouton_quit.get_value()== true)
    {
        return true;
    }
    return false;
}

/*!
*  \brief supprime un arc
*/
void Graph::suppr_arc(int idx)
{
    int idx_arc;
    bool ok =false;
    if(m_interface->m_bouton_suppr_a.get_value()==true) //supprime si le bouton est appuyé
    {
        while(ok == false) //blindage du choix d'arc a supprimer
        {
            std::cout<<"entrer l'index de l'arc que vous voulez supprimer : ";
            std::cin>> idx_arc;
            for(auto &elem : m_arcs)
            {
                if(elem.first == idx_arc)
                {
                    ok = true;
                }
            }
        }

        //ref sur l'arc a enlever
        Arc & arc_sup = m_arcs.at(idx_arc);
        std::cout<< "arc a enlever : "<<idx_arc<<" "<<arc_sup.m_from<<" -> "<<arc_sup.m_to<<"  "<<arc_sup.m_weight<<std::endl;

        std::cout<<" pred initial : "<<m_Sommets[arc_sup.m_from].m_in.size()<< " succ initial : "<<m_Sommets[arc_sup.m_from].m_out.size()<< " pred final :  "<<m_Sommets[arc_sup.m_to].m_in.size()<< " succ final : "<<m_Sommets[arc_sup.m_to].m_out.size()<<std::endl;
        std::cout<< m_arcs.size()<<std::endl;

        if(m_interface && arc_sup.m_interface) //on efface dans l'interface
        {
            m_interface->m_main_box.remove_child(arc_sup.m_interface->m_top_edge);
        }

        //on efface dans les sommets
        m_Sommets[arc_sup.m_from].m_out.erase(std::remove(m_Sommets[arc_sup.m_from].m_out.begin(),m_Sommets[arc_sup.m_from].m_out.end(),m_arcs.at(idx_arc).m_to),m_Sommets[arc_sup.m_from].m_out.end());
        m_Sommets[arc_sup.m_to].m_in.erase(std::remove(m_Sommets[arc_sup.m_to].m_in.begin(),m_Sommets[arc_sup.m_to].m_in.end(),m_arcs.at(idx_arc).m_from),m_Sommets[arc_sup.m_to].m_in.end());
        //on efface dans les arcs
        m_arcs.erase(idx_arc);

        //verification
        std::cout<<" pred initial : "<<m_Sommets[arc_sup.m_from].m_in.size()<< " succ initial : "<<m_Sommets[arc_sup.m_from].m_out.size()<< " pred final :  "<<m_Sommets[arc_sup.m_to].m_in.size()<< " succ final : "<<m_Sommets[arc_sup.m_to].m_out.size()<<std::endl;
        std::cout<< m_arcs.size()<<std::endl;

    }//fin if bouton
    if(idx !=-1) //si on supprime un sommet, il faut aussi supprimer les arcs qui lui sont relies
    {
        Arc & arc_sup = m_arcs.at(idx);
        std::cout<< "arc a enlever : "<<idx<<" "<<arc_sup.m_from<<" -> "<<arc_sup.m_to<<"  "<<arc_sup.m_weight<<std::endl;

        std::cout<<" pred initial : "<<m_Sommets[arc_sup.m_from].m_in.size()<< " succ initial : "<<m_Sommets[arc_sup.m_from].m_out.size()<< " pred final :  "<<m_Sommets[arc_sup.m_to].m_in.size()<< " succ final : "<<m_Sommets[arc_sup.m_to].m_out.size()<<std::endl;
        std::cout<< m_arcs.size()<<std::endl;

        if(m_interface && arc_sup.m_interface) //on efface dans l'interface
        {
            m_interface->m_main_box.remove_child(arc_sup.m_interface->m_top_edge);
        }
        std::cout<<" indice somme dep : "<<arc_sup.m_from<<std::endl;
        m_Sommets[arc_sup.m_from].m_out.erase(std::remove(m_Sommets[arc_sup.m_from].m_out.begin(),m_Sommets[arc_sup.m_from].m_out.end(),m_arcs.at(idx).m_to),m_Sommets[arc_sup.m_from].m_out.end());
        m_Sommets[arc_sup.m_to].m_in.erase(std::remove(m_Sommets[arc_sup.m_to].m_in.begin(),m_Sommets[arc_sup.m_to].m_in.end(),m_arcs.at(idx).m_from),m_Sommets[arc_sup.m_to].m_in.end());
        m_arcs.erase(idx);

        std::cout<<" pred initial : "<<m_Sommets[arc_sup.m_from].m_in.size()<< " succ initial : "<<m_Sommets[arc_sup.m_from].m_out.size()<< " pred final :  "<<m_Sommets[arc_sup.m_to].m_in.size()<< " succ final : "<<m_Sommets[arc_sup.m_to].m_out.size()<<std::endl;
        std::cout<< m_arcs.size()<<std::endl;
        std::cout<<"Fait"<<std::endl;
    }//fin if sommet supp
    m_interface->m_bouton_suppr_a.set_value(false);//on remet la valeur du bouton a false

}

/*!
 *  \brief supprime un sommet
 *
 *  supprime un sommet et les arcs qui lui sont relie
 */
void Graph::suppr_sommet()
{
    int idx_s;
    bool ok = false;
    if(m_interface->m_bouton_suppr_s.get_value()==true)//si le bouton est appuye
    {
        while(ok == false) //blindage du choix du sommet a supprimer
        {
            std::cout<<"entrer l'index du sommet que vous voulez supprimer ( il faut supprimer les arcs qui le relie avant) : ";
            std::cin>> idx_s;
            for(auto &elem : m_Sommets)
            {
                if(elem.first == idx_s)
                {
                    ok = true;
                }
            }
        }

        Sommet & somm_sup = m_Sommets.at(idx_s);
        std::cout<< "sommet a enlever : "<<idx_s<<" "<<somm_sup.m_nom<<"  "<<somm_sup.m_value<<std::endl;

        //on efface dans l'interface
        if(m_interface && somm_sup.m_interface)
        {
            m_interface->m_main_box.remove_child(somm_sup.m_interface->m_top_box);
        }

        //on efface d'abord les arcs relies au sommet :
        std::cout<<m_arcs.size()<<std::endl;
        std::vector<int> arc_sup_dep;
        std::vector<int> arc_sup_fin;
        //on trouve les indices des arcs
        for(auto &elem : m_arcs)
        {
            if(elem.second.m_from == idx_s)
            {
                std::cout<<"arc trouve. "<<elem.first<<" sommet de depart"<<std::endl;;
                arc_sup_dep.push_back(elem.first);
            }
            if(elem.second.m_to == idx_s)
            {
                std::cout<<"arc trouve. "<<elem.first<<" sommet d'arriver"<<std::endl;
                arc_sup_fin.push_back(elem.first);
            }
        }
        //on les effaces
        for(auto &el : arc_sup_dep)
        {
            suppr_arc(el);
        }

        for(auto &el : arc_sup_fin)
        {
            suppr_arc(el);
        }


        //on efface dans la map de sommet du graphe
        std::cout<<"taille  m_sommets avant : "<<m_Sommets.size()<<std::endl;
        m_Sommets.erase(idx_s);
        std::cout<<"taille m_sommets apres : "<<m_Sommets.size()<<std::endl;


        //verification que l'effacement a bien ete fait
        for(auto &elem : m_Sommets)
        {
            std::cout<<"index sommets : "<<elem.first<<std::endl;
        }
        for(auto &elem : m_arcs)
        {
            std::cout<<"index arcs "<<elem.first<<" "<<elem.second.m_index<<std::endl;
        }
        m_interface->m_bouton_suppr_s.set_value(false);
        //return true;

    }//fin if bouton
    m_interface->m_bouton_suppr_s.set_value(false);//on remet le bouton a false
    //return false;
}

/*!
 *  \brief cherche les composantes fortement connexe du graphe
 */
///cherche les compo connexes *******ATTENTION A L'UTILISATION POUR LA SOUTENANCE ! BANCAL**********
void Graph::cherche_connexe()
{
    //initialisation
    int idx = 80;
    int j = 0;
    bool ok = false;
    std::queue<Sommet> file_pred;
    std::queue<Sommet> file_succ;
    std::vector<Sommet> comp_forte_connexe;
    std::vector<Sommet> vect_pred;
    std::vector<Sommet> vect_succ;
    std::vector<std::vector<Sommet>> liste_compo_connexe_p;
    std::vector<std::queue<Sommet>> liste_compo_connexe_s;
    Sommet tmp_s;
    bool stop = false;
    if(m_interface->m_bouton_connexe.get_value()== true)//si le bouton est appuye
    {
        //Reset des marquages
        /*for(unsigned int i = 0; i<m_Sommets.size(); i++)
        {
            m_Sommets[i].is_marq = false;
        }*/
        for(auto &elem : m_Sommets)
        {
            elem.second.is_marq = false;
        }
        while(ok == false) //blindage de la saisie
        {
            std::cout<<"entrer le sommet de depart : ";
            std::cin>>idx;
            for(auto &elem : m_Sommets)
            {
                if(elem.first == idx)
                {
                    ok = true;
                }
            }
        }

        m_Sommets[idx].is_marq = true;
        file_pred.push(m_Sommets[idx]);
        file_succ.push(m_Sommets[idx]);
        //on regarde la connexite dans les predecesseurs
        std::cout<<"element de la file des predecesseurs : "<<std::endl;
        while(!file_pred.empty() && stop!=true)
        {
            std::cout<< file_pred.front().m_nom<<"; ";
            tmp_s = file_pred.front();
            vect_pred.push_back(file_pred.front());
            file_pred.pop();
            std::cout<<"size : "<<tmp_s.m_in.size()<<std::endl;


            /*for(unsigned int i = 0; i<tmp_s.m_in.size();i++)
            {
                std::cout<<"valeur : "<<tmp_s.m_in[i]<<std::endl;
                if(m_Sommets[tmp_s.m_in[i]].is_marq == false)
                {
                    file_pred.push(m_Sommets[tmp_s.m_in[i]]);
                    m_Sommets[tmp_s.m_in[i]].is_marq = true;
                }
                if( j !=idx && (tmp_s.m_in[i] == idx))
                {
                    std::cout<< "Circuit !"<<std::endl;
                    stop = true;
                }
                j++;
            } //fin for*/
            for(auto &elem : tmp_s.m_in)
            {
                std::cout<<"valeur : "<<elem<<std::endl;
                if(m_Sommets[elem].is_marq == false)
                {
                    file_pred.push(m_Sommets[elem]);
                    m_Sommets[elem].is_marq = true;
                }
                if(j !=idx && elem == idx)
                {
                    std::cout<< "Circuit !"<<std::endl;
                    stop = true;
                }
                j++;
            }//fin for
        }//fin while

        //on prend toutes les composantes qui ne sont pas vide de predeccesseur, car elle ne mene a rien
        //std::cout<< "taille vect pred: "<<vect_pred.size()<<std::endl;
        /*for(unsigned int i= 0; i<vect_pred.size(); i++)
        {
            if(vect_pred[i].m_in.size() !=0)
            {
                file_pred.push(vect_pred[i]);
            }
        }*/
        for(auto &elem : vect_pred)
        {
            if(elem.m_in.size() !=0)
            {
                file_pred.push(elem);
            }
        }
        //std::cout<< "taille file pred : "<<file_pred.size()<<std::endl;
        //on vide le vecteur
        while(!vect_pred.empty())
        {
            vect_pred.pop_back();
        }
        //on remplit le vecteur des bon elements
        while(!file_pred.empty())
        {
            vect_pred.push_back(file_pred.front());
            file_pred.pop();
        }
        //std::cout<< "taille vect pred pred 2: "<<vect_pred.size()<<std::endl;
        //std::cout<< "taille file pred 2 : "<<file_pred.size()<<std::endl;
        //reset des marquages
        /*for(unsigned int i = 0; i<m_Sommets.size(); i++)
        {
            m_Sommets[i].is_marq = false;
        }*/

        for(auto &elem : m_Sommets)
        {
            elem.second.is_marq = false;
        }
        std::cout<<"elements de la file des successeurs : "<<std::endl;
        m_Sommets[idx].is_marq = true;
        j = 0;
        stop = false;
        //on regarde la connexite dans les succeceurs
        while(!file_succ.empty() && stop!=true)
        {
            std::cout<< file_succ.front().m_nom<<"; ";
            tmp_s = file_succ.front();
            vect_succ.push_back(file_succ.front());
            file_succ.pop();
            std::cout<<"size : "<<tmp_s.m_out.size()<<std::endl;
            for(unsigned int i=0; i<tmp_s.m_out.size(); i++)
            {
                std::cout<<"valeur : "<<tmp_s.m_out[i]<<std::endl;
                if(m_Sommets[tmp_s.m_out[i]].is_marq == false)
                {
                    file_succ.push(m_Sommets[tmp_s.m_out[i]]);
                    m_Sommets[tmp_s.m_out[i]].is_marq = true;
                }
                if( j !=idx && (tmp_s.m_out[i] == idx))
                {
                    std::cout<< "Circuit !"<<std::endl;
                    stop = true;
                }
                j++;
            }//fin for
        }//fin while

        //std::cout<< "taille vect succ : "<<vect_pred.size()<<std::endl;
        //on prend toutes c=les composantes qui ne sont pas vide de predeccesseur, car elle ne mene a rien
        /*for(unsigned int i= 0; i<vect_succ.size(); i++)
        {
            if(vect_succ[i].m_out.size() !=0)
            {
                file_succ.push(vect_succ[i]);
            }
        }*/
        for(auto &elem : vect_succ)
        {
            if(elem.m_out.size() !=0)
            {
                file_succ.push(elem);
            }
        }
        //std::cout<< "taille file succ : "<<file_pred.size()<<std::endl;
        //on vide le vecteur
        while(!vect_succ.empty())
        {
            vect_succ.pop_back();
        }
        //on remplit le vecteur des bon elements
        while(!file_succ.empty())
        {
            vect_succ.push_back(file_succ.front());
            file_succ.pop();
        }
        //std::cout<< "taille vect succ pred 2: "<<vect_pred.size()<<std::endl;
        //std::cout<< "taille file succ 2 : "<<file_pred.size()<<std::endl;


        //on affiche les elements des 2 vecteurs
        std::cout<<"pred : "<<std::endl;
        /*for(unsigned int i = 0; i<vect_pred.size(); i++)
        {
            std::cout<< vect_pred[i].m_nom<<std::endl;
        }
        std::cout<<"succ : "<<std::endl;
        for(unsigned int i = 0; i<vect_succ.size(); i++)
        {
            std::cout<< vect_succ[i].m_nom<<std::endl;
        }*/

        for(auto &elem : vect_pred)
        {
            std::cout<< elem.m_nom<<std::endl;
        }
        std::cout<<"succ : "<<std::endl;
        for(auto &elem : vect_succ)
        {
            std::cout<< elem.m_nom<<std::endl;
        }

        //on troouve dans le vecteur principal les elements de la compo forte connexe
        /*for(unsigned int i=0; i<vect_pred.size(); i++)
        {
            for(unsigned int k=0; k<vect_succ.size(); k++)
            {
                if (vect_pred[i].m_nom ==  vect_succ[k].m_nom)
                    {
                        comp_forte_connexe.push_back(vect_pred[i]);
                        std::cout<< "OK"<<std::endl;
                    }
            }
        }*/


        for(auto &elem : vect_pred)
        {
            for(auto &el : vect_succ)
            {
                if (elem.m_nom ==  el.m_nom)
                {
                    comp_forte_connexe.push_back(elem);
                    std::cout<< "OK"<<std::endl;
                }
            }
        }
        //on les marques (couleur : Marron)
        /*for(unsigned int i = 0; i<comp_forte_connexe.size(); i++)
        {
            for(unsigned int k = 0; k<m_Sommets.size(); k++)
            {
                if(comp_forte_connexe[i].m_nom == m_Sommets[k].m_nom && comp_forte_connexe[i].m_value == m_Sommets[k].m_value)
                {
                    m_Sommets[k].m_color = 1;
                }
            }
        }*/
        for(auto &elem  : comp_forte_connexe)
        {
            for(auto &el : m_Sommets)
            {
                if(elem.m_nom == el.second.m_nom && elem.m_value == el.second.m_value)
                {
                    el.second.m_color = 1;
                }
            }
        }
        std::cout<<"Fin :)"<<std::endl;
    }//fin if bouton
    m_interface->m_bouton_connexe.set_value(false);//on remet le bouton a false

}//fin cherche connexe

/*!
 *  \brief remet tous les marquage de l'interface a zero (les couleurs
 */
void Graph::remise_zero()
{

    if(m_interface->m_bouton_retirer.get_value() == true) //si le bouton de reset est appuye
    {
        for(auto &elem : m_Sommets)
        {
            elem.second.m_color = 0;
        }
    }
    m_interface->m_bouton_retirer.set_value(false);//on remet le bouton a false
}


/*!
*  \brief Etude temporelle
*
*  Etude temporelle de l'ecosysteme
*/
///Etude temporelle, MARCHE mais pas de coeff
int Graph::simu( int tours_de_boucle)
{
    int t= tours_de_boucle;
    if(m_interface->m_bouton_etude.get_value()==true)
    {
        for(auto &b : m_Sommets)
        {
            ///////////////////////////AUGMENTATION/////////////////////////////
            double nb_proies=0;
            //double nb_proies_temp=0;
            double pertes_totales=0;
            for (auto &elem1 : b.second.m_in)
            {
                if(m_Sommets[elem1].m_value>0)
                {
                    nb_proies++;
                }
            }
            b.second.m_value+= b.second.m_croissance_basic;
            b.second.m_value+= nb_proies;
            //////////////////////////PERTES///////////////////////////////////////
            for( unsigned int i = 0; i<b.second.m_out.size(); i++)
            {
                pertes_totales++;
            }

            if(b.second.m_value>nb_proies)
            {
                b.second.m_value--;
            }
            if((b.second.m_value-pertes_totales)>0)
            {
                b.second.m_value-=pertes_totales;
            }
            else if ((b.second.m_value-pertes_totales)<=0)
            {
                b.second.m_value=0;
            }
        }
    }
    if(m_interface->m_checkbox_debeug.get_value() ==true)
    {
        m_interface->m_bouton_etude.set_value(false);
    }
    t=tours_de_boucle++;
    return t;
}


/*!
*  \brief Etude temporelle
*
*  Etude temporelle de l'ecosysteme
*/
///Etude temporelle 1
int Graph::simu2( int tours_de_boucle)
{
    int t= tours_de_boucle;
    if(m_interface->m_bouton_etude2.get_value()==true)
    {
        ///////////////////// GROSSE BOUCLE DE TT LES SOMMETS /////////////////////
        for(auto &b : m_Sommets)
        {
            double nb_proies_temp=0;
            double pertes_totales=0;
            double croissance=0;
            /////////////// CALCULE DE LA CONSOMATION /////////////////

            for (const auto &elem1 : b.second.m_in) //loop dans vetor de id: des sommets pointe par b
            {
                double poid=0;
                if((m_Sommets[elem1].m_value>0)) //Si le nb de proies pas nul
                {

                    for (auto elem2 : m_arcs)                             //Cherhe l arc qui relie les 2 especes
                    {

                        if((elem2.second.m_to == b.first) &&(elem2.second.m_from == elem1))
                        {
                            poid = elem2.second.m_weight;                 //chope le poid de l arc
                        }
                    }
                    if (m_Sommets[elem1].m_value - (poid *b.second.m_value ) >= 0 ) // si il y a asses de proies
                    {
                        nb_proies_temp += (m_Sommets[elem1].m_value)*poid;
                    }
                    if (m_Sommets[elem1].m_value - (poid *b.second.m_value ) < 0 ) // si il y a pas asse de proies
                    {
                        nb_proies_temp +=(poid *b.second.m_value ) - m_Sommets[elem1].m_value;
                    }

                }
            }
            ////////////// CALCULE DES PERTES /////////////////////////////
            for(const auto &elem3 : b.second.m_out)
            {
                double poid=0;

                if(( m_Sommets[elem3].m_value > 0) ) //Si le nombre de predateurs et le nombre de la population est supp a 0
                {
                    for (auto elem4 : m_arcs)                             //Cherhe l arc qui relie les 2 especes
                    {
                        if((elem4.second.m_from == b.first) && (elem4.second.m_to == elem3))
                        {
                            poid = elem4.second.m_weight;
                        }

                    }
                    pertes_totales += (m_Sommets[elem3].m_value)*poid;
                }
            }
            ////////// AFFECTATIONS DES VALEURS ET BLINDAGE //////////////////////////////////
            if(nb_proies_temp>0)  //si nb proies et population non nulle
            {
                croissance = ( b.second.m_coeff_reproduction * b.second.m_value * (1 - (b.second.m_value) / (nb_proies_temp )));
                croissance = croissance * b.second.m_croissance_basic;
                b.second.m_value += croissance + b.second.m_croissance_basic;
            }
            else if(nb_proies_temp <= 0)
            {
                b.second.m_value --;
            }
            if ((b.second.m_value - pertes_totales) >= 0 )
            {
                b.second.m_value -= pertes_totales;
            }
            else if((b.second.m_value - pertes_totales) < 0 )
            {
                b.second.m_value=0;
            }

            else
            {
                croissance = b.second.m_croissance_basic;
                croissance -= pertes_totales;
                if ((nb_proies_temp <= 0)&&((b.second.m_value))>0) //Poulation positive mais plus a bouffe :'(
                {
                    b.second.m_value--;
                    if(b.second.m_value<=0)
                    {
                        b.second.m_value =0;   //meurt
                    }
                }
            }
        }
    }
    if(m_interface->m_checkbox_debeug.get_value() ==true)
    {
        m_interface->m_bouton_etude2.set_value(false);
    }

    t=tours_de_boucle++;
    rest(20);
    return t;
}

/*!
*  \brief Stop l'etude temporelle
*/
void Graph::arret_etude()
{
    if(m_interface->m_bouton_arret.get_value() ==true)
    {
        m_interface->m_bouton_etude.set_value(false);
        m_interface->m_bouton_etude2.set_value(false);
    }
    m_interface->m_bouton_arret.set_value(false);
}

///affiche les attributs des sommets du graphe + etat des piles
void Graph::affiche_tab()
{
    std::cout<<std::endl;
    for(auto &elem : m_Sommets)
    {
        std::cout<<"index S : "<<elem.first<<" "<<elem.second.m_index<<" idx_T : "<<elem.second.m_idx_T<<" decouverte : "<<elem.second.m_decouverte<<" actif : "<<elem.second.m_actif<<std::endl;

    }
    if(!m_pile_tarjan.empty())
    {
        std::cout<<" top tarjan : "<<m_pile_tarjan.top().m_index<<std::endl;
    }
    if(!m_parcours.empty())
    {
        std::cout<<"parcours top : "<<m_parcours.top().m_index<<std::endl;
    }

}

///Initialise la forte connexite ( les attributs )
void Graph :: Init_forte_connexe()
{
    for(auto &elem : m_Sommets) //Pour tous les sommets
    {
        elem.second.m_actif = true;
        elem.second.m_decouverte = -1;
        elem.second.m_idx_T = -1;
        elem.second.is_marq = false;
        elem.second.m_index = elem.first;
        elem.second.m_etat = 0;
        elem.second.m_premier = false;
        elem.second.m_color = 0;
        //std::cout<<"sommet : "<<elem.first<<" "<<elem.second.m_index<<std::endl;
    }
    //std::cout<<"Boucle arcs. Size : "<<m_arcs.size()<<std::endl;
    for(auto &elem : m_arcs)
    {
        elem.second.m_marq = false;
        //std::cout<<"arc : "<<elem.first<<"index : "<<elem.second.m_index<<" sommet de dep : "<<elem.second.m_from<<" sommet d'arrivee : "<<elem.second.m_to<<std::endl;
    }
    m_nb_forte_connexe = 0;

}

///fonction de parcours du graphe ( en profondeur  : recursif)
void Graph::fonction4(int idx_u)
{
    //std::cout<<"DEBUT fonction4 "<<std::endl;
    //std::cout<<"u : "<<idx_u<<std::endl;
    std::vector<int> liste_arcs_sortant;

    for(auto &arc : m_arcs)
    {
        if(arc.second.m_from == idx_u)
        {
            liste_arcs_sortant.push_back(arc.second.m_index);
        }
    }
    for(auto &idx_a : liste_arcs_sortant)
    {

        Arc &el = m_arcs[idx_a];
        //std::cout<<"index arc sortant trouve : "<<arc.second.m_index<<std::endl;
        //std::cout<<"indice d'arc sortant : "<<idx_a<<std::endl;

        //std::cout<<"index arc select : "<<el.m_index<<"marq : "<<el.m_marq<<std::endl;
        Sommet &v = m_Sommets[el.m_to];
        if(el.m_marq == false) //4
        {
            el.m_marq = true; //4.1


            //std::cout<<"index du sommet a la fin de l'arc : "<<v.m_index<<std::endl;
            if(v.m_etat == 1 && v.m_actif == true) //4.2
            {
                //std::cout<<"si le sommet est deja marque et est actif"<<std::endl;
                for(auto &u : m_Sommets)
                {
                    if(u.second.m_index == idx_u)
                    {
                        u.second.m_idx_T = std::min(u.second.m_idx_T, v.m_idx_T);
                        //std::cout<<" MAJ de u .idx_T : "<< u.second.m_idx_T<<std::endl;
                        break;
                    }
                }

            }
            else //4.3
            {
                //std::cout<<"si le sommet est nouveau"<<std::endl;
                m_i = m_i+1;
                v.m_decouverte = m_i;
                v.m_idx_T = v.m_decouverte;
                v.is_marq = true;
                v.m_etat = 1;
                //std::cout<<"nouveau u (normalement v) : "<<v.m_index<<std::endl;
                //std::cout<< "nouvelle valeurs. i: "<<m_i<<"v.b : "<<v.m_idx_T<<"v.l : "<<v.m_decouverte<<std::endl;
                m_pile_tarjan.push(v);
                m_parcours.push(v);
                fonction4(v.m_index);
                //std::cout<<"sortie du milieu " <<std::endl;
            }
        }
        else if(v.m_etat == 2 && v.m_actif == true)
        {
            for(auto &u : m_Sommets)
            {
                if(u.second.m_index == idx_u)
                {
                    u.second.m_idx_T = std::min(u.second.m_idx_T, v.m_idx_T);
                    //std::cout<<" MAJ de u .idx_T : "<< u.second.m_idx_T<<std::endl;
                    break;
                }
            }
        }

    }
    //std::cout<<"fin du 4 : plus d'arc sortant non marque"<<std::endl;

    //std::cout<<"u : "<<idx_u<<std::endl;
    for(auto &u : m_Sommets)
    {
        if(u.second.m_index == idx_u)
        {
            if(u.second.m_actif == true)
            {


                if(u.second.m_idx_T < u.second.m_decouverte)//5
                {
                    //std::cout<<"entree 5.2"<<std::endl;
                    u.second.m_etat = 2;
                    m_parcours.pop();
                    if(m_parcours.empty())
                    {
                        //std::cout<<" pile vide ! "<<std::endl;
                        break;
                    }
                    Sommet &w = m_parcours.top();
                    //std::cout<<"index du sommet avant dans la pile "<<w.m_index<<std::endl;
                    w.m_idx_T = std::min(w.m_idx_T,u.second.m_idx_T);
                    //std::cout<<" MAJ de l'index de u : "<<w.m_index<<std::endl;
                    //std::cout<<"Retour au 4"<<std::endl;
                    fonction4(w.m_index);
                    //std::cout<<"sortie de fonction f4 : "<<idx_u<<std::endl;
                }
                else if (u.second.m_decouverte == u.second.m_idx_T)
                {

                    m_nb_forte_connexe = m_nb_forte_connexe+1;
                    if(m_parcours.empty())
                    {
                        //std::cout<<" pile parcours vide ! "<<std::endl;
                        break;
                    }
                    std::cout<<"forte connecivite trouvee"<<std::endl;
                    //std::cout<<"u : "<<u.second.m_index<<std::endl;
                    std::cout<<"succeceurs : "<<m_parcours.top().m_index<<"; ";
                    for(auto &elem : m_Sommets)
                    {

                        if(elem.first == m_parcours.top().m_index)
                        {
                            //std::cout<<"passer ! "<<std::endl;
                            elem.second.m_actif = false;
                            elem.second.is_marq = true;
                            elem.second.m_color = m_nb_forte_connexe;
                            elem.second.m_premier = true;
                        }
                    }
                    while(m_pile_tarjan.top().m_index != m_parcours.top().m_index)
                    {
                        std::cout<<m_pile_tarjan.top().m_index<<"; ";
                        for(auto &elem : m_Sommets)
                        {
                            if(elem.first == m_pile_tarjan.top().m_index)
                            {
                                //std::cout<<"passer ! "<<std::endl;
                                elem.second.m_actif = false;
                                elem.second.is_marq = true;
                                elem.second.m_color = m_nb_forte_connexe;
                            }
                        }
                        m_pile_tarjan.pop();

                    }
                    m_pile_tarjan.pop();
                    m_parcours.pop();
                }
            }
            break;
        }
    }
    //affiche_tab();

}

///Recherche toutes les composantes fortements connexes d'un graphe -- graphe 4
void Graph::cherche_forte_connexe()
{

    if(m_interface->m_bouton_connexe.get_value() == true)
    {
        m_i = 0;
        //std::cout<<"Cliquer"<<std::endl;
        //std::cout<<"Boucle sommets. Size : "<<m_Sommets.size()<<std::endl;

        Init_forte_connexe();
        std::cout<<"NO problemo pour initialiser"<<std::endl;

        for(auto &elem : m_Sommets) //1
        {
            //std::cout<<std::endl;
            //std::cout<< "sommet index : "<<elem.first<<" "<<elem.second.m_index<<" actif : "<<elem.second.m_actif<<" marq : "<<elem.second.is_marq<<std::endl;

            if(elem.second.is_marq == false)//2
            {
                //std::cout<< "s non marque : initialisation de s"<<std::endl;

                m_i = m_i+1;//3
                //std::cout<<"i : "<<m_i<<std::endl;
                elem.second.m_decouverte = m_i;
                elem.second.m_idx_T = m_i;


                elem.second.m_etat = 1; //U est en traitement
                m_pile_tarjan.push(elem.second);
                m_parcours.push(elem.second);
                /*std::cout<< "ok : index "<<elem.second.m_index<<"decouverte : "<<elem.second.m_decouverte<<"idx_T : "<<elem.second.m_idx_T<<std::endl;
                std::cout<<"pour chaque arcs sortant : taille "<<elem.second.m_out.size()<<std::endl;
                std::cout<<"index de u : "<<elem.second.m_index<<std::endl;*/


                fonction4(elem.second.m_index);
                //std::cout<<" u : "<<u.m_index<<std::endl;
                //std::cout<<" ICI Sortie def de fonction 4"<<std::endl;
                //affiche_tab();

            }//fin if sommet non marque


        }//fin for chaque sommet
        std::cout<<"FIN"<<std::endl;
    }//fin if bouton

    m_interface->m_bouton_connexe.set_value(false);

}

///Affiche le graphe reduit
void Graph ::affiche_graphe_reduit()
{
    if(m_interface->m_bouton_graphe_red.get_value() == true)
    {
        int choix;
        std::cout<<"Voulez vous enregistrer votre graphe ? : 1 pour oui, 0 pour non : ";
        std::cin>>choix;
        while(choix !=0 && choix!=1)
        {
            std::cout<<"Vous avez entre un mauvais numero, veuillez retaper SVP ? : 1 pour oui, 0 pour non : ";
            std::cin>>choix;
        }
        if(choix == 1)
        {
            sauvegarde_graph_2();
        }
        int nb = 0;
        for(auto &elem : m_Sommets)
        {
            if(elem.second.m_premier == true)
            {
                nb = nb+1;
            }
        }
        m_arcs.erase(m_arcs.begin(),m_arcs.end());
        m_Sommets.erase(m_Sommets.begin(), m_Sommets.end());
        m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
        for(int i = 0; i<nb; i++)
        {add_interfaced_vertex(i,0,100,200,"");}
        //std::cout<<"sommets creer"<<std::endl;
        add_interfaced_edge(0,0,1,0);
        //std::cout<<"arc creer"<<std::endl;
    }
    m_interface->m_bouton_graphe_red.set_value(false);
}


