#include <OpenLayer.hpp>
#include <loadpng.h>
#include <string>
#include <vector>
#include <math.h>

using namespace std;
using namespace ol;

float abs(float a) {
	if (a<0) return -a;
	return a;
}

#define COUNT (40000)

class Point {
	public:
		Point() {
			x = 0;
			y = 0;
			dx = 0;
			dy = 0;
			len = 0;
		}
		void fix() {
			len = max(abs(dx),abs(dy));
			if (len==0) return;
			if (len<1) return;
			dx/=len;
			dy/=len;
			len/=len;
		}
	public:
		float x,y, dx,dy, len;
};
float sq(float a) {
	return a*a;
}
float dist(float x1, float y1, float x2, float y2) {
	return sqrt(sq(x2-x1) + sq(y2-y1));
}


float random( float min, float max ) {
   return min + float(rand())/float(RAND_MAX) * (max-min);
}



class WindMap {
	public:
		WindMap(int _width, int _height) {
			width = _width;
			height = _height;
			gridx = width/4;
			gridy = height/4;
			printf("gridx %i,%i\n", gridx,gridy);
			points = new Point[gridx*gridy];
			for (int y=0; y<gridy; y++)
				for (int x=0; x<gridx; x++) {
					points[x+y*gridx].x = x*4;
					points[x+y*gridx].y = y*4;
				}
		}
		~WindMap() {
			delete points;
			points = NULL;
		}
		Point &getPoint(float xf, float yf) {
			int x = (int)(xf/4);
			int y = (int)(yf/4);
			if (x<0) x = 0;
			if (y<0) y = 0;
			if (x>=gridx) x=gridx-1;
			if (y>=gridy) y=gridy-1;
			return getPoint(x,y);
		}
		Point &getPoint(int x, int y) {
			return points[x+y*gridx];
		}
		void stroke(int mouseX, int mouseY, int pmouseX, int pmouseY) {
			for (int y=0; y<gridy; y++)
				for (int x=0; x<gridx; x++) {
					Point &p = getPoint(x,y);
					float d = dist(p.x,p.y,mouseX,mouseY)*128*12/width;
					if (d<64 && dist(mouseX,mouseY,pmouseX,pmouseY)>0 && d>0) {
						p.dx += ((mouseX-pmouseX)-p.dx)/d;
						p.dy += ((mouseY-pmouseY)-p.dy)/d;
						p.fix();
					}
			}

		}
		void draw() {
			for (int y=0; y<gridy; y++)
				for (int x=0; x<gridx; x++) {
					Point &p = getPoint(x,y);
					GfxRend::CircleOutline(p.x, p.y, 2.0f, Rgba::GREEN);
					GfxRend::Line(p.x,p.y, p.x+p.dx*10, p.y+p.dy*10, Rgba::BLUE);
				}
		}
	public:
		int width,height;
	private:
		int gridx,gridy;

		Point *points;
};






int main() {
	// Choose a random seed from the computer clock
	srand( time( 0 ));

	// Initialize the program //
	// Initialize both mouse and keyboard //
	if( Setup::SetupProgram( true, true ) == false ) {
	allegro_message( "Error during the program startup!" );
	exit( -1 );
	}

	// Open the game in a 800 x 600 window //
	if( Setup::SetupScreen( 640, 480, WINDOWED ) == false ) {
	allegro_message( "Error while trying to set graphics mode!" );
	//exit( -1 );
	}

	// Load the font with size 17 x 14 and a white color //
	TextRenderer normalTextRenderer( "Fonts/Neuropol.ttf", 17, 12, Rgba::WHITE );

	// Create a bigger sized (40 x 30) copy of the font //
	TextRenderer biggerTextRenderer( normalTextRenderer, 35, 25 );

	// Is the game still running? //
	bool gameRunning = true;

	// The fps we're targetting to run the game at //
	// (The actual fps will be as high as possible with this //
	//  hardware, but the game will run just like if the fps was 100) //
	const float defaultFps = 100.0;

	// FpsCounter to calculates fps and delta time //
	// We should start the counter right before the game loop starts //
	FpsCounter::Start( defaultFps );

	int pmouseX = mouse_x, pmouseY = mouse_y;

	WindMap map(640, 480);


	Point particles[COUNT];
	for (int i=0; i<COUNT; i++) {
		particles[i].x = random(0,map.width);
		particles[i].y = random(0,map.height);
	}


	while( gameRunning ) {
		// Tell the fps counter that a new frame has started //
		FpsCounter::NewFrameStarted();

		if (key[KEY_ESC])
			gameRunning = false;

		// Get delta time since last frame
		float deltaTime = FpsCounter::GetDeltaTime();

		if (mouse_b && (mouse_x!=pmouseX || mouse_y!=pmouseY))
			map.stroke(mouse_x,mouse_y,pmouseX,pmouseY);
		pmouseX = mouse_x;
		pmouseY = mouse_y;




		GfxRend::FillScreen( Rgba(200,200,200) );


		for (int i=0; i<COUNT; i++) {
			Point &p = particles[i];
			Point &c = map.getPoint(p.x,p.y);
			p.dx+=(c.dx-p.dx)/20 * deltaTime;
			p.dy+=(c.dy-p.dy)/20 * deltaTime;

			c.dx+=(p.dx-c.dx)/200 * deltaTime;
			c.dy+=(p.dy-c.dy)/200 * deltaTime;

			p.x+=p.dx * 2 * deltaTime;
			p.y+=p.dy * 2 *deltaTime;


			if (p.x<0) { p.x+=map.width; }
			if (p.y<0) { p.y+=map.height; }
			if (p.x>=map.width) { p.x-=map.width; }
			if (p.y>=map.height) { p.y-=map.height; }

			//GfxRend::CircleGradient(p.x, p.y, 5.0f, Rgba::BLACK, Rgba::INVISIBLE);
			GfxRend::Point(p.x,p.y,Rgba::BLACK);
		}
		GfxRend::CircleOutline(mouse_x, mouse_y, 32.0f, Rgba::RED);

		//map.draw();

		string text = "Deltatime=" + ToString( deltaTime )
					+ "\nFPS: " + ToString( int( FpsCounter::GetFps() ));


		// Make sure that the screen isn't rotated, stretched or moved when the texts are rendered //
		Transforms::ResetTransforms();

		// We don't want the color channel modifications to affect text rendering //
		// So we revert all channels back to 1.0 //
		Transforms::SetColorChannels( Rgba::WHITE );

		// Print the shadow of the text to the screen at x = 4, baselineY = 16
		Rgba col = normalTextRenderer.GetColor();
		normalTextRenderer.SetColor( Rgba::BLACK );
		normalTextRenderer.Print( text, 5-1, 15+1 );

		normalTextRenderer.SetColor( col );

		// Print the text to the screen at x = 5, baselineY = 15
		normalTextRenderer.Print( text, 5, 15 );


		// Show the current frame in the screen //
		GfxRend::RefreshScreen();

	}


	return 0;
}
END_OF_MAIN()

