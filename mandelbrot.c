#include <stdio.h>
#include <ncurses.h>
#include <math.h>

void mandel(float viewport_xMin, float viewport_xMax, float viewport_yMin, float viewport_yMax, int terminal_xMax, int terminal_yMax, int n)
{

        float xStep = (viewport_xMax-viewport_xMin)/terminal_xMax; //our scaled coordinate system based on the real and imaginary
        float yStep = (viewport_yMax-viewport_yMin)/terminal_yMax;   //bounds of the mandelbrot set and our terminal window size

        int i = 0;
        float Cx = 0;
        float Cy = 0;

        float Zx = 0;
        float Zy = 0;
        float Zx2 = 0;
        float Zy2 = 0;

        char escape[7] = {' ','.','-','+','*','x','%'};
        int escape_time;

        for(int iY=0;iY<terminal_yMax;iY++)
        {
                Cy = viewport_yMin + (iY*yStep);
                if (fabs(Cy)<(yStep/2)) Cy=0.0;
                for(int iX=0;iX<terminal_xMax;iX++)
                {
                        Cx = viewport_xMin + (iX*xStep);
                        Zx=0.0;
                        Zy=0.0;
                        Zx2 = Zx*Zx;
                        Zy2 = Zy*Zy;
                        for(i=0;i<n && (Zx2+Zy2)<4;++i)
                                {
                                        Zy=(2*Zx*Zy)+Cy;
                                        Zx=(Zx2-Zy2)+Cx;
                                        Zx2=Zx*Zx;
                                        Zy2=Zy*Zy;
                                };
                        if(i==n)
                                {
                                        mvaddch(iY,iX,'#');
                                }
                        else if(i<n)
                                {
                                        escape_time=floor(7*i/n);
                                        if(escape_time<7) mvaddch(iY,iX,escape[escape_time]);
                                }
                }
        }

}


int main()
{
	//BEGIN INITIALIZATION

        initscr();
        
	noecho(); //don't echo what we type to the console
	
	cbreak(); //grab individual chars
        
	keypad(stdscr,TRUE);//accept keypad input
	
	curs_set(0); //we don't want to display the actual cursor because it's gonna move all over the place
	
	//define our vars to hold the dimensions of our terminal in characters
        int terminal_xMax, terminal_yMax;
	//grab them using ncurses. ncurses uses row,col notation so x and y are swapped.
        getmaxyx(stdscr,terminal_yMax,terminal_xMax);

	//here we initialize the boundaries of our viewport on the complex plane.
	//in the mandel() function, these will be transformed from our terminal coords.
	float viewport_xMin = -2.5;
	float viewport_xMax = 1;
	float viewport_yMin = -1;
	float viewport_yMax = 1;
	
	int n = 100; //initialize the number of iterations we want to use
	float scale = 1.0; //we initialize this to keep track of how zoomed in we are

        refresh(); //perform an initial screen refresh before we go to the main loop

	int ch; //finally, we declare this var to hold the keyboard input char.

	mandel(viewport_xMin, viewport_xMax, viewport_yMin, viewport_yMax, terminal_xMax, terminal_yMax, n);
	//INITIALIZATION DONE
	
	//BEGIN MAIN PROGRAM LOOP
	while(1)
	{ 

	ch = getch();

	switch(ch)
	{	case KEY_LEFT:
			viewport_xMin -= 0.1*scale;
			viewport_xMax -= 0.1*scale;
			break;
		case KEY_RIGHT:
			viewport_xMin += 0.1*scale;
			viewport_xMax += 0.1*scale;
			break;
		case KEY_UP:
			viewport_yMin -= 0.1*scale;
			viewport_yMax -= 0.1*scale;
        		break;
		case KEY_DOWN:
			viewport_yMin += 0.1*scale;
			viewport_yMax += 0.1*scale;
			break;
		case 'z':
			n = floor(n*1.1);
			viewport_xMin+=(viewport_xMax-viewport_xMin)*0.1;
			viewport_xMax-=(viewport_xMax-viewport_xMin)*0.1;
			viewport_yMin+=(viewport_yMax-viewport_yMin)*0.1;
			viewport_yMax-=(viewport_yMax-viewport_yMin)*0.1;
			scale=scale*0.8;
			break;
                case 'x':
			n = ceil(n*0.91);
                        viewport_xMin-=(viewport_xMax-viewport_xMin)*0.1;
                        viewport_xMax+=(viewport_xMax-viewport_xMin)*0.1;
                        viewport_yMin-=(viewport_yMax-viewport_yMin)*0.1;
                        viewport_yMax+=(viewport_yMax-viewport_yMin)*0.1;
			scale=scale*1.1;
                        break;
	}
	mandel(viewport_xMin, viewport_xMax, viewport_yMin, viewport_yMax, terminal_xMax, terminal_yMax, n);
	}
        return 0;
}
