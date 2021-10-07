CC = gcc

CFLAGS = -pthread

TARGET = mini_project.c

output: 
	$(CC) $(CFLAGS) $(TARGET) -o output
	
clean:
	$(RM) output
