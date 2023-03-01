import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.*;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.time.Instant;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;
import java.util.*;

// singleton
public class TXTOps {
    private static TXTOps instance;
    public TXTOps() {

    }

    public static TXTOps getInstance() {
        if (instance == null) {
            instance = new TXTOps();
        }
        return instance;
    }

    public void copyFile (String from, String to) {
        BufferedReader reader;
        StringBuilder buffer = new StringBuilder();
        try {
            reader = new BufferedReader(new FileReader(from));
            String line = reader.readLine();
            while (line != null) {
                buffer.append(line).append("\n");

                line = reader.readLine();
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        String fileContents = buffer.toString();

        fileContents = fileContents.substring(0, fileContents.length() - 1);

        TXTOps txtOps = TXTOps.getInstance();
        txtOps.writeToFile(to, fileContents);
    }

    public void createFile (String fileName) {
        File file = new File(fileName);
        try
        {
            boolean created = file.createNewFile();
        }
        catch(IOException e){
            e.printStackTrace();
        }
    }

    public void writeToFile (String fileName, String text) {
        try {
            FileWriter myWriter = new FileWriter(fileName, false);
            myWriter.write(text);
            myWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void deleteFile (String fileName) {
        File file = new File(fileName);
        boolean deleted = file.delete();
    }

    public void appendToFile (String fileName, String text) {
        try {
            FileWriter myWriter = new FileWriter(fileName, true);
            myWriter.write(text);
            myWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void clearFile (String fileName) {
        try {
            FileWriter myWriter = new FileWriter(fileName, false);
            myWriter.write("");
            myWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public List<String> readCommandFile (String fileName) {
        List<String> commands = new ArrayList<String>();
        BufferedReader reader;
        try {
            reader = new BufferedReader(new FileReader(fileName));
            String line = reader.readLine();
            while (line != null) {
                commands.add(line);

                line = reader.readLine();
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return commands;
    }

    public String streamToJSON (Stream stream) {
        CSVOps csvOps = CSVOps.getInstance();

        ObjectMapper mapper = new ObjectMapper();
        String streamerName = csvOps.getStreamerNameByID(stream.getStreamerID());

        if (streamerName == null) {
            streamerName = "Unknown";
        }

        String time = Utils.calculateTime(stream.getLength());

        ZonedDateTime dateTime = Instant.ofEpochSecond(stream.getDateAdded()).atZone(ZoneId.of("GMT"));
        String formattedDate = dateTime.format(DateTimeFormatter.ofPattern("dd-MM-yyyy"));

        JSONFormatter formatter = new JSONFormatter(Integer.toString(stream.getID()), stream.getName(),
                streamerName, Long.toString(stream.getNoOfStreams()), time, formattedDate);


        String json = "";
        try {
            json = mapper.writeValueAsString(formatter);
        } catch (JsonProcessingException e) {
            throw new RuntimeException(e);
        }

        return json;
    }


}
