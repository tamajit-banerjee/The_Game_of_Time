#include "game.h"

Player::Player(){
    name = "Not Entered yet!";
    xpos = 0, ypos = 0;
    old_xpos = 0, old_ypos = 0;
    score = 0;
    time = 500;
    right = 0, left = 0, up = 0, down = 0;
    width = PLAYER_WIDTH; height = PLAYER_HEIGHT;
    playerId = 1;
    renderCycle = 1;
    player_no = 1;
    attack = false;
    attack_counter = 0;
}


Player::Player(const Player &p){

    name = p.name;
    xpos = p.xpos, ypos = p.ypos;
    score = p.score;
    time = p.time;
    right = 0, left = 0, up = 0, down = 0;
    width = p.width, height = p.height;
    playerId = p.playerId;
    renderCycle = p.renderCycle;
    player_no = p.player_no;
    attack_counter=p.attack_counter;
    attack = false;
    attack_dir = p.attack_dir;
    
}

void Player::encode(int x[]){

    x[0] = xpos;
    x[1] = ypos;
    x[2] = attack;
    x[3] = attack_dir;
    x[4] = attack_counter;
    x[5] = playerId;

}

void Player::decode(int y[]){
    old_xpos = xpos;
    old_ypos = ypos;
    xpos = y[0];
    ypos = y[1];
    attack = y[2];
    attack_dir = y[3];
    attack_counter = y[4];
    playerId = y[5];
}

void Player::set_time(int t){
    time = t;
}

void Player:: init(){
    right = 0, left = 0, up = 0, down = 0;
    attack_counter=0;
    attack = false;
    attack_dir =-1;
    bullets.clear();
}

int Player::get_time(){
    return time;
}

void Player::setPosCenter(int i, int j){
    xpos = i*CELL_SIZE + CELL_SIZE/2 - width/2;
    ypos = j*CELL_SIZE + CELL_SIZE/2 - height/2;
}

void Player::draw(SDL_Renderer *renderer, TTF_Font *font){
    if(final_freeze){
        disp_text(renderer,"OUT", font, xpos , ypos+20);
        return;
    }
    SDL_Rect destR;
    destR.h = height;
    destR.w = width;
    destR.x = xpos;
    destR.y = ypos + SCORE_DISPLAY_HEIGHT;

    SDL_Rect srcR;
    srcR.h = PLAYER_HEIGHT_SRC;
    srcR.w = PLAYER_WIDTH_SRC;
    
    if(xpos > old_xpos)
        srcR.y = 2*PLAYER_HEIGHT_SRC;
    else if(xpos < old_xpos)
        srcR.y = 1*PLAYER_HEIGHT_SRC;
    else if(ypos < old_ypos)
        srcR.y = 3*PLAYER_HEIGHT_SRC;
    else if(ypos > old_ypos)
        srcR.y = 0;
    else{
        srcR.y = 0;
        renderCycle = 0;
    }
    srcR.x = (3*playerId + int(renderCycle/RENDER_PLAYER_DELAY)) * PLAYER_WIDTH_SRC;

    // SDL_RenderCopy(renderer, Tex,  NULL, &destR);
    if(SDL_RenderCopyEx(renderer, Tex,  &srcR, &destR, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"Players not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
    renderCycle = (renderCycle+1)%(3*RENDER_PLAYER_DELAY) ;
    disp_text(renderer, "P" , font, xpos + width, ypos+5);
    if(player_no == 1)
        disp_text(renderer,  "1", font, xpos + width, ypos+20);
    else
        disp_text(renderer,  "2", font, xpos + width, ypos+20);

    if(freeze){
        SDL_RenderCopyEx(renderer, freezeTex,  &srcR, &destR, 0.0, NULL, SDL_FLIP_NONE);
    }
    
}

void Player::dispName(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, name, font, xpos, ypos);
}
void Player::dispScore(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Score: ", font, xpos, ypos);
    std::string temp_str = std::to_string(score);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
void Player::dispTime(SDL_Renderer *renderer, TTF_Font *font, int xpos, int ypos){
    disp_text(renderer, "Time: ", font, xpos, ypos);
    std::string temp_str = std::to_string(time);
    char* char_type = (char*) temp_str.c_str();
    disp_text(renderer, char_type, font, xpos+50, ypos);
}
std::pair<int,int> Player::move(int s){
    old_xpos = xpos, old_ypos = ypos;
    int new_x = xpos, new_y = ypos;
    if(!freeze and !final_freeze){
        if(right)
            new_x+=s;
        if(left)
            new_x-=s;
        if(up)
            new_y-=s;
        if(down)
            new_y+=s;
    }
    return std::make_pair(new_x, new_y);
}

void Player::handleKeyDown(int key){
    switch(key){
        case SDLK_LEFT:
            left = 1;
            break;
        case SDLK_RIGHT:
            right = 1;
            break;
        case SDLK_UP:
            up = 1;
            break;
        case SDLK_DOWN:
            down = 1;
            break;
        case  SDLK_a:
            attack = true;
            attack_dir = 0;
            attack_counter =0;
            break;
        case  SDLK_d:
            attack = true;
            attack_dir = 1;
            attack_counter =0;
            break;
        case  SDLK_w:
            attack = true;
            attack_dir = 2;
            attack_counter =0;
            break;
        case  SDLK_s:
            attack = true;
            attack_dir = 3;
            attack_counter =0;
            break;
        default:
            break;
    }
}
void Player::handleKeyUp(int key){
    switch(key){
        case SDLK_LEFT:
            left = 0;
            break;
        case SDLK_RIGHT:
            right = 0;
            break;
        case SDLK_UP:
            up = 0;
            break;
        case SDLK_DOWN:
            down = 0;
            break;
        case  SDLK_a:
            attack = false;
            break;
        case  SDLK_d:
            attack = false;
            break;
        case  SDLK_w:
            attack = false;
            break;
        case  SDLK_s:
            attack = false;
            break;
        default:
            break;
    }
}

std::pair<int, int> Player::getMazeCoordinates(SDL_Rect &r){
    int i = 0;
    int j = 0;
    while(i<MAZEROWS){
        while(j<MAZECOLS){
            if(xpos>=r.w * j)
                j+=1;
            if(ypos>=r.h * i)
                i+=1;
            if(xpos<r.w * j && ypos<r.h * i)
                return std::make_pair(i-1, j-1);
        }
    }
    return std::make_pair(-1, -1);
}


void Game::renderPeriscope(){
    SDL_Rect dstR;
    dstR.x = 0; dstR.y = 0;
    dstR.w = 2*SCREEN_WIDTH; 
    dstR.h = 2*SCREEN_HEIGHT;

    if(level == 3){
        dstR.w = 1.5*dstR.w; 
        dstR.h = 1.5*dstR.h;
    }
    if(isServer){
        dstR.x = sPlayer.xpos + sPlayer.width/2 - dstR.w/2;
        dstR.y = sPlayer.ypos + sPlayer.height/2 - dstR.h/2 + SCORE_DISPLAY_HEIGHT;
    }
    else{
        dstR.x = cPlayer.xpos + cPlayer.width/2 - dstR.w/2;
        dstR.y = cPlayer.ypos + cPlayer.height/2 - dstR.h/2 + SCORE_DISPLAY_HEIGHT;
    }
    if(SDL_RenderCopyEx(renderer, periTex,  NULL, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
        std::cout<<"Periscope view not rendered properly\n";
        std::cout<<SDL_GetError()<<"\n";
        exit(EXIT_FAILURE);
    }
}

int Game::askPlayerAvatar(){
    char id[1];
    memset(id, ' ', 1);
    SDL_Event e;
    int position = 0;
    int ok = false;
    bool showError = false;
    int SDL_keys[] = {SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7};
    while (!ok) {

        if (SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT){
                return -1;
            }
            if (e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_ESCAPE)
                    return -1;
                if ((e.key.keysym.sym >= SDL_keys[0] && e.key.keysym.sym <= SDL_keys[7])) {
                    if(!isServer && e.key.keysym.sym == SDL_keys[sPlayer.playerId]){
                        showError = true;
                        continue;
                    }
                    if (position > 0) {
                        position = 0;
                    }
                    id[position] = e.key.keysym.sym;
                    position++;
                }
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    position--;
                    if (position < 0) {
                        position = 0;
                    }
                    id[position] = ' ';
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    id[position] = 0;
                    ok = true;
                }
            }
        }

        SDL_Rect srcR[8];
        for(int i = 0; i<8; i++){
            srcR[i].w = PLAYER_WIDTH_SRC; srcR[i].h = PLAYER_HEIGHT_SRC;
            srcR[i].y = 0;
            srcR[i].x = (i*3 + 1) * PLAYER_WIDTH_SRC;
        }

        SDL_Rect dstR[8];
        for(int i = 0; i<8; i++){
            dstR[i].w = PLAYER_WIDTH; dstR[i].h = PLAYER_HEIGHT;
            dstR[i].y = SCREEN_HEIGHT/3+20 + SCORE_DISPLAY_HEIGHT;
            dstR[i].x = 10 + SCREEN_WIDTH/2 + (i-4)*(PLAYER_WIDTH + 10);
        }


        usleep(200);
        SDL_RenderClear(renderer);

        if(showError){
            char * intChars[] = {"0", "1", "2", "3", "4", "5", "6", "7"};
            char * splayerAvatar = intChars[sPlayer.playerId];
            const char* c = "Player 1 has chosen Avatar ";
            char* full_text;
            full_text=static_cast<char *>(malloc(strlen(c)+strlen(splayerAvatar)));
            strcpy(full_text,c);
            strcat(full_text,splayerAvatar);
            disp_text_center(renderer, full_text, font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3)+145);
            disp_text_center(renderer, "Please Chose a different avatar!", font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3)+165);
        }
            
        if(isServer)
            disp_text_center(renderer, "Player 1" , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3)-45);
        else{
            disp_text_center(renderer, "Player 2" , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3)-45);
        }    
            
        char c[] = "Please chose Your Avatar";
        disp_text_center(renderer, c , font, int(SCREEN_WIDTH/2)+10, int(SCREEN_HEIGHT/3)-20);
        disp_text_center(renderer, id , font, int(SCREEN_WIDTH/2), int(SCREEN_HEIGHT/3)+120);

        for(int i = 0; i<8; i++){
            if(SDL_RenderCopyEx(renderer, sPlayer.Tex,  &srcR[i], &dstR[i], 0.0, NULL, SDL_FLIP_NONE) < 0){
                std::cout<<"Avatars not rendered properly\n";
                std::cout<<SDL_GetError()<<"\n";
                exit(EXIT_FAILURE);
            }
            char * digits[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
            char * p_id = digits[i]; 
            
            disp_text_center(renderer, p_id , font, dstR[i].x+dstR[i].w/2 , int(SCREEN_HEIGHT/3)+80);
        }

        SDL_RenderPresent(renderer);
    }
    if(isServer)
        sPlayer.playerId = std::stoi(id);
    else    
        cPlayer.playerId = std::stoi(id);

    return 1;
}

void Game::updateTime(){
    if(gameTime % TIME_STEP == 0){
        if(sPlayer.get_time()>0)
            sPlayer.set_time(sPlayer.get_time() - TIME_STEP);
        else { 
            if(!sPlayer.final_freeze && isServer)
                sounds.play("death", false, 8);
            sPlayer.final_freeze = true;
        }
        if(cPlayer.get_time()>0)
            cPlayer.set_time(cPlayer.get_time() - TIME_STEP);
        else { 
            if(!cPlayer.final_freeze && !isServer)
                sounds.play("death", false, 8);
            cPlayer.final_freeze = true;
        }

    }
}

void Game::handlePlayerActivities(Player & p){
    std::pair<int, int> s_p = p.move(SPEED); 
    if(!checkWallCollisions(s_p.first, p.ypos, p.width, p.height)){
        p.xpos = s_p.first;
    }
    if(!checkWallCollisions(p.xpos, s_p.second, p.width, p.height)){
        p.ypos = s_p.second;
    }

    if( p.attack && p.attack_counter%1000 == 1 && p.score >= BULLET_COST && !p.final_freeze && !p.freeze ){
        if(isServer)
            sounds.play("shoot", false, 50);
        Bullet b(p.xpos + p.width/2 - BULLET_WIDTH/2,p.ypos + p.height/2 - BULLET_HEIGHT/2  ,p.attack_dir);
        p.bullets.push_back(b);
        p.score -= BULLET_COST;
    }

    if(p.attack)
        ++p.attack_counter;
}