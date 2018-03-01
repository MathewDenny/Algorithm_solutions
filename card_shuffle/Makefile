
OBJS =		calculate_round.o

TARGET =	calculate_round

$(TARGET):	$(OBJS)
	gcc $(OBJS) -o $(TARGET)
	
$(OBJS): calculate_round.c
	gcc -c -g calculate_round.c -o $(OBJS)
	
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
