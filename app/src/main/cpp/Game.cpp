#include "Game.hpp"

Game::Game(Vector2 const& ScreenDims) : inGame(false),
                                        player(ScreenDims),
                                        startButton(ScreenDims.x / 2, ScreenDims.y / 2, 150, 150, "Start")
{
    textures.loadAll();
    statsMobs = LoadAllMobStats();
}

void Game::actualize(const float dt)
{
    if (inGame) // Si on est en partie
    {
        checkSpawn();               // Check apparition des ennemis
        player.actualize(dt);       // Actions du joueur (tirer, changer d'arme), déplacement et animation des balles
        checkCollisionsTirs();      // Check des collisions entre les balles et les ennemis

        for (int i = 0; i < ennemies.size(); ++i)
            ennemies[i]->actualize(player, dt);   // Actualise les attaques et les animations des ennemis
    }
    else    // On est sur le menu
    {
        if (startButton.isPressed())
        {
            level.loadLvl(1);
            inGame = true;
            clockLevel.restart();
        }
    }
}

void Game::Draw()
{
    if (inGame)
    {
        player.Draw(true);

        for (int i = 0; i < ennemies.size(); ++i)
            ennemies[i]->Draw(false);
    }
    else
    {
        startButton.Draw();
    }
}

void Game::checkSpawn()
{
    /*
     * get<0> est le float de l'horaire de spawn
     * get<1> est l'int de la quantité à spawn
     * get<2> est la string du type de mob
     */
    if (level.ended())
        return;

    if (clockLevel.getElapsedTime() > level.getNextTimeCode())
    {
        for (int i = 0; i < level.getNextQuantity(); ++i)
        {
            string newType(level.getNextType());

            if (newType == "Ether_black")
                ennemies.push_back(new EtherBlack(textures.texEtherBlack, statsMobs[newType]));
            else if (newType == "Ether_blue")
                ennemies.push_back(new EtherBlue(textures.texEtherBlue, statsMobs[newType]));
            else if (newType == "Ether_gold")
                ennemies.push_back(new EtherGold(textures.texEtherGold, statsMobs[newType]));
            else
            {
                LOGE("Error Game.cpp : unknown mob level : %d, type : %s", level.numlvl, newType.c_str());
                exit(1);
            }
        }
        level.nextStep();

        // ----- Check fin niveau ---------
        if (level.ended())
        {
            //  --------- Niveau vient de se terminer ----------
        }
    }
}

void Game::checkCollisionsTirs()
{
    for (int i = 0; i < player.balles.size(); ++i)  // Utiliser .size() pour la lisibilité
    {
        bool balleSupprimee = false;  // Marqueur pour savoir si la balle a été supprimée
        for (int j = 0; j < ennemies.size(); ++j)
        {
            if (collisionCercleCercle(player.balles[i]->colCircle, ennemies[j]->colCircle))
            {
                // Si l'ennemi est touché et meurt
                if (ennemies[j]->blesser(player.balles[i]) == true)
                    removeElement(ennemies, j);   // Supprime l'ennemi

                // Supprimer la balle
                removeElement(player.balles, i);  // Supprime la balle à l'index i
                balleSupprimee = true;            // Marquer que la balle est supprimée
                --i;                              // Ajuster l'indice i pour ne pas sauter d'éléments

                // Sortir de la boucle car on ne veut pas continuer à vérifier d'autres ennemis
                break;
            }
        }

        // Si la balle a été supprimée, il faut arrêter l'itération sur les balles
        if (balleSupprimee)
        {
            break;
        }
    }
}

