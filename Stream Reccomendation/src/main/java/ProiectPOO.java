import java.util.List;

public class ProiectPOO {

    public static String streamersUtil = "./src/main/java/streamersUtil.csv";
    public static String streamsUtil = "./src/main/java/streamsUtil.csv";
    public static String usersUtil = "./src/main/java/usersUtil.csv";

    public static void main(String[] args) {
        TXTOps txtOps = TXTOps.getInstance();
        CSVOps csvOps = CSVOps.getInstance();

        String testFile = "./src/main/resources/inputs1/testFile.txt";
        txtOps.clearFile(testFile);
        if(args == null) {
            System.out.println("Nothing to read here");
            return;
        }

        String streamersCSV = "./src/main/resources/" + args[0];
        String streamsCSV = "./src/main/resources/" + args[1];
        String usersCSV = "./src/main/resources/" + args[2];
        String commandsTXT = "./src/main/resources/" + args[3];

        txtOps.clearFile(streamersUtil);
        txtOps.clearFile(streamsUtil);
        txtOps.clearFile(usersUtil);

        txtOps.copyFile(streamersCSV, streamersUtil);
        txtOps.copyFile(streamsCSV, streamsUtil);
        txtOps.copyFile(usersCSV, usersUtil);

        List<String> lines = txtOps.readCommandFile(commandsTXT);

        for (String line : lines) {
            Integer ID = Utils.getID(line);
            String role = csvOps.findByID(ID);
            String command = Utils.getCommand(line);

            if (role == null) {
                return;
                // TODO: throw exception
            }
            CommandFacade commandFacade = CommandFacade.getInstance();

            if (command.equals("ADD")) {
                commandFacade.add(ID, role, line);
            } else if (command.equals("LIST")){
                if (role.equals("streamer")) {
                    commandFacade.listStreamer(ID, role, line);
                } else if (role.equals("user")) {
                    commandFacade.listUser(ID, role, line);
                }
            } else if (command.equals("DELETE")) {
                commandFacade.delete(ID, role, line);
            } else if (command.equals("LISTEN")) {
                commandFacade.listen(ID, role, line);
            } else if (command.equals("RECOMMEND")) {
                commandFacade.recommend(ID, role, line);
            } else if (command.equals("SURPRISE")) {
                commandFacade.surprise(ID, role, line);
            }
        }
    }
}

/*
    Design patterns used:
    - Facade
    - Factory
    - Singleton
    - Iterator
 */