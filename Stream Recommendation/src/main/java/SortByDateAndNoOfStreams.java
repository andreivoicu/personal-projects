import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Comparator;
import java.util.Date;

public class SortByDateAndNoOfStreams implements Comparator<Stream> {
    public int compare(Stream a, Stream b) {
        DateFormat format = new SimpleDateFormat("dd/MM/yyyy");
        Date dateA = null;
        Date dateB = null;
        try {
            dateA = new SimpleDateFormat("dd/MM/yyyy").parse(format.format(a.getDateAdded()));
            dateB = new SimpleDateFormat("dd/MM/yyyy").parse(format.format(b.getDateAdded()));
        } catch (ParseException e) {
            throw new RuntimeException(e);
        }

        if (dateA.equals(dateB)) {
            return (int) (b.getNoOfStreams() - a.getNoOfStreams()); // descending order
        } else {
            if (dateA.after(dateB)) {
                return -1;
            } else {
                return 1;
            }
        }
    }
}
