#include "Piece.hpp"
#include "Elephant.hpp"
#include "Rhinoceros.hpp"
#include "Lion.hpp"
#include "Case.hpp"
#include "Barriere.hpp"
#include "Player.hpp"
#include "Board.hpp"
#include "Game.hpp"




int main() {
    
    
    // Création d'un pion (à remplacer par des éléphants, rhinocéros, lions)
    Piece RPiece('R', 0, 0);
    Piece RPiece2('R', 0, 1);
    Piece RPiece3('R', 0, 2);

    Piece LPiece('L', 1, 0);
    Piece LPiece2('L', 1, 1);
    Piece LPiece3('L', 1, 2);

    Piece EPiece('E', 2, 0);
    Piece EPiece2('E', 2, 1);
    Piece EPiece3('E', 2, 2);

    Case case1;
    case1.setPiece(&RPiece);
    
    Barriere b;
    b.setDroite(true);
    b.setGauche(true);
    
    Player p(0,'R');

    Board board ;

    std::cout << board << std::endl;

    board.placePiece(0,0,RPiece);
    board.placePiece(2,2,RPiece2);
    board.placePiece(3,5,RPiece3);

    board.placePiece(1,1,LPiece);
    board.placePiece(2,6,LPiece2);
    board.placePiece(4,3,LPiece3);

    board.placePiece(3,1,EPiece);
    board.placePiece(7,6,EPiece2);
    board.placePiece(6,2,EPiece3);



    board.addBarrier(1,0,'H');
    board.addBarrier(0,0,'D');
    board.addBarrier(1,5,'H');
    board.addBarrier(1,6,'H');
    board.addBarrier(2,0,'D');


    std::cout << board << std::endl;
    

    /*
    Game game;
    game.start();
    */



    return 0;
}
