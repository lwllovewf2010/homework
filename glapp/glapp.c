#include <GLUT/glut.h>
#include <stdio.h>


void drawImage( void ) {

	int i;

	glClear( GL_COLOR_BUFFER_BIT );
	glColor3f( 0.54, 0.66, 0.88 );
	glLoadIdentity();
	glBegin( GL_POLYGON );

#if 0
	for( i = 0 ; i < 3 ; i++ ) {

		glVertex3f() ;
	}
#endif

	glEnd();
	glFlush();
}


int main( int argc, char **argv ) {

	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB );
	glutInitWindowSize( 300, 300 );
	glutInitWindowPosition( 50, 50 );
	glutCreateWindow( "Merck's GLUT Example" );

	glutDisplayFunc( drawImage );

	glutMainLoop();

	return 0;
}

