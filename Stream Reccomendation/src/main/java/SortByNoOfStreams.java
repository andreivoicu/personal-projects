import java.util.Comparator;

public class SortByNoOfStreams implements Comparator<Stream> {
    public int compare(Stream a, Stream b) {
        return (int) (b.getNoOfStreams() - a.getNoOfStreams()); // descending order
    }
}

