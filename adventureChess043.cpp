#include "SFML/Graphics.hpp"
//#include <SFML/Audio.hpp>
#include <time.h>
#include <bits/stdc++.h>
#include <boost/asio.hpp>
#include <ctime>
#include <unistd.h>
#include <fstream>
//#include <boost/lexical_cast>




class GameBoard
{
private:
	bool debug = false;
	public:
		int height;
		int width;
		int rows;
		int cols;
		int tile_size;
		sf::Vector2f offset;

		std::vector<std::vector<int>> board;
		std::vector<std::vector<bool>> visible;
		sf::Vector2i viewPiece;
		std::vector<sf::Vector2i> validMoves;

		bool didILose;

	GameBoard()
	{
		rows = 15;
		cols = 8;
		tile_size = 56;
		height = rows * tile_size;
		width = cols * tile_size;
		offset.x = 56;
		offset.y = 56;
		std::vector<std::vector<int>> vec( rows , std::vector<int> (cols, 0));
		board = vec;
		std::vector<std::vector<bool>> vec2( rows, std::vector<bool> (cols, debug));
		visible = vec2;
	}

	int getX (int x) const
	{
		return tile_size*x + offset.x;
	}

	int getY (int y) const
	{
		return tile_size*y + offset.y;
	}

	bool checkValid( sf::Vector2i move )
	{
		for( int i=0; i < validMoves.size(); i++ )
		{
			if( move == validMoves[i] ){ return true; }
		}
		return false;
	}

	void updateVisible()
	{
		int scoutRange = 3;
		for( int i=0; i<rows; i++ )
		{
			for( int j=0; j<cols; j++ )
			{
				if( board[i][j] == 11 )
				{
					for( int a=0; a<rows; a++ )
					{
						for( int b=0; b<cols; b++ )
						{
							if( abs(i-a)<3 && abs(j-b)<3 )
							{
								visible[a][b]=true;
							}
						}
					}
				}
				else if( board[i][j] > 0 )
				{
					int y=i,x=j;
					if( x > 0 && y > 0 )
					{
						visible[ y - 1 ][ x - 1 ] = true;
					}

					if( x > 0 && y < rows - 1 )
					{
						visible[ y + 1 ][ x - 1 ] = true;
					}

					if( x < cols - 1 && y < rows - 1 )
					{
						visible[ y + 1 ][ x + 1 ] = true;
					}

					if( x < cols - 1 && y > 0 )
					{
						visible[ y - 1 ][ x + 1 ] = true;
					}

					if( x < cols -1 )
					{
						visible[ y ][ x + 1 ] = true;
					}

					if( y < rows -1 )
					{
						visible[ y + 1 ][ x ] = true;
					}

					if( y > 0 )
					{
						visible[ y - 1 ][ x ] = true;
					}

					if( x > 0 )
					{
						visible[ y ][ x - 1 ] = true;
					}
				}
			}
		}
	}

	bool isGameOver()
	{
		bool flag1=false,flag2=false;
		for( int i=0; i<rows; i++)
		{
			for( int j=0; j<cols; j++)
			{
				if( board[i][j] ==  4 ){ flag1 = true; }
				if( board[i][j] == -4 ){ flag2 = true; }
			}
		}
		didILose = !flag1;
		return !(flag1&flag2);
	}

};

namespace GameTest
{
	static int testno = 0;

	void test()
	{
		std::cout<<"test "<<testno++<<std::endl;
	}

	void test(std::string s)
	{
		std::cout<<"test "<<s<<std::endl;
	}

	void display(const GameBoard& chessBoard)
	{
		for(int i=0;i<chessBoard.rows;i++){
    		for(int j=0;j<chessBoard.cols;j++){
    			std::cout<<chessBoard.board[i][j]<<' ';
    		}std::cout<<std::endl;
    	}
	}

	void testSet(GameBoard& chessBoard)
	{
		for(int i=0;i<chessBoard.cols;i++){chessBoard.board[chessBoard.rows-3][i]=6;}
    	for(int i=1;i<6;i++){chessBoard.board[chessBoard.rows-1][i-1]=i;}
    	chessBoard.board[chessBoard.rows-1][5]=3;
    	chessBoard.board[chessBoard.rows-1][6]=2;
    	chessBoard.board[chessBoard.rows-1][7]=1;
    	for(int i=0;i<6;i++){chessBoard.board[chessBoard.rows-2][i]=7+i;}
    	//chessBoard.board[1][1]=2;
    	//chessBoard.board[4][7]=5;
    	//chessBoard.board[3][7]=3;
    	//for(int i=0;i<chessBoard.cols;i++){chessBoard.board[2][i]=-6;}
    	//for(int i=1;i<6;i++){chessBoard.board[0][i-1]=-i;}
	}
}

class GamePieces
{
private:
	char myColor;
	char opColor;
public:
	const int capital = 14 + 126;
	std::vector<int> limits;
	std::vector<std::string> codes;
	std::vector<int> costs;

	GamePieces()
	{
		generateCodes();
		generateCosts();
		generateLimits();
	}


	void setColor(const bool x)
	{
		myColor = x?'w':'b';
		opColor = x?'b':'w';
	}

	void generateLimits()
	{
		limits.assign(13,0);
		limits[1] = 2;
		limits[2] = 2;
		limits[3] = 2;
		limits[4] = 0;
		limits[5] = 0;
		limits[6] = 2;

		limits[7] = 2;
		limits[8] = 2;
		limits[9] = 1;
		limits[10]= 1;
		limits[11]= 10;
		limits[12]= 1;
	}

	void generateCodes()
	{
		codes.assign(13,"null");
		codes[1] = "R";
		codes[2] = "B";
		codes[3] = "Kn";
		codes[4] = "K";
		codes[5] = "Q";
		codes[6] = "P";

		codes[7] = "As";
		codes[8] = "Ar";
		codes[9] = "Ne";
		codes[10]= "Wz";
		codes[11]= "Sc";
		codes[12]= "Gh";
	}

	void generateCosts()
	{
		costs.assign(13,0);
		costs[1] = 7;
		costs[2] = 5;
		costs[3] = 4;
		costs[4] = 100;
		costs[5] = 20;
		costs[6] = 1;

		costs[7] = 3;
		costs[8] = 4;
		costs[9] = 5;
		costs[10]= 3;
		costs[11]= 2;
		costs[12]= 4;
	}

	std::string IdToString(int x)
	{
		return codes[x];
	}

	int revalPoints( const GameBoard& context )
	{
		int remaining = capital;
		for(int i=context.rows-3;i<context.rows;i++)
		{
			for(int j=0;j<context.cols;j++)
			{
				remaining -= costs[context.board[i][j]];
			}
		}
		return remaining;
	}

	void Validate( const sf::Vector2i pos, GameBoard& context )
	{
		//std::cout<<pos.x<<' '<<pos.y<<std::endl;
		sf::Vector2i switcher = sf::Vector2i(pos.y,pos.x);
		if( pos == context.viewPiece ){ context.validMoves.clear();    return; }

		if( context.board[ pos.x ][ pos.y ] == 6 ){ context.validMoves = pawnValid(     switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 1 ){ context.validMoves = rookValid(     switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 2 ){ context.validMoves = knightValid(   switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 3 ){ context.validMoves = bishopValid(   switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 4 ){ context.validMoves = kingValid(     switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 5 ){ context.validMoves = queenValid(    switcher, context ); visibleValid(context); return; }

		if( context.board[ pos.x ][ pos.y ] == 7  ){ context.validMoves = assasinValid( switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 8  ){ context.validMoves = archerValid(  switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 9  ){ context.validMoves = necroValid(   switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 10 ){ context.validMoves = wizardValid(  switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 11 ){ context.validMoves = scoutValid(   switcher, context ); visibleValid(context); return; }
		if( context.board[ pos.x ][ pos.y ] == 12 ){ context.validMoves = ghostValid(   switcher, context ); visibleValid(context); return; }

		context.validMoves.clear();
	}

	void visibleValid( GameBoard& context )
	{

		std::vector<sf::Vector2i> moves;

		for( int i=0; i<context.validMoves.size(); i++ )
		{
			if( context.visible[context.validMoves[i].y][context.validMoves[i].x] == true )
			{
				moves.push_back(sf::Vector2i( context.validMoves[i].x, context.validMoves[i].y ));
			}
		}

		context.validMoves = moves;
	}

	std::vector<sf::Vector2i> pawnValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		sf::Vector2i temp;

		//std::cout<<"pawn"<<std::endl;

		if( pos.y < context.rows - 1 )
		{
			if( context.board[ pos.y - 1 ][ pos.x] == 0 )
			{
				temp = sf::Vector2i( pos.x, pos.y - 1 );
				moves.push_back( temp );
			}

			if( pos.x > 0 && context.board[ pos.y - 1 ][ pos.x - 1 ] < 0)
			{
				temp = sf::Vector2i( pos.x - 1, pos.y - 1 );
				moves.push_back( temp );
			}
 
			if( pos.x < context.cols - 1 && context.board[ pos.y - 1 ][ pos.x + 1 ] < 0 )
			{
				temp = sf::Vector2i( pos.x + 1, pos.y - 1 );
				moves.push_back( temp );
			}
		}
		return moves;
	}

	std::vector<sf::Vector2i> rookValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;

		//std::cout<<"rook"<<std::endl;

		//std::cout<<pos.x<<" "<<pos.y<<std::endl;


		for(int i = 1; pos.x + i < context.cols; i++ )
		{
			if( context.board[ pos.y ][ pos.x + i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x + i , pos.y ) );

			if( context.board[ pos.y ][ pos.x + i ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; pos.x - i > -1; i++ )
		{
			if( context.board[ pos.y ][ pos.x - i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x - i , pos.y ) );

			if( context.board[ pos.y ][ pos.x - i ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; pos.y + i < context.rows; i++ )
		{
			if( context.board[ pos.y + i ][ pos.x ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x , pos.y + i ) );

			if( context.board[ pos.y + i ][ pos.x ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; pos.y - i > -1; i++ )
		{
			if( context.board[ pos.y - i ][ pos.x ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x , pos.y - i ) );

			if( context.board[ pos.y - i ][ pos.x ] < 0 )
			{
				break;
			}
		}

		return moves;
		
	}

	std::vector<sf::Vector2i> knightValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;

		//std::cout<<"knight"<<std::endl;

		if( pos.x > 1 )
		{
			if( pos.y > 0 )
			{
				if(context.board[ pos.y - 1 ][ pos.x - 2 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x - 2 , pos.y - 1 ) );
				}
			}

			if( pos.y < context.rows - 1 )
			{
				if(context.board[ pos.y + 1 ][ pos.x - 2 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x - 2 , pos.y + 1 ) );
				}
			}

		}

		if( pos.x < context.cols - 2 )
		{
			if( pos.y > 0 )
			{
				if(context.board[ pos.y - 1 ][ pos.x + 2 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x + 2 , pos.y - 1 ) );
				}
			}

			if( pos.y < context.rows - 1 )
			{
				if(context.board[ pos.y + 1 ][ pos.x + 2 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x + 2 , pos.y + 1 ) );
				}
			}

		}

		if( pos.y > 1 )
		{
			if( pos.x > 0 )
			{
				if(context.board[ pos.y - 2 ][ pos.x - 1 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x - 1 , pos.y - 2 ) );
				}
			}

			if( pos.x < context.cols - 1 )
			{
				if(context.board[ pos.y - 2 ][ pos.x + 1 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x + 1 , pos.y - 2 ) );
				}
			}

		}

		if( pos.y < context.rows - 2 )
		{

			if( pos.x > 0 )
			{
				if(context.board[ pos.y + 2 ][ pos.x - 1 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x - 1 , pos.y + 2 ) );
				}
			}

			if( pos.x < context.cols - 1 )
			{
				if(context.board[ pos.y + 2 ][ pos.x + 1 ] < 1 )
				{
					moves.push_back( sf::Vector2i( pos.x + 1 , pos.y + 2 ) );
				}
			}

		}


		return moves;
	}

	std::vector<sf::Vector2i> bishopValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;

		//std::cout<<"bishop"<<std::endl;

		for(int i = 1; ((pos.x + i) < context.cols) && ((pos.y + i) < context.rows); i++ )
		{
			if( context.board[ pos.y + i ][ pos.x + i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x + i , pos.y + i ) );

			if( context.board[ pos.y + i ][ pos.x + i ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; pos.x + i < context.cols && pos.y - i > -1; i++ )
		{
			if( context.board[ pos.y - i ][ pos.x + i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x + i , pos.y - i ) );

			if( context.board[ pos.y - i ][ pos.x + i ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; pos.x - i > - 1 && pos.y + i < context.rows; i++ )
		{
			if( context.board[ pos.y + i ][ pos.x - i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x - i , pos.y + i ) );

			if( context.board[ pos.y + i ][ pos.x - i ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; pos.x - i > - 1 && pos.y - i > - 1; i++ )
		{
			if( context.board[ pos.y - i ][ pos.x - i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x - i , pos.y - i ) );

			if( context.board[ pos.y - i ][ pos.x - i ] < 0 )
			{
				break;
			}
		}


		return moves;
	}

	std::vector<sf::Vector2i> queenValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		std::vector<sf::Vector2i> rookMoves = rookValid( pos, context );
		std::vector<sf::Vector2i> bishopMoves = bishopValid( pos, context );
		moves.insert( moves.end(), rookMoves.begin(), rookMoves.end() );
		moves.insert( moves.end(), bishopMoves.begin(), bishopMoves.end() );

		return moves;
	}

	std::vector<sf::Vector2i> kingValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		if( pos.x > 0 && pos.y > 0 )
		{
			if( context.board[ pos.y - 1 ][ pos.x - 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x - 1 , pos.y - 1 ) ); }
		}

		if( pos.x > 0 && pos.y < context.rows - 1 )
		{
			if( context.board[ pos.y + 1 ][ pos.x - 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x - 1 , pos.y + 1 ) ); }
		}

		if( pos.x < context.cols - 1 && pos.y < context.rows - 1 )
		{
			if( context.board[ pos.y + 1 ][ pos.x + 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x + 1 , pos.y + 1 ) ); }
		}

		if( pos.x < context.cols - 1 && pos.y > 0 )
		{
			if( context.board[ pos.y - 1 ][ pos.x + 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x + 1 , pos.y - 1 ) ); }
		}

		if( pos.x < context.cols -1 )
		{
			if( context.board[ pos.y ][ pos.x + 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x + 1 , pos.y ) ); }
		}

		if( pos.y < context.rows -1 )
		{
			if( context.board[ pos.y + 1 ][ pos.x ] < 1 ){ moves.push_back( sf::Vector2i( pos.x , pos.y + 1 ) ); }
		}

		if( pos.y > 0 )
		{
			if( context.board[ pos.y - 1 ][ pos.x ] < 1 ){ moves.push_back( sf::Vector2i( pos.x , pos.y - 1 ) ); }
		}

		if( pos.x > 0 )
		{
			if( context.board[ pos.y ][ pos.x - 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x - 1, pos.y ) ); }
		}

		return moves;
	}

	std::vector<sf::Vector2i> assasinValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		int range = 4;
		for(int i=1; (pos.y - i > -1) && (i<range); i++)
		{
			if( context.board[ pos.y - i ][ pos.x ] < 1 ){ moves.push_back( sf::Vector2i( pos.x , pos.y - i ) ); }
		}

		if( pos.y > 0 )
		{
			if( pos.x > 0)
			{
				if( context.board[ pos.y - 1 ][ pos.x - 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x - 1, pos.y - 1 ) ); }		
			}
			if( pos.x < context.cols - 1 )
			{
				if( context.board[ pos.y - 1 ][ pos.x + 1 ] < 1 ){ moves.push_back( sf::Vector2i( pos.x + 1, pos.y - 1 ) ); }
			}	
			
		}
		return moves;
	}

	std::vector<sf::Vector2i> archerValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		int range = 3;

		if( pos.x < context.cols -1 )
		{
			if( context.board[ pos.y ][ pos.x + 1 ] == 0 ){ moves.push_back( sf::Vector2i( pos.x + 1 , pos.y ) ); }
		}

		if( pos.y < context.rows -1 )
		{
			if( context.board[ pos.y + 1 ][ pos.x ] == 0 ){ moves.push_back( sf::Vector2i( pos.x , pos.y + 1 ) ); }
		}

		if( pos.y > 0 )
		{
			if( context.board[ pos.y - 1 ][ pos.x ] == 0 ){ moves.push_back( sf::Vector2i( pos.x , pos.y - 1 ) ); }
		}

		if( pos.x > 0 )
		{
			if( context.board[ pos.y ][ pos.x - 1 ] == 0 ){ moves.push_back( sf::Vector2i( pos.x - 1, pos.y ) ); }
		}


		for(int i = 1; ((pos.x + i) < context.cols) && ((pos.y + i) < context.rows) && (i<range); i++ )
		{
			if( context.board[ pos.y + i ][ pos.x + i ] > 0 )
			{
				break;
			}

			if( context.board[ pos.y + i ][ pos.x + i ] < 0 )
			{
				moves.push_back( sf::Vector2i( pos.x + i , pos.y + i ) );
				break;
			}
		}

		for(int i = 1; (pos.x + i < context.cols) && (pos.y - i > -1) && (i<range); i++ )
		{
			if( context.board[ pos.y - i ][ pos.x + i ] > 0 )
			{
				break;
			}

			if( context.board[ pos.y - i ][ pos.x + i ] < 0 )
			{
				moves.push_back( sf::Vector2i( pos.x + i , pos.y - i ) );
				break;
			}
		}

		for(int i = 1; (pos.x - i > - 1) && (pos.y + i < context.rows) && (i<range); i++ )
		{
			if( context.board[ pos.y + i ][ pos.x - i ] > 0 )
			{
				break;
			}

			if( context.board[ pos.y + i ][ pos.x - i ] < 0 )
			{
				moves.push_back( sf::Vector2i( pos.x - i , pos.y + i ) );
				break;
			}
		}

		for(int i = 1; (pos.x - i > - 1) && (pos.y - i > - 1) && (i<range); i++ )
		{
			if( context.board[ pos.y - i ][ pos.x - i ] > 0 )
			{
				break;
			}

			if( context.board[ pos.y - i ][ pos.x - i ] < 0 )
			{
				moves.push_back( sf::Vector2i( pos.x - i , pos.y - i ) );
				break;
			}
		}



		return moves;
	}

	std::vector<sf::Vector2i> necroValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		//std::cout<<"necro"<<std::endl;

		if( pos.y > 1 )
		{

			if( pos.x > 0 && context.board[ pos.y - 1 ][ pos.x - 1 ] < 1 )
			{
				moves.push_back( sf::Vector2i( pos.x - 1, pos.y - 1 ) );
			}
 
			if( pos.x < context.cols - 1 && context.board[ pos.y - 1 ][ pos.x + 1 ] < 1 )
			{
				moves.push_back( sf::Vector2i( pos.x + 1, pos.y - 1 ) );
			}
		}
		/*
		if( pos.y < context.rows - 1 )
		{

			if( pos.x > 0 && context.board[ pos.y + 1 ][ pos.x - 1 ] < 0 )
			{
				moves.push_back( sf::Vector2i( pos.x - 1, pos.y + 1 ) );
			}
 
			if( pos.x < context.cols - 1 && context.board[ pos.y + 1 ][ pos.x + 1 ] < 0 )
			{
				moves.push_back( sf::Vector2i( pos.x + 1, pos.y + 1 ) );
			}
		}
		*/


		return moves;
	}

	std::vector<sf::Vector2i> wizardValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;

		for(int i=0; i<context.rows; i++)
		{
			for(int j=0; j<context.cols; j++)
			{
				if( context.board[i][j] != 10 && context.board[i][j] > 0 )
				{
					moves.push_back( sf::Vector2i( j, i ) );
				}
			}
		}

		return moves;
	}

	std::vector<sf::Vector2i> scoutValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		int range=3;
		for(int i=0; i<context.rows; i++)
		{
			for(int j=0; j<context.cols; j++)
			{
				if( abs(i-pos.y)<range && abs(j-pos.x)<range && context.board[i][j] == 0 )
				{
					moves.push_back( sf::Vector2i( j, i ) );
				}
			}
		}

		return moves;
	}

	std::vector<sf::Vector2i> ghostValid( const sf::Vector2i pos, const GameBoard& context )
	{
		std::vector<sf::Vector2i> moves;
		int hrange = 3;
		int vrange = 3;

		for(int i = 1; (pos.x + i < context.cols) && (i<hrange); i++ )
		{
			if( context.board[ pos.y ][ pos.x + i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x + i , pos.y ) );

			if( context.board[ pos.y ][ pos.x + i ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; (pos.x - i > -1) && (i<hrange); i++ )
		{
			if( context.board[ pos.y ][ pos.x - i ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x - i , pos.y ) );

			if( context.board[ pos.y ][ pos.x - i ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; (pos.y + i < context.rows) && (i<vrange); i++ )
		{
			if( context.board[ pos.y + i ][ pos.x ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x , pos.y + i ) );

			if( context.board[ pos.y + i ][ pos.x ] < 0 )
			{
				break;
			}
		}

		for(int i = 1; (pos.y - i > -1) && (i<hrange); i++ )
		{
			if( context.board[ pos.y - i ][ pos.x ] > 0 )
			{
				break;
			}

			moves.push_back( sf::Vector2i( pos.x , pos.y - i ) );

			if( context.board[ pos.y - i ][ pos.x ] < 0 )
			{
				break;
			}
		}

		return moves;
	}

	void moveHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		if( context.board[from.y][from.x] == 1  ){ rookHandle(    context, from, to ); }
		if( context.board[from.y][from.x] == 2  ){ knightHandle(  context, from, to ); }
		if( context.board[from.y][from.x] == 3  ){ bishopHandle(  context, from, to ); }
		if( context.board[from.y][from.x] == 4  ){ kingHandle(    context, from, to ); }
		if( context.board[from.y][from.x] == 5  ){ queenHandle(   context, from, to ); }
		if( context.board[from.y][from.x] == 6  ){ pawnHandle(    context, from, to ); }
		if( context.board[from.y][from.x] == 7  ){ assasinHandle( context, from, to ); }
		if( context.board[from.y][from.x] == 8  ){ archerHandle(  context, from, to ); }
		if( context.board[from.y][from.x] == 9  ){ necroHandle(   context, from, to ); }
		if( context.board[from.y][from.x] == 10 ){ wizardHandle(  context, from, to ); }
		if( context.board[from.y][from.x] == 11 ){ scoutHandle(   context, from, to ); }
		if( context.board[from.y][from.x] == 12 ){ ghostHandle(   context, from, to ); }
	}

	void pawnHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 6;
	}

	void rookHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 1;
	}

	void knightHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 2;
	}

	void bishopHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 3;
	}

	void queenHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 5;
	}

	void kingHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 4;
	}

	void assasinHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 7;
	}

	void archerHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		if( from.x != to.x && from.y != to.y )
		{
			context.board[from.y][from.x] = 8;
			context.board[to.y][to.x]     = 0;
			return;
		}
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 8;
	}

	void necroHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		if( context.board[to.y][to.x] < 0 )
		{
			context.board[from.y][from.x] = 9;
			context.board[to.y][to.x]     = 9;
			return;
		}
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 9;
	}

	void wizardHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = context.board[to.y][to.x];
		context.board[to.y][to.x]     = 0;
	}

	void scoutHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x] = 11;
	}

	void ghostHandle( GameBoard& context, sf::Vector2i from, sf::Vector2i to )
	{
		if( context.board[to.y][to.x] < 0 )
		{
			context.board[from.y][from.x] = 0;
			context.board[to.y][to.x]     = -context.board[to.y][to.x];
			return;
		}
		context.board[from.y][from.x] = 0;
		context.board[to.y][to.x]     = 12;
	}

};

class GameTextures
{
public:
	sf::Texture silverTile,blackTile,boardTexture,figuresTexture;
	sf::Texture frameTexture, voidTexture, cancelTexture, resetTexture;

	sf::Texture wPawnTexture,wRookTexture,wBishopTexture;
	sf::Texture wKnightTexture,wKingTexture,wQueenTexture;
	sf::Texture wAssasinTexture,wArcherTexture,wNecroTexture;
	sf::Texture wWizardTexture,wScoutTexture,wGhostTexture;

	sf::Texture bPawnTexture,bRookTexture,bBishopTexture;
	sf::Texture bKnightTexture,bKingTexture,bQueenTexture;
	sf::Texture bAssasinTexture,bArcherTexture,bNecroTexture;
	sf::Texture bWizardTexture,bScoutTexture,bGhostTexture;


	GameTextures()
	{
		//figuresTexture.loadFromFile("/home/xenosblue/Pictures/figures.png");
    	//boardTexture.loadFromFile("/home/xenosblue/Pictures/board.png");
    	silverTile.loadFromFile("chess_pieces/silver.png");
    	blackTile.loadFromFile("chess_pieces/blackk.jpeg");
    	frameTexture.loadFromFile("chess_pieces/frame5.png");
    	voidTexture.loadFromFile("chess_pieces/void.jpeg");
    	cancelTexture.loadFromFile("chess_pieces/cancel.png");
    	resetTexture.loadFromFile("chess_pieces/reset.png");


    	wPawnTexture.loadFromFile("chess_pieces/wPawn.png");
    	wRookTexture.loadFromFile("chess_pieces/wRook.png");
    	wKnightTexture.loadFromFile("chess_pieces/wKnight.png");
    	wBishopTexture.loadFromFile("chess_pieces/wBishop.png");
    	wQueenTexture.loadFromFile("chess_pieces/wQueen.png");
    	wKingTexture.loadFromFile("chess_pieces/wKing.png");

    	wAssasinTexture.loadFromFile("chess_pieces/assasinw1.png");
    	wArcherTexture.loadFromFile("chess_pieces/archw5.png");
    	wNecroTexture.loadFromFile("chess_pieces/necrow1.png");
    	wWizardTexture.loadFromFile("chess_pieces/wizw1.png");
    	wScoutTexture.loadFromFile("chess_pieces/scout2w.png");
    	wGhostTexture.loadFromFile("chess_pieces/ghost2w.png");

    	bPawnTexture.loadFromFile("chess_pieces/bPawn.png");
    	bRookTexture.loadFromFile("chess_pieces/bRook.png");
    	bKnightTexture.loadFromFile("chess_pieces/bKnight.png");
    	bBishopTexture.loadFromFile("chess_pieces/bBishop.png");
    	bQueenTexture.loadFromFile("chess_pieces/bQueen.png");
    	bKingTexture.loadFromFile("chess_pieces/bKing.png");

    	bAssasinTexture.loadFromFile("chess_pieces/assasinb1.png");
    	bArcherTexture.loadFromFile("chess_pieces/archb5.png");
    	bNecroTexture.loadFromFile("chess_pieces/necrob1.png");
    	bWizardTexture.loadFromFile("chess_pieces/wizb1.png");
    	bScoutTexture.loadFromFile("chess_pieces/scout2b.png");
    	bGhostTexture.loadFromFile("chess_pieces/ghost2b.png");
	}
};

class GameSprites : public GameTextures
{
public:
	sf::Sprite boardSprite,silverTileSprite,blackTileSprite;
	sf::Sprite frameSprite, voidSprite;
	std::vector<sf::Sprite> pieces;
	//GameTextures gt;

	GameSprites()
	{
		//gt = GameTextures();
		boardSprite.setTexture(boardTexture);
		silverTileSprite.setTexture(silverTile);
		blackTileSprite.setTexture(blackTile);
		frameSprite.setTexture(frameTexture);
		voidSprite.setTexture(voidTexture);
	}

	void setSprites(const GameBoard& context)
	{
		pieces.clear();

		for(int i=0;i<context.rows;i++)
		{
			for(int j=0;j<context.cols;j++)
			{
				if( context.board[i][j] == 6 )
				{
					sf::Sprite temp;
					temp.setTexture(wPawnTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 1 )
				{
					sf::Sprite temp;
					temp.setTexture(wRookTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 2 )
				{
					sf::Sprite temp;
					temp.setTexture(wKnightTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 3 )
				{
					sf::Sprite temp;
					temp.setTexture(wBishopTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 4 )
				{
					sf::Sprite temp;
					temp.setTexture(wQueenTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 5 )
				{
					sf::Sprite temp;
					temp.setTexture(wKingTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 7 )
				{
					sf::Sprite temp;
					temp.setTexture(wAssasinTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 8 )
				{
					sf::Sprite temp;
					temp.setTexture(wArcherTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 9 )
				{
					sf::Sprite temp;
					temp.setTexture(wWizardTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 10 )
				{
					sf::Sprite temp;
					temp.setTexture(wNecroTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 11 )
				{
					sf::Sprite temp;
					temp.setTexture(wScoutTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == 12 )
				{
					sf::Sprite temp;
					temp.setTexture(wGhostTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}




				else if( context.board[i][j] == -6 )
				{
					sf::Sprite temp;
					temp.setTexture(bPawnTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -1 )
				{
					sf::Sprite temp;
					temp.setTexture(bRookTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -2 )
				{
					sf::Sprite temp;
					temp.setTexture(bKnightTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -3 )
				{
					sf::Sprite temp;
					temp.setTexture(bBishopTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -4 )
				{
					sf::Sprite temp;
					temp.setTexture(bQueenTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -5 )
				{
					sf::Sprite temp;
					temp.setTexture(bKingTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -7 )
				{
					sf::Sprite temp;
					temp.setTexture(bAssasinTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -8 )
				{
					sf::Sprite temp;
					temp.setTexture(bArcherTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -9 )
				{
					sf::Sprite temp;
					temp.setTexture(bWizardTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -10 )
				{
					sf::Sprite temp;
					temp.setTexture(bNecroTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -11 )
				{
					sf::Sprite temp;
					temp.setTexture(bScoutTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}
				else if( context.board[i][j] == -12 )
				{
					sf::Sprite temp;
					temp.setTexture(bGhostTexture);
					temp.setPosition(context.getX(j),context.getY(i));
					pieces.push_back(temp);
				}

				else{}
			}
		}	
	}

	void reloadSprites(const GameBoard& context)
	{

		
	}
};


class GameViews
{
public:
	sf::View mainScreen, logView, chatView;

	GameViews()
	{
		logView.setViewport(sf::FloatRect(0.55f,0.02f,0.7f,0.7f));
		logView.setSize(1200,1200);
		mainScreen.setViewport(sf::FloatRect(0,0,1.0f,1.0f));
		mainScreen.setSize(1000,1000);
		chatView.setViewport(sf::FloatRect(0.55f,0.5f,0.7f,0.7f));
		chatView.setSize(1200,1200);
	}
};


class Button
{
private:
    sf::Vector2f location;
    sf::Vector2f size;
    bool isClicked;
    bool isClicker;

public:
	sf::Text text;
    sf::RectangleShape rect;
	Button(std::string _text, sf::Vector2f _location, sf::Vector2f _size, bool _isClicker)
	{
	    location = _location;
	    size = _size;
	    isClicker = _isClicker;
	    isClicked = false;
	    rect = sf::RectangleShape(size);
	    rect.setPosition(location);
	    rect.setFillColor(sf::Color(255,0,127,255));
	    rect.setOutlineThickness(10);
	    rect.setOutlineColor(sf::Color(255,0,57,100));
	    sf::Font font;
		font.loadFromFile("OpenSans-Regular.ttf");
	    text = sf::Text(_text,font);
	    text.setFillColor(sf::Color::White);
	    sf::Vector2f temp(20,5);
	    text.setPosition(location+temp);
	}

	bool checkClick (sf::Vector2i mousePos)
	{
	    if( rect.getGlobalBounds().contains(mousePos.x,mousePos.y) )
	    {
	    	click();
	    	return true;
	    }
	    return false;
	}

	void click()
	{
	    isClicked = isClicker? !isClicked : 0;
	}
};


class GameConnection
{
private:
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::io_service m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
public:
    GameConnection(int mode, boost::asio::io_service& m_io_service):m_socket(m_io_service),m_acceptor(m_io_service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 9998+mode))
    {
    }

    void accept()
    {
        m_acceptor.accept(m_socket);
    	//std::string s = boost::lexical_cast<std::string>(m_socket.remote_endpoint());
    	//GameTest::test(s);
    }

    void connect(const std::string ip)
    {
        m_socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip),9999));
    }

    std::string getData()
    {
        boost::asio::streambuf buf;
        boost::asio::read_until(m_socket, buf, "\n");
        std::string data = boost::asio::buffer_cast<const char*>(buf.data());
        return data;
    }


    void sendData(const std::string& message)
    {
        boost::asio::write(m_socket,
              boost::asio::buffer(message + "\n"));
    }
	

	void stringToMove( const std::string message, GameBoard& context )
	{
		sf::Vector2i oldPos(0,0);
		sf::Vector2i newPos(0,0);
		int fromVal;
		int toVal;
		std::string  val="";

		int i=0;
		for( i=0; i<message.length(); i++)
		{
			if( message[i] == '|' ){ oldPos.x = std::stoi(val); val=""; continue; }
			if( message[i] == '=' ){ oldPos.y = std::stoi(val); val=""; continue; }
			if( message[i] == ';' ){ fromVal  = std::stoi(val); val=""; i++; break; }
			val += message[i];
		}

		//GameTest::test("string to move 777");

		for( ; i<message.length(); i++)
		{
			//GameTest::test("----->");
			//GameTest::test(val);
			if( message[i] == '|' ){ newPos.x = std::stoi(val); val=""; continue; }
			if( message[i] == '=' ){ newPos.y = std::stoi(val); val=""; continue; }
			if( message[i] == ';' ){ toVal    = std::stoi(val); val=""; break; }
			val += message[i];
		}

		//GameTest::test("string to move 999");

		context.board[ context.rows - oldPos.y - 1 ][ context.cols - oldPos.x - 1 ] = -fromVal;
		context.board[ context.rows - newPos.y - 1 ][ context.cols - newPos.x - 1 ] = -toVal;

		//GameTest::test("string to move 111");
	}

	std::string moveToString( sf::Vector2i from, sf::Vector2i to, int fromVal, int toVal )
	{
		std::string message = "";
		message += std::to_string(from.x) + "|" + std::to_string(from.y) + "=" + std::to_string(fromVal) + ";";
		message += std::to_string(to.x) + "|"  + std::to_string(to.y) + "=" + std::to_string(toVal) + ";";
		return message;
	}

	void stringToRow( const std::string message, GameBoard& context )
	{
		std::string  val="";
		std::vector<int> rowVec;
		int rno;

		for( int i=0; i<message.length(); i++)
		{
			if( message[i] == ':' ){ rno = std::stoi(val); val=""; continue; }
			if( message[i] == ',' ){ rowVec.push_back(std::stoi(val)); val=""; continue; }
			if( message[i] == ';' ){ rowVec.push_back(std::stoi(val)); val=""; break; }
			val += message[i];
		}

		for(int i=0; i<context.cols; i++)
		{
			context.board[ context.rows - rno - 1 ][ context.cols - i - 1 ] = -rowVec[i];
		}
	}

	std::string rowToString( const int rno, const GameBoard& context )
	{
		std::string message = std::to_string( rno ) + ':';
		for( int i=0; i<context.cols; i++ ){ message += std::to_string( context.board[rno][i] ) + ','; }
		message.pop_back();
		message += ";";
		return message;
	}

	void sendRowToString( const int rno, const GameBoard& context )
	{
		sendData( rowToString( rno, context ) );
	}

	void readStringToRow( GameBoard& context )
	{
		std::string message = getData();
		//std::cout<<message<<std::endl;
		stringToRow( message, context );
	}

	void sendMoveToString(  sf::Vector2i from, sf::Vector2i to, int fromVal, int toVal )
	{
		std::string message = moveToString( from, to, fromVal, toVal ); 
		sendData( message );
	}

	void readStringToMove( GameBoard& context )
	{
		std::string message = getData();
		//std::cout<<message<<std::endl;
		stringToMove( message, context );
		//std::cout<<"read string done"<<std::endl;
	}
};


std::string getSystemIP()
{
	std::string ip;
	system("dig +short myip.opendns.com @resolver1.opendns.com > iptext");
	std::ifstream infile("iptext");
	infile >> ip;
	std::string echoIP = "echo " + ip;
	system(echoIP.c_str());
	return ip;
}





bool startScreen( std::string& user_name )
{

	bool isServer;
	sf::RenderWindow startScreen( sf::VideoMode( 700, 500 ), "Adventure Chess" );


    sf::Font font;
	font.loadFromFile("OpenSans-Regular.ttf");
	sf::Text playerText("enter name",font);
    std::string playerInput;
    playerText.setPosition(300,100);
    playerText.setFillColor(sf::Color::White);

    Button playGameButton("Play Game", sf::Vector2f(400,300), sf::Vector2f(200,50), true);
    playGameButton.text.setFont(font);
    Button createGameButton("Create Game", sf::Vector2f(100,300), sf::Vector2f(200,50), true);
    createGameButton.text.setFont(font);

    while (startScreen.isOpen())
    {
    	sf::Vector2i mousePos = sf::Mouse::getPosition( startScreen );
        sf::Event event;
        while (startScreen.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                startScreen.close();
                exit(0);
            }

            else if (event.type == sf::Event::TextEntered)
            {
            	if(event.text.unicode == '\b')
	            {
	            	if( playerInput.size() > 0 )
	            	{
		            	playerText.move(4,0);
		            	playerInput.erase( playerInput.length() - 1, 1 );
		            	playerText.setString(playerInput);
	            	}
	            }

	            else
				{
					playerText.move(-4,0);
				    playerInput +=event.text.unicode;
				    playerText.setString(playerInput);
				}
            }

            else if( event.type == sf::Event::MouseButtonPressed )
			{
				if( createGameButton.checkClick(mousePos) )
				{
					isServer = true;
					startScreen.close();
				}
				else if( playGameButton.checkClick(mousePos) )
				{
					isServer = false;
					startScreen.close();
				}
			}


        }


        startScreen.clear();
        startScreen.draw(playGameButton.rect);
        startScreen.draw(playGameButton.text);
        startScreen.draw(createGameButton.rect);
        startScreen.draw(createGameButton.text);
        startScreen.draw(playerText);
        startScreen.display();
    }

    user_name = playerInput;
    return isServer;
}

std::string strategyScreen( bool isServer, std::vector<std::vector<int>>& strategy )
{
	sf::RenderWindow stratScreen( sf::VideoMode( 700, 500 ), "Adventure Chess Strategy" );
	//sf::RenderWindow stratScreen( sf::VideoMode( 1000, 1000 ), "Adventure Chess Strategy" );


	std::vector<std::vector<int>> territory( 3 , std::vector<int> (8, 0));
	GamePieces gp;
	GameSprites gs;
	GameBoard gb;
	gb.offset = sf::Vector2f(gb.tile_size,-gb.tile_size*(gb.rows-4));
	int points =  gp.capital;


    sf::Font font;
	font.loadFromFile("OpenSans-Regular.ttf");
	//std::cout<<(isServer?getSystemIP():"enter ip")<<std::endl;
	sf::Text playerText(isServer?getSystemIP():"enter ip",font);
    std::string playerInput;
    playerText.setPosition(0,0);
    playerText.setFillColor(sf::Color::White);


    sf::Text remainingPoints(std::to_string(points), font);
    remainingPoints.setPosition(660,5);
    remainingPoints.setFillColor(sf::Color::White);

    Button confirmGameButton("Confirm", sf::Vector2f(475,425), sf::Vector2f(200,50), true);
	confirmGameButton.text.setFont(font);


	bool ismove = false;
	int pieceId = -1;
	int selectPiece = 0;
	float dx=0,dy=0;
	sf::Vector2i newPos,oldPos;


	for(int i=0;i<gb.cols;i++){ gb.board[gb.rows-3][i] = 6; }
	gb.board[gb.rows-1][3] = 4;
	gb.board[gb.rows-1][4] = 5;

	std::vector<sf::Sprite> spriteButtons;
	
	sf::Sprite rookButton;    rookButton.setTexture(gs.wRookTexture);		rookButton.setPosition(gb.getX(1),gb.getY(16));
	sf::Sprite knightButton;  knightButton.setTexture(gs.wKnightTexture);   knightButton.setPosition(gb.getX(2),gb.getY(16));
	sf::Sprite bishopButton;  bishopButton.setTexture(gs.wBishopTexture);   bishopButton.setPosition(gb.getX(3),gb.getY(16));
	sf::Sprite cancelButton;  cancelButton.setTexture(gs.cancelTexture);     cancelButton.setPosition(gb.getX(4),gb.getY(16));
	sf::Sprite resetButton;   resetButton.setTexture(gs.resetTexture);     resetButton.setPosition(gb.getX(5),gb.getY(16));
	sf::Sprite pawnButton;    pawnButton.setTexture(gs.wPawnTexture);       pawnButton.setPosition(gb.getX(6),gb.getY(16));

	sf::Sprite assasinButton; assasinButton.setTexture(gs.wAssasinTexture); assasinButton.setPosition(gb.getX(1),gb.getY(17));
	sf::Sprite archerButton;  archerButton.setTexture(gs.wArcherTexture);   archerButton.setPosition(gb.getX(2),gb.getY(17));
	sf::Sprite necroButton;   necroButton.setTexture(gs.wNecroTexture);     necroButton.setPosition(gb.getX(3),gb.getY(17));
	sf::Sprite wizardButton;  wizardButton.setTexture(gs.wWizardTexture);   wizardButton.setPosition(gb.getX(4),gb.getY(17));
	sf::Sprite scoutButton;   scoutButton.setTexture(gs.wScoutTexture);     scoutButton.setPosition(gb.getX(5),gb.getY(17));
	sf::Sprite ghostButton;   ghostButton.setTexture(gs.wGhostTexture);     ghostButton.setPosition(gb.getX(6),gb.getY(17));

	spriteButtons.push_back(rookButton);spriteButtons.push_back(knightButton);spriteButtons.push_back(bishopButton);
	spriteButtons.push_back(cancelButton);spriteButtons.push_back(resetButton);spriteButtons.push_back(pawnButton);

	spriteButtons.push_back(assasinButton);spriteButtons.push_back(archerButton);spriteButtons.push_back(necroButton);
	spriteButtons.push_back(wizardButton);spriteButtons.push_back(scoutButton);spriteButtons.push_back(ghostButton);


	gs.setSprites(gb);
	gb.updateVisible();
	remainingPoints.setString(std::to_string(gp.revalPoints(gb)));


    while (stratScreen.isOpen())
    {
    	sf::Vector2i mousePos = sf::Mouse::getPosition( stratScreen );
        sf::Event event;
        while (stratScreen.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                stratScreen.close();
                exit(0);
            }

            else if (event.type == sf::Event::TextEntered && !isServer)
            {
            	if(event.text.unicode == '\b')
	            {
	            	if( playerInput.size() > 0 )
	            	{
		            	//playerText.move(4,0);
		            	playerInput.erase( playerInput.length() - 1, 1 );
		            	playerText.setString(playerInput);
	            	}
	            }

	            else
				{
					//playerText.move(-4,0);
				    playerInput +=event.text.unicode;
				    playerText.setString(playerInput);
				}
            }

            else if( event.type == sf::Event::MouseButtonPressed )
			{
				if( confirmGameButton.checkClick(mousePos) )
				{
					stratScreen.close();
				}

			}

			if(event.type == sf::Event::MouseButtonPressed )
			{
					for(int i=0;i<gs.pieces.size();i++)
					{
						if(gs.pieces[i].getGlobalBounds().contains(mousePos.x,mousePos.y))
						{
							sf::Vector2f p = gs.pieces[i].getPosition();
	                  		oldPos = sf::Vector2i( int( ( p.x - gb.offset.x ) / gb.tile_size ), int( ( p.y - gb.offset.y ) / gb.tile_size ) );
	                  		if(oldPos.y==gb.rows-3){break;}
	                  		gb.validMoves.clear();
	                  		for(int i=0;i<gb.cols;i++)
	                  		{
	                  			for(int j=gb.rows-3;j<gb.rows;j++)
	                  			{
	                  				if( gb.board[j][i] == 0 ){ gb.validMoves.push_back( sf::Vector2i( i , j ) ); }
	                  			}
	                  		}

	                  		ismove = true;
	                  		pieceId = i;
                    		dx = mousePos.x - gs.pieces[i].getPosition().x;
                    		dy = mousePos.y - gs.pieces[i].getPosition().y;
                    		break;
						}else{
							sf::Vector2f p(mousePos.x,mousePos.y);
	                  		oldPos = sf::Vector2i( int( ( p.x - gb.offset.x ) / gb.tile_size ), int( ( p.y - gb.offset.y ) / gb.tile_size ) );
	                  		

	                  		if(-1<oldPos.x && oldPos.x<gb.cols && gb.rows-3<oldPos.y && oldPos.y<gb.rows)
	                  		{
	                  			if( ((gb.board[ oldPos.y ][ oldPos.x ] == 0) && (points>=gp.costs[selectPiece])) || ((selectPiece == 0) && (gb.board[ oldPos.y ][ oldPos.x ] != 4) && (gb.board[ oldPos.y ][ oldPos.x ] != 5)))// && gb.board[ oldPos.y ][ oldPos.x ] != 4 )
	                  			{
	                  				gb.board[ oldPos.y ][ oldPos.x ] = selectPiece;
	                  				points = gp.revalPoints(gb);
	                  				remainingPoints.setString(std::to_string(points));
	                  				gs.setSprites(gb);
	                  			}
	                  		}else{

	                  			for( int i=0; i<spriteButtons.size(); i++ )
								{
									if( spriteButtons[i].getGlobalBounds().contains(mousePos.x,mousePos.y) )
									{
										if(i == 3){ selectPiece = 0; break;}
										else if( i == 4 )
										{
											for(int i=0;i<gb.cols;i++){for( int j=gb.rows-2;j<gb.rows;j++){gb.board[j][i]=0;}};
											gb.board[gb.rows-1][3] = 4;
											gb.board[gb.rows-1][4] = 5;
											points = gp.revalPoints(gb);
											remainingPoints.setString(std::to_string(points));
	                  						gs.setSprites(gb);
											break;
										}
										else
										{
											selectPiece = i+1;
											//std::cout<<selectPiece<<std::endl;
										}
									}
								}
	                  		}
						
							
						}
					}
			}


			if(event.type==sf::Event::MouseButtonReleased)
			{

	            sf::Vector2f p = gs.pieces[pieceId].getPosition();
	            newPos = sf::Vector2i( int( ( p.x - gb.offset.x ) / gb.tile_size + 0.5 ), int( ( p.y - gb.offset.y ) / gb.tile_size + 0.5 ) );
	            if( gb.checkValid(newPos) )
	            {
	            	gb.board[ newPos.y ][ newPos.x ] = gb.board[ oldPos.y ][ oldPos.x ];
	            	gb.board[ oldPos.y ][ oldPos.x ] = 0;
	            	gs.setSprites(gb);
	            }
	            else{ gs.setSprites(gb); }


				pieceId = -1;
				ismove=false;
				dx=0;
				dy=0;
          	}




        }

        if( ismove && (mousePos.x < gb.width + gb.offset.x) && (mousePos.y < gb.height + gb.offset.y) )
		{
			gs.pieces[ pieceId ].setPosition( mousePos.x - dx, mousePos.y - dy );
		}


        stratScreen.clear();
        stratScreen.draw(confirmGameButton.rect);
        stratScreen.draw(confirmGameButton.text);
        stratScreen.draw(remainingPoints);
        stratScreen.draw(playerText);

		
	    for(int i=0;i<gb.cols;i++)
	    {
	        for(int j=gb.rows-3;j<gb.rows;j++)
	        {
	            if((i+j)%2==0)
	            {
	            	gs.silverTileSprite.setPosition(gb.getX(i),gb.getY(j));
	            	stratScreen.draw(gs.silverTileSprite);
	            }
	            else
	            {
	            	gs.blackTileSprite.setPosition(gb.getX(i),gb.getY(j));
	            	stratScreen.draw(gs.blackTileSprite);
	           	}
	        }
	    }

    	for(int i=0;i<gs.pieces.size();i++) 
    	{
    		//gs.pieces[i].move(-offsetX,-offsetY);
    		stratScreen.draw(gs.pieces[i]);
    		//window.draw(chessSprites.pieces[pieceId]);
    	}

    	for( int i=0; i<spriteButtons.size(); i++ )
    	{
    		stratScreen.draw(spriteButtons[i]);
    	}
	    


        stratScreen.display();
    }

    for(int i=0;i<3;i++)
    {
    	for(int j=0;j<gb.cols;j++)
    	{
    		territory[i][j] = gb.board[gb.rows-3+i][j]; 
    	}
    }

    strategy = territory;
    return playerInput;
}

bool gameMainScreen( bool isServer, std::string user_name, std::string ip,  std::vector<std::vector<int>> strategy)
{
	sf::RenderWindow window( sf::VideoMode( 1000, 1000 ), "Adventure Chess" );
	GameBoard chessBoard = GameBoard();
	GameSprites chessSprites = GameSprites();
	GamePieces chessPieces = GamePieces();
	GameViews chessViews = GameViews();
	boost::asio::io_service m_io_service;
	GameConnection gcService(int(isServer), m_io_service);
	sf::Font font;
	font.loadFromFile("OpenSans-Regular.ttf");

/*
	sf::Text waitingText("waiting for connection...",font);
	waitingText.setPosition(450,500);
	waitingText.setFillColor(sf::Color::White);
	window.draw(waitingText);
	window.display();
*/

	srand(time(0));
	bool myTurn;
	std::string opp_name;

	//std::cout<<strategy.size()<<" "<<strategy[0].size()<<std::endl;
	for(int i=0;i<3;i++)
    {
    	for(int j=0;j<chessBoard.cols;j++)
    	{
    		//std::cout<<i<<" "<<j<<std::endl;
    		//std::cout<<chessBoard.rows-3+i<<" "<<j<<std::endl;
    		chessBoard.board[chessBoard.rows-3+i][j] = strategy[i][j]; 
    	}
    }


	//GameTest::testSet(chessBoard);

	if(isServer)
	{
		myTurn = bool(rand()%2);
		gcService.accept();
		gcService.sendData(user_name);
		opp_name = gcService.getData();
		gcService.sendData(std::to_string(int(myTurn)));
	}
	else
	{
		gcService.connect(ip);
		opp_name = gcService.getData();
		gcService.sendData(user_name);
		myTurn = !(bool(std::stoi(gcService.getData())));
	}

	//std::cout<<"myTurn: "<<myTurn<<std::endl;

	for(int i=0;i<3;i++)
	{
		if( myTurn )
		{
			gcService.sendRowToString( chessBoard.rows - i - 1, chessBoard );
			//sleep(1);
			gcService.readStringToRow( chessBoard );
			sleep(1);
		}
		else
		{
			gcService.readStringToRow( chessBoard );
			//sleep(1);
			gcService.sendRowToString( chessBoard.rows - i - 1, chessBoard );
			sleep(1);
		}
		
		myTurn = !myTurn;
	}

    //GameTest::display(chessBoard);

	sf::Text timerText("",font);
	timerText.setPosition(900,950);
	timerText.setFillColor(sf::Color::White);
	sf::Text uNameText(user_name,font);
	sf::Text vsText("VS",font);
	sf::Text oNameText(opp_name,font);
	uNameText.setPosition(800-(20*user_name.length()),0);
	vsText.setPosition(850,0);
	oNameText.setPosition(900,0);
	uNameText.setFillColor(sf::Color::White);
	vsText.setFillColor(sf::Color::White);
	oNameText.setFillColor(sf::Color::White);



	/////////////////////////////////

    chessSprites.setSprites(chessBoard);
    chessBoard.updateVisible();


    std::vector<sf::CircleShape> circles;
    srand(time(0));
    for(int i=0;i<91;i++)
    {
    	circles.push_back(sf::CircleShape((rand()%100)+10));
    	circles[i].setFillColor(sf::Color(255,rand()%256,rand()%256,rand()%256));
    	circles[i].setPosition(rand()%1000,rand()%1000);
    }
    	

    bool ismove = false;
	int pieceId = -1;
	float dx=0,dy=0;
	sf::Vector2i newPos,oldPos;


	//sf::RenderWindow window( sf::VideoMode( 1000, 1000 ), "Adventure Chess" );
	while(window.isOpen())
	{

		if(chessBoard.isGameOver()){ return chessBoard.didILose; }
		sf::Vector2i mousePos = sf::Mouse::getPosition( window );
		sf::Event e;
		
		while( window.pollEvent(e) )
		{
			if( e.type == sf::Event::Closed )
			{
				window.close();
			}

			if(e.type == sf::Event::MouseButtonPressed && mousePos.x < chessBoard.width + chessBoard.offset.x && mousePos.y < chessBoard.height + chessBoard.offset.y )
			{
				
					for(int i=0;i<chessSprites.pieces.size();i++)
					{
						if(chessSprites.pieces[i].getGlobalBounds().contains(mousePos.x,mousePos.y))
						{
							sf::Vector2f p = chessSprites.pieces[i].getPosition();
	                  		oldPos = sf::Vector2i( int( ( p.x - chessBoard.offset.x ) / chessBoard.tile_size ), int( ( p.y - chessBoard.offset.y ) / chessBoard.tile_size ) );

	                  		sf::Vector2i switcher = sf::Vector2i( oldPos.y, oldPos.x );
	                  		chessPieces.Validate( switcher, chessBoard );

	                  		ismove = true;
	                  		pieceId = i;
                    		dx = mousePos.x - chessSprites.pieces[i].getPosition().x;
                    		dy = mousePos.y - chessSprites.pieces[i].getPosition().y;
						}
					}
			}


			if(e.type==sf::Event::MouseButtonReleased)
			{
	                

	                if(chessSprites.pieces[ pieceId ].getPosition().x < chessBoard.width + chessBoard.offset.x && chessSprites.pieces[pieceId].getPosition().y < chessBoard.height + chessBoard.offset.y )
	                {
	                  sf::Vector2f p = chessSprites.pieces[pieceId].getPosition();
	                  newPos = sf::Vector2i( int( ( p.x - chessBoard.offset.x ) / chessBoard.tile_size + 0.5 ), int( ( p.y - chessBoard.offset.y ) / chessBoard.tile_size + 0.5 ) );
	                  //sf::Vector2i switcher = sf::Vector2i( newPos.y, newPos.x );
	                  if( chessBoard.checkValid(newPos) && myTurn )
	                  {
	                  	//chessBoard.board[ newPos.y ][ newPos.x ] = chessBoard.board[ oldPos.y ][ oldPos.x ];
	                  	//chessBoard.board[ oldPos.y ][ oldPos.x ] = 0;
	                  	chessPieces.moveHandle( chessBoard, oldPos, newPos );
	                  	chessSprites.setSprites(chessBoard);
	                  	chessBoard.updateVisible();
	                  	gcService.sendMoveToString(oldPos,newPos,chessBoard.board[oldPos.y][oldPos.x],chessBoard.board[newPos.y][newPos.x]);
	                  	myTurn = !myTurn;
	                  }
	                  else{ chessSprites.setSprites(chessBoard); }

	                }
	                

		        	pieceId = -1;
		        	ismove=false;
		            dx=0;
		            dy=0;
		            chessBoard.validMoves.clear();
          	}

		};

		if( ismove && (mousePos.x < chessBoard.width + chessBoard.offset.x) && (mousePos.y < chessBoard.height + chessBoard.offset.y) )
		{
			chessSprites.pieces[ pieceId ].setPosition( mousePos.x - dx, mousePos.y - dy );
		}






		window.clear();

		for(int i=0;i<circles.size();i++){window.draw(circles[i]);}
		window.setView(chessViews.mainScreen);

		window.draw(uNameText);
		window.draw(vsText);
		window.draw(oNameText);
		window.draw(timerText);
		
	    for(int i=0;i<chessBoard.cols;i++)
	    {
	        for(int j=0;j<chessBoard.rows;j++)
	        {
	            if((i+j)%2==0)
	            {
	            	chessSprites.silverTileSprite.setPosition(chessBoard.getX(i),chessBoard.getY(j));
	            	window.draw(chessSprites.silverTileSprite);
	            }
	            else
	            {
	            	chessSprites.blackTileSprite.setPosition(chessBoard.getX(i),chessBoard.getY(j));
	            	window.draw(chessSprites.blackTileSprite);
	           	}
	        }
	    }


	    for(int i=0;i<chessBoard.validMoves.size();i++) 
    	{
    		chessSprites.frameSprite.setPosition(chessBoard.getX(chessBoard.validMoves[i].x),chessBoard.getY(chessBoard.validMoves[i].y));
    		window.draw(chessSprites.frameSprite);
    	}


	    ////DRAW PIECES///// 
    	for(int i=0;i<chessSprites.pieces.size();i++) 
    	{
    		window.draw(chessSprites.pieces[i]);
    		//window.draw(chessSprites.pieces[pieceId]);
    	}

    	for(int i=0;i<chessBoard.cols;i++) 
    	{
    		for(int j=0;j<chessBoard.rows;j++) 
    		{
    			if(!chessBoard.visible[j][i])
    			{
    				chessSprites.voidSprite.setPosition(chessBoard.getX(i),chessBoard.getY(j));
    				window.draw(chessSprites.voidSprite);
    			}
    		}
    	}
	    
	    window.display();

	    if(!myTurn)
	    {
	    	//std::cout<<"here"<<std::endl;
	    	sleep(1);
	    	gcService.readStringToMove( chessBoard ); 
	    	myTurn = !myTurn;
	    	//std::cout<<"done"<<std::endl; 
	    	chessSprites.setSprites(chessBoard);
	    }
	}

	return true;
}


int main()
{
	std::string user_name="Player1";
	std::vector<std::vector<int>> strategy;

	bool isServer = startScreen( user_name );
	std::string ip = strategyScreen( isServer, strategy );
ip = "127.0.0.1";
	bool win = gameMainScreen( isServer, user_name, ip, strategy );

	if(win){std::cout<<"You Won :)"<<std::endl;}
	else   {std::cout<<"You Lost:("<<std::endl;}


	return 0;
}
//dig +short myip.opendns.com @resolver1.opendns.com > iptext