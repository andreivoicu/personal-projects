public class Factory {
    public Person createPerson(String type, Integer ID, String name) {
        if (type.equals("Streamer")) {
            return new Streamer(ID, name);
        } else if (type.equals("User")) {
            return new User(ID, name);
        } else {
            return null;
        }
    }
}
