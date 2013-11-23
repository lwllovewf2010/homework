import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringTokenizer;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.DoubleWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.NullWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.Writable;
import org.apache.hama.HamaConfiguration;
import org.apache.hama.bsp.HashPartitioner;
import org.apache.hama.bsp.TextOutputFormat;
import org.apache.hama.graph.AverageAggregator;
import org.apache.hama.graph.Edge;
import org.apache.hama.graph.GraphJob;
import org.apache.hama.graph.Vertex;
import org.apache.hama.graph.VertexInputReader;


public class MerckPageRank {

	public static class MerckPageRankVertex extends
			Vertex<Text, NullWritable, Writable> {

    static double DF = 0.85;
    static double MAX_CONV_ERR = 0.001;

		@Override
		public void setup( Configuration conf ) {

			String val = conf.get( "hama.pagerank.alpha" );
			if( val != null ) {

				DF = Double.parseDouble( val );
			}

			val = conf.get( "hama.graph.max.convergence.error" );
			if( val != null ) {

				MAX_CONV_ERR = Double.parseDouble( val );
			}
		}

		@Override
		public void compute( Iterable<Writable> messages )
			throws IOException {

			if( this.getSuperstepCount() == 0 ) {

				this.setValue(
					new DoubleWritable(
						1.0 / this.getNumVertices() ) );
			}

			if( this.getSuperstepCount() > 54 ) {

				voteToHalt();
				return;
			}

			List<Text> ids;

			int tmp = (int)(this.getSuperstepCount() % 3);
			switch( tmp ) {

			case 0:
				double sum = 0;

				if( this.getSuperstepCount() != 0 ) {

					for( Writable msg : messages ) {

						sum += Double.parseDouble( msg.toString() );
					}

					if( sum != 0 ) {

						this.setValue(
							new DoubleWritable(
								(1 - DF) + sum)
							);
					}
				}

				sendMessageToNeighbors( this.getVertexID() );
				break;

			case 1:
				int inEdgeCounts = 0;
				ids = new ArrayList<Text>();

				for( Writable msg : messages ) {

					inEdgeCounts++;
					ids.add((Text) msg);
				}

				for( Text id : ids ) {

					this.sendMessage(id, new Text(this.getVertexID() + ":"
							+ inEdgeCounts + ":" + this.getEdges().size()));
				}
				break;

			case 2:
				Map<String, Double> inMap
					= new HashMap<String, Double>();

				Map<String, Double> inOut
					= new HashMap<String, Double>();

				double iInSum = 0;
				double iOutSum = 0;
				String id;
				double iIn;
				double iOut;

				ids = new ArrayList<Text>();
				for (Writable msg : messages) {

					String[] value = ((Text) msg).toString().split(":");
					id = value[0];
					iIn = Double.parseDouble(value[1]);
					iOut = Double.parseDouble(value[2]);
					iInSum += iIn;
					iOutSum += iOut;
					inMap.put(id, iIn);
					inOut.put(id, iOut);
					ids.add(new Text(id));
				}

				double distrbute;
				double pr = Double.parseDouble(((DoubleWritable) this
						.getValue()).toString());
				
				Text targetid;
				for (Edge<Text, NullWritable>  edge: this.getEdges()) {

					targetid = edge.getDestinationVertexID();
					
					distrbute = DF
							* (inMap.get(targetid.toString()) / iInSum)
							* (inOut.get(targetid.toString()) / iOutSum) * pr;
					this.sendMessage(targetid, new Text(Double.toString(distrbute)));
				}
				break;

			default:
				break;
			}
		}

	}

	public static class MerckPagerankSeqReader
			extends
			VertexInputReader<LongWritable, Text, Text, NullWritable, DoubleWritable> {

		@Override
		public boolean parseVertex(LongWritable key, Text value,
				Vertex<Text, NullWritable, DoubleWritable> vertex)
				throws Exception {

			String[] split = value.toString().split( "\t" );
			vertex.setVertexID( new Text( split[ 0 ] ) );

			StringTokenizer tokenizer = new StringTokenizer( split[ 1 ] );
			while( tokenizer.hasMoreTokens() ) {

				vertex.addEdge( new Edge<Text, NullWritable>( new Text( tokenizer
						.nextToken() ), null ) );
			}

			return true;
		}
	}

	public static GraphJob createJob(String[] args, HamaConfiguration conf)
			throws IOException {

		GraphJob pageJob = new GraphJob( conf, MerckPageRank.class );
		pageJob.setJobName( "WeightedPageRank" );

		pageJob.setVertexClass( MerckPageRankVertex.class );
		pageJob.setInputPath( new Path(args[0]) );
		pageJob.setOutputPath( new Path(args[1]) );
		pageJob.setMaxIteration( 30 );

		pageJob.set( "hama.pagerank.alpha", "0.85" );
		pageJob.set( "hama.graph.max.convergence.error", "0.001" );

		if( args.length == 3 ) {

			pageJob.setNumBspTask(
				Integer.parseInt( args[ 2 ] ) );
		}

		pageJob.setAggregatorClass( AverageAggregator.class );
		pageJob.setVertexInputReaderClass( MerckPagerankSeqReader.class );
		pageJob.setVertexIDClass( Text.class );
		pageJob.setVertexValueClass( Text.class );
		pageJob.setEdgeValueClass( NullWritable.class );
		pageJob.setInputKeyClass( LongWritable.class );
		pageJob.setInputValueClass( Text.class );
		pageJob.setPartitioner( HashPartitioner.class );
		pageJob.setOutputFormat( TextOutputFormat.class );
		pageJob.setOutputKeyClass( Text.class );
		pageJob.setOutputValueClass( DoubleWritable.class );

		return pageJob;
	}

	public static void main(String[] args) throws IOException,
			InterruptedException, ClassNotFoundException {

		if( args.length < 2 ) {

			System.out.println( "Usage: <input> <output> [tasks]\n" );
			System.exit( -1 );
			return;
		}

		HamaConfiguration conf
			= new HamaConfiguration(
					new Configuration() );

		GraphJob pageJob
			= createJob( args, conf );
	}
}


