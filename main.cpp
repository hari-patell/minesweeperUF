#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cctype>
#include <string>
#include "game.cpp"
#include "leaderboard.cpp"
#include <chrono>


using namespace std;

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}



int main() {
    std::ifstream inputFile("files/config.cfg");
    bool stop = true;

    int numCol, numRows, numMines;
    inputFile >> numCol >> numRows >> numMines;
    int height = (numRows * 32) + 100;
    int width = numCol * 32;








    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);


    sf::Font font;
    font.loadFromFile("files/font.ttf");

    //"Welcome" text created
    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width / 2, (height / 2) - 150);

    //"Enter your name" text created
    sf::Text nameText("Enter your name:", font, 20);
    nameText.setFillColor(sf::Color::White);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, width / 2, (height / 2) - 75);

    //"Name input" text created
    sf::Text inputText("|", font, 20);
    inputText.setFillColor(sf::Color::White);
    setText(inputText, width / 2 - 10, (height / 2)-45);
    string playerName;

    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                stop = false;
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && playerName.size()>0){
                window.close();
            }
            else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char typedChar = static_cast<char>(event.text.unicode);
                    if (std::isalpha(typedChar) && playerName.size() < 10) {
                        if (playerName.empty()) { // First letter, capitalize
                            playerName += std::toupper(typedChar);
                        }
                        else {
                            playerName += std::tolower(typedChar);
                        }
                        inputText.setString(playerName + "|");
                        setText(inputText, width / 2 - 10, (height / 2)-45);

                    }
                    else if (event.text.unicode == 8 && !playerName.empty()) { // Backspace
                        playerName.pop_back();
                        inputText.setString(playerName + "|");
                        setText(inputText, width / 2 - 10, (height / 2)-45);

                    }
                }
            }
        }

        // Clear the window
        window.clear(sf::Color::Blue);

        // Draw text objects
        window.draw(welcomeText);
        window.draw(nameText);
        window.draw(inputText);

        window.display();
    }

    if (!stop) {
        return 0; //so that the next window isn't opened unless we clicked enter
    }




    //GAME WINDOW
    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);



    Board board(numCol, numRows, numMines);
    board.setmines();
    bool debugTrue = false;
    bool pauseBool = false;
    bool firstClick = true;
    bool openLeaderboard = false;
    int counter=0;


    //texture for debug button
    sf::Texture temp;
    temp.loadFromFile("files/images/debug.png");
    sf::Sprite debugger;
    debugger.setTexture(temp);
    debugger.setPosition((numCol*32)-304, 32*(numRows+0.5f));

    sf::Texture leaderboardTexture;
    leaderboardTexture.loadFromFile("files/images/leaderboard.png");
    sf::Sprite leaderboardButton;
    leaderboardButton.setTexture(leaderboardTexture);
    leaderboardButton.setPosition((numCol * 32) - 176, 32 * (numRows + 0.5));

    sf::Texture temporary;
    temporary.loadFromFile("files/images/pause.png");
    sf::Sprite pause;
    pause.setTexture(temporary);
    pause.setPosition((numCol*32)-240, 32*(numRows + 0.5f));

    sf::Texture temporary1;
    temporary1.loadFromFile("files/images/play.png");
    sf::Sprite play;
    play.setTexture(temporary1);
    play.setPosition((numCol*32)-240, 32*(numRows + 0.5f));


    auto start = chrono::high_resolution_clock::now();
    auto pauseStart = chrono::high_resolution_clock::now();
    int elapsedMinutes = 0;
    int elapsedSeconds = 0;
    int pause_seconds = 0;
    int total_seconds = 0;



    while (gameWindow.isOpen()) {


        // Process events
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
        }


        gameWindow.clear(sf::Color::White);
        gameWindow.draw(leaderboardButton);
        gameWindow.draw(debugger);
        gameWindow.draw(pause);


        //check to reveal
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            sf::Vector2f worldMousePosition = gameWindow.mapPixelToCoords(mousePosition);

            int row = worldMousePosition.y / 32;
            int column = worldMousePosition.x / 32;

            if (row < (numRows) && column < (numCol) && !board.gameover && !pauseBool) {
                if (firstClick){
                    firstClick = false;
                    if(board.tiles[row][column].tilestate != 0 || board.tiles[row][column].isMine){
                        while (board.tiles[row][column].tilestate != 0 || board.tiles[row][column].isMine) {
                            board = Board(numCol, numRows, numMines);
                            board.setmines();
                        }
                         // Update firstClick to false after ensuring the first click condition
                    }
                }
                cerr << "Left click at Row: " << row << ", Column: " << column << endl;
                board.revealTilesInitial(column, row);
            }



        }

        //flag maker
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            sf::Vector2f worldMousePosition = gameWindow.mapPixelToCoords(mousePosition);

            int row = worldMousePosition.y / 32;
            int column = worldMousePosition.x / 32;

            if(row < (numRows) && column < (numCol) && !firstClick && !pauseBool){
                cerr << "Row: " << row << ", Column: " << column << endl;
                board.flagger(column, row);
            }

        }

        //debug thingy
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            sf::Vector2f worldMousePosition = gameWindow.mapPixelToCoords(mousePosition);
            if (mousePosition.x >= debugger.getPosition().x && mousePosition.x <= debugger.getPosition().x + debugger.getGlobalBounds().width &&
                mousePosition.y >= debugger.getPosition().y && mousePosition.y <= debugger.getPosition().y + debugger.getGlobalBounds().height) {
                cerr << "Debug Button clicked" << endl;
                if(debugTrue){
                    debugTrue = false;
                }else{
                    debugTrue = true;
                }
            }
        }

        //play and pause button
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            sf::Vector2f worldMousePosition = gameWindow.mapPixelToCoords(mousePosition);
            if (pause.getGlobalBounds().contains(worldMousePosition) && !board.gameover && !board.gameWon) {
                cerr << "Pause/Play Button clicked" << endl;
                pauseBool = !pauseBool; // Toggle the pauseBool variable
                debugTrue = false;
                if(pauseBool){
                    pauseStart = chrono::high_resolution_clock::now();
                } else{
                    auto pauseEnd = chrono::high_resolution_clock::now();
                    chrono::duration<double> elapsed_pause_seconds = pauseEnd - pauseStart;
                    pause_seconds += static_cast<int>(elapsed_pause_seconds.count());

                }
            }
        }



        //if the win, lose, or happy sprites are clicked
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            sf::Vector2f worldMousePosition = gameWindow.mapPixelToCoords(mousePosition);
            if (board.lose.getGlobalBounds().contains(worldMousePosition) || board.win.getGlobalBounds().contains(worldMousePosition) || board.happy.getGlobalBounds().contains(worldMousePosition)) {
                board = Board(numCol, numRows, numMines);
                firstClick = true;
                board.setmines();
                start = chrono::high_resolution_clock::now(); // Reset the start time
                pause_seconds = 0;

            }

        }


        //leaderboard button pressed
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
            sf::Vector2i mousePosition = sf::Mouse::getPosition(gameWindow);
            sf::Vector2f worldMousePosition = gameWindow.mapPixelToCoords(mousePosition);
            if (leaderboardButton.getGlobalBounds().contains(worldMousePosition)) {
                pauseBool = !pauseBool;
                debugTrue = false;
                openLeaderboard = !openLeaderboard;
                cout<<"i have been clicked sire" << endl;
                if(pauseBool){
                    pauseStart = chrono::high_resolution_clock::now();
                }

            }

        }



        //updates timer only if game isn't paused and game isn't over and game isn't won.
        if (!pauseBool && !board.gameover && !board.gameWon) {
            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            total_seconds = static_cast<int>(elapsed_seconds.count());
            total_seconds = total_seconds - pause_seconds;
            elapsedMinutes = total_seconds / 60;
            elapsedSeconds = total_seconds % 60;
        }



        board.timerDraw(gameWindow, elapsedMinutes, elapsedSeconds);





        if(pauseBool && !debugTrue && !board.gameover && !board.gameWon){
            gameWindow.clear(sf::Color::White);
            gameWindow.draw(debugger);
            gameWindow.draw(leaderboardButton);
            gameWindow.draw(play);
            board.boardDraw(gameWindow);
            board.gamePause(gameWindow);
            board.setSmiley(gameWindow);
            board.timerDraw(gameWindow, elapsedMinutes, elapsedSeconds);
        }else if(debugTrue){
            gameWindow.draw(pause);
            gameWindow.draw(leaderboardButton);
            board.boardDraw(gameWindow);
            board.debug(gameWindow);
            board.setSmiley(gameWindow);
            board.timerDraw(gameWindow, elapsedMinutes, elapsedSeconds);
        }else if(!pauseBool && !debugTrue){
            gameWindow.clear(sf::Color::White);
            gameWindow.draw(debugger);
            gameWindow.draw(pause);
            gameWindow.draw(leaderboardButton);
            board.boardDraw(gameWindow);
            board.setSmiley(gameWindow);
            board.timerDraw(gameWindow, elapsedMinutes, elapsedSeconds);
        }
        gameWindow.display();



        //THIS IS FOR THE LEADERBOARD
        if(openLeaderboard){
            sf::RenderWindow leaderboardWindow(sf::VideoMode(numCol*16, (numRows*16)+50), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
            Leaderboard leaderboard;
            cout << "initial" << endl;
            leaderboard.read();
            leaderboard.sort();
            if(board.gameWon){
                leaderboard.addEntry(elapsedMinutes, elapsedSeconds, playerName);
                leaderboard.read();
                leaderboard.sort();
            }

            while (leaderboardWindow.isOpen()){
                sf::Event event1;
                while (leaderboardWindow.pollEvent(event1)) {
                    if (event1.type == sf::Event::Closed) {
                        auto pauseEnd = chrono::high_resolution_clock::now();
                        chrono::duration<double> elapsed_pause_seconds = pauseEnd - pauseStart;
                        pause_seconds += static_cast<int>(elapsed_pause_seconds.count());
                        openLeaderboard = !openLeaderboard;
                        pauseBool = false;
                        leaderboardWindow.close();
                    }
                }
                // Clear the leaderboardWindow
                leaderboardWindow.clear(sf::Color::Blue);

                sf::Text leaderTitle("LEADERBOARD", font, 20);
                leaderTitle.setFillColor(sf::Color::White);
                leaderTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
                setText(leaderTitle, (numCol*16)/2, (((numRows*16)+50)/2) - 120);

                sf::Text summary(leaderboard.generateSummary(), font, 18);
                summary.setFillColor(sf::Color::White);
                summary.setStyle(sf::Text::Bold);
                setText(summary, (numCol*16)/2, (((numRows*16)+50)/2) +20);


                leaderboardWindow.draw(leaderTitle);
                leaderboardWindow.draw(summary);
                leaderboardWindow.display();

            }
        }




    }



    return 0;
}



