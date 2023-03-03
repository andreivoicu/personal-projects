import java.time.Instant;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

public class CommandFacade {
    private static CommandFacade instance;
    private CommandFacade() {
    }

    public static CommandFacade getInstance() {
        if (instance == null) {
            instance = new CommandFacade();
        }
        return instance;
    }


    public void add(Integer ID, String role, String line) {
        if (!role.equals("streamer")) {
            return;
        }

        String testFile = "./src/main/resources/inputs1/testFile.txt";
        TXTOps txtOps = TXTOps.getInstance();
        CSVOps csvOps = CSVOps.getInstance();

        String[] data = line.split(" ");
        Integer streamType = Integer.valueOf(data[2]);
        Integer streamID = Integer.valueOf(data[3]);
        Integer streamGenre = Integer.valueOf(data[4]);
        Long length = Long.valueOf(data[5]);

        StringBuilder nameBuilder = new StringBuilder();
        for (int i = 6; i < data.length; i++) {
            nameBuilder.append(" ").append(data[i]);
        }

        String streamName = nameBuilder.toString().substring(1);

        Long time = Instant.now().getEpochSecond();

        Stream currentStream = new Stream(streamType, streamID, streamGenre,
                ID, 0L, length, time, streamName.toString());

        if (!csvOps.songExists(streamName))
            txtOps.appendToFile(ProiectPOO.streamsUtil, currentStream.toString());
    }

    public void listStreamer(Integer ID, String role, String line) {
        if (!role.equals("streamer")) {
            return;
        }
        CSVOps csvOps = CSVOps.getInstance();
        TXTOps txtOps = TXTOps.getInstance();

        List<Stream> streams = csvOps.getStreams(ID, true);
        Collections.reverse(streams);

        String json = Utils.jsonalizeStreamList(streams);
        txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", json.toString() + "\n");
        System.out.println(json.toString());
    }

    public void listUser(Integer ID, String role, String line) {
        if (!role.equals("user")) {
            return;
        }

        CSVOps csvOps = CSVOps.getInstance();
        TXTOps txtOps = TXTOps.getInstance();

        List<Integer> streamIDs = csvOps.getStreamIDsFromUser(ID);
        List<Stream> streams = new ArrayList<Stream>();
        for (Integer streamID : streamIDs) {
            streams.add(csvOps.getStreams(streamID, false).get(0));
        }

        String json = Utils.jsonalizeStreamList(streams);
        txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", json + "\n");
        System.out.println(json);
    }

    public void delete(Integer ID, String role, String line) {
        if (!role.equals("streamer")) {
            return;
        }

        TXTOps txtOps = TXTOps.getInstance();

        String streamsCSV = "./src/main/resources/inputs1/streams.csv";
        CSVOps csvOps = CSVOps.getInstance();

        Integer streamID = Integer.valueOf(line.split(" ")[2]);
//        txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", "StreamID:" + streamID.toString() + "\n");
        csvOps.deleteStream(streamID, ID);
    }

    public void listen(Integer ID, String role, String line) {
        if (!role.equals("user")) {
            return;
        }

        TXTOps txtOps = TXTOps.getInstance();
        CSVOps csvOps = CSVOps.getInstance();

        Integer streamID = Integer.valueOf(line.split(" ")[2]);
//        txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", "StreamID:" + streamID.toString() + "\n");

        User currentUser = csvOps.getUserByID(ID);
        Stream currentStream = csvOps.getStreamByID(streamID);

        if (currentUser == null || currentStream == null) {
            return;
        }

        String initialUser = currentUser.toString();
        String initialStream = currentStream.toString();

        boolean streamExists = currentUser.checkStream(streamID);
        if (!streamExists) {
            currentUser.addStream(streamID);
        }
        currentStream.listenStream();

        String updatedUser = currentUser.toString();
        String updatedStream = currentStream.toString();
        if (!streamExists) {
            csvOps.updateFile(ProiectPOO.usersUtil, initialUser, updatedUser);
        }
        csvOps.updateFile(ProiectPOO.streamsUtil, initialStream, updatedStream);
    }

    public void recommend(Integer ID, String role, String line) {
        if (!role.equals("user")) {
            return;
        }

        TXTOps txtOps = TXTOps.getInstance();
        CSVOps csvOps = CSVOps.getInstance();

        String type = line.split(" ")[2];
        int streamType = -1;

        if (type.equals("SONG")) {
            streamType = 1;
        } else if (type.equals("PODCAST")) {
            streamType = 2;
        } else if (type.equals("AUDIOBOOK")) {
            streamType = 3;
        }

        // fac o lista cu streamurile de tipul respectiv si o lista cu streamerii
        List<Integer> streamIDs = csvOps.getStreamIDsFromUser(ID);

        // makes sure only correct streams are in the list
        streamIDs = csvOps.updateStreamIDsByType(streamIDs, streamType);

        List<Stream> listenedStreams = new ArrayList<Stream>();
        List<Integer> listenedStreamersIDs = new ArrayList<Integer>();

        for (Integer streamID : streamIDs) {
            listenedStreams.add(csvOps.getStreamByID(streamID));
        }

        StreamRepository streamRepo = new StreamRepository(listenedStreams);
        for (Iterator iter = streamRepo.getIterator(); iter.hasNext();) {
            Stream stream = (Stream) iter.next();
            if (!listenedStreamersIDs.contains(stream.getStreamerID())) {
                listenedStreamersIDs.add(stream.getStreamerID());
            }
        }

        // fac o lista cu streamurile neascultate de la streamerii preferati (pe care i a ascultat)

        List<Stream> recommendedStreams = csvOps.getUnlistenedStreams(streamType, listenedStreamersIDs, streamIDs);

        // sortez lista cum trebuie si iau primele 5
        recommendedStreams.sort(new SortByNoOfStreams());

        List <Stream> finalRecommendedStreams = new ArrayList<Stream>();
        streamRepo = new StreamRepository(recommendedStreams);
        int counter = 0;

        for (Iterator iter = streamRepo.getIterator(); iter.hasNext() && counter < 5; counter++) {
            finalRecommendedStreams.add((Stream) iter.next());
        }

        String json = Utils.jsonalizeStreamList(finalRecommendedStreams);
        txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", json + "\n");
        System.out.println(json);
    }

    public void surprise(Integer ID, String role, String line) {
        if (!role.equals("user")) {
            return;
        }

        TXTOps txtOps = TXTOps.getInstance();
        CSVOps csvOps = CSVOps.getInstance();

        String type = line.split(" ")[2];
        int streamType = -1;

        if (type.equals("SONG")) {
            streamType = 1;
        } else if (type.equals("PODCAST")) {
            streamType = 2;
        } else if (type.equals("AUDIOBOOK")) {
            streamType = 3;
        }

        // fac o lista cu streamurile de tipul respectiv si o lista cu streamerii
        List<Integer> streamIDs = csvOps.getStreamIDsFromUser(ID);

        // makes sure only correct streams are in the list
        streamIDs = csvOps.updateStreamIDsByType(streamIDs, streamType);

        List<Stream> listenedStreams = new ArrayList<Stream>();
        List<Integer> listenedStreamersIDs = new ArrayList<Integer>();

        for (Integer streamID : streamIDs) {
            listenedStreams.add(csvOps.getStreamByID(streamID));
        }

        StreamRepository streamRepo = new StreamRepository(listenedStreams);
        for (Iterator iter = streamRepo.getIterator(); iter.hasNext();) {
            Stream stream = (Stream) iter.next();
                if (!listenedStreamersIDs.contains(stream.getStreamerID())) {
                    listenedStreamersIDs.add(stream.getStreamerID());
            }
        }

        List<Integer> unlistenedStreamersIDs = csvOps.getUnlistenedStreamers(listenedStreamersIDs, streamType);
        List<Integer> dummyList = new ArrayList<Integer>();
        List<Stream> unlistenedStreams = csvOps.getUnlistenedStreams(streamType, unlistenedStreamersIDs, dummyList);

        unlistenedStreams.sort(new SortByDateAndNoOfStreams());
        List<Stream> finalUnlistenedStreams = new ArrayList<Stream>();
        streamRepo = new StreamRepository(unlistenedStreams);
        int counter = 0;

        for (Iterator iter = streamRepo.getIterator(); iter.hasNext() && counter < 3; counter++) {
            finalUnlistenedStreams.add((Stream) iter.next());
        }

        String json = Utils.jsonalizeStreamList(finalUnlistenedStreams);
        txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", json + "\n");
        System.out.println(json);

        String[] data = json.split("}");
        for (String s : data) {
            txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", s + "\n");
        }
    }
}

