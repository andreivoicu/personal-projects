import java.util.*;
import java.util.concurrent.TimeUnit;

public class Utils {
    public Utils () {
    }

    public static Integer getID (String line) {
        StringTokenizer st = new StringTokenizer(line, " ");

        return Integer.parseInt(st.nextToken());
    }

    public static String getCommand(String line) {
        StringTokenizer st = new StringTokenizer(line, " ");
        st.nextToken();
        return st.nextToken();
    }

    public static String calculateTime (Long seconds) {
        long hours = seconds / 3600;
        long minutes = (seconds % 3600) / 60;
        seconds = seconds % 60;

        String hoursString = "";
        String minutesString = "";
        String secondsString = "";

        if (hours < 10) {
            hoursString = "0" + Long.toString(hours);
        } else {
            hoursString = Long.toString(hours);
        }

        if (minutes < 10) {
            minutesString = "0" + Long.toString(minutes);
        } else {
            minutesString = Long.toString(minutes);
        }

        if (seconds < 10) {
            secondsString = "0" + seconds.toString();
        } else {
            secondsString = seconds.toString();
        }

        String retString = "";

        if (hours != 0) {
            retString = hoursString + ":" + minutesString + ":" + secondsString;
        } else {
            retString = minutesString + ":" + secondsString;
        }

        return retString;
    }

    public static String jsonalizeStreamList (List<Stream> streams) {
        TXTOps txtOps = new TXTOps();

        StringBuilder json = new StringBuilder("");
        json.append("[");
        for (Stream stream : streams) {
            json.append(txtOps.streamToJSON(stream)).append(",");
            txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", stream.getName() + "\n");
        }
        json.deleteCharAt(json.length() - 1).append("]");

        return json.toString();
    }
}
