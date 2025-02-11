//
// Created by Hari Patel on 4/16/24.
//
using namespace std;
#pragma once
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cctype>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <map>


struct Tile{
    int tilestate = 0;
    bool revealed = false;
    bool flagged = false;
    bool isMine = false;
    vector <Tile*> neighbors;



};

class Board{
public:
    vector<vector<Tile>> tiles;
    int numColumns;
    int numRows;
    int numMines;
    int flags = 0;
    int finalCounter = 0;
    sf::Sprite revealed;
    sf::Sprite mine;
    sf::Sprite one;
    sf::Sprite two;
    sf::Sprite three;
    sf::Sprite four;
    sf::Sprite five;
    sf::Sprite six;
    sf::Sprite seven;
    sf::Sprite eight;
    sf::Sprite flag;
    sf::Sprite lose;
    sf::Sprite win;
    sf::Sprite happy;
    sf::Sprite counter;
    sf::Sprite counter1;
    sf::Sprite counter2;
    sf::Sprite counter3;
    sf::Sprite timer1;
    sf::Sprite timer2;
    sf::Sprite timer3;
    sf::Sprite timer4;





    map<string, sf::Texture> icontextures;
    map<int, sf::IntRect> digits;
    bool gameover = false;
    bool gameWon = false;
    bool showleaders = false;



    Board(int col, int row, int mines){
        numColumns = col;
        numMines = mines;
        numRows = row;
        sf::Texture holder;
        holder.loadFromFile("files/images/mine.png");
        icontextures.emplace("mine", holder);
        holder.loadFromFile("files/images/tile_revealed.png");
        icontextures.emplace("revealed", holder);
        holder.loadFromFile("files/images/number_1.png");
        icontextures.emplace("1", holder);
        holder.loadFromFile("files/images/number_2.png");
        icontextures.emplace("2", holder);
        holder.loadFromFile("files/images/number_3.png");
        icontextures.emplace("3", holder);
        holder.loadFromFile("files/images/number_4.png");
        icontextures.emplace("4", holder);
        holder.loadFromFile("files/images/number_5.png");
        icontextures.emplace("5", holder);
        holder.loadFromFile("files/images/number_6.png");
        icontextures.emplace("6", holder);
        holder.loadFromFile("files/images/number_7.png");
        icontextures.emplace("7", holder);
        holder.loadFromFile("files/images/number_8.png");
        icontextures.emplace("8", holder);
        holder.loadFromFile("files/images/flag.png");
        icontextures.emplace("flag", holder);
        holder.loadFromFile("files/images/face_happy.png");
        icontextures.emplace("happy", holder);
        holder.loadFromFile("files/images/face_lose.png");
        icontextures.emplace("lose", holder);
        holder.loadFromFile("files/images/face_win.png");
        icontextures.emplace("win", holder);

        sf::Texture digittext;
        digittext.loadFromFile("files/images/digits.png");
        icontextures.emplace("digits", digittext);

        for(int i = 0; i<=10; i++){
            digits[i]=sf::IntRect(i*21,0,21,32); //creates the rects for the digits
        }

        for(int i=0; i< row; i++) {
            vector<Tile> empty;
            tiles.push_back(empty);
            for (int j = 0; j < col; j++) {
                Tile temptile;
                tiles[i].push_back(temptile);
            }
        }
    }

    void timerDraw(sf::RenderWindow &win , int min, int sec){
        int tensMin = (min / 10) % 10;
        int onesMin = min % 10;
        int tensSec = (sec / 10) % 10;
        int onesSec = sec % 10;
        timer1.setTexture(icontextures["digits"]);
        timer1.setTextureRect(digits[tensMin]);
        timer1.setPosition((numColumns * 32) - 97, 32 * (numRows + 0.5) + 16);
        timer2.setTexture(icontextures["digits"]);
        timer2.setTextureRect(digits[onesMin]);
        timer2.setPosition((numColumns * 32) - 97 + 21, 32 * (numRows + 0.5) + 16);
        timer3.setTexture(icontextures["digits"]);
        timer3.setTextureRect(digits[tensSec]);
        timer3.setPosition((numColumns * 32) - 54, 32 * (numRows + 0.5) + 16);
        timer4.setTexture(icontextures["digits"]);
        timer4.setTextureRect(digits[onesSec]);
        timer4.setPosition((numColumns * 32) - 54 + 21, 32 * (numRows + 0.5) + 16);
        win.draw(timer1);
        win.draw(timer2);
        win.draw(timer3);
        win.draw(timer4);

    }

    void boardDraw(sf::RenderWindow &win){
        finalCounter = numMines - flags;
        if(finalCounter < 0){
            counter.setTexture(icontextures["digits"]);
            counter.setTextureRect(digits[10]);
            counter.setPosition(12, 32*(numRows+ 0.5) + 16);
            win.draw(counter);
        }
        int hundreds = (abs(finalCounter) / 100) % 10;

        int tens = (abs(finalCounter) / 10) % 10;

        int ones = abs(finalCounter) % 10;

        counter1.setTexture(icontextures["digits"]);
        counter1.setTextureRect(digits[hundreds]);
        counter1.setPosition(33, 32*(numRows+ 0.5) + 16);
        counter2.setTexture(icontextures["digits"]);
        counter2.setTextureRect(digits[tens]);
        counter2.setPosition(33+21, 32*(numRows+ 0.5) + 16);
        counter3.setTexture(icontextures["digits"]);
        counter3.setTextureRect(digits[ones]);
        counter3.setPosition(33+21+21, 32*(numRows+ 0.5) + 16);
        win.draw(counter1);
        win.draw(counter2);
        win.draw(counter3);
        for(int i=0; i< numColumns; i++){
            for(int j = 0; j< numRows; j++){
                sf::Texture holder;
                holder.loadFromFile("files/images/tile_hidden.png");
                sf::Sprite sprite;
                sprite.setTexture(holder);
                sprite.setPosition(i*32, j*32);
                win.draw(sprite);
                if(tiles[j][i].revealed){
                    revealed.setTexture(icontextures["revealed"]);
                    revealed.setPosition(i*32, j*32);
                    win.draw(revealed);
                    if(tiles[j][i].tilestate == 1){
                        one.setTexture(icontextures["1"]);
                        one.setPosition(i*32, j*32);
                        win.draw(one);
                    }
                    if(tiles[j][i].tilestate == 2){
                        two.setTexture(icontextures["2"]);
                        two.setPosition(i*32, j*32);
                        win.draw(two);
                    }
                    if(tiles[j][i].tilestate == 3){
                        three.setTexture(icontextures["3"]);
                        three.setPosition(i*32, j*32);
                        win.draw(three);
                    }
                    if(tiles[j][i].tilestate == 4){
                        four.setTexture(icontextures["4"]);
                        four.setPosition(i*32, j*32);
                        win.draw(four);
                    }
                    if(tiles[j][i].tilestate == 5){
                        five.setTexture(icontextures["5"]);
                        five.setPosition(i*32, j*32);
                        win.draw(five);
                    }
                    if(tiles[j][i].tilestate == 6){
                        six.setTexture(icontextures["6"]);
                        six.setPosition(i*32, j*32);
                        win.draw(six);
                    }
                    if(tiles[j][i].tilestate == 7){
                        seven.setTexture(icontextures["7"]);
                        seven.setPosition(i*32, j*32);
                        win.draw(seven);
                    }
                    if(tiles[j][i].tilestate == 8){
                        eight.setTexture(icontextures["8"]);
                        eight.setPosition(i*32, j*32);
                        win.draw(eight);
                    }
                    if(tiles[j][i].isMine){
                        mine.setTexture(icontextures["mine"]);
                        mine.setPosition(i*32, j*32);
                        win.draw(mine);
                    }
                } else if (tiles[j][i].revealed == false && tiles[j][i].flagged){
                    flag.setTexture(icontextures["flag"]);
                    flag.setPosition(i*32, j*32);
                    win.draw(flag);
                }

            }
        }
    }


    void setmines(){
        auto test = std::chrono::high_resolution_clock::now();
        auto test2 = duration_cast<std::chrono::nanoseconds>(test.time_since_epoch()).count();
        srand(test2);

        int mines_set = 0;
        while(mines_set < numMines){
            int randomCol = rand() % numColumns;
            int randomRow = rand() % numRows;
            if(!tiles[randomRow][randomCol].isMine){
                tiles[randomRow][randomCol].isMine = true;
                mines_set++;
            } else {
                continue;
            }

        }
        calculateNeighbors();
        calculateTileStates();
    }

    void calculateNeighbors() {
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numColumns; j++) {
                Tile& currentTile = tiles[i][j];

                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++){

                        if (dx == 0 && dy == 0) {
                            continue;
                        }

                        int neighborX = j + dx;
                        int neighborY = i + dy;

                        if (neighborX >= 0 && neighborX < numColumns && neighborY >= 0 && neighborY < numRows) {
                            Tile& neighborTile = tiles[neighborY][neighborX];
                            currentTile.neighbors.push_back(&neighborTile);
                        }
                    }
                }
            }
        }
    }

    void calculateTileStates() {
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numColumns; j++) {
                Tile& currentTile = tiles[i][j];
                int mineCount = 0;

                for (Tile* neighborTile : currentTile.neighbors) {
                    if (neighborTile->isMine) {
                        mineCount++;
                    }
                }
                currentTile.tilestate = mineCount;
            }
        }
    }

   //this code should take in x and y vals to recursively reveal all the tiles
    void revealNeighborsrecursive(Tile& tile){
        if (tile.isMine || tile.flagged){
            return;
        }


        tile.revealed = true;

        for (Tile* neighborTile : tile.neighbors){
            if (!neighborTile->revealed && tile.tilestate ==0){
                revealNeighborsrecursive(*neighborTile);
            }
        }
    }

    void revealTilesInitial(int x, int y){
        if(tiles[y][x].flagged){
            return;
        }
        if (tiles[y][x].isMine==false && tiles[y][x].tilestate == 0){ //fixed the if statement so that it will only go
            revealNeighborsrecursive(tiles[y][x]); // to the recursive revealer if both a mine AND has 0 neighbors
        } else if (tiles[y][x].isMine){
            tiles[y][x].revealed = true;
            tiles[y][x].tilestate = 0;
            gameover = true;
        }else {
            tiles[y][x].revealed = true;
        }
    }
    // end of the code that reveals tiels

    void debug(sf::RenderWindow &win){
        for(int i=0; i< numColumns; i++){
            for(int j = 0; j< numRows; j++){
                if(tiles[j][i].isMine){
                    sf::Texture holder;
                    holder.loadFromFile("files/images/tile_hidden.png");
                    sf::Sprite sprite;
                    sprite.setTexture(holder);
                    sprite.setPosition(i*32, j*32);
                    win.draw(sprite);
                    mine.setTexture(icontextures["mine"]);
                    mine.setPosition(i*32, j*32);
                    win.draw(mine);
                }


            }
        }
    }

    void flagger(int x, int y){
        if(tiles[y][x].flagged == true){
            tiles[y][x].flagged = false;
            flags--;
            cout<<flags<<endl;

        }else{
            tiles[y][x].flagged = true;
            flags++;
            cout<<flags<<endl;
        }

    }

    bool wincondition(){
        int counter=0;
        for(int i=0; i< numColumns; i++) {
            for (int j = 0; j < numRows; j++) {
                if((tiles[j][i].revealed) && (!tiles[j][i].isMine)){
                    counter++;
                }
            }
        }
        if(counter== (numRows*numColumns-numMines)){
            return true;
        }else{
            return false;
        }
    }

    void setSmiley(sf::RenderWindow &window){
        if(gameover){
            lose.setTexture(icontextures["lose"]);
            lose.setPosition(((numColumns/2.0f)*32)-32, 32*(numRows + 0.5));
            window.draw(lose);
            debug(window);
        }else if(wincondition()){
            gameWon = true;
            win.setTexture(icontextures["win"]);
            win.setPosition(((numColumns/2.0f)*32)-32, 32*(numRows + 0.5));
            window.draw(win);
            showleaders = true;


            for(int i=0; i< numColumns; i++) {
                for (int j = 0; j < numRows; j++) {
                    if(tiles[j][i].isMine){
                        flag.setTexture(icontextures["flag"]);
                        flag.setPosition(i*32, j*32);
                        window.draw(flag);
                    }
                }
            }
        }else{
            happy.setTexture(icontextures["happy"]);
            happy.setPosition(((numColumns/2.0f)*32)-32, 32*(numRows + 0.5));
            window.draw(happy);
        }
    }


    void gamePause(sf::RenderWindow &win){
        for(int i=0; i< numColumns; i++) {
            for (int j = 0; j < numRows; j++) {
                sf::Texture holder;
                holder.loadFromFile("files/images/tile_revealed.png");
                sf::Sprite sprite;
                sprite.setTexture(holder);
                sprite.setPosition(i * 32, j * 32);
                win.draw(sprite);
            }
        }
    }

};