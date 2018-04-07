int variable_globale_inch_allah_sa_marche=0;

#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
SommetInterface::SommetInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
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


/// Gestion du Sommet avant l'appel à l'interface
void Sommet::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Sommet après l'appel à l'interface
void Sommet::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
    m_coordx = m_interface->m_top_box.get_posx();
    m_coordy = m_interface->m_top_box.get_posy();
}



/***************************************************
                    ARC
****************************************************/


/// Le constructeur met en place les éléments de l'interface
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


    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Arc avant l'appel à l'interface
void Arc::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );

    ///Copier l'index dans le label d'index
    m_interface->m_label_index.set_message(std::to_string( (int)m_index ));
}

/// Gestion du Arc après l'appel à l'interface
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

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
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

    //troisieme bouton pour ajouter
    m_tool_box.add_child(m_bouton_ajout);
    m_bouton_ajout.set_frame(3,58,80,20);
    m_bouton_ajout.set_bg_color(VERTCLAIR);
    m_bouton_ajout.add_child(m_bouton_label_ajout);
    m_bouton_label_ajout.set_message("Ajouter");

    //quatrieme bouton pour supprimer sommet
    m_tool_box.add_child(m_bouton_suppr_s);
    m_bouton_suppr_s.set_frame(3,83,80,40);
    m_bouton_suppr_s.set_bg_color(VERTCLAIR);
    m_bouton_suppr_s.add_child(m_bouton_label_suppr_s);
    m_bouton_label_suppr_s.set_message("Supprimer sommet");

    //bouton pour supprimer arc
    m_tool_box.add_child(m_bouton_suppr_a);
    m_bouton_suppr_a.set_frame(3,128,80,40);
    m_bouton_suppr_a.set_bg_color(VERTCLAIR);
    m_bouton_suppr_a.add_child(m_bouton_label_suppr_a);
    m_bouton_label_suppr_a.set_message("Supprimer arc");


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

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
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

/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_Sommets.find( variable_globale_inch_allah_sa_marche)!= m_Sommets.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    SommetInterface *vi = new SommetInterface( variable_globale_inch_allah_sa_marche, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de somet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_Sommets[idx] = Sommet(value, vi);
     variable_globale_inch_allah_sa_marche++;
}


/// ajout de sommets
void Graph::add_interfaced_Sommet(int idx, double value, int x, int y, std::string path, int pic_idx )
{

    std::string path_txt;
    std::string name;
    std::string temp;
    int choix;




    if ( m_interface->m_bouton_ajout.get_value()== true)
    {
        while(choix!=1 && choix!=2 && choix !=3) //blindage choix
        {
            std::cout<<"entrer le graphe que vous voulez (1,2 ou 3) pour l'ajout de sommet : ";
            std::cin>>choix;
            if(choix==1)
            {
                path_txt = "ressources/graphe1/Noms_Dino.txt";
            }
            if(choix==2)
            {
            path_txt = "ressources/graphe2/Noms_graphe2";
            }
            if(choix ==3)
            {
                path_txt = "ressources/graphe3/Noms_Sib.txt";
            }
        }
        std::ifstream fichier (path_txt,std::ios::in);
        if(fichier)
        {
            std::string line;
            std::cout<<"Voici la liste de sommets disponible a ajouter dans ce graph:"<<std::endl;
            while(getline(fichier, line))
                {
                    std::cout<<line<<std::endl;
                }

        }
        else  // sinon
                {std::cerr << "Impossible d'ouvrir le fichier ici !" << std::endl;}
                std::cout<<"Chosir stp"<<std::endl;
            std::cin>> name;
            name+=".jpg";
            add_interfaced_vertex(variable_globale_inch_allah_sa_marche,value,x,y,name,pic_idx);
    }
 m_interface->m_bouton_ajout.set_value(false);
}


/// Aide à l'ajout d'arcs interfacés
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

    ArcInterface *ei = new ArcInterface(m_Sommets[id_vert1], m_Sommets[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_arcs[idx] = Arc(weight, ei);
    m_arcs[idx].m_from = id_vert1;
    m_arcs[idx].m_to = id_vert2;
    m_Sommets[id_vert1].m_out.push_back(idx);
    m_Sommets[id_vert2].m_in.push_back(idx);
}

///sert a savoir si le bouton charge est appuyé, si oui, on lance le chargement, en effacant toutes les données existantes
void Graph::is_charge()
{

    if(m_interface->m_bouton_charge.get_value()==true)
    {

        m_arcs.erase(m_arcs.begin(),m_arcs.end());
        m_Sommets.erase(m_Sommets.begin(), m_Sommets.end());
        charge_file(true);
    }
}

///charge un graphe a partir d'un fichier
void Graph::charge_file(bool charge)
{
        std::vector<std::string> aretes_tmp;
        std::string sousChaine;
        std::string str_tmp;
        int poids;
        int m1;
        int m2;
        int value;
        int coordsx;
        int coordsy;
        int choix = 0;
        std::string path;



        while(choix!=1 && choix!=2 && choix !=3)
        {
            std::cout<<"entrer le graphe que vous voulez (1,2 ou 3) pour le chargement du graphe : ";
            std::cin>>choix;
            if(choix == 1)
            {
                path = "ressources/graphe1/Graphe_Dino.txt";
            }
            if(choix == 2)
            {
            path = "ressources/graphe2/Graphe_Prehi.txt";
            }
            if(choix == 3)
            {
                path = "ressources/graphe3/Graphe_Sib.txt";
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
                else { std::cerr << "Votre fichier ne contient pas un ordre correct !" << std::endl; }

                /// Remplissage du vecteur de sommet
                // Les sommets doivent être définis avant les arcs
                // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
                for(int i=0; i<m_ordre; i++) // On renseigne le nom des sommets par le nom des personnes du fichier
                {
                    getline(fichier,line);
                    std::stringstream ss(line);
                    while(getline(ss,sousChaine,'/'))
                        {
                            aretes_tmp.push_back(sousChaine);
                        }

                    str_tmp = aretes_tmp.back();
                    coordsy = atoi(str_tmp.c_str());
                    aretes_tmp.pop_back();
                    str_tmp = aretes_tmp.back();
                    coordsx = atoi(str_tmp.c_str());
                    aretes_tmp.pop_back();
                    str_tmp = aretes_tmp.back();
                    value = atoi(str_tmp.c_str());
                    aretes_tmp.pop_back();
                    nom_sommet_temp = aretes_tmp.back();;
                    //add_interfaced_vertex(i, value, coordsx, coordsy, nom_sommet_temp+".bmp");
                    add_interfaced_vertex(i, value, coordsx, coordsy, nom_sommet_temp+".jpg");
                    m_Sommets[i].m_nom = nom_sommet_temp;
                    m_Sommets[i].m_coordx = coordsx;
                    m_Sommets[i].m_coordy = coordsy;
                }

                std::cout<<aretes_tmp.size()<<std::endl;
                std::cout<<m_Sommets.size()<<std::endl;
                for (int i =0; i<m_ordre; i++)
                {
                    std::cout<<m_Sommets.find(i)->first<<" "<<m_Sommets.find(i)->second.m_nom<<std::endl;
                }

                //on charge le nb d'arete
                getline(fichier, line); // On lit la première ligne qui est un entier représentant l'ordre
                if(atoi(line.c_str())) // Si la premiere ligne coreespond à un nombre reel
                {
                    m_nb_arete = atoi(line.c_str()); //converti la chaine de caractere recu en integer
                }
                else { std::cerr << "Votre fichier ne contient pas un ordre correct !" << std::endl; }

                ///on charge les aretes et on remplit le vecteur d'arete
                /// Les arcs doivent être définis entre des sommets qui existent !
                // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
                for(int i=0; i<m_nb_arete; i++) //pour toutes les autres lignes. On lit le contenu, on le divise en plusieurs parties
                {                               //puis on met les informations recoltees dans leurs variables. extremite1, extremite2, poids
                    getline(fichier,line);
                    std::stringstream ss(line);
                    while(getline(ss,sousChaine,'/'))
                        {
                            aretes_tmp.push_back(sousChaine);
                        }

                    str_tmp = aretes_tmp.back();
                    poids = atoi(str_tmp.c_str());
                    aretes_tmp.pop_back();
                    str_tmp = aretes_tmp.back();
                    m2 = atoi(str_tmp.c_str());
                    aretes_tmp.pop_back();
                    str_tmp = aretes_tmp.back();
                    m1 = atoi(str_tmp.c_str());
                    add_interfaced_edge(i, m1, m2, poids);
                    m_arcs[i].m_index = i;
                } //fin for
            } //fin if fichier
            else  // sinon
                {std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;}
        }//fin if bool
        m_interface->m_bouton_charge.set_value(false);
}

///sauvegarde un graphe, les positions, etc...
void Graph::sauvegarde_graph()
{
    if(m_interface->m_bouton_sauv.get_value() == true)
    {
    std::string path;
    std::cout<<"entrer le chemin pour enregistrer votre graphe : ";
    std::cin>>path;
    std::ofstream fichier(path, std::ios::out | std::ios::trunc);  //déclaration du flux et ouverture du fichier
    if(fichier)
            {
                fichier <<m_ordre<<std::endl;
                for(unsigned int i = 0; i<m_Sommets.size(); i++)
                {
                    fichier<<m_Sommets[i].m_nom <<"/"<<m_Sommets[i].m_value<<"/"<<m_Sommets[i].m_coordx<<"/"<<m_Sommets[i].m_coordy<<std::endl;
                    std::cout<<m_Sommets[i].m_nom <<"/"<<m_Sommets[i].m_value<<"/"<<m_Sommets[i].m_coordx<<"/"<<m_Sommets[i].m_coordy<<std::endl;
                }
                fichier<<m_nb_arete<<std::endl;
                std::cout<<m_arcs[0].m_from<<" "<<m_arcs[0].m_to<<std::endl;
                for(int j = 0; j<m_nb_arete; j++)
                {
                    fichier<<m_arcs[j].m_from<<"/"<<m_arcs[j].m_to<<"/"<<m_arcs[j].m_weight<<std::endl;
                    std::cout<<m_arcs[j].m_from<<"/"<<m_arcs[j].m_to<<"/"<<m_arcs[j].m_weight<<std::endl;
                }
                fichier.close();
            }
            else
                {std::cerr << "Impossible d'ouvrir le fichier !" << std::endl;}
    }


    m_interface->m_bouton_sauv.set_value(false);
}


bool Graph::quitter()
{
        if(m_interface->m_bouton_quit.get_value()== true)
        {
            return true;
        }
        return false;
}

///supprime un arc
void Graph::suppr_arc()
{
    int idx_arc;
    if(m_interface->m_bouton_suppr_a.get_value()==true)
    {

        std::cout<<"entrer l'index de l'arc que vous voulez supprimer : ";
        std::cin>> idx_arc;
        //ref sur l'arc a enlever
        Arc & arc_sup = m_arcs.at(idx_arc);
        std::cout<< "arc a enlever : "<<idx_arc<<" "<<arc_sup.m_from<<" -> "<<arc_sup.m_to<<"  "<<arc_sup.m_weight<<std::endl;

        std::cout<<m_Sommets[arc_sup.m_from].m_in.size()<< " "<<m_Sommets[arc_sup.m_from].m_out.size()<< " "<<m_Sommets[arc_sup.m_to].m_in.size()<< " "<<m_Sommets[arc_sup.m_to].m_out.size()<<std::endl;
        std::cout<< m_arcs.size()<<std::endl;

        if(m_interface && arc_sup.m_interface) //on efface dans l'interface
        {
            m_interface->m_main_box.remove_child(arc_sup.m_interface->m_top_edge);
        }

        //on efface dans les sommets
        std::vector<int> &vefrom = m_Sommets[arc_sup.m_from].m_out; //
        std::vector<int> &veto = m_Sommets[arc_sup.m_to].m_in;
        vefrom.erase(std::remove(vefrom.begin(),vefrom.end(),idx_arc),vefrom.end());
        veto.erase(std::remove(veto.begin(),veto.end(),idx_arc),veto.end());
        m_arcs.erase(idx_arc);

        std::cout<<m_Sommets[arc_sup.m_from].m_in.size()<< " "<<m_Sommets[arc_sup.m_from].m_out.size()<< " "<<m_Sommets[arc_sup.m_to].m_in.size()<< " "<<m_Sommets[arc_sup.m_to].m_out.size()<<std::endl;
        std::cout<< m_arcs.size()<<std::endl;
    }
    m_interface->m_bouton_suppr_a.set_value(false);

}

///supprime un sommet
void Graph::suppr_sommet()
{
    int idx_s;
    if(m_interface->m_bouton_suppr_s.get_value()==true)
    {

        std::cout<<"entrer l'index du sommet que vous voulez supprimer ( il faut supprimer les arcs qui le relie avant) : ";
        std::cin>> idx_s;

        Sommet & somm_sup = m_Sommets.at(idx_s);
        std::cout<< "sommet a enlever : "<<idx_s<<" "<<somm_sup.m_nom<<"  "<<somm_sup.m_value<<std::endl;

        if(m_interface && somm_sup.m_interface)
        {
            m_interface->m_main_box.remove_child(somm_sup.m_interface->m_top_box);
        }
        m_Sommets.erase(idx_s);
    }
    m_interface->m_bouton_suppr_s.set_value(false);
}


