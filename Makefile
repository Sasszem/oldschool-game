CC = gcc
CFLAGS = -Wall
LDFLAGS =
OBJFILES = game.o data.o items.o battles.o doors.o util.o progress.o commands.o save.o story.o
TARGET = game

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

editor:
	$(CC) $(CGLAG) -o save save.o encrypt.c

clean:
	rm -f $(OBJFILES) $(TARGET) save *~
