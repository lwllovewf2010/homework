import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


public class test {

	byte input[];
	private InputStream is;
	int temp;


	test( String filename ) {

		System.out.println( "test init\n" );
		try {

			System.out.println( "Open IS\n" );
			is = new FileInputStream( filename );
		}
		catch( FileNotFoundException e ) {

			System.out.println( "File not found\n" );
			return;
		}
	}


	public boolean readGraphics(
		int dataRows,
		int dataCols,
		int major ) throws IOException {

		int i;

		System.out.println( "allocate memory\n" );
		input = new byte[ 4 * (dataRows * dataCols) ];

		System.out.println( "sizeof(float) = " + sizeof( float ) + "\n" );

		i = 0;
		while( i < 3 ) {
			
			c = (char)is.read();
			if( c == '\n' )
				i = i + 1;
		}

		try {

			temp = is.read();
			System.out.println( "CHAR = " + temp + "\n" );
		}
		catch( IOException e ) {

			System.out.println( "IOException\n" );
			return false;
		}

		return true;
	}


	public static void main( String args[] ) {

		test p = new test( "image.pgm" );

		p.readGraphics( 502, 458, 1 );
		System.out.println( "Hello\n" );
	}
}


