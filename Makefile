CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = game.o data.o items.o battles.o doors.o util.o progress.o commands.o
TARGET = game
all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)
clean:
	rm -f $(OBJFILES) $(TARGET) *~
