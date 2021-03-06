#include "game.h"
#include <chrono>
#include <thread>


MazeCell::MazeCell(){
    MazeCell(0);
    srcR.w = 32;
    srcR.h = 32;
}
MazeCell::MazeCell(int arg_id){
    update(arg_id);
    srcR.w = 32;
    srcR.h = 32;
}

void MazeCell::update(int arg_id){
    id = arg_id;
    
    srcR.x = srcR.w * (id%10);
    srcR.y = srcR.h * (id/10);
    
}

void MazeCell::removeWall(int direction){
    switch(direction){
        case 0:
            removeWall("top");
            break;
        case 1:
            removeWall("bottom");
            break;
        case 2:
            removeWall("right");
            break;
        case 3:
            removeWall("left");
            break;
        default:
            std::cout<<"Invalid use of removeWall()!\n";
            exit(EXIT_FAILURE);
            break;

    }
}

void MazeCell::removeWall(char *direction){
    int factor = -1;

    if(strcmp(direction, "left") == 0)
        factor = 0;
    else if(strcmp(direction, "right") == 0)
        factor = 1;
    else if(strcmp(direction, "top") == 0)
        factor = 2;
    else if(strcmp(direction, "bottom") == 0)
        factor = 3;
    else{
        std::cout<<"Invalid use of removeWall(). Please use top, bottom, left or right as argument!\n";
        exit(EXIT_FAILURE);
    }

    if( (id>>factor) % 2 == 1){
        update(id - (1<<factor) );
    }
    else{
        std::cout<<"Wall to remove does not exist\n";
        exit(EXIT_FAILURE);
    }

}


bool Game::ok(int x, int y){
    if(x>=0 && x<MAZEROWS && y>=0 && y<MAZECOLS){
        return true;
    }else
        return false;
}

// void Game::dfs(int x, int y){
    
//     std::pair<int,int> dir[] = {std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};

//     while(true){
//         int random = std::rand();
//         random = random%4;
//         if(ok(x+dir[random].first,y+dir[random].second) && maze[x+dir[random].first][y+dir[random].second].id == 15 ){
//             switch (random) {
//                 case 1:
//                     maze[x][y].removeWall("top");
//                     maze[x+dir[random].first][y+dir[random].second].removeWall("bottom");
//                     break;
//                 case 0:
//                     maze[x][y].removeWall("bottom");
//                     maze[x+dir[random].first][y+dir[random].second].removeWall("top");
//                     break;
//                 case 2:
//                     maze[x][y].removeWall("right");
//                     maze[x+dir[random].first][y+dir[random].second].removeWall("left");
//                     break;
//                 case 3:
//                     maze[x][y].removeWall("left");
//                     maze[x+dir[random].first][y+dir[random].second].removeWall("right");
//                     break;
//                 default:
//                     break;
//             }
//             dfs(x+dir[random].first,y+dir[random].second);
//         }
//         int cnt = 0;
//         for(int i=0;i<4;i++){
//             if(ok(x+dir[i].first,y+dir[i].second) && maze[x+dir[i].first][y+dir[i].second].id == 15  ){
//                 ++cnt;
//             }
//         }
//         if(!cnt)
//             break;
//     }
    
// }

void Game:: random_wall_removal(){

    //srand( seedi + level );

      std::pair<int,int> dir[] = {std::make_pair(0,-1),std::make_pair(0,1),std::make_pair(-1,0),std::make_pair(1,0)};

    int i = rand()%MAZEROWS;
    int j = rand()%MAZECOLS;
    std::vector<int> store;
    for(int k=0;k<4;k++)
    if(ok(i+dir[k].first,j+dir[k].second) && (((maze[i][j].id)>>k)%2 == 1)  )
        store.push_back(k);

    while( store.size() == 0){
    i = rand()%MAZEROWS;
    j = rand()%MAZECOLS;

    for(int k=0;k<4;k++)
    if(ok(i+dir[k].first,j+dir[k].second) && (((maze[i][j].id)>>k)%2 == 1)  )
        store.push_back(k);
    
    }

    int x = i;
    int y = j;

    std::shuffle(store.begin(),store.end(),std::default_random_engine(rand()));

    int rom = store[0];

            switch (rom) {
                case 2:
                    maze[x][y].removeWall("top");
                    maze[x+dir[rom].first][y+dir[rom].second].removeWall("bottom");
                    break;
                case 3:
                    maze[x][y].removeWall("bottom");
                    maze[x+dir[rom].first][y+dir[rom].second].removeWall("top");
                    break;
                case 1:
                    maze[x][y].removeWall("right");
                    maze[x+dir[rom].first][y+dir[rom].second].removeWall("left");
                    break;
                case 0:
                    maze[x][y].removeWall("left");
                    maze[x+dir[rom].first][y+dir[rom].second].removeWall("right");
                    break;
                default:
                    break;
        }

}

int parent[MAZECOLS*MAZEROWS];
int size[MAZECOLS*MAZEROWS];

int find_set(int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v]);
}

void make_set(int v) {
    parent[v] = v;
    size[v] = 1;
}

bool union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (size[a] < size[b])
            std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }else
        return false;
}

void Game:: maze_gen(){
    std::pair<int,int> dir[] = {std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};
    std::vector<std::pair<std::pair<int,int>,std::pair<int,int> > > store;

    for(int i=0;i<MAZEROWS;i++){
        for(int j=0;j<MAZECOLS;j++){
            std::pair<int,int> p1 = std::make_pair(i,j);
            for(int k=0;k<4;k++){
                if(ok(i+dir[k].first,j+dir[k].second)){
                    std::pair<int,int> p2 = std::make_pair(i+dir[k].first,j+dir[k].second);
                    store.push_back(std::make_pair(p1,p2));
                }
            }
        }
    }

    std::shuffle(store.begin(),store.end(),std::default_random_engine(seedi));

    for(int i=0;i<MAZECOLS*MAZEROWS;i++)
        make_set(i);
    for(int i=0;i<store.size();i++){

        std::pair<int,int> p1 = store[i].first;
        std::pair<int,int> p2 = store[i].second;
        int l1 = p1.first*MAZECOLS + p1.second ;
        int l2 = p2.first*MAZECOLS + p2.second ;


        if(union_sets(l1,l2)){
            int k =0;
            for( ;k<4 ; k++){
                if(p1.first+dir[k].first == p2.first and  p1.second+dir[k].second == p2.second)
                    break;
            }
            int random = k;
            int x = p1.first;
            int y = p1.second;
            switch (random) {
                case 1:
                    maze[x][y].removeWall("top");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("bottom");
                    break;
                case 0:
                    maze[x][y].removeWall("bottom");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("top");
                    break;
                case 2:
                    maze[x][y].removeWall("right");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("left");
                    break;
                case 3:
                    maze[x][y].removeWall("left");
                    maze[x+dir[random].first][y+dir[random].second].removeWall("right");
                    break;
                default:
                    break;
            }
            SDL_RenderClear(renderer);
            renderMaze();
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            coinCycle+=2;
            timeCycle+=2;
            SDL_RenderPresent(renderer);
            
        }
    }
}




void Game::renderMaze(){
    int cell_width = CELL_SIZE;
    int cell_height = CELL_SIZE;
    int coin_width = COIN_SIZE;
    int coin_height = COIN_SIZE;
    for(int i =0; i<MAZEROWS; i++){
        for(int j = 0; j<MAZECOLS; j++){
            
            maze[i][j].dstR.w = cell_width;
            maze[i][j].dstR.h = cell_height;
            maze[i][j].dstR.x = maze[i][j].dstR.w * j;
            maze[i][j].dstR.y = maze[i][j].dstR.h * i ;

            SDL_Rect temp;
            temp.w =  maze[i][j].dstR.w; temp.h =  maze[i][j].dstR.h;
            temp.x =  maze[i][j].dstR.x; temp.y =  maze[i][j].dstR.y + SCORE_DISPLAY_HEIGHT;
            if(SDL_RenderCopyEx(renderer, mazeTex,  &maze[i][j].srcR, &temp, 0.0, NULL, SDL_FLIP_NONE) < 0){
                std::cout<<"Maze cell"<<i<<", "<<j<<" not rendered properly\n";
                std::cout<<SDL_GetError()<<"\n";
                exit(EXIT_FAILURE);
            }

            SDL_Rect dstR, srcR;
            srcR.w = 160;
            srcR.h = 160;
            srcR.x = srcR.w * ((coinCycle/10)%8);
            srcR.y = 0;

            dstR.w = coin_width;
            dstR.h = coin_height;
            dstR.x = cell_width * j + (cell_width - coin_width)/2;
            dstR.y = cell_height * i + (cell_height - coin_height)/2 + SCORE_DISPLAY_HEIGHT;
            
            if(maze[i][j].hascoin){
                if(SDL_RenderCopyEx(renderer, coinTex,  &srcR, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
                    std::cout<<"Coin not rendered properly\n";
                    std::cout<<SDL_GetError()<<"\n";
                    exit(EXIT_FAILURE);
                }
            }
            srcR.w = 360;
            srcR.h = 360;
            srcR.x = srcR.w * ((coinCycle/10)%8);
            srcR.y = 0;

            if(maze[i][j].hastime){
                if(SDL_RenderCopyEx(renderer, timeTex,  &srcR, &dstR, 0.0, NULL, SDL_FLIP_NONE) < 0){
                    std::cout<<"Time not rendered properly\n";
                    std::cout<<SDL_GetError()<<"\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
    coinCycle = (coinCycle + 1)%80;
    timeCycle = (timeCycle + 1)%80;
}

void Game::placeCoins(){

    for(int k=0;k<4;k++){
    int random_i = rand() % MAZEROWS;
    int random_j = rand() % MAZECOLS;
    SDL_Rect rect;
    rect.x = 0; rect.y = 0;
    rect.w = CELL_SIZE; rect.h = CELL_SIZE;
    std::pair<int, int> s_i_j = sPlayer.getMazeCoordinates(rect);
    std::pair<int, int> c_i_j = cPlayer.getMazeCoordinates(rect);
        while(maze[random_i][random_j].hascoin == true || maze[random_i][random_j].hastime == true || abs(random_i - s_i_j.first) + abs(random_j - s_i_j.second) <= COIN_DIST || abs(random_i - c_i_j.first) + abs(random_j - c_i_j.second) <= COIN_DIST ){
            random_i = rand() % MAZEROWS;
            random_j = rand() % MAZECOLS;
        }
        maze[random_i][random_j].hascoin = true;
    }
}

void Game::placeTimes(){

    for(int k=0;k<4;k++){
    int random_i = rand() % MAZEROWS;
    int random_j = rand() % MAZECOLS;
    SDL_Rect rect;
    rect.x = 0; rect.y = 0;
    rect.w = CELL_SIZE; rect.h = CELL_SIZE;
    std::pair<int, int> s_i_j = sPlayer.getMazeCoordinates(rect);
    std::pair<int, int> c_i_j = cPlayer.getMazeCoordinates(rect);

        while(maze[random_i][random_j].hascoin == true || maze[random_i][random_j].hastime == true || abs(random_i - s_i_j.first) + abs(random_j - s_i_j.second) <= COIN_DIST || abs(random_i - c_i_j.first) + abs(random_j - c_i_j.second) <= COIN_DIST ){
            random_i = rand() % MAZEROWS;
            random_j = rand() % MAZECOLS;
        }
        maze[random_i][random_j].hastime = true;

    }
}


void Game::mazeInit(){
    for(int i =0; i<MAZEROWS; i++){
        for(int j = 0; j<MAZECOLS; j++){
            maze[i][j].update(15);
            maze[i][j].hascoin = false;
            maze[i][j].hastime = false;
            for(int k=0;k<MAZEROWS*MAZECOLS;k++)
                maze[i][j].to_go[k] = -1;
        }
    }
    coinCycle = 0;
    timeCycle = 0;
    placeCoins();
    placeTimes();
}



void Game::maze_dist_update(){

        //std::pair<int,int> dir[] = {std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,1),std::make_pair(0,-1)};

        for(int i =0; i<MAZEROWS; i++){
        for(int j = 0; j<MAZECOLS; j++){
            //do bfs 
            std::queue<int> q;
            std::vector<bool> used(MAZECOLS*MAZEROWS);
            std::vector<int> d(MAZECOLS*MAZEROWS), p(MAZECOLS*MAZEROWS);

            int s = i*MAZECOLS+j;

            q.push(s);
            used[s] = true;
            p[s] = -1;
            d[s] = 0;
            
            while (!q.empty()) {

                int v = q.front();
                q.pop();

                int row = v/MAZECOLS;
                int col = v%MAZECOLS;
                int id = maze[row][col].id;
                if((id>>0)%2 == 0){
                    int u = (row)*MAZECOLS + col - 1;
                    if (!used[u]) {
                        used[u] = true;
                        q.push(u);
                        d[u] = d[v] + 1;
                        p[u] = v;
                        maze[row][col-1].to_go[s] = 1;
                    }
                }

                if((id>>1)%2 == 0){
                    int u = (row)*MAZECOLS + col + 1;
                    if (!used[u]) {
                        used[u] = true;
                        q.push(u);
                        d[u] = d[v] + 1;
                        p[u] = v;
                        maze[row][col+1].to_go[s] = 0;
                    }
                }

                if((id>>2)%2 == 0){
                    int u = (row-1)*MAZECOLS + col;
                    if (!used[u]) {
                        used[u] = true;
                        q.push(u);
                        d[u] = d[v] + 1;
                        p[u] = v;
                        maze[row-1][col].to_go[s] = 3;
                    }
                }

                if((id>>3)%2 == 0){
                    int u = (row+1)*MAZECOLS + col;
                    if (!used[u]) {
                        used[u] = true;
                        q.push(u);
                        d[u] = d[v] + 1;
                        p[u] = v;
                        maze[row+1][col].to_go[s] = 2;
                    }
                }
            }

        }
    }

}



bool iscollidingwall(int x, int y, int w, int h, SDL_Rect maze_rect){
    if (x >= maze_rect.x + maze_rect.w || maze_rect.x >= x + w)
        return false;
    if (y >= maze_rect.y + maze_rect.h || maze_rect.y >= y + h)
        return false;

    return true;
    
}

int pow(int x, int y){
    if(y <= 0)
        return 1;
    else    
        return x * pow(x, y-1);
}

bool Game::checkWallCollisions(int x, int y, int w, int h){
    for(int i =0; i<MAZEROWS; i++){
        for(int j = 0; j<MAZECOLS; j++){
            int id = maze[i][j].id;
            
            SDL_Rect maze_rect[4];
            // left
            maze_rect[0] = maze[i][j].dstR;
            maze_rect[0].w /= WALL_RATIO;
            // right
            maze_rect[1] = maze[i][j].dstR;
            maze_rect[1].w /= WALL_RATIO;
            maze_rect[1].x += maze[i][j].dstR.w - maze_rect[1].w;
            // top
            maze_rect[2] = maze[i][j].dstR;
            maze_rect[2].h /= WALL_RATIO;
            // bottom
            maze_rect[3] = maze[i][j].dstR;
            maze_rect[3].h /= WALL_RATIO;
            maze_rect[3].y += maze[i][j].dstR.h - maze_rect[3].h;
            
            for(int k = 0; k<4; k++){
                if((int(id/pow(2, k))%2 != 0) && iscollidingwall(x, y, w, h, maze_rect[k])){
                    return true;
                }
            }
        }
    }
    return false;
}

bool isOnPower(int x, int y, int w, int h, SDL_Rect & rect){
    SDL_Rect play_temp;
    play_temp.x = x; play_temp.y = y; play_temp.w = PLAYER_WIDTH; play_temp.h = PLAYER_HEIGHT;

    SDL_Rect temp_pow;
    temp_pow.x = rect.x + (CELL_SIZE - COIN_SIZE)/2; 
    temp_pow.y = rect.y + (CELL_SIZE - COIN_SIZE)/2; 
    temp_pow.w = COIN_SIZE; temp_pow.h = COIN_SIZE;
    return SDL_HasIntersection(&play_temp, &temp_pow);
}

bool playerOnCoin(Player & p, MazeCell & m){

    if(m.hascoin && isOnPower(p.xpos, p.ypos, p.width, p.height, m.dstR)){
      // m.hascoin = false;
        p.score += COIN_SCORE;
        return true;
    }
    return false;
}
bool playerOnTime(Player & p, MazeCell & m){
    if(m.hastime && isOnPower(p.xpos, p.ypos, p.width, p.height, m.dstR)){
       // m.hastime = false;
        p.set_time(p.get_time()+TIME_INCREASE);
        return true;
    }
    return false;

}

void Game::updateCoinTime(Player & p, MazeCell & m, bool playerIsServer){
   // srand(seedi + sPlayer.get_time() + cPlayer.get_time()  );
    
    int random_i = rand() % MAZEROWS;
    int random_j = rand() % MAZECOLS;
    SDL_Rect rect;
    rect.x = 0; rect.y = 0;
    rect.w = CELL_SIZE; rect.h = CELL_SIZE;
    std::pair<int, int> s_i_j = sPlayer.getMazeCoordinates(rect);
    std::pair<int, int> c_i_j = cPlayer.getMazeCoordinates(rect);
    if(playerOnCoin(p, m)){
        if((playerIsServer && isServer) || (!playerIsServer && !isServer))
            sounds.play("coin", false, 30);
        while(maze[random_i][random_j].hascoin == true || maze[random_i][random_j].hastime == true || abs(random_i - s_i_j.first) + abs(random_j - s_i_j.second) <= COIN_DIST || abs(random_i - c_i_j.first) + abs(random_j - c_i_j.second) <= COIN_DIST ){
            random_i = rand() % MAZEROWS;
            random_j = rand() % MAZECOLS;
        }
        maze[random_i][random_j].hascoin = true;
        m.hascoin = false;
    }
    random_i = rand() % MAZEROWS;
    random_j = rand() % MAZECOLS;
    if(playerOnTime(p, m)){
        if((playerIsServer && isServer) || (!playerIsServer && !isServer))
            sounds.play("time", false);

        
        while(maze[random_i][random_j].hascoin == true || maze[random_i][random_j].hastime == true || abs(random_i - s_i_j.first) + abs(random_j - s_i_j.second) <= COIN_DIST || abs(random_i - c_i_j.first) + abs(random_j - c_i_j.second) <= COIN_DIST ){
            random_i = rand() % MAZEROWS;
            random_j = rand() % MAZECOLS;
        }
        int random = rand()%10;
        if(random < PROBABILITY  )
            maze[random_i][random_j].hastime = true;
        m.hastime = false;
    }
}
void Game::checkCoinTimeEat(){
    std::pair<int, int> s_co = sPlayer.getMazeCoordinates(maze[0][0].dstR);
    std::pair<int, int> c_co = cPlayer.getMazeCoordinates(maze[0][0].dstR);
    
    updateCoinTime(sPlayer, maze[s_co.first][s_co.second], true);
    updateCoinTime(sPlayer, maze[s_co.first + 1][s_co.second], true);
    updateCoinTime(sPlayer, maze[s_co.first - 1][s_co.second], true);
    updateCoinTime(sPlayer, maze[s_co.first][s_co.second + 1], true);
    updateCoinTime(sPlayer, maze[s_co.first][s_co.second -1], true);
    updateCoinTime(cPlayer, maze[c_co.first][c_co.second]);
    updateCoinTime(cPlayer, maze[c_co.first + 1][c_co.second]);
    updateCoinTime(cPlayer, maze[c_co.first - 1][c_co.second]);
    updateCoinTime(cPlayer, maze[c_co.first][c_co.second + 1]);
    updateCoinTime(cPlayer, maze[c_co.first][c_co.second -1]);

}
