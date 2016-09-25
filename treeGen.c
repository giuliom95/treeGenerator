#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "sdl.h"

#define PI 3.14159265

#define BRANCHES 30

#define LEAVES_DELTA_MULTIPLIER 0.01
#define LEAVES_ANGLE_STEP 512
#define LEAVES_RETURN_SECTION_WIDTH 16
#define LEAVES_MIN_RADIUS 0.07
#define LEAVES_MAX_RADIUS LEAVES_MIN_RADIUS*2


void save_to_ppm( uint8_t* data, char* file_name, uint8_t insert_time ) {
	
	uint16_t x, y;
	FILE* out;
	char full_file_name[40];
	
	if( insert_time )
		sprintf( full_file_name, "%s-%i.pgm", file_name, time(NULL));
	else
		sprintf( full_file_name, "%s.pgm", file_name);
		
	out = fopen(full_file_name, "w");
	
	fprintf(out, "P2\n%i %i\n255\n", SCREEN_WIDTH, SCREEN_HEIGHT);

	for(y = 0; y < SCREEN_HEIGHT; y++) {
		for(x = 0; x < SCREEN_WIDTH; x++) {
			fprintf(out, "%i ", data[x+(y*SCREEN_WIDTH)]);
		}
		fprintf(out, "\n");
	} 

	fclose(out);
}


void draw_leaves( double startX, double startY ) {
	
	
	
	if( startX == 0 && startY == 0 )
		return;
	
	uint8_t* pixel_data = (uint8_t*)calloc(SCREEN_WIDTH*SCREEN_HEIGHT, sizeof(uint8_t));
	
	double angle, start_distance, distance, delta, origin_distance;	
	
	angle = 0;
	origin_distance = sqrt(sqrt(startX*startX+startY*startY));
	start_distance = (((rand()*LEAVES_MIN_RADIUS)/RAND_MAX)+LEAVES_MIN_RADIUS)/origin_distance;
	distance = start_distance;

	glBegin(GL_LINES);
		glVertex2f(startX, startY);
		glVertex2f(startX+distance*cos(angle), startY+distance*sin(angle));
	glEnd();
	
	while (angle < ((2*LEAVES_RETURN_SECTION_WIDTH-1)*PI/LEAVES_RETURN_SECTION_WIDTH)) {

		angle+=PI/LEAVES_ANGLE_STEP;

		delta = LEAVES_DELTA_MULTIPLIER*((float)rand()-(RAND_MAX/2))/RAND_MAX;

		distance += delta;

		if( distance > LEAVES_MAX_RADIUS/origin_distance)
			distance = LEAVES_MAX_RADIUS/origin_distance;
	
		if( distance < LEAVES_MIN_RADIUS/origin_distance ) 
			distance = LEAVES_MIN_RADIUS/origin_distance;

		glBegin(GL_LINES);
			glVertex2f(startX, startY);
			glVertex2f(startX+distance*cos(angle), startY+distance*sin(angle));
		glEnd();
		
		
	}

	delta =  (start_distance-distance)/((PI/LEAVES_RETURN_SECTION_WIDTH)/(PI/LEAVES_ANGLE_STEP));

	while (angle < 2*PI) {

		angle+=PI/LEAVES_ANGLE_STEP;

		distance += delta;

		glBegin(GL_LINES);
			glVertex2f(startX, startY);
			glVertex2f(startX+distance*cos(angle), startY+distance*sin(angle));
		glEnd();
		
	}
	
	free(pixel_data);
}

int main(int argc, char** argv) {
	
	double angle, distance, newX, newY; 
	double oldX, oldY, oldAngle, startAngle;
	int divisions;
	uint8_t* pixel_data;
	
	SDL_init();
	glColor3ub(255,255,255);
	
	srand(time(NULL));

	pixel_data = (uint8_t*)calloc(SCREEN_WIDTH*SCREEN_HEIGHT, sizeof(uint8_t));
	
	for(int i = 0; i < BRANCHES; i++) {
	
		oldX = oldY = 0;
		startAngle = oldAngle = 2*PI/BRANCHES*i;	
		
		divisions = 3 + rand() % 2;
		
		for(int j = 0; j < divisions; j++) {
	
			for(int k = 0; k < 2; k++) {
				
				if(j==0)
					k++;
				
				if( k==0 ) {
					angle = PI/3*rand()/RAND_MAX-PI/6;
					distance = 0.07*rand()/RAND_MAX+0.07;
				} else {
					angle = -1*(angle+PI/10*rand()/RAND_MAX);
						
					distance = distance+(0.04*rand()/RAND_MAX+0.04);
				}
				
	
				newX = distance*(cos(oldAngle)*cos(angle)+sin(oldAngle)*sin(angle))+oldX;
				newY = distance*(-sin(oldAngle)*cos(angle)+cos(oldAngle)*sin(angle))+oldY;
				
				glBegin(GL_LINES);
					glVertex2f(oldX, oldY);
					glVertex2f(newX, newY);
				glEnd();
				
				
				if(k==0) {
					draw_leaves(newX, newY);
					draw_leaves(oldX, oldY);
				} else if(j+1 == divisions)
					draw_leaves(newX, newY);
				
				
			}
			oldX = newX;
			oldY = newY;
	
			
			oldAngle = asin(oldY) / acos(oldX) + startAngle;
			if(oldAngle < 0)
				oldAngle = 2*PI - oldAngle;
			
		}
	}
	
	glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RED, GL_UNSIGNED_BYTE, pixel_data);
	save_to_ppm(pixel_data, "test", 1);
	
	free(pixel_data);
	
	
	SDL_clear_color_buffer();
	
	SDL_shut_down();
}
