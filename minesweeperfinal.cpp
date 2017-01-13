#include <iostream
#include<ctime>
#include <stdlib.h>
#include <string>
#include <windows.h>
using namespace std;
int beginmine=0;
typedef unsigned char casa;

enum fieldValues : casa{ OPEN, CLOSED = 10, MINE, UNKNOWN, FLAG, RA };

class fieldData
{
public:
	fieldData() : value( CLOSED ), open( false ) {}
	casa value;
	bool open, mine, flag;
};

class game
{
	int dx, dy, mMines,nrMines;
	fieldData* field; bool go;
	int ump[6];
public:
	game()
	{ if( field ) delete [] field; }

	game( int x, int y )
        {go = false; dx = x; dy = y;
		field = new fieldData[x * y];
		memset( field, 0, x * y * sizeof( fieldData ) );

		nrMines = (  x * y )-8*(  x * y ) / 10;

		mMines = 0;
		srand (time(NULL));
		int minx, miny, m = 0;
		for( ; m < nrMines; m++ )
		{
			do
			{ minx = rand() % dx; miny = rand() % dy; }
			while( field[minx + dx * miny].mine );
			field[minx + dx * miny].mine = true;
		}
		ump[0] = ' ';
		ump[1] = '.';
		ump[2] = '*';
		ump[3] = '?';
		ump[4] = 'F';
		ump[5] = 'X';
	}

	void introduction()
	{
		string c, a;
		int col, row;
		while( !go )
		{
			drawBoard();
			cout << "Introduceti litera corespunzatoare coloanei, numarul corespunzator randului si o actiune:\nActiuni: o -> deschide, f -> steag, ? ->necunoscut,d->descopera\n";
			cin >> c >> row >> a;
			if( c[0] > 'Z' ) c[0] -= 32; if( a[0] > 'Z' ) a[0] -= 32;
			col = c[0] - 65;
			makeMove( col, row-1, a );
		}
	}

private:
	void makeMove( int x, int y, string a )
	{
		fieldData* fd = &field[dx* y + x];
		if( fd->open && fd->value < CLOSED &&(a[0]=='o'||a[0]=='F'||a[0]=='?'))
		{
			cout << "Celula e deschisa";
			Sleep( 4000 ); return;
		}
		if( a[0] == 'O' ) openCell( x, y );
		else if( a[0] == 'F' )
		{
			fd->open = true;
			fd->value = FLAG;
			mMines++;
			checkWin();
		}
		else
            if(a[0]=='?')
		{
			fd->open = true;
			fd->value = UNKNOWN;
		}
        if(a[0]=='D')

   {if(fd->open && fd->value < CLOSED&&getMineCount(x,y)==numberflag(x,y))
        openspecial(x,y);
	}}


	bool openCell( int x, int y )
	{
		if( !isInside( x, y ) ) return false;
		if( field[x + y * dx].mine ) explozie();
		else
		{
			if( field[x + y * dx].value ==UNKNOWN )
			{
				field[x + y * dx].value = CLOSED;
				field[x + y * dx].open = false;
				mMines--;
			}
			recOpen( x, y );
			checkWin();
		}

       return true;
	}
	int numberflag(int x,int y )
	{int nrf = 0;
		for( int b = -1; b< 2; b++ )
		for( int a = -1; a < 2; a++ )
		{
			if( a == 0 && b == 0 ) continue;
			if( field[x + a + ( y + b ) * dx].value==FLAG) nrf++;
		}

		return nrf;

	}
	bool openspecial(int x,int y)
	{int expl=0;fieldData* fd;
	 for( int b = -1; b< 2; b++ )
		for( int a = -1; a < 2; a++ )
		{
        if( a == 0 && b == 0 ) continue;
        else

		if( isInside( x+a, y+b ) )
        {fd = &field[x+a + (y+b) * dx];


		if( field[x+a + (y+b) * dx].mine )
        {expl++;
        if( fd->value == FLAG )
            {
            fd->open = true;
            fd->value = fd->mine ? MINE : RA;
            }
            else if( fd->mine )
            {fd->open = true;
             fd->value = MINE;}
        }
		else
		{
			if( field[x+a + (y+b) * dx].value ==UNKNOWN )
			{
				field[x+a + (y+b) * dx].value = CLOSED;
				field[x+a + (y+b)* dx].open = false;
				mMines--;
			}
			recOpen( x+a, y+b );
			checkWin();
		}
		if (expl>0)explozie();


	}}
    return true;
		}




	void drawBoard()
	{
        system( "cls" );
		cout << "Marked mines: " << mMines << " from " << nrMines << "\n\n";
		for( int x = 0; x < dx; x++ )
		cout << "  " << ( char )( 65 + x ) << " ";
		cout << "\n"; int yy;
		for( int y = 0; y < dy; y++ )
		{
			yy = y * dx;
			for( int x = 0; x < dx; x++ )
			cout << "+---";

			cout << "+\n"; fieldData* fd;
			for( int x = 0; x < dx; x++ )
			{
				fd = &field[x + yy]; cout<< "| ";
				if( !fd->open ) cout << ( char )ump[1] << " ";
				else
				{
					if( fd->value > 9 )
					cout << ( char )ump[fd->value - 9] << " ";
					else
					{
						if( fd->value < 1 ) cout << "  ";
						else cout << ( char )(fd->value + 48 ) << " ";
					}
				}
			}
			cout << "| " << y + 1 << "\n";
		}
		for( int x = 0; x < dx; x++ )
		cout << "+---";

		cout << "+\n\n";
	}

	void checkWin()
	{
		int z = dx * dy - nrMines, yy;
		fieldData* fd;
		for( int y = 0; y < dy; y++ )
		{
			yy = dx * y;
			for( int x = 0; x < dx; x++ )
			{
				fd = &field[x + yy];
				if( fd->open && fd->value != FLAG ) z--;
			}
		}
		if( !z ) lastMsg( "You won the game!");
	}

	void explozie()
	{
		int yy; fieldData* fd;
		for( int y = 0; y < dy; y++ )
		{
			yy = dx* y;
			for( int x = 0; x < dx; x++ )
			{
				fd = &field[x + yy];
				if( fd->value == FLAG )
				{
					fd->open = true;
					fd->value = fd->mine ? MINE : RA;
				}
				else if( fd->mine )
				{
					fd->open = true;
					fd->value = MINE;
				}
			}
		}
		lastMsg( "YOU LOSE!" );
	}

	void lastMsg( string s )
	{
		go = true; drawBoard();
		cout << s << "\n\n";
	}

	bool isInside( int x, int y ) { return ( x > -1 && y > -1 && x < dx && y <dy ); }

	void recOpen( int x, int y )
	{
		if( !isInside( x, y ) || field[x + y * dx].open ) return;
		int bc =getMineCount( x, y );
		field[x + y * dx].open = true;
		field[x + y * dx].value = bc;
		if( bc ) return;

		for( int j = -1; j < 2; j++ )
		for( int i = -1; i < 2; i++ )
		{
			if( i == 0 && j == 0 ) continue;
			recOpen( x + i, y + j );
		}
	}

	int getMineCount( int x, int y )
	{
		int m = 0;
		for( int b = -1; b< 2; b++ )
		for( int a = -1; a < 2; a++ )
		{
			if( a == 0 && b == 0 ) continue;
			if( isInside( x + a, y + b ) && field[x + a + ( y + b ) * dx].mine ) m++;
		}

		return m;
	}

};

int main()
 {system("color 72");
   int a,b,n;
    cout<<"Alege nivel de dificultate:"<<endl;
	cout<<"1-usor"<<endl;
	cout<<"2-mediu"<<endl;
	cout<<"3-dificil"<<endl;
    do
    {cin>>n;}while(n==0||n>3);

	if(n==1)
    {a=6;b=6;
    }
    else
    if(n==2)
    {a=9;b=9;
}
    else
    {
    a=12;b=12;
    }
srand (time(NULL));
    game g( a,b);
	g.introduction();
	int h;
    cout<<"Doriti sa mai jucati"<<endl<<"1-DA"<<endl<<"2-NU"<<endl;
	cin>>h;
	while(h==1)
    {game g( a,b );
    srand (time(NULL));
	g.introduction();
	cout<<"Doriti sa mai jucati"<<endl<<"1-DA"<<endl<<"2-NU"<<endl;
	cin>>h;
    }
    return 0;
}
