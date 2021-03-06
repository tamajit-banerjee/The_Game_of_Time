#include "game.h"

void Game::init(SDL_Renderer *arg_renderer, TTF_Font *arg_font )
{

	renderer = arg_renderer;
    font = arg_font;

    isRunning = true;

    // loadTexture("player", "resources/player.bmp");
    loadTexture("player", "assets/resources/players_combined.bmp");
    loadTexture("monster", "assets/resources/monster.bmp");
    loadTexture("maze", "assets/resources/maze.bmp");
    loadTexture("coin", "assets/resources/coins.bmp");
    loadTexture("time", "assets/resources/time.bmp");
    loadTexture("bullet", "assets/resources/bullet.bmp");
    loadTexture("periscope", "assets/resources/periscope.bmp");
    loadTexture("freeze", "assets/resources/players_freeze.bmp");

    sPlayer.playerId = 1;
    sPlayer.player_no = 1;

    cPlayer.playerId = 1;
    cPlayer.player_no = 2;

    sounds.init();

}

void Game::update(){

    gameTime++;

    srand( seedi + sPlayer.get_time() + cPlayer.get_time() );

    if(gameTime%WALL_REMOVE_TIME == 0){
        random_wall_removal();
        maze_dist_update();
    }

    updateMonsters();
    handleMonsterCollisions();

    updateBullets(sPlayer);
    updateBullets(cPlayer);

    Bullet_hit_Player();

    checkCoinTimeEat();

    updateTime();
    
    handlePlayerActivities(sPlayer);
    handlePlayerActivities(cPlayer);

}

void Game::render(){
    SDL_RenderClear(renderer);

    renderMaze();

    render_bullets();

    for(int i = 0 ; i<MONSTERS; i++){
        monsters[i].draw(renderer, font);
    }

    sPlayer.draw(renderer, font);
    cPlayer.draw(renderer, font);

    if(level >= 3)
        renderPeriscope();

    sPlayer.dispName(renderer, font, 100, -SCORE_DISPLAY_HEIGHT + SCORE_DISPLAY_HEIGHT/4);
    sPlayer.dispScore(renderer, font, 50, -SCORE_DISPLAY_HEIGHT + 2 * SCORE_DISPLAY_HEIGHT/4);
    sPlayer.dispTime(renderer, font, 150, -SCORE_DISPLAY_HEIGHT + 2 * SCORE_DISPLAY_HEIGHT/4);


    cPlayer.dispName(renderer, font, SCREEN_WIDTH - 100 - 80, -SCORE_DISPLAY_HEIGHT + SCORE_DISPLAY_HEIGHT/4);
    cPlayer.dispScore(renderer, font, SCREEN_WIDTH - 150 - 80, -SCORE_DISPLAY_HEIGHT + 2 * SCORE_DISPLAY_HEIGHT/4);
    cPlayer.dispTime(renderer, font, SCREEN_WIDTH - 50 - 80, -SCORE_DISPLAY_HEIGHT + 2* SCORE_DISPLAY_HEIGHT/4);

    SDL_RenderPresent(renderer);
}

void Game::loadTexture(char *textName, char *path){
    SDL_Surface* tmpSurface;
    if(strcmp(textName, "player") == 0){
        tmpSurface = SDL_LoadBMP(path);
        cPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        sPlayer.Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "monster") == 0){
        tmpSurface = SDL_LoadBMP(path);
        for(int i = 0; i<MONSTERS; i++){
            monsters[i].Tex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        }
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "maze") == 0){
        tmpSurface = SDL_LoadBMP(path);
        mazeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "coin") == 0){
        tmpSurface = SDL_LoadBMP(path);
        coinTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "time") == 0){
        tmpSurface = SDL_LoadBMP(path);
        timeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "bullet") == 0){
        tmpSurface = SDL_LoadBMP(path);
        bullet = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "periscope") == 0){
        tmpSurface = SDL_LoadBMP(path);
        periTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }
    else if(strcmp(textName, "freeze") == 0){
        tmpSurface = SDL_LoadBMP(path);
        sPlayer.freezeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        cPlayer.freezeTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);
    }

}








void Game::levelStart(int arg_level , int seedx ){

    level = arg_level;

    seedi = seedx;

    for(int i = 0; i<SLEEP_UNIT; i++){
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        disp_text(renderer, "Level: ", font, 290, 220);
        std::string temp_str = std::to_string(level);
        char* char_type = (char*) temp_str.c_str();
        disp_text(renderer, char_type, font, 340, 220);
        SDL_RenderPresent(renderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        if(toQuit()){
            isRunning = false;
            return;
        }

    }
    sounds.play("clock", true, 20);

    srand(seedi);
    gameTime = 0;
    cPlayer.init();
    sPlayer.init();
    cPlayer.set_time(LEVEL_TIME);
    sPlayer.set_time(LEVEL_TIME);
    cPlayer.freeze = false;
    cPlayer.final_freeze = false;
    sPlayer.freeze = false;
    sPlayer.final_freeze = false;
    int random_i = std::rand() % int(MAZEROWS/3);
    int random_j = std::rand() % int(MAZECOLS/3);
    
    sPlayer.setPosCenter(random_i, random_j);
    cPlayer.setPosCenter(MAZEROWS - 1 - random_i, MAZECOLS - 1 - random_j);

    mazeInit();
    maze_gen();
    maze_dist_update();


    initMonsters();

    isLevelRunning = true;
    

}

void Game::levelEnd()
{
    sounds.stop();
    sounds.play("level_end");

    for(int i = 0; i<SLEEP_UNIT; i++){
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        disp_text(renderer, "Results", font, 300, 140);
        
        sPlayer.dispName(renderer, font, 250, 200);
        sPlayer.dispScore(renderer, font, 350, 200);

        cPlayer.dispName(renderer, font, 250, 250);
        cPlayer.dispScore(renderer, font, 350, 250);

        SDL_RenderPresent(renderer);

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        if(toQuit()){
            isRunning = false;
            return;
        }
    }

}

void Game::handleEvents()
{
	
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
            isRunning = false;
        if(isServer)
            sPlayer.handleKeyDown(event.key.keysym.sym);
        else
            cPlayer.handleKeyDown(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        if(isServer)
            sPlayer.handleKeyUp(event.key.keysym.sym);
        else
            cPlayer.handleKeyUp(event.key.keysym.sym);
        break;
	default:
		break;
	}
}

bool Game::toQuit(){
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        switch (e.type){
            case SDL_QUIT :
                return true;
            case SDL_KEYDOWN:
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    return true;
            default:
                return false;
        }
    }
    return false;
}