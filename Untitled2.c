void control(nhan_vat &player, nhan_vat &k, nhan_vat enemy[],SDL_Event &e, bool &is_play, bool &pause)
{
    switch(e.key.keysym.sym)
    {
        case SDLK_UP:
        {
            Mix_PlayChannel(-1, gJump, 0);
            player.status = 0;
            player.speed_y = 42;
            while(player.speed_y > 0 && is_play == true && pause == false)
            {
                is_play = !check_collision(player, k);
                player.y -= player.speed_y;
                player.speed_y -= 4;
                gBackgroundTexture.render(0,0,NULL);
                gScoreTexture.loadFromRenderedText(trans(score).c_str(), textColor);
                gScoreTexture.render(SCREEN_WIDTH-100, 0,&_SCORE);
                //enemy_generation(obstacles, enemy);
                /*int x = obstacles.size();
                for(int i = 0; i < x; i++)
                {
                    obstacles[i].auto_move(score);
                }*/
                rng(k, enemy);
                player.animation( 6);
                SDL_Delay(50);
                score++;
            }
            while(player.y < 470 && is_play == true && pause == false)
            {
                is_play = !check_collision(player, k);
                player.speed_y += 6;
                player.y += player.speed_y;
                gBackgroundTexture.render(0,0,NULL);
                gScoreTexture.loadFromRenderedText(trans(score).c_str(), textColor);
                gScoreTexture.render(SCREEN_WIDTH-100, 0,&_SCORE);
                rng(k, enemy);
                player.animation(6);
                SDL_Delay(50);
                score++;

            }
            break;
        }
        case SDLK_SPACE:
        {
            if(pause == false)
            pause = true;
            if(pause == true)
            pause = false;
            break;
        }
        case SDLK_DOWN:
        {
            if(is_play == true && pause == false)
            {
                player.status = 1;
                player.y += 30;
                is_play = !check_collision(player, k);
            }
            break;
        }
        default:
        break;
    }
}
