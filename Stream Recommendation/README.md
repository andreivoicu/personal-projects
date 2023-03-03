# Stream Recommendation System (Java)

The application stores information about streamers and users of a 
streaming application and has various functionalities. The application 
supports 3 types of streams: songs, podcasts and audiobooks.

Design Patterns used:
- **Singleton** - for CSVOps and TXTOps classes;
- **Factory** - for the instantiation of the "user" and "streamer" classes;
- **Iterator** - for iteration through "user" and "streamer" collections;
- **Facade** - adds a layer of abstraction to the system;

The data is stored in 3 different files and at the beginning of every run
of the program the data is copied into these files from the CSV input files.

The CommandFacade class contains a method for each command that can be used:
- **add** - a streamer adds a new stream;
- **list** - for a streamer, it returns a json with all the streams he uploaded,
and for a user it returns a json with all the streams he has listened to;
- **delete** -a streamer deletes a stream;
- **listen** - a user listens to a stream;
- **recommend** - a user gets a list of recommended streams using the next rule:
from the list of listened streamers by the user will be chosen the 5 most popular 
streams the user has not listened to yet;
- **suprise** - a user gets a list of recommended streams using the next rule: from
the list of non-listened streamers by the user there will be chosen the 3 most recent
added streams.

Commands format:
- **add** - <streamerId: Integer> ADD <streamType: Integer> <id: Integer> <streamGenre: Integer> <length: Long> <name: String>
- **list** - <streamerId: Integer> LIST / <userId: Integer> LIST
- **delete** - <streamerId: Integer> DELETE <streamId: Integer>
- **listen** - <userId: Integer> LISTEN <streamId: Integer>
- **recommend** - <userId: Integer>
  RECOMMEND [SONG | PODCAST | AUDIOBOOK]
- **suprise** - <userId: Integer>
  SURPRISE [SONG | PODCAST | AUDIOBOOK]
