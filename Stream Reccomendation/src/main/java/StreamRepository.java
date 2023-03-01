import java.util.List;

public class StreamRepository implements Container{
    List<Stream> streams;

    public StreamRepository(List<Stream> streams) {
        this.streams = streams;
    }

    @Override
    public Iterator getIterator() {
        return new StreamIterator();
    }

    private class StreamIterator implements Iterator {
        int index;

        @Override
        public boolean hasNext() {
            return index < streams.size();
        }

        @Override
        public Object next() {
            if (this.hasNext()) {
                return streams.get(index++);
            }
            return null;
        }
    }
}
