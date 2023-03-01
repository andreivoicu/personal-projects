public class Streamer extends Person{
    private Integer ID;
    private String name;
    private Integer streamerType;

    public Streamer(Integer ID, String name) {
        this.ID = ID;
        this.name = name;
    }

    public void setStreamerType(Integer streamerType) {
        this.streamerType = streamerType;
    }
}
