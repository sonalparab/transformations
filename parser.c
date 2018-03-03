#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  color c;
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';

    printf(":%s:\n",line);
    
    if(strcmp(line,"line") == 0){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n",line);
      int i = 0;
      double args[6];
      char *nums = line;
      while(i < 6){
	args[i] = atof(strsep(&nums, " "));
	i++;
      }
      add_edge(edges,args[0],args[1],args[2],args[3],args[4],args[5]);
    }

    else if(strcmp(line,"ident") == 0){
      ident(transform);
    }

    else if(strcmp(line,"scale") == 0){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n",line);
      int i = 0;
      double args[3];
      char *nums = line;
      while(i < 3){
	args[i] = atof(strsep(&nums, " "));
	i++;
      }
      struct matrix *s = make_scale(args[0],args[1],args[2]);
      matrix_mult(s,transform);
    }

    else if(strcmp(line,"move") == 0){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n",line);
      int i = 0;
      double args[3];
      char *nums = line;
      while(i < 3){
	args[i] = atof(strsep(&nums, " "));
	i++;
      }
      struct matrix *t = make_translate(args[0],args[1],args[2]);
      matrix_mult(t,transform);
    }

    else if(strcmp(line,"rotate") == 0){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n",line);
      char *nums = line;
      char *axis = strsep(&nums, " ");
      double theta = atof(strsep(&nums, " "));
      theta = theta * M_PI / 180;

      struct matrix *r;
      if(strcmp(axis,"x") == 0)
	r = make_rotX(theta);
      else if(strcmp(axis,"y") == 0)
	r = make_rotY(theta);
      else if(strcmp(axis,"z") == 0)
	r = make_rotZ(theta);
      
      matrix_mult(r,transform);
    }

    else if(strcmp(line,"apply") == 0){
      matrix_mult(transform,edges);
    }
    
    else if(strcmp(line,"display") == 0){
      clear_screen(s);
      c.red = 0;
      c.green = 255;
      c.blue = 255;
      draw_lines(edges, s, c);
      display(s);
    }

    else if(strcmp(line,"save") == 0){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      printf(":%s:\n",line);
      save_extension(s, line);
    }

    else if(strcmp(line,"quit") == 0){
      exit(0);
    }

    else{
      printf("Not a valid command\n");
    }
  }
}
  
