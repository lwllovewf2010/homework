package org.myorg;

import java.io.IOException;
import java.util.*;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;


public class MerckSort {

    public static class Map extends MapReduceBase
		implements Mapper<LongWritable, Text, IntWritable, Text> {

      private Text word = new Text();

      public void map( LongWritable key, Text value,
			OutputCollector<IntWritable, Text> output, Reporter reporter )
				throws IOException {

          StringTokenizer tokenizer =
			  new StringTokenizer( value.toString() );

		  IntWritable val = new IntWritable( Integer.parseInt( value.toString() ));

          word.set( value.toString() );
          output.collect( val, word );
      }
    }

    public static class Reduce extends MapReduceBase
		implements Reducer<IntWritable, Text, IntWritable, Text> {

      public void reduce( IntWritable key,
		Iterator<Text> values,
		OutputCollector<IntWritable, Text> output, Reporter reporter )
			throws IOException {

        output.collect( key, values.next() );
      }
    }

    public static void main( String[] args )
		throws Exception {

      JobConf conf = new JobConf( MerckSort.class );
      conf.setJobName( "mercksort" );

      conf.setOutputKeyClass( IntWritable.class );
      conf.setOutputValueClass( Text.class );

      conf.setMapperClass( Map.class );
      conf.setCombinerClass( Reduce.class );
      conf.setReducerClass( Reduce.class );

      conf.setInputFormat( TextInputFormat.class );
      conf.setOutputFormat( TextOutputFormat.class );

      FileInputFormat.setInputPaths( conf, new Path( args[ 0 ] ) );
      FileOutputFormat.setOutputPath( conf, new Path( args[ 1 ] ) );

      JobClient.runJob( conf );
    }
}

