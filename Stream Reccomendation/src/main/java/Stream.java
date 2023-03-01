public class Stream {
    private Integer streamType;
    private Integer ID;
    private Integer streamGenre;
    private Integer streamerID;

    private Long noOfStreams;
    private Long length;
    private Long dateAdded;

    private String name;

    public Stream(Integer streamType, Integer ID, Integer streamGenre, Integer streamerID,
                   Long noOfStreams, Long length, Long dateAdded, String name) {
        this.streamType = streamType;
        this.ID = ID;
        this.streamGenre = streamGenre;
        this.streamerID = streamerID;
        this.noOfStreams = noOfStreams;
        this.length = length;
        this.dateAdded = dateAdded;
        this.name = name;
    }

    public String toString() {
        return "\n\"" + this.streamType + "\",\"" + this.ID + "\",\"" + this.streamGenre + "\",\""
                + this.noOfStreams + "\",\"" + this.streamerID + "\",\"" + this.length + "\",\""
                + this.dateAdded + "\",\"" + this.name + "\"";
    }

    public static Stream toObject(String line) {
        line = line.replaceAll("\n", "");
        String[] tokens = line.split(",");

        Integer streamType = Integer.parseInt(tokens[0].substring(1, tokens[0].length() - 1));
        Integer ID = Integer.parseInt(tokens[1].substring(1, tokens[1].length() - 1));
        Integer streamGenre = Integer.parseInt(tokens[2].substring(1, tokens[2].length() - 1));
        Long noOfStreams = Long.parseLong(tokens[3].substring(1, tokens[3].length() - 1));
        Integer streamerID = Integer.parseInt(tokens[4].substring(1, tokens[4].length() - 1));
        Long length = Long.parseLong(tokens[5].substring(1, tokens[5].length() - 1));
        Long dateAdded = Long.parseLong(tokens[6].substring(1, tokens[6].length() - 1));

        StringBuilder name = new StringBuilder(tokens[7]);
        if (line.length() >= 8) {
            for (int i = 8; i < tokens.length; i++) {
                name.append(",").append(tokens[i]);
            }
        }
        name = new StringBuilder(name.substring(1, name.length() - 1));

        return new Stream(streamType, ID, streamGenre, streamerID, noOfStreams, length, dateAdded, name.toString());
    }

    public void listenStream() {
        this.noOfStreams++;
    }

    public Integer getStreamType() {
        return this.streamType;
    }

    public Integer getID() {
        return this.ID;
    }

    public Integer getStreamGenre() {
        return this.streamGenre;
    }

    public Long getNoOfStreams() {
        return this.noOfStreams;
    }

    public Integer getStreamerID() {
        return this.streamerID;
    }

    public long getLength() {
        return this.length;
    }

    public Long getDateAdded() {
        return this.dateAdded;
    }

    public String getName() {
        return this.name;
    }

    public void setStreamType(Integer streamType) {
        this.streamType = streamType;
    }

    public void setID(Integer ID) {
        this.ID = ID;
    }

    public void setStreamGenre(Integer streamGenre) {
        this.streamGenre = streamGenre;
    }

    public void setStreamerID(Integer streamerID) {
        this.streamerID = streamerID;
    }

    public void setNoOfStreams(Long noOfStreams) {
        this.noOfStreams = noOfStreams;
    }

    public void setLength(Long length) {
        this.length = length;
    }

    public void setDateAdded(Long dateAdded) {
        this.dateAdded = dateAdded;
    }

    public void setName(String name) {
        this.name = name;
    }
}
