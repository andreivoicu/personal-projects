import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class CSVOps {
    private static CSVOps instance;
    private CSVOps() {
    }

    public static CSVOps getInstance() {
        if (instance == null) {
            instance = new CSVOps();
        }
        return instance;
    }

    public boolean searchCSV (String fileName, Integer ID) {
        TXTOps txtOps = TXTOps.getInstance();
        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(fileName));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                Integer currentID = -1;
                if (fileName.equals(ProiectPOO.streamersUtil)) {
                    currentID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                } else if (fileName.equals(ProiectPOO.usersUtil)) {
                    currentID = Integer.valueOf(data[0].substring(1, data[0].length() - 1));
                }

                if (currentID.equals(ID)) {
                    return true;
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return false;
    }

    public String findByID (Integer ID) {
        if (searchCSV(ProiectPOO.streamersUtil, ID)) {
            return "streamer";
        } else if (searchCSV(ProiectPOO.usersUtil, ID)) {
            return "user";
        }

        return null;
    }

    public boolean songExists (String songName) {
        TXTOps txtOps = TXTOps.getInstance();
        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.streamsUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");

                String currentSongName = data[7].substring(1, data[7].length() - 1);
                if (currentSongName.equals(songName)) {
                    return true;
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return false;
    }

    // streamerID = true -> search for streamerID
    // streamerID = false -> search for streamID
    public List<Stream> getStreams (Integer ID, boolean streamerID) {
        List<Stream> streams = new ArrayList<Stream>();

        TXTOps txtOps = TXTOps.getInstance();
        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.streamsUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                Integer currentID = -1;
                if (streamerID) {
                    currentID = Integer.valueOf(data[4].substring(1, data[4].length() - 1));
                } else {
                    currentID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                }

                if (currentID.equals(ID)) {
                    streams.add(Stream.toObject(row));
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return streams;
    }

    public List<Integer> getStreamIDsFromUser(Integer ID) {
        List<Integer> streamIDs = new ArrayList<Integer>();

        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.usersUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                Integer currentID = Integer.valueOf(data[0].substring(1, data[0].length() - 1));
                if (currentID.equals(ID)) {
                    String streamIDsString = data[2].substring(1, data[2].length() - 1);
                    String[] streamIDsArray = streamIDsString.split(" ");
                    for (String streamID : streamIDsArray) {
                        if (!streamID.equals("")) {
                            streamIDs.add(Integer.valueOf(streamID));
                        }
                    }
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return streamIDs;
    }

    public List<Integer> updateStreamIDsByType(List<Integer> initialList, int streamType) {
        List<Integer> streamIDs = new ArrayList<Integer>();

        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.streamsUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                Integer currentStreamType = Integer.valueOf(data[0].substring(1, data[0].length() - 1));
                Integer currentStreamID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                if (initialList.contains(currentStreamID) && currentStreamType.equals(streamType)) {
                    streamIDs.add(currentStreamID);
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return streamIDs;
    }

    public User getUserByID (Integer ID) {
        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.usersUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                Integer currentID = Integer.valueOf(data[0].substring(1, data[0].length() - 1));
                if (currentID.equals(ID)) {
                    String username = data[1].substring(1, data[1].length() - 1);

                    Factory factory = new Factory();
                    User user = (User) factory.createPerson("User", ID, username);

                    String streamIDs = data[2].substring(1, data[2].length() - 1);
                    String[] streamIDsArray = streamIDs.split(" ");
                    for (String streamID : streamIDsArray) {
                        if (!streamID.equals("")) {
                            user.addStream(Integer.valueOf(streamID));
                        }
                    }

                    return user;
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public String getStreamerNameByID (Integer ID) {
        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.streamersUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                Integer currentID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                String streamerName = data[2].substring(1, data[2].length() - 1);
                if (currentID.equals(ID)) {
                    return streamerName;
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public Stream getStreamByID (Integer ID) {
        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.streamsUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                Integer currentID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                if (currentID.equals(ID)) {
                    return Stream.toObject(row);
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public void deleteStream(Integer streamID, Integer streamerID) {
        TXTOps txtOps = TXTOps.getInstance();

        BufferedReader reader;
        StringBuilder buffer = new StringBuilder();
        try {
            reader = new BufferedReader(new FileReader(ProiectPOO.streamsUtil));
            String line = reader.readLine();
            buffer.append(line).append("\n");
            line = reader.readLine();

            while (line != null) {
                String[] data = line.split(",");
                Integer currentStreamID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                Integer currentStreamerID = Integer.valueOf(data[4].substring(1, data[4].length() - 1));
                if (!currentStreamID.equals(streamID) || !currentStreamerID.equals(streamerID)) {
                    buffer.append(line).append("\n");
                }

                line = reader.readLine();
            }
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        String fileContents = buffer.toString();
        fileContents = fileContents.substring(0, fileContents.length() - 1);

        txtOps.writeToFile(ProiectPOO.streamsUtil, fileContents);
//        txtOps.appendToFile("./src/main/resources/inputs1/testFile.txt", fileContents);
    }

    public void updateFile (String file, String oldLine, String newLine) {
        BufferedReader reader;
        StringBuilder buffer = new StringBuilder();
        try {
            reader = new BufferedReader(new FileReader(file));
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

        fileContents = fileContents.replace(oldLine, newLine);
        fileContents = fileContents.substring(0, fileContents.length() - 1);

        TXTOps txtOps = TXTOps.getInstance();
        txtOps.writeToFile(file, fileContents);
    }

    public List<Stream> getUnlistenedStreams (int streamType, List<Integer> listenedStreamersIDs,
                                              List<Integer> listenedStreamIDs) {
        List<Stream> unlistenedStreams = new ArrayList<Stream>();

        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.streamsUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                int currentStreamType = Integer.parseInt(data[0].substring(1, data[0].length() - 1));
                Integer currentID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                Integer currentStreamerID = Integer.valueOf(data[4].substring(1, data[4].length() - 1));

                if (!listenedStreamIDs.contains(currentID) && streamType == currentStreamType
                        && listenedStreamersIDs.contains(currentStreamerID)) {
                     unlistenedStreams.add(Stream.toObject(row));
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return unlistenedStreams;
    }


    public List<Integer> getUnlistenedStreamers(List<Integer> listenedStreamersIDs, int streamType) {
        List<Integer> unlistenedStreamers = new ArrayList<Integer>();

        try {
            BufferedReader csvReader = new BufferedReader(new FileReader(ProiectPOO.streamersUtil));
            String row = csvReader.readLine();
            while ((row = csvReader.readLine()) != null) {
                String[] data = row.split(",");
                int streamerType = Integer.parseInt(data[0].substring(1, data[0].length() - 1));
                Integer currentID = Integer.valueOf(data[1].substring(1, data[1].length() - 1));
                if (streamerType == streamType && !listenedStreamersIDs.contains(currentID)) {
                    unlistenedStreamers.add(currentID);
                }
            }
            csvReader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return unlistenedStreamers;
    }
}
