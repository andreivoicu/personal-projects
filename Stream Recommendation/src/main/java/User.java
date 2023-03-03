import java.util.ArrayList;
import java.util.List;

public class User extends Person{
    private Integer ID;
    private String name;
    private List<Integer> streams = new ArrayList<Integer>();

    public User(Integer ID, String name) {
        this.ID = ID;
        this.name = name;
    }

    public void addStream(Integer streamID) {
        this.streams.add(streamID);
    }

    public boolean checkStream(Integer streamID) {
        return this.streams.contains(streamID);
    }

    public String toString () {
        StringBuilder retValue = new StringBuilder();
        retValue.append("\"").append(this.ID).append("\",\"").append(this.name).append("\",\"");

        for (Integer stream : this.streams) {
            retValue.append(stream).append(" ");
        }

        retValue = new StringBuilder(retValue.substring(0, retValue.length() - 1) + "\"");

        return retValue.toString();
    }

}
