
void Graph::affiche_tab()
{
    std::cout<<std::endl;
    for(auto &elem : m_Sommets)
    {
        std::cout<<"index S : "<<elem.first<<" "<<elem.second.m_index<<" idx_T : "<<elem.second.m_idx_T<<" decouverte : "<<elem.second.m_decouverte<<" actif : "<<elem.second.m_actif<<std::endl;

    }
}

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
            //elem.second.m_nb_arc_sortant = elem.second.m_in.size();
            //elem.second.m_pred = -1;
            std::cout<<"sommet : "<<elem.first<<" "<<elem.second.m_index<<std::endl;
        }
        std::cout<<"Boucle arcs. Size : "<<m_arcs.size()<<std::endl;
        for(auto &elem : m_arcs)
        {
            elem.second.m_marq = false;
            std::cout<<"arc : "<<elem.first<<"index : "<<elem.second.m_index<<" sommet de dep : "<<elem.second.m_from<<" sommet d'arrivee : "<<elem.second.m_to<<std::endl;
        }

}

Sommet Graph::fonction4(Sommet &u)
{
    std::cout<<"DEBUT fonction4 "<<std::endl;
    //affiche_tab();
    std::cout<<"u : "<<u.m_index<<std::endl;
    std::vector<int> liste_arcs_sortant;

    for(auto &arc : m_arcs)
    {
        if(arc.second.m_from == u.m_index)
        {
            liste_arcs_sortant.push_back(arc.second.m_index);
        }
    }
    for(auto &idx_a : liste_arcs_sortant)
    {

        Arc &el = m_arcs[idx_a];
        //std::cout<<"index arc sortant trouve : "<<arc.second.m_index<<std::endl;
        std::cout<<"indice d'arc sortant : "<<idx_a<<std::endl;

        std::cout<<"index arc select : "<<el.m_index<<"marq : "<<el.m_marq<<std::endl;

        if(el.m_marq == false) //4
        {
            el.m_marq = true; //4.1

            Sommet &v = m_Sommets[el.m_to];
            std::cout<<"index du sommet a la fin de l'arc : "<<v.m_index<<std::endl;
            if(v.m_etat == 1 && v.m_actif == true) //4.2
            {
                std::cout<<"si le sommet est deja marque et est actif"<<std::endl;
                u.m_idx_T = std::min(u.m_idx_T, v.m_idx_T);
                std::cout<<" MAJ de u .idx_T : "<<u.m_idx_T<<std::endl;
            }
            else //4.3
            {
                std::cout<<"si le sommet est nouveau"<<std::endl;
                m_i = m_i+1;
                v.m_decouverte = m_i;
                v.m_idx_T = v.m_decouverte;
                //u.m_succ = v.m_index;
                v.is_marq = true;
                v.m_etat = 1;
                std::cout<< "nouvelle valeurs. i: "<<m_i<<"v.b : "<<v.m_idx_T<<"v.l : "<<v.m_decouverte<<std::endl;
                u = v;
                std::cout<<"nouveau u (normalement v) : "<<u.m_index<<std::endl;
                m_pile_tarjan.push(u);
                m_parcours.push(u);
                u = fonction4(u);
                //fonction4(v);
                return u;
            }
        }

    }
    std::cout<<"fin du 4 : plus d'arc sortant non marque"<<std::endl;
    //std::cout<<"sortie du 4. u = "<<u.m_index<<std::endl;
    //affiche_tab();
    std::cout<<"u : "<<u.m_index<<std::endl;
    if(u.m_idx_T < u.m_decouverte)//5
    {
        //boucle = true;
        std::cout<<"entree 5.2"<<std::endl;
        u.m_etat = 2;
        m_parcours.pop();
        Sommet &w = m_parcours.top();
        std::cout<<"index du sommet avant dans la pile "<<w.m_index<<std::endl;
        w.m_idx_T = std::min(w.m_idx_T,u.m_idx_T);
        u = w;
        std::cout<<" MAJ de l'index de u : "<<u.m_index<<std::endl;
        std::cout<<"Retour au 4"<<std::endl;
         u = fonction4(u);
         //fonction4(w);
    }
    else if (u.m_decouverte == u.m_idx_T)
    {
        //boucle = false;
        std::cout<<"u : "<<u.m_index<<std::endl;
        std::cout<<"forte connecivite trouvee"<<std::endl;
        std::cout<<"succeceurs : "<<m_parcours.top().m_index<<"; ";
        for(auto &elem : m_Sommets)
        {

            if(elem.first == m_parcours.top().m_index)
            {
                std::cout<<"passer ! "<<std::endl;
                elem.second.m_actif = false;
                elem.second.is_marq = true;
            }
        }
        //m_parcours.top().m_actif = false;
        while(m_pile_tarjan.top().m_index != m_parcours.top().m_index)
        {
            std::cout<<m_pile_tarjan.top().m_index<<"; ";
            for(auto &elem : m_Sommets)
            {
                if(elem.first == m_pile_tarjan.top().m_index)
                {
                    std::cout<<"passer ! "<<std::endl;
                    elem.second.m_actif = false;
                    elem.second.is_marq = true;
                }
            }
            //m_pile_tarjan.top().m_actif = false ;
            m_pile_tarjan.pop();
        }
    /*while(!m_pile_tarjan.empty()) //on vide les piles
    {
        m_pile_tarjan.pop();
    }
    while(!m_parcours.empty())
    {
        m_parcours.pop();
    }*/
    affiche_tab();

    /*if( !m_pile.empty())
        {u = m_pile.top();}*/

        //std::cout<<"successeur de u "<<u.m_succ<<std::endl;
        //return u;
    }
    //m_pile.pop();
    /*if( !m_pile.empty())
        {u = m_pile.top();}*/
    //fin 4
    /*while(!m_pile_tarjan.empty()) //on vide les piles
    {
        m_pile_tarjan.pop();
    }
    while(!m_parcours.empty())
    {
        m_parcours.pop();
    }*/
    std::cout<< "u a la fin de 4 : "<<u.m_index<<std::endl;
    return u;
}

void Graph::cherche_forte_connexe()
{
    //initialisation
    //int i = 0;
    //std::stack<Sommet> pile;

    //int idx_a;
    //std::vector<
    //std::vector<Sommet> successeurs;


    if(m_interface->m_bouton_connexe.get_value() == true)
    {
        m_i = 0;
        std::cout<<"Cliquer"<<std::endl;
        std::cout<<"Boucle sommets. Size : "<<m_Sommets.size()<<std::endl;

        Init_forte_connexe();
        std::cout<<"NO problem pour initialiser"<<std::endl;

        for(auto &elem : m_Sommets) //1
        {
            std::cout<<std::endl;
            std::cout<< "sommet index : "<<elem.first<<" "<<elem.second.m_index<<" actif : "<<elem.second.m_actif<<" marq : "<<elem.second.is_marq<<std::endl;

            if(elem.second.is_marq == false)//2
            {
                std::cout<< "s non marque : initialisation de s"<<std::endl;

                //elem.second.m_pred = elem.second.m_index;
                m_i = m_i+1;//3
                std::cout<<"i : "<<m_i<<std::endl;
                elem.second.m_decouverte = m_i;
                elem.second.m_idx_T = m_i;
                //elem.second.m_nidx = elem.second.m_index;

                Sommet & u = elem.second;

                //u.is_marq = true;
                u.m_etat = 1; //U est en traitement
                //m_pile_tarjan.push(u); //on met u dans les 2 piles
                m_pile_tarjan.push(u);
                //m_parcours.push(u);
                m_parcours.push(u);
                std::cout<< "ok : index "<<u.m_index<<"decouverte : "<<u.m_decouverte<<"idx_T : "<<u.m_idx_T<<std::endl;
                //for(auto &el : m_arcs)
                std::cout<<"pour chaque arcs sortant : taille "<<u.m_out.size()<<std::endl;
                std::cout<<"index de u : "<<u.m_index<<std::endl;


                 u = fonction4(u);
                //std::cout<<" u : "<<u.m_index<<std::endl;
                /*while(!m_pile.empty())
                {
                     fonction4(u);
                }*/
                //std::cout<<" ICI Sortie def de fonction 4"<<std::endl;
                affiche_tab();

            }//fin if sommet non marque
            /*else if(elem.second.m_actif == true)
            {

                elem.second.m_decouverte = m_i;
                elem.second.m_idx_T = m_i;
                Sommet &u = elem.second;

                m_pile_tarjan.push(u);
                //m_parcours.push(u);
                m_parcours.push(u);
                std::cout<< "ok : index "<<u.m_index<<"decouverte : "<<u.m_decouverte<<"idx_T : "<<u.m_idx_T<<std::endl;
                //for(auto &el : m_arcs)
                std::cout<<"pour chaque arcs sortant : taille "<<u.m_out.size()<<std::endl;
                std::cout<<"index de u : "<<u.m_index<<std::endl;
                u = fonction4(u);

            }*/
            for(auto &elem : m_Sommets)
            {
                if(elem.second.m_actif == false)
                {
                    elem.second.m_color = 1;
                }
            }


        }//fin for chaque sommet
        std::cout<<"FIN"<<std::endl;
    }//fin if bouton

    m_interface->m_bouton_connexe.set_value(false);

}

