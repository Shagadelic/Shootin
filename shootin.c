#include "shootin.h"

// Allow nonblocking terminal polling
int kbhit() {
        struct timeval tv;
	fd_set fds;
        tv.tv_sec = 0;
        tv.tv_usec = 0; // disable timeout for structure (sec and usec to 0)
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds); // file descriptor or stdin
        select(STDOUT_FILENO, &fds, NULL, NULL, &tv); // stdout monitors stdin with no timeout
        return FD_ISSET(STDIN_FILENO, &fds); // status check
}

void nonblock(int state) {
        struct termios ttystate;
        // get terminal state
        tcgetattr(STDIN_FILENO, &ttystate);

        if (state == NB_ENABLE) {
	        // turn off canonical mode and echo
		ttystate.c_lflag &= ~(ICANON | ECHO);
	        // after a single character is read, read() returns
	        ttystate.c_cc[VMIN] = 1;
	} else if (state == NB_DISABLE) {
       	        // turn on canonical mode once done to reset terminal back to original I/O state              
		ttystate.c_lflag |= (ICANON | ECHO);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

// Pass by value as character coordinates mustn't be modified
void *smallProjectile(void *cPVals) {
	int x = ((projectileVals *) cPVals)->x, 
		y = ((projectileVals *) cPVals)->y;
	short dir = ((projectileVals *) cPVals)->dir;

	int limit;
	short collision = 0;

	switch (dir) {
		case NORTH: 
			limit = FIELD_Y;
			break;
		case EAST: 
			limit = FIELD_X;
			break;
		case SOUTH: 
			limit = 0;
			break;
		case WEST: 
			limit = 0;
			break;
		default: 
			printw("Invalid direction (smallProjectile())");	
			return NULL;
			
	}

	if (dir & 1) {	//WEST or EAST
		while ((x != limit) && !collision) { 
			// T: WEST, F: EAST 
			x = (dir & 2) ? x - 1 : x + 1; 
			mvprintw(y, x, ".");
			usleep(PROJECTILE_SPEED);
		}
	} else {
		while ((y != limit) && !collision) { 
			// T: SOUTH F: NORTH
			y = (dir & 2) ? y - 1 : y + 1; 
			mvprintw(y, x, ".");
			usleep(PROJECTILE_SPEED);
		}
	}
}

int collisionCheck(int x, int y, short dir) {
	return 1;
}

void actionPoll(int startX, int startY, short startDir) {
	// internals
	int threadCount = 0;	
	int curPress;
	projectileVals *curPVals;

	// external values
	int mainCharX = startX, mainCharY = startY;
	short mainCharDir = startDir;

	while (!GAME_OVER) {
		threadCount = (threadCount = MAX_PROJECTILE) ? 0 : threadCount;
		if (kbhit()) {
			curPress = getch();
			switch(curPress){
				case KEY_UP: 
					if (collisionCheck(mainCharX, mainCharY - 1, NORTH))
						mainCharY++;
						dir = NORTH;
					break;
				case KEY_DOWN:
					if (collisionCheck(mainCharX, mainCharY + 1, NORTH))
						mainCharY++;
						dir = SOUTH
					break;
				case KEY_LEFT:
					if (collisionCheck(mainCharX - 1, mainCharY, NORTH))
						mainCharX--;	
						dir = WEST;
					break;
				case KEY_RIGHT:
					if (collisionCheck(mainCharX - 1, mainCharY, NORTH))
						mainCharX++;
						dir = EAST;
					break;
				case 'z':
					curPVals = (projectileVals *)
						malloc(sizeof(projectileVals));
					curPVals->x = mainCharX;
					curPVals->y = mainCharY;
					curPVals->dir = mainCharDir;

					pthread_create(projectile[threadCount++],
						NULL, smallProjectile,
						(void *) curPVals);
					break;
				case 'x':
					break;
				default:
					break;
			}
			mvprintw(mainCharY, mainCharX, "M");
			switch(mainCharDir) {
				case NORTH: 	
					mvprintw(mainCharY + 1, mainCharX, "^");
					break;	
				case EAST: 	
					mvprintw(mainCharY, mainCharX + 1, ">");
					break;
				case SOUTH: 	
					mvprintw(mainCharY - 1, mainCharX, "_");
					break;
				case WEST:	
					mvprintw(mainCharY, mainCharX - 1, "<");
					break;
				default: 
					printw("Invalid direction (smallProjectile())");	
					return;

			}
			
			refresh();
		}
	}
}

void drawPlayField(void *state) {
	int i = 0, j = 0;

	if (state == NULL) {
		for ( ; i < FIELD_X; i++) {
			mvprintw(FIELD_Y - 1, i, "-");
		}
		for ( ; j < FIELD_Y; j++) {
			mvprintw(j, i, "-");
		}
		GAME_OVER = 0;
	} else {
		// TODO: this will be changed later
		drawPlayField(NULL);
	}
}

int main() {
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	noecho();
	nonblock(NB_ENABLE);
	while (1) {
	drawPlayField(NULL);
	refresh();
	}
	
	/* actionProc = fork();
	if (actionProc == 0) {
		actionPoll(rand() % 50, rand() % 50, (short) rand() % 4);
		exit(0);
	} else {
		while (1);
	}

	nonblock(NB_DISABLE);	
	endwin(); */
	return 0;
}


